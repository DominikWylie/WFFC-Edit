#pragma once

#include "InputObserver.h"
#include "CommandObserver.h"
#include <vector>
#include <unordered_map>
#include <list>

class CommandManager : public InputObserver
{

public:
	virtual void KeyDown(int key) override;
	virtual void KeyUp(int key) override;
	virtual void mouseDown(MouseInput mouse) override;
	virtual void mouseUp(MouseInput mouse) override;
	virtual void scrollWheelMove(float wheel) override;
	virtual void mousePosition(DirectX::SimpleMath::Vector2 mousePosition) override;

	virtual std::vector<int> getKeysToObserve() override { return {VK_CONTROL, 'Z', 'C', 'V', 'Y'}; };
	virtual std::vector<MouseInput> getMouseInputsToObserve() override { return{}; };
	virtual bool getScrollWheelToObserve() override { return false; };
	virtual bool getMousePositionToObserve() override { return false; };

	void AttachObserver(CommandObserver* observer);

private:

	void checkAndCallObservers();

	std::vector<CommandObserver*> observerList[4];

	std::unordered_map<Command, std::vector<int>> keyCombos =
	{
		{Command::copy, {VK_CONTROL, 'C'}},
		{Command::paste, {VK_CONTROL, 'V'}},
		{Command::redo, {VK_CONTROL, 'Y'}},
		{Command::undo, {VK_CONTROL, 'Z'}}
	};

	std::list<int> keysDown;
};

