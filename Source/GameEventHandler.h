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
	void OnWindowResized(unsigned int windowWidth, unsigned int windowHeight) override;

	void AddKeyAxisControl(unsigned int keyCode, AxisControl& control, float weight = 1.0f);
	void AddKeyActionControl(unsigned int keyCode, ActionControl& control);
	void AddKeyBinaryControl(unsigned int keyCode, BinaryControl& control);

	void AddButtonAxisControl(unsigned int mouseButton, AxisControl& control, float weight = 1.0f);
	void AddButtonActionControl(unsigned int mouseButton, ActionControl& control);
	void AddButtonBinaryControl(unsigned int mouseButton, BinaryControl& control);

	void AddMouseMotionControl(MotionControl& control, float weight = 1.0f);

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
};