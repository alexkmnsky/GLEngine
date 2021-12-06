#pragma once

/** @brief Used for storing the delta mouse motion along a single axis. */
class MotionControl
{
public:
	MotionControl()
	{
		amount = 0.0f;
	}

	enum class Axis
	{
		X,
		Y
	};

	inline void AddAmount(float amount) { this->amount += amount; }
	inline void ResetAmount() { this->amount = 0; }
	inline float GetAmount() { return amount; }

private:
	float amount;
};