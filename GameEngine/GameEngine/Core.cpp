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

float fast_rsqrt( float number )
{
        long i;
        float x2, y;
        const float threehalfs = 1.5F;
 
        x2 = number * 0.5F;
        y  = number;
        i  = * ( long * ) &y;                       // evil floating point bit level hacking
        i  = 0x5f3759df - ( i >> 1 );               
        y  = * ( float * ) &i;
        y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
//      y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
        return y;
}

void WideCharToMultiByte(char* dest, const wchar_t* src, unsigned int max)
{
	for(unsigned int i = 0; i < max; i++)
	{
		dest[i] = (char)src[i];
		if(src[i] == L'\0') break;
	}
}

void MultiByteToWideChar(wchar_t* dest, const char* src, unsigned int max)
{
	for(unsigned int i = 0; i < max; i++)
	{
		dest[i] = (wchar_t)src[i];
		if(src[i] == L'\0') break;
	}
}

void Convert16BitWchar(wchar_t* dest, const unsigned short* src, unsigned int max)
{
	for(unsigned int i = 0; i < max; i++)
	{
		dest[i] = (wchar_t)src[i];
		if(src[i] == (unsigned short)L'\0') break;
	}
}

void ConvertWcharTo16Bit(unsigned short* dest, const wchar_t* src, unsigned int max)
{
	for(unsigned int i = 0; i < max; i++)
	{
		dest[i] = (unsigned short)src[i];
		if(src[i] == L'\0') break;
	}
}

void GetStringMap(map<string, string>& lst, const char* input, char delim1, char delim2)
{
	stringstream ss(input);
	string tst;
	while(getline(ss, tst, delim1))
	{
		if(tst.size()>1)
		{
			stringstream ss2(tst);
			string name, value;
			if(getline(ss2, name, '=') && getline(ss2, value, delim2))
			{
				lst[name] = value;
			}
		}
	}
}

void GetStringArray(vector<string>& lst, const char* input, char delim1)
{
	stringstream ss(input);
	string tst;
	while(getline(ss, tst, delim1))
	{
		lst.push_back(tst);
	}
}

void GetStringArray(vector<int>& lst, const char* input, char delim1)
{
	stringstream ss(input);
	string tst;

	while(getline(ss, tst, delim1))
	{
		lst.push_back(atoi(tst.c_str()));
	}
}

void GetStringArray(vector<float>& lst, const char* input, char delim1)
{
	stringstream ss(input);
	string tst;

	while(getline(ss, tst, delim1))
	{
		lst.push_back((float)atof(tst.c_str()));
	}
}

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

void RectangleI::operator+=(const RectangleI& rect)
{
	left += rect.left;
	top += rect.top;
	right += rect.right;
	bottom += rect.bottom;

}

RectangleI RectangleI::operator+(const RectangleI& rect)
{
	RectangleI rc(left,top,right,bottom);
	rc.left += rect.left;
	rc.top += rect.top;
	rc.right += rect.left;
	rc.bottom += rect.top;
	return rc;
}

//

RectangleF::RectangleF()
{
	left=top=right=bottom=0;
}

RectangleF::RectangleF(float left, float top, float right, float bottom)
{
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
}

void RectangleF::operator+=(RectangleF rect)
{
	left += rect.left;
	top += rect.top;
	right += rect.right;
	bottom += rect.bottom;

}

RectangleF RectangleF::operator+(RectangleF rect)
{
	RectangleF rc(left,top,right,bottom);
	rc.left += rect.left;
	rc.top += rect.top;
	rc.right += rect.left;
	rc.bottom += rect.top;
	return rc;
}

//
PointI::PointI()
{
	left=top=0;
}

PointI::PointI(int left, int top)
{
	this->left = left;
	this->top = top;
}

void PointI::operator+=(PointI rect)
{
	left += rect.left;
	top += rect.top;
}

PointI PointI::operator+(PointI rect)
{
	PointI rc(left,top);
	rc.left += rect.left;
	rc.top += rect.top;
	return rc;
}

bool PointI::Intersect(PointI const& point, RectangleF const& rect)
{
	if(point.left >= rect.left && point.left <= rect.right)
	{
		if(point.top >= rect.top && point.top <= rect.bottom) return true;
	}
	return false;
}

PointF::PointF()
{
	left=top=0;
}

PointF::PointF(float left, float top)
{
	this->left = left;
	this->top = top;
}

void PointF::operator+=(PointF rect)
{
	left += rect.left;
	top += rect.top;
}

PointF PointF::operator+(PointF rect)
{
	PointF rc(left,top);
	rc.left += rect.left;
	rc.top += rect.top;
	return rc;
}

bool PointF::Intersect(PointF const& point, RectangleF const& rect)
{
	if(point.left >= rect.left && point.left <= rect.right)
	{
		if(point.top >= rect.top && point.top <= rect.bottom) return true;
	}
	return false;
}

//
Size::Size()
{
	width=height=0;
}

Size::Size(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
}

void Size::operator+=(PointI rect)
{
	width += rect.left;
	height += rect.top;
}

Size Size::operator+(PointI rect)
{
	Size rc(width,height);
	rc.width += rect.left;
	rc.height += rect.top;
	return rc;
}

//color
ColorARGB32::ColorARGB32(unsigned char a, unsigned char r, unsigned char g, unsigned char b)
{
	m_colorA = a;
	m_colorR = r;
	m_colorG = g;
	m_colorB = b;
}

unsigned char ColorARGB32::GetA()
{
	return m_colorA;
}

unsigned char ColorARGB32::GetR()
{
	return m_colorR;
}

unsigned char ColorARGB32::GetG()
{
	return m_colorG;
}

unsigned char ColorARGB32::GetB()
{
	return m_colorB;
}

void ColorARGB32::SetA(unsigned char val)
{
	m_colorA = val;
}

void ColorARGB32::SetR(unsigned char val)
{
	m_colorR = val;
}

void ColorARGB32::SetG(unsigned char val)
{
	m_colorG = val;
}

void ColorARGB32::SetB(unsigned char val)
{
	m_colorB = val;
}

void ColorARGB32::SetColor(unsigned int color)
{
	ColorARGB32 c = *((ColorARGB32*)&color);
	*this = c;
}

ColorARGB32::operator unsigned int()
{
	return *((unsigned int*) this);
}

ColorARGB32::ColorARGB32(unsigned int color)
{
	SetColor(color);
}

void StrToLower(string& str)
{
	for(unsigned int i = 0; i<str.size(); i++)
	{
		str[i] = tolower(str[i]);
	}
}

bool iequals(const string& a, const string& b)
{
    unsigned int sz = a.size();
    if (b.size() != sz)
        return false;
    for (unsigned int i = 0; i < sz; ++i)
        if (tolower(a[i]) != tolower(b[i]))
            return false;
    return true;
}

bool icompstrs::operator() (const string& s1, const string& s2) const
{
	int i = 0;
#ifdef __AVM2__
	i = iequals(s1.c_str(), s2.c_str());
#else
#ifdef ANDROID
	i = strcasecmp(s1.c_str(), s2.c_str());
#else
	i = _strcmpi(s1.c_str(), s2.c_str());
#endif
#endif
	if(i<0) return true;
	return false;
}

float WrapAngle(float angle, float max)
{
	if( angle > 0.0f ) 
	{
		return fmod( angle, max );
	}
	else 
	{
		return (max) - fmod( fabs( angle ), (max) );
	}
}