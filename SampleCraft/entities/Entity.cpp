#include "entities/Entity.h"

namespace SampleCraft
{
	void Entity::show()
	{
		state = SHOW;
	}

	Entity::Entity()
		:state(HIDE)
	{
		
	}

	Entity::~Entity()
	{
	}
}
