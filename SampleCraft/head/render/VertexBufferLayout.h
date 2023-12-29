#pragma once
#include <assert.h>
#include <vector>
#include <glad/glad.h>
#include "render/Renderer.h"

namespace SampleCraft
{

	struct VertexBufferElement
	{
		unsigned int type;
		unsigned int count;
		bool normalized;

		VertexBufferElement(unsigned int type,
			unsigned int count,
			bool normalized)
			:type(type), count(count), normalized(normalized)
		{

		}

		static unsigned int getSizeOfType(unsigned int type)
		{
			switch (type)
			{
			case GL_FLOAT: return 4;
			case GL_UNSIGNED_INT: return 4;
			case GL_UNSIGNED_BYTE: return 1;
			}
			assert(false);
			return 0;
		}
	};

	class VertexBufferLayout
	{
	private:
		std::vector<VertexBufferElement> m_elements;
		unsigned int m_stride;

	public:
		VertexBufferLayout()
			:m_stride(0)
		{

		}
		~VertexBufferLayout()
		{

		}

	public:
		template<typename Type>
		void add(int count)
		{
			assert(false);
		}

		template <>
		void add<float>(int count)
		{
			m_elements.emplace_back(GL_FLOAT, count, false);
			m_stride += VertexBufferElement::getSizeOfType(GL_FLOAT) * count;
		}
		template <>
		void add<unsigned int>(int count)
		{
			m_elements.emplace_back(GL_UNSIGNED_INT, count, false);
			m_stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT) * count;
		}
		template <>
		void add<unsigned char>(int count)
		{
			m_elements.emplace_back(GL_UNSIGNED_BYTE, count, false);
			m_stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE) * count;
		}

		inline const std::vector<VertexBufferElement>& getElements() const { return m_elements; }
		inline unsigned int getStride() const { return m_stride; }
	};

}
