
// CG_AnimationView.cpp : implementation of the CCG_AnimationView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "CG_Animation.h"
#endif

#include "CG_AnimationDoc.h"
#include "CG_AnimationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCG_AnimationView

IMPLEMENT_DYNCREATE(CCG_AnimationView, CView)

BEGIN_MESSAGE_MAP(CCG_AnimationView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CCG_AnimationView construction/destruction

CCG_AnimationView::CCG_AnimationView()
{
	// TODO: add construction code here
	m_hRC = NULL;
	m_pDC = NULL;
	m_nPrevTime = GetTickCount();
}

CCG_AnimationView::~CCG_AnimationView()
{
}

BOOL CCG_AnimationView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CCG_AnimationView drawing

void CCG_AnimationView::OnDraw(CDC* /*pDC*/)
{
	CCG_AnimationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	m_iGL.Display();
}


// CCG_AnimationView printing

BOOL CCG_AnimationView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCG_AnimationView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCG_AnimationView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CCG_AnimationView diagnostics

#ifdef _DEBUG
void CCG_AnimationView::AssertValid() const
{
	CView::AssertValid();
}

void CCG_AnimationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCG_AnimationDoc* CCG_AnimationView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCG_AnimationDoc)));
	return (CCG_AnimationDoc*)m_pDocument;
}
#endif //_DEBUG


// CCG_AnimationView message handlers

void CCG_AnimationView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	m_iGL.SetProjectView(cx, cy);
}

BOOL CCG_AnimationView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	//return CView::OnEraseBkgnd(pDC);
	return false;
}


int CCG_AnimationView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	return Initialize(new CClientDC(this));
}

void CCG_AnimationView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
	Release();
}


int CCG_AnimationView::Initialize(CDC *pDC)
{
	if (m_pDC || m_hRC) {
		::AfxMessageBox(_T("Already initialized"));
		return -1;
	}
	if (!(m_pDC = pDC)) {
		::AfxMessageBox(_T("Fail to get device context"));
		return -1;
	}

	if (!SetPixelFormat(NULL)) {	// Pixel format 설정
		::AfxMessageBox(_T("SetupPixelFormat failed"));
		return -1;
	}
	if (!(m_hRC = wglCreateContext(m_pDC->GetSafeHdc()))) {	// RC 획득
		::AfxMessageBox(_T("wglCreateContext failed"));
		return -1;
	}
	if (!wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC)) {// Current RC 설정
		::AfxMessageBox(_T("wglMakeCurrent failed"));
		return -1;
	}

	m_iGL.Initialize();
	SetTimer(1394, 30, NULL);
	return 0;	// -1: Fail | 0: Well done
}

void CCG_AnimationView::Release(void)
{
	if (!wglMakeCurrent(0, 0))
		::AfxMessageBox(_T("wglMakeCurrent failed"));

	if (m_hRC && !wglDeleteContext(m_hRC))
		::AfxMessageBox(_T("wglDeleteContext filed"));

	if (m_pDC)
		delete m_pDC;

	m_hRC = NULL;	// Must be NULL
	m_pDC = NULL;	// Must be NULL
}

BOOL CCG_AnimationView::SetPixelFormat(PIXELFORMATDESCRIPTOR * pPFD)
{
	if (!pPFD) {
		PIXELFORMATDESCRIPTOR pfd = {
			sizeof(PIXELFORMATDESCRIPTOR),
			1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL,
			PFD_TYPE_RGBA, 24, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 16, 0, 0,
			PFD_MAIN_PLANE, 0, 0, 0, 0 };
		pPFD = &pfd;
	}

	int nPF = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), pPFD);
	if (!nPF) {
		::AfxMessageBox(_T("ChoosePixelFormat failed"));
		return FALSE;
	}

	if (!::SetPixelFormat(m_pDC->GetSafeHdc(), nPF, pPFD)) {
		::AfxMessageBox(_T("SetPixelFormat failed"));
		return FALSE;
	}

	return TRUE;
}



void CCG_AnimationView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	// check value for timer interrup
	int timerSet = 0;

	m_iGL.OnKeyDown(nChar);

	// invalidate client area for refresh. 모든 클라이언트 영역을 무효화 한다. 새로 그려줌 
	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CCG_AnimationView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	m_iGL.OnMouseWheel(zDelta);

	CView::OnMouseWheel(nFlags, zDelta, pt);
	return TRUE;
}

void CCG_AnimationView::OnMouseMove(UINT nFlags, CPoint pt)
{
	float fPt[2];
	fPt[0] = pt.y;
	fPt[1] = pt.x;

	m_iGL.OnMouseMove(nFlags, fPt);
}

void CCG_AnimationView::OnLButtonUp(UINT nFlags, CPoint pt)
{
	float fPt[2];
	fPt[0] = pt.y;
	fPt[1] = pt.x;

	m_iGL.OnLButtonUp(nFlags, fPt);
	CView::OnLButtonUp(nFlags, pt);
}

void CCG_AnimationView::OnLButtonDown(UINT nFlags, CPoint pt)
{
	float fPt[2];
	fPt[0] = pt.y;
	fPt[1] = pt.x;

	m_iGL.OnLButtonDown(nFlags, fPt);
	CView::OnLButtonDown(nFlags, pt);
}

void CCG_AnimationView::OnRButtonUp(UINT nFlags, CPoint pt)
{
	float fPt[2];
	fPt[0] = pt.y;
	fPt[1] = pt.x;

	m_iGL.OnRButtonUp(nFlags, fPt);
	CView::OnRButtonUp(nFlags, pt);
}

void CCG_AnimationView::OnRButtonDown(UINT nFlags, CPoint pt)
{
	float fPt[2];
	fPt[0] = pt.y;
	fPt[1] = pt.x;

	m_iGL.OnRButtonDown(nFlags, fPt);
	CView::OnRButtonDown(nFlags, pt);
}

void CCG_AnimationView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent)
	{
	case 1394:
		Invalidate();
		break;
	}

	int t = GetTickCount();
	int n = t - m_nPrevTime;
	m_nPrevTime = t;

	m_iGL.OnTimer(n);
	CView::OnTimer(nIDEvent);
}

void CCG_AnimationView::OnSaveASE(std::string sPath)
{
	m_iGL.OnSave(sPath);
}

void CCG_AnimationView::OnLoadASE(std::string sPath)
{
	m_iGL.OnLoadASE(sPath);
}

void CCG_AnimationView::OnCapture(int nKeyFrame, std::string sAniName)
{
	m_iGL.OnCapture(nKeyFrame, sAniName);
}
