#include "ObjectEditor.h"
#include "DisplayObject.h"
#include "DeviceResources.h"
#include "Picker.h"



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

void ObjectEditor::DrawTranslators(Matrix view, Matrix projection, DirectX::SimpleMath::Matrix& m_world, RECT winRect, std::shared_ptr<DX::DeviceResources>& deviceResources)
{
	if (!objectSelected) {
		return;
	}

	std::vector<DirectX::BoundingBox> boxList;

	DirectX::BoundingBox boxX;

	boxX.Center = selectedObject->m_position + DirectX::XMFLOAT3((translatorLength / 2), 0.f, 0.f);
	boxX.Extents = DirectX::XMFLOAT3(translatorLength / 2, cubeRadius, cubeRadius);

	boxList.push_back(boxX);

	DirectX::BoundingBox boxY;

	boxY.Center = selectedObject->m_position + DirectX::XMFLOAT3(0.f, translatorLength / 2, 0.f);
	boxY.Extents = DirectX::XMFLOAT3(cubeRadius, translatorLength / 2, cubeRadius);

	boxList.push_back(boxY);

	DirectX::BoundingBox boxZ;

	boxZ.Center = selectedObject->m_position + DirectX::XMFLOAT3(0.f, 0.f, translatorLength / 2);
	boxZ.Extents = DirectX::XMFLOAT3(cubeRadius, cubeRadius, translatorLength / 2);

	boxList.push_back(boxZ);

	collidedTranslator = Picker::MousePick(mousePos, boxList, m_world, projection, view, winRect, deviceResources);

	if (collidedTranslator != -1) {

		//get the axis
		//set the location of the respective plane to where the mouse is
		//make a plane on that axis sounds easier and will let me move 2 axes at the same time


		//boxList
	}

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

	drawX();
	drawY();
	drawZ();

	primitiveBatch->End();

	context->OMSetDepthStencilState(oldDepthStencilState.Get(), 0);
}

void ObjectEditor::updateObject(DisplayObject* object)
{
	selectedObject = object;
}

void ObjectEditor::KeyDown(int key)
{
}

void ObjectEditor::KeyUp(int key)
{
}

void ObjectEditor::mouseDown(MouseInput mouse)
{
}

void ObjectEditor::mouseUp(MouseInput mouse)
{
}

void ObjectEditor::scrollWheelMove(float wheel)
{
}

void ObjectEditor::mousePosition(DirectX::SimpleMath::Vector2 mousePosition)
{
	mousePos = mousePosition;
}

void ObjectEditor::drawX()
{
	DirectX::XMVECTORF32 colour;

	if (collidedTranslator == 0) {
		colour = DirectX::Colors::White;
	}
	else {
		colour = DirectX::Colors::Red;
	}

	DirectX::SimpleMath::Vector3 endPos = selectedObject->m_position + Vector3{ translatorLength, 0.f, 0.f };

	//x
	primitiveBatch->DrawLine(
		DirectX::VertexPositionColor(selectedObject->m_position, colour),
		DirectX::VertexPositionColor(endPos, colour)
	);

	//back plate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{ 0.f, -cubeRadius, cubeRadius }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ 0.f, cubeRadius, cubeRadius }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ 0.f, cubeRadius, -cubeRadius }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ 0.f, -cubeRadius,-cubeRadius }, colour)
	);

	//front plate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius * 2, -cubeRadius, cubeRadius }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius * 2, cubeRadius, cubeRadius }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius * 2, cubeRadius, -cubeRadius }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius * 2, -cubeRadius,-cubeRadius }, colour)
	);

	//side plate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{ 0.f, -cubeRadius, cubeRadius }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius * 2, -cubeRadius, cubeRadius }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius * 2, -cubeRadius, -cubeRadius }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ 0.f, -cubeRadius, -cubeRadius }, colour)
	);

	//other side plate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{ 0.f, cubeRadius, cubeRadius }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius * 2, cubeRadius, cubeRadius }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius * 2, cubeRadius, -cubeRadius }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ 0.f, cubeRadius, -cubeRadius }, colour)
	);

	//top plate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{ 0.f, -cubeRadius, cubeRadius }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius * 2, -cubeRadius, cubeRadius }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius * 2, cubeRadius, cubeRadius }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ 0.f, cubeRadius, cubeRadius }, colour)
	);

	//bottom plate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{ 0.f, -cubeRadius, -cubeRadius }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius * 2, -cubeRadius, -cubeRadius }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius * 2, cubeRadius, -cubeRadius }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ 0.f, cubeRadius, -cubeRadius }, colour)
	);
}

void ObjectEditor::drawY()
{
	DirectX::XMVECTORF32 colour;

	if (collidedTranslator == 1) {
		colour = DirectX::Colors::White;
	}
	else {
		colour = DirectX::Colors::Green;
	}

	DirectX::SimpleMath::Vector3 endPos = selectedObject->m_position + Vector3{ 0.f, translatorLength, 0.f };

	//y
	primitiveBatch->DrawLine(
		DirectX::VertexPositionColor(selectedObject->m_position, colour),
		DirectX::VertexPositionColor(endPos, colour)
	);

	//back plate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, 0.f, cubeRadius}, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, 0.f, cubeRadius}, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, 0.f , -cubeRadius}, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, 0.f, -cubeRadius}, colour)
	);

	//front plate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, cubeRadius * 2, cubeRadius }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, cubeRadius * 2, cubeRadius }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, cubeRadius * 2 , -cubeRadius }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, cubeRadius * 2, -cubeRadius }, colour)
	);

	//side plate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, 0.f, cubeRadius }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, cubeRadius * 2, cubeRadius }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, cubeRadius * 2 , -cubeRadius }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, 0.f, -cubeRadius }, colour)
	);

	//other side plate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, 0.f, cubeRadius }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, cubeRadius * 2, cubeRadius }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, cubeRadius * 2 , -cubeRadius }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, 0.f, -cubeRadius }, colour)
	);

	//top plate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, 0.f, cubeRadius }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, cubeRadius * 2, cubeRadius }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, cubeRadius * 2 , cubeRadius }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, 0.f, cubeRadius }, colour)
	);

	//bottom plate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, 0.f, -cubeRadius }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, cubeRadius * 2, -cubeRadius }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, cubeRadius * 2 , -cubeRadius }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, 0.f, -cubeRadius }, colour)
	);
}

void ObjectEditor::drawZ()
{
	DirectX::XMVECTORF32 colour;

	if (collidedTranslator == 2) {
		colour = DirectX::Colors::White;
	}
	else {
		colour = DirectX::Colors::Blue;
	}
	//z
	primitiveBatch->DrawLine(
		DirectX::VertexPositionColor(selectedObject->m_position, colour),
		DirectX::VertexPositionColor(selectedObject->m_position + Vector3{ 0.f, 0.f, 2.f }, colour)
	);

	//make cube - migh change so it overlaps the line
	//so end is the centre of the back quad

	DirectX::SimpleMath::Vector3 endPos = selectedObject->m_position + Vector3{ 0.f, 0.f, translatorLength };

	//back plate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, cubeRadius, 0.f }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, cubeRadius, 0.f }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, -cubeRadius, 0.f }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, -cubeRadius, 0.f }, colour)
	);

	//front plate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, cubeRadius, cubeRadius * 2 }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, cubeRadius, cubeRadius * 2 }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, -cubeRadius, cubeRadius * 2 }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, -cubeRadius, cubeRadius * 2 }, colour)
	);

	//sidePlate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{-cubeRadius, cubeRadius, 0.f}, colour),
		DirectX::VertexPositionColor(endPos + Vector3{-cubeRadius, cubeRadius, cubeRadius * 2}, colour),
		DirectX::VertexPositionColor(endPos + Vector3{-cubeRadius, -cubeRadius, cubeRadius * 2}, colour),
		DirectX::VertexPositionColor(endPos + Vector3{-cubeRadius, -cubeRadius, 0.f}, colour)
	);

	//otherSidePlate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, cubeRadius, 0.f }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, cubeRadius, cubeRadius * 2 }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, -cubeRadius, cubeRadius * 2 }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, -cubeRadius, 0.f }, colour)
	);

	//top plate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, cubeRadius, 0.f }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, cubeRadius, cubeRadius * 2 }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, cubeRadius, cubeRadius * 2 }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, cubeRadius, 0.f }, colour)
	);

	//bottom plate
	primitiveBatch->DrawQuad(
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, -cubeRadius, 0.f }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ -cubeRadius, -cubeRadius, cubeRadius * 2 }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, -cubeRadius, cubeRadius * 2 }, colour),
		DirectX::VertexPositionColor(endPos + Vector3{ cubeRadius, -cubeRadius, 0.f }, colour)
	);
}
