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
	const unsigned int SCR_WIDTH = 1500;
	const unsigned int SCR_HEIGHT = 900;

	float lastX = (float)SCR_WIDTH / 2.0;
	float lastY = (float)SCR_HEIGHT / 2.0;
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


	//初始化OpenGL相关设置
	void Application::openGLInit()
	{
		if(!glfwInit())
		{
			std::cerr << "[openGL ERROR] glfwInit break! \n";
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		m_window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "SampleCraft", NULL, NULL);
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
	//绘制发光物体
	void Application::drawLight()
	{
		shader_manager->light_shader->use();
		glm::mat4 view = camera->getLookAt();
		glm::mat4 projection = glm::perspective(glm::radians(camera->m_zoom),
			(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
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
	//绘制不透明物体
	void Application::drawNormalBlock()
	{
		shader_manager->normal_shader->use();

		//设置通用矩阵
		glm::mat4 view = camera->getLookAt();
		glm::mat4 projection = glm::perspective(glm::radians(camera->m_zoom),
			(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
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
			for(int j = -20;j<=40;j++)
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


		// 木板
		for (int i = -14; i <= 4; i++) {
			for (int j = 5; j <= 18; j++) {
				shader_manager->normal_shader->setInt("u_texture", texture_manager->oak_planks_texture);
				glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(i, 1, j));

				shader_manager->normal_shader->setMat4("u_model", model);
				shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model)));
				renderer->draw(step->getVertexArray(), shader_manager->normal_shader);
			}
		}
		
		//draw the stairs of the eaves
		for(int j = 6;j<=11;++j)
		{
			int k = 8 + j-6;
			for(int i = 5;i>=-15;--i)
			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(i, j, k)) * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				shader_manager->normal_shader->setMat4("u_model", model);
				shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model)));
				renderer->draw(stair->getVertexArray(), shader_manager->normal_shader);
			}
		}
		for (int j = 6; j <= 11; ++j)
		{
			int k = 19 - j + 6;
			for (int i = 5; i >= -15; --i)
			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(i, j, k));
				shader_manager->normal_shader->setMat4("u_model", model);
				shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model)));
				renderer->draw(stair->getVertexArray(), shader_manager->normal_shader);
			}
		}
		//draw the plank of the eaves
		shader_manager->normal_shader->setInt("u_texture", texture_manager->stripped_birch_log_texture);
		for (int j = 6; j <= 10; ++j)
		{
			for (int k = 9 - 6 + j; k <= 18 + 6 - j; ++k)
			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(3, j, k));
				shader_manager->normal_shader->setMat4("u_model", model);
				shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model)));
				renderer->draw(block->getVertexArray(), shader_manager->normal_shader);
				model = glm::translate(glm::mat4(1.0f), glm::vec3(-13, j, k));
				shader_manager->normal_shader->setMat4("u_model", model);
				shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model)));
				renderer->draw(block->getVertexArray(), shader_manager->normal_shader);
			}
		}
		//draw the plank step on the top front of the house
		for(int i = 4;i>=-14;--i)
		{
			for(int k = 5 ;k<=8;++k)
			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(i, 6, k));
				shader_manager->normal_shader->setMat4("u_model", model);
				shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model)));
				renderer->draw(step->getVertexArray(), shader_manager->normal_shader);
			}
		}
		// 从下往上 第一层柱子 4棵
		for (int i = 0; i < 4; i++)
		{
			for (int j = 1; j <= 5; j++) 
			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(4 - i * 6, j, 5));
				shader_manager->normal_shader->setMat4("u_model", model);
				shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model)));
				shader_manager->normal_shader->setInt("u_texture", texture_manager->oak_log_side_texture);
				renderer->draw(oak_log->getVertexArraySide(), shader_manager->normal_shader);
				shader_manager->normal_shader->setInt("u_texture", texture_manager->oak_log_top_texture);
				renderer->draw(oak_log->getVertexArrayTop(), shader_manager->normal_shader);
			}
		}

		// 从下往上 第一层柱子 2棵
		for (int j = 1; j <= 5; j++) {
			glm::mat4 model1 = glm::translate(glm::mat4(1.0f), glm::vec3(4, j, 8));
			shader_manager->normal_shader->setMat4("u_model", model1);
			shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model1)));
			shader_manager->normal_shader->setInt("u_texture", texture_manager->oak_log_side_texture);
			renderer->draw(oak_log->getVertexArraySide(), shader_manager->normal_shader);
			shader_manager->normal_shader->setInt("u_texture", texture_manager->oak_log_top_texture);
			renderer->draw(oak_log->getVertexArrayTop(), shader_manager->normal_shader);

			glm::mat4 model2 = glm::translate(glm::mat4(1.0f), glm::vec3(-14, j, 8));
			shader_manager->normal_shader->setMat4("u_model", model2);
			shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model2)));
			shader_manager->normal_shader->setInt("u_texture", texture_manager->oak_log_side_texture);
			renderer->draw(oak_log->getVertexArraySide(), shader_manager->normal_shader);
			shader_manager->normal_shader->setInt("u_texture", texture_manager->oak_log_top_texture);
			renderer->draw(oak_log->getVertexArrayTop(), shader_manager->normal_shader);
		}

		// 从下往上 第三层柱子 3棵
		for (int j = 1; j <= 5; j++) {
			glm::mat4 model1 = glm::translate(glm::mat4(1.0f), glm::vec3(4, j, 19));
			shader_manager->normal_shader->setMat4("u_model", model1);
			shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model1)));
			shader_manager->normal_shader->setInt("u_texture", texture_manager->oak_log_side_texture);
			renderer->draw(oak_log->getVertexArraySide(), shader_manager->normal_shader);
			shader_manager->normal_shader->setInt("u_texture", texture_manager->oak_log_top_texture);
			renderer->draw(oak_log->getVertexArrayTop(), shader_manager->normal_shader);

			glm::mat4 model2 = glm::translate(glm::mat4(1.0f), glm::vec3(-8, j, 19));
			shader_manager->normal_shader->setMat4("u_model", model2);
			shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model2)));
			shader_manager->normal_shader->setInt("u_texture", texture_manager->oak_log_side_texture);
			renderer->draw(oak_log->getVertexArraySide(), shader_manager->normal_shader);
			shader_manager->normal_shader->setInt("u_texture", texture_manager->oak_log_top_texture);
			renderer->draw(oak_log->getVertexArrayTop(), shader_manager->normal_shader);

			glm::mat4 model3 = glm::translate(glm::mat4(1.0f), glm::vec3(-14, j, 19));
			shader_manager->normal_shader->setMat4("u_model", model3);
			shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model3)));
			shader_manager->normal_shader->setInt("u_texture", texture_manager->oak_log_side_texture);
			renderer->draw(oak_log->getVertexArraySide(), shader_manager->normal_shader);
			shader_manager->normal_shader->setInt("u_texture", texture_manager->oak_log_top_texture);
			renderer->draw(oak_log->getVertexArrayTop(), shader_manager->normal_shader);
		}

		// 外围 从左下到右下
		for (int i = -13; i <= 3; i++) {
			if (i == -12 || i == -11 || i == -10) {
				continue;
			}
			for (int j = 1; j <= 5; j++) {
				if (i<2 && i>-5 && 2<j&& j <5)
				{
					continue;
				}

				glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(i, j, 9));
				shader_manager->normal_shader->setMat4("u_model", model);
				shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model)));
				shader_manager->normal_shader->setInt("u_texture", texture_manager->stripped_birch_log_texture);
				renderer->draw(oak_log->getVertexArraySide(), shader_manager->normal_shader);
				shader_manager->normal_shader->setInt("u_texture", texture_manager->stripped_birch_log_top_texture);
				renderer->draw(oak_log->getVertexArrayTop(), shader_manager->normal_shader);
			}
		}

		// 外围 从左下到左上
		for (int i = 9; i <= 18; i++) {
			for (int j = 1; j <= 5; j++) {
				glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(3, j, i));
				shader_manager->normal_shader->setMat4("u_model", model);
				shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model)));
				shader_manager->normal_shader->setInt("u_texture", texture_manager->stripped_birch_log_texture);
				renderer->draw(oak_log->getVertexArraySide(), shader_manager->normal_shader);
				shader_manager->normal_shader->setInt("u_texture", texture_manager->stripped_birch_log_top_texture);
				renderer->draw(oak_log->getVertexArrayTop(), shader_manager->normal_shader);
			}
		}

		// 外围 从右下到右上
		for (int i = 9; i <= 18; i++) {
			for (int j = 1; j <= 5; j++) {
				glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-13, j, i));
				shader_manager->normal_shader->setMat4("u_model", model);
				shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model)));
				shader_manager->normal_shader->setInt("u_texture", texture_manager->stripped_birch_log_texture);
				renderer->draw(oak_log->getVertexArraySide(), shader_manager->normal_shader);
				shader_manager->normal_shader->setInt("u_texture", texture_manager->stripped_birch_log_top_texture);
				renderer->draw(oak_log->getVertexArrayTop(), shader_manager->normal_shader);
			}
		}

		// 外围 从左上到右上
		for (int i = -13; i <= 3; i++) {
			for (int j = 1; j <= 5; j++) {
				glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(i, j, 18));
				shader_manager->normal_shader->setMat4("u_model", model);
				shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model)));
				shader_manager->normal_shader->setInt("u_texture", texture_manager->stripped_birch_log_texture);
				renderer->draw(oak_log->getVertexArraySide(), shader_manager->normal_shader);
				shader_manager->normal_shader->setInt("u_texture", texture_manager->stripped_birch_log_top_texture);
				renderer->draw(oak_log->getVertexArrayTop(), shader_manager->normal_shader);
			}
		}


		// 左上角
		shader_manager->normal_shader->setInt("u_texture", texture_manager->oak_planks_texture);
		glm::mat4 model5 = glm::translate(glm::mat4(1.0f), glm::vec3(4, 1, 29));

		shader_manager->normal_shader->setMat4("u_model", model5);
		shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model5)));
		renderer->draw(step->getVertexArray(), shader_manager->normal_shader);

		// 右上角
		shader_manager->normal_shader->setInt("u_texture", texture_manager->oak_planks_texture);
		glm::mat4 model6 = glm::translate(glm::mat4(1.0f), glm::vec3(-14, 1, 29));

		shader_manager->normal_shader->setMat4("u_model", model6);
		shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model6)));
		renderer->draw(step->getVertexArray(), shader_manager->normal_shader);

	}
	//绘制透明物体
	void Application::drawTrans()
	{

		shader_manager->trans_shader->use();
		shader_manager->trans_shader->setVec3("u_light_color", glm::vec3(20.0, 20.0, 18.0));
		shader_manager->trans_shader->use();

		//设置通用矩阵
		glm::mat4 view = camera->getLookAt();
		glm::mat4 projection = glm::perspective(glm::radians(camera->m_zoom),
			(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		shader_manager->trans_shader->setMat4("u_view", view);
		shader_manager->trans_shader->setMat4("u_projection", projection);
		//设置光照
		shader_manager->trans_shader->setVec3("u_light_color", sun->getLightStrength());
		shader_manager->trans_shader->setVec3("u_light_pos", sun->getPosition(have_past_time));
		if (sun->getLunaPosition(have_past_time).y > 0.0f)
		{
			shader_manager->trans_shader->setVec3("u_luna_light_color", sun->getLunaLightStrength());
			shader_manager->trans_shader->setVec3("u_luna_light_pos", sun->getLunaPosition(have_past_time));
		}
		else
		{
			shader_manager->trans_shader->setVec3("u_luna_light_color", glm::vec3(0.0f, 0.0f, 0.0f));
			shader_manager->trans_shader->setVec3("u_luna_light_pos", sun->getLunaPosition(have_past_time));
		}

		// draw the tree's leaves
		shader_manager->trans_shader->setInt("u_texture", texture_manager->leaves_texture);
		shader_manager->trans_shader->setVec4("u_texture_color", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

		for(int k = 2;k<=5;++k)
		{
			int i = k-5;
			for(;i<=5-k;++i)
			{
				int j = k-5;
				for(;j<=5-k;++j)
				{
					glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(i, k, j));
					shader_manager->trans_shader->setMat4("u_model", model);
					shader_manager->trans_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model)));
					renderer->draw(block->getVertexArray(), shader_manager->trans_shader);
				}
			}

		}

		//draw the glass of the house
		shader_manager->trans_shader->setInt("u_texture", texture_manager->glass_texture);
		shader_manager->trans_shader->setVec4("u_texture_color", 
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		for(int i  = -4;i<2;++i)
		{
			for(int j = 3;j<5;++j)
			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(i, j, 9));
				shader_manager->trans_shader->setMat4("u_model", model);
				shader_manager->trans_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model)));
				renderer->draw(block->getVertexArray(), shader_manager->trans_shader);
			}
		}
	}

	//绘制不透明物体
	void Application::drawNormalBlockForShadow()
	{
		shader_manager->normal_shader->use();

		//设置通用矩阵
		glm::mat4 view = camera->getLookAt();
		glm::mat4 projection = glm::perspective(glm::radians(camera->m_zoom),
			(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		shader_manager->normal_shader->setMat4("u_view", view);
		shader_manager->normal_shader->setMat4("u_projection", projection);
		//设置光照
		shader_manager->normal_shader->setVec3("u_light_color", sun->getLightStrength());
		shader_manager->normal_shader->setVec3("u_light_pos", sun->getPosition(have_past_time));
		if (sun->getLunaPosition(have_past_time).y > 0.0f)
		{
			shader_manager->normal_shader->setVec3("u_luna_light_color", sun->getLunaLightStrength());
			shader_manager->normal_shader->setVec3("u_luna_light_pos", sun->getLunaPosition(have_past_time));
		}
		else
		{
			shader_manager->normal_shader->setVec3("u_luna_light_color", glm::vec3(0.0f, 0.0f, 0.0f));
			shader_manager->normal_shader->setVec3("u_luna_light_pos", sun->getLunaPosition(have_past_time));
		}
		//draw snow floor
		//设置纹理
		shader_manager->normal_shader->setInt("u_texture", texture_manager->snow_texture);
		for (int i = -20; i <= 20; ++i)
		{
			for (int j = -20; j <= 40; j++)
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
		for (int j = 1; j <= 4; ++j)
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, j, 0.0f));
			shader_manager->normal_shader->setMat4("u_model", model);
			shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model)));
			shader_manager->normal_shader->setInt("u_texture", texture_manager->oak_log_side_texture);
			renderer->draw(oak_log->getVertexArraySide(), shader_manager->normal_shader);
			shader_manager->normal_shader->setInt("u_texture", texture_manager->oak_log_top_texture);
			renderer->draw(oak_log->getVertexArrayTop(), shader_manager->normal_shader);
		}


		// 木板
		for (int i = -14; i <= 4; i++) {
			for (int j = 5; j <= 18; j++) {
				shader_manager->normal_shader->setInt("u_texture", texture_manager->oak_planks_texture);
				glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(i, 1, j));

				shader_manager->normal_shader->setMat4("u_model", model);
				shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model)));
				renderer->draw(step->getVertexArray(), shader_manager->normal_shader);
			}
		}

		//draw the stairs of the eaves
		for (int j = 6; j <= 11; ++j)
		{
			int k = 8 + j - 6;
			for (int i = 5; i >= -15; --i)
			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(i, j, k)) * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				shader_manager->normal_shader->setMat4("u_model", model);
				shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model)));
				renderer->draw(stair->getVertexArray(), shader_manager->normal_shader);
			}
		}
		for (int j = 6; j <= 11; ++j)
		{
			int k = 19 - j + 6;
			for (int i = 5; i >= -15; --i)
			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(i, j, k));
				shader_manager->normal_shader->setMat4("u_model", model);
				shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model)));
				renderer->draw(stair->getVertexArray(), shader_manager->normal_shader);
			}
		}
		//draw the plank of the eaves
		shader_manager->normal_shader->setInt("u_texture", texture_manager->stripped_birch_log_texture);
		for (int j = 6; j <= 10; ++j)
		{
			for (int k = 9 - 6 + j; k <= 18 + 6 - j; ++k)
			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(3, j, k));
				shader_manager->normal_shader->setMat4("u_model", model);
				shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model)));
				renderer->draw(block->getVertexArray(), shader_manager->normal_shader);
				model = glm::translate(glm::mat4(1.0f), glm::vec3(-13, j, k));
				shader_manager->normal_shader->setMat4("u_model", model);
				shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model)));
				renderer->draw(block->getVertexArray(), shader_manager->normal_shader);
			}
		}
		//draw the plank step on the top front of the house
		for (int i = 4; i >= -14; --i)
		{
			for (int k = 5; k <= 8; ++k)
			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(i, 6, k));
				shader_manager->normal_shader->setMat4("u_model", model);
				shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model)));
				renderer->draw(step->getVertexArray(), shader_manager->normal_shader);
			}
		}
		// 从下往上 第一层柱子 4棵
		for (int i = 0; i < 4; i++)
		{
			for (int j = 1; j <= 5; j++)
			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(4 - i * 6, j, 5));
				shader_manager->normal_shader->setMat4("u_model", model);
				shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model)));
				shader_manager->normal_shader->setInt("u_texture", texture_manager->oak_log_side_texture);
				renderer->draw(oak_log->getVertexArraySide(), shader_manager->normal_shader);
				shader_manager->normal_shader->setInt("u_texture", texture_manager->oak_log_top_texture);
				renderer->draw(oak_log->getVertexArrayTop(), shader_manager->normal_shader);
			}
		}

		// 从下往上 第一层柱子 2棵
		for (int j = 1; j <= 5; j++) {
			glm::mat4 model1 = glm::translate(glm::mat4(1.0f), glm::vec3(4, j, 8));
			shader_manager->normal_shader->setMat4("u_model", model1);
			shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model1)));
			shader_manager->normal_shader->setInt("u_texture", texture_manager->oak_log_side_texture);
			renderer->draw(oak_log->getVertexArraySide(), shader_manager->normal_shader);
			shader_manager->normal_shader->setInt("u_texture", texture_manager->oak_log_top_texture);
			renderer->draw(oak_log->getVertexArrayTop(), shader_manager->normal_shader);

			glm::mat4 model2 = glm::translate(glm::mat4(1.0f), glm::vec3(-14, j, 8));
			shader_manager->normal_shader->setMat4("u_model", model2);
			shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model2)));
			shader_manager->normal_shader->setInt("u_texture", texture_manager->oak_log_side_texture);
			renderer->draw(oak_log->getVertexArraySide(), shader_manager->normal_shader);
			shader_manager->normal_shader->setInt("u_texture", texture_manager->oak_log_top_texture);
			renderer->draw(oak_log->getVertexArrayTop(), shader_manager->normal_shader);
		}

		// 从下往上 第三层柱子 3棵
		for (int j = 1; j <= 5; j++) {
			glm::mat4 model1 = glm::translate(glm::mat4(1.0f), glm::vec3(4, j, 19));
			shader_manager->normal_shader->setMat4("u_model", model1);
			shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model1)));
			shader_manager->normal_shader->setInt("u_texture", texture_manager->oak_log_side_texture);
			renderer->draw(oak_log->getVertexArraySide(), shader_manager->normal_shader);
			shader_manager->normal_shader->setInt("u_texture", texture_manager->oak_log_top_texture);
			renderer->draw(oak_log->getVertexArrayTop(), shader_manager->normal_shader);

			glm::mat4 model2 = glm::translate(glm::mat4(1.0f), glm::vec3(-8, j, 19));
			shader_manager->normal_shader->setMat4("u_model", model2);
			shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model2)));
			shader_manager->normal_shader->setInt("u_texture", texture_manager->oak_log_side_texture);
			renderer->draw(oak_log->getVertexArraySide(), shader_manager->normal_shader);
			shader_manager->normal_shader->setInt("u_texture", texture_manager->oak_log_top_texture);
			renderer->draw(oak_log->getVertexArrayTop(), shader_manager->normal_shader);

			glm::mat4 model3 = glm::translate(glm::mat4(1.0f), glm::vec3(-14, j, 19));
			shader_manager->normal_shader->setMat4("u_model", model3);
			shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model3)));
			shader_manager->normal_shader->setInt("u_texture", texture_manager->oak_log_side_texture);
			renderer->draw(oak_log->getVertexArraySide(), shader_manager->normal_shader);
			shader_manager->normal_shader->setInt("u_texture", texture_manager->oak_log_top_texture);
			renderer->draw(oak_log->getVertexArrayTop(), shader_manager->normal_shader);
		}

		// 外围 从左下到右下
		for (int i = -13; i <= 3; i++) {
			if (i == -12 || i == -11 || i == -10) {
				continue;
			}
			for (int j = 1; j <= 5; j++) {
				if (i<2 && i>-5 && 2 < j && j < 5)
				{
					continue;
				}

				glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(i, j, 9));
				shader_manager->normal_shader->setMat4("u_model", model);
				shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model)));
				shader_manager->normal_shader->setInt("u_texture", texture_manager->stripped_birch_log_texture);
				renderer->draw(oak_log->getVertexArraySide(), shader_manager->normal_shader);
				shader_manager->normal_shader->setInt("u_texture", texture_manager->stripped_birch_log_top_texture);
				renderer->draw(oak_log->getVertexArrayTop(), shader_manager->normal_shader);
			}
		}

		// 外围 从左下到左上
		for (int i = 9; i <= 18; i++) {
			for (int j = 1; j <= 5; j++) {
				glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(3, j, i));
				shader_manager->normal_shader->setMat4("u_model", model);
				shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model)));
				shader_manager->normal_shader->setInt("u_texture", texture_manager->stripped_birch_log_texture);
				renderer->draw(oak_log->getVertexArraySide(), shader_manager->normal_shader);
				shader_manager->normal_shader->setInt("u_texture", texture_manager->stripped_birch_log_top_texture);
				renderer->draw(oak_log->getVertexArrayTop(), shader_manager->normal_shader);
			}
		}

		// 外围 从右下到右上
		for (int i = 9; i <= 18; i++) {
			for (int j = 1; j <= 5; j++) {
				glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-13, j, i));
				shader_manager->normal_shader->setMat4("u_model", model);
				shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model)));
				shader_manager->normal_shader->setInt("u_texture", texture_manager->stripped_birch_log_texture);
				renderer->draw(oak_log->getVertexArraySide(), shader_manager->normal_shader);
				shader_manager->normal_shader->setInt("u_texture", texture_manager->stripped_birch_log_top_texture);
				renderer->draw(oak_log->getVertexArrayTop(), shader_manager->normal_shader);
			}
		}

		// 外围 从左上到右上
		for (int i = -13; i <= 3; i++) {
			for (int j = 1; j <= 5; j++) {
				glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(i, j, 18));
				shader_manager->normal_shader->setMat4("u_model", model);
				shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model)));
				shader_manager->normal_shader->setInt("u_texture", texture_manager->stripped_birch_log_texture);
				renderer->draw(oak_log->getVertexArraySide(), shader_manager->normal_shader);
				shader_manager->normal_shader->setInt("u_texture", texture_manager->stripped_birch_log_top_texture);
				renderer->draw(oak_log->getVertexArrayTop(), shader_manager->normal_shader);
			}
		}


		// 左上角
		shader_manager->normal_shader->setInt("u_texture", texture_manager->oak_planks_texture);
		glm::mat4 model5 = glm::translate(glm::mat4(1.0f), glm::vec3(4, 1, 29));

		shader_manager->normal_shader->setMat4("u_model", model5);
		shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model5)));
		renderer->draw(step->getVertexArray(), shader_manager->normal_shader);

		// 右上角
		shader_manager->normal_shader->setInt("u_texture", texture_manager->oak_planks_texture);
		glm::mat4 model6 = glm::translate(glm::mat4(1.0f), glm::vec3(-14, 1, 29));

		shader_manager->normal_shader->setMat4("u_model", model6);
		shader_manager->normal_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model6)));
		renderer->draw(step->getVertexArray(), shader_manager->normal_shader);

	}
	//绘制透明物体
	void Application::drawTransForShadow()
	{

		shader_manager->trans_shader->use();
		shader_manager->trans_shader->setVec3("u_light_color", glm::vec3(20.0, 20.0, 18.0));
		shader_manager->trans_shader->use();

		//设置通用矩阵
		glm::mat4 view = camera->getLookAt();
		glm::mat4 projection = glm::perspective(glm::radians(camera->m_zoom),
			(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		shader_manager->trans_shader->setMat4("u_view", view);
		shader_manager->trans_shader->setMat4("u_projection", projection);
		//设置光照
		shader_manager->trans_shader->setVec3("u_light_color", sun->getLightStrength());
		shader_manager->trans_shader->setVec3("u_light_pos", sun->getPosition(have_past_time));
		if (sun->getLunaPosition(have_past_time).y > 0.0f)
		{
			shader_manager->trans_shader->setVec3("u_luna_light_color", sun->getLunaLightStrength());
			shader_manager->trans_shader->setVec3("u_luna_light_pos", sun->getLunaPosition(have_past_time));
		}
		else
		{
			shader_manager->trans_shader->setVec3("u_luna_light_color", glm::vec3(0.0f, 0.0f, 0.0f));
			shader_manager->trans_shader->setVec3("u_luna_light_pos", sun->getLunaPosition(have_past_time));
		}

		// draw the tree's leaves
		shader_manager->trans_shader->setInt("u_texture", texture_manager->leaves_texture);
		shader_manager->trans_shader->setVec4("u_texture_color", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

		for (int k = 2; k <= 5; ++k)
		{
			int i = k - 5;
			for (; i <= 5 - k; ++i)
			{
				int j = k - 5;
				for (; j <= 5 - k; ++j)
				{
					glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(i, k, j));
					shader_manager->trans_shader->setMat4("u_model", model);
					shader_manager->trans_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model)));
					renderer->draw(block->getVertexArray(), shader_manager->trans_shader);
				}
			}

		}

		//draw the glass of the house
		shader_manager->trans_shader->setInt("u_texture", texture_manager->glass_texture);
		shader_manager->trans_shader->setVec4("u_texture_color",
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		for (int i = -4; i < 2; ++i)
		{
			for (int j = 3; j < 5; ++j)
			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(i, j, 9));
				shader_manager->trans_shader->setMat4("u_model", model);
				shader_manager->trans_shader->setMat4("u_tr_in_model", glm::inverse(glm::transpose(model)));
				renderer->draw(block->getVertexArray(), shader_manager->trans_shader);
			}
		}
	}
	void Application::run()
	{
		glm::vec3 light_pos = { 0.0f,50.0f,0.0f };
		glm::vec3 last_camera_pos = camera->m_position;

		GLuint depth_map_fbo;
		glGenFramebuffers(1, &depth_map_fbo);
		//创建深度缓冲纹理
		//把生成的深度纹理作为帧缓冲的深度缓冲
		glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture_manager->shadow_map, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glm::mat4 light_proj;
		glm::mat4 light_view;
		glm::mat4 lightSpaceMatrix;
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

			//print the camera's coords in the console
			if (last_camera_pos != camera->m_position)
			{
				std::cout << "\n x: " << (int)camera->m_position.x
					<< "\n y: " << (int)camera->m_position.y
					<< "\n z: " << (int)camera->m_position.z;
			}
			last_camera_pos = camera->m_position;


			// 1. 首选渲染深度贴图
			glViewport(0, 0, texture_manager->SHADOW_WIDTH, texture_manager->SHADOW_HEIGHT);
			glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
			glClear(GL_DEPTH_BUFFER_BIT);
			
			light_proj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
			light_view = glm::lookAt(sun->getPosition(have_past_time), 
				glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
			lightSpaceMatrix = light_proj * light_view;
			shader_manager->simple_depth_shader->use();
			shader_manager->simple_depth_shader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
			drawNormalBlock();
			drawTrans();

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			// 2. 像往常一样渲染场景，但这次使用深度贴图
			glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			glBindTexture(GL_TEXTURE_2D, texture_manager->shadow_map);
			//draw the actual objects
			drawLight();
			shader_manager->normal_shader->use();
			shader_manager->normal_shader->setInt("u_shadow_map", texture_manager->shadow_map);
			shader_manager->normal_shader->setMat4("u_light_space_mat", lightSpaceMatrix);

			drawNormalBlock();
			drawTrans();

			glfwSwapBuffers(m_window);
			glfwPollEvents();
		}
	}


}
