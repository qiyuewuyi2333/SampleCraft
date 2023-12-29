#pragma once
#include "glad/glad.h"
namespace SampleCraft
{
	class VertexBuffer;
	class VertexBufferLayout;

	class VertexArray
	{
	private:
		unsigned int m_renderer_id;
		unsigned int m_count = 0;

	public:

		VertexArray();
		~VertexArray();

		inline unsigned int getCount() const { return m_count; }

	public:
		void addBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& vertexBufferLayout);
		inline void bind() const { glBindVertexArray(m_renderer_id); }
		inline void unbind() const { glBindVertexArray(0); }
	};
	
}
