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

Size C_Device::GetBackBufferSize()
{
	return Size(width, height);
}

void C_Device::SetBackBufferSize(Size size)
{
	width = size.width;
	height = size.height;
}

bool C_Device::GetFullscreen()
{
	//not implemented
	return true;
}

void C_Device::SetFullscreen(bool val)
{
	//not implemented
}

C_Window* C_Device::GetWindow()
{
	return win;
}

bool C_Device::GetGood()
{
	return good;
}

C_Device::C_Device(wchar_t* title, bool fullscreen)
{
	good = true;
	lost = false;
	width = 0;
	height = 0;
	win = new C_Window(L"", 0, 0);
	autorelease = true;
	OnDeviceCreated();
}

C_Device::C_Device(C_Window* win, bool fullscreen)
{
	this->win = win;
	good = true;
	lost = false;
	width = 0;
	height = 0;
	autorelease = false;
	OnDeviceCreated();
}

C_Device::~C_Device()
{
	if(good) OnDeviceRelease();
	if(autorelease && win!=0) delete win;
}

void C_Device::Clear(unsigned int color)
{
	ColorARGB32 col(color);
	glClearColor(col.GetR()/255.0f, col.GetG()/255.0f, col.GetB()/255.0f, col.GetA()/255.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void C_Device::StartDrawing()
{
	//not implemented
}

void C_Device::EndDrawing()
{
	//not implemented	
}

bool C_Device::Present()
{
	//not implemented
	return true;
}

bool C_Device::ResetDevice()
{
	OnDeviceReset();
	return true;
}

bool C_Device::CheckDevice()
{
	//not implemented
	return true;
}

void C_Device::SetView(const RectangleF& view)
{
	glViewport(view.left, view.top, view.right - view.left, view.bottom - view.top);
	viewtransform.left = view.left;
	viewtransform.top = view.top;
}

const PointI& C_Device::GetViewTransform() const
{
	return viewtransform;
}