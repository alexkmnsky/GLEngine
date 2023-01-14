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

/** @brief Interface for application events we want to listen to. */
class IApplicationEventHandler
{
public:
	IApplicationEventHandler() {}
	virtual ~IApplicationEventHandler() {}

	virtual void Update() {}
	virtual void OnKeyDown(unsigned int keyCode, bool isRepeat) {}
	virtual void OnKeyUp(unsigned int keyCode, bool isRepeat) {}
	virtual void OnMouseDown(unsigned int mouseButton, unsigned char numberOfClicks) {}
	virtual void OnMouseUp(unsigned int mouseButton, unsigned char numberOfClicks) {}
	virtual void OnMouseMove(unsigned int mousePositionX, unsigned int mousePositionY,
		int deltaX, int deltaY) {}
	virtual void OnWindowResize(unsigned int windowWidth, unsigned int windowHeight) {}

private:
	// Disallow copy and assign
	IApplicationEventHandler(const IApplicationEventHandler& other) = delete;
	void operator=(const IApplicationEventHandler& other) = delete;
};
