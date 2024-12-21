//
// Created by Catalin Chirosca on 20/12/24.
//
#include "api/server.hpp"

Server::Server(std::shared_ptr<session::Session> session) {
	try {
		auto userService = std::make_shared<UserService>(session);
		userController = std::make_shared<UserController>(userService);
	} catch (const std::exception& e) {
		std::cerr << "Failed to initialize server: " << e.what() << std::endl;
		throw;
	}
}

void Server::setupRoutes() {
	srv.Get("/users", [this](const auto& req, auto& res) {
		userController->getUsers(req, res);
	});
	srv.Get("/user", [this](const auto& req, auto& res) {
		userController->getUser(req, res);
	});
	srv.Post("/createUser", [this](const auto& req, auto& res) {
		userController->createUser(req, res);
	});
	srv.Post("/updateUser", [this](const auto& req, auto& res) {
		userController->updateUser(req, res);
	});
	srv.Post("/deleteUser", [this](const auto& req, auto& res) {
		userController->deleteUser(req, res);
	});
	srv.Post("/deleteUsers", [this](const auto& req, auto& res) {
		userController->deleteUsers(req, res);
	});

	srv.set_base_dir(".");
	srv.set_default_headers({
		{"Access-Control-Allow-Origin", "*"},
		{"Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS"},
		{"Access-Control-Allow-Headers", "Content-Type, Authorization"},
	});

	srv.Options(".*", [](const httplib::Request&, httplib::Response& res) {
		res.status = 204;
	});
}

void Server::setup() {
	srv.set_logger([](const auto& req, const auto&) {
		std::cout << "Incoming request: " << req.method << " " << req.path << std::endl;
	});
	srv.set_error_handler([](const auto& req, auto& res) {
		std::cerr << "Error occurred while handling: " << req.path << std::endl;
		if (res.status == 200)
			res.status = 500;
		if (res.body.empty())
			res.set_content("{\"error\": \"Internal Server Error\"}", "application/json");
	});
	srv.set_exception_handler([](const auto& req, auto& res, std::exception_ptr ep) {
		std::cerr << "Exception occurred while handling: " << req.path << std::endl;
		try {
			if (ep) {
				std::rethrow_exception(ep);
			}
		} catch (const std::exception& e) {
			std::cerr << "Exception details: " << e.what() << std::endl;
		}
		res.status = 500;
		res.set_content("{\"error\": \"Server Exception\"}", "application/json");
	});
	setupRoutes();
}


void Server::start(const std::string& url, const int port) {

	std::cout << "Server starting on " << url << ":" << port << std::endl;

	setup();

	if (srv.bind_to_port(url.c_str(), port)) {
		srv.listen_after_bind();  // Questo blocca l'esecuzione
	} else {
		throw std::runtime_error("Failed to bind to port");
	}
}

void Server::stop() {
	srv.stop();
}
