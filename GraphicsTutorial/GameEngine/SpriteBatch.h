#pragma once

#include <GL/glew.h>
#include <glm\glm.hpp> //vec4
#include <vector>

#include "Vertex.h"

/*This class is so we can batch a multiple sprites together in one vbo
and a single draw call. Do this for each individual sprite is really
inefficient. */

namespace GameEngine {
	
	//We want to sort the the textures thats we store in Glyphs.
	//Not sure when we'll need the other types of sorts.
	enum class GlyphSortType {
		NONE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE
	};
	
	//Sprite structure and properties like depth (layer), texture, 
	//six vertices for quad, and more, this is good for sorting. So
	//if a lot of sprites are to be drawn at once, all textures that are
	//alike can be drawn together, so that we can minimize the number of
	//draw calls and texture switching.
	struct Glyph {
		GLuint texture;
		float depth;

		Vertex topLeft;
		Vertex bottomLeft;
		Vertex topRight;
		Vertex bottomRight;
	};

	//Each batch is going to store an offset in our vertex buffer object (_vbo)
	//so that when we call glDrawArrays, we can specify that we want to start at a
	//certain point (the offeset). This is so we don't have to have multiple vbo's.
	class RenderBatch {
	public:
		//These need to be named differently from the variables below
		//We are also initializing them here.
		RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) : offset(Offset),
			numVertices(NumVertices), texture(Texture) {}

		GLuint offset; //see above
		GLuint numVertices; //number of vertices we need to draw
		GLuint texture;

	private:
	};

	class SpriteBatch {
	public:
		SpriteBatch();
		~SpriteBatch();

		void init(); //initialization

		//Setting the default sort type to texture.
		void begin(GlyphSortType sortType = GlyphSortType::TEXTURE); //getting ready to draw
		void end(); //post processing, like sorting

		//destRect will contain our positions, uvRect will contain our dimensions. Or something. We are also
		//passing these in byreference so that we don't have to make another copy every time this is called
		//because this will probably be called many times. We don't want to change these variables though
		//we can also pass them in with the parameter "const". We don't have to pass the texture in byref
		//because its just an unsigned int.
		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color); //add all sprites to batch

		void renderBatch(); //render to screen

	private:
		void createRenderBatches();
		void createVertexArray();
		void sortGlyphs();

		static bool compareFrontToBack(Glyph* a, Glyph* b);
		static bool compareBackToFront(Glyph* a, Glyph* b);
		static bool compareTexture(Glyph* a, Glyph* b);

		GLuint _vbo;
		GLuint _vao;

		GlyphSortType _sortType;
		
		//Because we are going to have to sort this vector frequently because we want to keep like textures
		//together, so that we can batch them together when we draw them with SpriteBatch, we want to sort 
		//pointers* instead of all of the data that would be stored in a glyph struct.
		std::vector<Glyph*> _glyphs;
		std::vector<RenderBatch> _renderBatches;

	};

}
