#include "ShaderManager.h"

namespace SampleCraft
{
	ShaderManager* ShaderManager::singleton = nullptr;
	ShaderManager* ShaderManager::createShaderManager()
	{
		if (singleton == nullptr)
		{
			singleton = new ShaderManager();
		}
		return singleton;
	}

	ShaderManager::ShaderManager()
	{
		//load shaders
		normal_shader = new Shader("./shaders/block/normal/normal.vert",
			"./shaders/block/normal/normal.frag");
		light_shader = new Shader("./shaders/block/light_source/light_source.vert",
		"./shaders/block/light_source/light_source.frag");
		log_shader = new Shader("./shaders/block/log/log.vert",
			"./shaders/block/log/log.frag");
		leaves_shader = new Shader("./shaders/block/leave/leave.vert",
			"./shaders/block/leave/leave.frag");
	}

	ShaderManager::~ShaderManager()
	{
		if (normal_shader)
			delete normal_shader;
		if (light_shader)
			delete light_shader;
		if (log_shader)
			delete log_shader;
		if (leaves_shader)
			delete leaves_shader;
	}
}
