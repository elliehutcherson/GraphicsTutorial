#include "SpriteBatch.h"

#include <algorithm> //for a sorting function

namespace GameEngine {

	SpriteBatch::SpriteBatch() :
		_vbo(0),
		_vao(0)
	{
	}


	SpriteBatch::~SpriteBatch()
	{
	}

	void SpriteBatch::init() {
		createVertexArray();
	}

	void SpriteBatch::begin(GlyphSortType sortType /* GlyphSortType::Texture */){
		_sortType = sortType;
		//clear out any left over data from the last call
		_renderBatches.clear(); 
		_glyphs.clear();
	}

	void SpriteBatch::end() {
		sortGlyphs();
		createRenderBatches();
	}

	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color) {
		//draw is going to want to add a glyph to a vector of glyphs.
		//So using pointers because it's less memory. The arrow points to the actual Glyph. 

		Glyph* newGlyph = new Glyph;
		newGlyph->texture = texture;
		newGlyph->depth = depth;
		
		//Basically, this is taking the place of our code that we set manually in sprite.cpp
		//and we can call spritebatch for any sprite that we have as long as we give spritebatch
		//the positions, uv coordinates (the coordinates of the sprite relative to itself,
		//from 0 to 1), dimensions, texture, color, depth, and color vector.
		
		//Also, because we are using glm::vec4, we have the methods x, y, z, and w. We are storing
		//coordinates in x and y, and height and width in z and w. 
		newGlyph->topLeft.color = color;
		newGlyph->topLeft.setPosition(destRect.x, destRect.y + destRect.w);
		newGlyph->topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);
		
		newGlyph->bottomLeft.color = color;
		newGlyph->bottomLeft.setPosition(destRect.x, destRect.y);
		newGlyph->bottomLeft.setUV(uvRect.x, uvRect.y);
	
		newGlyph->bottomRight.color = color;
		newGlyph->bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
		newGlyph->bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);
		
		newGlyph->topRight.color = color;
		newGlyph->topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
		newGlyph->topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

		_glyphs.push_back(newGlyph); //adding it to our vector of glyphs.
	}

	void SpriteBatch::renderBatch() {
		
		//Have to bine the vertex array before we can draw anything.
		glBindVertexArray(_vao);

		for (int i = 0; i < _renderBatches.size(); i++) {
			glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);

			glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
		}

		glBindVertexArray(0); //unbindng
	}

	void SpriteBatch::createRenderBatches() {
		std::vector <Vertex> vertices;
		//This just speeds things up a little bit, because we know the size it should be.
		vertices.resize(_glyphs.size() * 6);

		if (_glyphs.empty()) {
			return;
		}
		//So what we could do is create a RenderBatch and then use push_back
		//to put it in _renderBatches. However, that variable is temporary
		//and it would get destroyed with the stack. Instead of wasting that resource,
		//we can use emplace_back, which takes out that intermediate step and creates the 
		//object within _renderBatches using the parameters we give it.

		//Since this is the first batch, the offset is 0, we always have 6 vertices for a quad
		//and we use the first texture in _glyphs.

		int cv = 0; //current vertex
		int offset = 0;
		_renderBatches.emplace_back(offset, 6, _glyphs[0]->texture);
		vertices[cv++] = _glyphs[0]->topLeft;
		vertices[cv++] = _glyphs[0]->bottomLeft;
		vertices[cv++] = _glyphs[0]->bottomRight;
		vertices[cv++] = _glyphs[0]->bottomRight;
		vertices[cv++] = _glyphs[0]->topRight;
		vertices[cv++] = _glyphs[0]->topLeft;
		offset += 6;

		//We have to start at one because we already did the first batch
		for (int cg = 1; cg < _glyphs.size(); cg++) { //current glyph
			//We only want to emplace_back to the renderbatch unless
			//the current texture is different from the previous texture.
			//that way we can make multiple draw calls off the same vbo.
			if (_glyphs[cg]->texture != _glyphs[cg - 1]->texture) {
				_renderBatches.emplace_back(offset, 6, _glyphs[cg]->texture);
			} else { //otherwise we just increase the number of vertices.
				//Back will get us the last element.
				_renderBatches.back().numVertices += 6;
			}
			//all of the vertices are getting placed in one vector,
			//but we can discern which vertices go with which texture 
			//by their offsets.

			_renderBatches.emplace_back(0, 6, _glyphs[cg]->texture);
			vertices[cv++] = _glyphs[cg]->topLeft;
			vertices[cv++] = _glyphs[cg]->bottomLeft;
			vertices[cv++] = _glyphs[cg]->bottomRight;
			vertices[cv++] = _glyphs[cg]->bottomRight;
			vertices[cv++] = _glyphs[cg]->topRight;
			vertices[cv++] = _glyphs[cg]->topLeft;
			offset += 6;
		}

		//we bind the vertex buffer object, so opengl knows where to send our data
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		//there is probably some data still left in our vbo, and we don't want it anymore, so a fast way
		//to write to our vbo would be to abandon it and have openGL create a new one for us, and we right to that
		//this is called orphaning the data, we do that by passing in a nullptr for our data
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		//now we want to upload our vertex data to our vertex buffer object.
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
		//now we unbind our buffer.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void SpriteBatch::createVertexArray() {
		//There is a lot of things that we have to do to draw our texture to the screen.
		//If we just bind a vertex array with all of the states we want for opengl, it would be a lot easier.

		if (_vao == 0) {
			glGenVertexArrays(1, &_vao);
		}
		glBindVertexArray(_vao);

		if (_vbo == 0) {
			glGenBuffers(1, &_vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		//The size of the array (the second parameter) need to know the number of elements for each piece of data
		//We are only using x and y coordinates, so there are 2 elements.
		//The stride is the size of the vertex struct.

		//This is the position attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

		//This is the color attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

		//This is the UV attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
		
		//Now we need to unbind the vertex attribute array.
		//This will disable all of our vretext attribute arrays (glDisableVertexAttribArray)
		//It will also unbind our vbo
		glBindVertexArray(0);
	}

	void SpriteBatch::sortGlyphs() {
		//stable_sort ensures that a items that are sort keep their oringal position/order when comparing two items of the same value/type.
		//stable_sort parameters take the beginning and end of the container,
		//the the third parameter is predicate/comparatory which is a function that we pass in
		//That function is what determines how to sort, what is greater than and what is less than
		//If you're just using numeric values, you don't need to use a function for the third parameter.
		switch (_sortType) {
			case GlyphSortType::BACK_TO_FRONT:
				
				std::stable_sort(_glyphs.begin(), _glyphs.end(), compareBackToFront);
				break;
			case GlyphSortType::FRONT_TO_BACK:
				std::stable_sort(_glyphs.begin(), _glyphs.end(), compareFrontToBack);
				break;
			case GlyphSortType::TEXTURE:
				std::stable_sort(_glyphs.begin(), _glyphs.end(), compareTexture);
				break;
		}
		
	}

	//we have to use the arrows because the inputs are pointers
	bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b) {
		return (a->depth < b->depth);
	}
	
	bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b) {
		return (a->depth > b->depth);
	}
	
	bool SpriteBatch::compareTexture(Glyph* a, Glyph* b) {
		return (a->texture > b->texture);
	}

}