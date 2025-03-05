#pragma once

#include "d3d11.h"
#include <vector>
#include <SimpleMath.h>

enum MouseInput {
	RMB,
	LMB,
	MMB
};

class InputObserver
{
public:
	virtual void KeyDown(int key) = 0;
	virtual void KeyUp(int key) = 0;
	virtual void mouseDown(MouseInput mouse) = 0;
	virtual void mouseUp(MouseInput mouse) = 0;
	virtual void mousePosition(DirectX::SimpleMath::Vector2 mousePosition) = 0;
	virtual void scrollWheelMove(float wheel) = 0;

	virtual std::vector<int> getKeysToObserve() = 0;
	virtual std::vector<MouseInput> getMouseInputsToObserve() = 0;
	virtual bool getScrollWheelToObserve() = 0;
	virtual bool getMousePositionToObserve() = 0;
};

