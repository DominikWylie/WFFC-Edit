#pragma once

#include "d3d11.h"
#include <SimpleMath.h>
#include "InputCommands.h"

class Camera
{
public:
	Camera();
	~Camera();

	DirectX::SimpleMath::Matrix Update(InputCommands& m_InputCommands, RECT windowRect);

	DirectX::SimpleMath::Vector3 getPosition() { return m_camPosition; }

private:

	//camera
	DirectX::SimpleMath::Vector3 m_camPosition;
	DirectX::SimpleMath::Vector3 m_camOrientation;
	DirectX::SimpleMath::Vector3 m_camLookAt;
	DirectX::SimpleMath::Vector3 m_camLookDirection;
	DirectX::SimpleMath::Vector3 m_camRight;
	float m_camRotRate;

	//functionality
	float m_movespeed;

	float previousMouseX = -1;
	float previousMouseY = -1;

	//doing custom anchor so the mouse will be in the same place after camera move
	DirectX::SimpleMath::Vector2 mouseAnchor = DirectX::SimpleMath::Vector2(5, 5);

	HCURSOR cursor;
};

