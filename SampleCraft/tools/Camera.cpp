#include "tools/Camera.h"

#include <glm/ext/matrix_transform.hpp>

namespace SampleCraft
{


	void Camera::updateFront()
	{
		// calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		front.y = sin(glm::radians(m_pitch));
		front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		m_front = glm::normalize(front);
		// also re-calculate the Right and Up vector
		m_right = glm::normalize(glm::cross(m_front, m_world_up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		m_up = glm::normalize(glm::cross(m_right, m_front));
	}

	Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
		: m_position(position), m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_world_up(up), m_yaw(yaw),
		  m_pitch(pitch), m_movement_speed(SPEED), m_mouse_sensitivity(SENSITIVITY), m_zoom(ZOOM)
	{
		updateFront();
	}

	glm::mat4 Camera::getLookAt()
	{
		return glm::lookAt(m_position, m_position + m_front, m_up);
	}

	void Camera::processKeyBoard(Camera_movement direction, float delta_time)
	{
		float move = delta_time * m_movement_speed;
		if (direction == FORWARD)
			m_position += m_front * move;
		else if (direction == BACKWARD)
			m_position -= m_front * move;
		else if (direction == LEFT)
			m_position -= m_right * move;
		else if (direction == RIGHT)
			m_position += m_right * move;
		else if (direction == UP)
			m_position += m_world_up * move;
		else if (direction == DOWN)
			m_position -= m_world_up * move;
	}

	void Camera::processMouse(float x_offset, float y_offset, GLboolean constrain_pitch, GLboolean constrain_yaw)
	{
		x_offset *= m_mouse_sensitivity;
		y_offset *= m_mouse_sensitivity;

		m_yaw += x_offset;
		m_pitch += y_offset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrain_pitch)
		{
			if (m_pitch > 89.0f)
				m_pitch = 89.0f;
			if (m_pitch < -89.0f)
				m_pitch = -89.0f;
		}
		if (constrain_yaw)
		{
			if (m_yaw > -1.0f)
				m_yaw = -1.0f;
			if (m_yaw < -179.0f)
				m_yaw = -179.0f;
		}

		// update Front, Right and Up Vectors using the updated Euler angles
		updateFront();
	}

	void Camera::processMouseScroll(float y_offset)
	{
		m_zoom -= (float)y_offset;
		if (m_zoom < 1.0f)
			m_zoom = 1.0f;
		if (m_zoom > 45.0f)
			m_zoom = 45.0f;

	}

	void Camera::setPosition(const glm::vec3& new_position)
	{
		m_position = new_position;
	}
	
}
