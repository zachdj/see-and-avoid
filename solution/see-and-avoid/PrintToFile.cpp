#include "PrintToFile.h"
#include <string>
#include <iostream>
#include <sstream>
#include <ctime>

const std::string PrintToFile::name = "Simulation.mylog";
const std::string PrintToFile::nameDebug = "SimulationDebug.mylog";
int PrintToFile::collisionNum = 1;
int PrintToFile::collisionDebugNum = 1;
std::ofstream writeFile;
std::ofstream writeDebugFile;

void PrintToFile::print(std::string printable, bool printHeader) {
	writeFile.open(name, std::ios::app);
	if (writeFile.is_open()) {
		if (printHeader) {
			std::stringstream num; num << collisionNum;
			writeFile << "Collision #" << collisionNum <<  "\n";
			writeFile << printable << "\n";
			collisionNum++;
		}
			
		else
			writeFile << printable << "\n";
	}
	else
		std::cout << "Cannot open file..." << std::endl;

	writeFile.close();
}

void PrintToFile::clearFile() {
	writeFile.open(name, std::ios::out);
	writeFile.clear();
	writeFile << "OpenGL Simulation Collision Results" << "\n";
	//Get the current time as well
	time_t now = time(0);
	tm *ltm = localtime(&now);
	std::stringstream hour, min, sec; hour << ltm->tm_hour; min << ltm->tm_min; sec << 1 + ltm->tm_sec;
	writeFile << "Time: " + hour.str() + ":" + min.str() + ":" + sec.str();
	writeFile << "\n" << "\n";
	writeFile.close();
}

void PrintToFile::printDebug(std::string printable, bool printHeader) {
	writeDebugFile.open(nameDebug, std::ios::app);
	if (writeDebugFile.is_open()) {
		if (printHeader) {
			std::stringstream num; num << collisionDebugNum;
			writeDebugFile << "Collision #" << collisionDebugNum << "\n";
			writeDebugFile << printable << "\n";
			collisionDebugNum++;
		}

		else
			writeDebugFile << printable << "\n";
	}
	else
		std::cout << "Cannot open file..." << std::endl;

	writeDebugFile.close();
}

void PrintToFile::clearDebugFile() {
	writeDebugFile.open(nameDebug, std::ios::out);
	writeDebugFile.clear();
	writeDebugFile << "OpenGL Simulation Debug File" << "\n";
	//Get the current time as well
	time_t now = time(0);
	tm *ltm = localtime(&now);
	std::stringstream hour, min, sec; hour << ltm->tm_hour; min << ltm->tm_min; sec << 1 + ltm->tm_sec;
	writeDebugFile << "Time: " + hour.str() + ":" + min.str() + ":" + sec.str();
	writeDebugFile << "\n" << "\n";
	writeDebugFile.close();
}