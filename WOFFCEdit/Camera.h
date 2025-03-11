#pragma once

#include "d3d11.h"
#include <SimpleMath.h>
#include "InputCommands.h"
#include "StepTimer.h"
#include "InputObserver.h"
#include <map>

class Camera : public InputObserver
{
public:
	Camera();
	~Camera();

	void Update(DX::StepTimer const& timer, RECT winRect);

	DirectX::SimpleMath::Vector3 getPosition() { return m_camPosition; }

	virtual void KeyDown(int key) override;
	virtual void KeyUp(int key) override;
	virtual void mouseDown(MouseInput mouse) override;
	virtual void mouseUp(MouseInput mouse) override;
	virtual void scrollWheelMove(float wheel) override;
	virtual void mousePosition(DirectX::SimpleMath::Vector2 mousePosition) override;

	virtual std::vector<int> getKeysToObserve() override;
	virtual std::vector<MouseInput> getMouseInputsToObserve() override;
	virtual bool getScrollWheelToObserve() override;
	virtual bool getMousePositionToObserve() override;


private:

	//camera
	DirectX::SimpleMath::Vector3 m_camPosition = DirectX::SimpleMath::Vector3(0.f, 3.7f, -3.5f);
	DirectX::SimpleMath::Vector3 m_camOrientation = DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f);
	DirectX::SimpleMath::Vector3 m_camLookAt = DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f);;
	DirectX::SimpleMath::Vector3 m_camLookDirection = DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f);;
	DirectX::SimpleMath::Vector3 m_camRight = DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f);;

public:
	////functionality
	float m_movespeed = 9.f;
private:

	DirectX::SimpleMath::Matrix view;
	DirectX::SimpleMath::Matrix projection;

	float previousMouseX = -1;
	float previousMouseY = -1;

	//doing custom anchor so the mouse will be in the same place after camera move
	DirectX::SimpleMath::Vector2 mouseAnchor = DirectX::SimpleMath::Vector2(5, 5);
	DirectX::SimpleMath::Vector2 mousePos = DirectX::SimpleMath::Vector2(5, 5);

	HCURSOR cursor;

	float cameraMoveSpeed = 7.f;

	std::vector<int> keys{ 'W', 'A', 'S', 'D', 'Q', 'E' };
	std::map<int, bool> KeysDown;
	std::vector<MouseInput> mice{ RMB, LMB };
	std::map<MouseInput, bool> miceDown;
	float mouseWheelDelta = 0.f;
	const float mouseWheelSensitivity = 5.f;

public:
	DirectX::SimpleMath::Matrix getView() { return view; }
	DirectX::SimpleMath::Matrix getProjection() { return projection; }
};

