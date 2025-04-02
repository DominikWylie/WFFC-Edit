#include "ObjectEditor.h"
#include "DisplayObject.h"
#include "DeviceResources.h"
#include "Picker.h"
#include <DirectXMath.h>


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

void ObjectEditor::DrawTranslators(
	Matrix view, 
	Matrix projection, 
	DirectX::SimpleMath::Matrix& m_world, 
	RECT winRect, 
	std::shared_ptr<DX::DeviceResources>& deviceResources)
{
	if (!objectSelected) {
		return;
	}

	mouseDelta = prevMousePos - mousePos;

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

			Vector3 oldMasterPosition = masterObject->m_position;

			if (lineTransltion) {
				switch (lineAxis) {
				case axisX:
					if (editState == translate) {
						masterObject->m_position.x = (cursorPlanePoint + objectCentreOffset).x;
					}
					else if (editState == rotate) {
						masterObject->m_orientation.x += mouseDelta.y;
					}
					else if (editState == scale) {
						masterObject->m_scale.x += mouseDelta.x * scaleMultiplier;
					}
					break;
				case axisY:
					if (editState == translate) {
						masterObject->m_position.y = (cursorPlanePoint + objectCentreOffset).y;
					}
					else if (editState == rotate) {
						masterObject->m_orientation.y += mouseDelta.x;
					}
					else if (editState == scale) {
						masterObject->m_scale.y += mouseDelta.y * scaleMultiplier;
					}
					break;
				case axisZ:
					if (editState == translate) {
						masterObject->m_position.z = (cursorPlanePoint + objectCentreOffset).z;
					}
					else if (editState == rotate) {
						masterObject->m_orientation.z += mouseDelta.y;
					}
					else if (editState == scale) {
						masterObject->m_scale.z += mouseDelta.x * scaleMultiplier;
					}
					break;
				}
			}
			else {
				masterObject->m_position = cursorPlanePoint + objectCentreOffset;
			}

			if (editState == translate) {
				//loop through the vector, setting all the selected objects to the master + the relative offset
				for (const auto object : selectedObjects) {
					//Vector3 offset = object.first->m_position - object.second;

					object.first->m_position = masterObject->m_position + object.second;
				}
			}
		}

		cursorPlanePoint = Picker::TranslatorPlaneIntersect(mousePos, chosenPlane, m_world, projection, view, winRect, deviceResources);

		if (firstRound) {
			objectCentreOffset = masterObject->m_position - cursorPlanePoint;
			firstRound = false;
		}
		
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

	prevMousePos = mousePos;
}

void ObjectEditor::updateObject(DisplayObject* object)
{
	//multi select
	//if shift is held just add to list, leave gizmo as first one
	//get relative locations of others to first one
	//and translate others relative to first one
	if (masterObject == object) {
		//skip this
	}
	else if (masterObject == nullptr || !shiftHeld) {
		masterObject = object;
		//multiSelect = false;
		setMultiselect(false);
	}
	else if (multiSelect) {
		selectedObjects.insert({object, object->m_position - masterObject->m_position});
		return;
	}
	else {
		//to remove the multiselect
		selectedObjects.clear();
		masterObject = object;
	}

	// if only 1, if more, leave it on the first
	boxList.clear();

	DirectX::BoundingBox boxX;
	DirectX::BoundingBox boxY;
	DirectX::BoundingBox boxZ;

	boxX.Center = masterObject->m_position + DirectX::XMFLOAT3((translatorLength / 2), 0.f, 0.f);
	boxX.Extents = DirectX::XMFLOAT3(translatorLength / 2, cubeRadius, cubeRadius);

	boxList.push_back(boxX);

	boxY.Center = masterObject->m_position + DirectX::XMFLOAT3(0.f, translatorLength / 2, 0.f);
	boxY.Extents = DirectX::XMFLOAT3(cubeRadius, translatorLength / 2, cubeRadius);

	boxList.push_back(boxY);

	boxZ.Center = masterObject->m_position + DirectX::XMFLOAT3(0.f, 0.f, translatorLength / 2);
	boxZ.Extents = DirectX::XMFLOAT3(cubeRadius, cubeRadius, translatorLength / 2);

	boxList.push_back(boxZ);

	//plane movers

	DirectX::BoundingBox boxXY;
	DirectX::BoundingBox boxYZ;
	DirectX::BoundingBox boxZX;

	boxXY.Center = masterObject->m_position + DirectX::XMFLOAT3(planeTranslatorLength / 2, planeTranslatorLength / 2, 0.f);
	boxXY.Extents = DirectX::XMFLOAT3(planeTranslatorLength / 2, planeTranslatorLength / 2, 0.1f);

	boxList.push_back(boxXY);

	boxYZ.Center = masterObject->m_position + DirectX::XMFLOAT3(0.f, planeTranslatorLength / 2, planeTranslatorLength / 2);
	boxYZ.Extents = DirectX::XMFLOAT3(0.1f, planeTranslatorLength / 2, planeTranslatorLength / 2);

	boxList.push_back(boxYZ);

	boxZX.Center = masterObject->m_position + DirectX::XMFLOAT3(planeTranslatorLength / 2, 0.f, planeTranslatorLength / 2);
	boxZX.Extents = DirectX::XMFLOAT3(planeTranslatorLength / 2, 0.1f, planeTranslatorLength / 2);

	boxList.push_back(boxZX);
}

DisplayObject* ObjectEditor::getDisplayObject()
{
	return masterObject;
}

bool ObjectEditor::getTranslatorHovered()
{
	return translatorHovered;
}

void ObjectEditor::setMultiselect(bool set)
{
	multiSelect = set;

	if (!set) {
		selectedObjects.clear();
	}
}

void ObjectEditor::KeyDown(int key)
{
	if(key == VK_SHIFT) {
		editState = scale;
		multiSelect = true;
		shiftHeld = true;
	}

	if (key == VK_CONTROL) {
		editState = rotate;
	}
}

void ObjectEditor::KeyUp(int key)
{
	if (key == VK_SHIFT || key == VK_CONTROL) {
		editState = translate;
	}

	if (key == VK_SHIFT) {
		shiftHeld = false;
	}
}

void ObjectEditor::mouseDown(MouseInput mouse)
{
	//currently not subscribed to these but a bit of safety
	if (mouse == RMB || mouse == MMB) {
		return;
	}

	RMBDown = true;

	lineTransltion = false;

	//0-2 is line translation
	//3-5 is plane translation

	switch (collidedTranslator) {
	case -1:
		return;
	case 0:
		//linear x
		lineTransltion = true;
		lineAxis = axisX;
	case 3:
		//xy plane
		chosenPlane = XMVectorSet(0.0f, 0.0f, 1.0f, -masterObject->m_position.z);
		break;
	case 1:
		//linear y
		lineTransltion = true;
		lineAxis = axisY;
	case 4:
		//yz plane
		chosenPlane = XMVectorSet(1.0f, 0.0f, 0.0f, -masterObject->m_position.x);
		break;
	case 2:
		//linear z
		lineTransltion = true;
		lineAxis = axisZ;
	case 5:
		//zx plane
		chosenPlane = XMVectorSet(0.0f, 1.0f, 0.0f, -masterObject->m_position.y);
	}
}

void ObjectEditor::mouseUp(MouseInput mouse)
{
	RMBDown = false;
	firstRound = true;

	updateObject(masterObject);
}

void ObjectEditor::scrollWheelMove(float wheel)
{
	if (editState == scale && objectSelected) {
		masterObject->m_scale += Vector3(wheel, wheel, wheel) * scaleMultiplier;
	}
}

void ObjectEditor::mousePosition(DirectX::SimpleMath::Vector2 mousePosition)
{
	prevMousePos = mousePos;
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

	DirectX::SimpleMath::Vector3 endPos = masterObject->m_position + Vector3{ translatorLength, 0.f, 0.f };

	//x
	primitiveBatch->DrawLine(
		DirectX::VertexPositionColor(masterObject->m_position, colour),
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

	DirectX::SimpleMath::Vector3 endPos = masterObject->m_position + Vector3{ 0.f, translatorLength, 0.f };

	//y
	primitiveBatch->DrawLine(
		DirectX::VertexPositionColor(masterObject->m_position, colour),
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
		DirectX::VertexPositionColor(masterObject->m_position, colour),
		DirectX::VertexPositionColor(masterObject->m_position + Vector3{ 0.f, 0.f, translatorLength }, colour)
	);

	//make cube - migh change so it overlaps the line
	//so end is the centre of the back quad

	DirectX::SimpleMath::Vector3 endPos = masterObject->m_position + Vector3{ 0.f, 0.f, translatorLength };

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
		DirectX::VertexPositionColor(masterObject->m_position + Vector3{ planeTranslatorLength, 0.f, 0.f }, colour),
		DirectX::VertexPositionColor(masterObject->m_position + Vector3{ planeTranslatorLength, planeTranslatorLength, 0.f }, colour)
	);

	primitiveBatch->DrawLine(
		DirectX::VertexPositionColor(masterObject->m_position + Vector3{ 0.f, planeTranslatorLength, 0.f }, colour),
		DirectX::VertexPositionColor(masterObject->m_position + Vector3{ planeTranslatorLength, planeTranslatorLength, 0.f }, colour)
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
		DirectX::VertexPositionColor(masterObject->m_position + Vector3{ 0.f, planeTranslatorLength, 0.f }, colour),
		DirectX::VertexPositionColor(masterObject->m_position + Vector3{ 0.f, planeTranslatorLength, planeTranslatorLength }, colour)
	);

	primitiveBatch->DrawLine(
		DirectX::VertexPositionColor(masterObject->m_position + Vector3{ 0.f, 0.f, planeTranslatorLength }, colour),
		DirectX::VertexPositionColor(masterObject->m_position + Vector3{ 0.f, planeTranslatorLength, planeTranslatorLength }, colour)
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
		DirectX::VertexPositionColor(masterObject->m_position + Vector3{ 0.f, 0.f, planeTranslatorLength }, colour),
		DirectX::VertexPositionColor(masterObject->m_position + Vector3{ planeTranslatorLength, 0.f, planeTranslatorLength }, colour)
	);

	primitiveBatch->DrawLine(
		DirectX::VertexPositionColor(masterObject->m_position + Vector3{ planeTranslatorLength, 0.f, 0.f }, colour),
		DirectX::VertexPositionColor(masterObject->m_position + Vector3{ planeTranslatorLength, 0.f, planeTranslatorLength }, colour)
	);
}
