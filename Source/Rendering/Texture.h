#pragma once

#include <string>
#include <GL/glew.h>

class Texture
{
public:
	/**
	 * Loads a texture from a file.
	 *
	 * @param fileName File path to the texture.
	 */
	Texture(const std::string& fileName);

	/**
	 * Binds the texture on the GPU to a specified unit/slot.
	 * 
	 * @param unit Texture slot to bind the texture to. OpenGL allows the unit to be from 0 to 31.
	 */
	void Bind(unsigned int unit);

	virtual ~Texture();

protected:
private:
	// Disallow copy and assign
	Texture(const Texture& other) = delete;
	void operator=(const Texture& other) = delete;

	GLuint texture;

};

