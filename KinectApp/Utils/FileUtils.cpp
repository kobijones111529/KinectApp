#include "FileUtils.hpp"

std::string FileUtils::readFile(const std::string &fileName) {
	// Open file for reading
	std::ifstream fileStream(fileName, std::ios::in);
	if (!fileStream) {
		fprintf(stderr, "Couldn't open file %s\n", fileName.c_str());
		return "";
	}
	
	std::string contents;
	// Resize string to length of file
	fileStream.seekg(0, std::ios::end);
	contents.resize(fileStream.tellg());
	// Read to string
	fileStream.seekg(0, std::ios::beg);
	fileStream.read(&contents[0], contents.size());
	// Close file
	fileStream.close();
	
	return contents;
}
