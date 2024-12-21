//
// Created by Catalin Chirosca on 21/12/24.
//
#include "api/controller/controller.hpp"

json Controller::createResponse(const bool status, const std::string& message, const json& data) {
	json response;
	response["status"] = status;
	response["message"] = message;
	response["data"] = data;
	return response;
}
json Controller::createErrorResponse(const std::string& message) {
	return createResponse(false, message);
}
json Controller::createSuccessResponse(const std::string& message, const json& data)  {
	return createResponse(true, message, data);
}
std::string Controller::checkMissingParamsGET(const std::vector<std::string>& params, const httplib::Request& req) {
	for (const auto& param: params) {
		if (not req.has_param(param))
			return param;
	}
	return "";
}
std::string Controller::checkMissingParamsPOST(const std::vector<std::string>& params, const httplib::Request& req) {
	const auto jsonBody = json::parse(req.body);
	for (const auto& param: params) {
		if (jsonBody.find(param) == jsonBody.end())
			return param;
	}
	return "";
}
std::string Controller::getParamValueGET(const std::string& param, const httplib::Request& req) {
	if (req.has_param(param))
		return req.get_param_value(param);
	return "";
}
std::string Controller::getParamValuePOST(const std::string& param, const httplib::Request& req) {
	if (const auto jsonBody = json::parse(req.body); jsonBody.find(param) != jsonBody.end())
		return jsonBody[param];
	return "";
}
