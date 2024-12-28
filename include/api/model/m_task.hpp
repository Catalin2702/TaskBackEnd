//
// Created by Catalin Chirosca on 23/12/24.
//

#ifndef M_TASK_HPP
#define M_TASK_HPP

#include <chrono>
#include <map>
#include <orm/column.hpp>
#include <orm/model.hpp>

enum class TaskStatus {
	OPEN,
	IN_PROGRESS,
	DONE
};

inline std::map<TaskStatus, std::string> TaskStatusDef {
	{TaskStatus::OPEN, "OPEN"},
	{TaskStatus::IN_PROGRESS, "IN_PROGRESS"},
	{TaskStatus::DONE, "DONE"},
};

TaskStatus taskStatusFromString(const std::string& status);
std::string taskStatusToString(const TaskStatus status);

class Task final : public model::Model {
public:
	Task();
	Task(const std::string& title, const int categoryId, const TaskStatus status, const std::string& description = "");

	column::ColumnRef<column::SerialColumn> id{std::make_shared<column::SerialColumn>("id", true, false)};
	column::ColumnRef<column::StringColumn> title{std::make_shared<column::StringColumn>("title", 100)};
	column::ColumnRef<column::TextColumn> description{std::make_shared<column::TextColumn>("description", false, true)};
	column::ColumnRef<column::IntegerColumn> categoryId{std::make_shared<column::IntegerColumn>("category_id")};
	column::ColumnRef<column::EnumColumn<TaskStatus>> status{std::make_shared<column::EnumColumn<TaskStatus>>("status", TaskStatusDef)};
	column::ColumnRef<column::TimestampColumn> created{std::make_shared<column::TimestampColumn>("created", false, true)};
	column::ColumnRef<column::TimestampColumn> updated{std::make_shared<column::TimestampColumn>("updated", false, true)};

	[[nodiscard]] json toJson() const override;
};

#endif //M_TASK_HPP
