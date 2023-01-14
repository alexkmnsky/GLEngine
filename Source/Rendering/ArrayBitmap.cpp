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
