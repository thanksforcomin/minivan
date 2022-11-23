#pragma once

#define GLEW_STATIC
#include "renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class TEST_cube
{
private:
	glm::mat4 m_model;
	glm::vec3 m_pos;
	float m_yaw, m_pitch;
	float m_lastx, m_lasty;
	//bool first_mouse;
	float mouse_sensitivity;
public:
	TEST_cube(glm::vec3 position = glm::vec3(0.0, 0.0, 0.0)) :
		m_yaw(-90.0f),
		m_pitch(0.0f),
		m_lastx(360),
		m_lasty(270),
		mouse_sensitivity(0.1f),
		m_pos(position)
	{
		m_model = glm::mat4(1.0f);
		m_model = glm::translate(m_model, position);
	}
	~TEST_cube() {

	}

	void resize(Vector3 parameter) {
		m_model = glm::scale(m_model, parameter);
	}

	void rotate(Vector3 rotation, float angle) {
		m_model = glm::rotate(m_model, angle, rotation);
	}

	void mouse_callback(float xpos, float ypos, bool placeholder) {
		m_yaw += (xpos - m_lastx) * mouse_sensitivity;
		m_pitch += (m_lasty - ypos) * mouse_sensitivity;

		m_lastx = xpos;
		m_lasty = ypos;

		glm::vec3 front;
		front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		front.y = sin(glm::radians(m_pitch));
		front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

		m_model = glm::lookAt(
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.0, 0.0, 0.0) + glm::normalize(front),
			glm::normalize(
				glm::cross(
					glm::normalize(
						glm::cross(
							glm::normalize(front), glm::vec3(0.0, 1.0, 0.0))), 
					glm::normalize(front)))
			);
	}
	glm::mat4 getModel() {
		return m_model;
	}
	glm::vec3 position() { return m_pos; }
	void set_position(glm::vec3 position) {
		m_model = glm::translate(glm::mat4(1.0), position);
	}
	void move(glm::vec3 direction) {
		m_model = glm::translate(m_model, direction);
	}
};

