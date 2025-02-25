#pragma once

#include "d3d11.h"
#include <SimpleMath.h>
#include "InputCommands.h"
#include "StepTimer.h"

class Camera
{
public:
	Camera();
	~Camera();

	DirectX::SimpleMath::Matrix Update(InputCommands& m_InputCommands, RECT windowRect, DX::StepTimer const& timer);

	DirectX::SimpleMath::Vector3 getPosition() { return m_camPosition; }

private:

	//camera
	DirectX::SimpleMath::Vector3 m_camPosition = DirectX::SimpleMath::Vector3(0.f, 3.7f, -3.5f);
	DirectX::SimpleMath::Vector3 m_camOrientation = DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f);
	DirectX::SimpleMath::Vector3 m_camLookAt = DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f);;
	DirectX::SimpleMath::Vector3 m_camLookDirection = DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f);;
	DirectX::SimpleMath::Vector3 m_camRight = DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f);;

	////functionality
	float m_movespeed = 0.3;

	float previousMouseX = -1;
	float previousMouseY = -1;

	//doing custom anchor so the mouse will be in the same place after camera move
	DirectX::SimpleMath::Vector2 mouseAnchor = DirectX::SimpleMath::Vector2(5, 5);

	HCURSOR cursor;

	float cameraMoveSpeed = 7.f;
};

