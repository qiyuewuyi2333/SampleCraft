#pragma once
#include <vector>

#include "glm/glm.hpp"

namespace SampleCraft
{
	enum EntityState
	{
		HIDE,SHOW,TODIE,
	};

	class Entity
	{
	public:
		void show();
		Entity();
		~Entity();

	protected:
		EntityState state;
		std::vector<unsigned> texture_ids;

	};
}
