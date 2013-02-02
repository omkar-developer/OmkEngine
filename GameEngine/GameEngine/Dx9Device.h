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

class C_Device
{
private:
	IDirect3D9* d3d9;
	IDirect3DDevice9* device;
	bool good;
	bool lost;
	bool autorelease;
	C_Window* win;
	LONG oldstyle;
	RECT oldrect;
	D3DPRESENT_PARAMETERS d3dpp;
	PointI viewtransform;

public:
	Size GetBackBufferSize();
	void SetBackBufferSize(Size size);

	bool GetFullscreen();
	void SetFullscreen(bool val);

	IDirect3DDevice9* GetDevice();

	bool GetGood();

	C_Window* GetWindow();

	Signal0 OnDeviceLost;
	Signal0 OnDeviceReset;
	Signal0 OnDeviceCreated;
	Signal0 OnDeviceRelease;
	
	C_Device(wchar_t* title, bool fullscreen = false);
	C_Device(C_Window* win, bool fullscreen = false);
	~C_Device();

	void StartDrawing();
	void EndDrawing();
	bool ResetDevice();
	bool CheckDevice();
	bool Present();
	void Clear(unsigned int color = 0);
	void SetView(const RectangleF& view);
	const PointI& GetViewTransform() const;
};