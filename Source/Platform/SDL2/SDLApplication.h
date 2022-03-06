#pragma once

#include "Events/IApplicationEventHandler.h"

class SDLApplication
{
public:
	static SDLApplication* Create();

	virtual ~SDLApplication();
	virtual void ProcessMessages(float deltaTime, IApplicationEventHandler& eventHandler);

	virtual bool IsRunning() { return isRunning; }

	virtual void LockMouse();
	virtual void UnlockMouse();

private:
	SDLApplication();

	// Disallow copy and assign
	SDLApplication(const SDLApplication& other) = delete;
	void operator=(const SDLApplication& other) = delete;

	bool isRunning;
	static unsigned int numInstances;
};
