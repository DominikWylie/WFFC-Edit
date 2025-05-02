#pragma once

#include "afxdialogex.h"
#include "resource.h"

class DisplayObject;

class ObjectViewer : public CDialogEx
{

	DECLARE_DYNAMIC(ObjectViewer)

public:

	ObjectViewer(CWnd* pParent = NULL);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBxincrease();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	void SetObjectData(DisplayObject* dObject);

	CStatic m_xTranslateLabel;
	CStatic m_yTranslateLabel;
	CStatic m_zTranslateLabel;

	DisplayObject* masterObject;

	int positionIncriment = 1;
	afx_msg void OnBxdecrease();
	afx_msg void OnBydecrease();
	afx_msg void OnByincrease();
	afx_msg void OnBzdecrease();
	afx_msg void OnBzincrease();
};

