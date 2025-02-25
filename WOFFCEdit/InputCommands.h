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

	//bool wheelMoved;
	float wheelDelta = 0.f;
};
