#pragma once

/** @brief Interface for application events we want to listen to. */
class IApplicationEventHandler
{
public:
	IApplicationEventHandler() {}
	virtual ~IApplicationEventHandler() {}

	/** @brief Called every frame before events are sent. */
	virtual void Update() {}

	virtual void OnKeyDown(unsigned int keyCode, bool isRepeat) {}
	virtual void OnKeyUp(unsigned int keyCode, bool isRepeat) {}
	virtual void OnMouseDown(unsigned int mouseButton, unsigned char numberOfClicks) {}
	virtual void OnMouseUp(unsigned int mouseButton, unsigned char numberOfClicks) {}
	virtual void OnMouseMove(unsigned int mousePositionX, unsigned int mousePositionY,
		int deltaX, int deltaY) {}

private:
	// Disallow copy and assign
	IApplicationEventHandler(const IApplicationEventHandler& other) = delete;
	void operator=(const IApplicationEventHandler& other) = delete;
};
