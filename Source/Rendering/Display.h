#pragma once

#include <SDL2/sdl.h>
#include <string>

class Display
{
public:
	/**
	 * Creates a window and configures necessary settings for OpenGL.
	 *
	 * @param width Window width in pixels.
	 * @param height Window height in pixels.
	 * @param title Window title to display.
	 */
	Display(int width, int height, const std::string& title);

	/**
	 * Fills the entire display with the specified color.
	 *
	 * @param r Red
	 * @param g Green
	 * @param b Blue
	 * @param a Alpha
	 */
	void Clear(float r, float g, float b, float a);

	/** @brief Update the display. This currently only swaps buffers. */
	void Update();

	/**
	 * Resizes the display to a specified width and height.
	 *
	 * @param width Window width in pixels.
	 * @param height Window height in pixels.
	 */
	void Resize(int width, int height);

	inline int GetWidth() { return width; }
	inline int GetHeight() { return height; }
	inline SDL_Window* GetWindow() { return window; }

	virtual ~Display();

protected:

private:
	// Disallow copy and assign
	Display(const Display& other) = delete;
	Display& operator=(const Display& other) = delete;

	int width;
	int height;

	SDL_Window* window;
	SDL_GLContext glContext;
};

