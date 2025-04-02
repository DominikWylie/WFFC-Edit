#include "HistoryManager.h"

HistoryManager::HistoryManager()
{
}

HistoryManager::~HistoryManager()
{
}

void HistoryManager::addHistory(std::vector<ObjectDelta> newHistory)
{
	history.push_back(newHistory);
}

std::vector<ObjectDelta> HistoryManager::goBack()
{
	//history is added as action is completed so to go back, go back one and send
	positionFromFront++;
	return history.at((history.size() - 1) - positionFromFront);
}

std::vector<ObjectDelta> HistoryManager::goForward()
{
	//if go forawrd when at forward do nothing
	if (positionFromFront == 0) {
		return history.at(history.size() - 1);
	}

	positionFromFront--;
	return history.at((history.size() - 1) - positionFromFront);
}

