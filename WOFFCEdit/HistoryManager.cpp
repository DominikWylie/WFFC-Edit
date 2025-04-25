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

	//will need to remove the data from the future history - maybe just empty the vector?

	//so this is the real location, the history starts 1 step pack
	positionFromFront = -1;
	positionOfFutureHistoryrecorded = -1;

	history.push_back(newHistory);
}

std::vector<ObjectDelta> HistoryManager::goBack()
{
	
	//add to the future history
	if (positionOfFutureHistoryrecorded < positionFromFront + 1) {

		std::vector<ObjectDelta> newHistory;

		for (const auto object : history.at((history.size() - 1) - (positionFromFront + 1))) {
			ObjectDelta objectTorecord;
			objectTorecord.object = object.object;
			objectTorecord.translate = object.object->m_position;
			objectTorecord.rotate = object.object->m_orientation;
			objectTorecord.scale = object.object->m_scale;
			newHistory.push_back(objectTorecord);
		}

		futureHistory.push_back(newHistory);

		positionOfFutureHistoryrecorded++;
	}

	if (history.size() - 1 == positionFromFront) {
		//positionFromFront++;
		return std::vector<ObjectDelta> {};
	}
	else if(history.size() == 0 || history.size() == positionFromFront){
		return std::vector<ObjectDelta> {};
	}

	//history is added as action is completed so to go back, go back one and send
	positionFromFront++;
	return history.at((history.size() - 1) - positionFromFront);

}

std::vector<ObjectDelta> HistoryManager::goForward()
{
	//when you go back take the object references and record where the pocation is befoire you go back


	int index = positionFromFront;
	
	if (positionFromFront > 0) {
		//positionFromFront -= 2;
		positionFromFront--;
	}

	////return history.at((history.size() - 1) - index);
	return futureHistory.at((history.size() - 1) - positionFromFront);



}

