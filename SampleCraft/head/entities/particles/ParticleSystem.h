#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H
#include <glm/vec3.hpp>

namespace SampleCraft
{
	struct Particle
	{
		int type;
		glm::vec3 position;
		glm::vec3 velocity;
		float life_time_mills;
		float size;
	};
	class ParticleSystem
	{
		
	};
}

#endif
