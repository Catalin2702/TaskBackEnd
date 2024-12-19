//
// Created by Catalin Chirosca on 10/12/24.
//

#include "orm/model.hpp"

namespace model {

	const std::vector<std::shared_ptr<column::ColumnBase>>& Model::getColumns() const {
		return columns;
	}
	std::vector<std::shared_ptr<column::ColumnBase>> Model::getDirtyColumns() const {
		std::vector<std::shared_ptr<column::ColumnBase>> result;
		for (const auto& column : columns) {
			if (column->isDirty())
				result.push_back(column);
		}
		return result;
	}
	std::shared_ptr<column::ColumnBase> Model::getPrimaryKey() {
		return primaryKey;
	}
	void Model::setColumnsValues(const pqxx::row& row) {
		auto& columns = getColumns();
		for (int i = 0; i < columns.size(); ++i) {
			columns[i]->initValue(row[i]);
			if (columns[i]->isPrimaryKey()) {
				setPrimaryKey(columns[i]);
			}
		}
	}
	void Model::setPrimaryKey(const std::shared_ptr<column::ColumnBase>& primaryKey) {
		this->primaryKey = primaryKey;
	}

	Model& Model::operator=(const Model& other) {
		if (this != &other) {
			tableName = other.tableName;
			columns.clear();
			columns.reserve(other.columns.size());
			for (const auto& col : other.columns) {
				columns.push_back(col->clone());
			}
		}
		return *this;
	}
	std::string Model::getTableName() const { return tableName; }
}
