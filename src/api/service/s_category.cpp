//
// Created by Catalin Chirosca on 23/12/24.
//
#include <climits>

#include "api/service/s_category.hpp"

CategoryService::CategoryService(std::shared_ptr<session::Session> session):
	session(std::move(session)) {
}

std::optional<std::vector<Category>> CategoryService::getCategories(const std::vector<unsigned long>& ids) const {
	const Category categorie{};
	auto query = session->query(categorie);
	if (not ids.empty())
		query = query.filter(categorie.id.in(ids));
	const auto results = query.all();
	session->commit();
	return results;
}
std::optional<std::vector<Category>> CategoryService::getUserCategories(const unsigned long userId) const {
	const Category categorie{};
	auto query = session->query(categorie);
	query = query.filter(categorie.userId == userId);
	const auto results = query.all();
	session->commit();
	return results;
}
std::optional<std::vector<Category>> CategoryService::getUsersCategories(const std::vector<unsigned long>& userIds) const {
	const Category categorie{};
	auto query = session->query(categorie);
	if (not userIds.empty()) {
		std::vector<int> unsignedIds;
		unsignedIds.reserve(userIds.size());
		for (const auto& id : userIds) {
			unsignedIds.push_back(id & INT_MAX);
		}
		query = query.filter(categorie.userId.in(unsignedIds));
	}
	const auto results = query.all();
	session->commit();
	return results;
}
std::optional<Category> CategoryService::getCategory(const unsigned long id) const {
	Category categorie{};
	auto query = session->query(categorie);
	if (id)
		query = query.filter(categorie.id == id);
	const auto result = query.first();
	session->commit();
	return result;
}
std::optional<Category> CategoryService::createCategory(const Category& categorie) const {
	const auto results = session->query(categorie).insert();
	session->commit();
	if (results.has_value())
		return results->front();
	return std::nullopt;
}
std::optional<Category> CategoryService::updateCategory(const unsigned long id, const Category& categorie) const {
	const auto results = session->query(categorie).filter(categorie.id == id).update();
	session->commit();
	if (results.has_value())
		return results.value();
	return std::nullopt;
}
unsigned long CategoryService::deleteCategory(const unsigned long id) const {
	const Category categorie{};
	const auto results = session->query(categorie).filter(categorie.id == id).remove();
	session->commit();
	return results.front();
}
std::vector<unsigned long> CategoryService::deleteCategories(const std::vector<unsigned long>& ids) const {
	const Category categorie{};
	const auto results = session->query(categorie).filter(categorie.id.in(ids)).remove();
	session->commit();
	return results;
}
