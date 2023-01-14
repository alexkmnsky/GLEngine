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
