#include <iostream>
#include <string>
#include <memory>
#include <orm/session.hpp>
#include "orm/column.hpp"
#include "orm/model.hpp"
#include "params.hpp"

class User final : public model::Model {
public:
	User() : Model("users") {
		registerStaticColumn<column::SerialColumn>(id);
		registerStaticColumn<column::StringColumn>(username);
		registerStaticColumn<column::StringColumn>(email);
		registerStaticColumn<column::StringColumn>(password_hash);
		registerStaticColumn<column::TimestampColumn>(created);
		registerStaticColumn<column::TimestampColumn>(updated);
	}

	column::ColumnRef<column::SerialColumn> id{std::make_shared<column::SerialColumn>("id", true)};
	column::ColumnRef<column::StringColumn> username{std::make_shared<column::StringColumn>("username", 50, false, false)};
	column::ColumnRef<column::StringColumn> email{std::make_shared<column::StringColumn>("email", 100, false, false)};
	column::ColumnRef<column::StringColumn> password_hash{std::make_shared<column::StringColumn>("password_hash", 255, false, false)};
	column::ColumnRef<column::TimestampColumn> created{std::make_shared<column::TimestampColumn>("created", false, false)};
	column::ColumnRef<column::TimestampColumn> updated{std::make_shared<column::TimestampColumn>("updated", false, false)};
};

int main() {

	const session::ConnectionMaker connectionMaker(DB_URI);

	const auto session = connectionMaker.getSession();

	std::vector<User> insertUsers;

	try {
		insertUsers = session->query(User()).all();
	} catch (std::exception& e) {
		std::cerr << "Query error: " << e.what() << std::endl;
	}

	for (const auto& user : insertUsers) {
		for (const auto& column : user.getColumns()) {
			std::cout << column->getFullName() << ": " << column->getValueAsString() << std::endl;
		}
	}

	std::vector<int> ids;

	try {
		ids = session->query(insertUsers).remove();
	} catch (std::exception& e) {
		std::cerr << "Remove error: " << e.what() << std::endl;
	}

	std::cout << "Removed " << ids.size() << " users." << std::endl;

	for (const auto& id : ids) {
		std::cout << "Removed user with id: " << id << std::endl;
	}

	session->commit();

	return 0;
}

