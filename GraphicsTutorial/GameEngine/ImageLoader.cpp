#include "ImageLoader.h"
#include "picoPNG.h"
#include "IOManger.h"
#include "Errors.h"

namespace GameEngine {

	/* binding entities is what makes them available to the GPU
	1:  generate a texture and assign its identifier to an unsigned integer variable.
	2:  bind the texture to the GL_TEXTURE bind point (or some such bind point).
	3:  specify the size and format of the texture bound to GL_TEXTURE target.
	4:  copy some image we want on the texture to the GL_TEXTURE target.
	*/

	GLTexture ImageLoader::loadPNG(std::string filePath) {
		GLTexture texture = {};

		//input data - from image retrieved by IOManger::readFileToBuffer
		std::vector<unsigned char> in;
		std::vector<unsigned char> out;

		//can initialize two vars on same line
		unsigned long width, height;

		if (IOManger::readFileToBuffer(filePath, in) == false) {
			fatalError("Failed to load PNG file to buffer!");
		}

		int errorCode = decodePNG(out, width, height, &(in[0]), in.size());
		if (errorCode != 0) {
			//std::to_string to convert something to string, it would still be converted
			//because c++ knows an int can be a string, but that's how to make sure.
			fatalError("decodePNG failed with error: " + std::to_string(errorCode));
		}
		//So now our in vector has been filled with the decoded data, becauser we sent it by reference.

		//Now we are generating a texture. Generating 1 texture, and give it our GLTexture.id by reference.
		glGenTextures(1, &(texture.id));

		//Now we bind the texture.
		glBindTexture(GL_TEXTURE_2D, texture.id);

		//Upload the image to the openGL texture.
		//unsigned char is an unsigned byte, which is the type of data we are feeding it.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

		//I think we are telling openGL how we want our image to be rendered, hence parameters.
		//GL_TEXTURE_WRAP - Is at texture wrapping parameter. How do we want the texture to wrap on one image.
		//example: if a texture extends beyond the given coordinates, do we repeat the image? cut off the rest of the image? etc.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//MipMaping, based on size of texture, in this case linear interpolation. 
		//A bad setting/paramter would be to use the next pixel in the case of mipmaping, making the texture look awful, instead of averaging.
		//Two settings, magnifying and minimizing. Too big vs too small, and what to do in either case.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);


		//Mipmaping is basically averaging pixels when an image is rendered smaller than it's native resolution.
		//If mipmaping isn't on then the image looks weird and gross.
		glGenerateMipmap(GL_TEXTURE_2D);

		//Now we release the texture, even though it would probably be released anyhow because of the stack.
		glBindTexture(GL_TEXTURE_2D, 0);

		texture.width = width;
		texture.height = height;

		return texture;
	}

}