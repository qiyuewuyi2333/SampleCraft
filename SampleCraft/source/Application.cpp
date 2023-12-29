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
	glm::vec4 day_color = { 0.52f, 0.80f, 0.98f, 1.0f };
	glm::vec4 night_color = { 0.074f, 0.274f, 0.584f, 1.0f };

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

	Application* Application::singleton = nullptr;

	Application::Application()
	{
		std::cout << "App was created successfully!\n";
	}

	Application::~Application()
	{
		delete	camera;
		delete	shader_manager;
		delete	texture_manager;
		delete	renderer;
		delete	block;
		delete	step;
		delete	sun;
		delete  oak_log;
		delete  stair;
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
		openGLInit();

		camera = new Camera(glm::vec3(0.0f, 2.0f, 0.0f));
		shader_manager = ShaderManager::createShaderManager();
		texture_manager = TextureManager::createTextureManager();
		renderer = Renderer::createRenderer();
		block = new Block();
		step = new Step();
		sun = new Sun();
		oak_log = new OakLog();
		stair = new Stair();
	}
	void Application::drawLight()
	{
		shader_manager->light_shader->use();
		glm::mat4 view = camera->getLookAt();
		glm::mat4 projection = glm::perspective(glm::radians(camera->m_zoom),
			(float)screen_width / (float)screen_height, 0.1f, 100.0f);
		glm::mat4 model = glm::translate(glm::mat4(1.0f), sun->getPosition(have_past_time));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		shader_manager->light_shader->setMat4("view", view);
		shader_manager->light_shader->setMat4("projection", projection);
		shader_manager->light_shader->setMat4("model", model);
		renderer->draw(block->getVertexArray(), shader_manager->light_shader);
		model = glm::translate(glm::mat4(1.0f), sun->getLunaPosition(have_past_time));
		shader_manager->light_shader->setMat4("model", model);
		renderer->draw(block->getVertexArray(), shader_manager->light_shader);
	}
	void Application::drawNormalBlock()
	{
		shader_manager->normal_shader->use();

		//设置通用矩阵
		glm::mat4 view = camera->getLookAt();
		glm::mat4 projection = glm::perspective(glm::radians(camera->m_zoom),
			(float)screen_width / (float)screen_height, 0.1f, 100.0f);
		shader_manager->normal_shader->setMat4("u_view", view);
		shader_manager->normal_shader->setMat4("u_projection", projection);
		//设置光照
		shader_manager->normal_shader->setVec3("u_light_color", sun->getLightStrength());
		shader_manager->normal_shader->setVec3("u_light_pos", sun->getPosition(have_past_time));
		if(sun->getLunaPosition(have_past_time).y>0.0f)
		{
			shader_manager->normal_shader->setVec3("u_luna_light_color",  sun->getLunaLightStrength());
			shader_manager->normal_shader->setVec3("u_luna_light_pos", sun->getLunaPosition(have_past_time));
		}else
		{
			shader_manager->normal_shader->setVec3("u_luna_light_color", glm::vec3(0.0f,0.0f,0.0f));
			shader_manager->normal_shader->setVec3("u_luna_light_pos", sun->getLunaPosition(have_past_time));
		}
		//draw snow floor
		//设置纹理
		shader_manager->normal_shader->setInt("u_texture", texture_manager->snow_texture);
		for(int i = -20;i<=20;++i)
		{
			for(int j = -20;j<=20;j++)
			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(i, 0.0f, j));
				shader_manager->normal_shader->setMat4("u_model", model);
				//求 转置矩阵的逆矩阵
				shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model)));
				renderer->draw(block->getVertexArray(), shader_manager->normal_shader);
			}
		}

		//draw the tree's log(main body)
		//设置纹理 先边后顶
		for(int j = 1;j<=4;++j)
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, j, 0.0f));
			shader_manager->normal_shader->setMat4("u_model", model);
			shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model)));
			shader_manager->normal_shader->setInt("u_texture", texture_manager->oak_log_side_texture);
			renderer->draw(oak_log->getVertexArraySide(), shader_manager->normal_shader);
			shader_manager->normal_shader->setInt("u_texture", texture_manager->oak_log_top_texture);
			renderer->draw(oak_log->getVertexArrayTop(), shader_manager->normal_shader);
		}


	}


	void Application::drawTrans()
	{

		shader_manager->leaves_shader->use();
		shader_manager->leaves_shader->setVec3("u_light_color", glm::vec3(20.0, 20.0, 18.0));

	}

	void Application::run()
	{
		glm::vec3 light_pos = { 0.0f,50.0f,0.0f };

		while (!glfwWindowShouldClose(m_window))
		{
			if(sun->getPosition(have_past_time).y>0.0f)
			{
				renderer->clear(day_color);
			}
			else
			{
				renderer->clear(night_color);
			}
			processInput(m_window, camera);
			current_time = glfwGetTime();
			delta_time = current_time - last_time;
			if(last_time == 0.0f)
			{
				delta_time = 0.0f;
			}
			last_time = current_time;
			have_past_time += delta_time;
			std::cout <<"\n x: " <<camera->m_position.x
					  <<"\n y: "<< camera->m_position.y
					  <<"\n z: "<< camera->m_position.z;
			
			drawLight();
			drawNormalBlock();
			drawTrans();

			glfwSwapBuffers(m_window);
			glfwPollEvents();
		}
	}


}
