#include "HistoryManager.h"
#include "DisplayObject.h"

HistoryManager::HistoryManager()
{
}

HistoryManager::~HistoryManager()
{
}

//position from front isnt wotking properly and goes out of sync

void HistoryManager::addHistory(std::vector<ObjectDelta> newHistory)
{
	while (positionFromFront > 0) {
		history.pop_back();
		positionFromFront--;
	}

	//so this is the real location, the history starts 1 step pack
	positionFromFront = -1;

	history.push_back(newHistory);
}

std::vector<ObjectDelta> HistoryManager::goBack()
{
	
	//if went back from end, record the end
	if (positionFromFront == -1) {

		std::vector<ObjectDelta> newHistory;

		for (const auto object : history.at(history.size() - 1)) {
			ObjectDelta objectTorecord;
			objectTorecord.object = object.object;
			objectTorecord.translate = object.object->m_position;
			objectTorecord.rotate = object.object->m_orientation;
			objectTorecord.scale = object.object->m_scale;
			newHistory.push_back(objectTorecord);
		}

		history.push_back(newHistory);

		positionFromFront = 0;
	}

	//i need to go now, contonue working on the hhgistory and redo, abouve is to record the crrent state of the changed objects when going back incase user goes back forward
	
	if (history.size() - 1 == positionFromFront) {
		positionFromFront++;
		return std::vector<ObjectDelta> {};
	}
	else if(history.size() == 0){
		return std::vector<ObjectDelta> {};
	}

	//history is added as action is completed so to go back, go back one and send
	positionFromFront++;
	return history.at((history.size() - 1) - positionFromFront);

	//need to use the oriogional state, maybe reload from db?
}

std::vector<ObjectDelta> HistoryManager::goForward()
{
	int index = positionFromFront;
	
	if (positionFromFront > 0) {
		positionFromFront -= 2;
	}

	return history.at((history.size() - 1) - index);
}

