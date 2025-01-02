//
// Created by Catalin Chirosca on 23/12/24.
//
#include "api/service/s_category.hpp"

CategoryService::CategoryService(const std::shared_ptr<session::Session>& session):
	session(session) {}

std::optional<std::vector<Category>> CategoryService::getCategories(const std::vector<unsigned long>& ids) const {
	const Category category{};
	auto query = this->session->query(category);
	if (not ids.empty())
		query = query.filter(category.id.in(ids));
	const auto results = query.all();
	this->session->commit();
	return results;
}
std::optional<std::vector<Category>> CategoryService::getUserCategories(const unsigned long userId) const {
	const Category category{};
	auto query = this->session->query(category);
	query = query.filter(category.userId == static_cast<int>(userId));
	const auto results = query.all();
	this->session->commit();
	return results;
}
std::optional<std::vector<Category>> CategoryService::getUsersCategories(const std::vector<unsigned long>& userIds) const {
	const Category category{};
	auto query = this->session->query(category);
	if (not userIds.empty()) {
		std::vector<int> intUserIds;
		intUserIds.reserve(userIds.size());
		for (const auto& id : userIds) {
			intUserIds.push_back(static_cast<int>(id));
		}
		query = query.filter(category.userId.in(intUserIds));
	}
	const auto results = query.all();
	this->session->commit();
	return results;
}
std::optional<Category> CategoryService::getCategory(const unsigned long id) const {
	Category category{};
	auto query = this->session->query(category);
	if (id)
		query = query.filter(category.id == id);
	const auto result = query.first();
	this->session->commit();
	return result;
}
std::optional<Category> CategoryService::createCategory(const std::string& title, const int userId, const std::string& description) const {
	const auto results = this->session->query(Category{title, userId, description}).insert();
	this->session->commit();
	if (results.has_value())
		return results->front();
	return std::nullopt;
}
std::optional<Category> CategoryService::updateCategory(const unsigned long id, const std::optional<std::string>& title, const std::optional<int> userId, const std::optional<std::string>& description) const {
	Category category{};
	if (title.has_value())
		category.title = title.value();
	if (userId.has_value())
		category.userId = userId.value();
	if (description.has_value())
		category.description = description.value();
	category.updated = std::chrono::system_clock::now();
	const auto results = this->session->query(category).filter(category.id == id).update();
	this->session->commit();
	if (results.has_value())
		return results.value();
	return std::nullopt;
}
unsigned long CategoryService::deleteCategory(const unsigned long id) const {
	const Category category{};
	const auto results = this->session->query(category).filter(category.id == id).remove();
	this->session->commit();
	return results.front();
}
std::vector<unsigned long> CategoryService::deleteCategories(const std::vector<unsigned long>& ids) const {
	const Category category{};
	const auto results = this->session->query(category).filter(category.id.in(ids)).remove();
	this->session->commit();
	return results;
}
std::vector<unsigned long> CategoryService::deleteUsersCategories(const std::vector<unsigned long>& userIds) const {
	const Category category{};
	std::vector<int> intUserIds;
	intUserIds.reserve(userIds.size());
	for (const auto& id : userIds) {
		intUserIds.push_back(static_cast<int>(id));
	}
	const auto results = this->session->query(category).filter(category.userId.in(intUserIds)).remove();
	this->session->commit();
	return results;
}
unsigned long CategoryService::deleteUserCategories(const unsigned long userId) const {
	const Category category{};
	const auto results = this->session->query(category).filter(category.userId == static_cast<int>(userId)).remove();
	this->session->commit();
	return results.front();
}
