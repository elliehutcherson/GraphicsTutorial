#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <string>

namespace GameEngine {

	//These are going to be bitwise variables
	//First bit, second bit, thrid bit, so the datatype to be passed in is unsigned int
	enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };

	class Window
	{
	public:
		Window();
		~Window();

		int create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags);

		void swapBuffer();

		int getScreenWidth() { _screenWidth; }
		int getScreenHeight() { _screenHeight; }

	private:
		SDL_Window* _sdlWindow;
		int _screenWidth, _screenHeight;
	};

}