#pragma once
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
#include <SimpleMath.h>

class DisplayObject;

using namespace DirectX::SimpleMath;
using namespace DirectX;

struct ObjectDelta {
	DisplayObject* object;
	Vector3 translate;
	Vector3 rotate;
	Vector3 scale;
};

class HistoryManager
{
public:

	HistoryManager();
	~HistoryManager();

	void addHistory(std::vector<ObjectDelta> newHistory);
	std::vector<ObjectDelta> goBack();
	std::vector<ObjectDelta> goForward();

private:

	int positionFromFront = 0;

	//record deltas
	//vector(for each transform change) of vectors(for evrey object) of structs(object reference and scale, rot, trans vec3s)

	std::vector<std::vector<ObjectDelta>> history;
};

