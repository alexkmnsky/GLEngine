#include "Application.h"

Application::Application(Display* display)
{
	this->isClosed = false;
	this->isMouseLocked = false;
	this->display = display;
}

void Application::ProcessEvents(IApplicationEventHandler& eventHandler)
{
	eventHandler.Update();

	SDL_Event e;

	// Loop over all application events
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		// If e.key.repeat is not zero, then it is a repeat
		case SDL_KEYDOWN:
			eventHandler.OnKeyDown(e.key.keysym.scancode, e.key.repeat != 0);
			break;
		case SDL_KEYUP:
			eventHandler.OnKeyUp(e.key.keysym.scancode, e.key.repeat != 0);
			break;
		case SDL_MOUSEBUTTONDOWN:
			eventHandler.OnMouseDown(e.button.button, e.button.clicks);
			break;
		case SDL_MOUSEBUTTONUP:
			eventHandler.OnMouseUp(e.button.button, e.button.clicks);
			break;
		case SDL_MOUSEMOTION:
			// Ignore motion if mouse is not locked
			if (isMouseLocked)
			{
				eventHandler.OnMouseMove(e.motion.x, e.motion.y, e.motion.xrel, e.motion.yrel);
			}
			break;
		case SDL_WINDOWEVENT:
			if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
			{
				display->Resize(e.window.data1, e.window.data2);
			}
			break;
		// Close window
		case SDL_QUIT:
			isClosed = true;
			break;
		default:
			break;
		};
	}
}

bool Application::ToggleMouseLock()
{
	isMouseLocked = !isMouseLocked;
	SDL_SetRelativeMouseMode(isMouseLocked ? SDL_TRUE : SDL_FALSE);

	return isMouseLocked;
}
