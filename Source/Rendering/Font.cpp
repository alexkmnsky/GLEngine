#include "Font.h"
#include <iostream>

#include <ft2build.h>
#include <freetype/freetype.h>

Font::Font(FT_Face face)
{
	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (unsigned char c = 0; c < 128; c++)
	{
		// Load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cerr << "Failed to load font glyph '" << c << "'" << std::endl;
			continue;
		}

		// Generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
			face->glyph->bitmap.width, face->glyph->bitmap.rows, // Width and height of texture
			0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		// Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Store character for later use
		Character character = { texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(unsigned int)face->glyph->advance.x };

		characters.insert(std::make_pair(c, character));
	}

	// Re-enable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 0);

	// Cleanup
	FT_Done_Face(face);
}
