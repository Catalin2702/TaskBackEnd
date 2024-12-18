//
// Created by Catalin Chirosca on 09/12/24.
//

#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <ctime>
#include <iomanip>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>

namespace tools {

	enum class SqlSpecialValue {
		NULL_VALUE,
		CURRENT_TIMESTAMP,
		CURRENT_DATE,
		CURRENT_TIME,
		NONE
	};

	std::tm make_ctime(const int year, const int month, const int day, const int hour = 0, const int min = 0, const int sec = 0);

	std::vector<std::string> split(const std::string& str, const char delimiter = ',');

	std::chrono::system_clock::time_point stringToDate(const std::string& dateStr, const std::string& format = "%Y-%m-%d");

	std::chrono::seconds stringToTime(const std::string& timeStr, const std::string& format = "%H:%M:%S");

	std::chrono::system_clock::time_point stringToDateTime(const std::string& dateTimeStr, const std::string& format = "%Y-%m-%d %H:%M:%S");

	std::chrono::system_clock::time_point stringToTimestamp(const std::string& dateTimeStr, const std::string& format = "%Y-%m-%d %H:%M:%S");

	std::string dateToString(const std::chrono::system_clock::time_point& date, const std::string& format = "%Y-%m-%d");

	std::string timeToString(const std::chrono::seconds& time, const std::string& format = "%H:%M:%S");

	std::string dateTimeToString(const std::chrono::system_clock::time_point& dateTime, const std::string& format = "%Y-%m-%d %H:%M:%S");

	std::string timestampToString(const std::chrono::system_clock::time_point& tp, const std::string& format = "%Y-%m-%d %H:%M:%S");

	template<typename T>
	std::string join(std::vector<T> elements, const std::string& sep = ",", const bool isString = false);

	std::string format(const std::string& value, const bool needsQuotes = false, const SqlSpecialValue specialValue = SqlSpecialValue::NONE);
}

#include "tools.tpp"

#endif //TOOLS_HPP
