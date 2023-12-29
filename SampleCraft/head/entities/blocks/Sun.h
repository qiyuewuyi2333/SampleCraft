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
		Sun();
		~Sun();
	private:
		float height;
		float speed;

	};
}

#endif


