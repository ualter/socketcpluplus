#pragma warning(disable: 4996)
#pragma warning(disable: 4244)
#pragma warning(disable: 4267)

#include "pch.h"
#include "SocketServer.h"
#include <ctime>
#include <thread>
#include <fstream>
#include <iostream>
#include <time.h>
#include "MultipleSocketApplication.h"

void taskSocketServer(SocketServer* server) {
	server->start();
}

void replaceAll(std::string &str, std::string replaceThis, std::string byThis, int compensate = 0) {
	size_t index = 0;
	while (true) {
		/* Locate the substring to replace. */
		index = str.find(replaceThis, index);
		if (index == std::string::npos) break;
		/* Make the replacement. */
		str.replace(index, byThis.length() + compensate, byThis);
		/* Advance index forward so the next iteration doesn't pick it up as well. */
		index += byThis.length() + compensate;
	}
}

void encodeForJson(std::string &str) {
	replaceAll(str, "\"", "|");
	replaceAll(str, "|", "\\\"", -1);
}

void calculateTime(int time[3], int speed, int distance) {
	if (speed > 9) {
		// convert nautical miles to km
		speed = speed * 1.852;

		int   hourI = distance / speed;
		float hourF = ((float)distance / speed) - hourI;
		int   minuI = hourF * 60;
		float minuF = (hourF * 60) - minuI;
		int   secoI = minuF * 60;
		float secoF = (minuF * 60) - secoI;
		int   milim = secoF * 1000;

		time[0] = hourI;
		time[1] = minuI;
		time[2] = secoI;
	}
	else {
		time[0] = 99;
		time[1] = 99;
		time[2] = 99;
	}
}
void calculateTimeFormatted(char* output, int speed, int distance) {
	int arrayTime[3];
	calculateTime(arrayTime, speed, distance);

	//sprintf_s(output, "%02d:%02d:%02d.%03d", hourI, minuI, secoI, milim);
	char buffer[10];
	sprintf_s(buffer, "%02d:%02d:%02d", arrayTime[0], arrayTime[1], arrayTime[2]);
	strncpy_s(output,10,buffer,10);
}

std::vector<string> splitStringBy(std::string line) {
	std::stringstream lineToSplit(line);
	std::string segment;
	std::vector<std::string> seglist;
	while (std::getline(lineToSplit, segment, ','))
	{
		seglist.push_back(segment);
	}
	return seglist;
}

struct airwayNavaid_t {
	std::string id;
	float latitude;
	float longitude;
};
struct airway_t {
	std::string id;
	std::string sequence;
	std::list<airwayNavaid_t> navaids;
};

airway_t readingAirway(std::string strAirway, std::string strBeginNavaid, std::string strEndNavaid);
void printAirway(airway_t airway);

void changeValue(struct airway_t* airway, std::string line) {
	airwayNavaid_t navaid;
	navaid.id = line;
	airway->navaids.push_back(navaid);
}

int main(int argc, char *argv[])
{
	// Testing pass-by-reference
	/*airway_t airway;
	airwayNavaid_t navaid;
	navaid.id = "LEBL";
	airway.navaids.push_back(navaid);
	changeValue(&airway,"SBGR");
	std::list<airwayNavaid_t>::iterator it = airway.navaids.begin();
	int index = 0;
	while (it != airway.navaids.end()) {
		index++;
		airwayNavaid_t navaid = *it;
		printf("\n     (%d) %s (%f/%f)", index, navaid.id.c_str(), navaid.latitude, navaid.longitude);
		it++;
	}*/

	// Testing Airway Code Block
	/*
	Bellow must return (chose among three airways, to test the right segment according with the Beginning and End of Navaids, precisely informed);
	(1) TORDU (40.257919/0.588190)
	 (2) DIKUT (40.007061/0.236080)
	 (3) SOPET (39.833839/-0.004680)
	 (4) VLC (39.485561/-0.483190)
	 (5) ASTRO (39.024380/-1.263000)
	 (6) XEBAR (38.521141/-2.092770)
	 (7) YES (38.360821/-2.352510)
	*/
	//airway_t airway = readingAirway("UM985", "TORDU", "YES");
	/*
	Below must return (the fix of the end is not precisely informed)
	(1) NARTA (36.056370/-12.557960)
	(2) ROLAR (34.211449/-15.205740)
	*/
	airway_t airway = readingAirway("UZ14", "NARTA", "LIDRO");
	printAirway(airway);

	//std:string s = "A,UL176,6";
	//printf("%d", s.find("A,"));

	/*
	int hour = 15;
	int min  = 6;
    string timePause = "ABCDE";

	if ( timePause.length() == 5 ) {
		int hourPause = atoi(timePause.substr(0, 2).c_str());
		int minPause = atoi(timePause.substr(3, 5).c_str());
		printf("%d:%d\n", hourPause, minPause);

		if (hourPause == hour && minPause == min) {
			printf("CHECKED!");
		}
		else {
			printf("NOPE!");
		}
	}*/

	// Testing Calculating Distance vs Time
	/*char time[10];
	calculateTimeFormatted(time,250,2955);
	printf(time);*/

	/*int arrayTime[3];
	calculateTime(arrayTime, speed, distance);
	printf(std::to_string(arrayTime[0]).c_str());*/

	/*
	// Testing ReplaceAll
	char msg[] = "Remote connection \"Ualter Desktop\" said to Pause!,[Origin: ::ffff:192.168.0.27:49236]";
	std::string str = std::string(msg);
	encodeForJson(str);
	printf(str.c_str());
	*/

	/*
	// Original Program Code
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
	*/

	return 0;
}


void convertLineToNavaidAddToAirway(std::string line, struct airway_t *airway) {
	airwayNavaid_t navaid;
	std::vector<string> splittedLine = splitStringBy(line);
	navaid.id = splittedLine.at(1);
	navaid.latitude = atof(splittedLine.at(2).c_str());
	navaid.longitude = atof(splittedLine.at(3).c_str());
	airway->navaids.push_back(navaid);
}

airway_t readingAirway(std::string strAirway, std::string strBeginNavaid, std::string strEndNavaid)
{
	std::string line;
	std::string seekAirway    = "A," + strAirway      + ",";
	std::string seekBegNavaid = "S," + strBeginNavaid + ",";
	std::string seekEndNavaid = "S," + strEndNavaid   + ",";
	char inputFilename[] = "c:/X-Plane 11/Custom Data/GNS430/navdata/ATS.txt";

	ifstream inFile;
	inFile.open(inputFilename);

	bool readingAirway = false;
	bool foundBegNavaid = false;
	bool foundEndNavaid = false;

	airway_t airway;

	if (inFile) {
		size_t pos;
		while (inFile.good() && !foundEndNavaid) {
			getline(inFile, line);

			if (readingAirway && (line.empty() || line.find("A,") == 0)) {
				printf("\n2-AQUI...:%s", line.c_str());
				if (!foundBegNavaid) {
					// Check if the next Airway has the same name we are looking for AND we didn't find the Beginnig Navaid yet so far, 
					// (If we didn't the Beginning Navaid so far...) we started the loop again, keep the searching...  at the next Airway sequence with the same name (Example: A,UZ14,9 - A,UZ14,39)
					pos = line.find(seekAirway);
					if (pos != std::string::npos) {
						// Searched Airway found
						readingAirway = true;
						std::vector<string> splittedLine = splitStringBy(line);
						airway.id = splittedLine.at(1);
						airway.sequence = splittedLine.at(2);
						printf("\n2-Airway start...:%s", line.c_str());
					}
					else {
						// End of an Airway and the next found it is not the Airway searched (We didn't find what we are searching)
						readingAirway = false;
					}
				}
				else {
					// End of an Airway that we only find the Start Navaid, not the End Navaid, so keep looking... 
					readingAirway = false;
					foundBegNavaid = false;
					foundEndNavaid = false;
					// Reset airway vector (not what we looking for), we only found the start, not the end of the track (the piece we are searching... Beginning-End Navaids of Airway)
					//airway.navaids.clear();
				}
			}
			else if (readingAirway) {
				if (foundBegNavaid) {
					// In case there's a Navaid Start to read, just read after found that Navaid Start
					printf("\n2-Line Added...:%s", line.c_str());
					convertLineToNavaidAddToAirway(line, &airway);
					// Check if this is the End Navaid Searched, IF informed at the entry paramater an End Navaid to stop reading, otherwise let's go until finish this Airway
					if (!strEndNavaid.empty()) {
						pos = line.find(seekEndNavaid);
						if (pos != std::string::npos)
						{
							foundEndNavaid = true;
							break;
						}
					}
				}
				else {
					pos = line.find(seekBegNavaid);
					if (pos != std::string::npos)
					{
						airway.navaids.clear();
						foundBegNavaid = true;
						printf("\n3-Line Added...:%s", line.c_str());
						convertLineToNavaidAddToAirway(line, &airway);
					}
				}
			}
			else {
				// Check if found the searched airway
				pos = line.find(seekAirway);
				if (pos != std::string::npos) {
					// Searched Airway found
					readingAirway = true;
					std::vector<string> splittedLine = splitStringBy(line);
					airway.id = splittedLine.at(1);
					airway.sequence = splittedLine.at(2);
					printf("\n1-Airway start...:%s", line.c_str());
				}
			}
		}
	}
	else {
		printf("FILE NOT FOUND!!!");
	}
	return airway;
}

void printAirway(airway_t airway) {

	printf("\n******************************** Result *******************************");
	printf("\n  Airway...: %s - %s", airway.id, airway.sequence);
	std::list<airwayNavaid_t>::iterator it = airway.navaids.begin();
	int index = 0;
	while (it != airway.navaids.end()) {
		index++;
		airwayNavaid_t navaid = *it;
		printf("\n     (%d) %s (%f/%f)", index, navaid.id.c_str(), navaid.latitude, navaid.longitude);
		it++;
	}
	printf("\n***********************************************************************\n");

}
