//
// Created by Catalin Chirosca on 20/12/24.
//

#ifndef SERVER_HPP
#define SERVER_HPP

#pragma once
#include <httplib.h>
#include <memory>
#include <string>
#include <orm/session.hpp>

#include "api/controller/c_category.hpp"
#include "api/controller/c_task.hpp"
#include "api/controller/c_user.hpp"

class Server {
public:
	explicit Server(const std::shared_ptr<session::Session>& session);
	void start(const std::string& url, const int port);
	void setup();
	void stop();
	void setupRoutes();

private:
	void setupCategoryRoutes();
	void setupTaskRoutes();
	void setupUserRoutes();

	httplib::Server srv;
	std::shared_ptr<CategoryController> categoryController;
	std::shared_ptr<TaskController> taskController;
	std::shared_ptr<UserController> userController;
};

#endif //SERVER_HPP
