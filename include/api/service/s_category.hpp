//
// Created by Catalin Chirosca on 23/12/24.
//

#ifndef S_CATEGORY_HPP
#define S_CATEGORY_HPP

#include <memory>
#include <vector>
#include <orm/session.hpp>

#include "api/model/m_category.hpp"
#include "service.hpp"

class CategoryService final: public Service {
public:
	explicit CategoryService(const std::shared_ptr<session::Session>& session);
	[[nodiscard]] std::optional<std::vector<Category>> getCategories(const std::vector<unsigned long>& ids = {}) const;
	[[nodiscard]] std::optional<std::vector<Category>> getUserCategories(const unsigned long userId) const;
	[[nodiscard]] std::optional<std::vector<Category>> getUsersCategories(const std::vector<unsigned long>& userIds = {}) const;
	[[nodiscard]] std::optional<Category> getCategory(const unsigned long id = 0) const;
	[[nodiscard]] std::optional<Category> createCategory(const Category& categorie) const;
	[[nodiscard]] std::optional<Category> updateCategory(const unsigned long id, const Category& categorie) const;
	[[nodiscard]] unsigned long deleteCategory(const unsigned long id) const;
	[[nodiscard]] std::vector<unsigned long> deleteCategories(const std::vector<unsigned long>& ids) const;
	[[nodiscard]] std::vector<unsigned long> deleteUsersCategories(const std::vector<unsigned long>& userIds) const;
    [[nodiscard]] unsigned long deleteUserCategories(const unsigned long userId) const;
private:
	std::shared_ptr<session::Session> session;
};

#endif //S_CATEGORY_HPP
