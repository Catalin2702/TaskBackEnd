//
// Created by Catalin Chirosca on 16/12/24.
//

#include "orm/session.hpp"

namespace session {

	Session::Session(const database::Database& db) : db(db), transaction(nullptr) { beginTransaction(); }
	Session::~Session() {
		if (transaction) {
			try {
				rollback();
			} catch (...) {
				std::cerr << "Error rolling back transaction" << std::endl;
			}
		}
	}
	void Session::commit() {
		if (transaction) {
			transaction->commit();
			transaction.reset();
		}
	}
	void Session::rollback() {
		if (transaction) {
			transaction->abort();
			transaction.reset();
		}
	}
	pqxx::result Session::execute(const std::string& query) {
		ensureTransaction();
		try {
			return transaction->exec(query);
		} catch (const std::exception& e) {
			rollback();
			throw std::runtime_error(std::string("Query execution failed: ") + e.what());
		}
	}

	void Session::beginTransaction() {
		if (transaction == nullptr) {
			transaction = std::make_unique<pqxx::work>(db.getConnection());
		}
	}
	void Session::ensureTransaction() {
		beginTransaction();
	}

	ConnectionMaker::ConnectionMaker(const std::string& uri) {
		try {
			db = std::make_unique<database::Database>(uri);
		} catch (std::exception& e) {
			std::cerr << "Error connecting to database: " << e.what() << std::endl;
		}
	}

	std::unique_ptr<Session> ConnectionMaker::getSession() const {
		return std::make_unique<Session>(*db);
	}

}