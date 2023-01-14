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

#include "Events/IApplicationEventHandler.h"
#include "Events/ActionControl.h"
#include "Events/AxisControl.h"
#include "Events/BinaryControl.h"
#include "Events/MotionControl.h"

#include <unordered_map>
#include <set>
#include <vector>
#include <utility> // std::pair
#include <tuple>
#include <functional>

class GameEventHandler : public IApplicationEventHandler
{
public:
	GameEventHandler() {}
	virtual ~GameEventHandler() {}

	void Update() override;
	void OnKeyDown(unsigned int keyCode, bool isRepeat) override;
	void OnKeyUp(unsigned int keyCode, bool isRepeat) override;
	void OnMouseDown(unsigned int mouseButton, unsigned char numberOfClicks) override;
	void OnMouseUp(unsigned int mouseButton, unsigned char numberOfClicks) override;
	void OnMouseMove(unsigned int mousePositionX, unsigned int mousePositionY, int deltaX, 
		int deltaY) override;
	void OnWindowResize(unsigned int windowWidth, unsigned int windowHeight) override;

	void AddKeyAxisControl(unsigned int keyCode, AxisControl& control, float weight = 1.0f);
	void AddKeyActionControl(unsigned int keyCode, ActionControl& control);
	void AddKeyBinaryControl(unsigned int keyCode, BinaryControl& control);
	void AddButtonAxisControl(unsigned int mouseButton, AxisControl& control, float weight = 1.0f);
	void AddButtonActionControl(unsigned int mouseButton, ActionControl& control);
	void AddButtonBinaryControl(unsigned int mouseButton, BinaryControl& control);
	void AddMouseMotionControl(MotionControl& control, float weight = 1.0f);

	void AddWindowResizeCallback(std::function<void(unsigned int, unsigned int)> callback);

private:
	// Disallow copy and assign
	GameEventHandler(const GameEventHandler& other) = delete;
	void operator=(const GameEventHandler& other) = delete;

	struct Controls
	{
		std::vector<std::pair<float, AxisControl*>> axisControls;
		std::vector<ActionControl*> actionControls;
		std::vector<BinaryControl*> binaryControls;
	};

	std::unordered_map<unsigned int, Controls> keyControls;
	std::unordered_map<unsigned int, Controls> buttonControls;
	std::vector<std::pair<float, MotionControl*>> motionControls;

	std::vector<std::function<void(unsigned int, unsigned int)>> windowResizedCallbacks;
};