#include <iostream>

#include <glew.h>
#include <glfw3.h>

#include "libfreenect.hpp"

#include "FileUtils.hpp"
#include "FreenectDevice.hpp"
#include "ShaderProgram.hpp"
#include "Window.hpp"

int main(int argc, const char * argv[]) {
	// Initialize GLFW
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return EXIT_FAILURE;
	}
	
	// Specify OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	
	// Specify core profile/forward compatibility, not doing so can cause GLFW window creation to fail
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	// Create window
	Window window(640, 480, "Kinect");
	
	// Prevents swapping buffers in the middle of a screen update (screen tearing)
	glfwSwapInterval(1);
	
	// Initialize GLEW (must be done after window creation because it needs an OpenGL context)
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return EXIT_FAILURE;
	}
	
	// Initialize Kinect
	Freenect::Freenect freenect;
	FreenectDevice* device = &freenect.createDevice<FreenectDevice>(0);
	device->startVideo();
	device->startDepth();
	device->setLed(LED_BLINK_GREEN);
	int freenectTiltDegrees = 30;
	
	std::vector<uint8_t> videoBuffer(device->videoResolution * 3);
	
	/* Temporary OpenGL stuff */
	
	GLuint vertexBuffer;
	GLuint vertexArray;
	GLuint texture;
	
	float vertices[] = {
		-1.0, -1.0, 0.0, 0.0, 1.0,
		-1.0, 1.0, 0.0, 0.0, 0.0,
		1.0, 1.0, 0.0, 1.0, 0.0,
		
		1.0, 1.0, 0.0, 1.0, 0.0,
		1.0, -1.0, 0.0, 1.0, 1.0,
		-1.0, -1.0, 0.0, 0.0, 1.0
	};
	
	ShaderProgram shaderProgram("Rendering/Shaders/BasicTextureVertexShader.glsl", "Rendering/Shaders/BasicTextureFragmentShader.glsl");
	
	glGenVertexArrays(1, &vertexArray);
	glGenBuffers(1, &vertexBuffer);
	
	glBindVertexArray(vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	float borderColor[] = { 0.0, 0.0, 0.0, 1.0 };
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	/* End temporary OpenGL stuff */
	
	// Main loop
	while (window.isOpen()) {
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		device->setTiltDegrees(freenectTiltDegrees);
		device->updateState();
		
		device->getRGB(videoBuffer);
		
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, &videoBuffer[0]);
		glGenerateMipmap(GL_TEXTURE_2D);
		
		glUseProgram(shaderProgram.getID());
		glBindVertexArray(vertexArray);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		// Present frame
		window.swapBuffers();
		glfwPollEvents();
	}
	
	device->stopVideo();
	device->stopDepth();
	device->setLed(LED_OFF);
	
	glfwTerminate();
	return EXIT_SUCCESS;
}
