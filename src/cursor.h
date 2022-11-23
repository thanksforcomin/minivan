#pragma once

#define GLEW_STATIC
#include "renderer.h"

#include <string>
#include <iostream>

#include "image.h"

class customCursor {
private:
	bool isCursorEnabled;
	image stb_img;
	GLFWimage tex_img;
	GLFWcursor* cursor;
public:
	customCursor(const std::string& filepath, const uint8_t& size = 32, bool isAligned = false);

	~customCursor();

	void toggleCursorOn() { 
		glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
		isCursorEnabled = true;
	}
	void toggleCursorOff() { 
		glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
		isCursorEnabled = false;
	}
	void toggleCursor() {
		//std::cout << glfwGetTime() << std::endl;
		if(isCursorEnabled) glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		isCursorEnabled = !isCursorEnabled;
	}

	inline int getImageId() { return stb_img.m_imageId; }
};
