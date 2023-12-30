#pragma once
#include <windows.h>

#include "render/Shader.h"

namespace SampleCraft
{
	class ShaderManager
	{
	public:
		static ShaderManager* createShaderManager();
		
	public:
		ShaderManager();
		~ShaderManager();
	private:
		static ShaderManager* singleton ;
	public:
		Shader* normal_shader = nullptr;
		Shader* light_shader = nullptr;
		Shader* log_shader = nullptr;
		Shader* trans_shader = nullptr;
		Shader* simple_depth_shader = nullptr;
	};
}
