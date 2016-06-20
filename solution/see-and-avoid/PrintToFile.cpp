#include "PrintToFile.h"
#include <string>
#include <iostream>

const std::string PrintToFile::name = "Andy.mylog";
std::ofstream writeFile;

void PrintToFile::print(std::string printable) {
	writeFile.open(name, std::ios::app);
	if (writeFile.is_open()) {
		writeFile << printable << "\n";
	}
	else
		std::cout << "Cannot open file..." << std::endl;
	writeFile.close();
}

void PrintToFile::clearFile() {
	writeFile.open(name, std::ios::out);
	writeFile.clear();
	writeFile.close();
}