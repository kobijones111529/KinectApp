#pragma once

#include <fstream>
#include <string>

class FileUtils {
public:
	static std::string readFile(const std::string &fileName);
	
private:
	FileUtils() { }
	~FileUtils() { }
};
