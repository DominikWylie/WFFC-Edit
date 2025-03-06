#include "ObjectEditor.h"
#include "DisplayObject.h"
#include <memory>
#include <d3d11.h>

//ObjectEditor::ObjectEditor(ID3D11DeviceContext* con, ID3D11Device* dev) : context(con), device(dev)
//{
//}

ObjectEditor::ObjectEditor(std::shared_ptr<DX::DeviceResources> m_deviceResources) : deviceResources(m_deviceResources)
{
}

ObjectEditor::~ObjectEditor()
{
}

void ObjectEditor::Initialize()
{

	auto context = deviceResources->GetD3DDeviceContext();
	auto device = deviceResources->GetD3DDevice();

	// Create PrimitiveBatch for rendering
	primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(context);

	// Create BasicEffect for simple shaders
	basicEffect = std::make_unique<DirectX::BasicEffect>(device);
	basicEffect->SetVertexColorEnabled(true);

	// Create input layout for vertex shader
	void const* shaderBytecode;
	size_t bytecodeLength;
	basicEffect->GetVertexShaderBytecode(&shaderBytecode, &bytecodeLength);
	device->CreateInputLayout(
		DirectX::VertexPositionColor::InputElements,
		DirectX::VertexPositionColor::InputElementCount,
		shaderBytecode, bytecodeLength,
		inputLayout.GetAddressOf()
	);
}

void ObjectEditor::DrawTranslators(DisplayObject object)
{

	//primitiveBatch->DrawLine(
	//    VertexPositionColor(m_displayList[selectedObject].m_position, { 1.f, 0.f, 0.f, 1.f }),
	//    VertexPositionColor(m_displayList[selectedObject].m_position + DirectX::SimpleMath::Vector3{ 5.f, 0.f, 0.f }, { 1.f, 0.f, 0.f, 1.f })
	//);

	auto context = deviceResources->GetD3DDeviceContext();

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dissableDepthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> oldDepthStencilState;

	// Disable depth testing (always draw on top)
	D3D11_DEPTH_STENCIL_DESC dissableDepthStencilDesc = {};
	dissableDepthStencilDesc.DepthEnable = FALSE;  // Disable depth test
	dissableDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dissableDepthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;


	deviceResources->GetD3DDevice()->CreateDepthStencilState(&dissableDepthStencilDesc, dissableDepthStencilState.GetAddressOf());

	D3D11_DEPTH_STENCIL_DESC oldDepthStencilDesc = {};
	context->OMGetDepthStencilState(oldDepthStencilState.GetAddressOf(), nullptr);

	// Set the "always on top" depth state
	context->OMSetDepthStencilState(dissableDepthStencilState.Get(), 0);

	context->IASetInputLayout(inputLayout.Get());
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// Apply world, view, and projection transformations
	basicEffect->SetWorld(Matrix::Identity);
	basicEffect->SetView(m_view);
	basicEffect->SetProjection(m_projection);
	basicEffect->Apply(context);

	primitiveBatch->Begin();


	primitiveBatch->DrawLine(
		VertexPositionColor(DirectX::SimpleMath::Vector3{ 0.f, 0.f, 0.f }, { 1.f, 0.f, 0.f, 1.f }),
		VertexPositionColor(DirectX::SimpleMath::Vector3{ 300.f, 300.f, 300.f }, { 1.f, 0.f, 0.f, 1.f })
	);

	primitiveBatch->End();

	context->OMSetDepthStencilState(oldDepthStencilState.Get(), 0);
}
