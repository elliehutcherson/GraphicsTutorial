#include "Window.h"
#include "Errors.h"

namespace GameEngine {
	Window::Window()
	{
	}


	Window::~Window()
	{
	}


	int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags) {

		//The flags for an opengl window use Uint32, same as an unsigned int
		Uint32 flags = SDL_WINDOW_OPENGL;

		//Using bitwise operators to store all of our variables into flags.
		if (currentFlags & INVISIBLE) {
			flags |= SDL_WINDOW_HIDDEN;
		}

		if (currentFlags & FULLSCREEN) {
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if (currentFlags & BORDERLESS) {
			flags |= SDL_WINDOW_BORDERLESS;
		}

		//The window title takes c_str/char*.
		_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);

		if (_sdlWindow == nullptr) {
			fatalError("SDL Window could not be created!");
		}

		SDL_GLContext glContext = SDL_GL_CreateContext(_sdlWindow);
		if (glContext == nullptr) {
			fatalError("SDL_GLContext could not be created!");
		}

		//GLEW_OK = 0, so if the captured value (the returned value) does not == 0 then something is wrong.
		GLenum error = glewInit();
		if (error != GLEW_OK) {
			fatalError("Something went wrong when initializing glew!");
		}

		std::printf("***   OpenGL Version: %s   ***\n", glGetString(GL_VERSION));

		//Anytime glClear is called (clearing the window of what was drawn), it is cleared to the color we set here.
		//glClear is called at the beginning of drawGame().
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

		//Set V-Sync On/Off
		SDL_GL_SetSwapInterval(0);

		//Enable alpha blending
		glEnable(GL_BLEND);
		//Take the source alpha, and get the inverse
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return 0;
	}

	void Window::swapBuffer() {
		//Swap our buffer and draw everything to the screen!
		SDL_GL_SwapWindow(_sdlWindow);
	}
}