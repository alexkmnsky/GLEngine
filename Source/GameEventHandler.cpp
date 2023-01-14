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

#include "GameEventHandler.h"

#include <iostream>

void GameEventHandler::Update()
{
	for (std::pair<float, MotionControl*>& motionControl : motionControls)
	{
		// Reset mouse motion every update to prevent accumulation
		motionControl.second->Set(0, 0); 
	}
}

void GameEventHandler::OnKeyDown(unsigned int keyCode, bool isRepeat)
{
	if (isRepeat) return;

	for (std::pair<float, AxisControl*>& axisControl : keyControls[keyCode].axisControls)
	{
		// Add the weight
		axisControl.second->Add(axisControl.first);
	}
	
	for (ActionControl* actionControl : keyControls[keyCode].actionControls)
	{
		actionControl->Push(ActionControl::PRESS);
	}

	for (BinaryControl* binaryControl : keyControls[keyCode].binaryControls)
	{
		binaryControl->Set(true);
	}
}

void GameEventHandler::OnKeyUp(unsigned int keyCode, bool isRepeat)
{
	if (isRepeat) return;

	for (std::pair<float, AxisControl*>& axisControl : keyControls[keyCode].axisControls)
	{
		// Subtract the weight (this undoes the addition performed by OnKeyDown)
		axisControl.second->Add(-axisControl.first);
	}
	
	for (ActionControl* actionControl : keyControls[keyCode].actionControls)
	{
		actionControl->Push(ActionControl::RELEASE);
	}

	for (BinaryControl* binaryControl : keyControls[keyCode].binaryControls)
	{
		// TODO: If there are multiple binary control bindings which are pressed at the same time,
		// when one is released, the binary control will be set to false even though the other
		// binding is still pressed. Low priority fix, but could be solved by keeping a count of
		// how many are pressed at a time.
		binaryControl->Set(false);
	}
}

void GameEventHandler::OnMouseDown(unsigned int mouseButton, unsigned char numberOfClicks)
{
	for (std::pair<float, AxisControl*>& axisControl : buttonControls[mouseButton].axisControls)
	{
		// Add the weight
		axisControl.second->Add(axisControl.first);
	}

	for (ActionControl* actionControl : buttonControls[mouseButton].actionControls)
	{
		actionControl->Push(ActionControl::PRESS);
	}

	for (BinaryControl* binaryControl : buttonControls[mouseButton].binaryControls)
	{
		binaryControl->Set(true);
	}
}

void GameEventHandler::OnMouseUp(unsigned int mouseButton, unsigned char numberOfClicks)
{
	for (std::pair<float, AxisControl*>& axisControl : buttonControls[mouseButton].axisControls)
	{
		// Subtract the weight (this undoes the addition performed by OnMouseDown)
		axisControl.second->Add(-axisControl.first);
	}

	for (ActionControl* actionControl : buttonControls[mouseButton].actionControls)
	{
		actionControl->Push(ActionControl::RELEASE);
	}

	for (BinaryControl* binaryControl : buttonControls[mouseButton].binaryControls)
	{
		// TODO: If there are multiple binary control bindings which are pressed at the same time,
		// when one is released, the binary control will be set to false even though the other
		// binding is still pressed. Low priority fix, but could be solved by keeping a count of
		// how many are pressed at a time.
		binaryControl->Set(false);
	}
}

void GameEventHandler::OnMouseMove(unsigned int mousePositionX, unsigned int mousePositionY, 
	int deltaX, int deltaY)
{
	for (std::pair<float, MotionControl*>& motionControl : motionControls)
	{
		// Multiply mouse movement delta by weight (a.k.a. sensitivity)
		motionControl.second->Add(deltaX * motionControl.first, deltaY * motionControl.first);
	}
}

void GameEventHandler::OnWindowResize(unsigned int windowWidth, unsigned int windowHeight)
{
	for (std::function<void(unsigned int, unsigned int)> callback : windowResizedCallbacks)
	{
		callback(windowWidth, windowHeight);
	}
}

void GameEventHandler::AddKeyAxisControl(unsigned int keyCode, AxisControl& control, float weight)
{
	keyControls[keyCode].axisControls.push_back(std::make_pair(weight, &control));
}

void GameEventHandler::AddKeyActionControl(unsigned int keyCode, ActionControl& control)
{
	keyControls[keyCode].actionControls.push_back(&control);
}

void GameEventHandler::AddKeyBinaryControl(unsigned int keyCode, BinaryControl& control)
{
	keyControls[keyCode].binaryControls.push_back(&control);
}

void GameEventHandler::AddButtonAxisControl(unsigned int mouseButton, AxisControl& control, 
	float weight)
{
	buttonControls[mouseButton].axisControls.push_back(std::make_pair(weight, &control));
}

void GameEventHandler::AddButtonActionControl(unsigned int mouseButton, ActionControl& control)
{
	buttonControls[mouseButton].actionControls.push_back(&control);
}

void GameEventHandler::AddButtonBinaryControl(unsigned int mouseButton, BinaryControl& control)
{
	buttonControls[mouseButton].binaryControls.push_back(&control);
}

void GameEventHandler::AddMouseMotionControl(MotionControl& control, float weight)
{
	motionControls.push_back(std::make_pair(weight, &control));
}

void GameEventHandler::AddWindowResizeCallback(std::function<void(unsigned int, unsigned int)> callback)
{
	windowResizedCallbacks.push_back(callback);
}
