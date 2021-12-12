#include "GameRenderContext.h"

void GameRenderContext::Flush()
{
	Texture* currentTexture = nullptr;
	for (auto it = meshRenderBuffer.begin(); it != meshRenderBuffer.end(); ++it)
	{
		VertexArray* vertexArray = it->first.first;
		Texture* texture = it->first.second;
		glm::mat4* transforms = it->second.data();
		size_t numTransforms = it->second.size();

		if (numTransforms == 0) // No instances to draw
		{
			continue;
		}

		if (texture != currentTexture)
		{
			shader.SetSampler("diffuse", *texture, sampler, 0);
		}

		// Index 4 is the list of instanced transform matrices
		vertexArray->UpdateBuffer(4, transforms, numTransforms * sizeof(glm::mat4));
		Draw(shader, *vertexArray, drawParameters, numTransforms);
		it->second.clear();
	}
}
