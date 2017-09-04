#include "GLSLProgram.h"
#include "Errors.h"

#include <fstream>
#include <vector>

namespace GameEngine {

	//You can use an initialization list for constructors.
	GLSLProgram::GLSLProgram() :
		_numAttributes(0),
		_programId(0),
		_vertexShaderId(0),
		_fragmentShaderId(0)
	{
	}


	GLSLProgram::~GLSLProgram()
	{
	}

	void GLSLProgram::addAttribute(const std::string& attributeName) {
		//So we need to bind the the program we instantiated, give the index of the attribute, and the attribute name.
		//_numAttributes++, ++_numAttributes, one adds after the line has been completed, the other adds before the line is completed.
		glBindAttribLocation(_programId, _numAttributes++, attributeName.c_str());
	}

	void GLSLProgram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {
		//Get a program object.
		_programId = glCreateProgram();

		_vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
		if (_vertexShaderId == 0) {
			fatalError("Vertex shader failed to be created!");
		}

		_fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
		if (_fragmentShaderId == 0) {
			fatalError("Fragment shader failed to be created!");
		}

		compileShader(vertexShaderFilePath, _vertexShaderId);
		compileShader(fragmentShaderFilePath, _fragmentShaderId);
	}

	void GLSLProgram::compileShader(const std::string& filePath, GLuint shaderId) {
		//Now that a vertex shader and a fragment shader have been created,
		//we need to load the code that we created in the shader files.
		//The fstream variable is defined at the same time the file is opened. 

		std::ifstream shaderFile(filePath);
		if (shaderFile.fail()) {
			perror(filePath.c_str());
			fatalError("Failed to open " + filePath);
		}

		//initializing variables.
		std::string fileContents = "";
		std::string line;

		//getline does not include the carriage return
		while (std::getline(shaderFile, line)) {
			fileContents += line + "\n";
		}

		shaderFile.close();

		//For some reason you can't just use &fileContents.c_str()
		//So we just create a char* variable and store it, then put it in there
		//For some reason that works. Since we only have on string in our c_str
		//we can just use nullptr instead of an array containing the length of all strings.
		const char* contentsPtr = fileContents.c_str();
		glShaderSource(shaderId, 1, &contentsPtr, nullptr);
		glCompileShader(shaderId);

		//Get Integer value of the CompileStatus, and throw it into success.
		GLint success = 0;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			//OpenGL creates a log whenever it compiles with errors and events.
			//If the id returned for the compile status fails, then we throw
			//the glchar vector into the log.

			//We are getting the length of the log created when we compiled the
			//shader. The we are going to initialize a vector that length (errorLog)
			//and store that log info in there.
			GLint maxLength = 0;
			glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character. I don't know why that's important.
			std::vector<char> errorLog(maxLength);
			glGetShaderInfoLog(shaderId, maxLength, &maxLength, &errorLog[0]);

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(shaderId); // Don't leak the shader.

			std::printf("%s\n", &(errorLog[0]));
			fatalError("Shader " + filePath + " failed to compile");
		}
	}

	void GLSLProgram::linkShaders() {
		//Attach our shaders to our program
		glAttachShader(_programId, _vertexShaderId);
		glAttachShader(_programId, _fragmentShaderId);

		//Link our program
		glLinkProgram(_programId);

		//Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(_programId, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &maxLength);

			//The maxLength includes the NULL character
			std::vector<char> errorLog(maxLength);
			glGetProgramInfoLog(_programId, maxLength, &maxLength, &errorLog[0]);

			//We don't need the program anymore.
			glDeleteProgram(_programId);
			//Don't leak shaders either.
			glDeleteShader(_vertexShaderId);
			glDeleteShader(_fragmentShaderId);

			std::printf("%s\n", &(errorLog[0]));
			fatalError("Shaders failed to link!");
		}

		//Always detach shaders after a successful link.
		glDetachShader(_programId, _vertexShaderId);
		glDetachShader(_programId, _fragmentShaderId);

		//Make sure you free up resources by releasing the memory.
		glDeleteShader(_vertexShaderId);
		glDeleteShader(_fragmentShaderId);
	}

	GLuint GLSLProgram::getUniformLocation(const std::string& uniformName) {
		GLint location = glGetUniformLocation(_programId, uniformName.c_str());
		if (location == GL_INVALID_INDEX) {
			fatalError("Uniform " + uniformName + " not found in shader!");
		}
		return location;
	}


	void GLSLProgram::use() {
		glUseProgram(_programId);
		for (int i = 0; i < _numAttributes; i++) {
			glEnableVertexAttribArray(i);
		}
	}

	void GLSLProgram::unuse() {
		glUseProgram(0);
		for (int i = 0; i < _numAttributes; i++) {
			glDisableVertexAttribArray(i);
		}
	}

}