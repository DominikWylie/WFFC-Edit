#include "Input.h"

Input::Input(InputCommands& toolInput) : m_toolInputCommands(toolInput) {
}

Input::~Input()
{
}

void Input::Update(MSG* msg)
{
	switch (msg->message)
	{
		//Global inputs,  mouse position and keys etc
	case WM_KEYDOWN:
		m_keyArray[msg->wParam] = true;
		break;

	case WM_KEYUP:
		m_keyArray[msg->wParam] = false;
		break;

	case WM_MOUSEMOVE:
		break;
	case WM_RBUTTONDOWN:
		m_toolInputCommands.RMBClicked = true;
		break;
	case WM_RBUTTONUP:
		m_toolInputCommands.RMBDown = false;
		m_toolInputCommands.RMBClicked = false;
		m_toolInputCommands.RMBUnclick = true;
		break;
	case WM_LBUTTONDOWN:
		//not too sure the best way nut down with button down for now
		m_toolInputCommands.LMBClicked = true;
		break;
	case WM_LBUTTONUP:
		m_toolInputCommands.LMBDown = false;
		m_toolInputCommands.LMBClicked = false;
		m_toolInputCommands.LMBUnclick = true;
		break;
	case WM_MOUSEWHEEL:
		//m_toolInputCommands.wheelMoved = true;
		m_toolInputCommands.wheelDelta = (float)GET_WHEEL_DELTA_WPARAM(msg->wParam) / 120;
		break;
	}

	//esc
	if (m_keyArray[VK_ESCAPE]) {
		PostQuitMessage(0);
	}

	m_toolInputCommands.mousePosX = msg->pt.x;
	m_toolInputCommands.mousePosY = msg->pt.y;

	//here we update all the actual app functionality that we want.  This information will either be used int toolmain, or sent down to the renderer (Camera movement etc
	//WASD movement
	if (m_keyArray['W'])
	{
		m_toolInputCommands.forward = true;
	}
	else m_toolInputCommands.forward = false;

	if (m_keyArray['S'])
	{
		m_toolInputCommands.back = true;
	}
	else m_toolInputCommands.back = false;
	if (m_keyArray['A'])
	{
		m_toolInputCommands.left = true;
	}
	else m_toolInputCommands.left = false;

	if (m_keyArray['D'])
	{
		m_toolInputCommands.right = true;
	}
	else m_toolInputCommands.right = false;
	//rotation
	if (m_keyArray['E'])
	{
		m_toolInputCommands.fall = true;
	}
	else m_toolInputCommands.fall = false;
	if (m_keyArray['Q'])
	{
		m_toolInputCommands.rise = true;
	}
	else m_toolInputCommands.rise = false;
}

void Input::tick()
{
	//if clicked, set down to true, if both true set clicked to false, RMBclicked is set in Update
	if (m_toolInputCommands.RMBClicked && !m_toolInputCommands.RMBDown) {
		m_toolInputCommands.RMBDown = true;
	}
	else if (m_toolInputCommands.RMBClicked && m_toolInputCommands.RMBDown) {
		m_toolInputCommands.RMBClicked = false;
	}

	if (m_toolInputCommands.LMBClicked && !m_toolInputCommands.LMBDown) {
		m_toolInputCommands.LMBDown = true;
	}
	else if (m_toolInputCommands.LMBClicked && m_toolInputCommands.LMBDown) {
		m_toolInputCommands.LMBClicked = false;
	}


	if (m_toolInputCommands.wheelDelta < -0.1f || m_toolInputCommands.wheelDelta > 0.1f) {
		m_toolInputCommands.wheelDelta = 0.f;
	}
}
