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
		trans_shader = new Shader("./shaders/block/trans/trans.vert",
			"./shaders/block/trans/trans.frag");
		simple_depth_shader = new Shader("./shaders/shadow/simple_depth.vert",
			"./shaders/shadow/simple_depth.frag");
	}

	ShaderManager::~ShaderManager()
	{
		if (normal_shader)
			delete normal_shader;
		if (light_shader)
			delete light_shader;
		if (log_shader)
			delete log_shader;
		if (trans_shader)
			delete trans_shader;
		if (simple_depth_shader)
			delete simple_depth_shader;
	}
}
