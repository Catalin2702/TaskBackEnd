//
// Created by Catalin Chirosca on 23/12/24.
//

#ifndef S_TASK_HPP
#define S_TASK_HPP

#include <memory>
#include <vector>
#include <nlohmann/json.hpp>
#include <orm/session.hpp>
#include <tools/tools.hpp>

#include "api/model/m_task.hpp"
#include "api/model/m_category.hpp"
#include "service.hpp"

class TaskService final: public Service {
public:
	explicit TaskService(std::shared_ptr<session::Session> session);
	[[nodiscard]] std::optional<std::vector<Task>> getTasks(const std::vector<unsigned long>& ids = {}) const;
	[[nodiscard]] std::optional<std::vector<Task>> getCategoryTasks(const unsigned long categoryId) const;
	[[nodiscard]] std::optional<std::vector<Task>> getUserTasks(const unsigned long userId) const;
	[[nodiscard]] std::optional<std::vector<Task>> getCategoriesTasks(const std::vector<unsigned long>& categoryIds = {}) const;
	[[nodiscard]] std::optional<std::vector<Task>> getUsersTasks(const std::vector<unsigned long>& usersIds = {}) const;
	[[nodiscard]] std::optional<Task> getTask(const unsigned long id = 0) const;
	[[nodiscard]] std::optional<Task> createTask(const std::string& title, const int categoryId, const TaskStatus status, const std::string& description = "") const;
	[[nodiscard]] std::optional<Task> updateTask(const unsigned long id, const std::optional<std::string>& title, const std::optional<int> categoryId, const std::optional<TaskStatus> status, const std::optional<std::string>& description) const;
	[[nodiscard]] unsigned long deleteTask(const unsigned long id) const;
	[[nodiscard]] std::vector<unsigned long> deleteTasks(const std::vector<unsigned long>& ids) const;
	[[nodiscard]] std::vector<unsigned long> deleteCategoriesTasks(const std::vector<unsigned long>& categoryIds) const;
    [[nodiscard]] std::vector<unsigned long> deleteCategoryTasks(const unsigned long categoryId) const;
    [[nodiscard]] std::vector<unsigned long> deleteUsersTasks(const std::vector<unsigned long>& userIds) const;
    [[nodiscard]] std::vector<unsigned long> deleteUserTasks(const unsigned long userId) const;
private:
	std::shared_ptr<session::Session> session;
};

#endif //S_TASK_HPP
