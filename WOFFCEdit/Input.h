#pragma once

#include <afxwin.h>
#include "InputCommands.h"

class Input
{
public:
	Input(InputCommands& toolInput);
	~Input();

	void Update(MSG *msg);
	void Tick();

private:
	char	m_keyArray[256];
	InputCommands& m_toolInputCommands;
};

