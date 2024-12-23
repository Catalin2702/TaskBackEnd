//
// Created by Catalin Chirosca on 23/12/24.
//

#ifndef M_CATEGORIE_HPP
#define M_CATEGORIE_HPP

#include <orm/column.hpp>
#include <orm/model.hpp>

class Categorie final : public model::Model {
public:
	Categorie();
	Categorie(const std::string& title, const std::string& description, const int userId);

	column::ColumnRef<column::SerialColumn> id{std::make_shared<column::SerialColumn>("id", true)};
	column::ColumnRef<column::StringColumn> title{std::make_shared<column::StringColumn>("title", 100)};
	column::ColumnRef<column::TextColumn> description{std::make_shared<column::TextColumn>("description", false, true)};
	column::ColumnRef<column::IntegerColumn> userId{std::make_shared<column::IntegerColumn>("user_id")};
	column::ColumnRef<column::TimestampColumn> created{std::make_shared<column::TimestampColumn>("created")};
	column::ColumnRef<column::TimestampColumn> updated{std::make_shared<column::TimestampColumn>("updated")};
};

#endif //M_CATEGORIE_HPP
