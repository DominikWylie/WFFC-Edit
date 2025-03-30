#include "MousePicker.h"
#include <vector>
#include "SimpleMath.h"

//#include "pch.h"

using namespace DirectX;

int MousePicker::Pick()
{

    float pickedDistance = 0;

    std::vector<int> selectedIDs;
    std::vector<float> pickedDistances;

    //setup near and far planes of frustum with mouse X and mouse y passed down from Toolmain. 
    //they may look the same but note, the difference in Z
    const XMVECTOR nearSource = XMVectorSet(mousePos.x, mousePos.y, 0.0f, 1.0f);
    const XMVECTOR farSource = XMVectorSet(mousePos.x, mousePos.y, 1.0f, 1.0f);

    //Loop through entire display list of objects and pick with each in turn. 
    for (int i = 0; i < m_displayList.size(); i++)
    {
        //Get the scale factor and translation of the object
        const XMVECTORF32 scale = { m_displayList[i].m_scale.x,		m_displayList[i].m_scale.y,		m_displayList[i].m_scale.z };
        const XMVECTORF32 translate = { m_displayList[i].m_position.x,	m_displayList[i].m_position.y,	m_displayList[i].m_position.z };

        //convert euler angles into a quaternion for the rotation of the object
        XMVECTOR rotate = Quaternion::CreateFromYawPitchRoll(m_displayList[i].m_orientation.y * 3.1415 / 180,
            m_displayList[i].m_orientation.x * 3.1415 / 180,
            m_displayList[i].m_orientation.z * 3.1415 / 180);

        //create set the matrix of the selected object in the world based on the translation, scale and rotation.
        XMMATRIX local = m_world * XMMatrixTransformation(g_XMZero, Quaternion::Identity, scale, g_XMZero, rotate, translate);

        //Unproject the points on the near and far plane, with respect to the matrix we just created.
        XMVECTOR nearPoint = XMVector3Unproject(nearSource, 0.0f, 0.0f, winRect.right, winRect.bottom, m_deviceResources->GetScreenViewport().MinDepth, m_deviceResources->GetScreenViewport().MaxDepth, m_projection, m_view, local);
        XMVECTOR farPoint = XMVector3Unproject(farSource, 0.0f, 0.0f, winRect.right, winRect.bottom, m_deviceResources->GetScreenViewport().MinDepth, m_deviceResources->GetScreenViewport().MaxDepth, m_projection, m_view, local);

        //turn the transformed points into our picking vector. 
        XMVECTOR pickingVector = farPoint - nearPoint;
        pickingVector = XMVector3Normalize(pickingVector);

        //loop through mesh list for object
        for (int y = 0; y < m_displayList[i].m_model.get()->meshes.size(); y++)
        {
            //checking for ray intersection
            if (m_displayList[i].m_model.get()->meshes[y]->boundingBox.Intersects(nearPoint, pickingVector, pickedDistance))
            {
                //selectedID = i;
                selectedIDs.push_back(i);
                pickedDistances.push_back(pickedDistance);

            }
        }
    }

    if (selectedIDs.empty()) {
        return -1;
    }

    auto cloasestDistanceIt = std::min_element(pickedDistances.begin(), pickedDistances.end());

    int indexOfCloasest = std::distance(pickedDistances.begin(), cloasestDistanceIt);

    //if we got a hit.  return it.  
    return selectedIDs.at(indexOfCloasest);
}
