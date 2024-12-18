//
// Created by Catalin Chirosca on 11/12/24.
//

#include <iostream>

#include "tools/tools.hpp"

namespace tools {
	std::tm make_ctime(const int year, const int month, const int day, const int hour, const int min, const int sec) {
		std::tm time = {};
		time.tm_year = year - 1900;
		time.tm_mon = month - 1;
		time.tm_mday = day;
		time.tm_hour = hour;
		time.tm_min = min;
		time.tm_sec = sec;
		return time;
	}

	std::vector<std::string> split(const std::string& str, const char delimiter) {
		std::vector<std::string> result;
		size_t start = 0;
		size_t end = str.find(delimiter);

		while (end != std::string::npos) {
			result.push_back(str.substr(start, end - start));
			start = end + 1;
			end = str.find(delimiter, start);
		}

		if (start < str.length()) {
			result.push_back(str.substr(start));
		}

		return result;
	}

	std::chrono::system_clock::time_point stringToDate(const std::string& dateStr, const std::string& format) {
		std::tm tm = {};
		std::istringstream ss(dateStr);
		ss >> std::get_time(&tm, format.c_str());

		if (ss.fail()) throw std::runtime_error("Date format not valid: " + dateStr);

		const auto time = std::mktime(&tm);
		return std::chrono::system_clock::from_time_t(time);
	}

	std::chrono::seconds stringToTime(const std::string& timeStr, const std::string& format) {
		std::tm tm = {};
		std::istringstream ss(timeStr);
		ss >> std::get_time(&tm, format.c_str());

		if (ss.fail()) throw std::runtime_error("Time format not valid: " + timeStr);

		return std::chrono::seconds(tm.tm_hour * 3600 + tm.tm_min * 60 + tm.tm_sec);
	}

	std::chrono::system_clock::time_point stringToDateTime(const std::string& dateTimeStr, const std::string& format) {
		std::tm tm = {};
		std::istringstream ss(dateTimeStr);
		ss >> std::get_time(&tm, format.c_str());

		if (ss.fail()) throw std::runtime_error("DateTime format not valid: " + dateTimeStr);

		const auto time = std::mktime(&tm);
		return std::chrono::system_clock::from_time_t(time);
	}

	std::chrono::system_clock::time_point stringToTimestamp(const std::string& dateTimeStr, const std::string& format) {
		size_t dotPos = dateTimeStr.find('.');
		std::string mainPart = dateTimeStr.substr(0, dotPos);

		std::tm tm = {};
		std::istringstream ss(mainPart);
		ss >> std::get_time(&tm, format.c_str());
		if (ss.fail()) {
			throw std::runtime_error("DateTime format not valid: " + dateTimeStr);
		}
		auto timePoint = std::chrono::system_clock::from_time_t(std::mktime(&tm));
		if (dotPos != std::string::npos) {
			std::string microsStr = dateTimeStr.substr(dotPos + 1);
			// Assicurati che la stringa sia di 6 cifre (microsecondi)
			microsStr = microsStr.substr(0, 6);
			while (microsStr.length() < 6) {
				microsStr += '0';
			}
			auto micros = std::chrono::microseconds(std::stoll(microsStr));
			timePoint += micros;
		}
		return timePoint;
	}

	std::string dateToString(const std::chrono::system_clock::time_point& date, const std::string& format) {
		const auto time = std::chrono::system_clock::to_time_t(date);
		std::stringstream ss;
		ss << std::put_time(std::localtime(&time), format.c_str());
		return ss.str();
	}

	std::string timeToString(const std::chrono::seconds& time, const std::string& format) {
		const auto hours = std::chrono::duration_cast<std::chrono::hours>(time);
		const auto minutes = std::chrono::duration_cast<std::chrono::minutes>(time % std::chrono::hours(1));
		const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(time % std::chrono::minutes(1));

		// Funzione helper per formattare a due cifre.
		auto twoDigits = [](const int num) {
			std::ostringstream oss;
			oss << std::setfill('0') << std::setw(2) << num;
			return oss.str();
		};

		std::map<std::string, std::string> mapping {
			{"%H", twoDigits(static_cast<int>(hours.count()))},
			{"%M", twoDigits(static_cast<int>(minutes.count()))},
			{"%S", twoDigits(static_cast<int>(seconds.count()))},
		};

		char delimiter = '\0';
		for (const char del: {'-', '/', ':', '_', ' '}) {
			if (format.find(del) != std::string::npos) {
				delimiter = del;
				break;
			}
		}

		if (delimiter == '\0') throw std::runtime_error("Time format not valid: " + format);

		const auto v_format = split(format, delimiter);
		std::string result;

		for (int i = 0; i < v_format.size(); ++i) {
			result += mapping.count(v_format[i]) ? mapping[v_format[i]] : v_format[i];
			if (i != v_format.size() - 1)
				result += delimiter;
		}

		return result;
	}

	std::string dateTimeToString(const std::chrono::system_clock::time_point& dateTime, const std::string& format) {
		const auto time = std::chrono::system_clock::to_time_t(dateTime);
		std::stringstream ss;
		ss << std::put_time(std::localtime(&time), format.c_str());
		return ss.str();
	}

	std::string timestampToString(const std::chrono::system_clock::time_point& tp, const std::string& format) {
		const auto ttime_t = std::chrono::system_clock::to_time_t(tp);
		const auto tm = *std::localtime(&ttime_t);

		const auto duration = tp.time_since_epoch();
		const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
		const auto micros = std::chrono::duration_cast<std::chrono::microseconds>(duration - seconds);

		std::ostringstream oss;
		oss << std::put_time(&tm, format.c_str())
			<< '.' << std::setfill('0') << std::setw(6) << micros.count();

		return oss.str();
	}

	std::string format(const std::string& value, const bool needsQuotes, const SqlSpecialValue specialValue) {
		if (specialValue != SqlSpecialValue::NONE)
			switch (specialValue) {
				case SqlSpecialValue::NULL_VALUE:
					return "NULL";
				case SqlSpecialValue::CURRENT_DATE:
					return "CURRENT_DATE";
				case SqlSpecialValue::CURRENT_TIME:
					return "CURRENT_TIME";
				case SqlSpecialValue::CURRENT_TIMESTAMP:
					return "CURRENT_TIMESTAMP";
				default:
					break;
			}
		return needsQuotes ? "'" + value + "'" : value;
	}

}