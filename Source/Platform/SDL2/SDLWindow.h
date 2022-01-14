#pragma once

#include "Application.h"

#include <SDL2/SDL.h>
#include <string>

typedef SDL_Window* WindowHandle;

class SDLWindow
{
public:
	SDLWindow(const Application& application, unsigned int width, unsigned int height,
		const std::string title);

	virtual ~SDLWindow();

	inline WindowHandle GetWindowHandle() { return window; }

	/**
	 * Sets the width and height for future retrieval. 
	 * Note: this function does not change the window size. It only updates the width and height. 
	 * 
	 * TODO: Add support for setting the window size.
	 * 
	 * @param width: New window width, which should reflect the current state.
	 * @param height: New window height, which should reflect the current state.
	 */
	void ChangeSize(unsigned int width, unsigned int height);
	void Present();

	inline unsigned int GetWidth() { return width; }
	inline unsigned int GetHeight() { return height; }

private:
	// Disallow copy and assign
	SDLWindow(const SDLWindow& other) = delete;
	void operator=(const SDLWindow& other) = delete;

	WindowHandle window;
	unsigned int width;
	unsigned int height;
};
