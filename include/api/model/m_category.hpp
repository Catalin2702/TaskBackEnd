//
// Created by Catalin Chirosca on 23/12/24.
//

#ifndef M_CATEGORY_HPP
#define M_CATEGORY_HPP

#include <orm/column.hpp>
#include <orm/model.hpp>

class Category final : public model::Model {
public:
	Category();
	Category(const std::string& title, const int userId, const std::string& description = "");

	column::ColumnRef<column::SerialColumn> id{std::make_shared<column::SerialColumn>("id", true)};
	column::ColumnRef<column::StringColumn> title{std::make_shared<column::StringColumn>("title", 100)};
	column::ColumnRef<column::TextColumn> description{std::make_shared<column::TextColumn>("description", false, true)};
	column::ColumnRef<column::IntegerColumn> userId{std::make_shared<column::IntegerColumn>("user_id")};
	column::ColumnRef<column::TimestampColumn> created{std::make_shared<column::TimestampColumn>("created")};
	column::ColumnRef<column::TimestampColumn> updated{std::make_shared<column::TimestampColumn>("updated")};

	[[nodiscard]] json toJson() const override;
};

#endif //M_CATEGORY_HPP
