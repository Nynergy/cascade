#pragma once

#include <fstream>
#include <string>

class FileProcessor {

private:
	std::string inputFile;
	std::ifstream infile;
	std::string line;

public:
	bool isEOF;

	FileProcessor(std::string inputFileIn);
	std::string poll();

};
