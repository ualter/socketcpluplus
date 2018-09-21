
#include "pch.h"
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

typedef websocketpp::server<websocketpp::config::asio> server;
typedef server::message_ptr message_ptr;

void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg) {
	std::string message = msg->get_payload();

	//std::cout << "on_message called with hdl: " << std::endl;
	std::cout << "Message: " << message << std::endl;

	// check for a special command to instruct the server to stop listening so
	// it can be cleanly exited.
	if (msg->get_payload() == "stop-listening") {
		s->stop_listening();
		return;
	}

	try {
		s->send(hdl, msg->get_payload(), msg->get_opcode());
	}
	catch (websocketpp::exception const & e) {
		std::cout << "Echo failed because: " << "(" << e.what() << ")" << std::endl;
	}
}

int main(int argc, char *argv[])
{
	server wsServer;
	try {
		wsServer.set_access_channels(websocketpp::log::alevel::none);
		wsServer.clear_access_channels(websocketpp::log::alevel::none);

		wsServer.init_asio();

		wsServer.set_message_handler(bind(&on_message,&wsServer,::_1,::_2));

		wsServer.listen(9002);

		std::cout << "Listening on 9002..." << std::endl;

		wsServer.start_accept();

		wsServer.run();
	}
	catch (websocketpp::exception const & e) {
		std::cout << e.what() << std::endl;
	}
	catch (...) {
		std::cout << "Error!!!" << std::endl;
	}

	return 0;
}