#pragma once

#include <ft2build.h>
#include <freetype/freetype.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <string>
#include <iostream>

#include "Font.h"
#include "Shader.h"
#include "IndexedModel.h"
#include "VertexArray.h"
#include "UniformBuffer.h"
#include "RenderDevice.h"
#include "RenderTarget.h"
#include "Sampler.h"

class TextRenderer
{
public:
	/**
	 * Initializes the text renderer and all used libraries.
	 * 
	 * @param width The window width in pixels.
	 * @param height The window height in pixels.
	 * @param shader Reference to the text shader which defines how characters are rendered.
	 */
	TextRenderer(int width, int height, RenderDevice& device, RenderTarget& target, Shader& shader, Sampler& sampler);

	/**
	 * Loads a font file and prepares the first 128 characters for rendering. Currently, only
	 * ASCII characters are loaded, for simplicity.
	 * 
	 * @param fileName Font file to load. Can be of of any format which FreeType supports.
	 * @param pixelSize Font size in pixels to generate.
	 * @return A Font object which contans the needed data for rendering.
	 */
	inline Font LoadFont(const std::string& fileName, unsigned int pixelSize)
	{
		FT_Face face;
		if (FT_New_Face(ft, fileName.c_str(), 0, &face))
		{
			std::cerr << "Font loading failed for font: " << fileName << std::endl;
		}
		FT_Set_Pixel_Sizes(face, 0, pixelSize);
		return Font(*device, face);
	}

	/**
	 * Renders text to the screen. This function will use the text shader specified in the
	 * constructor of the text renderer.
	 * 
	 * @param font The font to use.
	 * @param text The string to render.
	 * @param x X position relative to the width specified in the constructor of the text renderer.
	 * @param y Y position relative to the height specified in the constructor of the text renderer.
	 * @param scale The scale of the text. Set to 1.0f for no effect.
	 * @param color The color of the text, in order of red, green, blue.
	 * @param isCentered Should the text be aligned to the center or to the top left. Defaults to
	 *		top left.
	 */
	void RenderText(Font& font, const std::string& text, float x, float y, float scale, 
		const glm::vec3& color, bool isCentered = false);

	// Cleans up FreeType library
	~TextRenderer();

private:
	// Disallow copy and assign
	TextRenderer(const TextRenderer& other) = delete;
	void operator=(const TextRenderer& other) = delete;

	FT_Library ft;

	RenderDevice::DrawParameters drawParameters;
	RenderDevice* device;
	RenderTarget* target;
	Shader& shader;
	Sampler& sampler;
	glm::mat4 projection;
};

