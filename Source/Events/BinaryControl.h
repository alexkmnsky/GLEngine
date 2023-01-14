/** Copyright (c) 2022-2023 Alexander Kaminsky and Maxwell Hunt */

#pragma once

class BinaryControl
{
public:
	BinaryControl() : state(false) {}

	inline void Set(bool state) { this->state = state; }
	inline bool IsActive() { return state; }

private:
	bool state;
};