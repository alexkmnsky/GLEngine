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
