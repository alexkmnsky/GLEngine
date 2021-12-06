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

	void ChangeSize(unsigned int width, unsigned int height);
	void Present();

	inline unsigned int GetWidth() { return width; }
	inline unsigned int GetHeight() { return height; }

private:
	WindowHandle window;
	unsigned int width;
	unsigned int height;
};
