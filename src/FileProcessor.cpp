#include "FileProcessor.hpp"

FileProcessor::FileProcessor(std::string inputFileIn) : inputFile(inputFileIn) {
	infile.open(inputFile, std::ios::in);

	if(!std::getline(infile, line)) {
		isEOF = true;
	} else {
		isEOF = false;
	}
}

std::string FileProcessor::poll() {
	std::string ret = line;

	if(!std::getline(infile, line)) {
		isEOF = true;
	}

	return ret;
}
