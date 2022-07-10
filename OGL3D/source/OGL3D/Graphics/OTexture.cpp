#include <OGL3D/Graphics/OTexture.h>
#include <glad/glad.h>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>

//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

OTexture::OTexture(const OTextureDesc& desc)
{
    if (desc.cubemap)
    {
        cubemapTexture(desc);

    }
    else
    {
        normalTexture(desc);
    }
}

OTexture::~OTexture()
{

}

ui32 OTexture::getId()
{
    return m_textureId;
}

ui32 OTexture::getCount()
{
    return m_count;
}

void OTexture::normalTexture(const OTextureDesc& desc)

{

    path = desc.textureFilePath;
    type = desc.tex_type;
    m_count = desc.num_of_texture;


    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps

    i32 width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(desc.textureFilePath, &width, &height, &nrChannels, 0);

    if (data)
    {
        GLenum format;
        if (nrChannels == 1)
        {
            format = GL_RED;
        }
        else if (nrChannels == 3)
        {
            format = GL_RGB;
        }
        else if (nrChannels == 4)
        {
            format = GL_RGBA;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        OGL3D_WARNING("Failed to load texture |" << desc.textureFilePath << " not found");
        return;
    }
    OGL3D_INFO("Texture  | " << desc.textureFilePath << " loaded successfully");
    stbi_image_free(data);
    m_sizeTexture = { height,width };

}

void OTexture::cubemapTexture(const OTextureDesc& desc)
{
    
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(false);

    for (unsigned int i = 0; i < desc.faces.size(); i++)
    {
        unsigned char* data = stbi_load(desc.faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
            std::cout<<"Texture  | " << desc.faces[i] << " loaded successfully\n";
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << desc.faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    m_sizeTexture.width = width;
    m_sizeTexture.height = height;

    m_cubemap = true;

}

void OTexture::bindTexture()
{
    if (m_cubemap)
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D,m_textureId);

    }
}

TextureSizeDesc OTexture::getTextureSizeDesc()
{
    return m_sizeTexture;
}
