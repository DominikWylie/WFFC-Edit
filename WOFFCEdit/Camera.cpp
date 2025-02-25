#include "Camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Camera::Camera()
{
	//functional
	m_movespeed = 0.30;
	m_camRotRate = 3.0;

	//camera
	m_camPosition.x = 0.0f;
	m_camPosition.y = 3.7f;
	m_camPosition.z = -3.5f;

	m_camOrientation.x = 0;
	m_camOrientation.y = 0;
	m_camOrientation.z = 0;

	m_camLookAt.x = 0.0f;
	m_camLookAt.y = 0.0f;
	m_camLookAt.z = 0.0f;

	m_camLookDirection.x = 0.0f;
	m_camLookDirection.y = 0.0f;
	m_camLookDirection.z = 0.0f;

	m_camRight.x = 0.0f;
	m_camRight.y = 0.0f;
	m_camRight.z = 0.0f;

	cursor = LoadCursor(NULL, IDC_ARROW);
}

Camera::~Camera() {

}

Matrix Camera::Update(InputCommands& m_InputCommands, RECT windowRect) {

	//TODO  any more complex than this, and the camera should be abstracted out to somewhere else
	//camera motion is on a plane, so kill the 7 component of the look direction
	Vector3 planarMotionVector = m_camLookDirection;
	planarMotionVector.y = 0.0;

	if (m_InputCommands.rotRight)
	{
		m_camOrientation.y -= m_camRotRate;
	}
	if (m_InputCommands.rotLeft)
	{
		m_camOrientation.y += m_camRotRate;
	}


	if (m_InputCommands.RMBDown) {
		if (m_InputCommands.RMBClicked) {
			//new click
			mouseAnchor.x = m_InputCommands.mousePosX;
			mouseAnchor.y = m_InputCommands.mousePosY;
			//ShowCursor(false);
			
		}

		//doesnt work in clicked
		SetCursor(NULL);

		m_camOrientation.y -= m_InputCommands.mousePosX - mouseAnchor.x;
		m_camOrientation.x -= m_InputCommands.mousePosY - mouseAnchor.y;

		//reset mouse pos to anchor

		SetCursorPos(mouseAnchor.x, mouseAnchor.y);
	}
	else if (m_InputCommands.RMBUnclick) {
		//ShowCursor(true);
		SetCursor(cursor);
		m_InputCommands.RMBUnclick = false;
	}

	float pitch = m_camOrientation.y * (3.1415 / 180);
	float yaw = m_camOrientation.x * (3.1415 / 180);

	m_camLookDirection.x = sinf(pitch) * cosf(yaw);
	m_camLookDirection.y = sinf(yaw);
	m_camLookDirection.z = cosf(yaw) * cosf(pitch);

	//create right vector from look Direction
	m_camLookDirection.Cross(Vector3::UnitY, m_camRight);

	//process input and update stuff
	if (m_InputCommands.forward)
	{
		m_camPosition += m_camLookDirection * m_movespeed;
	}
	if (m_InputCommands.back)
	{
		m_camPosition -= m_camLookDirection * m_movespeed;
	}
	if (m_InputCommands.right)
	{
		m_camPosition += m_camRight * m_movespeed;
	}
	if (m_InputCommands.left)
	{
		m_camPosition -= m_camRight * m_movespeed;
	}

	//update lookat point
	m_camLookAt = m_camPosition + m_camLookDirection;

	//apply camera vectors
	return Matrix::CreateLookAt(m_camPosition, m_camLookAt, Vector3::UnitY);

}
