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

/**
 * @brief Used to store uniform data for a shader program. A uniform buffer can be used to share
 * uniforms between different programs, as well as quickly change between sets of uniforms for the
 * same program object. 
 */
class UniformBuffer
{
public:
	/**
	 * @param device Render device to use.
	 * @param dataSize The size in bytes of the buffer.
	 * @param usage Hints for what the user will be doing with the buffer.
	 * @param data A pointer to data that will be copied into the data store for initialization, or
	 *		nullptr if no data is to be copied.
	 */
	UniformBuffer(RenderDevice& device, size_t dataSize, RenderDevice::BufferUsage usage,
		const void* data = nullptr) : device(&device), size(dataSize)
	{
		deviceID = this->device->CreateUniformBuffer(data, dataSize, usage);
	}

	virtual ~UniformBuffer()
	{
		deviceID = device->ReleaseUniformBuffer(deviceID);
	}

	void Update(const void* data, size_t dataSize)
	{
		device->UpdateUniformBuffer(deviceID, data, dataSize);
	}

	void Update(const void* data) { Update(data, size); }

	unsigned int GetID() { return deviceID; }

private:
	// Disallow copy and assign
	UniformBuffer(const UniformBuffer& other) = delete;
	void operator=(const UniformBuffer& other) = delete;

	RenderDevice* device;
	unsigned int deviceID;
	size_t size;
};
