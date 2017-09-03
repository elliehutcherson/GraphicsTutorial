#pragma once

#include "GLTexture.h"

#include <map>

/*
This class is so that when we render a texture mulitple times, we don't have to keep
multiple instances of the same image in the cached memory, we only need it in there
once and it can be referenced multiple times. And because we could possibly have lots
of textures, we need to store our textures in an organized/ordered structure, so we can't
use a vector or an array. We are going to use a map, which is basically a dictionary. We
need that ordered data structure because if we were trying to search for an item in a very
large array, we would have to iterate through it every time. If we have 100,000 textures,
that would be really shitty. The number of times you have to search in a map is log2(n) I think.
*/
class TextureCache
{
public:
	TextureCache();
	~TextureCache();

	//This is to find a texture within our map if it exists.
	GLTexture getTexture(std::string texturePath);

private:
	//This looks weird, but it's just defining the type
	//before initializing _textureMap.
	std::map<std::string, GLTexture> _textureMap;

};

