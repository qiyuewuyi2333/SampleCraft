#ifndef OAKLOG_H
#define OAKLOG_H
#include "entities/Entity.h"
#include "render/VertexBuffer.h"
#include "render/VertexBufferLayout.h"

namespace SampleCraft
{

	class OakLog:public Entity
	{
	public:
		inline const VertexArray& getVertexArraySide() const { return vertex_array_side; }
		inline const VertexArray& getVertexArrayTop() const { return vertex_array_top; }
	public:
		OakLog();
		~OakLog();
	private:

		VertexArray vertex_array_top;
		VertexArray vertex_array_side;
		VertexBuffer vertex_buffer_side;
		VertexBuffer vertex_buffer_top;
		VertexBufferLayout vertex_buffer_layout;
	};
}

#endif
