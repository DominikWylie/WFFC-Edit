#pragma once

#include "pch.h"
#include <memory>
//#include <d3d11.h>
#include <SimpleMath.h>
#include "DeviceResources.h"
#include "InputObserver.h"

class DisplayObject;
class ID3D11DeviceContext;
class ID3D11Device;

namespace XD {
	class DeviceResources;
}

//namespace std {
//	class shared_ptr;
//}

using namespace DirectX::SimpleMath;
using namespace DirectX;

class ObjectEditor : public InputObserver
{
public:
	ObjectEditor();
	~ObjectEditor();

	void Initialize(ID3D11DeviceContext* con, ID3D11Device* dev);

	void DrawTranslators(Matrix view, Matrix projection, DirectX::SimpleMath::Matrix& m_world, RECT winRect, std::shared_ptr<DX::DeviceResources>& deviceResources);

	void updateObject(DisplayObject* object);
	DisplayObject* getDisplayObject();

	bool getTranslatorHovered();

	bool objectSelected = false;

	virtual void KeyDown(int key) override;
	virtual void KeyUp(int key) override;
	virtual void mouseDown(MouseInput mouse) override;
	virtual void mouseUp(MouseInput mouse) override;
	virtual void scrollWheelMove(float wheel) override;
	virtual void mousePosition(DirectX::SimpleMath::Vector2 mousePosition) override;

	virtual std::vector<int> getKeysToObserve() override { return{}; };
	virtual std::vector<MouseInput> getMouseInputsToObserve() override { return{ LMB }; };
	virtual bool getScrollWheelToObserve() override { return false; };
	virtual bool getMousePositionToObserve() override { return true; };

private:

	void drawX();
	void drawY();
	void drawZ();

	void drawXY();
	void drawYZ();
	void drawZX();

	float translatorLength = 2.f;
	float planeTranslatorLength;
	int collidedTranslator = -1;
	bool translatorHovered = false;


	Vector2 mousePos = Vector2(1, 1);

	std::vector<DirectX::BoundingBox> boxList;

	enum {
		x,
		y,
		z,
		none
	} dragAxis;

	XMVECTOR chosenPlane = XMVectorSet(0.f, 0.f, 0.f, 0.f);

	bool RMBDown = false;

	DisplayObject* selectedObject = nullptr;

	Vector3 cursorPlanePoint = Vector3(0, 0, 0);
	Vector3 objectCentreOffset = Vector3(0, 0, 0);
	bool firstRound = true;

	//std::shared_ptr<DX::DeviceResources> deviceResources;

	float cubeRadius = 0.1f;

	ID3D11DeviceContext* context;
	ID3D11Device* device;

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> primitiveBatch;
	std::unique_ptr<DirectX::BasicEffect> basicEffect;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
};

