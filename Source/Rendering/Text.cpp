/** Copyright (c) 2022-2023 Alexander Kaminsky and Maxwell Hunt */

#include "Text.h"
#include "IndexedModel.h"
#include "TextRenderer.h"

Text::Text(RenderDevice& device, TextRenderer& textRenderer, Font* font, const std::string& text,
	Anchor anchor, const std::vector<Layer>& style, const Transform& transform) : device(&device), 
	textRenderer(&textRenderer), font(font), text(text), anchor(anchor), style(style),
	transform(transform)
{
	GenerateVertexArray();
}

Text::~Text()
{
	delete vertexArray;
}

void Text::GenerateVertexArray()
{
	IndexedModel textModel;
	textModel.AllocateElement(2); // Positions
	textModel.AllocateElement(2); // Texture Coordinates
	textModel.SetInstancedElementStartIndex(2); // Begin instanced data
	textModel.AllocateElement(4); // Color
	textModel.AllocateElement(1); // Smoothing
	textModel.AllocateElement(1); // Offset
	textModel.AllocateElement(16); // Transform Matrix

	int x = 0, y = 0;
	// Find the width of the text for centering
	textWidth = 0.0f, textHeight = 0.0f;
	// Loop over all characters in the text to render
	for (unsigned int i = 0; i < text.size(); i++)
	{
		Font::Character character = font->GetCharacter(text[i]);

		const float xPosition = x + character.bearing.x;
		const float yPosition = y + (character.size.y - character.bearing.y);

		const float width = character.size.x;
		const float height = character.size.y;

		// Add quad vertex positions
		textModel.AddElement2f(0, xPosition, yPosition + height);
		textModel.AddElement2f(0, xPosition, yPosition);
		textModel.AddElement2f(0, xPosition + width, yPosition);
		textModel.AddElement2f(0, xPosition + width, yPosition + height);

		textWidth = std::max(textWidth, xPosition + width);
		textHeight = std::max(textHeight, yPosition + height);

		const float minX = character.origin.x;
		const float minY = character.origin.y;
		const float maxX = character.origin.x + character.size.x;
		const float maxY = character.origin.y + character.size.y;

		// Add quad texture coordinates
		textModel.AddElement2f(1, minX / font->GetTextureSize().x, minY / font->GetTextureSize().y);
		textModel.AddElement2f(1, minX / font->GetTextureSize().x, maxY / font->GetTextureSize().y);
		textModel.AddElement2f(1, maxX / font->GetTextureSize().x, maxY / font->GetTextureSize().y);
		textModel.AddElement2f(1, maxX / font->GetTextureSize().x, minY / font->GetTextureSize().y);

		const unsigned int indexOffset = i * 4; // 4 indices per quad
		textModel.AddIndices3i(indexOffset + 0, indexOffset + 1, indexOffset + 2);
		textModel.AddIndices3i(indexOffset + 0, indexOffset + 2, indexOffset + 3);

		// Advance x position for next glyph
		// Note that advance is the number of 1/64 pixels
		x += (character.advance >> 6); // Bitshift by 6 (2^6 = 64)
	}

	VertexArray* newVertexArray = new VertexArray(*device, textModel, RenderDevice::USAGE_DYNAMIC_DRAW);
	delete vertexArray;
	vertexArray = newVertexArray;

	UpdateStyleAndTransformation();
}

void Text::UpdateStyleAndTransformation()
{
	unsigned int numInstances = style.size();

    std::vector<glm::vec4> colors;
	std::vector<float> smoothing;
	std::vector<float> offsets;
	std::vector<glm::mat4> transforms;

	// This aligns the origin to the center so that rotations and scaling occur about the center
	Transform centerTransform(-glm::vec3(textWidth, textHeight, 0.f) / 2.f);

	Transform anchorTransform;
	switch (anchor)
	{
	case Text::Anchor::TOP_LEFT:
		anchorTransform.SetPosition(glm::vec3(textWidth, -textHeight, 0.f) / 2.f);
		break;
	case Text::Anchor::TOP:
        anchorTransform.SetPosition(glm::vec3(0.f, -textHeight, 0.f) / 2.f);
		break;
	case Text::Anchor::TOP_RIGHT:
        anchorTransform.SetPosition(glm::vec3(-textWidth, -textHeight, 0.f) / 2.f);
		break;
	case Text::Anchor::RIGHT:
        anchorTransform.SetPosition(glm::vec3(-textWidth, 0.f, 0.f) / 2.f);
		break;
	case Text::Anchor::BOTTOM_RIGHT:
        anchorTransform.SetPosition(glm::vec3(-textWidth, textHeight, 0.f) / 2.f);
		break;
	case Text::Anchor::BOTTOM:
        anchorTransform.SetPosition(glm::vec3(0.f, textHeight, 0.f) / 2.f);
		break;
	case Text::Anchor::BOTTOM_LEFT:
        anchorTransform.SetPosition(glm::vec3(textWidth, textHeight, 0.f) / 2.f);
		break;
	case Text::Anchor::LEFT:
        anchorTransform.SetPosition(glm::vec3(textWidth, 0.f, 0.f) / 2.f);
		break;
	default:
		break;
	}

    for (const Layer& layer : style)
    {
        colors.push_back(layer.color);
        smoothing.push_back(layer.smoothing);
        offsets.push_back(layer.offset);

        glm::mat4 finalTransform = textRenderer->GetProjection();
        finalTransform *= anchorTransform.GetModel();
        finalTransform *= transform.GetModel();
        finalTransform *= centerTransform.GetModel();
        finalTransform *= layer.transform.GetModel();
        transforms.push_back(finalTransform);
    }

	vertexArray->UpdateBuffer(2, colors.data(),     colors.size()     * sizeof(glm::vec4));
	vertexArray->UpdateBuffer(3, smoothing.data(),  smoothing.size()  * sizeof(float));
	vertexArray->UpdateBuffer(4, offsets.data(),    offsets.size()    * sizeof(float));
	vertexArray->UpdateBuffer(5, transforms.data(), transforms.size() * sizeof(glm::mat4));
}
