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
#include "AABB.h"

class IndexedModel
{
public:
	IndexedModel() : instancedElementsStartIndex((unsigned int)-1) {}

	unsigned int CreateVertexArray(RenderDevice& device, RenderDevice::BufferUsage usage) const;

	void AllocateElement(unsigned int elementSize);

	void AddElement1f(unsigned int elementIndex, float e0);
	void AddElement2f(unsigned int elementIndex, float e0, float e1);
	void AddElement3f(unsigned int elementIndex, float e0, float e1, float e2);
	void AddElement4f(unsigned int elementIndex, float e0, float e1, float e2, float e3);

	void AddIndices1i(unsigned int i0);
	void AddIndices2i(unsigned int i0, unsigned int i1);
	void AddIndices3i(unsigned int i0, unsigned int i1, unsigned int i2);
	void AddIndices4i(unsigned int i0, unsigned int i1, unsigned int i2, unsigned int i3);

	AABB GetAABBForElementArray(unsigned int index);

	inline unsigned int GetNumIndices() const { return indices.size(); }

	inline void SetInstancedElementStartIndex(unsigned int elementIndex)
	{
		instancedElementsStartIndex = elementIndex;
	}

private:
	std::vector<unsigned int> indices;
	std::vector<unsigned int> elementSizes;
	std::vector<std::vector<float>> elements;
	unsigned int instancedElementsStartIndex;
};
