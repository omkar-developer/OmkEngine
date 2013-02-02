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
	if(device==0) return Size(0,0);
	IDirect3DSurface9* bb = 0;
	device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &bb);
	if(bb==0) return Size(0,0);
	D3DSURFACE_DESC ds;
	bb->GetDesc(&ds);
	bb->Release();
	return Size(ds.Width, ds.Height);
}

void C_Device::SetBackBufferSize(Size size)
{
	if(good && d3dpp.Windowed == FALSE)
	{
		d3dpp.BackBufferWidth = size.width;
		d3dpp.BackBufferHeight = size.height;
		ResetDevice();
	}
}

bool C_Device::GetFullscreen()
{
	if(d3dpp.Windowed) return false;
	return true;
}

void C_Device::SetFullscreen(bool val)
{
	if(good)
	{
		if(val) 
		{
			D3DDISPLAYMODE mode;

			d3d9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode);

			ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
			d3dpp.BackBufferFormat = mode.Format;
			d3dpp.BackBufferHeight = mode.Height;
			d3dpp.BackBufferWidth = mode.Width;
			d3dpp.FullScreen_RefreshRateInHz = mode.RefreshRate;
			d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
			d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
			d3dpp.EnableAutoDepthStencil = 1;
			d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
			d3dpp.Windowed = FALSE;
			LONG old = 0;
			old = GetWindowLongPtr(win->GetHandle(), GWL_STYLE);
			old &= ~WS_SIZEBOX;
			old &= ~WS_CAPTION;
			SetWindowLongPtr(win->GetHandle(), GWL_STYLE, old);
	
		}
		else
		{
			d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
			d3dpp.EnableAutoDepthStencil = 1;
			d3dpp.Windowed = 1;
			d3dpp.FullScreen_RefreshRateInHz = 0;
			d3dpp.BackBufferHeight = 0;
			d3dpp.BackBufferWidth = 0;

			SetWindowLongPtr(win->GetHandle(), GWL_STYLE, oldstyle);
			SetWindowPos(win->GetHandle(), 0, oldrect.left, oldrect.top, oldrect.right - oldrect.left, oldrect.bottom - oldrect.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOOWNERZORDER);
		}
		ResetDevice();
		device->SetRenderState(D3DRS_ZENABLE, TRUE);
	}
}

IDirect3DDevice9* C_Device::GetDevice()
{
	return device;
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
	d3d9 = 0;
	device = 0;
	good = false;
	lost = false;
	win = 0;
	autorelease = true;
	oldstyle = 0;

	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	if(d3d9==0)
	{
		return;
	}

	D3DDISPLAYMODE mode;

	d3d9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode);

	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	d3dpp.BackBufferFormat = mode.Format;
	d3dpp.BackBufferHeight = mode.Height;
	d3dpp.BackBufferWidth = mode.Width;
	d3dpp.FullScreen_RefreshRateInHz = mode.RefreshRate;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.EnableAutoDepthStencil = 1;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	d3dpp.Windowed = 0;

	if(fullscreen != true)
	{
		d3dpp.Windowed = 1;
		d3dpp.FullScreen_RefreshRateInHz = 0;
		d3dpp.BackBufferHeight = 0;
		d3dpp.BackBufferWidth = 0;
		win = new C_Window(title, 640, 480);
	}
	else
	{
		win = new C_Window(title, mode.Width, mode.Height, false, false);
	}

	oldstyle = GetWindowLongPtr(win->GetHandle(), GWL_STYLE);
	GetWindowRect(win->GetHandle(), &oldrect);

	if(fullscreen)
	{
		LONG old = 0;
		old = GetWindowLongPtr(win->GetHandle(), GWL_STYLE);
		old &= ~WS_SIZEBOX;
		old &= ~WS_CAPTION;
		SetWindowLongPtr(win->GetHandle(), GWL_STYLE, old);
	}

	win->SetVisible(true);
	d3dpp.hDeviceWindow = win->GetHandle();

	HRESULT hres = d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &d3dpp, &device);	
	if(hres!=D3D_OK)
	{
		hres = d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &d3dpp, &device);
		if(hres!=D3D_OK)
		{
			return;
		}
	}
	good = true;
	device->SetRenderState(D3DRS_ZENABLE, TRUE);
	OnDeviceCreated();
}

C_Device::C_Device(C_Window* win, bool fullscreen)
{
	this->win = win;
	d3d9 = 0;
	device = 0;
	good = false;
	lost = false;
	autorelease = false;
	oldstyle = 0;

	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	if(d3d9==0)
	{
		return;
	}

	D3DDISPLAYMODE mode;

	d3d9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode);

	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	d3dpp.BackBufferFormat = mode.Format;
	d3dpp.BackBufferHeight = mode.Height;
	d3dpp.BackBufferWidth = mode.Width;
	d3dpp.FullScreen_RefreshRateInHz = mode.RefreshRate;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.EnableAutoDepthStencil = 1;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	d3dpp.Windowed = 0;

	if(fullscreen != true)
	{
		d3dpp.Windowed = 1;
		d3dpp.FullScreen_RefreshRateInHz = 0;
		d3dpp.BackBufferHeight = 0;
		d3dpp.BackBufferWidth = 0;
	}

	HWND h = win->GetHandle();
	oldstyle = GetWindowLongPtr(h, GWL_STYLE);
	GetWindowRect(h, &oldrect);

	if(fullscreen)
	{
		LONG old = 0;
		old = GetWindowLongPtr(h, GWL_STYLE);
		old &= ~WS_SIZEBOX;
		old &= ~WS_CAPTION;
		SetWindowLongPtr(h, GWL_STYLE, old);
	}
	
	d3dpp.hDeviceWindow = h;

	HRESULT hres = d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, h, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &d3dpp, &device);	
	if(hres!=D3D_OK)
	{
		hres = d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, h, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &d3dpp, &device);
		if(hres!=D3D_OK)
		{
			return;
		}
	}
	good = true;
	device->SetRenderState(D3DRS_ZENABLE, TRUE);
	OnDeviceCreated();
}

C_Device::~C_Device()
{
	if(good) OnDeviceRelease();
	if(autorelease && win!=0) delete win;
	if(device!=0) device->Release();
	if(d3d9!=0) d3d9->Release();
}

void C_Device::Clear(unsigned int color)
{
	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f, 0);
}

void C_Device::StartDrawing()
{
	device->BeginScene();
}

void C_Device::EndDrawing()
{
	device->EndScene();
	
}

bool C_Device::Present()
{
	if(SUCCEEDED(device->Present(0,0,0,0))) return true;
	return false;
}

bool C_Device::ResetDevice()
{
	if(good) 
	{
		OnDeviceLost();
		if(FAILED(device->Reset(&d3dpp)))
		{
			return false;
		}
		OnDeviceReset();
		return true;
	}
	return false;
}

bool C_Device::CheckDevice()
{
	if(good)
	{
		HRESULT res = device->TestCooperativeLevel();
		if(res==D3DERR_DEVICELOST)
		{	
			Sleep(200);
			return false;
		}
		else if( res == D3DERR_DRIVERINTERNALERROR )
		{
			MessageBox(0, L"Internal Driver Error", 0, 0);
			PostQuitMessage(0);
			return false;
		}
		else if(res == D3DERR_DEVICENOTRESET )
		{
			if(ResetDevice())
			{
				return true;
			}
			else
			{
				MessageBox(0, L"Cannot reset device", 0, 0);
				PostQuitMessage(0);
				return false;
			}
		}
		return true;
	}
	return false;
}

void C_Device::SetView(const RectangleF& view)
{
	if(device==0) return;
	D3DVIEWPORT9 v;
	v.MaxZ = 1.0f;
	v.MinZ = 0;
	v.X = (DWORD)view.left;
	v.Y = (DWORD)view.top;
	v.Width = (DWORD)(view.right - view.left);
	v.Height = (DWORD)(view.bottom - view.top);
	device->SetViewport(&v);
	viewtransform.left = v.X;
	viewtransform.top = v.Y;
}

const PointI& C_Device::GetViewTransform() const
{
	return viewtransform;
}