#pragma once

#include "GLTexture.h"

#include <string>

namespace GameEngine {

	class ImageLoader
	{
	public:
		static GLTexture loadPNG(std::string filePath);
	};

}
