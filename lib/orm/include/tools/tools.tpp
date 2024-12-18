#pragma once

namespace tools {

	template<typename T>
	std::string join(std::vector<T> elements, const std::string& sep, const bool isString) {
		if (elements.empty())
			return "";

		std::ostringstream result;

		if (isString)
			result << "'" << elements[0] << "'";
		else
			result << elements[0];

		for (size_t i = 1; i < elements.size(); ++i) {
			if (isString)
				result << sep << "'" << elements[i] << "'";
			else
				result << sep << elements[i];
		}

		return result.str();
	}
}
