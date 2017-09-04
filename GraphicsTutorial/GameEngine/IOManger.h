#pragma once

#include <vector>
#include <string>

namespace GameEngine {

	class IOManger
	{
	public:
		//So that we can return a boolean variable and fill the
		//provided by reference vector with the file contents.
		//Because we are reading binary data, unsigned char is more fitting.
		static bool readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer);
	};

}