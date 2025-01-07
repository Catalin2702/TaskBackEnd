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
	json response;
	try {

		if (const auto users = userService->getUsers(ids);
			users.has_value()) {
			json data;
			data["users"] = json::array();
			for (const auto& user: users.value())
				data["users"].push_back(user.toJson());
			response = createSuccessResponse("", data);
		}
		else
			response = createErrorResponse("Users not found");
		res.status = 200;
	}
	catch (std::exception& e) {
		const std::string message =  "UserController::getUsers error: " + std::string(e.what());
		std::cerr << message << std::endl;
		response = createErrorResponse(message);
		res.status = 500;
	}
	res.set_content(response.dump(), "application/json");
}
void UserController::getUser(const httplib::Request& req, httplib::Response& res) const {
	res.set_header("Content-Type", "application/json");
	json response;
	if (not req.has_param("id")) {
		response = createErrorResponse("Missing id parameter");
		res.status = 400;
		res.set_content(response.dump(), "application/json");
		return;
	}
	try {
		const auto id = std::stoul(req.get_param_value("id"));
		if (const auto user = userService->getUser(id); user.has_value()) {
			json data;
			data["user"] = user.value().toJson();
			response = createSuccessResponse("", data);
		}
		else
			response = createErrorResponse("User not found");
		res.status = 200;
	}
	catch (std::exception& e) {
		const std::string message =  "UserController::getUser error: " + std::string(e.what());
		std::cerr << message << std::endl;
		response = createErrorResponse(message);
		res.status = 500;
	}
	res.set_content(response.dump(), "application/json");
}
void UserController::createUser(const httplib::Request& req, httplib::Response& res) const {
	res.set_header("Content-Type", "application/json");
	json response;
	const std::vector<std::string> params = {"username", "email"};
	if (const auto missingParam = checkMissingParamsPOST(params, req); not missingParam.empty()) {
		const auto errorMessage = "Missing " + missingParam + " parameter";
		response = createErrorResponse(errorMessage);
		res.status = 400;
		res.set_content(response.dump(), "application/json");
		return;
	}
	try {
		json jsonBody = json::parse(req.body);
		const auto username = getParamValueFromJson("username", jsonBody);
		const auto email = getParamValueFromJson("email", jsonBody);

		if (const auto newUser = userService->createUser(User{username, email});
			newUser.has_value()) {
			json data;
			data["user"] = newUser.value().toJson();
			response = createSuccessResponse("", data);
		}
		else
			response = createErrorResponse("User not created");
		res.status = 201;
	}
	catch (std::exception& e) {
		const std::string message =  "UserController::createUser error: " + std::string(e.what());
		std::cerr << message << std::endl;
		response = createErrorResponse(message);
		res.status = 500;
	}
	res.set_content(response.dump(), "application/json");
}
void UserController::updateUser(const httplib::Request& req, httplib::Response& res) const {
	res.set_header("Content-Type", "application/json");
	const auto jsonBody = json::parse(req.body);
	json response;
	if (not jsonBody.contains("id")) {
		response = createErrorResponse("Missing id parameter");
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
		user.updated = std::chrono::system_clock::now();

		if (const auto newUser = userService->updateUser(id, user);
			newUser.has_value()) {
			json data;
			data["user"] = newUser.value().toJson();
			response = createSuccessResponse("", data);
		}
		else
			response = createErrorResponse("User not updated");
		res.status = 200;
	}
	catch (std::exception& e) {
		const std::string message =  "UserController::updateUser error: " + std::string(e.what());
		std::cerr << message << std::endl;
		response = createErrorResponse(message);
		res.status = 500;
	}
	res.set_content(response.dump(), "application/json");
}
void UserController::deleteUser(const httplib::Request& req, httplib::Response& res) const {
	res.set_header("Content-Type", "application/json");
	const auto jsonBody = json::parse(req.body);
	json response;
	if (not jsonBody.contains("id")) {
		response = createErrorResponse("Missing id parameter");
		res.status = 400;
		res.set_content(response.dump(), "application/json");
		return;
	}
	try {
		const auto id = jsonBody["id"].get<unsigned long>();
		if (const auto deletedId = userService->deleteUser(id); deletedId) {
			json data;
			data["id"] = deletedId;
			response = createSuccessResponse("", data);
		}
		else
			response = createErrorResponse("User not deleted");
		res.status = 200;
	}
	catch (std::exception& e) {
		const std::string message =  "UserController::deleteUser error: " + std::string(e.what());
		std::cerr << message << std::endl;
		response = createErrorResponse(message);
		res.status = 500;
	}
	res.set_content(response.dump(), "application/json");
}
void UserController::deleteUsers(const httplib::Request& req, httplib::Response& res) const {
	res.set_header("Content-Type", "application/json");
	const auto jsonBody = json::parse(req.body);
	json response;
	if (not jsonBody.contains("ids")) {
		response = createErrorResponse("Missing ids parameter");
		res.status = 400;
		res.set_content(response.dump(), "application/json");
		return;
	}
	try {
		const auto ids = jsonBody["ids"].get<std::vector<unsigned long>>();
		if (const auto deletedIds = userService->deleteUsers(ids);
			not deletedIds.empty() and deletedIds[0]) {
			json data;
			data["ids"] = json::array();
			for (const auto& id: deletedIds)
				data["ids"].push_back(id);
			response = createSuccessResponse("", data);
		}
		else
			response = createErrorResponse("Users not deleted");
		res.status = 200;
	}
	catch (std::exception& e) {
		const std::string message =  "UserController::deleteUsers error: " + std::string(e.what());
		std::cerr << message << std::endl;
		response = createErrorResponse(message);
		res.status = 500;
	}
	res.set_content(response, "application/json");
}
