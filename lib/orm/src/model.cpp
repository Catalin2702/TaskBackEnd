//
// Created by Catalin Chirosca on 10/12/24.
//

#include "orm/model.hpp"

namespace model {

	const std::vector<std::shared_ptr<column::ColumnBase>>& Model::getColumns() const {
		return columns;
	}
	const column::ColumnBase *Model::getPrimaryKey() const {
		return &*primaryKey;
	}
	void Model::setColumnsValues(const pqxx::row& row) const {
		auto& columns = getColumns();
		for (int i = 0; i < columns.size(); ++i) {
			columns[i]->setValueFromField(row[i]);
		}
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
