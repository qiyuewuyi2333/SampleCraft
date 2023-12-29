#pragma once
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "tools/Camera.h"

namespace SampleCraft
{
	class Application
	{
	public:
		Application();
		~Application();

		static Application* createApplication();
		void init();
		void run();

	private:
		void openGLInit();
		void processInput(GLFWwindow* window, Camera* m_camera);

	private:

		GLFWwindow* m_window = nullptr;
	};
}

