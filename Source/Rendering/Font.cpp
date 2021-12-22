#include "Font.h"
#include <iostream>

#include <ft2build.h>
#include <freetype/freetype.h>

Font::Font(RenderDevice& device, FT_Face face) : device(&device)
{
	for (unsigned char c = 0; c < 128; c++)
	{
		// Load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cerr << "Failed to load font glyph '" << c << "'" << std::endl;
			continue;
		}

		unsigned int texture = this->device->CreateTexture2D(face->glyph->bitmap.width,
			face->glyph->bitmap.rows, face->glyph->bitmap.buffer, RenderDevice::FORMAT_R,
			RenderDevice::FORMAT_R, false, false, 0, 1);

		// Store character for later use
		Character character = { texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(unsigned int)face->glyph->advance.x };

		characters.insert(std::make_pair(c, character));
	}

	// Cleanup
	FT_Done_Face(face);
}

Font::~Font()
{
	// Delete all character textures
	for (const auto& character : characters)
	{
		this->device->ReleaseTexture2D(character.second.textureID);
	}
}
