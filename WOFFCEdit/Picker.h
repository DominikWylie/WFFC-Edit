#pragma once
#include <vector>
#include <memory>
#include <windows.h>
#include "d3d11.h"
#include "SimpleMath.h"

//namespace DirectX {
//	namespace SimpleMath {
//		class Vector2;
//		class Matrix;
//		//struct BoundingBox;
//	}
//}

namespace DX {
	class DeviceResources;
}

class DisplayObject;

enum Axes {
	xAxis,
	yAxis,
	zAxis
};

class Picker
{
public:
	static int MousePick(
		DirectX::SimpleMath::Vector2& mousePos,
		std::vector<DisplayObject>& displayList,
		DirectX::SimpleMath::Matrix& m_world,
		DirectX::SimpleMath::Matrix& m_projection,
		DirectX::SimpleMath::Matrix& m_view,
		RECT& winRect,
		std::shared_ptr<DX::DeviceResources>& m_deviceResources
	);

	static int MousePick(
		DirectX::SimpleMath::Vector2& mousePos,
		std::vector<DirectX::BoundingBox>& boxList,
		DirectX::SimpleMath::Matrix& m_world,
		DirectX::SimpleMath::Matrix& m_projection,
		DirectX::SimpleMath::Matrix& m_view,
		RECT& winRect,
		std::shared_ptr<DX::DeviceResources>& m_deviceResources
	);

	static DirectX::SimpleMath::Vector3 TranslatorPlaneIntersect(
		DirectX::SimpleMath::Vector2& mousePos,
		DirectX::XMVECTOR plane,
		DirectX::SimpleMath::Matrix& m_world,
		DirectX::SimpleMath::Matrix& m_projection,
		DirectX::SimpleMath::Matrix& m_view,
		RECT& winRect,
		std::shared_ptr<DX::DeviceResources>& m_deviceResources
	);
};

