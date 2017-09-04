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
	_camera.init(_screenWidth,_screenHeight);
}

MainGame::~MainGame()
{
}

void MainGame::run() {
	initSystems();

	//Now we are passing in real world coordinates because of our camera class
	_sprites.push_back(new GameEngine::Sprite());
	_sprites.back()->init(0.0f, 0.0f, _screenWidth/4, _screenHeight/4, "Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
	
	_sprites.push_back(new GameEngine::Sprite());
	_sprites.back()->init(_screenWidth/4, 1.0f, _screenWidth/4, _screenHeight/4, "Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

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

	const float CAMERA_SPEED = 20.f;
	const float SCALE_SPEED = 0.05f;

	while (SDL_PollEvent(&myEvent)) {
		switch (myEvent.type) {
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
			case SDL_MOUSEMOTION:
				//std::cout << myEvent.motion.x << "," << myEvent.motion.y << std::endl;
				break;
			case SDL_KEYDOWN:
				//to check the key
				switch (myEvent.key.keysym.sym) {
					case SDLK_w:
						_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
						break;
					case SDLK_s:
						_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
						break;
					case SDLK_a:
						_camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
						break;
					case SDLK_d:
						_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
						break;
					case SDLK_q:
						_camera.setScale(_camera.getScale() + SCALE_SPEED);
						break;
					case SDLK_e:
						_camera.setScale(_camera.getScale() - SCALE_SPEED);
				}
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

	//Set the constantly changing time variable
	//We only need to set the uniform whenever we need to change it.
	//So first we find the location of the variable
	GLint timeLocation = _colorProgram.getUniformLocation("time");
	////After getting the location, we need to send it a new value, sending 1 float hence "1f".
	glUniform1f(timeLocation,_time);

	//This is the P variable in our colorshading.vert. The P variable is for our orthogrphaic
	//matrix from the Camera2D class.
	GLint pLocation = _colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();
	
	//now we need to upload the matrix to the gpu with gluniform calls
	//The last parameter is the value, which is our cameraMatrix. We need to pass
	// it in byref, and to do that we pass in its a pointer to its first index, just
	//like we would with an other vector/array. A matrix is a two-dimensional array bassically, 
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));
	

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

		_camera.update();

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