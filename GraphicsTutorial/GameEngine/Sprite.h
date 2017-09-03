#pragma once
#include <GL/glew.h>
#include "GLTexture.h"

#include <string>

class Sprite
{
public:
	Sprite();
	~Sprite();

	//Initialzie the sprite, x and y coordinates, width and height of the sprite object
	//(in this case an image), and now we are include the file path of the texture we want to load.
	void init(float x, float y, float width, float height, std::string texturePath);

	void draw();

private:
	float _x;
	float _y;
	float _width;
	float _height;
	GLuint _vboId;
	GLTexture _texture;

};

