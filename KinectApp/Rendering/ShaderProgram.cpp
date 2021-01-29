#include "ShaderProgram.hpp"

ShaderProgram::ShaderProgram(const std::string &vertexShaderFile, const std::string &fragmentShaderFile) {
	// Create shaders
	GLuint vertexShaderID = createShader(GL_VERTEX_SHADER, vertexShaderFile);
	GLuint fragmentShaderID = createShader(GL_FRAGMENT_SHADER, fragmentShaderFile);
	// Link program
	m_programID = linkProgram(vertexShaderID, fragmentShaderID);
	// Shaders can (and should) be deleted after linking
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

ShaderProgram::~ShaderProgram() {
	// Cleanup
	glDeleteProgram(m_programID);
}

GLuint ShaderProgram::createShader(GLenum type, const std::string &shaderFile) {
	GLuint id = glCreateShader(type);
	
	// Compile shader from source file
	std::string shaderSourceString = FileUtils::readFile(shaderFile);
	const char *shaderSource = shaderSourceString.c_str();
	glShaderSource(id, 1, &shaderSource, NULL);
	glCompileShader(id);
	
	// Check for compile errors and return
	int success;
	char infoLog[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		fprintf(stderr, "Failed to compile shader \"%s\": %s\n", shaderFile.c_str(), infoLog);
		return NULL;
	}
	
	return id;
}

GLuint ShaderProgram::linkProgram(GLuint vertexShaderID, GLuint fragmentShaderID) {
	GLuint id = glCreateProgram();
	
	// Attatch shaders and link
	glAttachShader(id, vertexShaderID);
	glAttachShader(id, fragmentShaderID);
	glLinkProgram(id);
	
	// Check for link errors and return
	int success;
	char infoLog[512];
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		fprintf(stderr, "Failed to link program: %s\n", infoLog);
		return NULL;
	}
	
	return id;
}
