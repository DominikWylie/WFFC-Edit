#pragma once

#include "pch.h"
#include <memory>
#include <d3d11.h>
#include <SimpleMath.h>
#include "DeviceResources.h"

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

class ObjectEditor
{
public:
	ObjectEditor();
	~ObjectEditor();

	void Initialize(ID3D11DeviceContext* con, ID3D11Device* dev);

	void DrawTranslators(Matrix view, Matrix projection, Vector2 mousePos, DirectX::SimpleMath::Matrix& m_world, RECT winRect, std::shared_ptr<DX::DeviceResources>& deviceResources);

	void updateObject(DisplayObject* object);

	bool objectSelected = false;

private:

	void drawX();
	void drawY();
	void drawZ();

	float translatorLength = 2.f;
	int collidedTranslator = -1;

	DisplayObject* selectedObject = nullptr;

	//std::shared_ptr<DX::DeviceResources> deviceResources;

	float cubeRadius = 0.1f;

	ID3D11DeviceContext* context;
	ID3D11Device* device;

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> primitiveBatch;
	std::unique_ptr<DirectX::BasicEffect> basicEffect;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
};

