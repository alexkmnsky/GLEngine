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
#include "Shader.h"
#include "VertexArray.h"
#include "RenderTarget.h"

class RenderContext
{
public:
	RenderContext(RenderDevice& device, RenderTarget& target, 
		RenderDevice::DrawParameters& drawParameters) : device(&device), target(&target),
		drawParameters(drawParameters) {}

	inline void Clear(bool shouldClearColor, bool shouldClearDepth, bool shouldClearStencil,
		float r, float g, float b, float a, unsigned int stencil)
	{
		device->SetDrawParameters(drawParameters);
		device->Clear(target->GetID(), shouldClearColor, shouldClearDepth, shouldClearStencil,
			r, g, b, a, stencil);
	}

	inline void Clear(float r, float g, float b, float a, bool shouldClearDepth = false)
	{
		device->SetDrawParameters(drawParameters);
		device->Clear(target->GetID(), true, shouldClearDepth, false, r, g, b, a, 0);
	}

	inline void Draw(Shader& shader, VertexArray& vertexArray,
		const RenderDevice::DrawParameters& drawParameters, unsigned int numInstances = 1)
	{
		device->Draw(target->GetID(), shader.GetID(), vertexArray.GetID(), drawParameters, 
			numInstances, vertexArray.GetNumIndices());
	}

	inline void Draw(Shader& shader, VertexArray& vertexArray,
		const RenderDevice::DrawParameters& drawParameters, unsigned int numInstances,
		unsigned int numIndices)
	{
		device->Draw(target->GetID(), shader.GetID(), vertexArray.GetID(), drawParameters,
			numInstances, vertexArray.GetNumIndices());
	}

protected:
	RenderDevice::DrawParameters& drawParameters;

private:
	RenderDevice* device;
	RenderTarget* target;
};
