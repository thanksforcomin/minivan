#pragma once

#define GLEW_STATIC
#include "renderer.h"

#include <iostream>
#include <map>
#include <functional>
#include <vector>
#include <memory>
#include <string>

#include "cursor.h"

class Controls
{
private: 
	static std::map<int, std::vector<std::function<void()>>> keyboard_press_observer;
	static std::map<int, std::vector<std::function<void()>>> keyboard_down_observer;
	static std::vector<std::function<void(float, float, bool)>> cursor_observer;

	static float m_xpos, m_ypos;
	static float m_lastx, m_lasty;
	static float m_xoffset, m_yoffset;

public:
	Controls();
	Controls(const std::string& filepath);
	~Controls();

	bool getKeyPressed(const int& key);
	bool getKeyReleased(const int& key);

	void bindKeyPress(const int& key, std::function<void()> responce) {
		keyboard_press_observer[key].push_back(responce);
	}

	void bindKeyDown(const int& key, std::function<void()> responce) {
		keyboard_down_observer[key].push_back(responce);
	}
	
	void bindCursor(std::function<void(float, float, bool)>&& responce) {
		cursor_observer.push_back(responce);
	}

	void processFastInput();

private:
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		//std::cout << "Key debug time! I know that your're tired, but ur code is " << glfwGetKey(glfwGetCurrentContext(), key) << "\n";
		if (keyboard_press_observer.find(key) == keyboard_press_observer.end() or glfwGetKey(glfwGetCurrentContext(), key) == GLFW_PRESS) return;
		auto&& observers = keyboard_press_observer.at(key);
		for (auto&& observer : observers)
		{				
			observer();
		}
	}
	static void mouse_position_callback(GLFWwindow* window, double xpos, double ypos) {
		m_xpos = xpos;
		m_ypos = ypos;
		m_xoffset = m_xpos - m_lastx;
		m_yoffset = m_lasty - m_ypos;
		m_lastx = m_xpos;
		m_lasty = m_ypos;
		for (auto&& observer : cursor_observer) {
			observer((float)m_xpos, (float)m_ypos, true);
		}
	}
public:
	inline float getXpos() { return m_xpos; }
	inline float getYpos() { return m_ypos; }
	inline float getXoffset() { return m_xoffset; }
	inline float getYoffset() { return m_yoffset; }
	//inline int debugGetCursorImageId() { return cursor.getImageId(); }
	//void initialize_cursor();
};

