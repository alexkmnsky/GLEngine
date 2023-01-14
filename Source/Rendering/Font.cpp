/** Copyright (c) 2022-2023 Alexander Kaminsky and Maxwell Hunt */

#include "Font.h"
#include <iostream>

#include <ft2build.h>
#include <freetype/freetype.h>

#include "TexturePacker.h"

Font::Font(FT_Library& ft, RenderDevice& device, const std::string& fileName, 
	unsigned int pixelSize) : device(&device)
{
	FT_Face face;
	if (FT_New_Face(ft, fileName.c_str(), 0, &face))
	{
		std::cerr << "Font loading failed for font: " << fileName << std::endl;
	}

	FT_Set_Pixel_Sizes(face, 0, pixelSize);

	TexturePacker texturePacker(glm::ivec2(256, 256));

	for (unsigned char c = 0x20; c < 0x7e; c++) // All printable ASCII characters
	{
		// Load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cerr << "Warning: Failed to load font glyph '" << c << "'" << std::endl;
			continue;
		}

		if (FT_Render_Glyph(face->glyph, FT_RENDER_MODE_SDF))
		{
			std::cerr << "Warning: Failed to render font glyph SDF '" << c << "'" << std::endl;
		}

		glm::ivec2 origin = texturePacker.PackTexture(face->glyph->bitmap.buffer,
			{ face->glyph->bitmap.width, face->glyph->bitmap.rows });

		// Store character for later use
		Character character = { origin,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(unsigned int)face->glyph->advance.x };

		characters.insert(std::make_pair(c, character));

	}

	textureID = this->device->CreateTexture2D(texturePacker.GetTextureSize().x, 
		texturePacker.GetTextureSize().y, texturePacker.GetBuffer(), RenderDevice::FORMAT_R,
		RenderDevice::FORMAT_R, false, false, 0, 1);

	textureSize = texturePacker.GetTextureSize();

	// Cleanup
	FT_Done_Face(face);
}

Font::~Font()
{
	this->device->ReleaseTexture2D(textureID);
}
