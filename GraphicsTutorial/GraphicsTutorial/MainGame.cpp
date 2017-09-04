#include "MainGame.h"
#include <GameEngine/Errors.h>

#include <iostream>
#include <string>

MainGame::MainGame() :
	_screenWidth(1024),
	_screenHeight(768),
	_time(0.0f),
	_gameState(GameState::PLAY),
	_maxFPS(60.0f)
{
}

MainGame::~MainGame()
{
}

void MainGame::run() {
	initSystems();

	_sprites.push_back(new GameEngine::Sprite());
	_sprites.back()->init(-1.0f, -1.0f, 1.0f, 1.0f, "Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
	
	_sprites.push_back(new GameEngine::Sprite());
	_sprites.back()->init(0.0f, 0.0f, 1.0f, 1.0f, "Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

	_sprites.push_back(new GameEngine::Sprite());
	_sprites.back()->init(0.0f, -1.0f, 1.0f, 1.0f, "Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

	gameLoop();
}

void MainGame::initSystems() {

	GameEngine::init();

	//This is where we initialize things the game needs, like a window.
	//This was a lot more complicated, but that complication has moved to the game engine.
	_window.create("Game Engine", _screenWidth, _screenHeight, 0); 

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
				//std::cout << myEvent.motion.x << "," << myEvent.motion.y << std::endl;
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
	//The texture is now being bound in _sprite.draw()
	//Because you can have multiple textures bound at one time, we are going to use the first one.
	glActiveTexture(GL_TEXTURE0);
	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");

	//I accidentally had the texture location set to 1, this came up with a black screen.
	//If you are doing multitexture, you would set the texture location equal to the active texture set above.
	glUniform1i(textureLocation, 0);


	//We only need to set the uniform whenever we need to change it.
	//So first we find the location of the variable
	GLint timeLocation = _colorProgram.getUniformLocation("time");
	////After getting the location, we need to send it a new value, sending 1 float hence "1f".
	glUniform1f(timeLocation,_time);

	//for each sprite in _sprites, draw it.
	for (int i = 0; i < _sprites.size(); i++) {
		_sprites[i]->draw();
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	_colorProgram.unuse();

	//Swap the buffer!
	_window.swapBuffer();

}

void MainGame::gameLoop() {
	while (_gameState != GameState::EXIT) {
		//Used for frame time measurijng
		float startTicks = SDL_GetTicks();

		proccessInput();
		_time += 0.05f;
		drawGame();
		calculateFPS();

		static int frameCount = 0;
		frameCount++;
		if (frameCount == 10) {
			std::cout << _fps << std::endl;
			frameCount = 0;
		}

		//Limit fps to max fps
		float frameTicks = SDL_GetTicks() - startTicks;
		if (1000.0f / _maxFPS > frameTicks) {
			SDL_Delay(1000.0 / _maxFPS - frameTicks);
		}
	}
}

void MainGame::calculateFPS() {
	static const int NUM_SAMPLES = 100;
	static float frameTimes[NUM_SAMPLES];
	static int currentFrame = 0;

	static float prevTicks = SDL_GetTicks();

	float currentTicks = SDL_GetTicks();

	_frameTime = currentTicks - prevTicks;
	frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

	prevTicks = currentTicks;

	int count;
	currentFrame++;
	if (currentFrame < NUM_SAMPLES) {
		count = currentFrame;
	}
	else {
		count = NUM_SAMPLES;
	}

	float frameTimeAverage = 0;
	for (int i = 0; i < count; i++) {
		frameTimeAverage += frameTimes[i];
	}
	frameTimeAverage /= count;

	if (frameTimeAverage > 0) {
		_fps = 1000.0f / frameTimeAverage;
	}
	else {
		_fps = 60.0f;
	}

}