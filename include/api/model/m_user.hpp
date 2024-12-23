//
// Created by Catalin Chirosca on 20/12/24.
//

#ifndef M_USER_HPP
#define M_USER_HPP

#include <orm/column.hpp>
#include <orm/model.hpp>

class User final : public model::Model {
public:
	User();
	User(const std::string& username, const std::string& email, const std::string& password_hash);

	column::ColumnRef<column::SerialColumn> id{std::make_shared<column::SerialColumn>("id", true, false)};
	column::ColumnRef<column::StringColumn> username{std::make_shared<column::StringColumn>("username", 50)};
	column::ColumnRef<column::StringColumn> email{std::make_shared<column::StringColumn>("email", 100)};
	column::ColumnRef<column::StringColumn> password_hash{std::make_shared<column::StringColumn>("password_hash", 255)};
	column::ColumnRef<column::TimestampColumn> created{std::make_shared<column::TimestampColumn>("created")};
	column::ColumnRef<column::TimestampColumn> updated{std::make_shared<column::TimestampColumn>("updated")};
};

#endif //M_USER_HPP
