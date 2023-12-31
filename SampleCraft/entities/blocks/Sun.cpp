
#include "entities/blocks/Sun.h"

namespace SampleCraft
{
	glm::vec3 Sun::getPosition(float have_past_time)
	{
		return glm::vec3(height * sin(speed * have_past_time), height * cos(speed * have_past_time), 0.0f);
	}

	Sun::Sun()
		:height(50.0f),
		speed(0.1f)
	{

	}

	Sun::~Sun()
	{
	}
}
