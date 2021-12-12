#pragma once

#include <string>
#include <cassert>

class ArrayBitmap
{
public:
	ArrayBitmap(int width = 1, int height = 1);
	ArrayBitmap(int width, int height, int* pixels);
	ArrayBitmap(int width, int height, int* pixels, int offsetX, int offsetY, int rowOffset);
	virtual ~ArrayBitmap();

	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }

	inline int Get(int x, int y) const
	{
		assert(x >= 0 && x < width);
		assert(y >= 0 && y < height);
		return pixels[x + y * width];
	}

	inline void Set(int x, int y, int pixel)
	{
		assert(x >= 0 && x < width);
		assert(y >= 0 && y < height);
		pixels[x + y * width] = pixel;
	}

	inline int* GetPixelArray() { return pixels; }
	inline const int* GetPixelArray() const { return pixels; }

private:
	int width;
	int height;
	int* pixels;

	inline size_t GetPixelsSize() const
	{
		return (size_t)(width * height);
	}
};
