//
// Created by Catalin Chirosca on 20/12/24.
//
#include "api/server.hpp"

#ifndef NDEBUG
	#define DEBUG_LOG(x) std::cout << x << std::endl;
#else
    #define DEBUG_LOG(x)
#endif

void Server::setupCategoryRoutes() {
	srv.Get("/category", [this](const auto& req, auto& res) { categoryController->getCategory(req, res); });
	srv.Get("/categories", [this](const auto& req, auto& res) { categoryController->getCategories(req, res); });
	srv.Get("/categories/user", [this](const auto& req, auto& res) { categoryController->getUserCategories(req, res); });
	srv.Get("/categories/users", [this](const auto& req, auto& res) { categoryController->getUsersCategories(req, res); });
	srv.Post("/category", [this](const auto& req, auto& res) { categoryController->createCategory(req, res); });
	srv.Put("/category", [this](const auto& req, auto& res) { categoryController->updateCategory(req, res); });
	srv.Delete("/category", [this](const auto& req, auto& res) { categoryController->deleteCategory(req, res); });
	srv.Delete("/categories", [this](const auto& req, auto& res) { categoryController->deleteCategories(req, res); });
	srv.Delete("/categories/user",[this](const auto& req, auto& res) { categoryController->deleteUserCategories(req, res); });
	srv.Delete("/categories/users", [this](const auto& req, auto& res) { categoryController->deleteUsersCategories(req, res); });
}
void Server::setupTaskRoutes() {
	srv.Get("/task", [this](const auto& req, auto& res) { taskController->getTask(req, res); });
	srv.Get("/tasks", [this](const auto& req, auto& res) { taskController->getTasks(req, res); });
	srv.Get("/tasks/user", [this](const auto& req, auto& res) { taskController->getUserTasks(req, res); });
	srv.Get("/tasks/users", [this](const auto& req, auto& res) { taskController->getUsersTasks(req, res); });
	srv.Post("/task", [this](const auto& req, auto& res) { taskController->createTask(req, res); });
	srv.Put("/task", [this](const auto& req, auto& res) { taskController->updateTask(req, res); });
	srv.Delete("/task", [this](const auto& req, auto& res) { taskController->deleteTask(req, res); });
	srv.Delete("/tasks", [this](const auto& req, auto& res) { taskController->deleteTasks(req, res); });
	srv.Delete("/tasks/user", [this](const auto& req, auto& res) { taskController->deleteUserTasks(req, res); });
	srv.Delete("/tasks/users", [this](const auto& req, auto& res) { taskController->deleteUsersTasks(req, res); });
}
void Server::setupUserRoutes() {
	srv.Get("/user", [this](const auto& req, auto& res) { userController->getUser(req, res); });
	srv.Get("/users", [this](const auto& req, auto& res) { userController->getUsers(req, res); });
	srv.Post("/user", [this](const auto& req, auto& res) { userController->createUser(req, res); });
	srv.Put("/user", [this](const auto& req, auto& res) { userController->updateUser(req, res); });
	srv.Delete("/user", [this](const auto& req, auto& res) { userController->deleteUser(req, res); });
	srv.Delete("/users", [this](const auto& req, auto& res) { userController->deleteUsers(req, res); });
}
Server::Server(const std::shared_ptr<session::Session>& session) {
	try {
		auto userService = std::make_shared<UserService>(session);
		userController = std::make_shared<UserController>(userService);
		auto categoryService = std::make_shared<CategoryService>(session);
		categoryController = std::make_shared<CategoryController>(categoryService);
		auto taskService = std::make_shared<TaskService>(session);
		taskController = std::make_shared<TaskController>(taskService);
	} catch (const std::exception& e) {
		std::cerr << "Failed to initialize server: " << e.what() << std::endl;
		throw;
	}
}
void Server::setupRoutes() {
	setupCategoryRoutes();
	setupTaskRoutes();
	setupUserRoutes();

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
		DEBUG_LOG("Incoming request: " << req.method << " " << req.path);
	});
	srv.set_error_handler([](const auto& req, auto& res) {
		if (res.status == 500)
			std::cerr << "Error occurred while handling: " << req.path << std::endl;
		else if (res.status == 200)
			res.status = 500;
		if (res.body.empty())
			res.set_content(R"({"error": "Internal Server Error"})", "application/json");
	});
	srv.set_exception_handler([](const auto& req, auto& res, const std::exception_ptr& ep) {
		std::cerr << "Exception occurred while handling: " << req.path << std::endl;
		try {
			if (ep) {
				std::rethrow_exception(ep);
			}
		} catch (const std::exception& e) {
			std::cerr << "Exception details: " << e.what() << std::endl;
		}
		res.status = 500;
		res.set_content(R"({"error": "Server Exception"})", "application/json");
	});
	setupRoutes();
}
void Server::start(const std::string& url, const int port) {

	std::cout << "Server starting on " << url << ":" << port << std::endl;

	setup();

	if (srv.bind_to_port(url, port)) {
		srv.listen_after_bind();  // Questo blocca l'esecuzione
	} else {
		throw std::runtime_error("Failed to bind to port");
	}
}
void Server::stop() {
	srv.stop();
}
