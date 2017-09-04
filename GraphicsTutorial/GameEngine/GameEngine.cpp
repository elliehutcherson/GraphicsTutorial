#include <SDL/SDL.h> 
#include <GL/glew.h>

#include "GameEngine.h"

namespace GameEngine {

	int init() {
		//Initialize SDL 
		SDL_Init(SDL_INIT_EVERYTHING);

		//Draws two windows instead of drawing on the same window over and over.
		//This attribute needs to be set before the window is created.
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		return 0;
	}

}