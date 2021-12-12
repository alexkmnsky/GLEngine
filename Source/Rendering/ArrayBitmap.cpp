#include "ArrayBitmap.h"

#include <cstring> // std::memcpy

ArrayBitmap::ArrayBitmap(int width, int height) : width(width), height(height)
{
	assert(width > 0 && height > 0);
	pixels = new int[GetPixelsSize()];
}

ArrayBitmap::ArrayBitmap(int width, int height, int* pixels) : width(width), height(height)
{
	assert(width > 0 && height > 0);
	assert(pixels != nullptr);

	size_t size = GetPixelsSize();

	this->pixels = new int[size];
	std::memcpy(this->pixels, pixels, size);
}

ArrayBitmap::ArrayBitmap(int width, int height, int* pixels, int offsetX, int offsetY, 
	int rowOffset) : width(width), height(height)
{
	assert(width > 0 && height > 0);
	assert(pixels != nullptr);
	assert(offsetX > 0 && offsetY > 0 && rowOffset > 0);

	size_t size = GetPixelsSize();

	this->pixels = new int[size];
	int* pixelsSource = pixels + offsetY + offsetX * rowOffset;

	for (size_t i = 0; i < (size_t)height; ++i, pixels += width, pixelsSource += rowOffset)
	{
		std::memcpy(this->pixels, pixelsSource, (size_t)width);
	}
}

ArrayBitmap::~ArrayBitmap()
{
	delete[] pixels;
	pixels = nullptr;
}
