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

	void DrawTranslators(DisplayObject& object, Matrix view, Matrix projection);

private:

	void drawX(DisplayObject& object);
	void drawY(DisplayObject& object);
	void drawZ(DisplayObject& object);

	//std::shared_ptr<DX::DeviceResources> deviceResources;

	ID3D11DeviceContext* context;
	ID3D11Device* device;

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> primitiveBatch;
	std::unique_ptr<DirectX::BasicEffect> basicEffect;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
};

