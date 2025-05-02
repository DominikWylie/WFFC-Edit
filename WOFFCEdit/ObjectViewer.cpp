
#include "stdafx.h"
#include "ObjectViewer.h"
#include "DisplayObject.h"

IMPLEMENT_DYNAMIC(ObjectViewer, CDialogEx)

BEGIN_MESSAGE_MAP(ObjectViewer, CDialogEx)
ON_COMMAND(IDC_BXINCREASE, &ObjectViewer::OnBxincrease)
ON_COMMAND(IDC_BXDECREASE, &ObjectViewer::OnBxdecrease)
ON_COMMAND(IDC_BYDECREASE, &ObjectViewer::OnBydecrease)
ON_COMMAND(IDC_BYINCREASE, &ObjectViewer::OnByincrease)
ON_COMMAND(IDC_BZDECREASE, &ObjectViewer::OnBzdecrease)
ON_COMMAND(IDC_BZINCREASE, &ObjectViewer::OnBzincrease)
END_MESSAGE_MAP()

ObjectViewer::ObjectViewer(CWnd* pParent)
	: CDialogEx(IDD_DIALOG2, pParent)
{
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
}

void ObjectViewer::OnBxincrease()
{
	if (!masterObject) {
		return;
	}

	masterObject->m_position.x += positionIncriment;

	UpdateData(true);
}

void ObjectViewer::OnBxdecrease()
{
	if (!masterObject) {
		return;
	}

	masterObject->m_position.x -= positionIncriment;

	UpdateData(true);
}


void ObjectViewer::OnBydecrease()
{
	if (!masterObject) {
		return;
	}

	masterObject->m_position.y -= positionIncriment;

	UpdateData(true);
}


void ObjectViewer::OnByincrease()
{
	if (!masterObject) {
		return;
	}

	masterObject->m_position.y += positionIncriment;

	UpdateData(true);
}


void ObjectViewer::OnBzdecrease()
{
	if (!masterObject) {
		return;
	}

	masterObject->m_position.z -= positionIncriment;

	UpdateData(true);
}


void ObjectViewer::OnBzincrease()
{
	if (!masterObject) {
		return;
	}

	masterObject->m_position.z += positionIncriment;

	UpdateData(true);
}
