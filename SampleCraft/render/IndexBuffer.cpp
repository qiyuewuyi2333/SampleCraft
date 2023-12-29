#include <glad/glad.h>
#include "render/IndexBuffer.h"



namespace SampleCraft
{
	unsigned IndexBuffer::getCount() const
	{
		return m_count;
	}


	IndexBuffer::IndexBuffer(const unsigned int* data, unsigned count)
		:m_count(count)
	{
		glGenBuffers(1, &m_renderer_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);


	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_renderer_id);
	}

	void IndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
	}

	void IndexBuffer::unBind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
