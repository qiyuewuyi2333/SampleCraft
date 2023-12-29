#include "entities/blocks/Step.h"


namespace SampleCraft
{
	const float step_data[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.0f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.5f,
         0.5f,  0.0f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.5f,
        -0.5f,  0.0f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.5f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.0f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.5f,
         0.5f,  0.0f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.5f,
        -0.5f,  0.0f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.5f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.0f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.5f,
        -0.5f,  0.0f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.5f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.5f,
        -0.5f,  0.0f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

         0.5f,  0.0f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.0f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.5f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.5f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.5f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.0f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,

        -0.5f,  0.0f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.0f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.0f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.0f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.0f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.0f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
	Step::Step()
		:Entity(),
		vertex_array(),
		vertex_buffer(step_data, 8 * 36 * sizeof(float)),
		vertex_buffer_layout()
	{
        vertex_buffer_layout.add<float>(3);
        vertex_buffer_layout.add<float>(3);
        vertex_buffer_layout.add<float>(2);
        vertex_array.addBuffer(vertex_buffer, vertex_buffer_layout);
        vertex_array.bind();
	}

	Step::~Step()
	{
	}
}
