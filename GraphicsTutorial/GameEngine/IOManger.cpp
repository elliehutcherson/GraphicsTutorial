#include "IOManger.h"
#include <fstream>

namespace GameEngine {

	bool IOManger::readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer) {
		//We are reading this to get a byte stream, most likely this is for images.
		std::ifstream myFile(filePath, std::ios::binary);
		if (myFile.fail()) {
			perror(filePath.c_str());
			return false;
		}

		//seek to the end, 0 bytes from the end (0, std::ios::end)
		myFile.seekg(0, std::ios::end);

		//Get the file size
		int fileSize = myFile.tellg();

		//seek to the beginning, 0 bytes from the beginning.
		myFile.seekg(0, std::ios::beg);

		//Reduce the file size by any header byes that might be present
		fileSize -= myFile.tellg();

		buffer.resize(fileSize);

		//A vector is basically a wrapper around an array, so if we 
		//reference the first element in the vector and point to that
		//address, it will give us the c_str(), because it is a pointer
		//to a char that is beginning the string.

		//So read apparently takes a char* but when we are giving this function
		//the buffer variable, it will be of type unsigned char because we are 
		//reading binary data. So we have to convert this to a char*
		myFile.read((char *)&(buffer[0]), fileSize);
		myFile.close();

		return true;
	}

}
