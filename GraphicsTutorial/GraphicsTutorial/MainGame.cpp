#include "MainGame.h"
#include "Errors.h"
#include "ImageLoader.h"

#include <iostream>
#include <string>

MainGame::MainGame() :
	_screenWidth(1024),
	_screenHeight(768),
	_time(0.0f),
	_window(nullptr),
	_gameState(GameState::PLAY)
{
}

MainGame::~MainGame()
{
}

void MainGame::run() {
	initSystems();

	_sprite.init(-1.0f, -1.0f, 2.0f, 2.0f);

	_playerTexture = ImageLoader::loadPNG("Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

	gameLoop();
}

void MainGame::initSystems() {
	/* These are all the systems that have to be instantiated/initialized before we
	begin the loop of our system. First we initialize SDL. Then a window is created and 
	we create a pointer to the instance of that window. Then we instantiate a new instance 
	of SDL_GLContext and creawte a pointer to the instance of that context. The context holds all
	of the properties pertaining to the new window.  Then we initialize glew. Also, any settings
	that are only set once (like at the step of initializiation) should be set within this function.*/

	//Initialize SDL 
	SDL_Init(SDL_INIT_EVERYTHING); 

	_window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,_screenWidth,_screenHeight,SDL_WINDOW_OPENGL);
	
	if (_window == nullptr) {
		fatalError("SDL Window could not be created!");
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(_window);
	if (glContext == nullptr) {
		fatalError("SDL_GLContext could not be created!");
	}

	//GLEW_OK = 0, so if the captured value (the returned value) does not == 0 then something is wrong.
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		fatalError("Something went wrong when initializing glew!");
	}
	
	//Draws two windows instead of drawing on the same window over and over.
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//Anytime glClear is called (clearing the window of what was drawn), it is cleared to the color we set here.
	//glClear is called at the beginning of drawGame().
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	initShaders();
}

void MainGame::initShaders() {
	_colorProgram.compileShaders("Shaders/colorShading.vert","Shaders/colorShading.frag");
	//vertexPosition is the vec2 variable listed in the vertex file from above.
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();
}

void MainGame::proccessInput() {
	/*The SDL_PollEvent() function takes a pointer to an SDL_Event structure 
	that is to be filled with event information. We know that if SDL_PollEvent() 
	removes an event from the queue then the event information will be placed in 
	our test_event structure, but we also know that the type of event will be placed 
	in the type member of test_event. So to handle each event type separately we 
	use a switch statement.*/
	
	SDL_Event myEvent;

	while (SDL_PollEvent(&myEvent)) {
		switch (myEvent.type) {
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
			case SDL_MOUSEMOTION:
				std::cout << myEvent.motion.x << "," << myEvent.motion.y << std::endl;
				break;
		}
	}
}

void MainGame::drawGame() {
	/* glEnableClientState is how you tell OpenGL that you're using a vertex array 
	for a particular fixed-function attribute (gl_Vertex, gl_Color, etc). Those are 
	all removed from core contexts. You should use glEnableVertexAttribArray to 
	enable a generic vertex attribute, and you use glVertexAttribPointer to associate 
	that attribute with a buffer object. */

	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_colorProgram.use();

	//Before we bind the texture, we want to make sure that we are using the first texture unit.
	//Because you can have multiple textures bound at one time, we are going to use the first one.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _playerTexture.id);
	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");

	//I accidentally had the texture location set to 1, this came up with a black screen.
	//If you are doing multitexture, you would set the texture location equal to the active texture set above.
	glUniform1i(textureLocation, 0);


	//We only need to set the uniform whenever we need to change it.
	//So first we find the location of the variable
	GLint timeLocation = _colorProgram.getUniformLocation("time");
	////After getting the location, we need to send it a new value, sending 1 float hence "1f".
	glUniform1f(timeLocation,_time);

	_sprite.draw();

	glBindTexture(GL_TEXTURE_2D, 0);

	_colorProgram.unuse();

	//Swap our buffer and draw everything to the screen!
	SDL_GL_SwapWindow(_window);
}

void MainGame::gameLoop() {
	while (_gameState != GameState::EXIT) {
		proccessInput();
		_time += 0.05f;
		drawGame();
	}
}