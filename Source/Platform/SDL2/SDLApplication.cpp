/*  
 * Copyright (c) 2022-2023 Alexander Kaminsky and Maxwell Hunt
 *  
 * This file incorporates work covered by the following copyright and permission notice:  
 *  
 *     Copyright (c) 2018 Benny Bobaganoosh
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *     
 *     The above copyright notice and this permission notice shall be included in all
 *     copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *     SOFTWARE.
 */

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

void SDLApplication::LockMouse()
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
}

void SDLApplication::UnlockMouse()
{
	SDL_SetRelativeMouseMode(SDL_FALSE);
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
		case SDL_WINDOWEVENT:
			if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				eventHandler.OnWindowResize(e.window.data1, e.window.data2);
			break;
		case SDL_QUIT:
			isRunning = false;
			break;
		default:
			break;
		}
	}
}
