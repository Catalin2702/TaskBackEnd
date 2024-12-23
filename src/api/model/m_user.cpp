//
// Created by Catalin Chirosca on 20/12/24.
//
#include "api/model/m_user.hpp"

User::User():
	Model("users") {
	registerEmptyColumn<column::SerialColumn>(id);
	registerEmptyColumn<column::StringColumn>(username);
	registerEmptyColumn<column::StringColumn>(email);
	registerEmptyColumn<column::TimestampColumn>(created);
	registerEmptyColumn<column::TimestampColumn>(updated);
}
User::User(const std::string& username, const std::string& email):
	Model("users") {
	registerEmptyColumn<column::SerialColumn>(id);
	registerColumn<column::StringColumn>(this->username, username);
	registerColumn<column::StringColumn>(this->email, email);
	registerColumn<column::TimestampColumn>(created, std::chrono::system_clock::now());
	registerColumn<column::TimestampColumn>(updated, std::chrono::system_clock::now());
}
json User::toJson() const {
	json userJson;
	userJson["id"] = id.getValue();
	userJson["username"] = username.getValue();
	userJson["email"] = email.getValue();
	userJson["created"] = tools::timestampToString(created.getValue());
	userJson["updated"] = tools::timestampToString(updated.getValue());
	return userJson;
}
