#include "CommandManager.h"

void CommandManager::KeyDown(int key)
{
	keysDown.push_back(key);
	checkAndCallObservers();
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
	//check all combos
	for (auto keyCombo : keyCombos) {
		//check al keys against the combo key

		int keyAmount = 0;
		
		for (int keyInCombo : keyCombo.second) {

			bool match = false;

			for (int keyDown : keysDown) {
				if (keyDown == keyInCombo) {
					match = true;
				}
			}

			//if (keyAmount == keyCombo.second.size()) {
			//	//found combo!
			//	int ree = 5;
			//}

			keyAmount++;
			if (match) {
				//continue to check the next
				if (keyAmount == keyCombo.second.size()) {
					//found combo!
					for (CommandObserver* observer : observerList[keyCombo.first]) {
						observer->commandCall(keyCombo.first);
					}
				}
				match = false;
				continue;
			}
			else{
				//go to next combo
				break;
			}
		}
		//if no match found, go to next combo
	}
	
	
	
	
	
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
