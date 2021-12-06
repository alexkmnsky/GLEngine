#pragma once

#include "IApplicationEventHandler.h"
#include "InputControl.h"
#include "MotionControl.h"

#include <unordered_map>
#include <vector>
#include <utility> // std::pair
#include <tuple>

class GameEventHandler : public IApplicationEventHandler
{
public:
	GameEventHandler() {}
	virtual ~GameEventHandler() {}

	virtual void Update();

	virtual void OnKeyDown(unsigned int keyCode, bool isRepeat);
	virtual void OnKeyUp(unsigned int keyCode, bool isRepeat);
	virtual void OnMouseDown(unsigned int mouseButton, unsigned char numberOfClicks);
	virtual void OnMouseUp(unsigned int mouseButton, unsigned char numberOfClicks);
	virtual void OnMouseMove(unsigned int mousePositionX, unsigned int mousePositionY,
		int deltaX, int deltaY);

	/**
	 * Adds a key to listen to.
	 * 
	 * @param keyCode The key to listen to. 
	 * @param inputControl The input control which the key should affect on press/release.
	 * @param weight How to influence the input control.
	 */
	void AddKeyControl(unsigned int keyCode, InputControl& inputControl, float weight = 1.0f);

	/**
	 * Adds a mouse button to listen to.
	 *
	 * @param mouseButton The mouse button to listen to.
	 * @param inputControl The input control which the button should affect on press/release.
	 * @param weight How to influence the input control.
	 */
	void AddMouseControl(unsigned int mouseButton, InputControl& inputControl, float weight = 1.0f);

	/**
	 * Adds a mouse motion axis to listen to.
	 * 
	 * @param motionControl The motion control which mouse movement should affect.
	 * @param axis The axis of mouse motion to listen to.
	 * @param weight How strongly to influence the motion (sensitivity).
	 */
	void AddMotionControl(MotionControl& motionControl, MotionControl::Axis axis, float weight = 1.0f);

private:
	// Disallow copy and assign
	GameEventHandler(const GameEventHandler& other) = delete;
	void operator=(const GameEventHandler& other) = delete;

	std::unordered_map<unsigned int, std::vector<std::pair<float, InputControl&>>> keys;
	std::unordered_map<unsigned int, std::vector<std::pair<float, InputControl&>>> buttons;
	std::vector<std::tuple<float, MotionControl::Axis, MotionControl&>> motion;

	void UpdateKeyInput(unsigned int inputCode, float direction, bool isRepeat);
	void UpdateButtonInput(unsigned int inputCode, float direction);
	void UpdateMotionInput(int deltaX, int deltaY);
};