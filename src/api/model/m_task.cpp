//
// Created by Catalin Chirosca on 23/12/24.
//
#include "api/model/m_task.hpp"

Task::Task():
	Model("tasks") {
	registerEmptyColumn<column::SerialColumn>(id);
	registerEmptyColumn<column::StringColumn>(title);
	registerEmptyColumn<column::TextColumn>(description);
	registerEmptyColumn<column::IntegerColumn>(categoryId);
	registerEmptyColumn<column::EnumColumn<TaskStatus>>(status);
}
Task::Task(const std::string& title, const std::string& description, const int categorieId, const TaskStatus status):
	Model("tasks") {
	registerEmptyColumn<column::SerialColumn>(id);
	registerColumn<column::StringColumn>(this->title, title);
	registerColumn<column::TextColumn>(this->description, description);
	registerColumn<column::IntegerColumn>(this->categoryId, categorieId);
	registerColumn<column::EnumColumn<TaskStatus>>(this->status, status);
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
			if (text == "OPEN")
				return TaskStatus::OPEN;
			if (text == "IN_PROGRESS")
				return TaskStatus::IN_PROGRESS;
			if (text == "DONE")
				return TaskStatus::DONE;
			throw std::invalid_argument("Invalid TaskStatus value: " + std::string(text));
		}

		static std::string to_string(const TaskStatus value) {
			switch (value) {
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
	};
}
