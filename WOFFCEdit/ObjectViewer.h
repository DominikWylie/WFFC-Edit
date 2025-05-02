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
//	afx_msg void test();
//	afx_msg void reetest(CCmdUI* pCmdUI);
	afx_msg void OnBxincrease();
	afx_msg void OnUpdateXtranslatetext(CCmdUI* pCmdUI);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	void SetObjectData(DisplayObject* dObject);

	void UpdateXtranslatetext(CCmdUI* pCmdUI);

	CStatic m_xTranslateLabel;
	CStatic m_yTranslateLabel;
	CStatic m_zTranslateLabel;

	DisplayObject* masterObject;
};

