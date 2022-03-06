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
