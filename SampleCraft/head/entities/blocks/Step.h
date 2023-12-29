#ifndef STEP_H
#define STEP_H
#include "entities/Entity.h"
#include "render/VertexBuffer.h"
#include "render/VertexBufferLayout.h"

namespace SampleCraft
{
	class Step:public Entity
	{
	public:
		inline const VertexArray& getVertexArray() const { return vertex_array; }
		Step();
		~Step();
	private:
		VertexArray vertex_array;
		VertexBuffer vertex_buffer;
		VertexBufferLayout vertex_buffer_layout;
	};
}

#endif
