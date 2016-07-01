
/*
Static class used for creating log files associate with given output and debugging. 
*/

#pragma once
#include <fstream>
#include <string>
#include <sstream>


class PrintToFile
{
public:

	static void print(std::string printable);
	static void printDebug(std::string printable);
	static void clearFile();
	static void clearDebugFile();

private:
	const static std::string name;
	const static std::string nameDebug;
};

