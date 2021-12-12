#include "Mesh.h"
#include <vector>
#include <iostream>

#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>

std::vector<IndexedModel> LoadModels(const std::string& fileName)
{
	std::vector<IndexedModel> models;
	Assimp::Importer importer;

	// Read our file
	// If the import fails, it will return NULL
	const aiScene* scene = importer.ReadFile(fileName.c_str(), aiProcess_Triangulate |
		aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	// Import failed
	if (!scene)
	{
		std::cerr << "Unable to load mesh/file: " << fileName << std::endl;
		return models;
	}

	// Loop over all models in the file
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		const aiMesh* model = scene->mMeshes[i];

		IndexedModel newModel;
		newModel.AllocateElement(3); // Positions
		newModel.AllocateElement(2); // Texture Coordinates
		newModel.AllocateElement(3); // Normals
		newModel.AllocateElement(3); // Tangents
		newModel.SetInstancedElementStartIndex(4); // Begin instanced data
		newModel.AllocateElement(16); // Transform matrix

		const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);

		// Loop over all vertices in the model
		for (unsigned int j = 0; j < model->mNumVertices; j++)
		{
			const aiVector3D position = model->mVertices[j];
			const aiVector3D normal = model->mNormals[j];

			// If the model does not have texture coordinates set them to zero
			const aiVector3D textureCoordinate = (model->HasTextureCoords(0) ? 
				model->mTextureCoords[0][j] : aiZeroVector);

			const aiVector3D tangent = model->mTangents[j];

			newModel.AddElement3f(0, position.x, position.y, position.z);
			newModel.AddElement2f(1, textureCoordinate.x, textureCoordinate.y);
			newModel.AddElement3f(2, normal.x, normal.y, normal.z);
			newModel.AddElement3f(3, tangent.x, tangent.y, tangent.z);
		}

		// Loop over all faces in the model
		for (unsigned int j = 0; j < model->mNumFaces; j++)
		{
			const aiFace& face = model->mFaces[j];
			// The model should be triangulated as we passed the aiProcess_Triangulate flag
			assert(face.mNumIndices == 3);
			newModel.AddIndices3i(face.mIndices[0], face.mIndices[1], face.mIndices[2]);
		}

		models.push_back(newModel);
	}

	return models;
}
/*
Mesh::Mesh(const std::string& fileName)
{
	// LoadModels will return an empty vector if
	// a) the file contains no meshes
	// b) the file failed to load
	IndexedModel model = LoadModels(fileName)[0];
	InitMesh(model);

	this->model = model;
}

Mesh::Mesh(Vertex* vertices, unsigned int numberOfVertices, unsigned int* indices,
	unsigned int numberOfIndices)
{
	IndexedModel model;

	// Convert the arrays into an IndexedModel...

	for (unsigned i = 0; i < numberOfVertices; i++)
	{
		model.positions.push_back(*vertices[i].GetPosition());
		model.textureCoordinates.push_back(*vertices[i].GetTextureCoordinate());
		model.normals.push_back(*vertices[i].GetNormal());
	}

	for (unsigned int i = 0; i < numberOfIndices; i++)
		model.indices.push_back(indices[i]);

	InitMesh(model);

	this->model = model;
}

void Mesh::UpdateVertexArrayBuffer(unsigned int bufferIndex, const void* data, uintptr_t dataSize,
	unsigned int usage)
{
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[bufferIndex]);
	glBufferData(GL_ARRAY_BUFFER, dataSize, data, usage);
}

void Mesh::InitMesh(const IndexedModel& model)
{
	drawCount = model.indices.size();

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);
	glGenBuffers(NUM_BUFFERS, vertexArrayBuffers);

	UpdateVertexArrayBuffer(POSITION_VB, model.positions.data(),
		model.positions.size() * sizeof(model.positions[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	UpdateVertexArrayBuffer(TEXTURECOORDINATE_VB, model.textureCoordinates.data(),
		model.textureCoordinates.size() * sizeof(model.textureCoordinates[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	UpdateVertexArrayBuffer(NORMAL_VB, model.normals.data(),
		model.normals.size() * sizeof(model.normals[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[INDEX_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(model.indices[0]), 
		&model.indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Mesh::Draw()
{
	glBindVertexArray(vertexArrayObject);
	glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vertexArrayObject);
}*/