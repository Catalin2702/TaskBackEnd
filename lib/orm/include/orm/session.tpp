#pragma once

namespace session {

	template<typename M>
	Query<M> Session::query(const M& model) {
		return Query<M>(model, this);
	}
	template<typename M>
	Query<M> Session::query(const std::vector<M>& models) {
		return Query<M>(models, this);
	}
	template <typename M>
	std::vector<M> Session::execute(const Query<M>& query) {
		std::vector<M> results;
		try {
			const auto queryResult = execute(query.toString());
			results.reserve(queryResult.size());
			for (const auto& row : queryResult) {
				M model;
				model.setColumnsValues(row);
				results.push_back(std::move(model));
			}
		} catch (const std::exception& e) {
			rollback();
			throw std::runtime_error(std::string("Query execution failed: ") + e.what());
		}
		return results;
	}

	template <typename M>
	Query<M>::Query(const Query& query):
		condition(query.condition), finalQuery(query.finalQuery), model(query.model), isBatch(query.isBatch), session(query.session) {}
	template <typename M>
	Query<M>::Query(const M& model, Session* session):
		model(model), isBatch(false), session(session) {}
	template <typename M>
	Query<M>::Query(const std::vector<M>& models, Session* session):
		isBatch(true), models(models), session(session) {
		if (models.empty())
			throw std::invalid_argument("Cannot create batch query with empty models");
		model = models.front();
	}

	template <typename M>
	Query<M> Query<M>::filter(const std::string& condition) const {
		Query query(model);
		query.condition = condition;
		return query;
	}
	template <typename M>
	template <typename T>
	Query<M> Query<M>::filter(condition::Condition<T> condition) const {
		Query query(model, session);
		query.condition = condition.toString();
		return query;
	}
	template <typename M>
	std::vector<M> Query<M>::all() {
		finalQuery = buildSelectQuery();
		return session->execute(*this);
	}
	template <typename M>
	M Query<M>::first() {
		finalQuery = buildSelectQuery(1);
		const auto results = session->execute(*this);
		if (results.empty())
			throw std::runtime_error("No results found");
		return results.front();
	}
	template <typename M>
	std::vector<M> Query<M>::limit(const int limit) {
		finalQuery = buildSelectQuery(limit);
		return session->execute(*this);
	}
	template<typename M>
	std::vector<M> Query<M>::insert() {
		if (isBatch)
			finalQuery = buildBatchInsertQuery();
		else
			finalQuery = buildSingleInsertQuery();
		return session->execute(*this);
	}
	template<typename M>
	M Query<M>::update(const std::vector<std::string>& columns) {
		finalQuery = buildUpdateQuery(columns);
		const auto results = session->execute(*this);
		if (results.empty())
			throw std::runtime_error("No results found");
		return results.front();
	}
	template <typename M>
	std::vector<int> Query<M>::remove() {
		finalQuery = buildDeleteQuery();
		const pqxx::result result = session->execute(finalQuery);
		std::vector<int> ids;
		ids.reserve(result.size());
		for (const auto& row : result) {
			ids.push_back(row[0].as<int>());
		}
		return ids;
	}

	template <typename M>
	std::string Query<M>::toString() const {
		return finalQuery;
	}
	template <typename M>
	Query<M>::operator std::string() const {
		return toString();
	}
	template<typename M>
	std::vector<std::shared_ptr<column::ColumnBase>> Query<M>::getColumns() const {
		return model.getColumns();
	}
	template<typename M>
	std::vector<std::shared_ptr<column::ColumnBase>> Query<M>::getDirtyColumns() const {
		return model.getDirtyColumns();
	}
	template<typename M>
	std::shared_ptr<column::ColumnBase> Query<M>::getPrimaryKey() {
		return model.getPrimaryKey();
	}
	template <typename M>
	std::vector<std::string> Query<M>::getColumnNames() const {
		std::vector<std::string> columns;
		const auto modelColumns = model.getColumns();
		columns.reserve(modelColumns.size());
		for (const auto& column : modelColumns)
			columns.push_back(column->getFullName());
		return columns;
	}
	template <typename M>
	std::string Query<M>::buildSelectQuery(const int limit) {
		std::ostringstream queryString;
		queryString << "SELECT " << tools::join(getColumnNames(), ", ") << std::endl;
		queryString << "FROM " << model.getTableName() << std::endl;
		if (not condition.empty())
			queryString << "WHERE " << condition << std::endl;
		if (limit > 0)
			queryString << "LIMIT " << limit << std::endl;
		queryString << ";" << std::endl;
		return queryString.str();
	}
	template<typename M>
	std::string Query<M>::buildSingleInsertQuery() const {
		const auto columns = getColumns();
		std::shared_ptr<column::ColumnBase> primaryKey;
		std::vector<std::string> columnNames;
		std::vector<std::string> columnValues;

		columnNames.reserve(columns.size());
		columnValues.reserve(columns.size());

		for (const auto& column : columns) {
			if (column->isPrimaryKey() && column->getType() == value::SqlType::Serial) {
				primaryKey = column->clone();
				continue;
			}
			columnNames.push_back(column->getName());
			columnValues.push_back(column->getValueAsString());
		}
		columnNames.shrink_to_fit();
		columnValues.shrink_to_fit();

		if (not primaryKey)
			throw std::runtime_error("No primary key found!");

		std::ostringstream query;
		query << "INSERT INTO " << model.getTableName()
			  << " (" << tools::join(columnNames, ", ")
			  << ")\nVALUES\n("
			  << tools::join(columnValues, ", ")
			  << ")\nRETURNING *;";

		return query.str();
	}
	template <typename M>
	std::string Query<M>::buildBatchInsertQuery() const {
		const auto columns = getColumns();
		std::vector<std::shared_ptr<column::ColumnBase>> insertColumns;
		std::vector<std::string> columnNames;
		std::shared_ptr<column::ColumnBase> primaryKey;

		insertColumns.reserve(columns.size());
		columnNames.reserve(columns.size());
		for (const auto& column : columns) {
			if (column->isPrimaryKey() && column->getType() == value::SqlType::Serial) {
				primaryKey = column->clone();
				continue;
			}
			insertColumns.push_back(column);
			columnNames.push_back(column->getName());
		}

		if (!primaryKey)
			throw std::runtime_error("No primary key found");

		std::ostringstream query;
		query << "INSERT INTO " << model.getTableName() << " (" << tools::join(columnNames, ", ") << ")\nVALUES\n";

		bool firstModel = true;
		for (const auto& currentModel : models) {
			if (!firstModel)
				query << ",\n";
			query << "(";
			const auto& currentColumns = currentModel.getColumns();
			std::vector<std::string> values;
			values.reserve(insertColumns.size());
			for (const auto& column : currentColumns) {
				if (column->isPrimaryKey() && column->getType() == value::SqlType::Serial) {
					continue;
				}
				values.push_back(column->getValueAsString());
			}
			query << tools::join(values, ", ");
			query << ")";
			firstModel = false;
		}

		query << "\nRETURNING *;";

		std::cout << "\n\n" << query.str() << "\n\n";

		return query.str();
	}
	template <typename M>
	std::string Query<M>::buildUpdateQuery(const std::vector<std::string>& columns) {
		const auto dirtyColumns = getDirtyColumns();
		if (dirtyColumns.empty()) {
			throw std::runtime_error("No columns to update");
		}

		const auto primaryKey = getPrimaryKey();
		std::cout << "Primary key: " << primaryKey->toString() << std::endl;
		if (not primaryKey) {
			throw std::runtime_error("No primary key found");
		}

		std::vector<std::shared_ptr<column::ColumnBase>> columnsToUpdate;
		if (columns.empty()) {
			columnsToUpdate = dirtyColumns;
		} else {
			for (const auto& column : dirtyColumns) {
				if (std::find(columns.begin(), columns.end(), column->getName()) != columns.end())
					columnsToUpdate.push_back(column);
			}
		}

		if (columnsToUpdate.empty()) {
			throw std::runtime_error("No columns to update after filtering");
		}

		std::vector<std::string> setStatements;
		setStatements.reserve(columnsToUpdate.size());

		for (const auto& column : columnsToUpdate) {
			if (column->isPrimaryKey()) {
				continue;
			}
			setStatements.push_back(column->getName() + " = " + column->getValueAsString());
		}

		std::ostringstream query;
		query << "UPDATE " << model.getTableName() << "\nSET\n"
			<< tools::join(setStatements, ",\n")
			<< "\nWHERE " << primaryKey->getName()
			<< " = " << primaryKey->getValueAsString() << "\n"
			<< "RETURNING *";

		return query.str();
	}
	template <typename M>
	std::string Query<M>::buildDeleteQuery() {
		std::ostringstream query;
		query << "DELETE FROM " << model.getTableName() << "\nWHERE ";
		const auto primaryKey = getPrimaryKey();
		if (condition.empty()) {
			if (not primaryKey) {
				throw std::runtime_error("No primary key found");
			}
			if (isBatch) {
				std::vector<std::string> ids;
				ids.reserve(models.size());
				for (auto& model : models) {
					ids.push_back(model.getPrimaryKey()->getValueAsString());
				}
				query << primaryKey->getName() << " IN (" << tools::join(ids, ", ") << ")";
			} else {
				query << primaryKey->getName() << " = " << primaryKey->getValueAsString();
			}
		} else {
			query << condition;
		}
		query << "\nRETURNING " << primaryKey->getName() << ";";

		std::cout << "\n\n" << query.str() << "\n\n";

		return query.str();
	}
}
