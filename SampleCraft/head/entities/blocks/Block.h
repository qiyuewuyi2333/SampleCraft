#pragma once
#include <glm/vec3.hpp>

#include "entities/Entity.h"
#include "render/VertexArray.h"
#include "render/VertexBuffer.h"
#include "render/VertexBufferLayout.h"

namespace SampleCraft
{
	class Block :public Entity
	{
	public:
		inline const VertexArray& getVertexArray() const { return vertex_array; }
	public:
		Block();
		~Block();
	private:

		glm::vec3 size;//Block's size ,default value = (1,1,1)
		glm::vec3 position;

	public:
		glm::vec3& getSize() ;
		void setSize(const glm::vec3& size);
		glm::vec3& getPosition();
		void setPosition(const glm::vec3& position) ;

	private:
		VertexArray vertex_array;
		VertexBuffer vertex_buffer;
		VertexBufferLayout vertex_buffer_layout;
	};
}
