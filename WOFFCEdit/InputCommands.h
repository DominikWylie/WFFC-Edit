#pragma once

struct InputCommands
{
	bool forward = false;
	bool back = false;
	bool right = false;
	bool left = false;
	bool rise = false;
	bool fall = false;

	bool LMBDown = false;
	bool LMBClicked = false;
	bool LMBUnclick = false;

	bool RMBDown = false;
	bool RMBClicked = false;
	bool RMBUnclick = false;

	float mousePosX = 0;
	float mousePosY = 0;

	//this is reset evrey tick in toolmail
	float wheelDelta = 0.f;

	//fututure refactor - seperate abstract movement commands from key inputs 
	//(remove the literal lmb etc replace with "move camera" and have the literal keys in a different stuct if needed)
};
