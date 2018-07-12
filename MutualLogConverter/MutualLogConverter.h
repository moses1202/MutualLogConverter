
// MutualLogConverter.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#include "MutualLogConverterDlg.h"

// CMutualLogConverterApp:
// See MutualLogConverter.cpp for the implementation of this class
//

class CMutualLogConverterApp : public CWinApp
{
public:
	CMutualLogConverterApp();
	~CMutualLogConverterApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	CMutualLogConverterDlg *dlg;

	DECLARE_MESSAGE_MAP()
};

extern CMutualLogConverterApp theApp;