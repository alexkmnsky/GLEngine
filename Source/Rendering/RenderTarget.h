#pragma once

#include "RenderDevice.h"
#include "Texture.h"

#include <stdexcept>
#include <iostream>

class RenderTarget
{
public:
	RenderTarget(RenderDevice& device) : device(&device), deviceID(0) {}

	RenderTarget(RenderDevice& device, Texture& texture, unsigned int width, unsigned int height,
		RenderDevice::FramebufferAttachment attachment = RenderDevice::ATTACHMENT_COLOR,
		unsigned int attachmentNumber = 0, unsigned int mipLevel = 0) : device(&device)
	{
		deviceID = this->device->CreateRenderTarget(texture.GetID(), width, height, attachment,
			attachmentNumber, mipLevel);

		CheckCompressed(texture);
	}

	RenderTarget(RenderDevice& device, Texture& texture,
		RenderDevice::FramebufferAttachment attachment = RenderDevice::ATTACHMENT_COLOR,
		unsigned int attachmentNumber = 0, unsigned int mipLevel = 0) : device(&device)
	{
		deviceID = this->device->CreateRenderTarget(texture.GetID(), texture.GetWidth(), 
			texture.GetHeight(), attachment, attachmentNumber, mipLevel);

		CheckCompressed(texture);
	}

	virtual ~RenderTarget()
	{
		deviceID = device->ReleaseRenderTarget(deviceID);
	}

	inline void UpdateSize(unsigned int width, unsigned int height)
	{
		device->UpdateRenderTarget(deviceID, width, height);
	}

	inline void CheckCompressed(const Texture& texture)
	{
		if (texture.IsCompressed())
		{
			std::cerr << "Error: Compressed textures cannot be used as render targets."
				<< std::endl;

			throw std::invalid_argument("Compressed textures cannot be used as render targets.");
		}

		if (texture.HasMipmaps())
		{
			std::cerr << "Warning: Rendering to a texture with mipmaps will not render to all "
				"mipmap levels! Unexpected results may occur." << std::endl;
		}
	}

	inline unsigned int GetID() { return deviceID; }

private:
	// Disallow copy and assign
	RenderTarget(const RenderTarget& other) = delete;
	void operator=(const RenderTarget& other) = delete;

	RenderDevice* device;
	unsigned int deviceID;
};
