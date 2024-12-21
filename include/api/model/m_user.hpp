//
// Created by Catalin Chirosca on 20/12/24.
//

#ifndef M_USER_HPP
#define M_USER_HPP

#include "orm/column.hpp"
#include "orm/model.hpp"

class User final : public model::Model {
public:
	User() : Model("users") {
		registerEmptyColumn<column::SerialColumn>(id);
		registerEmptyColumn<column::StringColumn>(username);
		registerEmptyColumn<column::StringColumn>(email);
		registerEmptyColumn<column::StringColumn>(password_hash);
		registerEmptyColumn<column::TimestampColumn>(created);
		registerEmptyColumn<column::TimestampColumn>(updated);
	}
	User(const std::string& username, const std::string& email, const std::string& password_hash):
		Model("users") {
		registerEmptyColumn<column::SerialColumn>(id);
		registerColumn<column::StringColumn>(this->username, username);
		registerColumn<column::StringColumn>(this->email, email);
		registerColumn<column::StringColumn>(this->password_hash, password_hash);
		registerColumn<column::TimestampColumn>(created, std::chrono::system_clock::now());
		registerColumn<column::TimestampColumn>(updated, std::chrono::system_clock::now());
	}

	column::ColumnRef<column::SerialColumn> id{std::make_shared<column::SerialColumn>("id", true)};
	column::ColumnRef<column::StringColumn> username{std::make_shared<column::StringColumn>("username", 50, false, false)};
	column::ColumnRef<column::StringColumn> email{std::make_shared<column::StringColumn>("email", 100, false, false)};
	column::ColumnRef<column::StringColumn> password_hash{std::make_shared<column::StringColumn>("password_hash", 255, false, false)};
	column::ColumnRef<column::TimestampColumn> created{std::make_shared<column::TimestampColumn>("created", false, false)};
	column::ColumnRef<column::TimestampColumn> updated{std::make_shared<column::TimestampColumn>("updated", false, false)};
};

#endif //M_USER_HPP
