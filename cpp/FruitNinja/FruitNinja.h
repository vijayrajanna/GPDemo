
// FruitNinja.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CFruitNinjaApp:
// See FruitNinja.cpp for the implementation of this class
//

class CFruitNinjaApp : public CWinApp
{
public:
	CFruitNinjaApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CFruitNinjaApp theApp;