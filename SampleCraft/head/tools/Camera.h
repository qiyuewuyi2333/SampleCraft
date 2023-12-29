#pragma once
#include <glm/glm.hpp>
typedef unsigned char GLboolean;

namespace SampleCraft
{


	enum Camera_movement
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN,
	};

	constexpr float YAW = -90.0f;
	constexpr float PITCH = 0.0f;
	constexpr float SPEED = 2.5f;
	constexpr float SENSITIVITY = 0.1f;
	constexpr float ZOOM = 45.0f;

	class Camera
	{
	public:
		glm::vec3 m_position;
		glm::vec3 m_front;
		glm::vec3 m_up;
		glm::vec3 m_world_up;
		glm::vec3 m_right;

		float m_yaw;
		float m_pitch;

		float m_movement_speed;
		float m_mouse_sensitivity;
		float m_zoom;

	public:
		void updateFront();
		Camera(glm::vec3 position = { 0.0f, 0.0f, 0.0f }, glm::vec3 up = { 0.0f, 1.0f, 0.0f }, float yaw = YAW,
			float pitch = PITCH);
		glm::mat4 getLookAt();
		void processKeyBoard(Camera_movement camera_movement, float delta_time);
		void processMouse(float x_offset, float y_offset, GLboolean constrain_y = true, GLboolean constrain_x = false);
		void processMouseScroll(float y_offset);

		void setPosition(const glm::vec3& new_position);
	};
}
