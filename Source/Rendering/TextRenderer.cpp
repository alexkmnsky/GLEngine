#include "TextRenderer.h"

#include <GLM/gtc/type_ptr.hpp>

TextRenderer::TextRenderer(unsigned int width, unsigned int height, RenderDevice& device, 
	RenderTarget& target, Shader& shader, Sampler& sampler) : device(&device), target(&target), 
	shader(shader),	sampler(sampler)
{
	if (FT_Init_FreeType(&ft))
	{
		std::cerr << "FreeType failed to initialize!" << std::endl;
	}

	projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height);

	drawParameters.primitiveType = RenderDevice::PRIMITIVE_TRIANGLES;
	drawParameters.sourceBlend = RenderDevice::BLEND_FUNC_SRC_ALPHA;
	drawParameters.destBlend = RenderDevice::BLEND_FUNC_ONE_MINUS_SRC_ALPHA;
	drawParameters.depthFunc = RenderDevice::DRAW_FUNC_ALWAYS;
	drawParameters.shouldWriteDepth = true;
}

void TextRenderer::RenderText(Text& text)
{
	device->SetShaderSampler(shader.GetID(), "texture0", text.GetFont()->GetTextureID(), 
		sampler.GetID(), 0);

	device->Draw(target->GetID(), shader.GetID(), text.GetVertexArray()->GetID(), drawParameters,
		text.GetNumLayers(), text.GetVertexArray()->GetNumIndices());
}

TextRenderer::~TextRenderer()
{
	FT_Done_FreeType(ft);
}