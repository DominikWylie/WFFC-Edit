#pragma once

#include "DeviceResources.h"
#include <SimpleMath.h>
#include <memory>

class DisplayObject;
//class D3D11DeviceContext;
//class D3D11Device;

//namespace XD {
//	class DeviceResources;
//}

class ObjectEditor
{
public:
	ObjectEditor(std::shared_ptr<DX::DeviceResources> m_deviceResources);
	//ObjectEditor(ID3D11DeviceContext* con, ID3D11Device* dev);
	~ObjectEditor();

	void Initialize();

	void DrawTranslators(DisplayObject object);

private:

	std::shared_ptr<DX::DeviceResources> deviceResources;

	//ID3D11DeviceContext* context;
	//ID3D11Device* device;

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> primitiveBatch;
	std::unique_ptr<DirectX::BasicEffect> basicEffect;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
};

