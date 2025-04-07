#include "HistoryManager.h"

HistoryManager::HistoryManager()
{
}

HistoryManager::~HistoryManager()
{
}

//diary for tomorrow:

//so when you select nothing first, theres issue with some psyco read access error in in sime dx11 math library. maybe try a diffrent pc?

//position from front isnt wotking properly and goes out of sync

void HistoryManager::addHistory(std::vector<ObjectDelta> newHistory)
{
	while (positionFromFront != 0) {
		history.pop_back();
		positionFromFront--;
	}

	history.push_back(newHistory);
}

std::vector<ObjectDelta> HistoryManager::goBack()
{
	switch (history.size()) {
	case 1:
		positionFromFront++;
		//history.clear();
	case 0:
		//an empty vector implies reload object
		return std::vector<ObjectDelta> {};
	}

	//history is added as action is completed so to go back, go back one and send
	positionFromFront++;
	return history.at((history.size() - 1) - positionFromFront);

	//need to use the oriogional state, maybe reload from db?
}

std::vector<ObjectDelta> HistoryManager::goForward()
{
	//if go forawrd when at front do nothing
	if (positionFromFront == 0) {
		return history.at(history.size() - 1);
	}

	positionFromFront--;
	return history.at((history.size() - 1) - positionFromFront);
}

