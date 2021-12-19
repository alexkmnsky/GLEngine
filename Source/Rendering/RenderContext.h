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
