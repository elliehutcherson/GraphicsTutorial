#include "InputManager.h"

namespace GameEngine {

	InputManager::InputManager()
	{
	}


	InputManager::~InputManager()
	{
	}

	void InputManager::pressKey(unsigned int keyId) {
		//_keyMap.insert
		//instead we can check to see if it exists, and if not then we insert true
		_keyMap[keyId] = true;
	}


	void InputManager::releaseKey(unsigned int keyId) {
		_keyMap[keyId] = false;
	}

	bool InputManager::isKeyPressed(unsigned int keyId) {
		//it will return a pointer, that's why we have to use ->
		//first and second are map keywords
		//first will return the key, second will return the value
		auto it = _keyMap.find(keyId);
		if (it != _keyMap.end()) {
			return it->second;
		} else {
			return false;
		}
	}

}