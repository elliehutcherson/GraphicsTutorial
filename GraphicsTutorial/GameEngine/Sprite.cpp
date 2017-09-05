#include "Sprite.h"
#include "Vertex.h"
#include "ResourceManager.h"

#include <cstddef>

namespace GameEngine {

	Sprite::Sprite()
	{
		//Setting your ids to 0 is good for error checking, and more.
		_vboId = 0;
	}

	Sprite::~Sprite()
	{
		//Releases the memory in the destrucutor.
		if (_vboId != 0) {
			glDeleteBuffers(1, &_vboId);
		}
	}

	void Sprite::init(float x, float y, float width, float height, std::string texturePath) {
		_x = x;
		_y = y;
		_width = width;
		_height = height;

		_texture = ResourceManager::getTexture(texturePath);

		if (_vboId == 0) {
			//The Gen Buffer takes a pointer (using the &), like most things in openGL.
			glGenBuffers(1, &_vboId);
		}

		Vertex vertexData[6];

		//First Triangle
		vertexData[0].setPosition(x + width, y + height);
		vertexData[0].setUV(1.0f, 1.0f);

		vertexData[1].setPosition(x, y + height);
		vertexData[1].setUV(0.0f, 1.0f);

		vertexData[2].setPosition(x, y);
		vertexData[2].setUV(0.0f, 0.0f);

		//Second Triangle
		vertexData[3].setPosition(x, y);
		vertexData[3].setUV(0.0f, 0.0f);

		vertexData[4].setPosition(x + width, y);
		vertexData[4].setUV(1.0f, 0.0f);

		vertexData[5].setPosition(x + width, y + height);
		vertexData[5].setUV(1.0f, 1.0f);

		for (int i = 0; i < 6; i++) {
			vertexData[i].setColor(255, 255, 255, 255);
		}

		vertexData[1].setColor(255, 255, 255, 255);
		vertexData[4].setColor(255, 255, 255, 255);

		/*To upload the data that we created in our vertexData, we bind it
		to a buffer,specifically the buffer we created above. Binding the
		buffer tells openGL that we now want that buffer to be active.
		The first argument is the type of buffer we are using (GL_ARRAY_BUFFER
		which is just your basic buffer). I'm also not sure why glBindBuffer only
		takes a GLUint instead of a pointer.*/

		glBindBuffer(GL_ARRAY_BUFFER, _vboId);

		//Also, apparently arrays can be used as pointers, and vise versa.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
		//We unbind the buffer. Not necessary, but good practice.
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	void Sprite::draw() {

		//We don't need to unbind the texture because something else could use it
		//What we should do is check to see if its bound, if not bind it.
		glBindTexture(GL_TEXTURE_2D, _texture.id);

		glBindBuffer(GL_ARRAY_BUFFER, _vboId);



		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(0);

		glDisableVertexAttribArray(1);
		
		glDisableVertexAttribArray(2);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}