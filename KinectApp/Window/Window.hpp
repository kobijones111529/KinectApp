#pragma once

#include <iostream>

#include <glfw3.h>

struct DestroyGLFWWindow {
	void operator()(GLFWwindow *ptr) {
		glfwDestroyWindow(ptr);
	}
};

class Window {
public:
	Window(int width, int height, const char *title);
	~Window();
	
	inline void swapBuffers() const { glfwSwapBuffers(m_Window.get()); }
	inline bool isOpen() const { return !glfwWindowShouldClose(m_Window.get()); }
	
private:
	std::unique_ptr<GLFWwindow, DestroyGLFWWindow> m_Window;
};
