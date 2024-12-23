//
// Created by Catalin Chirosca on 23/12/24.
//
#include "api/service/s_task.hpp"

TaskService::TaskService(std::shared_ptr<session::Session> session):
	session(std::move(session)) {}

std::optional<std::vector<Task>> TaskService::getTasks(const std::vector<unsigned long>& ids) const {
	const Task task{};
	auto query = session->query(task);
	if (not ids.empty())
		query = query.filter(task.id.in(ids));
	const auto results = query.all();
	session->commit();
	return results;
}
std::optional<std::vector<Task>> TaskService::getCategoryTasks(const unsigned long categoryId) const {
	const Task task{};
	auto query = session->query(task);
	query = query.filter(task.categoryId == static_cast<int>(categoryId));
	const auto results = query.all();
	session->commit();
	return results;
}
std::optional<std::vector<Task>> TaskService::getUserTasks(const unsigned long userId) const {
	const Category category{};
	const auto categories = session->query(category).filter(category.userId == static_cast<int>(userId)).all();
	if (not categories.has_value())
		return std::nullopt;
	const Task task{};
	std::vector<int> categoryIds;
	categoryIds.reserve(categories->size());
	for (const auto& cat : categories.value()) {
		categoryIds.push_back(static_cast<int>(cat.id.getValue()));
	}
	const auto tasks = session->query(task).filter(task.categoryId.in(categoryIds)).all();
	session->commit();
	return tasks;
}
std::optional<std::vector<Task>> TaskService::getCategoriesTasks(const std::vector<unsigned long>& categoryIds) const {
	const Task task{};
	auto query = session->query(task);
	if (not categoryIds.empty()) {
		std::vector<int> unsignedIds;
		unsignedIds.reserve(categoryIds.size());
		for (const auto& id : categoryIds)
			unsignedIds.push_back(static_cast<int>(id));
		query = query.filter(task.categoryId.in(unsignedIds));
	}
	const auto results = query.all();
	session->commit();
	return results;
}
std::optional<std::vector<Task>> TaskService::getUsersTasks(const std::vector<unsigned long>& usersIds) const {
	if (usersIds.empty())
		return std::nullopt;
	const Category category{};
	std::vector<int> u_ids;
	u_ids.reserve(usersIds.size());
	for (const auto& id : usersIds)
		u_ids.push_back(static_cast<int>(id));
	const auto categories = session->query(category).filter(category.userId.in(u_ids)).all();
	if (not categories.has_value())
		return std::nullopt;
	const Task task{};
	std::vector<int> c_ids;
	c_ids.reserve(categories->size());
	for (const auto& cat : categories.value())
		c_ids.push_back(static_cast<int>(cat.id.getValue()));
	const auto tasks = session->query(task).filter(task.categoryId.in(c_ids)).all();
	session->commit();
	return tasks;
}
std::optional<Task> TaskService::getTask(const unsigned long id) const {
	const Task task{};
	const auto result = session->query(task).filter(task.id == id).first();
	session->commit();
	return result;
}
std::optional<Task> TaskService::createTask(const Task& task) const {
	const auto results = session->query(task).insert();
	session->commit();
	if (results.has_value())
		return results->front();
	return std::nullopt;
}
std::optional<Task> TaskService::updateTask(const unsigned long id, const Task& task) const {
	const auto results = session->query(task).filter(task.id == id).update();
	session->commit();
	if (results.has_value())
		return results.value();
	return std::nullopt;
}
unsigned long TaskService::deleteTask(const unsigned long id) const {
	const Task task{};
	const auto results = session->query(task).filter(task.id == id).remove();
	session->commit();
	return results.front();
}
std::vector<unsigned long> TaskService::deleteTasks(const std::vector<unsigned long>& ids) const {
	const Task task{};
	const auto results = session->query(task).filter(task.id.in(ids)).remove();
	session->commit();
	return results;
}
