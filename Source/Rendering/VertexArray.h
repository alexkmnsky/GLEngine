#pragma once

#include "RenderDevice.h"
#include "IndexedModel.h"

class VertexArray
{
public:
	VertexArray(RenderDevice& device, const IndexedModel& model, RenderDevice::BufferUsage usage) :
		device(&device), numIndices(model.GetNumIndices())
	{
		deviceID = model.CreateVertexArray(device, usage);
	}

	virtual ~VertexArray()
	{
		deviceID = device->ReleaseVertexArray(deviceID);
	}
	
	inline void UpdateBuffer(unsigned int bufferIndex, const void* data, size_t dataSize)
	{
		device->UpdateVertexArrayBuffer(deviceID, bufferIndex, data, dataSize);
	}

	inline unsigned int GetID() { return deviceID; }
	inline unsigned int GetNumIndices() { return numIndices; }

private:
	// Disallow copy and assign
	VertexArray(const VertexArray& other) = delete;
	void operator=(const VertexArray& other) = delete;

	RenderDevice* device;
	unsigned int deviceID;
	unsigned int numIndices;
};
