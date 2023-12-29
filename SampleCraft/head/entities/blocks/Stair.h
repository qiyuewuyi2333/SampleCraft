#ifndef STAIR_H
#define STAIR_H
#include "entities/Entity.h"
#include "render/VertexArray.h"
#include "render/VertexBuffer.h"
#include "render/VertexBufferLayout.h"

namespace SampleCraft
{
	class Stair : public Entity
	{
	public:
		inline const VertexArray& getVertexArray() const { return vertex_array; }
		Stair();
		~Stair();
	private:
		VertexArray vertex_array;
		VertexBuffer vertex_buffer;
		VertexBufferLayout vertex_buffer_layout;
	};
}
#endif
