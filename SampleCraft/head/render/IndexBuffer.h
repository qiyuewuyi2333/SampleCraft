#pragma once

namespace SampleCraft
{
	class IndexBuffer
	{
	private:
		unsigned int m_renderer_id;
		unsigned int m_count;
	public:
		IndexBuffer(const unsigned int* data, unsigned int count);
		~IndexBuffer();

		void bind() const;
		void unBind() const;

		unsigned int getCount() const;
	};

}
