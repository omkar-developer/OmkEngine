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

UIWString::UIWString(const char* str)
{
	tmp.clear();
	unsigned int i=0;
	do 
	{
		tmp.push_back(str[i]);		
	}while(str[i++]!=L'\0');
}

UIWString::UIWString(const wchar_t* str)
{
	tmp.clear();
	unsigned int i=0;
	do 
	{
		tmp.push_back(str[i]);		
	}while(str[i++]!=L'\0');

}

UIWString::UIWString(const wchar_t str)
{
	tmp.clear();
	tmp.push_back(str);
	tmp.push_back(L'\0');
}

UIWString::UIWString(const long str)
{
	wstring tm;
	wstringstream stm;
	stm << str;
	tm = stm.str();
	tm += L'\0';
	tmp.clear();
	unsigned int i=0;
	do 
	{
		tmp.push_back(tm[i]);
	}while(tm[i++]!=L'\0');
}

UIWString::UIWString(const unsigned long str)
{
	wstring tm;
	wstringstream stm;
	stm << str;
	tm = stm.str();
	tm += L'\0';
	tmp.clear();
	unsigned int i=0;

	do 
	{
		tmp.push_back(tm[i]);
	}while(tm[i++]!=L'\0');
}

UIWString::UIWString(const int str)
{
	wstring tm;
	wstringstream stm;
	stm << str;
	tm = stm.str().c_str();
	tm += L'\0';
	tmp.clear();
	unsigned int i=0;

	do 
	{
		tmp.push_back(tm[i]);
	}while(tm[i++]!=L'\0');
}

UIWString::UIWString(const double str)
{
	wstring tm;
	wstringstream stm;
	stm << str;
	tm = stm.str();
	tm += L'\0';
	tmp.clear();
	unsigned int i=0;

	do 
	{
		tmp.push_back(tm[i]);
	}while(tm[i++]!=L'\0');
}

void UIWString::AttachString(UIWString str)
{
	tmp.erase(tmp.begin()+tmp.size()-1);

	unsigned int i=0;

	for(unsigned int i=0; i<str.Length(); i++)
	{
		tmp.push_back(str.GetPtr()[i]);		
	}
}

void UIWString::AddString(UIWString str)
{
	tmp.clear();

	unsigned int i=0;

	for(unsigned int i=0; i<str.Length(); i++)
	{
		tmp.push_back(str.GetPtr()[i]);		
	}
}

void UIWString::InsertString(UIWString str, unsigned int index)
{
	if(index>=Length()) return;

	unsigned int i=0;

	for(unsigned int i=0; i<str.Length()-1; i++)
	{
		tmp.insert(tmp.begin()+index+i, str.GetPtr()[i]);		
	}
}

void UIWString::DeleteString(unsigned int index, unsigned int length)
{
	if(index+length>=Length()) return;

	unsigned int i=0;

	for(unsigned int i=0; i<length; i++)
	{
		tmp.erase(tmp.begin()+index);		
	}
}

void UIWString::Clear()
{
	tmp.clear();
	tmp.push_back(L'\0');
}

unsigned int UIWString::Length()
{
	return tmp.size();
}

wchar_t* UIWString::GetPtr()
{
	return &tmp[0];
}

bool UIWString::operator==(UIWString str)
{
	unsigned int length = Length();
	if(length!=str.Length()) return 0;

	for(unsigned int i=0; i<length; i++)
	{
		if(tmp[i]!=str[i]) return 0;	
	}
	return 1;
}

bool UIWString::operator<(const UIWString& str) const
{
	return wcscmp(&tmp[0], str) < 0;
}

UIWString UIWString::operator+=(UIWString str)
{
	AttachString(str);
	return *this;
}

UIWString::operator wchar_t*()
{
	return &tmp[0];
}

UIWString::operator wchar_t*() const
{
	return (wchar_t*)&tmp[0];
}