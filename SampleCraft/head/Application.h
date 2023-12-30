#pragma once
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "ShaderManager.h"
#include "entities/blocks/OakLog.h"
#include "entities/blocks/Stair.h"
#include "entities/blocks/Step.h"
#include "entities/blocks/Sun.h"
#include "render/Renderer.h"
#include "render/TextureManager.h"
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
		void drawLight();
		void drawNormalBlock();
		void drawTransForShadow();
		void drawTrans();
		void drawNormalBlockForShadow();

	private:
		void openGLInit();
		void processInput(GLFWwindow* window, Camera* m_camera);

	private:
		static Application* singleton;
		ShaderManager* shader_manager = nullptr;
		TextureManager* texture_manager = nullptr;
		Renderer* renderer = nullptr;
		GLFWwindow* m_window = nullptr;

		Block* block = nullptr;
		Step* step = nullptr;
		Sun* sun = nullptr;
		OakLog* oak_log = nullptr;
		Stair* stair = nullptr;
	};
}

