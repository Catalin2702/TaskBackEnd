//
// Created by Catalin Chirosca on 08/12/24.
//

#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <memory>
#include <pqxx/pqxx>

namespace database {

	class Database {
	public:
		explicit Database(const Database& database);
		explicit Database(const std::string& uri);
		~Database();
		[[nodiscard]] pqxx::connection& getConnection() const;
	private:
		std::unique_ptr<pqxx::connection> connection;
	};

}

#endif //DATABASE_HPP
