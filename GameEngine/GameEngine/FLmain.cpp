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

#define SOUNDBUFFERSIZE 8192

using namespace std;
bool initiated = false;

float sounddata[SOUNDBUFFERSIZE];
void* sdataptr = 0;

int wwidth = 800;
int wheight = 480;

bool menabled = true;
bool senabled = true;

void PrintAVMLog(const string& str)
{
	inline_as3(
		"trace(CModule.readString(%0, %1));\n"
		:: "r" (str.c_str()), "r"(str.size())
		);
}

void OpenURL(string str)
{
	inline_as3(
		"com.adobe.flascc.Console.OpenUrl(CModule.readString(%0, %1));\n"
		:: "r" (str.c_str()), "r"(str.size())
		);
}

void Close()
{

}

int main(int argc, char **argv)
{
	sdataptr = &sounddata[0];
	memset(sdataptr, 0, sizeof(float) * SOUNDBUFFERSIZE);

	Game = GetGameAPI(L"Editor", 800, 480, true);
	
	Game->GetWindow()->OnClose.RegisterHandler<Close>();
	Game->GetMainContainer()->SetEditable(false);
#ifdef USEBOX2DPHYSICS
	Game->m_DebugDraw = false;
#endif
	Game->DrawBoxes = false;
	Game->ShowFPS = true;

	/*Game->GetDevice()->SetBackBufferSize(Size(800, 480));
	Game->LoadFile(L"mainmenu.lvl");*/
	
	AS3_GoAsync();
}

extern "C"
{
	int ackey = 0;
	int acx = 0;
	int acy = 0;

	void KeyDown()
	{
		if(Game)
		{
			Game->GetMainContainer()->KeyDown(ackey);
		}
	}

	void KeyUp()
	{
		if(Game)
		{
			Game->GetMainContainer()->KeyUp(ackey);
		}
	}

	void MouseDown()
	{
		if(Game)
		{
			Game->GetMainContainer()->MouseDown(KEY_LBUTTON, 0, PointI(acx, acy));
		}
	}

	void MouseUp()
	{
		if(Game)
		{
			Game->GetMainContainer()->MouseUp(KEY_LBUTTON, 0, PointI(acx, acy));
		}
	}

	void MouseMove()
	{
		if(Game)
		{
			Game->GetMainContainer()->MouseMove(PointI(acx, acy));
		}
	}

	void MainLoop()
	{	
		glEnable ( GL_COLOR_MATERIAL );
		if(!initiated)
		{
			Game->GetDevice()->SetBackBufferSize(Size(wwidth, wheight));
			Game->GetMainContainer()->AdjustSize(*Game->GetDevice());
			Game->Unpause();
			initiated = true;
		}
		else
		{
			Game->Loop();
		}
	}

	void WResized()
	{
		Game->GetDevice()->SetBackBufferSize(Size(wwidth, wheight));
		Game->GetMainContainer()->AdjustSize(*Game->GetDevice());
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, wwidth, wheight, 0, -10, 10);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	void WActivated()
	{
		if(Game!=0)
		{
			Game->EnableMusic(menabled);
			Game->EnableSound(senabled);
		}
	}

	void WDeactivated()
	{
		if(Game)
		{
			Game->Pause();
			menabled = Game->settings.GetMusic();
			senabled = Game->settings.m_ensound;
			Game->EnableMusic(false);
			Game->EnableSound(false);
		}
	}

	void UpdateSData()
	{
		if(Game->GetAllLoaded())
		m_mixer.GetData(sounddata, SOUNDBUFFERSIZE);
	}

	void ExitApp()
	{
		if(Game!=0) 
		{
			Game->QuitApp();
			//Game->ReleaseAll();
			delete Game;
			Game = 0;
		}
	}
}