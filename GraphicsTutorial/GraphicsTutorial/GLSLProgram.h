#pragma once
#include <string>
#include <GL\glew.h>

//Basically, our program that was written in two text files needs to be compiled
//into a program that OpenGL can use.

class GLSLProgram
{
public:
	GLSLProgram();
	~GLSLProgram();

	void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

	void linkShaders();
	
	//We have to tell our GLSL program how many attributes that we are adding
	//and what they are. Things like color, texture, positions... etc.
	void addAttribute(const std::string& attributeName);

	//To  use a uniform variable, we have to retrieve its id or its location.
	GLuint getUniformLocation(const std::string& uniformName);

	//Apparently we have to tell openGL to use our programs that we create.
	//This is where that happens. This will appear in the drawGame function in MainGame.cpp
	//I think the real reason we need to create functions for this is because _programId is
	//a private varaible.
	void use();
	void unuse();

private:
	int _numAttributes;

	//Need an id to hold onto the actual shader program
	//As well as the individual shaders as well. Just like the VBOs.
	GLuint _programId; //Our shader program.

	GLuint _vertexShaderId;
	GLuint _fragmentShaderId;
	void compileShader(const std::string& filePath, GLuint shaderId);
};

