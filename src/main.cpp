#include <orm/session.hpp>
#include "params.hpp"
#include "api/server.hpp"


int main() {

	try {
		const session::ConnectionMaker connectionMaker(DB_URI);
		Server srv(connectionMaker.getSession());
		srv.start(SERVER_URL, SERVER_PORT);
	}
	catch (std::exception& e) {
		std::cerr << "Error starting server: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}
