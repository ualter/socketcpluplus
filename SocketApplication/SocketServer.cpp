#include "pch.h"
#include "SocketServer.h"

SocketServer::SocketServer(int port) {
	this->port = port;
}

SocketServer::~SocketServer() {
	this->stop();
}

void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg) {
	std::string message = msg->get_payload();

	std::cout << "Message: " << message << std::endl;

	// check for a special command to instruct the server to stop listening so
	// it can be cleanly exited.
	if (msg->get_payload() == "<<STOP>>") {
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

void SocketServer::start() {
	try {
		this->wsServer.set_access_channels(websocketpp::log::alevel::none);
		this->wsServer.clear_access_channels(websocketpp::log::alevel::none);

		this->wsServer.init_asio();
		this->wsServer.set_message_handler(bind(&on_message, &this->wsServer, ::_1, ::_2));
		this->wsServer.listen(this->port);

		cout << "Listening on " << this->port << "..." << std::endl;

		this->wsServer.start_accept();
		this->wsServer.run();

	} catch (websocketpp::exception const & e) {
		std::cout << e.what() << std::endl;
	} catch (...) {
		std::cout << "Error!!!" << std::endl;
	}
}

void SocketServer::stop() {
	this->wsServer.stop_listening();
}