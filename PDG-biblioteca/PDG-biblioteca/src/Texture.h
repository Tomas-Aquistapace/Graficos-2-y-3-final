#pragma once
#include "Exports.h"
#include <iostream>
class SABASAENGINE_API Texture
{
private:
	unsigned int rendererID;
	std::string filePath;
	unsigned char* localBuffer;
	int width;
	int height;
	int BPP;

public:
	Texture(const std::string texturePath);
	~Texture();
	unsigned int getTex();
	void Bind(unsigned int slot);
	void Unbind();
};