#pragma once

#include <GL/glew.h>


/*Apparently you need to a multiple of 4 bytes.
Float is 4 bytes, GLubyte is always going to be 1 byte
So if you had 3 bytes in your GLubyte array (color[3]),
then you would have a total of 11 bytes, which is bad
for alignment issues...

Also, when we pass our color array as bytes, it converts
each variable into floats, this happens automatically
so we just use vec4 in our color shader program.*/

struct Position {
	float x;
	float y;
};

struct Color {
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;
};

//The UV struct is for loading our texture coordinates in our  GLSLProgram
struct UV {
	float u;
	float v;
};

struct Vertex {
	Position position;
	Color color;
	UV uv;

	//And apparently this doesn't take up anymore space in the ram, so that's good.
	void setPosition(float x, float y) {
		position.x = x;
		position.y = y;
	}

	void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;
	}

	void setUV(float u, float v) {
		uv.u = u;
		uv.v = v;
	}

};