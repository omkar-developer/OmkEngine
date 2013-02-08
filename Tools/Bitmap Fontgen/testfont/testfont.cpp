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
#include <Windows.h>
#include <GdiPlus.h>
#include <string>
#include <vector>
#include <fstream>

using namespace Gdiplus;
using namespace std;

#include "Variant.h"

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

int _tmain(int argc, _TCHAR* argv[])
{	
	wstring fname = L"ONKFont";
	wstring output = L"Font";
	string resname = "FontImg";

	if(argc<=1)
	{
		printf("Syntax : fontgen \"Font Name\" \"[optional default FontImg]Image Resource Name\" \"[Optional]Output File Path\" \nExample : fontgen \"Arial\" \"FontImg\" \"C:\\font\" \n");
		//return 0;
	}
	else if(argc==2)
	{
		if(wcscmp(argv[1], L"help")==0 || wcscmp(argv[1], L"-help")==0 || wcscmp(argv[1], L"--help")==0 || wcscmp(argv[1], L"\\help")==0 || wcscmp(argv[1], L"-h")==0)
		{
		printf("Syntax : fontgen \"Font Name\" \"[optional default FontImg]Image Resource Name\" \"[Optional]Output File Path\" \nExample : fontgen \"Arial\" \"FontImg\" \"C:\\font\" \n");
			return 0;
		}
		fname = argv[1];
	}
	else if(argc==3)
	{
		fname = argv[1];
		char bf[256];
		wcstombs(bf, argv[2], 256);
		resname = bf;
	}
	else if(argc==4)
	{
		fname = argv[1];
		char bf[256];
		wcstombs(bf, argv[2], 256);
		resname = bf;
		output = argv[3];
	}

	vector<SpriteFrame> frames;
	vector<Variant> Indexes;

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	CLSID pngClsid;

	UINT sz = 0, size = 0;
	GetImageEncodersSize(&sz, &size);

	char* buff = new char[size];
	ImageCodecInfo* info = (ImageCodecInfo*) buff;
	GetImageEncoders(sz, size, info);

	for(unsigned int i=0; i<sz; i++)
	{
		if(wcscmp(info[i].MimeType, L"image/png")==0)
		{
			pngClsid = info[i].Clsid;
		}
	}

	delete buff;

	Bitmap* bmp = new Bitmap(512, 512, PixelFormat32bppARGB);
	Graphics* g = new Graphics(bmp);
	//g->DrawRectangle(&Pen(Color::Black), 10, 10, 200, 200);
	//g->SetCompositingMode(CompositingModeSourceCopy);
	g->SetCompositingQuality(CompositingQualityHighQuality);
	g->SetInterpolationMode(InterpolationModeHighQualityBicubic);
	//g->SetPixelOffsetMode(PixelOffsetModeHighQuality);
	g->SetSmoothingMode(SmoothingModeAntiAlias);
	g->SetTextRenderingHint(TextRenderingHintAntiAlias);
	
	unsigned int fsize = 32;
	unsigned int fspace = 4;
	
	printf("\nEnter Font Size : ");
	scanf("%d", &fsize);
	printf("\nEnter Outline Size : ");
	scanf("%d", &fspace);

	unsigned int fcspace = fsize - fspace * 2;

	unsigned int sx = 0, sy = 0;
	float sw = 0.0f;
	bool loop = true;
	for(unsigned int i = 0; i<16; i++)
	{
		for(unsigned int f = 0; f<16; f++)
		{
			StringFormat fmt;
			fmt.SetAlignment(StringAlignmentNear);
			fmt.SetLineAlignment(StringAlignmentCenter);

			FontFamily ff(fname.c_str());

			wchar_t c = (char)(i*16+f);

			Pen p(Color::Black, fspace);
			p.SetLineJoin(LineJoinRound);
			p.SetLineCap(LineCapRound, LineCapRound, DashCapRound);

			Font fnt(&ff, fcspace, 0, UnitPixel);
			RectF bRect;
			g->MeasureString(&c, 1, &fnt, RectF(0, 0, fsize, fsize), &fmt, &bRect);

			if((char)(i*16+f)==' ')
			{
				bRect.Width += 15;
			}

			if(sw + bRect.Width>512)
			{
				sy += 1;
				sw = 0;
				sx = 0;
			}
			else
			{
				sx += 1;
			}

			GraphicsPath path;
			path.AddString(&c, 1, &ff, 0, fcspace, RectF(sw, sy*fsize, fsize, fsize), &fmt);

			//if(fspace>0) g->DrawPath(&p, &path);
			g->FillPath(&SolidBrush(Color::White), &path);			

			//Frames
			SpriteFrame frame;
			frame.delay = 0.1f;
			frame.rect.left = (int)(sw) + 8;
			frame.rect.top = (int)(sy*fsize);
			frame.rect.right = (int)(frame.rect.left + bRect.Width) - 8;
			frame.rect.bottom = (int)(frame.rect.top + bRect.Height);
			frames.push_back(frame);

			/*RectF rc2(frame.rect.left, frame.rect.top, frame.rect.right - frame.rect.left, frame.rect.bottom - frame.rect.top);
			g->DrawRectangle(&Pen(Color::Black), rc2);*/

			char b[50];
			_itoa(i*16+f, b, 10);
			Variant str = b;
			Indexes.push_back(str);		

			sw += bRect.Width;

			if((char)(i*16+f)=='~')
			{
				loop = false;
				break;
			}
		}
		if(!loop) break;
	}

	wstring outputf = output;
	wstring outputs = output;
	outputf += L".png";
	outputs += L".iset";

	bmp->Save(outputf.c_str(), &pngClsid);

	//save frames
	Variant str, str2;
	bool autogen = false;
	bool good = false;

	//wxString str3 = diag.GetPath();
	const wchar_t* fpath = outputs.c_str();

	ofstream fs(fpath, fstream::binary);
	str = resname.c_str();
	autogen = false;
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

	delete g;
	delete bmp;
	GdiplusShutdown(gdiplusToken);
	return 0;
}

