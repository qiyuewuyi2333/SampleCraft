#include "render/TextureManager.h"
#include "render/stb_image/stb_image.h"

namespace SampleCraft
{
    TextureManager* TextureManager::singleton = nullptr;
	TextureManager* TextureManager::createTextureManager()
    {
        if (singleton == nullptr)
            singleton = new TextureManager();
        return singleton;
    }

    TextureManager::TextureManager()
        :m_file_path(""), m_local_buffer(nullptr),
        m_width(0), m_height(0), m_bit_per_pixel(0)
    {
        snow_texture =loadTexture2D("./resource/textures/blocks/snow/snow.png");
        oak_planks_texture = loadTexture2D("./resource/textures/blocks/oak_planks/oak_planks.png");
        oak_log_side_texture = loadTexture2D("./resource/textures/blocks/oak_log/oak_log.png");
        oak_log_top_texture = loadTexture2D("./resource/textures/blocks/oak_log/oak_log_top.png");
        leaves_texture = loadTexture2D("./resource/textures/blocks/leaves/spruce_leaves.png");

        bind(GL_TEXTURE_2D, 1, snow_texture);
        bind(GL_TEXTURE_2D, 2, oak_planks_texture);
        bind(GL_TEXTURE_2D, 3, oak_log_side_texture);
        bind(GL_TEXTURE_2D, 4, oak_log_top_texture);
        bind(GL_TEXTURE_2D, 5, leaves_texture);
    }

    TextureManager::~TextureManager()
    {
        for (unsigned& id : m_renderer_ids)
        {
            glDeleteTextures(1, &id);
        }
    }

    int TextureManager::loadTexture2D(const std::string& file_path)
    {
        unsigned int textureID;
        m_file_path = file_path;
        m_local_buffer = stbi_load(m_file_path.c_str(), &m_width, &m_height, &m_bit_per_pixel, 4);

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_local_buffer);
        glBindTexture(GL_TEXTURE_2D, 0);

        if (m_local_buffer)
            stbi_image_free(m_local_buffer);
        m_renderer_ids.push_back(textureID);
        return textureID;
    }

    int TextureManager::loadTextureCubemap(std::vector<std::string> faces)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                );
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        m_renderer_ids.push_back(textureID);
        return textureID;
    }

    void TextureManager::bind(GLenum texture_type, unsigned slot, unsigned texture_id) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(texture_type, texture_id);
    }

    void TextureManager::unbind(GLenum texture_type) const
    {
        glBindTexture(texture_type, 0);
    }

}
