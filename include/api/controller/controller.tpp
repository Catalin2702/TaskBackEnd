#pragma once

template <typename T>
T Controller::getParamValueFromJson(const std::string& param, const json& jsonBody) {
	if (jsonBody.contains(param)) {
		return jsonBody[param].get<T>();
	}
	return T();
}
