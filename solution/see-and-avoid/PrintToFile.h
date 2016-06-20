#pragma once
#include <fstream>
#include <string>

class PrintToFile
{
public:

	static void print(std::string printable);
	static void clearFile();

private:
	const static std::string name;


};

