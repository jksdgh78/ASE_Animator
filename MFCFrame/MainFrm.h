
// MainFrm.h : interface of the CMainFrame class
//

#pragma once

#include "CoolDialogbar.h"
#include "MainProcessDlg.h"
class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

	// Attributes
public:

	// Operations
public:

	// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

	CCoolDialogBar	m_MainProcessDlgBar;
	CMainProcessDlg m_MainProcessDlg;

	// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	/*afx_msg void OnViewMainProcess();
	afx_msg void OnUpdateViewMainProcess(CCmdUI* pCmdUI);*/
	DECLARE_MESSAGE_MAP()

};


