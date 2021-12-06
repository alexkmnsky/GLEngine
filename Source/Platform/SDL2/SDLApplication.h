#pragma once

#include "Events/IApplicationEventHandler.h"

class SDLApplication
{
public:
	static SDLApplication* Create();

	virtual ~SDLApplication();
	virtual void ProcessMessages(float deltaTime, IApplicationEventHandler& eventHandler);

	virtual inline bool IsRunning() { return isRunning; }

private:
	SDLApplication();

	// Disallow copy and assign
	SDLApplication(const SDLApplication& other) = delete;
	void operator=(const SDLApplication& other) = delete;

	bool isRunning;
	static unsigned int numInstances;
};
