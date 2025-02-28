#include "Camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Camera::Camera()
{
	cursor = LoadCursor(NULL, IDC_ARROW);
}

Camera::~Camera() {

}

Matrix Camera::Update(InputCommands* m_InputCommands, RECT windowRect, DX::StepTimer const& timer) {

	//TODO  any more complex than this, and the camera should be abstracted out to somewhere else
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

	if (m_InputCommands->rise)
	{
		//m_camOrientation.y -= m_camRotRate;

		m_camPosition.y -= (m_movespeed * dt);
	}
	if (m_InputCommands->fall)
	{
		//m_camOrientation.y += m_camRotRate;

		m_camPosition.y += (m_movespeed * dt);

	}


	if (m_InputCommands->RMBDown) {
		if (m_InputCommands->RMBClicked) {
			//new click
			mouseAnchor.x = m_InputCommands->mousePosX;
			mouseAnchor.y = m_InputCommands->mousePosY;
			//ShowCursor(false);
		}

		SetCursor(NULL);


		//doesnt work in clicked

		m_camOrientation.y -= ((m_InputCommands->mousePosX - mouseAnchor.x) * dt) * cameraMoveSpeed;
		m_camOrientation.x -= ((m_InputCommands->mousePosY - mouseAnchor.y) * dt) * cameraMoveSpeed;

		//reset mouse pos to anchor

		SetCursorPos(mouseAnchor.x, mouseAnchor.y);

		m_movespeed += m_InputCommands->wheelDelta;

		if (m_movespeed < -0.5f) m_movespeed = 0.f;

	}
	else if (m_InputCommands->RMBUnclick) {
		//ShowCursor(true);
		SetCursor(cursor);
		m_InputCommands->RMBUnclick = false;
	}

	float pitch = m_camOrientation.y * (3.1415 / 180);
	float yaw = m_camOrientation.x * (3.1415 / 180);

	m_camLookDirection.x = sinf(pitch) * cosf(yaw);
	m_camLookDirection.y = sinf(yaw);
	m_camLookDirection.z = cosf(yaw) * cosf(pitch);

	//create right vector from look Direction
	m_camLookDirection.Cross(Vector3::UnitY, m_camRight);

	//process input and update stuff
	if (m_InputCommands->forward)
	{
		m_camPosition += (m_camLookDirection * m_movespeed) * dt;
	}
	if (m_InputCommands->back)
	{
		m_camPosition -= (m_camLookDirection * m_movespeed) * dt;
	}
	if (m_InputCommands->right)
	{
		m_camPosition += (m_camRight * m_movespeed) * dt;
	}
	if (m_InputCommands->left)
	{
		m_camPosition -= (m_camRight * m_movespeed) * dt;
	}

	//update lookat point
	m_camLookAt = m_camPosition + m_camLookDirection;

	//apply camera vectors
	return Matrix::CreateLookAt(m_camPosition, m_camLookAt, Vector3::UnitY);

}
