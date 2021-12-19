#pragma once

#include "Rendering/RenderContext.h"
#include "Rendering/Camera.h"

#include <map>
#include <utility>

class GameRenderContext : public RenderContext
{
public:
	GameRenderContext(RenderDevice& device, RenderTarget& target,
		RenderDevice::DrawParameters& drawParameters, Shader& shader, Sampler& sampler,
		Camera& camera) : RenderContext(device, target, drawParameters), shader(shader), 
		sampler(sampler), camera(camera) {}

	inline void RenderMesh(VertexArray& vertexArray, Texture& texture, const glm::mat4& transform)
	{
		meshRenderBuffer[std::make_pair(&vertexArray, &texture)].push_back(
			camera.GetViewProjection() * transform);
	}

	void Flush();

private:
	Shader& shader;
	Sampler& sampler;
	Camera& camera;
	std::map<std::pair<VertexArray*, Texture*>, std::vector<glm::mat4>> meshRenderBuffer;
};
