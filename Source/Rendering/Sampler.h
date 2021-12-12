#pragma once

#include "RenderDevice.h"

class Sampler
{
public:
	Sampler(RenderDevice& device,
		RenderDevice::SamplerFilter minFilter = RenderDevice::FILTER_NEAREST_MIPMAP_LINEAR,
		RenderDevice::SamplerFilter magFilter = RenderDevice::FILTER_LINEAR,
		RenderDevice::SamplerWrapMode wrapU = RenderDevice::WRAP_CLAMP,
		RenderDevice::SamplerWrapMode wrapV = RenderDevice::WRAP_CLAMP,
		float anisotropy = 0.0f) : device(&device)
	{
		deviceID = this->device->CreateSampler(minFilter, magFilter, wrapU, wrapV, anisotropy);
	}

	virtual ~Sampler()
	{
		deviceID = device->ReleaseSampler(deviceID);
	}

	inline unsigned int GetID() { return deviceID; }

private:
	// Disallow copy and assign
	Sampler(const Sampler& other) = delete;
	void operator=(const Sampler& other) = delete;

	RenderDevice* device;
	unsigned int deviceID;
};
