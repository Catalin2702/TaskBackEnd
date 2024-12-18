#pragma once
namespace model {

	template <typename T>
	void Model::registerStaticColumn(std::shared_ptr<T>& property) {
		static_assert(std::is_base_of_v<column::ColumnBase, T>, "Template must derive from ColumnBase!");
		if (property->isPrimaryKey()) {
			if (this->primaryKey)
				throw std::runtime_error(
					"Can't use " + property->getName() + " as primary key in table " + getTableName() + ". " +
					primaryKey->getName() + " has already been registered as a primary key."
				);
			this->primaryKey = property;
		}
		if (property->getTableName().empty()) property->setTableName(getTableName());
		columns.push_back(property);
	}

}
