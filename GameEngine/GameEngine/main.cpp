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

#include "stdafx.h"

#ifndef _LIB

void Close()
{
	PostQuitMessage(0);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	#ifdef _MSC_VER
	#ifdef _DEBUG
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF |  _CRTDBG_LEAK_CHECK_DF);
	#endif
	#endif

	Game = GetGameAPI(L"Editor", 800, 480, true);
	Game->Init();
	Game->GetWindow()->OnClose.RegisterHandler<Close>();
	Game->GetMainContainer()->SetEditable(false);
	Game->ShowFPS = false;
#ifdef USEBOX2DPHYSICS
	Game->m_DebugDraw = false;
#endif
	Game->DrawBoxes = false;

	Game->LoadFile(L"mainmenu.lvl");
	Game->GetMainContainer()->AdjustSize(*Game->GetDevice());
	Game->Unpause();

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while(msg.message!=WM_QUIT)
	{
		if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Game->Loop();
		}
	}

	delete Game;

	return msg.wParam;
}
#endif