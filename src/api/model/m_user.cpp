//
// Created by Catalin Chirosca on 20/12/24.
//
#include "api/model/m_user.hpp"

User::User():
	Model("users") {
	registerEmptyColumn<column::SerialColumn>(id);
	registerEmptyColumn<column::StringColumn>(username);
	registerEmptyColumn<column::StringColumn>(email);
	registerEmptyColumn<column::StringColumn>(password_hash);
	registerEmptyColumn<column::TimestampColumn>(created);
	registerEmptyColumn<column::TimestampColumn>(updated);
}
User::User(const std::string& username, const std::string& email, const std::string& password_hash):
	Model("users") {
	registerEmptyColumn<column::SerialColumn>(id);
	registerColumn<column::StringColumn>(this->username, username);
	registerColumn<column::StringColumn>(this->email, email);
	registerColumn<column::StringColumn>(this->password_hash, password_hash);
	registerColumn<column::TimestampColumn>(created, std::chrono::system_clock::now());
	registerColumn<column::TimestampColumn>(updated, std::chrono::system_clock::now());
}
