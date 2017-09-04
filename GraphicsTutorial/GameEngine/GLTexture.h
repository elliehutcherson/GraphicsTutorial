#pragma once
#include <GL\glew.h>

namespace GameEngine {

	//GLTexture struct type redefinition error - fixed with pragma once

	struct GLTexture {
		GLuint id;
		int width;
		int height;
	};

}