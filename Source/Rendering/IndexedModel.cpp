#include "IndexedModel.h"

#include <glm/gtc/type_ptr.hpp>
#include <cassert>

unsigned int IndexedModel::CreateVertexArray(RenderDevice& device, 
	RenderDevice::BufferUsage usage) const
{
	// Number of components common across all instances. Typically these are components such as 
	// vertex positions, vertex texture coordinates, etc.
	unsigned int numVertexComponents = elementSizes.size(); 

	// Number of instance components (unique to all instances). For example, an instanced
	// component could be a transformation matrix for each instance.
	unsigned int numInstanceComponents = instancedElementsStartIndex == (unsigned int)-1
		? 0 // If the start index is set to (unsigned int)-1 there are no instance components
		: (numVertexComponents - instancedElementsStartIndex);

	numVertexComponents -= numInstanceComponents;

	std::vector<const float*> vertexDataVector;

	for (unsigned int i = 0; i < numVertexComponents; i++)
	{
		vertexDataVector.push_back(&(elements[i][0]));
	}

	const float** vertexData = vertexDataVector.data();
	const unsigned int* vertexElementSizes = elementSizes.data();

	unsigned int numVertices = elements[0].size() / vertexElementSizes[0];
	unsigned int numIndices = indices.size();

	return device.CreateVertexArray(vertexData, vertexElementSizes, numVertexComponents,
		numInstanceComponents, numVertices, indices.data(), numIndices, usage);
}

void IndexedModel::AllocateElement(unsigned int elementSize)
{
	elementSizes.push_back(elementSize);
	elements.push_back(std::vector<float>());
}

void IndexedModel::AddElement1f(unsigned int elementIndex, float e0)
{
	assert(elementIndex < elementSizes.size());
	elements[elementIndex].push_back(e0);
}

void IndexedModel::AddElement2f(unsigned int elementIndex, float e0, float e1)
{
	assert(elementIndex < elementSizes.size());
	elements[elementIndex].push_back(e0);
	elements[elementIndex].push_back(e1);
}

void IndexedModel::AddElement3f(unsigned int elementIndex, float e0, float e1, float e2)
{
	assert(elementIndex < elementSizes.size());
	elements[elementIndex].push_back(e0);
	elements[elementIndex].push_back(e1);
	elements[elementIndex].push_back(e2);
}

void IndexedModel::AddElement4f(unsigned int elementIndex, float e0, float e1, float e2, float e3)
{
	assert(elementIndex < elementSizes.size());
	elements[elementIndex].push_back(e0);
	elements[elementIndex].push_back(e1);
	elements[elementIndex].push_back(e2);
	elements[elementIndex].push_back(e3);
}

void IndexedModel::AddIndices1i(unsigned int i0)
{
	indices.push_back(i0);
}

void IndexedModel::AddIndices2i(unsigned int i0, unsigned int i1)
{
	indices.push_back(i0);
	indices.push_back(i1);
}

void IndexedModel::AddIndices3i(unsigned int i0, unsigned int i1, unsigned int i2)
{
	indices.push_back(i0);
	indices.push_back(i1);
	indices.push_back(i2);
}

void IndexedModel::AddIndices4i(unsigned int i0, unsigned int i1, unsigned int i2, unsigned int i3)
{
	indices.push_back(i0);
	indices.push_back(i1);
	indices.push_back(i2);
	indices.push_back(i3);
}

AABB IndexedModel::GetAABBForElementArray(unsigned int index)
{
	if (elementSizes[index] != 3)
	{
		return AABB(); // Empty AABB as we lack 3D points
	}

	std::vector<glm::vec3> points;
	for (unsigned int i = 0; i < elements[index].size() - 3; i += 3)
	{
		// Convert each set of 3 floats into a vec3
		points.push_back(glm::make_vec3(elements[index].data() + i));
	}

	return AABB(points);
}

/*void IndexedModel::CalculateNormals()
{
	// Iterate over all triangles in the model
	for (unsigned int i = 0; i < indices.size(); i += 3)
	{
		// Get the three vertex indices of the triangle in the model
		int i0 = indices[i];
		int i1 = indices[i + 1];
		int i2 = indices[i + 2];

		// Calculate the normal of the triangle
		glm::vec3 v1 = positions[i1] - positions[i0];
		glm::vec3 v2 = positions[i2] - positions[i0];

		glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

		// Set the normal for each vertex
		normals[i0] += normal;
		normals[i1] += normal;
		normals[i2] += normal;
	}

	// Iterate over all vertices and normalize the normal
	for (unsigned int i = 0; i < positions.size(); i++)
		normals[i] = glm::normalize(normals[i]);
}*/
