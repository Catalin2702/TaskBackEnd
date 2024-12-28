//
// Created by Catalin Chirosca on 23/12/24.
//

#ifndef C_TASK_HPP
#define C_TASK_HPP

#include <chrono>
#include <httplib.h>
#include <memory>

#include "controller.hpp"
#include "api/service/s_task.hpp"

class TaskController final: public Controller {
public:
	explicit TaskController(const std::shared_ptr<TaskService>& service);

	void getTasks(const httplib::Request& req, httplib::Response& res) const;
	void getCategoryTasks(const httplib::Request& req, httplib::Response& res) const;
	void getUserTasks(const httplib::Request& req, httplib::Response& res) const;
	void getCategoriesTasks(const httplib::Request& req, httplib::Response& res) const;
	void getUsersTasks(const httplib::Request& req, httplib::Response& res) const;
	void getTask(const httplib::Request& req, httplib::Response& res) const;
	void createTask(const httplib::Request& req, httplib::Response& res) const;
	void updateTask(const httplib::Request& req, httplib::Response& res) const;
	void deleteTask(const httplib::Request& req, httplib::Response& res) const;
	void deleteTasks(const httplib::Request& req, httplib::Response& res) const;
	void deleteCategoriesTasks(const httplib::Request& req, httplib::Response& res) const;
	void deleteCategoryTasks(const httplib::Request& req, httplib::Response& res) const;
	void deleteUsersTasks(const httplib::Request& req, httplib::Response& res) const;
	void deleteUserTasks(const httplib::Request& req, httplib::Response& res) const;
private:
	std::shared_ptr<TaskService> taskService;
};

#endif //C_TASK_HPP
