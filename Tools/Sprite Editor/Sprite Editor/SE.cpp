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

#include "Windows.h"
#include "resource.h"
#include <gdiplus.h>
#include "commctrl.h"
#include "strsafe.h"
#include "Windowsx.h"
#include <vector>
#include <string>
#include <sstream>
#include "d3d9.h"
#include "d3dx9.h"
#include <fstream>

using namespace Gdiplus;
using namespace std;

#include "Variant.h"

wstringstream tmpstr;

struct RectangleI
{
	int left;
	int top;
	int right;
	int bottom;

	RectangleI();
	RectangleI(int left, int top, int right, int bottom);
};

RectangleI::RectangleI()
{
	left=top=right=bottom=0;
}

RectangleI::RectangleI(int left, int top, int right, int bottom)
{
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
}

struct SpriteFrame
{
	float delay;
	RectangleI rect;

	SpriteFrame()
	{
		delay = 0.0f;
		rect.left = 0;
		rect.top = 0;
		rect.right = 0;
		rect.bottom = 0;
	}

	SpriteFrame(float delay, RectangleI& rect)
	{
		this->delay = delay;
		this->rect = rect;
	}

	SpriteFrame(float delay, int left, int top, int right, int bottom)
	{
		this->delay = delay;
		this->rect.left = left;
		this->rect.top = top;
		this->rect.right = right;
		this->rect.bottom = bottom;
	}
};

struct imgs
{
	wstring path;
	WORD x;
	WORD y;
};

struct anim
{
	WORD x;
	WORD y;
};

Bitmap* bmp;
Bitmap* grid;
Bitmap* backb;
Bitmap* OImage;
Bitmap* SelImage = NULL;

int GridSpW =0;
int GridSpH =0;
WORD ImageWidth = 0;
WORD ImageHeight = 0;
BOOL picking = FALSE;
BOOL pickmultiple = FALSE;
WORD selx = 0;
WORD sely = 0;
BOOL gridw = FALSE;
BOOL ForEditing = FALSE;
WORD selx2 = 0;
WORD sely2 = 0;
WORD curframe = 0;

vector<imgs> images;
vector<anim> animation;


LPDIRECT3DDEVICE9 device = NULL;
LPDIRECT3DTEXTURE9 texture = NULL;
LPD3DXSPRITE spr = NULL;

void InitD3d(HWND wnd)
{
	IDirect3D9* d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.hDeviceWindow = wnd;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = 1;

	HRESULT res = d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, wnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &device);
	if(FAILED(res)) MessageBox(0, L"Direct3D9 Device Creation Failed", L"Sprite Editor", MB_OK);
	d3d9->Release();
}

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
   UINT  num = 0; 
   UINT  size = 0; 

   ImageCodecInfo* pImageCodecInfo = NULL;

   GetImageEncodersSize(&num, &size);
   if(size == 0)
      return -1;

   pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL)
      return -1;

   GetImageEncoders(num, size, pImageCodecInfo);

   for(UINT j = 0; j < num; ++j)
   {
      if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
      {
         *pClsid = pImageCodecInfo[j].Clsid;
         free(pImageCodecInfo);
         return j;
      }    
   }

   free(pImageCodecInfo);
   return -1;
}

void DrawBoxes(WORD iwidth, WORD iheight, WORD boxw=16, WORD boxh=16, WORD type=3)
{
	Graphics g(bmp);
	g.Clear(Color::Black);

	SolidBrush white(Color(255,255,255));
	SolidBrush gray(Color(200,200,200));
	SolidBrush black(Color(0,0,0));

	WORD maxw = iwidth / boxw;
	WORD maxh = iheight / boxh;

	for(WORD f=0; f<maxh; f++)
	{
		for(WORD i=0; i<maxw; i++)
		{
			if((f%2)?!(i%2):(i%2))
			{
				if(type==3 || type==2)
				{
					g.FillRectangle(&white, RectF(i*boxw,f*boxh,boxw,boxh));
					gridw = FALSE;
				}
				else if(type==1)
				{
					g.FillRectangle(&black, RectF(i*boxw,f*boxh,boxw,boxh));
					gridw = TRUE;
				}
			}
			else
			{
				if(type==3)
				{
					g.FillRectangle(&gray, RectF(i*boxw,f*boxh,boxw,boxh));
					gridw = FALSE;
				}
				else if(type==2)
				{
					g.FillRectangle(&white, RectF(i*boxw,f*boxh,boxw,boxh));
					gridw = FALSE;
				}
				else if(type==1)
				{
					g.FillRectangle(&black, RectF(i*boxw,f*boxh,boxw,boxh));
					gridw = TRUE;
				}
			}

		}	
	}
}

void DrawGrid(int w, int h, WORD iwidth, WORD iheight, BOOL num=TRUE, BOOL sg=TRUE)
{
	Graphics g2(grid);
	g2.Clear(Color::Transparent);
	Font fn(L"Ariel", 20, FontStyleRegular, UnitPixel);

	WORD tx, ty;
	tx = iwidth/w +1;
	ty = iheight/h +1;

	for(int i=1; i<tx; i++)
	{
		g2.DrawLine(&Pen(gridw?Color::White:Color::Black, 2), i*w, 0, i*w, iheight);
		if(sg)g2.DrawLine(&Pen(gridw?Color::White:Color::Gray, 1), ((i-1)*w)+w/2, 0, ((i-1)*w)+w/2, iheight);
	}	

	for(int i=1; i<ty; i++)
	{
		g2.DrawLine(&Pen(gridw?Color::White:Color::Black, 2), 0, i*h, iwidth, i*h);
		if(sg)g2.DrawLine(&Pen(gridw?Color::White:Color::Gray, 1), 0, ((i-1)*h) + h/2, iwidth, ((i-1)*h) + h/2);
	}

	if(num)
	{
		WCHAR tmp[20];
		WCHAR pos[10];

		int rowc = iwidth/w;
		for(int i=0; i<ty-1; i++)
		{
			for(int f=0; f<tx-1; f++)
			{

				_itow_s(i*rowc+f, pos, 5, 10); 
				tmp[0] = L'\0';
				StringCbCat(tmp, 20*2, pos);		
				/*StringCbCat(tmp, 20*2, L",");
				StringCbCat(tmp, 20*2, y);*/
				g2.DrawString(tmp, -1, &fn, RectF((f*w)+w/2-16,(i*h)+h/2-12,70,22), 0, &SolidBrush(gridw?Color::White:Color::Black));
			}
		}
	}	
	GridSpW = w;
	GridSpH = h;
	ImageWidth = iwidth;
	ImageHeight = iheight;
}

void SetTackBars(HWND vtrack, HWND htrack, WORD w, WORD h, WORD iwidth, WORD iheight)
{
	WORD Range2 = iwidth/w - 400/w -1;
	WORD Range1 = iheight/h - 400/h -1;
	PostMessage(vtrack, TBM_SETRANGE, 1, MAKELONG(0, Range1));
	PostMessage(htrack, TBM_SETRANGE, 1, MAKELONG(0, Range2));
}

void Refresh(HWND hwndDlg)
{
	delete bmp;
	delete grid;
	delete backb;
	delete OImage;

	WCHAR width[10];
	WCHAR height[10];

	GetWindowText(GetDlgItem(hwndDlg,IDC_WIDTH), width, 10);
	GetWindowText(GetDlgItem(hwndDlg,IDC_HEIGHT), height, 10);

	int w = _wtoi(width);
	int h = _wtoi(height);

	WCHAR width1[10];
	WCHAR height1[10];

	GetWindowText(GetDlgItem(hwndDlg,IDC_WIDTH1), width1, 10);
	GetWindowText(GetDlgItem(hwndDlg,IDC_HEIGHT1), height1, 10);

	int w1 = _wtoi(width1);
	int h1 = _wtoi(height1);

	bmp = new Bitmap(w1,h1, PixelFormat24bppRGB);
	grid = new Bitmap(w1,h1, PixelFormat32bppARGB);
	backb = new Bitmap(1024,1024, PixelFormat32bppARGB);
	OImage = new Bitmap(1024,1024, PixelFormat32bppARGB);

	SendDlgItemMessage(hwndDlg, IDC_GRID, BM_SETCHECK, BST_CHECKED, 0);
	SendDlgItemMessage(hwndDlg, IDC_SGRID, BM_SETCHECK, BST_CHECKED, 0);
	SendDlgItemMessage(hwndDlg, IDC_NUM, BM_SETCHECK, BST_CHECKED, 0);
	SendDlgItemMessage(hwndDlg, IDC_NUM3, BM_SETCHECK, BST_CHECKED, 0);
	SendDlgItemMessage(hwndDlg, IDC_NUM2, BM_SETCHECK, BST_UNCHECKED, 0);
	SendDlgItemMessage(hwndDlg, IDC_NUM1, BM_SETCHECK, BST_UNCHECKED, 0);

	DrawBoxes(w1,h1,8,8);

	DrawGrid(w,h, w1,h1, 1, 1);

	SetTackBars(GetDlgItem(hwndDlg, IDC_SLIDER1), GetDlgItem(hwndDlg, IDC_SLIDER2), w, h, w1, h1);

	GridSpW = w;
	GridSpH = h;
	ImageWidth = w1;
	ImageHeight = h1;
}

void DrawAllImages()
{	
	Graphics g(OImage);
	g.Clear(Color::Transparent);

	if(!ForEditing) 
	{
		for(int i=0; i<images.size(); i++)
		{			
			Bitmap b(images[i].path.data());
			g.DrawImage(&b, images[i].x, images[i].y, b.GetWidth(), b.GetHeight());
		}
	}
}


INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if(uMsg == WM_PAINT)
	{
		DWORD pos = SendMessage(GetDlgItem(hwndDlg, IDC_SLIDER1), TBM_GETPOS, 0, 0);
		DWORD pos2 = SendMessage(GetDlgItem(hwndDlg, IDC_SLIDER2), TBM_GETPOS, 0, 0);
		BOOL grids = SendDlgItemMessage(hwndDlg, IDC_GRID, BM_GETCHECK, 0, 0);

		PAINTSTRUCT ps;
		BeginPaint(hwndDlg, &ps);

		HDC dc = GetDC(GetDlgItem(hwndDlg,IDC_STATIC1));
		Graphics g(backb);
		Graphics g2(dc);
		int y = -GridSpH * pos;
		int x = -GridSpW * pos2;
		g.Clear(Color::LightGray);
		g.DrawImage(bmp, RectF(x,y, bmp->GetWidth(), bmp->GetHeight()));	
		g.DrawImage(OImage, RectF(x,y, OImage->GetWidth(), OImage->GetHeight()));
		if(ForEditing) g.DrawImage(SelImage, x+selx2, y+sely2, SelImage->GetWidth(), SelImage->GetHeight());
		if(grids)
		g.DrawImage(grid, RectF(x,y, grid->GetWidth(), grid->GetHeight()));
		if(picking)g.FillRectangle(&SolidBrush(Color(100, 0, 100, 200)), RectF(selx * GridSpW, sely * GridSpH, GridSpW, GridSpH));

		g2.DrawImage(backb, RectF(0,0, backb->GetWidth(), backb->GetHeight()));
		ReleaseDC(hwndDlg, dc);
		EndPaint(hwndDlg, &ps);
	}
	else if(uMsg == WM_INITDIALOG)
	{
		SetWindowText(GetDlgItem(hwndDlg, IDC_WIDTH), L"64");
		SetWindowText(GetDlgItem(hwndDlg, IDC_HEIGHT), L"64");
		SetWindowText(GetDlgItem(hwndDlg, IDC_WIDTH1), L"512");
		SetWindowText(GetDlgItem(hwndDlg, IDC_HEIGHT1), L"512");
		SetWindowText(GetDlgItem(hwndDlg, IDC_EDIT1), L"100");

		SendDlgItemMessage(hwndDlg, IDC_GRID, BM_SETCHECK, BST_CHECKED, 0);
		SendDlgItemMessage(hwndDlg, IDC_SGRID, BM_SETCHECK, BST_CHECKED, 0);
		SendDlgItemMessage(hwndDlg, IDC_NUM, BM_SETCHECK, BST_CHECKED, 0);
		SendDlgItemMessage(hwndDlg, IDC_NUM3, BM_SETCHECK, BST_CHECKED, 0);
		SendDlgItemMessage(hwndDlg, IDC_NUM2, BM_SETCHECK, BST_UNCHECKED, 0);
		SendDlgItemMessage(hwndDlg, IDC_NUM1, BM_SETCHECK, BST_UNCHECKED, 0);

		DrawBoxes(512,512,8,8);

		DrawGrid(64,64, 512,512, 1, 1);

		SetTackBars(GetDlgItem(hwndDlg, IDC_SLIDER1), GetDlgItem(hwndDlg, IDC_SLIDER2), 64, 64, 512, 512);

		ImageWidth = 512;
		ImageHeight = 512;

		GridSpW = 64;
		GridSpH = 64;		
		
//		CLSID pngClsid;
//		GetEncoderClsid(L"image/jpeg", &pngClsid);
//		EncoderParameters en;
//		en.Count = 0;
//		en.Parameter[0].Guid = EncoderQuality;
//		en.Parameter[0].NumberOfValues = 1;
//		en.Parameter[0].Type = EncoderParameterValueTypeLong;
//		ULONG quality = 50;
//		en.Parameter[0].Value = &quality;

//		Status st = bmp->Save(L"D:\\ok.jpg", &pngClsid, NULL);

		InitD3d(GetDlgItem(hwndDlg, IDC_STATIC2));
	}
	else if(uMsg == WM_COMMAND)
	{
		if(wParam==IDC_BUTTON11 || wParam==IDC_SGRID || wParam==IDC_NUM)
		{
			WCHAR width[10];
			WCHAR height[10];

			GetWindowText(GetDlgItem(hwndDlg,IDC_WIDTH), width, 10);
			GetWindowText(GetDlgItem(hwndDlg,IDC_HEIGHT), height, 10);

			int w = _wtoi(width);
			int h = _wtoi(height);

			BOOL sgrid = SendDlgItemMessage(hwndDlg, IDC_SGRID, BM_GETCHECK, 0, 0);
			BOOL num = SendDlgItemMessage(hwndDlg, IDC_NUM, BM_GETCHECK, 0, 0);
			
			DrawGrid(w,h, ImageWidth, ImageHeight, num, sgrid);		

			SetTackBars(GetDlgItem(hwndDlg, IDC_SLIDER1), GetDlgItem(hwndDlg, IDC_SLIDER2), w, h, ImageWidth, ImageHeight);

			RedrawWindow(hwndDlg, NULL, NULL, RDW_INTERNALPAINT);
		}
		else if(wParam==IDC_GRID)
		{
			RedrawWindow(hwndDlg, NULL, NULL, RDW_INTERNALPAINT);
		}
		else if(wParam==IDC_BUTTON12)
		{
			Refresh(hwndDlg);
			RedrawWindow(hwndDlg, NULL, NULL, RDW_INTERNALPAINT);
		}
		else if(wParam==IDC_PICK)
		{
			picking = TRUE;
			SetCapture(hwndDlg);
			POINT pt;
			pt.x = 100;
			pt.y = 100;
			ClientToScreen(hwndDlg, &pt);
			SetCursorPos(pt.x, pt.y);
			RedrawWindow(hwndDlg, NULL, NULL, RDW_INTERNALPAINT);
		}
		else if(wParam==IDC_PICK1)
		{
			picking = TRUE;
			pickmultiple = TRUE;
			SetCapture(hwndDlg);
			POINT pt;
			pt.x = 100;
			pt.y = 100;
			ClientToScreen(hwndDlg, &pt);
			SetCursorPos(pt.x, pt.y);
			RedrawWindow(hwndDlg, NULL, NULL, RDW_INTERNALPAINT);
		}
		else if(wParam==IDC_BUTTON3)
		{
			WCHAR tmp[20];
			GetWindowText(GetDlgItem(hwndDlg, IDC_EDIT2), tmp, 20);
			if(wcslen(tmp)>0)
		    ListBox_AddString(GetDlgItem(hwndDlg, IDC_LIST2), tmp);
		}
		else if(wParam==IDC_BUTTON13)
		{
			int sel = ListBox_GetCurSel(GetDlgItem(hwndDlg, IDC_LIST2));
			if(sel!=LB_ERR) ListBox_DeleteString(GetDlgItem(hwndDlg, IDC_LIST2), sel);
		}
		else if(wParam==IDC_BUTTON14)
		{
			ListBox_ResetContent(GetDlgItem(hwndDlg, IDC_LIST2));
		}
		else if(wParam==IDC_BUTTON5)
		{
			int sel = ListBox_GetCurSel(GetDlgItem(hwndDlg, IDC_LIST2));
			if(sel>0)
			{
				WCHAR tmp[MAX_PATH];
				ListBox_GetText(GetDlgItem(hwndDlg, IDC_LIST2), sel, tmp);
				ListBox_InsertString(GetDlgItem(hwndDlg, IDC_LIST2), sel-1, tmp);
				ListBox_DeleteString(GetDlgItem(hwndDlg, IDC_LIST2), sel +1);
				ListBox_SetCurSel(GetDlgItem(hwndDlg, IDC_LIST2), sel -1);
			}
		}
		else if(wParam==IDC_BUTTON4)
		{
			int sel = ListBox_GetCurSel(GetDlgItem(hwndDlg, IDC_LIST2));
			int max = ListBox_GetCount(GetDlgItem(hwndDlg, IDC_LIST2));
			if(sel<max-1)
			{
				WCHAR tmp[MAX_PATH];
				ListBox_GetText(GetDlgItem(hwndDlg, IDC_LIST2), sel, tmp);
				ListBox_DeleteString(GetDlgItem(hwndDlg, IDC_LIST2), sel);
				ListBox_InsertString(GetDlgItem(hwndDlg, IDC_LIST2), sel+1, tmp);
				ListBox_SetCurSel(GetDlgItem(hwndDlg, IDC_LIST2), sel + 1);
			}
		}
		else if(wParam==IDC_NUM1)
		{
			DrawBoxes(ImageWidth,ImageHeight,8,8, 1);
			BOOL sgrid = SendDlgItemMessage(hwndDlg, IDC_SGRID, BM_GETCHECK, 0, 0);
			BOOL num = SendDlgItemMessage(hwndDlg, IDC_NUM, BM_GETCHECK, 0, 0);

			DrawGrid(GridSpW,GridSpH, ImageWidth, ImageHeight, num, sgrid);	

			SendDlgItemMessage(hwndDlg, IDC_NUM3, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hwndDlg, IDC_NUM2, BM_SETCHECK, BST_UNCHECKED, 0);
			RedrawWindow(hwndDlg, NULL, NULL, RDW_INTERNALPAINT);
		}
		else if(wParam==IDC_NUM2)
		{
			DrawBoxes(ImageWidth,ImageHeight,8,8, 2);
			BOOL sgrid = SendDlgItemMessage(hwndDlg, IDC_SGRID, BM_GETCHECK, 0, 0);
			BOOL num = SendDlgItemMessage(hwndDlg, IDC_NUM, BM_GETCHECK, 0, 0);

			DrawGrid(GridSpW,GridSpH, ImageWidth, ImageHeight, num, sgrid);	

			SendDlgItemMessage(hwndDlg, IDC_NUM3, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hwndDlg, IDC_NUM1, BM_SETCHECK, BST_UNCHECKED, 0);
			RedrawWindow(hwndDlg, NULL, NULL, RDW_INTERNALPAINT);
		}
		else if(wParam==IDC_NUM3)
		{
			DrawBoxes(ImageWidth,ImageHeight,8,8, 3);
			BOOL sgrid = SendDlgItemMessage(hwndDlg, IDC_SGRID, BM_GETCHECK, 0, 0);
			BOOL num = SendDlgItemMessage(hwndDlg, IDC_NUM, BM_GETCHECK, 0, 0);

			DrawGrid(GridSpW,GridSpH, ImageWidth, ImageHeight, num, sgrid);	

			SendDlgItemMessage(hwndDlg, IDC_NUM2, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hwndDlg, IDC_NUM1, BM_SETCHECK, BST_UNCHECKED, 0);
			RedrawWindow(hwndDlg, NULL, NULL, RDW_INTERNALPAINT);
		}
		else if(wParam==IDC_BUTTON1)
		{
			OPENFILENAME ofn;
			WCHAR filename[MAX_PATH];
			filename[0] = L'\0';

			ZeroMemory(&ofn, sizeof(OPENFILENAME));

			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hwndDlg;
			ofn.lpstrFile = filename;
			ofn.nMaxFile = sizeof(filename);
			ofn.lpstrFilter = L"All\0*.*\0All Image Files\0*.png;*.bmp;*.jpg;*.jpeg\0Bmp\0*.bmp\0Jpeg\0*.jpg\0Png\0*.png\0";
			ofn.nFilterIndex = 2;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			if (GetOpenFileName(&ofn)==TRUE) 
			{
				WCHAR tmp[MAX_PATH + 10];
				tmp[0]=L'\0';
				StringCchCat(tmp, MAX_PATH + 10, ofn.lpstrFile);
				StringCchCat(tmp, MAX_PATH + 10, L" (0,0)");
				ListBox_AddString(GetDlgItem(hwndDlg, IDC_LIST1), tmp);
				imgs tmp2;
				tmp2.path = ofn.lpstrFile;
				tmp2.x = 0;
				tmp2.y = 0;
				images.push_back(tmp2);
				DrawAllImages();
				RedrawWindow(hwndDlg, NULL, NULL, RDW_INTERNALPAINT);
			}
		}
		else if(wParam==IDC_BUTTON2)
		{
			int sel = ListBox_GetCurSel(GetDlgItem(hwndDlg, IDC_LIST1));
			if(sel!=LB_ERR)
			{
				ListBox_DeleteString(GetDlgItem(hwndDlg, IDC_LIST1), sel);
				images.erase(images.begin()+sel);
				DrawAllImages();
				RedrawWindow(hwndDlg, NULL, NULL, RDW_INTERNALPAINT);
			}
		}
		else if(wParam==IDC_BUTTON16)
		{
			ListBox_ResetContent(GetDlgItem(hwndDlg, IDC_LIST1));
			images.clear();
			DrawAllImages();
			RedrawWindow(hwndDlg, NULL, NULL, RDW_INTERNALPAINT);
		}
		else if(wParam==IDC_BUTTON8)
		{
			int sel = ListBox_GetCurSel(GetDlgItem(hwndDlg, IDC_LIST1));
			if(sel!=LB_ERR)
			{
				ForEditing = FALSE;
				SelImage = new Bitmap(images[sel].path.data());
				ForEditing = TRUE;		
				DrawAllImages();
				SetCapture(hwndDlg);
				POINT pt;
				int sel = ListBox_GetCurSel(GetDlgItem(hwndDlg, IDC_LIST1));
				pt.x = images[sel].x + SelImage->GetWidth()/2;
				pt.y = images[sel].y + SelImage->GetHeight()/2;
				ClientToScreen(hwndDlg, &pt);
				SetCursorPos(pt.x, pt.y);
				RedrawWindow(hwndDlg, NULL, NULL, RDW_INTERNALPAINT);
			}
		}
		else if(wParam==IDC_BUTTON15)
		{
			DrawAllImages();
			RedrawWindow(hwndDlg, NULL, NULL, RDW_INTERNALPAINT);
		}
		else if(wParam==IDC_BUTTON6)
		{
			animation.clear();
			int count = ListBox_GetCount(GetDlgItem(hwndDlg, IDC_LIST2));

			int sel = ListBox_GetCurSel(GetDlgItem(hwndDlg, IDC_LIST2));
			if(sel!=-1)
			{
				WCHAR tmp[260];
				ListBox_GetText(GetDlgItem(hwndDlg, IDC_LIST2), sel, tmp);

				wstringstream wstr;

				for(WORD f=0; f<260; f++)
				{
					wstr << tmp[f];
					if(tmp[f]==L',' || tmp[f]==L'\0')
					{
						wstr << "\0";

						WORD pos;
						pos = _wtoi(wstr.str().c_str());
						wstr.str(L"");
						wstr.clear();

						WORD rowc = ImageWidth / GridSpW;

						WORD y = floor((float)pos / rowc);
						WORD x = pos % rowc;

						anim a;
						a.x = x;
						a.y = y;
						animation.push_back(a);
					}
				}				
		
				DrawAllImages();

				WCHAR tmp4[260];
				GetWindowText(GetDlgItem(hwndDlg, IDC_EDIT1), tmp4, 260);
				UINT time = _wtoi(tmp4);
				if(time<10)time = 10;

				if(texture!=NULL)
				{
					texture->Release();
					texture = NULL;
				}

				if(spr!=NULL)
				{
					spr->Release();
					spr = NULL;
				}

				CLSID pngClsid;
				GetEncoderClsid(L"image/png", &pngClsid);

				OImage->Save(L"tmp.png", &pngClsid);

				D3DXCreateTextureFromFile(device, L"tmp.png", &texture);

				D3DXCreateSprite(device, &spr);

				SetTimer(hwndDlg, 10, time, NULL);
			}
		}
		else if(wParam==IDC_BUTTON7)
		{
			KillTimer(hwndDlg, 10);
			animation.clear();
			if(texture!=NULL)
			{
				texture->Release();
				texture = NULL;
			}
			if(spr!=NULL)
			{
				spr->Release();
				spr = NULL;
			}
			curframe = 0;
		}
		else if(wParam==IDC_BUTTON9)
		{
			OPENFILENAME ofn;
			WCHAR filename[MAX_PATH];
			filename[0] = L'\0';

			ZeroMemory(&ofn, sizeof(OPENFILENAME));

			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hwndDlg;
			ofn.lpstrFile = filename;
			ofn.nMaxFile = sizeof(filename);
			ofn.lpstrFilter = L"Png\0*.png\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST;

			if (GetSaveFileName(&ofn)==TRUE) 
			{
				CLSID pngClsid;
				GetEncoderClsid(L"image/png", &pngClsid);
				StringCchCat(filename, 260, L".png");

				OImage->Save(filename, &pngClsid);				
			}
		}
		else if(wParam==IDC_BUTTON10)
		{
			for(unsigned int i = 0; i<(ImageWidth/GridSpW) * (ImageHeight/GridSpH); i++)
			{
				WCHAR stmp[10];
				_itow(i, stmp, 10);
				ListBox_AddString(GetDlgItem(hwndDlg, IDC_LIST2), stmp);
			}
		}
		else if(wParam==IDC_BUTTON17)
		{
			OPENFILENAME ofn;
			WCHAR filename[MAX_PATH];
			filename[0] = L'\0';

			ZeroMemory(&ofn, sizeof(OPENFILENAME));

			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hwndDlg;
			ofn.lpstrFile = filename;
			ofn.nMaxFile = sizeof(filename);
			ofn.lpstrFilter = L"iset\0*.iset\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_LONGNAMES | OFN_OVERWRITEPROMPT;

			if(GetSaveFileName(&ofn)==TRUE) 
			{
				vector<SpriteFrame> frames;
				vector<Variant> Indexes;

				WCHAR tmp4[260];
				GetWindowText(GetDlgItem(hwndDlg, IDC_EDIT1), tmp4, 260);
				UINT time = _wtoi(tmp4);
				if(time<10)time = 10;

				char resname[260];
				GetWindowTextA(GetDlgItem(hwndDlg, IDC_EDIT3), resname, 260);

				SpriteFrame frame2;
				frame2.delay = time / 1000.0f;
				frame2.rect = RectangleI(0, 0, GridSpW, GridSpH);

				frames.push_back(frame2);

				int count = ListBox_GetCount(GetDlgItem(hwndDlg, IDC_LIST2));

				for(unsigned int i = 0; i<count; i++)
				{
					WCHAR tmp[260];
					char tmp2[260];
					ListBox_GetText(GetDlgItem(hwndDlg, IDC_LIST2), i, tmp);

					wcstombs(tmp2, tmp, 260);

					Variant str = tmp2;
					Indexes.push_back(str);
				}

				//save frames
				Variant str, str2;
				bool autogen = true;
				bool good = false;

				ofstream fs(filename, fstream::binary);
				str = resname;
				autogen = true;
				str >> fs;
				fs.write((char*)&autogen, sizeof(bool));

				for(int i=0; i<frames.size(); i++)
				{
					fs.write((char*)&frames[i].delay, sizeof(float));
					fs.write((char*)&frames[i].rect, sizeof(RectangleI));
					good = true;
				}

				SpriteFrame frame;
				frame.rect = RectangleI(0,0,0,0);
				frame.delay = 0;
				fs.write((char*)&frame.delay, sizeof(float));
				fs.write((char*)&frame.rect, sizeof(RectangleI));

				for(int i=0; i<Indexes.size(); i++)
				{
					Indexes[i] >> fs;
				}

				fs.close();
				//end frameset
			}
		}
		else if(wParam==IDC_BUTTON18)
		{
			OPENFILENAME ofn;
			WCHAR filename[MAX_PATH];
			filename[0] = L'\0';

			ZeroMemory(&ofn, sizeof(OPENFILENAME));

			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hwndDlg;
			ofn.lpstrFile = filename;
			ofn.nMaxFile = sizeof(filename);
			ofn.lpstrFilter = L"iset\0*.iset\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_LONGNAMES;

			if(GetOpenFileName(&ofn)==TRUE) 
			{
				vector<SpriteFrame> frames;
				vector<Variant> Indexes;

				Indexes.clear();
				frames.clear();
				Variant str, str2;
				bool autogen = false;
				bool good = false;
				SpriteFrame frame;

				fstream fs(filename, ios::in | ios::binary);
				str << fs;
				fs.read((char*)&autogen, sizeof(bool));

				if(autogen==false)
				{
					fs.close();
					MessageBox(hwndDlg, L"Advanced imageset files cannot be edited in this editor please use editor which comes with resource editor instead.", L"Error", MB_ICONERROR);
					return 0;
				}

				SetWindowTextA(GetDlgItem(hwndDlg, IDC_EDIT3), str);

				while(!fs.eof())
				{
					fs.read((char*)&frame.delay, sizeof(float));
					fs.read((char*)&frame.rect, sizeof(RectangleI));
					good = true;
					if(frame.delay == 0 && frame.rect.left == 0 && frame.rect.right == 0 && frame.rect.top == 0 && frame.rect.bottom == 0)
					{
						break;
					}
					else
					{
						frames.push_back(frame);
					}
				}

				while(!fs.eof())
				{
					str2.ClearString();
					str2 << fs;
					Indexes.push_back(str2);
				}

				fs.close();
				
				ListBox_ResetContent(GetDlgItem(hwndDlg, IDC_LIST2));

				if(frames.size()>0)
				{
					int delay = frames[0].delay * 1000;
					int gridw = frames[0].rect.right;
					int gridh = frames[0].rect.bottom;

					WCHAR tm1[10];
					_itow(delay, tm1, 10);
					SetWindowText(GetDlgItem(hwndDlg, IDC_EDIT1), tm1);

					_itow(gridw, tm1, 10);
					SetWindowText(GetDlgItem(hwndDlg, IDC_WIDTH), tm1);
					_itow(gridh, tm1, 10);
					SetWindowText(GetDlgItem(hwndDlg, IDC_HEIGHT), tm1);
				}

				for(unsigned int i = 0; i<Indexes.size(); i++)
				{
					string tmstrs = Indexes[i];
					if(tmstrs=="NULL") break;
					WCHAR stmp[260];
					mbstowcs(stmp, tmstrs.c_str(), 260);
					ListBox_AddString(GetDlgItem(hwndDlg, IDC_LIST2), stmp);
				}

				//redraw
				WCHAR width[10];
				WCHAR height[10];

				GetWindowText(GetDlgItem(hwndDlg,IDC_WIDTH), width, 10);
				GetWindowText(GetDlgItem(hwndDlg,IDC_HEIGHT), height, 10);

				int w = _wtoi(width);
				int h = _wtoi(height);

				BOOL sgrid = SendDlgItemMessage(hwndDlg, IDC_SGRID, BM_GETCHECK, 0, 0);
				BOOL num = SendDlgItemMessage(hwndDlg, IDC_NUM, BM_GETCHECK, 0, 0);
			
				DrawGrid(w,h, ImageWidth, ImageHeight, num, sgrid);		

				SetTackBars(GetDlgItem(hwndDlg, IDC_SLIDER1), GetDlgItem(hwndDlg, IDC_SLIDER2), w, h, ImageWidth, ImageHeight);

				RedrawWindow(hwndDlg, NULL, NULL, RDW_INTERNALPAINT);
			}
		}
	}
	else if(uMsg == WM_TIMER)
	{
		if(wParam==10)
		{
			if(animation.size()>0)
			{
				if(curframe == animation.size()) curframe = 0;
				device->Clear(0,0,D3DCLEAR_TARGET, D3DCOLOR_ARGB(1,0,0,0), 0, 0);
				RECT rect;
				rect.left = animation[curframe].x * GridSpW;
				rect.top = animation[curframe].y * GridSpH;
				rect.bottom = rect.top + GridSpH;
				rect.right = rect.left + GridSpW;

				device->BeginScene();
				spr->Begin(0);
				spr->Draw(texture, &rect, &D3DXVECTOR3(0,0,0), &D3DXVECTOR3(0,0,0), D3DXCOLOR(1,1,1,1));
				spr->End();
				device->EndScene();
				device->Present(0,0,0,0);
				++curframe;
			}
		}
	}
	else if(uMsg == WM_VSCROLL || uMsg == WM_HSCROLL)
	{
		if(LOWORD(wParam)==TB_ENDTRACK)
		{
			RedrawWindow(hwndDlg, NULL, NULL, RDW_INTERNALPAINT);
		}
	}
	else if(uMsg == WM_MOUSEMOVE)
	{
		if(picking)
		{
			WORD x = LOWORD(lParam);
			WORD y = HIWORD(lParam);
			
			if(x>2 && x<600 && y>2 && y<800)
			{
				WCHAR tmp[20];
				WCHAR p[10];

				DWORD pos2 = SendMessage(GetDlgItem(hwndDlg, IDC_SLIDER1), TBM_GETPOS, 0, 0);
				DWORD pos = SendMessage(GetDlgItem(hwndDlg, IDC_SLIDER2), TBM_GETPOS, 0, 0);

				//_itow_s(x/GridSpW+pos, x1, 5, 10);
				//_itow_s(y/GridSpH+pos2, y1, 5, 10);

				int posx = x/GridSpW+pos;
				int posy = y/GridSpH+pos2;
				int rowc = ImageWidth / GridSpW;

				_itow_s(posy*rowc+posx, p, 5, 10);

				tmp[0] = L'\0';
				StringCchCat(tmp, 20, p);
				/*StringCchCat(tmp, 20, L",");
				StringCchCat(tmp, 20, y1);*/
				SetWindowText(GetDlgItem(hwndDlg, IDC_EDIT2),tmp);
				if(selx != x/GridSpW || sely != y/GridSpH) RedrawWindow(hwndDlg, NULL, NULL, RDW_INTERNALPAINT);
				selx = x/GridSpW;
				sely = y/GridSpH;
			}
		}
		else if(ForEditing)
		{
			WORD x = LOWORD(lParam);
			WORD y = HIWORD(lParam);
			
			if(x>2 && x<600 && y>2 && y<800)
			{
				x-= SelImage->GetWidth()/2;
				y-= SelImage->GetHeight()/2;
				selx2 = x;
				sely2 = y;
				RedrawWindow(hwndDlg, NULL, NULL, RDW_INTERNALPAINT);
			}			
		}
	}
	else if(uMsg == WM_LBUTTONUP)
	{
		if(ForEditing)
		{
			ForEditing = FALSE;
			ReleaseCapture();

			delete SelImage;
			int sel = ListBox_GetCurSel(GetDlgItem(hwndDlg, IDC_LIST1));

			DWORD pos = SendMessage(GetDlgItem(hwndDlg, IDC_SLIDER1), TBM_GETPOS, 0, 0);
			DWORD pos2 = SendMessage(GetDlgItem(hwndDlg, IDC_SLIDER2), TBM_GETPOS, 0, 0);

			int y = -GridSpH * pos;
			int x = -GridSpW * pos2;

			images[sel].x = x + selx2;
			images[sel].y = y + sely2;

			DrawAllImages();
			RedrawWindow(hwndDlg, NULL, NULL, RDW_INTERNALPAINT);
		}
		if(!pickmultiple && picking)
		{
			picking = FALSE;
			ReleaseCapture();
			RedrawWindow(hwndDlg, NULL, NULL, RDW_INTERNALPAINT);
		}
		else if(picking && pickmultiple)
		{
			WCHAR tmp[20];
			GetWindowText(GetDlgItem(hwndDlg, IDC_EDIT2), tmp, 20);
			if(tmpstr.str().size()>0)
			{
				tmpstr << ",";
			}

			tmpstr << tmp;								
		}
	}
	else if(uMsg == WM_RBUTTONUP)
	{
		if(pickmultiple)
		{
			picking = FALSE;
			pickmultiple = FALSE;
			ReleaseCapture();
			RedrawWindow(hwndDlg, NULL, NULL, RDW_INTERNALPAINT);
			if(tmpstr.str().size()>0)
			ListBox_AddString(GetDlgItem(hwndDlg, IDC_LIST2), tmpstr.str().c_str());	
			tmpstr.str(L"");
			tmpstr.clear();
		}
	}

	switch(uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nShowCmd)
{
	InitCommonControls();
	GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
   
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	bmp = new Bitmap(1024,1024, PixelFormat24bppRGB);
	grid = new Bitmap(1024,1024, PixelFormat32bppARGB);
	backb = new Bitmap(1024,1024, PixelFormat32bppARGB);
	OImage = new Bitmap(1024,1024, PixelFormat32bppARGB);

	DialogBox(hInstance, (LPCWSTR) IDD_DIALOG1, NULL, DialogProc);

	delete bmp;
	delete grid;
	delete OImage;
	delete backb;
	GdiplusShutdown(gdiplusToken);
	if(texture!=NULL) texture->Release();
	if(spr!=NULL) spr->Release();
	if(device!=NULL) device->Release();
	return 0;
}