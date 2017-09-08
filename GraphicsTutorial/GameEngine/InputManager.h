#pragma once

#include <glm/glm.hpp>

#include <unordered_map>

namespace GameEngine {

	class InputManager
	{
	public:
		InputManager();
		~InputManager();
	
		void pressKey(unsigned int keyId);
		void releaseKey(unsigned int keyId);

		void setMouseCoords(float x, float y);

		bool isKeyPressed(unsigned int keyId);

		//GETTERS 
		//because this isn't going to change anything within InputManager
		//we should list this as const, we don't have to though, it's just correct.
		glm::vec2 getMouseCoords() const { return _mouseCoords; }

	private:
		std::unordered_map<unsigned int, bool> _keyMap;
		glm::vec2 _mouseCoords;
	};

}