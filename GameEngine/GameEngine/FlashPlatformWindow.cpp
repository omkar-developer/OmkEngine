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

C_Window::C_Window(const wchar_t* title, int width, int height, bool resize, bool caption)
{
	width = 0;
	height = 0;
}

C_Window::~C_Window()
{
	
}

PointI C_Window::GetPosition()
{
	//not implemented
	return PointI(0,0);
}

void C_Window::SetPosition(PointI pos)
{
	//not implemented
}
	
Size C_Window::GetSize()
{
	return Size((unsigned int)width, (unsigned int)height);
}

void C_Window::SetSize(Size size)
{
	//not implemented
}

wstring C_Window::GetTitle()
{
	//not implemented
	return L"";
}

void C_Window::SetTitle(wstring val)
{
	//not implemented
}

bool C_Window::GetVisible()
{
	//not implemented
	return true;
}

void C_Window::SetVisible(bool val)
{
	//not implemented
}

RectangleI C_Window::GetDrawArea()
{
	//not implemented
	return RectangleI(0,0,0,0);
}

bool C_Window::IsFocused()
{
	//not implemented
	return true;
}

void C_Window::SetFocus()
{
	//not implemented
}

bool C_Window::Close()
{
	//not implemented
	return true;
}

void C_Window::CaptureMouse()
{
	//not implemented
}

bool C_Window::ReleaseMouse()
{
	//not implemented
	return true;
}

void C_Window::Paint()
{
	//not implemented
}

void C_Window::Refresh()
{
	//not implemented
}

void C_Window::ShowCursor()
{
	//not implemented
}

void C_Window::HideCursor()
{
	//not implemented
}

PointI C_Window::GetCursorPos()
{
	//not implemented
	return PointI(0, 0);
}

void C_Window::SetCursorPos(PointI pt)
{
	//not implemented
}

bool C_Window::ClientToScreen(PointI& pt)
{
	//not implemented
}

bool C_Window::ScreenToClient(PointI& pt)
{
	//not implemented
}

bool C_Window::KeyState(int key)
{
	//not implemented
	return false;
}