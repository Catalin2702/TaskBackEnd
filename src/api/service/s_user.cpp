//
// Created by Catalin Chirosca on 20/12/24.
//
#include "api/service/s_user.hpp"

UserService::UserService(std::shared_ptr<session::Session> session):
	session(std::move(session)) {}

std::optional<std::vector<User>> UserService::getUsers(const std::vector<unsigned long>& ids) const {
	const User user{};
	auto query = session->query(user);
	if (not ids.empty())
		query = query.filter(user.id.in(ids));
	const auto results = query.all();
	session->commit();
	return results;
}
std::optional<User> UserService::getUser(const unsigned long id) const {
	User user{};
	auto query = session->query(user);
	if (id)
		query = query.filter(user.id == id);
	const auto result = query.first();
	session->commit();
	return result;
}
std::optional<User> UserService::createUser(const User& user) const {
	const auto results = session->query(user).insert();
	session->commit();
	if (results.has_value())
		return results->front();
	return std::nullopt;
}
std::optional<User> UserService::updateUser(const unsigned long id, const User& user) const {
	const auto results = session->query(user).filter(user.id == id).update();
	session->commit();
	if (results.has_value())
		return results.value();
	return std::nullopt;
}
unsigned long UserService::deleteUser(const unsigned long id) const {
	const User user{};
	const auto results = session->query(user).filter(user.id == id).remove();
	session->commit();
	return results.front();
}
std::vector<unsigned long> UserService::deleteUsers(const std::vector<unsigned long>& ids) const {
	const User user{};
	const auto results = session->query(user).filter(user.id.in(ids)).remove();
	session->commit();
	return results;
}
json UserService::userToJson(const User& user) {
	json userJson;
	userJson["id"] = user.id.getValue();
	userJson["username"] = user.username.getValue();
	userJson["email"] = user.email.getValue();
	userJson["created"] = tools::timestampToString(user.created.getValue());
	userJson["updated"] = tools::timestampToString(user.updated.getValue());
	return userJson;
}
