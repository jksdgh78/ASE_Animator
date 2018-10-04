
// CG_Animation.h : main header file for the CG_Animation application
//
#pragma once

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CCG_AnimationApp:
// See CG_Animation.cpp for the implementation of this class
//

class CCG_AnimationApp : public CWinApp
{
public:
	CCG_AnimationApp();


	// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCG_AnimationApp theApp;
