/** Copyright (c) 2022-2023 Alexander Kaminsky and Maxwell Hunt */

#pragma once

#include <queue>

class ActionControl
{
public:
	enum Action
	{
		PRESS, 
		RELEASE
	};
	
	ActionControl() {}

	inline void Push(Action action) { actions.push(action); }
	inline bool IsEmpty() { return actions.empty(); }
	inline Action Pop()
	{
		Action front = actions.front();
		actions.pop();
		return front;
	}

private:
	std::queue<Action> actions;
};