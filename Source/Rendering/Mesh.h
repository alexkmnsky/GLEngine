#pragma once

#include "AABB.h"
#include "IndexedModel.h"

#include <GLM/glm.hpp>
#include <GL/glew.h>
#include <string>
#include <vector>

/**
 * @brief Container for vertex data. 
 * 
 * Contains a position, texture coordinate, normal, and tangent.
 */
class Vertex
{
public:
	/** @brief Vertex constructor. Normal defaults to zero. */
	Vertex(const glm::vec3& position, const glm::vec2& textureCoordinate,
		const glm::vec3& normal = glm::vec3(0, 0, 0), const glm::vec3& tangent = glm::vec3(0, 0, 0))
	{
		this->position = position;
		this->textureCoordinate = textureCoordinate;
		this->normal = normal;
		this->tangent = tangent;
	}

	inline glm::vec3* GetPosition() { return &position; }
	inline glm::vec2* GetTextureCoordinate() { return &textureCoordinate; }
	inline glm::vec3* GetNormal() { return &normal; }
	inline glm::vec3* GetTangent() { return &tangent; }

protected:
private:
	glm::vec3 position;
	glm::vec2 textureCoordinate;
	glm::vec3 normal;
	glm::vec3 tangent;
};

/**
 * Loads all meshes from a file.
 * 
 * @param fileName File path to the model.
 * @returns A list of all meshes, in IndexedModel form.
 */
std::vector<IndexedModel> LoadModels(const std::string& fileName);


//class Mesh
//{
//public:
//	/**
//	 * Creates a mesh from an array of vertices and indices.
//	 * 
//	 * @param vertices Array of vertices.
//	 * @param numberOfVertices Number of elements in the vertices array.
//	 * @param indices Array of vertex indices.
//	 * @param numberOfIndices Number of elements in the indices array.
//	 */
//	Mesh(Vertex* vertices, unsigned int numberOfVertices, unsigned int* indices,
//		unsigned int numberOfIndices);
//
//	/**
//	 * Loads a mesh from a file.
//	 * If the file contains more than one model, the first model in the file will be used.
//	 * 
//	 * To load multiple models, see LoadModels.
//	 * 
//	 * @param fileName File path to the model.
//	 */
//	Mesh(const std::string& fileName);
//
//	/**
//	 * Creates a mesh using an IndexedModel.
//	 * 
//	 * @param model The model to prepare for rendering.
//	 */
//	inline Mesh(IndexedModel& model)
//	{
//		InitMesh(model);
//		this->model = model;
//	}
//
//	/**
//	 * @brief Instructs OpenGL to draw the mesh/triangles.
//	 * Note that a shader and texture should be bound and/or updated before drawing the mesh.
//	 */
//	void Draw();
//
//	/**
//	 * Finds a bounding box using the model's positions.
//	 * 
//	 * @see AABB::AABB(std::vector<glm::vec3>& points)
//	 * @return AABB enclosing the model.
//	 */
//	inline AABB GetAABB() {	return AABB(model.positions); }
//
//	virtual ~Mesh();
//
//protected:
//private:
//	// Disallow copy and assign
//	Mesh(const Mesh& other) = delete;
//	void operator=(const Mesh& other) = delete;
//
//	void UpdateVertexArrayBuffer(unsigned int bufferIndex, const void* data, uintptr_t dataSize,
//		unsigned int usage = GL_DYNAMIC_DRAW);
//	void InitMesh(const IndexedModel& model);
//
//	enum
//	{
//		POSITION_VB,
//		TEXTURECOORDINATE_VB,
//		NORMAL_VB,
//
//		INDEX_VB,
//
//		NUM_BUFFERS
//	};
//
//	IndexedModel model;
//
//	GLuint vertexArrayObject;
//	GLuint vertexArrayBuffers[NUM_BUFFERS];
//	unsigned int drawCount;
//};
//
