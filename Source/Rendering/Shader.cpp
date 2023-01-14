/*  
 * Copyright (c) 2022-2023 Alexander Kaminsky and Maxwell Hunt
 *  
 * This file incorporates work covered by the following copyright and permission notice:  
 *  
 *     Copyright (c) 2018 Benny Bobaganoosh
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *     
 *     The above copyright notice and this permission notice shall be included in all
 *     copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *     SOFTWARE.
 */

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