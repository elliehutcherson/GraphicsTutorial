#pragma once
#include "TextureCache.h"

#include <string>

//Because we will only need one instance of our resource manager, we don't
//need a constructor or destructor, so it will be a static class
class ResourceManager
{
public:
	static GLTexture getTexture(std::string texturePath);

private:
	static TextureCache _textureCache;
};

