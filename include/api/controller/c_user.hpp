//
// Created by Catalin Chirosca on 20/12/24.
//

#ifndef C_USER_HPP
#define C_USER_HPP

#include <chrono>
#include <httplib.h>
#include <memory>

#include "controller.hpp"
#include "api/service/s_user.hpp"

class UserController final: public Controller {
public:
	explicit UserController(const std::shared_ptr<UserService>& service);

	void getUsers(const httplib::Request& req, httplib::Response& res) const;
	void getUser(const httplib::Request& req, httplib::Response& res) const;
	void createUser(const httplib::Request& req, httplib::Response& res) const;
	void updateUser(const httplib::Request& req, httplib::Response& res) const;
	void deleteUser(const httplib::Request& req, httplib::Response& res) const;
	void deleteUsers(const httplib::Request& req, httplib::Response& res) const;
private:
	std::shared_ptr<UserService> userService;
};

#endif //C_USER_HPP
