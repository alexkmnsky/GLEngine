#include "GameEventHandler.h"

#include <SDL2/SDL.h>

void GameEventHandler::Update()
{
	for (auto t : motion)
	{
		std::get<2>(t).ResetAmount();
	}
}

void GameEventHandler::OnKeyDown(unsigned int keyCode, bool isRepeat)
{
	UpdateKeyInput(keyCode, 1.0f, isRepeat);
}

void GameEventHandler::OnKeyUp(unsigned int keyCode, bool isRepeat)
{
	UpdateKeyInput(keyCode, -1.0f, isRepeat);
}

void GameEventHandler::OnMouseDown(unsigned int mouseButton, unsigned char numberOfClicks)
{
	UpdateButtonInput(mouseButton, 1.0f);
}

void GameEventHandler::OnMouseUp(unsigned int mouseButton, unsigned char numberOfClicks)
{
	UpdateButtonInput(mouseButton, -1.0f);
}

void GameEventHandler::OnMouseMove(unsigned int mousePositionX, unsigned int mousePositionY,
	int deltaX, int deltaY)
{
	UpdateMotionInput(deltaX, deltaY);
}

void GameEventHandler::AddKeyControl(unsigned int keyCode, InputControl& inputControl,
	float weight)
{
	keys[keyCode].push_back(std::pair<float, InputControl&>(weight, inputControl));
}

void GameEventHandler::AddMouseControl(unsigned int mouseButton, InputControl& inputControl,
	float weight)
{
	buttons[mouseButton].push_back(std::pair<float, InputControl&>(weight, inputControl));
}

void GameEventHandler::AddMotionControl(MotionControl& motionControl, MotionControl::Axis axis,
	float weight)
{
	motion.push_back(std::tuple<float, MotionControl::Axis, MotionControl&>(
		weight, axis, motionControl));
}

void GameEventHandler::UpdateKeyInput(unsigned int inputCode, float direction, bool isRepeat)
{
	if (isRepeat)
		return;
	
	// Loop over each pair of weight and InputControl
	for (auto p : keys[inputCode])
	{
		p.second.AddAmount(p.first * direction);
	}
}

void GameEventHandler::UpdateButtonInput(unsigned int inputCode, float direction)
{
	// Loop over each pair of weight and InputControl
	for (auto p : buttons[inputCode])
	{
		p.second.AddAmount(p.first * direction);
	}
}

void GameEventHandler::UpdateMotionInput(int deltaX, int deltaY)
{
	for (auto t : motion)
	{
		// Set motion for X axis
		if (std::get<1>(t) == MotionControl::Axis::X)
			std::get<2>(t).AddAmount((float)deltaX);

		// Set motion for Y axis
		else if (std::get<1>(t) == MotionControl::Axis::Y)
			std::get<2>(t).AddAmount((float)deltaY);
	}	
}
