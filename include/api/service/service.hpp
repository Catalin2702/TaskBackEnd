//
// Created by Catalin Chirosca on 21/12/24.
//

#ifndef SERVICE_HPP
#define SERVICE_HPP

#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

class Service {
public:
	static std::string stringToJson(const std::string& str);
	static std::string jsonToString(const json& json);
};

#endif //SERVICE_HPP
