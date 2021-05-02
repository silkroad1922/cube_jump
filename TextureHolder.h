#pragma once
#ifndef TEXTURE_HOLDER_H
#define TEXTURE_HOLDER_H

#include "raylib.h"

class TextureHolder
{
public:
	~TextureHolder() { UnloadTexture(texture); }
	void setTexture(const char* filename) { texture = LoadTexture(filename); }
	Texture2D getTexture() { return texture; }
private:
	Texture2D texture;
};

#endif // !TEXTURE_HOLDER_H
