#pragma once

#include <SDL\SDL.h>
#include <GL\glew.h>
#include <glm/glm.hpp>

#include <GameEngine\GameEngine.h>
#include <GameEngine/GLSLProgram.h>
#include <GameEngine/GLTexture.h>
#include <GameEngine/Sprite.h>
#include <GameEngine/Window.h>

#include <vector>

enum class GameState{PLAY,EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:
	void initSystems();
	void initShaders();
	void gameLoop();
	void proccessInput();
	void drawGame();
	void calculateFPS();

	GameEngine::Window _window;
	int _screenWidth;
	int _screenHeight;
	GameState _gameState;

	std::vector <GameEngine::Sprite*> _sprites;

	GameEngine::GLSLProgram _colorProgram;
	GameEngine::GLTexture _playerTexture;

	float _fps;
	float _maxFPS;
	float _frameTime;

	float _time;
};