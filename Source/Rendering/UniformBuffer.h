#pragma once

#include "RenderDevice.h"

class UniformBuffer
{
public:
	UniformBuffer(RenderDevice& device, size_t dataSize, RenderDevice::BufferUsage usage,
		const void* data = nullptr) : device(&device), size(dataSize)
	{
		deviceID = this->device->CreateUniformBuffer(data, dataSize, usage);
	}

	virtual ~UniformBuffer()
	{
		deviceID = device->ReleaseUniformBuffer(deviceID);
	}

	inline void Update(const void* data, size_t dataSize)
	{
		device->UpdateUniformBuffer(deviceID, data, dataSize);
	}

	inline void Update(const void* data) { Update(data, size); }

	inline unsigned int GetID() { return deviceID; }

private:
	// Disallow copy and assign
	UniformBuffer(const UniformBuffer& other) = delete;
	void operator=(const UniformBuffer& other) = delete;

	RenderDevice* device;
	unsigned int deviceID;
	size_t size;
};
