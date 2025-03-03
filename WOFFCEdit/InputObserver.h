#pragma once

#include <vector>

class InputObserver
{
public:
	//virtual InputObserver();
	virtual ~InputObserver();

	//virtual void update();
	virtual void keyDown(int key);
	virtual void keyUp(int key);

	virtual std::vector<int> getKeysToObserve();
};

