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

#define MAX_KEY			0xFFFFFFFF

#define KEY_ESCAPE		111
#define KEY_F1			131
#define KEY_F2			132
#define KEY_F3			133
#define KEY_F4			134
#define KEY_F5			135
#define KEY_F6			136
#define KEY_F7			137
#define KEY_F8			138
#define KEY_F9			139
#define KEY_F10			140
#define KEY_F11			141
#define KEY_F12			142
#define KEY_LBUTTON		1
#define KEY_RBUTTON		MAX_KEY
#define KEY_MBUTTON		MAX_KEY
#define KEY_XBUTTON1	MAX_KEY
#define KEY_XBUTTON2	MAX_KEY
#define KEY_BACK		4
#define KEY_TAB			61
#define KEY_ENTER		66
#define KEY_LCONTROL	113
#define KEY_RCONTROL	114
#define KEY_CONTROL		113
#define KEY_LALT		57
#define KEY_RALT		58
#define KEY_ALT			57
#define KEY_PAUSE		121
#define KEY_SPACE		62
#define KEY_PAGEUP		92
#define KEY_PAGEDOWN	93
#define KEY_END			123
#define KEY_HOME		122
#define KEY_LEFT		21
#define KEY_UP			19
#define KEY_RIGHT		22
#define KEY_DOWN		20
#define KEY_PRINTSCR	120
#define KEY_INSERT		124
#define KEY_DELETE		67
#define KEY_NUMPAD0		144
#define KEY_NUMPAD1		145
#define KEY_NUMPAD2		146
#define KEY_NUMPAD3		147
#define KEY_NUMPAD4		148
#define KEY_NUMPAD5		149
#define KEY_NUMPAD6		150
#define KEY_NUMPAD7		151
#define KEY_NUMPAD8		152
#define KEY_NUMPAD9		153
#define KEY_0			7
#define KEY_1			8
#define KEY_2			9
#define KEY_3			10
#define KEY_4			11
#define KEY_5			12
#define KEY_6			13
#define KEY_7			14
#define KEY_8			15
#define KEY_9			16
#define KEY_A			29
#define KEY_B			30
#define KEY_C			31
#define KEY_D			32
#define KEY_E			33
#define KEY_F			34
#define KEY_G			35
#define KEY_H			36
#define KEY_I			37
#define KEY_J			38
#define KEY_K			39
#define KEY_L			40
#define KEY_M			41
#define KEY_N			42
#define KEY_O			43
#define KEY_P			44
#define KEY_Q			45
#define KEY_R			46
#define KEY_S			47
#define KEY_T			48
#define KEY_U			49
#define KEY_V			50
#define KEY_W			51
#define KEY_X			52
#define KEY_Y			53
#define KEY_Z			54

#define MOUSEB_LEFT		1
#define MOUSEB_RIGHT	MAX_KEY
#define MOUSEB_MIDDLE	MAX_KEY
#define MOUSEB_X1		MAX_KEY
#define MOUSEB_X2		MAX_KEY


class C_Window
{
private:
	float width, height;

public:
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
