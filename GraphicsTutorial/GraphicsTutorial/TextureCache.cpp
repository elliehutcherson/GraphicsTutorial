#include "TextureCache.h"
#include "ImageLoader.h"

#include <iostream>

TextureCache::TextureCache()
{
}


TextureCache::~TextureCache()
{
}


GLTexture TextureCache::getTexture(std::string texturePath) {
	
	//The iterator declaration for this sucks. map(key,value)iterator variable
	//std::map<std::string, GLTexture>::iterator mit = _textureMap.find(texturePath)
	//auto can discern something like this because there's only one thing it could be.
	auto mit = _textureMap.find(texturePath);

	//check if its not in the map
	if (mit == _textureMap.end()) {
		GLTexture newTexture = ImageLoader::loadPNG(texturePath);

		//a pair is two values that are combined together, like k,v.
		//std::pair<std::string, GLTexture> newPair(texturePath, newTexture);
		//_textureMap.insert(newPair)

		_textureMap.insert(make_pair(texturePath, newTexture));

		std::cout << "Loaded new texture!\n";
		return newTexture;
	}

	//if its found, we want to return the value (where we store the texture)
	std::cout << "Loaded cached texture!\n";
	return mit->second;
}
