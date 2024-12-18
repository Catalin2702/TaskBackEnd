//
// Created by Catalin Chirosca on 08/12/24.
//

#include <iostream>

#include "orm/database.hpp"

namespace database {

	Database::Database(const Database& database): connection(std::make_unique<pqxx::connection>(database.getConnection().connection_string())) {}
	Database::Database(const std::string& uri): connection(std::make_unique<pqxx::connection>(uri)) {}
	Database::~Database() { connection->close(); }
	pqxx::connection& Database::getConnection() const { return *connection; }

}
