#include "Input.h"


Input::Input(InputCommands& toolInput) : m_toolInputCommands(toolInput) {
}

Input::~Input()
{

}

void Input::Update(MSG* msg, int windowX, int windowY)
{
	switch (msg->message)
	{
		//Global inputs,  mouse position and keys etc
	case WM_KEYDOWN:

		for (InputObserver* observer : observerKeyArray[msg->wParam]) {
			observer->KeyDown(msg->wParam);
		}

		break;

	//case WM_SYSKEYDOWN:

	//	if (msg->wParam == VK_LSHIFT) {
	//		int reee = 2;
	//	}

	case WM_KEYUP:

		for (InputObserver* observer : observerKeyArray[msg->wParam]) {
			observer->KeyUp(msg->wParam);
		}

		break;

	case WM_MOUSEMOVE:
	{
		DirectX::SimpleMath::Vector2 pos = DirectX::SimpleMath::Vector2(msg->pt.x - windowX, msg->pt.y - windowY);

		for (InputObserver* observer : observerMousePositionArray) {
			observer->mousePosition(pos);
		}

		break;
	}
	case WM_RBUTTONDOWN:

		for (InputObserver* observer : observerMouseArray[RMB]) {
			observer->mouseDown(RMB);
		}

		break;
	case WM_RBUTTONUP:

		for (InputObserver* observer : observerMouseArray[RMB]) {
			observer->mouseUp(RMB);
		}

		break;
	case WM_LBUTTONDOWN:

		for (InputObserver* observer : observerMouseArray[LMB]) {
			observer->mouseDown(LMB);
		}

		break;
	case WM_LBUTTONUP:

		for (InputObserver* observer : observerMouseArray[LMB]) {
			observer->mouseUp(LMB);
		}

		break;
	case WM_MOUSEWHEEL:

		float wheelDelta = (float)GET_WHEEL_DELTA_WPARAM(msg->wParam) / 120;

		for (InputObserver* observer : observerScrollWheellArray) {
			observer->scrollWheelMove(wheelDelta);
		}

		break;
	}

	//esc
	if (msg->wParam == VK_ESCAPE) {
		PostQuitMessage(0);
	}

}


void Input::Tick()
{

}

void Input::AttachObserver(InputObserver* observer)
{
	std::vector<int> keys = observer->getKeysToObserve();

	for (int key : keys) {
		observerKeyArray[key].push_back(observer);
	}

	std::vector<MouseInput> mouses = observer->getMouseInputsToObserve();

	for (MouseInput mouse : mouses) {
		observerMouseArray[mouse].push_back(observer);
	}

	if (observer->getScrollWheelToObserve()) {
		observerScrollWheellArray.push_back(observer);
	}

	if (observer->getMousePositionToObserve()) {
		observerMousePositionArray.push_back(observer);
	}
}
