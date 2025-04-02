#pragma once

#include "pch.h"
#include <memory>
//#include <d3d11.h>
#include <SimpleMath.h>
#include "DeviceResources.h"
#include "InputObserver.h"
#include <unordered_map>

class DisplayObject;
class ID3D11DeviceContext;
class ID3D11Device;

namespace XD {
	class DeviceResources;
}

using namespace DirectX::SimpleMath;
using namespace DirectX;

class ObjectEditor : public InputObserver
{
public:
	ObjectEditor();
	~ObjectEditor();

	void Initialize(ID3D11DeviceContext* con, ID3D11Device* dev);

	void DrawTranslators(
		Matrix view, 
		Matrix projection, 
		DirectX::SimpleMath::Matrix& m_world, 
		RECT winRect, 
		std::shared_ptr<DX::DeviceResources>& deviceResources
	);

	void updateObject(DisplayObject* object);
	DisplayObject* getDisplayObject();

	bool getTranslatorHovered();

	//set multiselect for game 
	void setMultiselect(bool set);

	bool objectSelected = false;

	virtual void KeyDown(int key) override;
	virtual void KeyUp(int key) override;
	virtual void mouseDown(MouseInput mouse) override;
	virtual void mouseUp(MouseInput mouse) override;
	virtual void scrollWheelMove(float wheel) override;
	virtual void mousePosition(DirectX::SimpleMath::Vector2 mousePosition) override;

	virtual std::vector<int> getKeysToObserve() override { return {VK_SHIFT, VK_CONTROL}; };
	virtual std::vector<MouseInput> getMouseInputsToObserve() override { return{ LMB }; };
	virtual bool getScrollWheelToObserve() override { return true; };
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
	bool shiftHeld = false;

	bool lineTransltion = false;
	enum {
		axisX, 
		axisY, 
		axisZ
	}lineAxis;

	enum {
		translate,
		scale,
		rotate
	}editState = translate;

	int circleSegments = 25;
	float circleRadius = 5.f;

	float scaleMultiplier = 0.1f;

	Vector2 mousePos = Vector2(1, 1);
	Vector2 prevMousePos = Vector2(1, 1);
	Vector2 mouseDelta = Vector2(1, 1);

	std::vector<DirectX::BoundingBox> boxList;

	XMVECTOR chosenPlane = XMVectorSet(0.f, 0.f, 0.f, 0.f);

	bool RMBDown = false;
	bool multiSelect = false;

	DisplayObject* masterObject = nullptr;

	//masterobject is not here, only the secondary selected objects
	//std::vector<DisplayObject*> selectedObjects;
	std::unordered_map<DisplayObject*, Vector3> selectedObjects;


	Vector3 cursorPlanePoint = Vector3(0, 0, 0);
	Vector3 objectCentreOffset = Vector3(0, 0, 0);
	bool firstRound = true;

	float cubeRadius = 0.1f;

	ID3D11DeviceContext* context;
	ID3D11Device* device;

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> primitiveBatch;
	std::unique_ptr<DirectX::BasicEffect> basicEffect;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
};

