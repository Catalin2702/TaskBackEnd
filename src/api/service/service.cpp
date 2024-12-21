//
// Created by Catalin Chirosca on 21/12/24.
//
#include "api/service/service.hpp"

std::string Service::stringToJson(const std::string& str) {
	return json::parse(str).dump();
}
std::string Service::jsonToString(const json& json) {
	return json.dump();
}