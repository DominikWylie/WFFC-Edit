#pragma once

#include <afxwin.h>
#include <afxext.h>
#include "InputObserver.h"
#include <vector>
#include <map>

class Input
{
public:
	Input();
	~Input();

	void Update(MSG *msg, int windowX, int windowY);
	void Tick();

	void AttachObserver(InputObserver* observer);

private:
	static const int KEYARRAYSIZE = 256;
	char	m_keyArray[KEYARRAYSIZE];

	std::vector<InputObserver*> observerKeyArray[256];
	std::vector<InputObserver*> observerMouseArray[3];
	std::vector<InputObserver*> observerScrollWheellArray;
	std::vector<InputObserver*> observerMousePositionArray;
};