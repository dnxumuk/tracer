
// viewer.h : main header file for the viewer application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CviewerApp:
// See viewer.cpp for the implementation of this class
//

class CviewerApp : public CWinAppEx
{
public:
	CviewerApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CviewerApp theApp;
