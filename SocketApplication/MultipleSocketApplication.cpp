#pragma warning(disable: 4244)

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

	// Let the Server work for 180 seconds
	std::clock_t start;
	double duration;
	start = std::clock();
	int interval = 10;
	while (TRUE) {
		duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
		//cout << "Time... " << duration << "..." << std::endl;
		double p1 = duration / interval;
		int    p2 = duration / interval;
		if ( (p1*interval) == (p2*interval) ) {
			server->broadcast("Hello from Server");
		}

		if (duration > 180) {
			break;
		}
	}

	// Stop Server
	std::cout << "Time waited: " << duration << endl;
	server->stop();

	threadTaskSocketServer.join();
	return 0;
}