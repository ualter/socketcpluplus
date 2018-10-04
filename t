[1;33mdiff --git a/SocketApplication/MultipleSocketApplication.cpp b/SocketApplication/MultipleSocketApplication.cpp[m
[1;33mindex a9b5890..fb1aa34 100644[m
[1;33m--- a/SocketApplication/MultipleSocketApplication.cpp[m
[1;33m+++ b/SocketApplication/MultipleSocketApplication.cpp[m
[35;47m@@ -1,3 +1,4 @@[m
[37;44m+[m[37;44m#pragma warning(disable: 4244)[m
 [m
 #include "pch.h"[m
 #include "SocketServer.h"[m
[35;47m@@ -13,15 +14,23 @@[m [mint main(int argc, char *argv[])[m
 	SocketServer* server = new SocketServer(9002);[m
 	std::thread threadTaskSocketServer(taskSocketServer, server);[m
 [m
[1;36m-	// Wait for 60 seconds[m
[37;44m+[m	[37;44m// Let the Server work for 180 seconds[m
 	std::clock_t start;[m
 	double duration;[m
 	start = std::clock();[m
[37;44m+[m	[37;44mint interval = 10;[m
 	while (TRUE) {[m
 		duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;[m
 		//cout << "Time... " << duration << "..." << std::endl;[m
[1;36m-		if ( duration > 60)[m
[1;36m-			 break;[m
[37;44m+[m		[37;44mdouble p1 = duration / interval;[m
[37;44m+[m		[37;44mint    p2 = duration / interval;[m
[37;44m+[m		[37;44mif ( (p1*interval) == (p2*interval) ) {[m
[37;44m+[m			[37;44mserver->broadcast("Hello from Server");[m
[37;44m+[m		[37;44m}[m
[37;44m+[m
[37;44m+[m		[37;44mif (duration > 180) {[m
[37;44m+[m			[37;44mbreak;[m
[37;44m+[m		[37;44m}[m
 	}[m
 [m
 	// Stop Server[m
[35;47m@@ -29,6 +38,5 @@[m [mint main(int argc, char *argv[])[m
 	server->stop();[m
 [m
 	threadTaskSocketServer.join();[m
[1;36m-[m
 	return 0;[m
 }[m
\ No newline at end of file[m
