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
