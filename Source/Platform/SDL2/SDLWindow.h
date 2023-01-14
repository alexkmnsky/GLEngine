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
