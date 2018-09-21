#include "pch.h"
#include <iostream>
#include <cpprest/ws_client.h>

using namespace std;
using namespace web;
using namespace web::websockets::client;

int main(int argc, char *argv[])
{
	websocket_client client;

	uri link = uri(L"ws://localhost:8080/websocket/chat/Iron");
	client.connect(link).wait();

	websocket_outgoing_message out_msg;
	out_msg.set_utf8_message("test");
	client.send(out_msg).wait();

	client.receive().then([](websocket_incoming_message in_msg) {
		return in_msg.extract_string();
	}).then([](string body) {
		cout << body << endl; // test
	}).wait();

	client.close().wait();

	return 0;
}