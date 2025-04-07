#pragma once

#include <afxext.h>
#include "pch.h"
#include "Game.h"
#include "sqlite3.h"
#include "SceneObject.h"
#include <vector>
#include "Input.h"
#include "Camera.h"
#include "ObjectEditor.h"
#include "CommandManager.h"
#include "HistoryManager.h"

class ToolMain : public InputObserver
{
public: //methods
	ToolMain();
	~ToolMain();

	//onAction - These are the interface to MFC
	int		getCurrentSelectionID();										//returns the selection number of currently selected object so that It can be displayed.
	void	onActionInitialise(HWND handle, int width, int height);			//Passes through handle and hieght and width and initialises DirectX renderer and SQL LITE
	void	onActionFocusCamera();
	void	onActionLoad();													//load the current chunk
	afx_msg	void	onActionSave();											//save the current chunk
	afx_msg void	onActionSaveTerrain();									//save chunk geometry

	void	Tick(MSG *msg);
	void	UpdateInput(MSG *msg);

	virtual void KeyDown(int key) override;
	virtual void KeyUp(int key) override;
	virtual void mouseDown(MouseInput mouse) override;
	virtual void mouseUp(MouseInput mouse) override;
	virtual void scrollWheelMove(float wheel) override {};
	virtual void mousePosition(DirectX::SimpleMath::Vector2 mousePosition) override;

	virtual std::vector<int> getKeysToObserve() override { return{VK_SHIFT}; };
	virtual std::vector<MouseInput> getMouseInputsToObserve() override { return{ LMB }; };
	virtual bool getScrollWheelToObserve() override { return false; };
	virtual bool getMousePositionToObserve() override { return true; };

public:	//variables
	std::vector<SceneObject>    m_sceneGraph;	//our scenegraph storing all the objects in the current chunk
	ChunkObject					m_chunk;		//our landscape chunk
	int m_selectedObject = -1;

	std::vector<int> m_selectedObjects;
	bool m_multiSelect = false;

	Camera camera;
	ObjectEditor objectEditor;
	
	//ID of current Selection

private:	//methods
	void	onContentAdded();
		
private:	//variables
	HWND	m_toolHandle;		//Handle to the  window
	Game	m_d3dRenderer;		//Instance of D3D rendering system for our tool
	CRect	WindowRECT;		//Window area rectangle. 
	char	m_keyArray[256];
	sqlite3 *m_databaseConnection;	//sqldatabase handle

	Input input;
	CommandManager commandManager;
	HistoryManager historyManager;

	int m_width;		//dimensions passed to directX
	int m_height;
	int m_currentChunk;			//the current chunk of thedatabase that we are operating on.  Dictates loading and saving. 
	
	RECT windowRect;
	
};
