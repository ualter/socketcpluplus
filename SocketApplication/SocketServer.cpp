#include "pch.h"
#include "SocketServer.h"

SocketServer::SocketServer(int port) {
	this->port = port;
}

SocketServer::~SocketServer() {
	this->stop();
}

void SocketServer::broadcast(string message) {

}

void SocketServer::on_open(websocketpp::connection_hdl hdl) {
	connection_data data;

	data.sessionid = this->sessionId++;
	data.name = "Connection";

	this->connections[hdl] = data;
	this->arraySocketClients.push_back(hdl);
}

void SocketServer::on_message(websocketpp::connection_hdl hdl, message_ptr msg) {
	std::string message = msg->get_payload();
	std::cout << "Message: " << message << std::endl;

	// check for a special command to instruct the server to stop listening so
	// it can be cleanly exited.
	if (msg->get_payload() == "<<STOP>>") {
		this->wsServer.stop_listening();
		return;
	}

	try {
		this->wsServer.send(hdl, msg->get_payload(), msg->get_opcode());
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
		//this->wsServer.set_open_handler(bind(&on_open::on_open, this, ::_1));
		//this->wsServer.set_message_handler(bind(&on_message, &this->wsServer, &this->arraySocketClients, ::_1, ::_2));
		this->wsServer.set_message_handler(bind(&SocketServer::on_message, this, ::_1, ::_2));
		//this->wsServer.set_close_handler(bind(&print_server::on_close, this, ::_1));
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