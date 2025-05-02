
#include "stdafx.h"
#include "ObjectViewer.h"
#include "DisplayObject.h"

IMPLEMENT_DYNAMIC(ObjectViewer, CDialogEx)

BEGIN_MESSAGE_MAP(ObjectViewer, CDialogEx)
//	ON_COMMAND(ID_BUTTON40001, &ObjectViewer::test)
//	ON_UPDATE_COMMAND_UI(ID_BUTTON40001, &ObjectViewer::reetest)
ON_COMMAND(IDC_BXINCREASE, &ObjectViewer::OnBxincrease)
ON_UPDATE_COMMAND_UI(IDC_XTRANSLATETEXT, &ObjectViewer::OnUpdateXtranslatetext)
END_MESSAGE_MAP()

ObjectViewer::ObjectViewer(CWnd* pParent)
	: CDialogEx(IDD_DIALOG2, pParent)
{
}

void ObjectViewer::OnBxincrease()
{
	// TODO: Add your command handler code here
}


void ObjectViewer::OnUpdateXtranslatetext(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void ObjectViewer::DoDataExchange(CDataExchange* pDX)
{
	if (!masterObject) {
		return;
	}

	CString posXText;
	CString posYText;
	CString posZText;

	posXText.Format(_T("%.2f"), masterObject->m_position.x);
	posYText.Format(_T("%.2f"), masterObject->m_position.y);
	posZText.Format(_T("%.2f"), masterObject->m_position.z);
	
	CDialogEx::DoDataExchange(pDX);

	//CStatic m_xTranslateLabel;
	DDX_Control(pDX, IDC_XTRANSLATETEXT, m_xTranslateLabel);
	m_xTranslateLabel.SetWindowText(posXText);

	DDX_Control(pDX, IDC_YTRANSLATETEXT, m_yTranslateLabel);
	m_yTranslateLabel.SetWindowText(posYText);
	
	DDX_Control(pDX, IDC_ZTRANSLATETEXT, m_zTranslateLabel);
	m_zTranslateLabel.SetWindowText(posZText);
}

void ObjectViewer::SetObjectData(DisplayObject* dObject)
{
	masterObject = dObject;


	//m_xTranslateLabel.SetWindowText(_T("New X Translate Value"));
}

void ObjectViewer::UpdateXtranslatetext(CCmdUI* pCmdUI)
{
	//m_xTranslateLabel.SetWindowText(_T("New X Translate Value"));

	//CStatic m_xTranslateLabel;

	//DDX_Control(pDX, IDC_XTRANSLATE_STATIC, m_xTranslateLabel);

	//CWnd* pWnd = GetDlgItem(IDC_XTRANSLATE_STATIC);
	//if (pWnd)
	//	pWnd->SetWindowText(_T("New X Translate Value"));

	//OnUpdateXtranslatetext(pCmdUI);

}
