//
// Created by Catalin Chirosca on 23/12/24.
//

#ifndef C_CATEGORY_HPP
#define C_CATEGORY_HPP

#include <chrono>
#include <httplib.h>
#include <memory>

#include "controller.hpp"
#include "api/service/s_category.hpp"

class CategoryController final: public Controller {
public:
	explicit CategoryController(const std::shared_ptr<CategoryService>& service);

	void getCategories(const httplib::Request& req, httplib::Response& res) const;
	void getUserCategories(const httplib::Request& req, httplib::Response& res) const;
	void getUsersCategories(const httplib::Request& req, httplib::Response& res) const;
	void getCategory(const httplib::Request& req, httplib::Response& res) const;
	void createCategory(const httplib::Request& req, httplib::Response& res) const;
	void updateCategory(const httplib::Request& req, httplib::Response& res) const;
	void deleteCategory(const httplib::Request& req, httplib::Response& res) const;
	void deleteCategories(const httplib::Request& req, httplib::Response& res) const;
	void deleteUsersCategories(const httplib::Request& req, httplib::Response& res) const;
	void deleteUserCategories(const httplib::Request& req, httplib::Response& res) const;
private:
	std::shared_ptr<CategoryService> categoryService;
};

#endif //C_CATEGORY_HPP
