#pragma once

#include <vector>

class InputObserver
{
public:
	//virtual ~InputObserver() {};

	virtual void KeyDown(int key) = 0;
	virtual void KeyUp(int key) = 0;

	virtual std::vector<int> getKeysToObserve() = 0;
};

