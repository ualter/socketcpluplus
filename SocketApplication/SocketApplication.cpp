
#include "pch.h"
#include <iostream>
#include<io.h>
#include<stdio.h>
#include<winsock2.h>
#include <sstream>

#pragma comment(lib,"ws2_32.lib") //Winsock Library


int main(int argc, char *argv[])
{
	WSADATA wsa;
	SOCKET s, new_socket;
	struct sockaddr_in server, client;
	int c;
	const char *message;
	char *buffer;
	int MAXRECV = 1024;
	buffer = (char*)malloc((MAXRECV + 1) * sizeof(char));

	printf("\nInitialising Winsock...\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}
	printf("Initialised.\n");

	//Create a socket
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}
	printf("Socket created.\n");

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8888);

	//Bind
	if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
	}

	puts("Bind done");

	listen(s, 3);

	//Accept and incoming connection
	puts("Waiting for incoming connections...");

	c = sizeof(struct sockaddr_in);
	new_socket = accept(s, (struct sockaddr *)&client, &c);
	if (new_socket == INVALID_SOCKET)
	{
		printf("accept failed with error code : %d", WSAGetLastError());
	}

	puts("Connection accepted");

	
	int valread = recv(new_socket, buffer, MAXRECV, 0);
	printf("Received: %s", buffer);

	//Reply to client
	std::string response = "response";
	std::stringstream wsss;
	wsss << "HTTP/1.1 200 OK\r\n"
		<< "Connection: keep-alive\r\n"
		<< "Content-Type: image/x-icon\r\n"
		<< "Content-Length: " << response.length() << "\r\n"
		<< "\r\n";
	std::string headers = wsss.str();
	int datalen = headers.size();
	int numSent;
	char *pdata = NULL;
	while (datalen > 0) {
		numSent = send(new_socket, headers.c_str(), headers.length(), 0);
		if (numSent == -1) {
			int error_code = WSAGetLastError();
			printf("recv failed with error code : %d", error_code);
		}
		pdata += numSent;
		datalen -= numSent;
	}

	valread = recv(new_socket, buffer, MAXRECV, 0);
	if (valread > 0) {
		printf("Received: %s", buffer);
	}

	closesocket(s);
	WSACleanup();

	return 0;
}
