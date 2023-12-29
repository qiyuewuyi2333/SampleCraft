#include "render/VertexArray.h"
#include "render/VertexBufferLayout.h"
#include "render/VertexBuffer.h"

namespace SampleCraft
{
	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_renderer_id);
	}
	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_renderer_id);
	}

	void VertexArray::addBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& vertexBufferLayout)
	{
		bind();
		vertexBuffer.bind();
		m_count = vertexBuffer.getCount();
		const auto& elements = vertexBufferLayout.getElements();
		unsigned int offset = 0;
		for(int i =0;i<elements.size();++i)
		{
			const auto& element = elements[i];
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, element.count, element.type, element.normalized,
			                      vertexBufferLayout.getStride(), (const void*)offset);

			offset += element.count * VertexBufferElement::getSizeOfType(element.type);
		}
	}
}

