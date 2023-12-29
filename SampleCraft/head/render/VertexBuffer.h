#pragma once


namespace SampleCraft
{
	class VertexBuffer
	{
	private:
		unsigned int m_renderer_id;
		unsigned int m_count = 0;
	public:
		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer();

		void bind() const;
		void unBind() const;
		inline unsigned int getCount() const { return m_count; }
	};
}
