//
// Created by Catalin Chirosca on 23/12/24.
//
#include "api/controller/c_task.hpp"

TaskController::TaskController(const std::shared_ptr<TaskService>& service):
	taskService(service) {}

void TaskController::getTasks(const httplib::Request& req, httplib::Response& res) const {
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

		if (const auto tasks = taskService->getTasks(ids);
			tasks.has_value()) {
			json data;
			data["tasks"] = json::array();
			for (const auto& task: tasks.value())
				data["tasks"].push_back(task.toJson());
			response = createSuccessResponse("", data);
		}
		else
			response = createErrorResponse("Tasks not found");
		res.status = 200;
	}
	catch (std::exception& e) {
		const std::string message =  "TaskController::getTasks error: " + std::string(e.what());
		std::cerr << message << std::endl;
		response = createErrorResponse(message);
		res.status = 500;
	}
	res.set_content(response.dump(), "application/json");
}
void TaskController::getCategoryTasks(const httplib::Request& req, httplib::Response& res) const {
	res.set_header("Content-Type", "application/json");
	json response;
	if (not req.has_param("categoryId")) {
		response = createErrorResponse("Missing categoryId parameter");
		res.status = 400;
		res.set_content(response.dump(), "application/json");
		return;
	}
	try {
		const auto categoryId = std::stoul(req.get_param_value("categoryId"));
		if (const auto tasks = taskService->getCategoryTasks(categoryId);
			tasks.has_value()) {
			json data;
			data["tasks"] = json::array();
			for (const auto& task: tasks.value())
				data["tasks"].push_back(task.toJson());
			response = createSuccessResponse("", data);
		}
		else
			response = createErrorResponse("Tasks not found");
		res.status = 200;
	}
	catch (std::exception& e) {
		const std::string message =  "TaskController::getCategoryTasks error: " + std::string(e.what());
		std::cerr << message << std::endl;
		response = createErrorResponse(message);
		res.status = 500;
	}
	res.set_content(response.dump(), "application/json");
}
void TaskController::getUserTasks(const httplib::Request& req, httplib::Response& res) const {
	res.set_header("Content-Type", "application/json");
	json response;
	if (not req.has_param("userId")) {
		response = createErrorResponse("Missing userId parameter");
		res.status = 400;
		res.set_content(response.dump(), "application/json");
		return;
	}
	try {
		const auto userId = std::stoul(req.get_param_value("userId"));
		if (const auto tasks = taskService->getUserTasks(userId);
			tasks.has_value()) {
			json data;
			data["tasks"] = json::array();
			for (const auto& task: tasks.value())
				data["tasks"].push_back(task.toJson());
			response = createSuccessResponse("", data);
		}
		else
			response = createErrorResponse("Tasks not found");
		res.status = 200;
	}
	catch (std::exception& e) {
		const std::string message =  "TaskController::getUserTasks error: " + std::string(e.what());
		std::cerr << message << std::endl;
		response = createErrorResponse(message);
		res.status = 500;
	}
	res.set_content(response.dump(), "application/json");
}
void TaskController::getCategoriesTasks(const httplib::Request& req, httplib::Response& res) const {
	res.set_header("Content-Type", "application/json");
	std::vector<unsigned long> categoryIds;
	if (req.has_param("categoryIds")) {
		const auto categoryIdsStr = req.get_param_value("categoryIds");
		const auto categoryIdsStrVec = tools::split(categoryIdsStr, ',');
		std::transform(categoryIdsStrVec.begin(), categoryIdsStrVec.end(), std::back_inserter(categoryIds), [](const std::string& id) {
			return std::stoul(id);
		});
	}
	json response;
	try {
		if (const auto tasks = taskService->getCategoriesTasks(categoryIds);
			tasks.has_value()) {
			json data;
			data["tasks"] = json::array();
			for (const auto& task: tasks.value())
				data["tasks"].push_back(task.toJson());
			response = createSuccessResponse("", data);
		}
		else
			response = createErrorResponse("Tasks not found");
		res.status = 200;
	}
	catch (std::exception& e) {
		const std::string message =  "TaskController::getCategoriesTasks error: " + std::string(e.what());
		std::cerr << message << std::endl;
		response = createErrorResponse(message);
		res.status = 500;
	}
	res.set_content(response.dump(), "application/json");
}
void TaskController::getUsersTasks(const httplib::Request& req, httplib::Response& res) const {
	res.set_header("Content-Type", "application/json");
	std::vector<unsigned long> userIds;
	if (req.has_param("userIds")) {
		const auto userIdsStr = req.get_param_value("userIds");
		const auto userIdsStrVec = tools::split(userIdsStr, ',');
		std::transform(userIdsStrVec.begin(), userIdsStrVec.end(), std::back_inserter(userIds), [](const std::string& id) {
			return std::stoul(id);
		});
	}
	json response;
	try {
		if (const auto tasks = taskService->getUsersTasks(userIds);
			tasks.has_value()) {
			json data;
			data["tasks"] = json::array();
			for (const auto& task: tasks.value())
				data["tasks"].push_back(task.toJson());
			response = createSuccessResponse("", data);
		}
		else
			response = createErrorResponse("Tasks not found");
		res.status = 200;
	}
	catch (std::exception& e) {
		const std::string message =  "TaskController::getUsersTasks error: " + std::string(e.what());
		std::cerr << message << std::endl;
		response = createErrorResponse(message);
		res.status = 500;
	}
	res.set_content(response.dump(), "application/json");
}
void TaskController::getTask(const httplib::Request& req, httplib::Response& res) const {
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
		if (const auto task = taskService->getTask(id);
			task.has_value()) {
			json data;
			data["task"] = task.value().toJson();
			response = createSuccessResponse("", data);
		}
		else
			response = createErrorResponse("Task not found");
		res.status = 200;
	}
	catch (std::exception& e) {
		const std::string message =  "TaskController::getTask error: " + std::string(e.what());
		std::cerr << message << std::endl;
		response = createErrorResponse(message);
		res.status = 500;
	}
	res.set_content(response.dump(), "application/json");
}
void TaskController::createTask(const httplib::Request& req, httplib::Response& res) const {
	res.set_header("Content-Type", "application/json");
	const auto jsonBody = json::parse(req.body);
	json response;
	if (const auto missingParam = checkMissingParamsPOST({"title", "categoryId"}, req); not missingParam.empty()) {
		const std::string message =  "TaskController::createTask error: Missing parameters: " + missingParam;
		std::cerr << message << std::endl;
		response = createErrorResponse(message);
		res.status = 400;
		res.set_content(response.dump(), "application/json");
		return;
	}
	try {
		const auto title = jsonBody["title"].get<std::string>();
		const auto categoryId = jsonBody["categoryId"].get<int>();
		const auto status_string = getParamValueFromJson("status", jsonBody);
		const auto description = getParamValueFromJson("description", jsonBody);

		const auto status = status_string.empty() ? TaskStatus::OPEN : taskStatusFromString(status_string);

		if (const auto createdTask = taskService->createTask(title, categoryId, status, description);
			createdTask.has_value()) {
			json data;
			data["task"] = createdTask.value().toJson();
			response = createSuccessResponse("", data);
		}
		else
			response = createErrorResponse("Task not created");
		res.status = 201;
	}
	catch (std::exception& e) {
		const std::string message =  "TaskController::createTask error: " + std::string(e.what());
		std::cerr << message << std::endl;
		response = createErrorResponse(message);
		res.status = 500;
	}
	res.set_content(response.dump(), "application/json");
}
void TaskController::updateTask(const httplib::Request& req, httplib::Response& res) const {
	res.set_header("Content-Type", "application/json");
	const auto jsonBody = json::parse(req.body);
	json response;
	if (not jsonBody.contains("id") or not jsonBody.contains("task")) {
		response = createErrorResponse("Missing id or task parameter");
		res.status = 400;
		res.set_content(response.dump(), "application/json");
		return;
	}
	try {
		const auto id = jsonBody["id"].get<unsigned long>();
		std::optional<std::string> title, description;
		std::optional<int> categoryId;
		std::optional<TaskStatus> status;
		if (jsonBody.contains("title"))
			title = jsonBody["title"].get<std::string>();
		if (jsonBody.contains("description"))
			description = jsonBody["description"].get<std::string>();
		if (jsonBody.contains("categoryId"))
			categoryId = jsonBody["categoryId"].get<int>();
		if (jsonBody.contains("status"))
			status = taskStatusFromString(jsonBody["status"].get<std::string>());

		if (const auto updatedTask = taskService->updateTask(id, title, categoryId, status, description);
			updatedTask.has_value()) {
			json data;
			data["task"] = updatedTask.value().toJson();
			response = createSuccessResponse("", data);
		}
		else
			response = createErrorResponse("Task not updated");
		res.status = 200;
	}
	catch (std::exception& e) {
		const std::string message =  "TaskController::updateTask error: " + std::string(e.what());
		std::cerr << message << std::endl;
		response = createErrorResponse(message);
		res.status = 500;
	}
	res.set_content(response.dump(), "application/json");
}
void TaskController::deleteTask(const httplib::Request& req, httplib::Response& res) const {
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
		if (const auto deletedId = taskService->deleteTask(id); deletedId) {
			json data;
			data["id"] = deletedId;
			response = createSuccessResponse("", data);
		}
		else
			response = createErrorResponse("Task not deleted");
		res.status = 200;
	}
	catch (std::exception& e) {
		const std::string message =  "TaskController::deleteTask error: " + std::string(e.what());
		std::cerr << message << std::endl;
		response = createErrorResponse(message);
		res.status = 500;
	}
	res.set_content(response.dump(), "application/json");
}
void TaskController::deleteTasks(const httplib::Request& req, httplib::Response& res) const {
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
		if (const auto deletedIds = taskService->deleteTasks(ids);
			not deletedIds.empty() and deletedIds.front() != 0) {
			json data;
			data["ids"] = deletedIds;
			response = createSuccessResponse("", data);
		}
		else
			response = createErrorResponse("Tasks not deleted");
		res.status = 200;
	}
	catch (std::exception& e) {
		const std::string message =  "TaskController::deleteTasks error: " + std::string(e.what());
		std::cerr << message << std::endl;
		response = createErrorResponse(message);
		res.status = 500;
	}
	res.set_content(response.dump(), "application/json");
}
void TaskController::deleteCategoriesTasks(const httplib::Request& req, httplib::Response& res) const {
	res.set_header("Content-Type", "application/json");
	const auto jsonBody = json::parse(req.body);
	json response;
	if (not jsonBody.contains("categoryIds")) {
		response = createErrorResponse("Missing categoryIds parameter");
		res.status = 400;
		res.set_content(response.dump(), "application/json");
		return;
	}
	try {
		const auto categoryIds = jsonBody["categoryIds"].get<std::vector<unsigned long>>();
		if (const auto deletedIds = taskService->deleteCategoriesTasks(categoryIds);
			not deletedIds.empty() and deletedIds.front() != 0) {
			json data;
			data["ids"] = deletedIds;
			response = createSuccessResponse("", data);
		}
		else
			response = createErrorResponse("Tasks not deleted");
		res.status = 200;
	}
	catch (std::exception& e) {
		const std::string message =  "TaskController::deleteCategoriesTasks error: " + std::string(e.what());
		std::cerr << message << std::endl;
		response = createErrorResponse(message);
		res.status = 500;
	}
	res.set_content(response.dump(), "application/json");
}
void TaskController::deleteCategoryTasks(const httplib::Request& req, httplib::Response& res) const {
	res.set_header("Content-Type", "application/json");
	const auto jsonBody = json::parse(req.body);
	json response;
	if (not jsonBody.contains("categoryId")) {
		response = createErrorResponse("Missing categoryId parameter");
		res.status = 400;
		res.set_content(response.dump(), "application/json");
		return;
	}
	try {
		const auto categoryId = jsonBody["categoryId"].get<unsigned long>();
		if (const auto deletedIds = taskService->deleteCategoryTasks(categoryId);
			deletedIds.empty() and deletedIds.front() != 0) {
			json data;
			data["id"] = deletedIds;
			response = createSuccessResponse("", data);
		}
		else
			response = createErrorResponse("Tasks not deleted");
		res.status = 200;
	}
	catch (std::exception& e) {
		const std::string message =  "TaskController::deleteCategoryTasks error: " + std::string(e.what());
		std::cerr << message << std::endl;
		response = createErrorResponse(message);
		res.status = 500;
	}
	res.set_content(response.dump(), "application/json");
}
void TaskController::deleteUsersTasks(const httplib::Request& req, httplib::Response& res) const {
	res.set_header("Content-Type", "application/json");
	const auto jsonBody = json::parse(req.body);
	json response;
	if (not jsonBody.contains("userIds")) {
		response = createErrorResponse("Missing userIds parameter");
		res.status = 400;
		res.set_content(response.dump(), "application/json");
		return;
	}
	try {
		const auto userIds = jsonBody["userIds"].get<std::vector<unsigned long>>();
		if (const auto deletedIds = taskService->deleteUsersTasks(userIds);
			not deletedIds.empty() and deletedIds.front() != 0) {
			json data;
			data["ids"] = deletedIds;
			response = createSuccessResponse("", data);
		}
		else
			response = createErrorResponse("Tasks not deleted");
		res.status = 200;
	}
	catch (std::exception& e) {
		const std::string message =  "TaskController::deleteUsersTasks error: " + std::string(e.what());
		std::cerr << message << std::endl;
		response = createErrorResponse(message);
		res.status = 500;
	}
	res.set_content(response.dump(), "application/json");
}
void TaskController::deleteUserTasks(const httplib::Request& req, httplib::Response& res) const {
	res.set_header("Content-Type", "application/json");
	const auto jsonBody = json::parse(req.body);
	json response;
	if (not jsonBody.contains("userId")) {
		response = createErrorResponse("Missing userId parameter");
		res.status = 400;
		res.set_content(response.dump(), "application/json");
		return;
	}
	try {
		const auto userId = jsonBody["userId"].get<unsigned long>();
		if (const auto deletedIds = taskService->deleteUserTasks(userId);
			not deletedIds.empty() and deletedIds.front() != 0) {
			json data;
			data["ids"] = deletedIds;
			response = createSuccessResponse("", data);
		}
		else
			response = createErrorResponse("Tasks not deleted");
		res.status = 200;
	}
	catch (std::exception& e) {
		const std::string message =  "TaskController::deleteUserTasks error: " + std::string(e.what());
		std::cerr << message << std::endl;
		response = createErrorResponse(message);
		res.status = 500;
	}
	res.set_content(response.dump(), "application/json");
}
