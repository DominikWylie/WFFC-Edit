#pragma once

#include "afxdialogex.h"
#include "resource.h"

class ObjectViewer : public CDialogEx
{
public:
	DECLARE_MESSAGE_MAP()
//	afx_msg void test();
//	afx_msg void reetest(CCmdUI* pCmdUI);
	afx_msg void OnBxincrease();
	afx_msg void OnUpdateXtranslatetext(CCmdUI* pCmdUI);
};

