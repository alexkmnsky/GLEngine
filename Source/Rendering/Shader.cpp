#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

/**
 * Loads a shader file into a string.
 * 
 * @param fileName: File name, including the file path if necessary as well as the file extension.
 * @param includeKeyword: Include preprocessor keyword (ex. "#include").
 */
static std::string LoadShader(const std::string& fileName, const std::string& includeKeyword);

Shader::Shader(RenderDevice& device, const std::string& fileName) : device(&device)
{
	std::string shaderText = LoadShader(fileName, "#include");

	deviceID = this->device->CreateShaderProgram(shaderText);
}

Shader::~Shader()
{
	deviceID = device->ReleaseShaderProgram(deviceID);
}

static std::string LoadShader(const std::string& fileName, const std::string& includeKeyword)
{
	// Load file and store as string...
	std::ifstream file;
	std::filesystem::path filePath(fileName);
	file.open(fileName);

	std::stringstream ss;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			
			// Check for include keyword
			if (line.find(includeKeyword) == std::string::npos)
			{
				ss << line << std::endl;
			}
			else
			{
				// Get everything after the space
				std::string includeFileName = line.substr(line.find(' ') + 1);

				// Find opening quote
				unsigned int open = includeFileName.find('"');
				// Find closing quote
				unsigned int close = includeFileName.find('"', open + 1);

				// If quotes are missing, error
				if (open == std::string::npos || close == std::string::npos)
				{
					std::cerr << "Unable to parse shader include keyword: " << fileName 
						<< std::endl;
				}

				// Get file name inside of quotes
				includeFileName = includeFileName.substr(open + 1, close - open - 1);

				// Append the file to the shader
				ss << LoadShader(filePath.parent_path().string() + includeFileName, includeKeyword) 
					<< std::endl;
			}
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return ss.str();
}