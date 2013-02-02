/*
 * Copyright (c) 2013 Omkar Kanase
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include "wx/wxprec.h"
 
#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#include "resource.h"

#include "CDialogs.h"

class MainApp: public wxApp // MainApp is the class for our application 
{
	// MainApp just acts as a container for the window, or frame in MainFrame
public:
	virtual bool OnInit(); 
};
 

DECLARE_APP(MainApp)

IMPLEMENT_APP(MainApp) // A macro that tells wxWidgets to create an instance of our application 
 

bool MainApp::OnInit() 
{
#ifdef _MSC_VER
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF |  _CRTDBG_LEAK_CHECK_DF);
#endif
#endif
	// Create an instance of our frame, or window 
	Dialog *MainWin = new Dialog(0);
#ifdef __WXMSW__
	MainWin->SetIcon(wxIcon("IDI_ICON", wxBITMAP_TYPE_ICO_RESOURCE));
#endif
	MainWin->Show(true); // show the window 	
	SetTopWindow(MainWin); // and finally, set it as the main window 
	return true;
} 
