#pragma once

#include <vector>

enum MouseInput {
	RMB,
	LMB,
	SCROLLCLICK
};

class InputObserver
{
public:
	//virtual ~InputObserver() {};

	virtual void KeyDown(int key) = 0;
	virtual void KeyUp(int key) = 0;
	virtual void mouseDown(MouseInput mouse) = 0;
	virtual void mouseUp(MouseInput mouse) = 0;
	virtual void scrollWheelMove(float wheel) = 0;

	virtual std::vector<int> getKeysToObserve() = 0;
	virtual std::vector<MouseInput> getMouseInputsToObserve() = 0;
	virtual bool getScrollWheelToObserve() = 0;
};

