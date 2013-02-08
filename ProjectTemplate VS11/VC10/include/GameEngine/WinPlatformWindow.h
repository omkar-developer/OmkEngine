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

#define KEY_ESCAPE		VK_ESCAPE
#define KEY_F1			VK_F1
#define KEY_F2			VK_F2
#define KEY_F3			VK_F3
#define KEY_F4			VK_F4
#define KEY_F5			VK_F5
#define KEY_F6			VK_F6
#define KEY_F7			VK_F7
#define KEY_F8			VK_F8
#define KEY_F9			VK_F9
#define KEY_F10			VK_F10
#define KEY_F11			VK_F11
#define KEY_F12			VK_F12
#define KEY_LBUTTON		VK_LBUTTON
#define KEY_RBUTTON		VK_RBUTTON
#define KEY_MBUTTON		VK_MBUTTON
#define KEY_XBUTTON1	VK_XBUTTON1
#define KEY_XBUTTON2	VK_XBUTTON2
#define KEY_BACK		VK_BACK
#define KEY_TAB			VK_TAB
#define KEY_ENTER		VK_RETURN
#define KEY_LCONTROL	VK_LCONTROL
#define KEY_RCONTROL	VK_RCONTROL
#define KEY_CONTROL		VK_CONTROL
#define KEY_LALT		VK_LMENU
#define KEY_RALT		VK_RMENU
#define KEY_ALT			VK_ALT
#define KEY_PAUSE		VK_PAUSE
#define KEY_SPACE		VK_SPACE
#define KEY_PAGEUP		VK_PRIOR
#define KEY_PAGEDOWN	VK_NEXT
#define KEY_END			VK_END
#define KEY_HOME		VK_HOME
#define KEY_LEFT		VK_LEFT
#define KEY_UP			VK_UP
#define KEY_RIGHT		VK_RIGHT
#define KEY_DOWN		VK_DOWN
#define KEY_PRINTSCR	VK_SNAPSHOT
#define KEY_INSERT		VK_INSERT
#define KEY_DELETE		VK_DELETE
#define KEY_NUMPAD0		VK_NUMPAD0
#define KEY_NUMPAD1		VK_NUMPAD1
#define KEY_NUMPAD2		VK_NUMPAD2
#define KEY_NUMPAD3		VK_NUMPAD3
#define KEY_NUMPAD4		VK_NUMPAD4
#define KEY_NUMPAD5		VK_NUMPAD5
#define KEY_NUMPAD6		VK_NUMPAD6
#define KEY_NUMPAD7		VK_NUMPAD7
#define KEY_NUMPAD8		VK_NUMPAD8
#define KEY_NUMPAD9		VK_NUMPAD9
#define KEY_0			'0'
#define KEY_1			'1'
#define KEY_2			'2'
#define KEY_3			'3'
#define KEY_4			'4'
#define KEY_5			'5'
#define KEY_6			'6'
#define KEY_7			'7'
#define KEY_8			'8'
#define KEY_9			'9'
#define KEY_A			'A'
#define KEY_B			'B'
#define KEY_C			'C'
#define KEY_D			'D'
#define KEY_E			'E'
#define KEY_F			'F'
#define KEY_G			'G'
#define KEY_H			'H'
#define KEY_I			'I'
#define KEY_J			'J'
#define KEY_K			'K'
#define KEY_L			'L'
#define KEY_M			'M'
#define KEY_N			'N'
#define KEY_O			'O'
#define KEY_P			'P'
#define KEY_Q			'Q'
#define KEY_R			'R'
#define KEY_S			'S'
#define KEY_T			'T'
#define KEY_U			'U'
#define KEY_V			'V'
#define KEY_W			'W'
#define KEY_X			'X'
#define KEY_Y			'Y'
#define KEY_Z			'Z'

#define MOUSEB_LEFT		MK_LBUTTON
#define MOUSEB_RIGHT	MK_RBUTTON
#define MOUSEB_MIDDLE	MK_MBUTTON
#define MOUSEB_X1		MK_XBUTTON1
#define MOUSEB_X2		MK_XBUTTON2


class C_Window
{
private:
	bool autoclose;
	HWND m_handle;

public:
	HWND GetHandle();
	PointI GetPosition();
	void SetPosition(PointI pos);
	Size GetSize();
	void SetSize(Size size);
	wstring GetTitle();
	void SetTitle(wstring val);
	bool GetVisible();
	void SetVisible(bool val);
	PointI GetCursorPos();
	void SetCursorPos(PointI pt);
	RectangleI GetDrawArea();

	Signal0 OnResize;
	Signal0 OnFocus;
	Signal0 OnLostFocus;
	Signal0 OnPaint;
	Signal0 OnClose;
	Signal0 OnVisible;

	Signal3<unsigned short, unsigned short, PointI const&> MouseDown;
	Signal3<unsigned short, unsigned short, PointI const&> MouseUp;
	Signal3<unsigned short, unsigned short, PointI const&> MouseDblClick;
	Signal1<PointI const&> MouseMove;
	Signal3<int, unsigned short, PointI const&> MouseWheel;

	Signal1<unsigned int> KeyDown;
	Signal1<unsigned int> KeyUp;
	Signal1<wchar_t> KeyChar;

	C_Window(const wchar_t* title, int width, int height, bool resize = true, bool caption = true);
	C_Window(HWND handle);
	~C_Window();

	bool IsFocused();
	void SetFocus();
	bool Close();
	void ShowCursor();
	void HideCursor();
	void CaptureMouse();
	bool ReleaseMouse();
	void Paint();
	void Refresh();
	bool ClientToScreen(PointI& pt);
	bool ScreenToClient(PointI& pt);
	bool KeyState(int key);
};
