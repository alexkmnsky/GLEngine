#include "Shader.h"
#include <iostream>
#include <fstream>

/** @brief Loads a shader file into a string. */
static std::string LoadShader(const std::string& fileName);

/** @brief Checks a shader for any compilation errors. */
static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);

/** @brief Creates a shader on the GPU. */
static GLuint CreateShader(const std::string& text, GLenum shaderType);

Shader::Shader(const std::string& fileName)
{
	program = glCreateProgram();

	// Load files and create the shaders
	shaders[0] = CreateShader(LoadShader(fileName + ".vert"), GL_VERTEX_SHADER);
	shaders[1] = CreateShader(LoadShader(fileName + ".frag"), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < NUM_SHADERS; i++)
		glAttachShader(program, shaders[i]);

	glLinkProgram(program);
	CheckShaderError(program, GL_LINK_STATUS, true, "Error: Program linking failed: ");

	glValidateProgram(program);
	CheckShaderError(program, GL_VALIDATE_STATUS, true, "Error: Program is invalid: ");

	uniforms[TRANSFORM_U] = glGetUniformLocation(program, "transform");
}

void Shader::Bind()
{
	glUseProgram(program);
}

void Shader::Update(const glm::mat4& matrix)
{
	// Update the model view projection uniform
	glUniformMatrix4fv(uniforms[TRANSFORM_U], 1, GL_FALSE, &matrix[0][0]);
}

void Shader::Update(const Transform& transform, const Camera& camera)
{
	// Update the model view projection uniform
	Update(camera.GetViewProjection() * transform.GetModel());
}

Shader::~Shader()
{
	// Delete all shaders from the GPU
	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		glDetachShader(program, shaders[i]);
		glDeleteShader(shaders[i]);
	}

	glDeleteProgram(program);
}

GLuint CreateShader(const std::string& text, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	if (shader == 0)
		std::cerr << "Error: Shader creation failed!" << std::endl;

	const GLchar* shaderSourceStrings[1];
	GLint shaderSourceStringLengths[1];

	shaderSourceStrings[0] = text.c_str();
	shaderSourceStringLengths[0] = text.length();

	glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLengths);

	glCompileShader(shader);
	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error: Shader compilation failed: ");

	return shader;
}


static std::string LoadShader(const std::string& fileName)
{
	// Load file and store as string...
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unabled to load shader: " << fileName << std::endl;
	}

	return output;
}

void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}