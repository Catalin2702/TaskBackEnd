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
	const User userModel;

	const session::ConnectionMaker connectionMaker(DB_URI);

	const auto session = connectionMaker.getSession();

	const auto users = session->query(userModel).filter(userModel.email->iLike("%CATALIN%")).all();
	auto user = users.front();

	user.email = "catalin.chirosca@otconsulting.com";

	const auto newUser = session->query(user).update();
	session->commit();

	for (const auto& column: newUser.getColumns()) {
		std::cout << column->toString() << " " << column->getValueAsString() << std::endl;
	}

	return 0;
}

