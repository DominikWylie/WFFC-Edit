#include "ObjectEditor.h"
#include "DisplayObject.h"
#include "DeviceResources.h"

//ObjectEditor::ObjectEditor(ID3D11DeviceContext* con, ID3D11Device* dev) : context(con), device(dev)
//{
//}

ObjectEditor::ObjectEditor() 
{
}

ObjectEditor::~ObjectEditor()
{
}

void ObjectEditor::Initialize(ID3D11DeviceContext* con, ID3D11Device* dev)
{

	context = con;
	device = dev;

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

void ObjectEditor::DrawTranslators(DisplayObject& object, Matrix view, Matrix projection)
{

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dissableDepthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> oldDepthStencilState;

	// Disable depth testing (always draw on top)
	D3D11_DEPTH_STENCIL_DESC dissableDepthStencilDesc = {};
	dissableDepthStencilDesc.DepthEnable = FALSE;  // Disable depth test
	dissableDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dissableDepthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;


	//deviceResources->GetD3DDevice()->CreateDepthStencilState(&dissableDepthStencilDesc, dissableDepthStencilState.GetAddressOf());
	device->CreateDepthStencilState(&dissableDepthStencilDesc, dissableDepthStencilState.GetAddressOf());

	D3D11_DEPTH_STENCIL_DESC oldDepthStencilDesc = {};
	context->OMGetDepthStencilState(oldDepthStencilState.GetAddressOf(), nullptr);

	// Set the "always on top" depth state
	context->OMSetDepthStencilState(dissableDepthStencilState.Get(), 0);

	context->IASetInputLayout(inputLayout.Get());
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	// Apply world, view, and projection transformations
	basicEffect->SetWorld(Matrix::Identity);
	basicEffect->SetView(view);
	basicEffect->SetProjection(projection);
	basicEffect->Apply(context);

	primitiveBatch->Begin();

	drawX(object);
	drawY(object);
	drawZ(object);

	//primitiveBatch->DrawQuad()

	//primitiveBatch->DrawLine(
	//	DirectX::VertexPositionColor(Vector3{ 0.f, 0.f, 0.f }, { 1.f, 0.f, 0.f, 1.f }),
	//	DirectX::VertexPositionColor(Vector3{ 300.f, 300.f, 300.f }, { 1.f, 0.f, 0.f, 1.f })
	//);

	primitiveBatch->End();

	context->OMSetDepthStencilState(oldDepthStencilState.Get(), 0);
}

void ObjectEditor::drawX(DisplayObject& object)
{
	DirectX::SimpleMath::Vector3 endPos = object.m_position + Vector3{ 2.f, 0.f, 0.f };

	//x
	primitiveBatch->DrawLine(
		DirectX::VertexPositionColor(object.m_position, { 1.f, 0.f, 0.f, 1.f }),
		DirectX::VertexPositionColor(endPos, { 1.f, 0.f, 0.f, 1.f })
	);

	float cubeRadius = 0.1f;

	//back plate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{ 0.f, -cubeRadius, cubeRadius }, DirectX::Colors::Red),
		DirectX::VertexPositionColor(endPos + Vector3{ 0.f, cubeRadius, cubeRadius }, DirectX::Colors::Red),
		DirectX::VertexPositionColor(endPos + Vector3{ 0.f, cubeRadius, -cubeRadius }, DirectX::Colors::Red),
		DirectX::VertexPositionColor(endPos + Vector3{ 0.f, -cubeRadius,-cubeRadius }, DirectX::Colors::Red)
	);

	//front plate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius * 2, -cubeRadius, cubeRadius }, DirectX::Colors::Red),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius * 2, cubeRadius, cubeRadius }, DirectX::Colors::Red),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius * 2, cubeRadius, -cubeRadius }, DirectX::Colors::Red),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius * 2, -cubeRadius,-cubeRadius }, DirectX::Colors::Red)
	);

	//side plate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{ 0.f, -cubeRadius, cubeRadius }, DirectX::Colors::Red),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius * 2, -cubeRadius, cubeRadius }, DirectX::Colors::Red),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius * 2, -cubeRadius, -cubeRadius }, DirectX::Colors::Red),
		DirectX::VertexPositionColor(endPos + Vector3{ 0.f, -cubeRadius, -cubeRadius }, DirectX::Colors::Red)
	);

	//other side plate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{ 0.f, cubeRadius, cubeRadius }, DirectX::Colors::Red),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius * 2, cubeRadius, cubeRadius }, DirectX::Colors::Red),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius * 2, cubeRadius, -cubeRadius }, DirectX::Colors::Red),
		DirectX::VertexPositionColor(endPos + Vector3{ 0.f, cubeRadius, -cubeRadius }, DirectX::Colors::Red)
	);

	//top plate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{ 0.f, -cubeRadius, cubeRadius }, DirectX::Colors::Red),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius * 2, -cubeRadius, cubeRadius }, DirectX::Colors::Red),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius * 2, cubeRadius, cubeRadius }, DirectX::Colors::Red),
		DirectX::VertexPositionColor(endPos + Vector3{ 0.f, cubeRadius, cubeRadius }, DirectX::Colors::Red)
	);

	//bottom plate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{ 0.f, -cubeRadius, -cubeRadius }, DirectX::Colors::Red),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius * 2, -cubeRadius, -cubeRadius }, DirectX::Colors::Red),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius * 2, cubeRadius, -cubeRadius }, DirectX::Colors::Red),
		DirectX::VertexPositionColor(endPos + Vector3{ 0.f, cubeRadius, -cubeRadius }, DirectX::Colors::Red)
	);
}

void ObjectEditor::drawY(DisplayObject& object)
{
	DirectX::SimpleMath::Vector3 endPos = object.m_position + Vector3{ 0.f, 2.f, 0.f };

	//y
	primitiveBatch->DrawLine(
		DirectX::VertexPositionColor(object.m_position, DirectX::Colors::Green),
		DirectX::VertexPositionColor(endPos, DirectX::Colors::Green)
	);
	
	float cubeRadius = 0.1f;

	//back plate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, 0.f, cubeRadius}, DirectX::Colors::Green),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, 0.f, cubeRadius}, DirectX::Colors::Green),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, 0.f , -cubeRadius}, DirectX::Colors::Green),
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, 0.f, -cubeRadius}, DirectX::Colors::Green)
	);

	//front plate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, cubeRadius * 2, cubeRadius }, DirectX::Colors::Green),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, cubeRadius * 2, cubeRadius }, DirectX::Colors::Green),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, cubeRadius * 2 , -cubeRadius }, DirectX::Colors::Green),
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, cubeRadius * 2, -cubeRadius }, DirectX::Colors::Green)
	);

	//side plate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, 0.f, cubeRadius }, DirectX::Colors::Green),
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, cubeRadius * 2, cubeRadius }, DirectX::Colors::Green),
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, cubeRadius * 2 , -cubeRadius }, DirectX::Colors::Green),
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, 0.f, -cubeRadius }, DirectX::Colors::Green)
	);

	//other side plate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, 0.f, cubeRadius }, DirectX::Colors::Green),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, cubeRadius * 2, cubeRadius }, DirectX::Colors::Green),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, cubeRadius * 2 , -cubeRadius }, DirectX::Colors::Green),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, 0.f, -cubeRadius }, DirectX::Colors::Green)
	);

	//top plate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, 0.f, cubeRadius }, DirectX::Colors::Green),
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, cubeRadius * 2, cubeRadius }, DirectX::Colors::Green),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, cubeRadius * 2 , cubeRadius }, DirectX::Colors::Green),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, 0.f, cubeRadius }, DirectX::Colors::Green)
	);

	//bottom plate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, 0.f, -cubeRadius }, DirectX::Colors::Green),
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, cubeRadius * 2, -cubeRadius }, DirectX::Colors::Green),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, cubeRadius * 2 , -cubeRadius }, DirectX::Colors::Green),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, 0.f, -cubeRadius }, DirectX::Colors::Green)
	);
}

void ObjectEditor::drawZ(DisplayObject& object)
{
	//z
	primitiveBatch->DrawLine(
		DirectX::VertexPositionColor(object.m_position, DirectX::Colors::Blue),
		DirectX::VertexPositionColor(object.m_position + Vector3{ 0.f, 0.f, 2.f }, DirectX::Colors::Blue)
	);

	//make cube - migh change so it overlaps the line
	//so end is the centre of the back quad

	DirectX::SimpleMath::Vector3 endPos = object.m_position + Vector3{ 0.f, 0.f, 2.f };

	float cubeRadius = 0.1f;

	//back plate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, cubeRadius, 0.f }, DirectX::Colors::Blue),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, cubeRadius, 0.f }, DirectX::Colors::Blue),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, -cubeRadius, 0.f }, DirectX::Colors::Blue),
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, -cubeRadius, 0.f }, DirectX::Colors::Blue)
	);

	//front plate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, cubeRadius, cubeRadius * 2 }, DirectX::Colors::Blue),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, cubeRadius, cubeRadius * 2 }, DirectX::Colors::Blue),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, -cubeRadius, cubeRadius * 2 }, DirectX::Colors::Blue),
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, -cubeRadius, cubeRadius * 2 }, DirectX::Colors::Blue)
	);

	//sidePlate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{-cubeRadius, cubeRadius, 0.f}, DirectX::Colors::Blue),
		DirectX::VertexPositionColor(endPos + Vector3{-cubeRadius, cubeRadius, cubeRadius * 2}, DirectX::Colors::Blue),
		DirectX::VertexPositionColor(endPos + Vector3{-cubeRadius, -cubeRadius, cubeRadius * 2}, DirectX::Colors::Blue),
		DirectX::VertexPositionColor(endPos + Vector3{-cubeRadius, -cubeRadius, 0.f}, DirectX::Colors::Blue)
	);

	//otherSidePlate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, cubeRadius, 0.f }, DirectX::Colors::Blue),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, cubeRadius, cubeRadius * 2 }, DirectX::Colors::Blue),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, -cubeRadius, cubeRadius * 2 }, DirectX::Colors::Blue),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, -cubeRadius, 0.f }, DirectX::Colors::Blue)
	);

	//top plate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, cubeRadius, 0.f }, DirectX::Colors::Blue),
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, cubeRadius, cubeRadius * 2 }, DirectX::Colors::Blue),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, cubeRadius, cubeRadius * 2 }, DirectX::Colors::Blue),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, cubeRadius, 0.f }, DirectX::Colors::Blue)
	);

	//bottom plate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, -cubeRadius, 0.f }, DirectX::Colors::Blue),
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, -cubeRadius, cubeRadius * 2 }, DirectX::Colors::Blue),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, -cubeRadius, cubeRadius * 2 }, DirectX::Colors::Blue),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, -cubeRadius, 0.f }, DirectX::Colors::Blue)
	);
}
