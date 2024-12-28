//
// Created by Catalin Chirosca on 23/12/24.
//
#include "api/model/m_task.hpp"

TaskStatus taskStatusFromString(const std::string& status) {
	if (status == "OPEN") return TaskStatus::OPEN;
	if (status == "IN_PROGRESS") return TaskStatus::IN_PROGRESS;
	if (status == "DONE") return TaskStatus::DONE;
	throw std::invalid_argument("Invalid TaskStatus value: " + status);
}
std::string taskStatusToString(const TaskStatus status) {
	switch (status) {
		case TaskStatus::OPEN:
			return "OPEN";
		case TaskStatus::IN_PROGRESS:
			return "IN_PROGRESS";
		case TaskStatus::DONE:
			return "DONE";
		default:
			throw std::invalid_argument("Invalid TaskStatus value");
	}
}

Task::Task():
	Model("tasks") {
	registerEmptyColumn<column::SerialColumn>(id);
	registerEmptyColumn<column::StringColumn>(title);
	registerEmptyColumn<column::TextColumn>(description);
	registerEmptyColumn<column::IntegerColumn>(categoryId);
	registerEmptyColumn<column::EnumColumn<TaskStatus>>(status);
	registerEmptyColumn<column::TimestampColumn>(created);
	registerEmptyColumn<column::TimestampColumn>(updated);
}
Task::Task(const std::string& title, const int categoryId, const TaskStatus status, const std::string& description):
	Model("tasks") {
	registerEmptyColumn<column::SerialColumn>(id);
	registerColumn<column::StringColumn>(this->title, title);
	registerColumn<column::TextColumn>(this->description, description);
	registerColumn<column::IntegerColumn>(this->categoryId, categoryId);
	registerColumn<column::EnumColumn<TaskStatus>>(this->status, status);
	registerColumn<column::TimestampColumn>(created, std::chrono::system_clock::now());
	registerColumn<column::TimestampColumn>(updated, std::chrono::system_clock::now());
}

json Task::toJson() const {
	json taskJson;
	taskJson["id"] = id.getValue();
	taskJson["title"] = title.getValue();
	taskJson["description"] = description.getValue();
	taskJson["categoryId"] = categoryId.getValue();
	taskJson["status"] = TaskStatusDef.at(status.getValue());
	return taskJson;
}


namespace pqxx {
	template<>
	struct string_traits<TaskStatus> {
		static TaskStatus from_string(const std::string_view& text) {
			return taskStatusFromString(std::string(text));
		}
		static std::string to_string(const TaskStatus value) {
			return taskStatusToString(value);
		}
	};
}
