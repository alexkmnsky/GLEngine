#include <SDL2/SDL.h>
#include <iostream>

#include "SDLApplication.h"

unsigned int SDLApplication::numInstances = 0;

SDLApplication* SDLApplication::Create()
{
	const unsigned int flags = SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS;
	const unsigned int initialized = SDL_WasInit(flags);
	if (initialized != flags && SDL_Init(flags) != 0)
	{
		std::cerr << "Error: SDL_Init: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	return new SDLApplication();
}

SDLApplication::SDLApplication()
{
	numInstances++;
	isRunning = true;
}

SDLApplication::~SDLApplication()
{
	numInstances--;
	if (numInstances == 0)
	{
		SDL_Quit();	
	}
}

void SDLApplication::ProcessMessages(float deltaTime, IApplicationEventHandler& eventHandler)
{
	eventHandler.Update();

	SDL_Event e;
	
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_KEYDOWN:
			eventHandler.OnKeyDown(e.key.keysym.scancode, e.key.repeat != 0);
			break;
		case SDL_KEYUP:
			eventHandler.OnKeyUp(e.key.keysym.scancode, e.key.repeat != 0);
			break;
		case SDL_MOUSEBUTTONDOWN:
			eventHandler.OnMouseDown(e.button.button, e.button.clicks);
			break;
		case SDL_MOUSEBUTTONUP:
			eventHandler.OnMouseUp(e.button.button, e.button.clicks);
			break;
		case SDL_MOUSEMOTION:
			eventHandler.OnMouseMove(e.motion.x, e.motion.y, e.motion.xrel, e.motion.yrel);
			break;
		case SDL_WINDOWEVENT_RESIZED:
			eventHandler.OnWindowResized(e.window.data1, e.window.data2);
			break;
		case SDL_QUIT:
			isRunning = false;
			break;
		default:
			break;
		}
	}
}
