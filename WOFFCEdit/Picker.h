#pragma once
#include <vector>
#include <memory>
#include <windows.h>

namespace DirectX {
	namespace SimpleMath {
		class Vector2;
		class Matrix;
	}
}

namespace DX {
	class DeviceResources;
}

class DisplayObject;

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
};

