#include "CommandManager.h"

void CommandManager::KeyDown(int key)
{
	keysDown.push_back(key);
}

void CommandManager::KeyUp(int key)
{
	//only one key at a time
	for (auto it = keysDown.begin(); it != keysDown.end(); ++it) {
		if (*it == key) {
			keysDown.erase(it);
			break;
		}
	}
}

void CommandManager::AttachObserver(CommandObserver* observer)
{
	std::vector<Command> commands = observer->getCommandsToObserve();

	for (Command command : commands) {
		observerList[command].push_back(observer);
	}
}

void CommandManager::checkAndCallObservers()
{
	for(auto keyCombo : keyCombos){

		bool allKeysFound = true;

		for (int keyForCombo : keyCombo.second) {
			//loop through, if not found key on pressed keys continue to next combo

			bool keyFound = false;

			for (int keyDown : keysDown) {
				if (keyDown == keyForCombo) {
					keyFound = true;
				}
			}

			if (!keyFound) {
				//yea just move on to next
				allKeysFound = false;
			}
		}

		if (allKeysFound) {
			for (CommandObserver* observer : observerList[keyCombo.first]) {
				observer->commandCall(keyCombo.first);
			}
		}
	}

}
