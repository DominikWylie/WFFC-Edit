#include "ObjectEditor.h"
#include "DisplayObject.h"
#include "DeviceResources.h"
#include "Picker.h"
#include <DirectXMath.h>

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

	planeTranslatorLength = translatorLength / 3.f;
}

void ObjectEditor::DrawTranslators(Matrix view, Matrix projection, DirectX::SimpleMath::Matrix& m_world, RECT winRect, std::shared_ptr<DX::DeviceResources>& deviceResources)
{
	if (!objectSelected) {
		return;
	}

	//if not dragging
	if (!RMBDown) {
		collidedTranslator = Picker::MousePick(mousePos, boxList, m_world, projection, view, winRect, deviceResources);
	}

	if (collidedTranslator != -1) {
		translatorHovered = true;
	}
	else {
		translatorHovered = false;
	}

	if (collidedTranslator != -1 && RMBDown) {

		if (!firstRound) {
			if (linearTransltion) {
				switch (linearAxis) {
				case axisX:
					selectedObject->m_position.x = (cursorPlanePoint + objectCentreOffset).x;
					break;
				case axisY:
					selectedObject->m_position.y = (cursorPlanePoint + objectCentreOffset).y;
					break;
				case axisZ:
					selectedObject->m_position.z = (cursorPlanePoint + objectCentreOffset).z;
					break;
				}
			}
			else {
				selectedObject->m_position = cursorPlanePoint + objectCentreOffset;
			}
		}

		cursorPlanePoint = Picker::TranslatorPlaneIntersect(mousePos, chosenPlane, m_world, projection, view, winRect, deviceResources);

		if (firstRound) {
			objectCentreOffset = selectedObject->m_position - cursorPlanePoint;
		}
		
		firstRound = false;

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

	drawXY();
	drawYZ();
	drawZX();

	primitiveBatch->End();

	context->OMSetDepthStencilState(oldDepthStencilState.Get(), 0);
}

void ObjectEditor::updateObject(DisplayObject* object)
{
	selectedObject = object;


	boxList.clear();

	DirectX::BoundingBox boxX;
	DirectX::BoundingBox boxY;
	DirectX::BoundingBox boxZ;

	boxX.Center = selectedObject->m_position + DirectX::XMFLOAT3((translatorLength / 2), 0.f, 0.f);
	boxX.Extents = DirectX::XMFLOAT3(translatorLength / 2, cubeRadius, cubeRadius);

	boxList.push_back(boxX);

	boxY.Center = selectedObject->m_position + DirectX::XMFLOAT3(0.f, translatorLength / 2, 0.f);
	boxY.Extents = DirectX::XMFLOAT3(cubeRadius, translatorLength / 2, cubeRadius);

	boxList.push_back(boxY);

	boxZ.Center = selectedObject->m_position + DirectX::XMFLOAT3(0.f, 0.f, translatorLength / 2);
	boxZ.Extents = DirectX::XMFLOAT3(cubeRadius, cubeRadius, translatorLength / 2);

	boxList.push_back(boxZ);

	//plane movers

	DirectX::BoundingBox boxXY;
	DirectX::BoundingBox boxYZ;
	DirectX::BoundingBox boxZX;

	boxXY.Center = selectedObject->m_position + DirectX::XMFLOAT3(planeTranslatorLength / 2, planeTranslatorLength / 2, 0.f);
	boxXY.Extents = DirectX::XMFLOAT3(planeTranslatorLength / 2, planeTranslatorLength / 2, 0.1f);

	boxList.push_back(boxXY);

	boxYZ.Center = selectedObject->m_position + DirectX::XMFLOAT3(0.f, planeTranslatorLength / 2, planeTranslatorLength / 2);
	boxYZ.Extents = DirectX::XMFLOAT3(0.1f, planeTranslatorLength / 2, planeTranslatorLength / 2);

	boxList.push_back(boxYZ);

	boxZX.Center = selectedObject->m_position + DirectX::XMFLOAT3(planeTranslatorLength / 2, 0.f, planeTranslatorLength / 2);
	boxZX.Extents = DirectX::XMFLOAT3(planeTranslatorLength / 2, 0.1f, planeTranslatorLength / 2);

	boxList.push_back(boxZX);

}

DisplayObject* ObjectEditor::getDisplayObject()
{
	return selectedObject;
}

bool ObjectEditor::getTranslatorHovered()
{
	return translatorHovered;
}

void ObjectEditor::KeyDown(int key)
{
}

void ObjectEditor::KeyUp(int key)
{
}

void ObjectEditor::mouseDown(MouseInput mouse)
{
	RMBDown = true;

	//putting in negative, as some where the coords are flipped?? no clue maybe a future issue but this fixes it

	linearTransltion = false;

	switch (collidedTranslator) {
	case -1:
		return;
	case 0:
		linearTransltion = true;
		linearAxis = axisX;
	case 3:
		//xy plane
		chosenPlane = XMVectorSet(0.0f, 0.0f, 1.0f, -selectedObject->m_position.z);
		//chosenPlane = XMVectorSet(0.0f, 0.0f, 1.0f, 0);
		break;
	case 1:
		linearTransltion = true;
		linearAxis = axisY;
	case 4:
		//yz plane
		chosenPlane = XMVectorSet(1.0f, 0.0f, 0.0f, -selectedObject->m_position.x);
		//chosenPlane = XMVectorSet(1.0f, 0.0f, 0.0f, 0);
		break;
	case 2:
		linearTransltion = true;
		linearAxis = axisZ;
	case 5:
		//zx plane
		chosenPlane = XMVectorSet(0.0f, 1.0f, 0.0f, -selectedObject->m_position.y);
		//chosenPlane = XMVectorSet(0.0f, 1.0f, 0.0f, 0);
	}
}

void ObjectEditor::mouseUp(MouseInput mouse)
{
	RMBDown = false;
	firstRound = true;

	updateObject(selectedObject);
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
		DirectX::VertexPositionColor(selectedObject->m_position + Vector3{ 0.f, 0.f, translatorLength }, colour)
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

void ObjectEditor::drawXY()
{
	DirectX::XMVECTORF32 colour;

	if (collidedTranslator == 3) {
		colour = DirectX::Colors::White;
	}
	else {
		colour = DirectX::Colors::Orange;
	}

	primitiveBatch->DrawLine(
		DirectX::VertexPositionColor(selectedObject->m_position + Vector3{ planeTranslatorLength, 0.f, 0.f }, colour),
		DirectX::VertexPositionColor(selectedObject->m_position + Vector3{ planeTranslatorLength, planeTranslatorLength, 0.f }, colour)
	);

	primitiveBatch->DrawLine(
		DirectX::VertexPositionColor(selectedObject->m_position + Vector3{ 0.f, planeTranslatorLength, 0.f }, colour),
		DirectX::VertexPositionColor(selectedObject->m_position + Vector3{ planeTranslatorLength, planeTranslatorLength, 0.f }, colour)
	);
}

void ObjectEditor::drawYZ()
{
	DirectX::XMVECTORF32 colour;

	if (collidedTranslator == 4) {
		colour = DirectX::Colors::White;
	}
	else {
		colour = DirectX::Colors::Orange;
	}

	primitiveBatch->DrawLine(
		DirectX::VertexPositionColor(selectedObject->m_position + Vector3{ 0.f, planeTranslatorLength, 0.f }, colour),
		DirectX::VertexPositionColor(selectedObject->m_position + Vector3{ 0.f, planeTranslatorLength, planeTranslatorLength }, colour)
	);

	primitiveBatch->DrawLine(
		DirectX::VertexPositionColor(selectedObject->m_position + Vector3{ 0.f, 0.f, planeTranslatorLength }, colour),
		DirectX::VertexPositionColor(selectedObject->m_position + Vector3{ 0.f, planeTranslatorLength, planeTranslatorLength }, colour)
	);
}

void ObjectEditor::drawZX()
{
	DirectX::XMVECTORF32 colour;

	if (collidedTranslator == 5) {
		colour = DirectX::Colors::White;
	}
	else {
		colour = DirectX::Colors::Orange;
	}

	primitiveBatch->DrawLine(
		DirectX::VertexPositionColor(selectedObject->m_position + Vector3{ 0.f, 0.f, planeTranslatorLength }, colour),
		DirectX::VertexPositionColor(selectedObject->m_position + Vector3{ planeTranslatorLength, 0.f, planeTranslatorLength }, colour)
	);

	primitiveBatch->DrawLine(
		DirectX::VertexPositionColor(selectedObject->m_position + Vector3{ planeTranslatorLength, 0.f, 0.f }, colour),
		DirectX::VertexPositionColor(selectedObject->m_position + Vector3{ planeTranslatorLength, 0.f, planeTranslatorLength }, colour)
	);
}
