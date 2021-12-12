#pragma once

#include "RenderDevice.h"
#include "ArrayBitmap.h"

#include <string>

class Texture
{
public:
	/**
	 * Loads a texture from a file.
	 *
	 * @param fileName File path to the texture.
	 */
	Texture(RenderDevice& device, const ArrayBitmap& textureData,
		RenderDevice::PixelFormat internalPixelFomat, bool generateMipmaps, bool shouldCompress);
	
	Texture(RenderDevice& device, const std::string& fileName,
		RenderDevice::PixelFormat internalPixelFomat, bool generateMipmaps, bool shouldCompress);

	virtual ~Texture();

	inline unsigned int GetID() { return textureID; }
	inline unsigned int GetWidth() const { return width; }
	inline unsigned int GetHeight() const { return height; }
	inline bool IsCompressed() const { return isCompressed; }
	inline bool HasMipmaps() const { return hasMipmaps; }

private:
	// Disallow copy and assign
	Texture(const Texture& other) = delete;
	void operator=(const Texture& other) = delete;

	RenderDevice* device;
	unsigned int textureID;
	unsigned int width;
	unsigned int height;
	bool isCompressed;
	bool hasMipmaps;
};

