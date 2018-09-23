#pragma once

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <string>

using namespace std;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

typedef websocketpp::server<websocketpp::config::asio> server;
typedef server::message_ptr message_ptr;

class SocketServer
{
public:
	SocketServer(int port);
	~SocketServer();
	void start();
	void stop();
private:
	int port;
	server wsServer;
};