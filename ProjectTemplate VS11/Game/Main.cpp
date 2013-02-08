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

#include "resource.h"

#include "CDialogs.h"

IMPLEMENT_APP(MainApp)

bool MainApp::OnInit() 
{
#ifdef _MSC_VER
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF |  _CRTDBG_LEAK_CHECK_DF);
#endif
#endif
	loop = true;
	// Create an instance of our frame, or window 
	Dialog *MainWin = new Dialog(0);
	Connect( wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(MainApp::onIdle) );
#ifdef __WXMSW__
	MainWin->SetIcon(wxIcon("IDI_ICON", wxBITMAP_TYPE_ICO_RESOURCE));
#endif
	MainWin->Show(true); // show the window 
	MainWin->app = this;
	SetTopWindow(MainWin); // and finally, set it as the main window 
	return true;
} 

void MainApp::DisconnectEvt()
{
	Disconnect( wxEVT_IDLE, wxIdleEventHandler(MainApp::onIdle) );
}

void MainApp::onIdle(wxIdleEvent& evt)
{
	if(!loop) return;
	Dialog* w = (Dialog*)GetTopWindow();
	evt.RequestMore();
	if(w==0) return;

	Game->Loop();
}