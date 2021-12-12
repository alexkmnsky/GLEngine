#pragma once

#include "RenderDevice.h"
#include "UniformBuffer.h"
#include "Texture.h"
#include "Sampler.h"

#include <string>

class Shader
{
public:
	/**
	 * Loads a shader from a file.
	 *
	 * @param device: The target render device to create the shader for.
	 * @param fileName: File path to the shader. VERTEX_SHADER_BUILD will be defined when compiling
	 * the vertex shader. FRAGMENT_SHADER_BUILD will be defined when compiling the fragment shader.
	 */
	Shader(RenderDevice& device, const std::string& fileName);
	virtual ~Shader();

	inline void SetUniformBuffer(const std::string& name, UniformBuffer& buffer)
	{
		device->SetShaderUniformBuffer(deviceID, name, buffer.GetID());
	}

	inline void SetSampler(const std::string& name, Texture& texture, Sampler& sampler,
		unsigned int unit)
	{
		device->SetShaderSampler(deviceID, name, texture.GetID(), sampler.GetID(), unit);
	}

	inline unsigned int GetID() { return deviceID; }

private:
	// Disallow copy and assign
	Shader(const Shader& other) = delete;
	void operator=(const Shader& other) = delete;

	RenderDevice* device;
	unsigned int deviceID;
};

