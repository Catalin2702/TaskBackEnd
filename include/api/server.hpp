//
// Created by Catalin Chirosca on 20/12/24.
//

#ifndef SERVER_HPP
#define SERVER_HPP

#pragma once
#include <httplib.h>
#include <memory>
#include <string>
#include <vector>

#include "api/controller/c_user.hpp"
#include "orm/session.hpp"

class Server {
public:
	explicit Server(std::shared_ptr<session::Session> session);
	void start(const std::string& url, const int port);
	void setup();
	void stop();
	void setupRoutes();

private:
	httplib::Server srv;
	std::shared_ptr<UserController> userController;
};

#endif //SERVER_HPP
