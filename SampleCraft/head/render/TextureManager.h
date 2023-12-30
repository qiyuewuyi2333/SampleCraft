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
		const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	private:
		static TextureManager* singleton;
	public:
		static TextureManager* createTextureManager();
		
		TextureManager();
		~TextureManager();

		int loadTexture2D(const std::string& file_path);
		int loadTextureCubemap(std::vector<std::string> faces);
		GLuint loadTextureShadow();
		void bind(GLenum texture_type, unsigned slot, unsigned texture_id) const;
		void unbind(GLenum texture_type) const;

	public:
		unsigned snow_texture;
		unsigned oak_planks_texture;
		unsigned oak_log_side_texture;
		unsigned oak_log_top_texture;
		unsigned stripped_birch_log_texture;
		unsigned stripped_birch_log_top_texture;
		unsigned leaves_texture;
		unsigned glass_texture;

		GLuint shadow_map;
	};
	
}
