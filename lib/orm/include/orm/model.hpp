#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <type_traits>
#include <pqxx/pqxx>

#include "column.hpp"

namespace model {
	class Model {
	public:
		Model(const Model& other): tableName(other.tableName), columns(other.columns) {}
		Model(Model&& other) noexcept = default;
		explicit Model(std::string tableName): tableName(std::move(tableName)) {}
		virtual ~Model() = default;
		[[nodiscard]] std::string getTableName() const;
		[[nodiscard]] const std::vector<std::shared_ptr<column::ColumnBase>>& getColumns() const;
		[[nodiscard]] const column::ColumnBase* getPrimaryKey() const;
		void setColumnsValues(const pqxx::row& row) const;
		Model& operator=(const Model& other);
		Model& operator=(Model&& other) noexcept = default;
	protected:
		template<typename T>
		void registerStaticColumn(std::shared_ptr<T>& property);
		std::string tableName;
	private:
		std::vector<std::shared_ptr<column::ColumnBase>> columns;
		std::shared_ptr<column::ColumnBase> primaryKey;
	};
}

#include "model.tpp"

#endif //MODEL_HPP
