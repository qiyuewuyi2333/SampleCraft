#include "render/VertexBuffer.h"
#include "render/VertexBufferLayout.h"

namespace SampleCraft
{
	VertexBuffer::VertexBuffer(const void* data, unsigned size)
		:m_count(size)
	{
		glGenBuffers(1, &m_renderer_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_renderer_id);
	}

	void VertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
	}

	void VertexBuffer::unBind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	
}
