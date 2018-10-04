
// CG_AnimationView.h : interface of the CCG_AnimationView class
//

#pragma once

#include "GLInterface.h"
#include "CG_AnimationDoc.h"

class CCG_AnimationView : public CView
{
protected: // create from serialization only
	CCG_AnimationView();
	DECLARE_DYNCREATE(CCG_AnimationView)

protected:

	// opengl interface
	CGLInterface m_iGL;
	int m_nPrevTime;

	int Initialize(CDC *pDC);
	BOOL SetPixelFormat(PIXELFORMATDESCRIPTOR *pPFD = NULL);
	void Release();

	HGLRC m_hRC;
	CDC * m_pDC;
// Attributes
public:
	CCG_AnimationDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CCG_AnimationView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint pt);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint pt);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint pt);

	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void OnSaveASE(std::string sPath);
	void OnLoadASE(std::string sPath);
	void OnCapture(int nKeyFrame, std::string sAniName);
	void OnPlayPause()
	{
		m_iGL.OnPlayPause();
	}

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CG_AnimationView.cpp
inline CCG_AnimationDoc* CCG_AnimationView::GetDocument() const
   { return reinterpret_cast<CCG_AnimationDoc*>(m_pDocument); }
#endif

