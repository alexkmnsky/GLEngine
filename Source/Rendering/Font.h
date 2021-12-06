#pragma once

#include <ft2build.h>
#include <freetype/freetype.h>

#include <GLM/glm.hpp>
#include <GL/glew.h>
#include <map>

class Font
{
public:
	/**
	 * Prepares the first 128 characters for rendering. Currently, only ASCII characters are
	 * loaded, for simplicity.
	 * 
	 * @param face Fontface to use.
	 */
	Font(FT_Face face);

	/** @brief Stores all metrics used for a glyph. */
	struct Character
	{
		unsigned int textureID; // ID handle of the glyph texture
		glm::ivec2 size;        // Size of glyph
		glm::ivec2 bearing;     // Offset from baseline to left/top of glyph
		unsigned int advance;   // Offset to advance to next glyph
	};

	/**
	 * Gets the needed data for rendering a specified character.
	 * 
	 * @param c The character to look up.
	 * @return Character struct.
	 */
	inline Character GetCharacter(char c) { return characters[c]; }

private:
	std::map<char, Character> characters;
};

