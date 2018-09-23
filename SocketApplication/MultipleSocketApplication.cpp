
#include "pch.h"
#include "SocketServer.h"
#include <ctime>
#include <thread>

void taskSocketServer(SocketServer* server) {
	server->start();
}

int main(int argc, char *argv[])
{
	SocketServer* server = new SocketServer(9002);
	std::thread threadTaskSocketServer(taskSocketServer, server);

	// Wait for 3 seconds
	std::clock_t start;
	double duration;
	start = std::clock();
	while (TRUE) {
		duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
		//cout << "Time... " << duration << "..." << std::endl;
		if ( duration > 3)
			 break;
	}

	// Stop Server
	std::cout << "Time waited: " << duration << endl;
	server->stop();

	threadTaskSocketServer.join();

	return 0;
}