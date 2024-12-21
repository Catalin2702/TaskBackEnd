//
// Created by Catalin Chirosca on 20/12/24.
//

#ifndef S_USER_HPP
#define S_USER_HPP

#include <memory>
#include <vector>
#include <nlohmann/json.hpp>

#include <orm/session.hpp>
#include <tools/tools.hpp>

#include "api/model/m_user.hpp"
#include "service.hpp"

using json = nlohmann::json;

class UserService final: public Service {
public:
	explicit UserService(std::shared_ptr<session::Session> session);
	[[nodiscard]] std::optional<std::vector<User>> getUsers(const std::vector<unsigned long>& ids = {}) const;
	[[nodiscard]] std::optional<User> getUser(const unsigned long id = 0) const;
	[[nodiscard]] std::optional<User> createUser(const User& user) const;
	[[nodiscard]] std::optional<User> updateUser(const unsigned long id, const User& user) const;
	[[nodiscard]] unsigned long deleteUser(const unsigned long id) const;
	[[nodiscard]] std::vector<unsigned long> deleteUsers(const std::vector<unsigned long>& ids) const;
	[[nodiscard]] static json userToJson(const User& user);
private:
	std::shared_ptr<session::Session> session;
};

#endif //S_USER_HPP
