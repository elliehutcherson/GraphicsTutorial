#include "MainGame.h"
#include <GameEngine/Errors.h>
#include <GameEngine/ResourceManager.h>

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

	gameLoop();
}

void MainGame::initSystems() {

	GameEngine::init();

	//This is where we initialize things the game needs, like a window.
	//This was a lot more complicated, but that complication has moved to the game engine.
	_window.create("Game Engine", _screenWidth, _screenHeight, 0); 

	initShaders();
	_spriteBatch.init();
	_fpsLimiter.init(_maxFPS);
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

	const float CAMERA_SPEED = 2.0f;
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
				_inputManager.pressKey(myEvent.key.keysym.sym);
				break;
			case SDL_KEYUP:
				_inputManager.releaseKey(myEvent.key.keysym.sym);
				break;
		}
	}

	if (_inputManager.isKeyPressed(SDLK_w)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
	}
	if (_inputManager.isKeyPressed(SDLK_s)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
	}
	if (_inputManager.isKeyPressed(SDLK_a)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
	}
	if (_inputManager.isKeyPressed(SDLK_d)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
	}
	if (_inputManager.isKeyPressed(SDLK_q)) {
		_camera.setScale(_camera.getScale() + SCALE_SPEED);
	}
	if (_inputManager.isKeyPressed(SDLK_e)) {
		_camera.setScale(_camera.getScale() - SCALE_SPEED);
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

	_spriteBatch.begin();

	glm::vec4 pos(0.0f, 0.0f, 50.f, 50.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	GameEngine::GLTexture texture = GameEngine::ResourceManager::getTexture("Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
	GameEngine::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	_spriteBatch.draw(pos, uv, texture.id, 0.0f, color);

	_spriteBatch.end();

	_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);

	_colorProgram.unuse();

	//Swap the buffer!
	_window.swapBuffer();

}

void MainGame::gameLoop() {
	while (_gameState != GameState::EXIT) {
		_fpsLimiter.begin();

		proccessInput();
		_time += 0.01f;

		_camera.update();

		drawGame();

		_fps = _fpsLimiter.end();
		
		static int frameCount = 0;
		frameCount++;
		if (frameCount == 10) {
			std::cout << _fps << std::endl;
			frameCount = 0;
		}
	}
}

