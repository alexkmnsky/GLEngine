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

#include "Texture.h"
#include <iostream>
#include <cstring> // std::memcpy

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(RenderDevice& device, const ArrayBitmap& textureData, 
	RenderDevice::PixelFormat internalPixelFormat, bool generateMipmaps, bool shouldCompress) : 
	device(&device), width(textureData.GetWidth()), height(textureData.GetHeight()),
	isCompressed(shouldCompress), hasMipmaps(generateMipmaps)
{
	textureID = this->device->CreateTexture2D(width, height, textureData.GetPixelArray(),
		RenderDevice::FORMAT_RGBA, internalPixelFormat, generateMipmaps, shouldCompress, 0, 0);
}

Texture::Texture(RenderDevice& device, const std::string& fileName, 
	RenderDevice::PixelFormat internalPixelFormat, bool generateMipmaps, bool shouldCompress) :
	device(&device), isCompressed(shouldCompress), hasMipmaps(generateMipmaps)
{
	int textureWidth, textureHeight, bytesPerPixel;
	unsigned char* imageData = stbi_load(fileName.c_str(), &textureWidth, &textureHeight,
		&bytesPerPixel, 4);

	if (imageData == nullptr)
	{
		std::cerr << "Texture loading failed for texture: " << fileName << std::endl;
	}

	width = textureWidth;
	height = textureHeight;

	textureID = this->device->CreateTexture2D(width, height, imageData,	RenderDevice::FORMAT_RGBA, 
		internalPixelFormat, generateMipmaps, shouldCompress, 0, 0);

	stbi_image_free(imageData);
}

Texture::~Texture()
{
	textureID = device->ReleaseTexture2D(textureID);
}
