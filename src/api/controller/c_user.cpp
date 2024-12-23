//
// Created by Catalin Chirosca on 20/12/24.
//
#include "api/controller/c_user.hpp"

UserController::UserController(const std::shared_ptr<UserService>& service):
	userService(service) {}

void UserController::getUsers(const httplib::Request& req, httplib::Response& res) const {
	res.set_header("Content-Type", "application/json");
	std::vector<unsigned long> ids;
	if (req.has_param("ids")) {
		const auto idsStr = req.get_param_value("ids");
		const auto idsStrVec = tools::split(idsStr, ',');
		std::transform(idsStrVec.begin(), idsStrVec.end(), std::back_inserter(ids), [](const std::string& id) {
			return std::stoul(id);
		});
	}
	try {
		json response;
		if (const auto users = userService->getUsers(ids);
			users.has_value()) {
			json data;
			data["users"] = json::array();
			for (const auto& user: users.value())
				data["users"].push_back(user.toJson());
			res.status = 200;
			response = createSuccessResponse("", data);
		}
		else {
			res.status = 404;
			response = createErrorResponse("Users not found");
		}
		res.set_content(response.dump(), "application/json");
	}
	catch (std::exception& e) {
		const std::string message =  "UserController::getUsers error: " + std::string(e.what());
		std::cerr << message << std::endl;
		const auto response = createErrorResponse(message);
		res.status = 500;
		res.set_content(response.dump(), "application/json");
	}
}
void UserController::getUser(const httplib::Request& req, httplib::Response& res) const {
	res.set_header("Content-Type", "application/json");
	if (not req.has_param("id")) {
		const auto response = createErrorResponse("Missing id parameter");
		res.status = 400;
		res.set_content(response.dump(), "application/json");
		return;
	}
	try {
		const auto id = std::stoul(req.get_param_value("id"));
		json response;
		if (const auto user = userService->getUser(id); user.has_value()) {
			json data;
			data["user"] = user.value().toJson();
			res.status = 200;
			response = createSuccessResponse("", data);
		}
		else {
			res.status = 404;
			response = createErrorResponse("User not found");
		}
		res.set_content(response.dump(), "application/json");
	}
	catch (std::exception& e) {
		const std::string message =  "UserController::getUser error: " + std::string(e.what());
		std::cerr << message << std::endl;
		const auto response = createErrorResponse(message);
		res.status = 500;
		res.set_content(response.dump(), "application/json");
	}
}
void UserController::createUser(const httplib::Request& req, httplib::Response& res) const {
	res.set_header("Content-Type", "application/json");
	const std::vector<std::string> params = {"username", "email", "password"};
	if (const auto missingParam = checkMissingParamsPOST(params, req); missingParam != "") {
		const auto errorMessage = "Missing " + missingParam + " parameter";
		auto errorResponse = createErrorResponse(errorMessage);
		res.status = 400;
		res.set_content(errorResponse.dump(), "application/json");
		return;
	}
	try {
		json jsonBody = json::parse(req.body);
		const auto username = getParamValueFromJson("username", jsonBody);
		const auto email = getParamValueFromJson("email", jsonBody);
		const auto password = getParamValueFromJson("password", jsonBody);
		const auto newUser = userService->createUser(
			User{username, email, picosha2::hash256_hex_string(password)}
		);
		json response;
		if (newUser.has_value()) {
			json data;
			data["user"] = newUser.value().toJson();
			res.status = 201;
			response = createSuccessResponse("", data);
		}
		else {
			res.status = 400;
			response = createErrorResponse("User not created");
		}
		res.set_content(response.dump(), "application/json");
	}
	catch (std::exception& e) {
		const std::string message =  "UserController::createUser error: " + std::string(e.what());
		std::cerr << message << std::endl;
		const auto response = createErrorResponse(message);
		res.status = 500;
		res.set_content(response.dump(), "application/json");
	}
}
void UserController::updateUser(const httplib::Request& req, httplib::Response& res) const {
	const auto jsonBody = json::parse(req.body);
	if (not jsonBody.contains("id")) {
		const auto response = createErrorResponse("Missing id parameter");
		res.status = 400;
		res.set_content(response.dump(), "application/json");
		return;
	}
	try {
		User user{};
		const auto id = jsonBody["id"].get<unsigned long>();
		if (const auto username = getParamValueFromJson("username", jsonBody); not username.empty())
			user.username = username;
		if (const auto email = getParamValueFromJson("email", jsonBody); not email.empty())
			user.email = email;
		if (const auto password = getParamValueFromJson("password", jsonBody); not password.empty())
			user.password_hash = picosha2::hash256_hex_string(password);
		user.updated = std::chrono::system_clock::now();

		json response;
		if (const auto newUser = userService->updateUser(id, user);
			newUser.has_value()) {
			json data;
			data["user"] = newUser.value().toJson();
			response = createSuccessResponse("", data);
			res.status = 200;
		}
		else {
			response = createErrorResponse("User not updated");
			res.status = 400;
		}
		res.set_content(response.dump(), "application/json");
	}
	catch (std::exception& e) {
		const std::string message =  "UserController::updateUser error: " + std::string(e.what());
		std::cerr << message << std::endl;
		const auto response = createErrorResponse(message);
		res.status = 500;
		res.set_content(response.dump(), "application/json");
	}
}
void UserController::deleteUser(const httplib::Request& req, httplib::Response& res) const {
	const auto jsonBody = json::parse(req.body);
	if (not jsonBody.contains("id")) {
		const auto response = createErrorResponse("Missing id parameter");
		res.status = 400;
		res.set_content(response.dump(), "application/json");
		return;
	}
	try {
		const auto id = jsonBody["id"].get<unsigned long>();
		json response;
		if (const auto deletedId = userService->deleteUser(id); deletedId) {
			json data;
			data["id"] = deletedId;
			response = createSuccessResponse("", data);
			res.status = 200;
		}
		else {
			response = createErrorResponse("User not deleted");
			res.status = 400;
		}
		res.set_content(response.dump(), "application/json");
	}
	catch (std::exception& e) {
		const std::string message =  "UserController::deleteUser error: " + std::string(e.what());
		std::cerr << message << std::endl;
		const auto response = createErrorResponse(message);
		res.status = 500;
		res.set_content(response.dump(), "application/json");
	}
}
void UserController::deleteUsers(const httplib::Request& req, httplib::Response& res) const {
	const auto jsonBody = json::parse(req.body);
	if (not jsonBody.contains("ids")) {
		const auto response = createErrorResponse("Missing ids parameter");
		res.status = 400;
		res.set_content(response.dump(), "application/json");
		return;
	}
	try {
		const auto ids = jsonBody["ids"].get<std::vector<unsigned long>>();

		json response;
		if (const auto deletedIds = userService->deleteUsers(ids);
			not deletedIds.empty() and deletedIds[0]) {
			json data;
			data["ids"] = json::array();
			for (const auto& id: deletedIds)
				data["ids"].push_back(id);
			response = createSuccessResponse("", data);
			res.status = 200;
		}
		else {
			response = createErrorResponse("Users not deleted");
			res.status = 400;
		}
		res.set_content(response.dump(), "application/json");
	}
	catch (std::exception& e) {
		const std::string message =  "UserController::deleteUsers error: " + std::string(e.what());
		std::cerr << message << std::endl;
		const auto response = createErrorResponse(message);
		res.status = 500;
		res.set_content(response, "application/json");
	}
}
