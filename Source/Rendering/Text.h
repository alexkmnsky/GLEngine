/** Copyright (c) 2022-2023 Alexander Kaminsky and Maxwell Hunt */

#pragma once

#include "Font.h"
#include "VertexArray.h"
#include "Transform.h"

#include <GLM/glm.hpp>
#include <string>

class TextRenderer;

class Text
{
public:
	enum class Anchor
	{
		CENTERED,
		TOP_LEFT,
		TOP,
		TOP_RIGHT,
		RIGHT,
		BOTTOM_RIGHT,
		BOTTOM,
		BOTTOM_LEFT,
		LEFT
	};

	struct Layer
	{
		glm::vec4 color;
		float smoothing;
		float offset;
		Transform transform;
	};

	Text(RenderDevice& device, TextRenderer& textRenderer, Font* font, const std::string& text,
		Anchor anchor, const std::vector<Layer>& style, const Transform& transform);

	virtual ~Text();

	inline void SetText(const std::string& text)
	{
		this->text = text;
		GenerateVertexArray();
	}

	inline std::string& GetText() { return text; }

	inline void SetAnchor(Anchor anchor)
	{
		this->anchor = anchor;
		UpdateStyleAndTransformation();
	}

	inline Anchor GetAnchor() { return anchor; }

	inline void SetStyle(const std::vector<Layer>& style)
	{
		this->style = style;
		UpdateStyleAndTransformation();
	}

	inline std::vector<Layer>& GetStyle() { return style; }

	inline void SetTransform(const Transform& transform)
	{
		this->transform = transform;
		UpdateStyleAndTransformation();
	}

	inline const Transform& GetTransform() { return transform; }
	
	inline void SetFont(Font* font)
	{
		this->font = font;
		GenerateVertexArray();
	}

	inline Font* GetFont() { return font; }

	inline VertexArray* GetVertexArray() { return vertexArray; }

	inline unsigned int GetNumLayers() { return style.size(); }

private:
	// Disallow copy and assign
	Text(const Text& other) = delete;
	void operator=(const Text& other) = delete;

	void GenerateVertexArray();
	void UpdateStyleAndTransformation();

	RenderDevice* device;
	TextRenderer* textRenderer;
	std::string text;
	Anchor anchor;
	std::vector<Layer> style;
	Font* font;
	Transform transform;

	float textWidth;
	float textHeight;

	VertexArray* vertexArray;
};
