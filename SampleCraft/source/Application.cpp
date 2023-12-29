#include "Application.h"

#include <iostream>
#include "cstdlib"
#include "Log.h"
#include "entities/blocks/Block.h"
#include "entities/blocks/OakLog.h"
#include "entities/blocks/Stair.h"
#include "entities/blocks/Step.h"
#include "entities/blocks/Sun.h"
#include "tools/Camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "render/TextureManager.h"

namespace SampleCraft
{
	Camera* camera = nullptr;
	unsigned int screen_width = 1500;
	unsigned int screen_height = 900;

	float lastX = (float)screen_width / 2.0;
	float lastY = (float)screen_width / 2.0;
	bool firstMouse = true;

	// timing
	float delta_time = 0.0f;
	float last_time = 0.0f;
	float current_time = 0.0f;
	float have_past_time = 0.0f;

	bool is_face_cull = false;
	void mouseCallback(GLFWwindow* window, double xposIn, double yposIn)
	{
		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;

		camera->processMouse(xoffset, yoffset);
	}

	void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		camera->processMouseScroll(static_cast<float>(yoffset));
	}

	void framebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	Application* singleton = nullptr;

	Application::Application()
	{
		std::cout << "App was created successfully!\n";
	}

	Application::~Application()
	{
		delete	camera;
		glfwTerminate();

	}

	Application* Application::createApplication()
	{
		if(singleton == nullptr)
		{
			singleton = new Application();
		}
		return singleton;
	}



	void Application::openGLInit()
	{
		if(!glfwInit())
		{
			std::cerr << "[openGL ERROR] glfwInit break! \n";
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		m_window = glfwCreateWindow(screen_width, screen_height, "SampleCraft", NULL, NULL);
		glfwMakeContextCurrent(m_window);

		glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);
		glfwSetCursorPosCallback(m_window, mouseCallback);
		glfwSetScrollCallback(m_window, scrollCallback);
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return;
		}

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(Log::messageCallback, 0);

		glEnable(GL_DEPTH_TEST);

		if(is_face_cull)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glFrontFace(GL_CW);
		}

		glEnable(GL_BLEND);

	}

	void Application::processInput(GLFWwindow* window, Camera* m_camera)
	{
		
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			m_camera->processKeyBoard(FORWARD, delta_time);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			m_camera->processKeyBoard(BACKWARD, delta_time);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			m_camera->processKeyBoard(LEFT, delta_time);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			m_camera->processKeyBoard(RIGHT, delta_time);
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			m_camera->processKeyBoard(DOWN, delta_time);
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			m_camera->processKeyBoard(UP, delta_time);
	}
	void Application::init()
	{
		camera = new Camera(glm::vec3(0.0f,2.0f,0.0f));
		openGLInit();
	}
	void Application::run()
	{
		Shader normal_block_shader("./shaders/block/normal/normal_block.vert",
			"./shaders/block/normal/normal_block.frag");
		Shader test_shader("./shaders/block/normal/test.vert",
			"./shaders/block/normal/test.frag");
		Shader light_shader("./shaders/block/light_source/light_source.vert",
			"./shaders/block/light_source/light_source.frag");
		Shader log_shader("./shaders/block/log/log.vert",
			"./shaders/block/log/log.frag");
		Shader leaves_shader("./shaders/block/leave/leave.vert",
			"./shaders/block/leave/leave.frag");

		Block block;
		Step step;
		Sun sun;
		OakLog oak_log;
		Stair stair;
		Renderer renderer;
		TextureManager texture_manager;
		
		unsigned snow_texture = texture_manager.loadTexture2D("./resource/textures/blocks/snow/snow.png");
		unsigned oak_planks_texture = texture_manager.loadTexture2D("./resource/textures/blocks/oak_planks/oak_planks.png");
		unsigned oak_log_side_texture = texture_manager.loadTexture2D("./resource/textures/blocks/oak_log/oak_log.png");
		unsigned oak_log_top_texture = texture_manager.loadTexture2D("./resource/textures/blocks/oak_log/oak_log_top.png");
		unsigned leaves_texture = texture_manager.loadTexture2D("./resource/textures/blocks/leaves/spruce_leaves.png");
		texture_manager.bind(GL_TEXTURE_2D, 0, snow_texture);
		texture_manager.bind(GL_TEXTURE_2D, 1, oak_planks_texture);
		texture_manager.bind(GL_TEXTURE_2D, 2, oak_log_side_texture);
		texture_manager.bind(GL_TEXTURE_2D, 3, oak_log_top_texture);
		texture_manager.bind(GL_TEXTURE_2D, 4, leaves_texture);

		test_shader.use();
		test_shader.setVec3("u_light_color", glm::vec3(20.0, 20.0, 18.0));
		log_shader.use();
		log_shader.setVec3("u_light_color", glm::vec3(20.0, 20.0, 18.0));
		leaves_shader.use();
		leaves_shader.setVec3("u_light_color", glm::vec3(20.0, 20.0, 18.0));

		while (!glfwWindowShouldClose(m_window))
		{
			glClearColor(0.52f, 0.80f, 0.98f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			processInput(m_window, camera);
			current_time = glfwGetTime();
			delta_time = current_time - last_time;
			if(last_time == 0.0f)
			{
				delta_time = 0.0f;
			}
			last_time = current_time;
			have_past_time += delta_time;

			glm::mat4 view = camera->getLookAt();
			glm::mat4 projection = glm::perspective(glm::radians(camera->m_zoom), (float)screen_width / (float)screen_height, 0.1f, 100.0f);
			glm::vec3 light_pos = { 0.0f,50.0f,0.0f };

			light_shader.use();
			glm::mat4 light_model = glm::translate(glm::mat4(1.0f), sun.getPosition(have_past_time));
			light_shader.setMat4("view", view);
			light_shader.setMat4("projection", projection);
			light_shader.setMat4("model", light_model);
			renderer.draw(block.getVertexArray(), light_shader);

			test_shader.use();
			test_shader.setInt("u_test_texture", 0);
			test_shader.setMat4("u_projection", projection);
			test_shader.setMat4("u_view", view);
			test_shader.setVec3("u_light_pos", sun.getPosition(have_past_time));
			for(int i = -20;i<=20;++i)
			{
				for(int j = -20;j<=20;++j)
				{
					glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(i, 0, j));
					test_shader.setMat4("u_model", model);
					glm::mat4 tr_in_model = glm::transpose(glm::inverse(model));
					test_shader.setMat4("u_tr_in_model", tr_in_model);
					renderer.draw(block.getVertexArray(), test_shader);
				}
			}

			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 3.0f, 0.0f));
			test_shader.setMat4("u_model", model);
			glm::mat4 tr_in_model = glm::transpose(glm::inverse(model));
			test_shader.setMat4("u_tr_in_model", tr_in_model);
			test_shader.setInt("u_test_texture", 1);
			renderer.draw(step.getVertexArray(), test_shader);

			model = glm::translate(glm::mat4(1.0f), glm::vec3(3, 1, 0));
			test_shader.setMat4("u_model", model);
			tr_in_model = glm::transpose(glm::inverse(model));
			test_shader.setMat4("u_tr_in_model", tr_in_model);
			test_shader.setInt("u_test_texture", 2);
			renderer.draw(oak_log.getVertexArraySide(), test_shader);
			test_shader.setInt("u_test_texture", 3);
			renderer.draw(oak_log.getVertexArrayTop(), test_shader);

			model = glm::translate(glm::mat4(1.0f), glm::vec3(6, 1, 0));
			test_shader.setMat4("u_model", model);
			tr_in_model = glm::transpose(glm::inverse(model));
			test_shader.setMat4("u_tr_in_model", tr_in_model);
			test_shader.setInt("u_test_texture", 1);
			renderer.draw(stair.getVertexArray(), test_shader);

			leaves_shader.use();
			leaves_shader.setMat4("u_projection", projection);
			leaves_shader.setMat4("u_view", view);
			leaves_shader.setVec3("u_light_pos", sun.getPosition(have_past_time));
			leaves_shader.setInt("u_test_texture", 4);
			leaves_shader.setVec3("u_leave_color", glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(glm::mat4(1.0f), glm::vec3(5, 1, 0));
			leaves_shader.setMat4("u_model", model);
			tr_in_model = glm::transpose(glm::inverse(model));
			leaves_shader.setMat4("u_tr_in_model", tr_in_model);
			renderer.draw(block.getVertexArray(), leaves_shader);


			glfwSwapBuffers(m_window);
			glfwPollEvents();
		}
	}

	

	
}
