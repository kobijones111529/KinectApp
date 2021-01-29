#include "Window.hpp"

Window::Window(int width, int height, const char *title) {
	m_Window = std::unique_ptr<GLFWwindow, DestroyGLFWWindow>(glfwCreateWindow(width, height, title, NULL, NULL));
	if (!m_Window) {
		fprintf(stderr, "Failed to create GLFW window\n");
		return;
	}
	
	glfwMakeContextCurrent(m_Window.get());
}

Window::~Window() { }
