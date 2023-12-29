#pragma once
#include <vector>

#include "render/Renderer.h"

namespace SampleCraft
{
	class TextureManager
	{
	public:
		std::vector<unsigned> m_renderer_ids;
		std::string m_file_path;
		unsigned char* m_local_buffer;
		int m_bit_per_pixel;
		int m_width;
		int m_height;
	private:
		static TextureManager* singleton;
	public:
		static TextureManager* createTextureManager();
		
		TextureManager();
		~TextureManager();

		int loadTexture2D(const std::string& file_path);
		int loadTextureCubemap(std::vector<std::string> faces);
		void bind(GLenum texture_type, unsigned slot, unsigned texture_id) const;
		void unbind(GLenum texture_type) const;

	public:
		unsigned snow_texture;
		unsigned oak_planks_texture;
		unsigned oak_log_side_texture;
		unsigned oak_log_top_texture;
		unsigned leaves_texture;
	};
	
}
