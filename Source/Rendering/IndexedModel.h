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
