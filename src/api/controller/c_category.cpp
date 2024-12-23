//
// Created by Catalin Chirosca on 23/12/24.
//
#include "api/controller/c_category.hpp"

CategoryController::CategoryController(const std::shared_ptr<CategoryService>& service):
	categoryService(service) {}

void CategoryController::getCategories(const httplib::Request& req, httplib::Response& res) const {
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
		
		if (const auto categories = categoryService->getCategories(ids);
			categories.has_value()) {
			json data;
			data["categories"] = json::array();
			for (const auto& category: categories.value())
				data["categories"].push_back(category.toJson());
			res.status = 200;
			response = createSuccessResponse("", data);
		}
		else {
			res.status = 404;
			response = createErrorResponse("Categories not found");
		}
	}
	catch (std::exception& e) {
		const std::string message =  "CategoryController::getCategories error: " + std::string(e.what());
		std::cerr << message << std::endl;
		response = createErrorResponse(message);
		res.status = 500;
	}
	res.set_content(response.dump(), "application/json");
}
void CategoryController::getUserCategories(const httplib::Request& req, httplib::Response& res) const {
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
		if (const auto categories = categoryService->getUserCategories(userId);
			categories.has_value()) {
			json data;
			data["categories"] = json::array();
			for (const auto& category: categories.value())
				data["categories"].push_back(category.toJson());
			res.status = 200;
			response = createSuccessResponse("", data);
		}
		else {
			res.status = 404;
			response = createErrorResponse("Categories not found");
		}
	}
	catch (std::exception& e) {
		const std::string message =  "CategoryController::getUserCategories error: " + std::string(e.what());
		std::cerr << message << std::endl;
		response = createErrorResponse(message);
		res.status = 500;
	}
	res.set_content(response.dump(), "application/json");
}
void CategoryController::getUsersCategories(const httplib::Request& req, httplib::Response& res) const {
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
		if (const auto categories = categoryService->getUsersCategories(userIds);
			categories.has_value()) {
			json data;
			data["categories"] = json::array();
			for (const auto& category: categories.value())
				data["categories"].push_back(category.toJson());
			res.status = 200;
			response = createSuccessResponse("", data);
		}
		else {
			res.status = 404;
			response = createErrorResponse("Categories not found");
		}
	}
	catch (std::exception& e) {
		const std::string message =  "CategoryController::getUsersCategories error: " + std::string(e.what());
		std::cerr << message << std::endl;
		response = createErrorResponse(message);
		res.status = 500;
	}
	res.set_content(response.dump(), "application/json");
}
void CategoryController::getCategory(const httplib::Request& req, httplib::Response& res) const {
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
		if (const auto category = categoryService->getCategory(id);
			category.has_value()) {
			json data;
			data["category"] = category.value().toJson();
			res.status = 200;
			response = createSuccessResponse("", data);
		}
		else {
			res.status = 404;
			response = createErrorResponse("Category not found");
		}
	}
	catch (std::exception& e) {
		const std::string message =  "CategoryController::getCategory error: " + std::string(e.what());
		std::cerr << message << std::endl;
		response = createErrorResponse(message);
		res.status = 500;
	}
	res.set_content(response.dump(), "application/json");
}
void CategoryController::createCategory(const httplib::Request& req, httplib::Response& res) const {
	res.set_header("Content-Type", "application/json");
	json response;
	const std::vector<std::string> params = {"title", "userId"};
	if (const auto missingParam = checkMissingParamsPOST(params, req); not missingParam.empty()) {
		const std::string message =  "CategoryController::createCategory error: Missing parameters: " + missingParam;
		std::cerr << message << std::endl;
		response = createErrorResponse(message);
		res.status = 400;
		res.set_content(response.dump(), "application/json");
		return;
	}
	try {
		json jsonBody = json::parse(req.body);
		const auto title = jsonBody["title"].get<std::string>();
		const auto userId = jsonBody["userId"].get<int>();
		const auto description = getParamValueFromJson<std::string>("description", jsonBody);

		if (const auto newCategory = categoryService->createCategory(Category(title, userId, description));
			newCategory.has_value()) {
			json data;
			data["category"] = newCategory.value().toJson();
			response = createSuccessResponse("", data);
			res.status = 201;
		}
		else {
			response = createErrorResponse("Category not created");
			res.status = 400;
		}
	}
	catch (std::exception& e) {
		const std::string message =  "CategoryController::createCategory error: " + std::string(e.what());
		std::cerr << message << std::endl;
		response = createErrorResponse(message);
		res.status = 500;
	}
	res.set_content(response.dump(), "application/json");
}
void CategoryController::updateCategory(const httplib::Request& req, httplib::Response& res) const {
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
		Category category{};
		const auto id = jsonBody["id"].get<unsigned long>();
		if (const auto title = getParamValueFromJson<std::string>("title", jsonBody); not title.empty())
			category.title = title;
		if (const auto description = getParamValueFromJson<std::string>("description", jsonBody); not description.empty())
			category.description = description;
		if (const auto userId = getParamValueFromJson<int>("userId", jsonBody); userId)
			category.userId = userId;
		category.updated = std::chrono::system_clock::now();

		if (const auto updatedCategory = categoryService->updateCategory(id, category);
			updatedCategory.has_value()) {
			json data;
			data["category"] = updatedCategory.value().toJson();
			response = createSuccessResponse("", data);
			res.status = 200;
		}
		else {
			response = createErrorResponse("Category not updated");
			res.status = 400;
		}
	}
	catch (std::exception& e) {
		const std::string message =  "CategoryController::updateCategory error: " + std::string(e.what());
		std::cerr << message << std::endl;
		response = createErrorResponse(message);
		res.status = 500;
	}
	res.set_content(response.dump(), "application/json");
}
void CategoryController::deleteCategory(const httplib::Request& req, httplib::Response& res) const {
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
		if (const auto deletedId = categoryService->deleteCategory(id); deletedId) {
			json data;
			data["id"] = deletedId;
			response = createSuccessResponse("", data);
			res.status = 200;
		}
		else {
			response = createErrorResponse("Category not deleted");
			res.status = 400;
		}
	}
	catch (std::exception& e) {
		const std::string message =  "CategoryController::deleteCategory error: " + std::string(e.what());
		std::cerr << message << std::endl;
		response = createErrorResponse(message);
		res.status = 500;
	}
	res.set_content(response.dump(), "application/json");
}
void CategoryController::deleteCategories(const httplib::Request& req, httplib::Response& res) const {
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
		if (const auto deletedIds = categoryService->deleteCategories(ids);
			not deletedIds.empty()) {
			json data;
			data["ids"] = deletedIds;
			response = createSuccessResponse("", data);
			res.status = 200;
		}
		else {
			response = createErrorResponse("Categories not deleted");
			res.status = 400;
		}
	}
	catch (std::exception& e) {
		const std::string message =  "CategoryController::deleteCategories error: " + std::string(e.what());
		std::cerr << message << std::endl;
		response = createErrorResponse(message);
		res.status = 500;
	}
	res.set_content(response.dump(), "application/json");
}
