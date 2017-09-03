#include "Errors.h"

#include <cstdlib>
#include <iostream>
#include <SDL/SDL.h>

void fatalError(std::string errorString) {
	//SDL_Quit will quit the game from anywhere in the code.
	std::cout << errorString << std::endl;
	std::cout << "Enter anykey to continue...";
	int tmp;
	std::cin >> tmp;
	SDL_Quit();
	exit(1);
}