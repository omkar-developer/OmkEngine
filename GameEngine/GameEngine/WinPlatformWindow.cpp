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

LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	C_Window* win = (C_Window*)((void*)GetWindowLongPtr(wnd, GWLP_USERDATA));
	//if(win!=0 && win->m_oldp != 0)
	//{
	//	win->m_oldp(wnd, msg, wParam, lParam);
	//}

	if(msg == WM_SIZE)
	{
		win->OnResize();
	}
	else if(msg ==  WM_ACTIVATE)
	{
		if(LOWORD(wParam)) win->OnFocus();
		if(!LOWORD(wParam)) win->OnLostFocus();
	}
	else if(msg ==  WM_PAINT)
	{
		win->OnPaint();
	}
	else if(msg ==  WM_CLOSE || msg ==  WM_DESTROY)
	{
		win->OnClose();
	}
	else if(msg ==  WM_SHOWWINDOW)
	{
		win->OnVisible();
	}
	else if(msg ==  WM_LBUTTONDOWN || msg ==  WM_RBUTTONDOWN || msg ==  WM_MBUTTONDOWN || msg ==  WM_XBUTTONDOWN)
	{
		unsigned int button = 0;
		switch(msg)
		{
		case WM_LBUTTONDOWN:
			button = KEY_LBUTTON;
			break;
		case WM_RBUTTONDOWN:
			button = KEY_RBUTTON;
			break;
		case WM_MBUTTONDOWN:
			button = KEY_MBUTTON;
			break;
		case WM_XBUTTONDOWN:
			if(HIWORD(wParam) == XBUTTON1)
			{
				button = KEY_XBUTTON1;
			}
			else if(HIWORD(wParam) == XBUTTON2)
			{
				button = KEY_XBUTTON2;
			}
			break;
		}
		win->MouseDown(button, LOWORD(wParam), PointI(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
	}
	else if(msg ==  WM_LBUTTONUP || msg ==  WM_RBUTTONUP || msg ==  WM_MBUTTONUP || msg ==  WM_XBUTTONUP)
	{
		unsigned int button = 0;
		switch(msg)
		{
		case WM_LBUTTONUP:
			button = KEY_LBUTTON;
			break;
		case WM_RBUTTONUP:
			button = KEY_RBUTTON;
			break;
		case WM_MBUTTONUP:
			button = KEY_MBUTTON;
			break;
		case WM_XBUTTONUP:
			if(HIWORD(wParam) == XBUTTON1)
			{
				button = KEY_XBUTTON1;
			}
			else if(HIWORD(wParam) == XBUTTON2)
			{
				button = KEY_XBUTTON2;
			}
			break;
		}
		win->MouseUp(button, LOWORD(wParam), PointI(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
	}
	else if(msg ==  WM_LBUTTONDBLCLK || msg ==  WM_RBUTTONDBLCLK || msg ==  WM_MBUTTONDBLCLK || msg ==  WM_XBUTTONDBLCLK)
	{
		unsigned int button = 0;
		switch(msg)
		{
		case WM_LBUTTONDBLCLK:
			button = KEY_LBUTTON;
			break;
		case WM_RBUTTONDBLCLK:
			button = KEY_RBUTTON;
			break;
		case WM_MBUTTONDBLCLK:
			button = KEY_MBUTTON;
			break;
		case WM_XBUTTONDBLCLK:
			if(HIWORD(wParam) == XBUTTON1)
			{
				button = KEY_XBUTTON1;
			}
			else if(HIWORD(wParam) == XBUTTON2)
			{
				button = KEY_XBUTTON2;
			}
			break;
		}
		win->MouseDblClick(button, LOWORD(wParam), PointI(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
	}
	else if(msg ==  WM_MOUSEMOVE)
	{
		win->MouseMove(PointI(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
	}
	else if(msg ==  WM_MOUSEWHEEL)
	{
		win->MouseWheel( short(HIWORD(wParam)) / WHEEL_DELTA, LOWORD(wParam), PointI(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
	}
	else if(msg ==  WM_KEYDOWN)
	{
		win->KeyDown(wParam);
		if(isascii(wParam) && wParam != VK_RETURN && wParam != VK_DELETE && wParam != VK_CLEAR && wParam != VK_TAB)
		{
			win->KeyChar(static_cast<wchar_t>(wParam));
		}
	}
	else if(msg ==  WM_KEYUP)
	{
		win->KeyUp(wParam);
	}
	else if(msg ==  WM_CHAR)
	{
		win->KeyChar(static_cast<wchar_t>(wParam));
	}

	return DefWindowProc(wnd, msg, wParam, lParam);
}

C_Window::C_Window(HWND handle)
{
	autoclose = false;
	m_handle = handle;
	autoclose = false;
	/*m_oldp = (WNDPROC) GetWindowLongPtr(m_handle, GWLP_WNDPROC);*/
	/*SetWindowLongPtr(m_handle, GWLP_USERDATA, (LONG) this);
	SetWindowLongPtr(m_handle, GWLP_WNDPROC, (LONG) WndProc);*/
	return;
}

C_Window::C_Window(const wchar_t* title, int width, int height, bool resize, bool caption)
{
	/*m_oldp = 0;*/
	autoclose = true;
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = CreateSolidBrush(RGB(0,0,0));
	wc.hInstance = GetModuleHandle(0);
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = title;
	wc.style = CS_DBLCLKS;

	RegisterClassEx(&wc);
	int ws = (caption ? WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | (resize ? WS_THICKFRAME : 0) : WS_POPUP) | WS_SYSMENU;
	RECT sz;
	sz.left = 0;
	sz.top = 0;
	sz.right = width;
	sz.bottom = height;
	AdjustWindowRect(&sz, ws, false);
	m_handle = CreateWindow(title, title,  ws, CW_USEDEFAULT, CW_USEDEFAULT, sz.right - sz.left, sz.bottom - sz.top, 0, 0, GetModuleHandle(0), 0);
	SetWindowLongPtr(m_handle, GWLP_USERDATA, (LONG) this);
}

C_Window::~C_Window()
{
	if(autoclose) DestroyWindow(m_handle);
}

HWND C_Window::GetHandle()
{
	return m_handle;
}

PointI C_Window::GetPosition()
{
	RECT rect;
	if(GetWindowRect(m_handle, &rect))
	{
		return PointI(rect.left, rect.top);
	}
	return PointI(0,0);
}

void C_Window::SetPosition(PointI pos)
{
	SetWindowPos(m_handle, HWND_TOP, pos.left, pos.top, 0, 0, SWP_NOSIZE);
}
	
Size C_Window::GetSize()
{
	RECT rect;
	if(GetWindowRect(m_handle, &rect))
	{
		return Size(rect.right - rect.left, rect.bottom - rect.top);
	}
	return Size(0,0);
}

void C_Window::SetSize(Size size)
{
	SetWindowPos(m_handle, HWND_TOP, 0, 0, size.width, size.height, SWP_NOMOVE);
}

wstring C_Window::GetTitle()
{
	wchar_t tmp[256];
	GetWindowText(m_handle, tmp, 256);
	return tmp;
}

void C_Window::SetTitle(wstring val)
{
	SetWindowText(m_handle, val.c_str());
}

bool C_Window::GetVisible()
{
	if(IsWindowVisible(m_handle)) return true;
	return false;
}

void C_Window::SetVisible(bool val)
{
	if(val) ShowWindow(m_handle, SW_SHOW);
	else ShowWindow(m_handle, SW_HIDE);
}

RectangleI C_Window::GetDrawArea()
{
	RECT rect;
	if(GetClientRect(m_handle, &rect))
	{
		return RectangleI(rect.left, rect.top, rect.right, rect.bottom);
	}
	return RectangleI(0,0,0,0);
}

bool C_Window::IsFocused()
{
	if(GetFocus() == m_handle) return true;
	return false;
}

void C_Window::SetFocus()
{
	::SetFocus(m_handle);
}

bool C_Window::Close()
{
	if(DestroyWindow(m_handle)) return true;
	return false;
}

void C_Window::CaptureMouse()
{
	SetCapture(m_handle);
}

bool C_Window::ReleaseMouse()
{
	if(ReleaseCapture()) return true;
	return false;
}

void C_Window::Paint()
{
	InvalidateRect(m_handle, 0, TRUE);
}

void C_Window::Refresh()
{
	UpdateWindow(m_handle);
}

void C_Window::ShowCursor()
{
	HCURSOR cur = SetCursor(NULL);
	if(cur != NULL) DestroyCursor(cur);
}

void C_Window::HideCursor()
{
	HCURSOR cur = SetCursor(LoadCursor(0, IDC_ARROW));
	if(cur != NULL) DestroyCursor(cur);
}

PointI C_Window::GetCursorPos()
{
	POINT pt;
	if(::GetCursorPos(&pt))
	{
		return PointI(pt.x, pt.y);
	}
	return PointI(0, 0);
}

void C_Window::SetCursorPos(PointI pt)
{
	::SetCursorPos(pt.left, pt.top);
}

bool C_Window::ClientToScreen(PointI& pt)
{
	POINT p;
	p.x = pt.left;
	p.y = pt.top;
	BOOL flag = ::ClientToScreen(m_handle, &p);
	pt.left = p.x;
	pt.top = p.y;
	if(flag) return true;
	return false;
}

bool C_Window::ScreenToClient(PointI& pt)
{
	POINT p;
	p.x = pt.left;
	p.y = pt.top;
	BOOL flag = ::ScreenToClient(m_handle, &p);
	pt.left = p.x;
	pt.top = p.y;
	if(flag) return true;
	return false;
}

bool C_Window::KeyState(int key)
{
	if(GetKeyState(key) & 0x8000) return true;
	return false;
}