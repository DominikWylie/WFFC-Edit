#pragma once

#include <afxwin.h>
#include "InputCommands.h"
#include "InputObserver.h"
#include <vector>
#include <map>

class Input
{
public:
	Input(InputCommands& toolInput);
	~Input();

	void Update(MSG *msg);
	void Tick();

	void AttachObserver(InputObserver* observer);

private:
	static const int KEYARRAYSIZE = 256;
	char	m_keyArray[KEYARRAYSIZE];
	InputCommands& m_toolInputCommands;

	std::vector<InputObserver*> observerArray[256];
};