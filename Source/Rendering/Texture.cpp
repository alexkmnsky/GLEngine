#include "Texture.h"
#include <iostream>
#include <cstring> // std::memcpy

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(RenderDevice& device, const ArrayBitmap& textureData, 
	RenderDevice::PixelFormat internalPixelFomat, bool generateMipmaps, bool shouldCompress) : 
	device(&device), width(textureData.GetWidth()), height(textureData.GetHeight()),
	isCompressed(shouldCompress), hasMipmaps(generateMipmaps)
{
	textureID = this->device->CreateTexture2D(width, height, textureData.GetPixelArray(),
		RenderDevice::FORMAT_RGBA, internalPixelFomat, generateMipmaps, shouldCompress);
}

Texture::Texture(RenderDevice& device, const std::string& fileName, 
	RenderDevice::PixelFormat internalPixelFomat, bool generateMipmaps, bool shouldCompress) :
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
		internalPixelFomat, generateMipmaps, shouldCompress);

	stbi_image_free(imageData);
}

Texture::~Texture()
{
	textureID = device->ReleaseTexture2D(textureID);
}
