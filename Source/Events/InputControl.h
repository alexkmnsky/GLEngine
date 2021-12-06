#pragma once

#include <algorithm>

/**
 * @brief Used for storing the input value for keys/mouse buttons. Clamped from -1 to 1. This can
 * be used for multiple keys/buttons, where different inputs have different influences on the
 * input control value.
 */
class InputControl
{
public:
	InputControl()
	{
		amount = 0.0f;
	}

	inline void AddAmount(float amount)	{ this->amount += amount; }
	inline float GetAmount() { return std::clamp(amount, -1.0f, 1.0f); }

private:
	float amount;
};