#pragma once

#include "Texture.h"
#include "RenderDevice.h"

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
	Font(FT_Library& ft, RenderDevice& device, const std::string& fileName,	unsigned int pixelSize);

	virtual ~Font();

	/** @brief Stores all metrics used for a glyph. */
	struct Character
	{
		glm::ivec2 origin;		// Origin of glyph on the texture atlas
		glm::ivec2 size;		// Size of glyph0 on the texture atlas
		glm::ivec2 bearing;		// Offset from baseline to left/top of glyph
		unsigned int advance;	// Offset to advance to next glyph
	};

	/**
	 * Gets the needed data for rendering a specified character.
	 * 
	 * @param c The character to look up.
	 * @return Character struct.
	 */
	inline const Character& GetCharacter(char c) { return characters[c]; }

	inline unsigned int GetTextureID() { return textureID; }
	inline glm::ivec2 GetTextureSize() { return textureSize; }

private:
	// Disallow copy and assign
	Font(const Font& other) = delete;
	void operator=(const Font& other) = delete;

	RenderDevice* device;

	unsigned int textureID;
	glm::ivec2 textureSize;

	std::map<char, Character> characters;
};

