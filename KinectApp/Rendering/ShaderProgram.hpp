#pragma once

#include <string>

#include <glew.h>

#include "FileUtils.hpp"

class ShaderProgram {
public:
	ShaderProgram(const std::string &vertexShaderFile, const std::string &fragmentShaderFile);
	~ShaderProgram();
	
	inline GLuint getID() const { return m_programID; }
	
private:
	GLuint m_programID;
	
	static GLuint createShader(GLenum type, const std::string &shaderFile);
	static GLuint linkProgram(GLuint vertexShaderID, GLuint fragmentShaderID);
};
