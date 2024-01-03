#ifndef SUN_H
#define SUN_H
#include "entities/blocks/Block.h"

namespace SampleCraft
{

	class Sun : public Block
	{
	public:
		inline const float& getheight() const
		{
			return height;
		}
		glm::vec3 getPosition(float have_past_time);
		glm::vec3 getLunaPosition(float have_past_time);
		Sun();
		~Sun();
	private:
		float height;
		float speed;
		glm::vec3 light_strength =  glm::vec3(1.0, 1.0, 0.9);
		glm::vec3 luna_light_strength = glm::vec3(0.5, 0.5, 0.5);

	public:
		glm::vec3& getLightStrength();
		glm::vec3& getLunaLightStrength();
	};
}

#endif


