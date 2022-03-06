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
