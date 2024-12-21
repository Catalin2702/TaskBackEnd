//
// Created by Catalin Chirosca on 21/12/24.
//

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <httplib.h>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;

class Controller {
public:
	static json createResponse(const bool status, const std::string& message, const json& data = json());
	static json createErrorResponse(const std::string& message);
	static json createSuccessResponse(const std::string& message, const json& data = json());
	static std::string checkMissingParamsGET(const std::vector<std::string>& params, const httplib::Request& req);
	static std::string checkMissingParamsPOST(const std::vector<std::string>& params, const httplib::Request& req);
	static std::string getParamValueGET(const std::string& param, const httplib::Request& req);
	static std::string getParamValuePOST(const std::string& param, const httplib::Request& req);
	template<typename T = std::string>
	static T getParamValueFromJson(const std::string& param, const json& jsonBody);
};

#include "controller.tpp"

#endif //CONTROLLER_HPP
