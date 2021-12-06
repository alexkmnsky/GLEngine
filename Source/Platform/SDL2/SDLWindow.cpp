#include "SDLWindow.h"
#include "Rendering/RenderDevice.h"

#include <stdexcept>

SDLWindow::SDLWindow(const Application& application, unsigned int width, unsigned int height, 
	const std::string title) : width(width), height(height)
{
	if (!RenderDevice::GlobalInit())
	{
		throw std::runtime_error("Render device could not be initialized.");
	}

	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height, SDL_WINDOW_OPENGL);
}

SDLWindow::~SDLWindow()
{
	SDL_DestroyWindow(window);
}

void SDLWindow::ChangeSize(unsigned int width, unsigned int height)
{
	// TODO: Implement
}

void SDLWindow::Present()
{
	SDL_GL_SwapWindow(window);
}
