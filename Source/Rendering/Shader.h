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

#pragma once

#include "RenderDevice.h"
#include "UniformBuffer.h"
#include "Texture.h"
#include "Sampler.h"

#include <string>

class Shader
{
public:
	/**
	 * Loads a shader from a file.
	 *
	 * @param device: The target render device to create the shader for.
	 * @param fileName: File path to the shader. VERTEX_SHADER_BUILD will be defined when compiling
	 * the vertex shader. FRAGMENT_SHADER_BUILD will be defined when compiling the fragment shader.
	 */
	Shader(RenderDevice& device, const std::string& fileName);
	virtual ~Shader();

	inline void SetUniformBuffer(const std::string& name, UniformBuffer& buffer)
	{
		device->SetShaderUniformBuffer(deviceID, name, buffer.GetID());
	}

	inline void SetSampler(const std::string& name, Texture& texture, Sampler& sampler,
		unsigned int unit)
	{
		device->SetShaderSampler(deviceID, name, texture.GetID(), sampler.GetID(), unit);
	}

	inline unsigned int GetID() { return deviceID; }

private:
	// Disallow copy and assign
	Shader(const Shader& other) = delete;
	void operator=(const Shader& other) = delete;

	RenderDevice* device;
	unsigned int deviceID;
};

