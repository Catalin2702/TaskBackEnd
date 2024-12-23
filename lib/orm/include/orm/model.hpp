#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
#include <type_traits>
#include <pqxx/pqxx>

#include "column.hpp"

using json = nlohmann::json;

namespace model {
	class Model {
	public:
		Model(const Model& other): tableName(other.tableName), columns(other.columns), primaryKey(other.primaryKey) {}
		Model(Model&& other) noexcept = default;
		explicit Model(std::string tableName): tableName(std::move(tableName)) {}
		virtual ~Model() = default;
		[[nodiscard]] std::string getTableName() const;
		[[nodiscard]] const std::vector<std::shared_ptr<column::ColumnBase>>& getColumns() const;
		[[nodiscard]] std::vector<std::shared_ptr<column::ColumnBase>> getDirtyColumns() const;
		[[nodiscard]] std::shared_ptr<column::ColumnBase> getPrimaryKey();
		void setColumnsValues(const pqxx::row& row);
		void setPrimaryKey(const std::shared_ptr<column::ColumnBase>& primaryKey);
		Model& operator=(const Model& other);
		Model& operator=(Model&& other) noexcept = default;
		[[nodiscard]] virtual json toJson() const = 0;
	protected:
		template<typename T>
		void registerEmptyColumn(std::shared_ptr<T>& property);
		template<typename T>
		void registerColumn(std::shared_ptr<T>& property, const typename T::value_type& value);
		std::string tableName;
	private:
		std::vector<std::shared_ptr<column::ColumnBase>> columns;
		std::shared_ptr<column::ColumnBase> primaryKey;
	};
}

#include "model.tpp"

#endif //MODEL_HPP
