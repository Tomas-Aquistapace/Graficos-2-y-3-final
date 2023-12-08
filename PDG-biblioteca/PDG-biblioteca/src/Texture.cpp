#include "Texture.h"
#include "stb_image.h"
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include <iostream>

Texture::Texture(const std::string texturePath)
{
	rendererID = 0;
	filePath = texturePath;
	localBuffer = nullptr;
	width = 0;
	height = 0;
	BPP = 0;
	stbi_set_flip_vertically_on_load(1);
	localBuffer = stbi_load(filePath.c_str(), &width, &height, &BPP, 4);
	glGenTextures(1, &rendererID);
	glBindTexture(GL_TEXTURE_2D, rendererID);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width,height,0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
}

Texture::~Texture()
{
	glDeleteTextures(1, &rendererID);
}

void Texture::Bind(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, rendererID);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
unsigned int Texture::getTex()
{
	return rendererID;
}