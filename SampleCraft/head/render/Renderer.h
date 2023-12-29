#pragma once
#include "Shader.h"
#include "render/VertexArray.h"
#include "render/IndexBuffer.h"

namespace SampleCraft
{

	class Renderer
	{
	public:
		void drawInstance() const;
		void drawElements(const VertexArray& vertex_array, const IndexBuffer& index_buffer, const Shader& shader) const;
		void draw(const VertexArray& vertex_array, const Shader& shader) const;
		void clear(const glm::vec4& clear_color) const;
	public:
		Renderer();
		~Renderer();
	private:
		unsigned int renderer_id;

		
	};
}
