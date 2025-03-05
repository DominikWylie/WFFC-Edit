#include "Camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Camera::Camera()
{
	cursor = LoadCursor(NULL, IDC_ARROW);

	for (int key : keys){
		KeysDown.insert({ key, false });
	}

	for (MouseInput mouse : mice) {
		miceDown.insert({ mouse, false });
	}
}

Camera::~Camera() {

}

Matrix Camera::Update(DX::StepTimer const& timer) {

	//camera motion is on a plane, so kill the 7 component of the look direction
	Vector3 planarMotionVector = m_camLookDirection;
	planarMotionVector.y = 0.0;

	float dt;

	//so camera cam be moved in the 1st second (not super importatnt but i did it)
	if (timer.GetFramesPerSecond() == 0) {
		dt = 1.f / 60.f;
	}
	else {
		dt = 1.f / timer.GetFramesPerSecond();
	}

	if (KeysDown.at('Q'))
	{
		m_camPosition.y -= (m_movespeed * dt);
	}
	if (KeysDown.at('E'))
	{
		m_camPosition.y += (m_movespeed * dt);
	}

	if (miceDown.at(RMB)) {

		SetCursor(NULL);

		m_camOrientation.y -= ((mousePos.x - mouseAnchor.x) * dt) * cameraMoveSpeed;
		m_camOrientation.x -= ((mousePos.y - mouseAnchor.y) * dt) * cameraMoveSpeed;

		//reset mouse pos to anchor

		SetCursorPos(mouseAnchor.x, mouseAnchor.y);

		m_movespeed += mouseWheelDelta * mouseWheelSensitivity;
		mouseWheelDelta = 0.f;

		if (m_movespeed < -0.5f) m_movespeed = 0.f;

	}

	float pitch = m_camOrientation.y * (3.1415 / 180);
	float yaw = m_camOrientation.x * (3.1415 / 180);

	m_camLookDirection.x = sinf(pitch) * cosf(yaw);
	m_camLookDirection.y = sinf(yaw);
	m_camLookDirection.z = cosf(yaw) * cosf(pitch);

	//create right vector from look Direction
	m_camLookDirection.Cross(Vector3::UnitY, m_camRight);

	//process input and update stuff
	if (KeysDown.at('W'))
	{
		m_camPosition += (m_camLookDirection * m_movespeed) * dt;
	}
	if (KeysDown.at('S'))
	{
		m_camPosition -= (m_camLookDirection * m_movespeed) * dt;
	}
	if (KeysDown.at('D'))
	{
		m_camPosition += (m_camRight * m_movespeed) * dt;
	}
	if (KeysDown.at('A'))
	{
		m_camPosition -= (m_camRight * m_movespeed) * dt;
	}

	//update lookat point
	m_camLookAt = m_camPosition + m_camLookDirection;

	//apply camera vectors
	return Matrix::CreateLookAt(m_camPosition, m_camLookAt, Vector3::UnitY);

	mouseWheelDelta = 0.f;
}

void Camera::KeyDown(int key)
{
	KeysDown.at(key) = true;
}

void Camera::KeyUp(int key)
{
	KeysDown.at(key) = false;
}

void Camera::mouseDown(MouseInput mouse)
{
	miceDown.at(mouse) = true;

	if (mouse == RMB) {
		mouseAnchor = mousePos;
		ShowCursor(false);
	}
}

void Camera::mouseUp(MouseInput mouse)
{
	miceDown.at(mouse) = false;

	if (mouse == RMB) {
		ShowCursor(true);
	}
}

void Camera::scrollWheelMove(float wheel)
{
	mouseWheelDelta = wheel;
}

void Camera::mousePosition(DirectX::SimpleMath::Vector2 mouse)
{
	mousePos = mouse;
}

std::vector<int> Camera::getKeysToObserve()
{
	return keys;
}

std::vector<MouseInput> Camera::getMouseInputsToObserve()
{
	return mice;
}

bool Camera::getScrollWheelToObserve()
{
	return true;
}

bool Camera::getMousePositionToObserve()
{
	return true;
}
