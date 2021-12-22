#include "TextRenderer.h"

#include <GLM/gtc/type_ptr.hpp>

TextRenderer::TextRenderer(int width, int height, RenderDevice& device, RenderTarget& target,
	Shader& shader, Sampler& sampler) : device(&device), target(&target), shader(shader),
	sampler(sampler)
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
	drawParameters.shouldWriteDepth = false;

	const static unsigned int elementSizes[] = { 2, 2 }; // 2 for position, 2 for texture coordinates
	const static unsigned int indices[] = { 0, 1, 2,
											0, 2, 3 };

	vao = this->device->CreateVertexArray(nullptr, elementSizes, 2, 0, 4, indices, 6,
		RenderDevice::USAGE_DYNAMIC_DRAW);
}

void TextRenderer::RenderText(Font& font, const std::string& text, float x, float y, float scale,
	const glm::vec3& color, bool isCentered)
{
	device->SetShaderMat4(shader.GetID(), "transform", glm::value_ptr(projection));
	device->SetShaderFloat3(shader.GetID(), "textColor", glm::value_ptr(color));

	// Find the width of the text for centering
	float textWidth = 0.0f, textAdvance = 0.0f;
	// Loop over all characters in the text to get the sizes
	for (char c : text)
	{
		Font::Character character = font.GetCharacter(c);
		float xPosition = textAdvance + character.bearing.x * scale;
		float width = character.size.x * scale;

		if (xPosition + width > textWidth)
		{
			textWidth = xPosition + width;
		}

		// Advance x position for next glyph
		// Note that advance is the number of 1/64 pixels
		textAdvance += (character.advance >> 6) * scale; // Bitshift by 6 (2^6 = 64)
	}

	// Loop over all characters in the text to render
	for (char c : text)
	{
		Font::Character character = font.GetCharacter(c);

		float xPosition = x + character.bearing.x * scale;
		float yPosition = y - (character.size.y - character.bearing.y) * scale;

		float width = character.size.x * scale;
		float height = character.size.y * scale;

		if (isCentered)
			xPosition -= textWidth / 2;

		const float positions[4][2] = {
			{ xPosition,         yPosition + height},
			{ xPosition,         yPosition,        },
			{ xPosition + width, yPosition,        },								   
			{ xPosition + width, yPosition + height}
		};

		const float textureCoordinates[4][2] = {
			{0.0f, 0.0f},
			{0.0f, 1.0f},
			{1.0f, 1.0f},
			{1.0f, 0.0f}
		};

		device->UpdateVertexArrayBuffer(vao, 0, positions, sizeof(float) * 4 * 2);
		device->UpdateVertexArrayBuffer(vao, 1, textureCoordinates, sizeof(float) * 4 * 2);

		device->SetShaderSampler(shader.GetID(), "text", character.textureID, sampler.GetID(), 0);
		device->Draw(target->GetID(), shader.GetID(), vao, drawParameters, 1, 6);

		// Advance x position for next glyph
		// Note that advance is the number of 1/64 pixels
		x += (character.advance >> 6) * scale; // Bitshift by 6 (2^6 = 64)
	}
}

TextRenderer::~TextRenderer()
{
	FT_Done_FreeType(ft);
	device->ReleaseVertexArray(vao);
}