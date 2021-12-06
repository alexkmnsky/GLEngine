#pragma once

#include "Events/IApplicationEventHandler.h"
#include "Rendering/Display.h"

class Application
{
public:
	Application(Display* display);

	/**
	 * Processes all currently pending application events.
	 *
	 * @param eventHandler Reference to an event handler which processes OnKeyDown, OnKeyUp,
	 *		OnMouseDown, OnMouseUp, and OnMouseMove events. The event handler must derive from
	 *		IApplicationEventHandler.
	 */
	void ProcessEvents(IApplicationEventHandler& eventHandler);

	inline bool IsClosed() { return isClosed; }

	/**
	 * @brief Toggles whether or not the mouse should be locked to the center of the screen..
	 * 
	 * @return If the mouse is now locked or not.
	 */
	bool ToggleMouseLock();

	virtual ~Application() {}

private:
	// Disallow copy and assign
	Application(const Application& other) = delete;
	Application& operator=(const Application& other) = delete;

	Display* display;
	bool isClosed;
	bool isMouseLocked;
};

