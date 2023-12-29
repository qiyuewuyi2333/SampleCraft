#include <glad/glad.h>
#include <glm/vec4.hpp>
#include "render/Renderer.h"
#include "render/Shader.h"
#include "render/IndexBuffer.h"


namespace SampleCraft
{

	void Renderer::clear(const glm::vec4& clear_color) const
	{
		glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	Renderer::Renderer()
	{
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::drawInstance() const
	{
	}

	void Renderer::drawElements(const VertexArray& vertex_array, const IndexBuffer& index_buffer,
		const Shader& shader) const
	{
		shader.use();
		vertex_array.bind();
		index_buffer.bind();
		glDrawElements(GL_TRIANGLES, index_buffer.getCount(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::draw(const VertexArray& vertex_array, const Shader& shader) const
	{
		shader.use();
		vertex_array.bind();
		glDrawArrays(GL_TRIANGLES, 0, vertex_array.getCount());
	}


}

