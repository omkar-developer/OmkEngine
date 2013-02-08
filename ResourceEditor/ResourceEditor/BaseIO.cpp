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

bool BaseIO::Write(Resource* val, unsigned int* sz, const char* name, bool hidden)
{
	map<string, string> lst;
	GetStringMap(lst, name);

	string restype = "name=";
	restype += lst["name"];
	restype += " Resource Type;help=Resource type (values : image - 1, font - 2, sound - 3, sprite - 4, object - 5, imageset - 6).";
	string resname = "name=";
	resname += lst["name"];
	resname += " Resource Name;help=Name of the resource.";
	if(val->IsValid())
	{
		Write((*val)->GetType(), sz, restype.c_str());
		Write((*val)->GetName(), sz, resname.c_str());
	}
	else
	{
		Write((unsigned int)0, sz, restype.c_str());
		Write((char*)"", sz, resname.c_str());
	}
	return true;
}

bool BaseIO::Read(Resource* val, unsigned int* sz, bool hidden)
{
	string str;
	unsigned int rtype = 0;
	Read(rtype);
	Read(str, sz);
	if(str.size()<1)
	{
		*val = 0;
		return false;
	}
	if(m_mgr==0)
	{
		*val = 0;
		return false;
	}
	m_mgr->GetResource(str.c_str(), rtype, val);
	return val->IsValid();
}

bool BaseIO::Write(const bool& val, unsigned int* sz, const char* name, bool hidden)
{
	unsigned int s = sizeof(bool);
	if(!Write(reinterpret_cast<const void*>(&val), &s, name))
	{
		if(sz!=0) *sz = s;
		return false;
	}
	if(sz!=0) *sz = s;
	return true;
}

bool BaseIO::Read(bool& val, unsigned int* sz, bool hidden)
{
	unsigned int s = sizeof(bool);
	if(!Read(reinterpret_cast<void*>(&val), &s))
	{
		if(sz!=0) *sz = s;
		return false;
	}
	if(sz!=0) *sz = s;
	return true;
}

bool BaseIO::Write(const char& val, unsigned int* sz, const char* name, bool hidden)
{
	unsigned int s = sizeof(char);
	if(!Write(reinterpret_cast<const void*>(&val), &s, name))
	{
		if(sz!=0) *sz = s;
		return false;
	}
	if(sz!=0) *sz = s;
	return true;
}
bool BaseIO::Read(char& val, unsigned int* sz, bool hidden)
{
	unsigned int s = sizeof(char);
	if(!Read(reinterpret_cast<void*>(&val), &s))
	{
		if(sz!=0) *sz = s;
		return false;
	}
	if(sz!=0) *sz = s;
	return true;
}

bool BaseIO::Write(const wchar_t& val, unsigned int* sz, const char* name, bool hidden)
{
	unsigned int s = sizeof(wchar_t);
	if(!Write(reinterpret_cast<const void*>(&val), &s, name))
	{
		if(sz!=0) *sz = s;
		return false;
	}
	if(sz!=0) *sz = s;
	return true;
}
bool BaseIO::Read(wchar_t& val, unsigned int* sz, bool hidden)
{
	unsigned int s = sizeof(wchar_t);
	if(!Read(reinterpret_cast<void*>(&val), &s))
	{
		if(sz!=0) *sz = s;
		return false;
	}
	if(sz!=0) *sz = s;
	return true;
}

bool BaseIO::Write(const short& val, unsigned int* sz, const char* name, bool hidden)
{
	unsigned int s = sizeof(short);
	if(!Write(reinterpret_cast<const void*>(&val), &s, name))
	{
		if(sz!=0) *sz = s;
		return false;
	}
	if(sz!=0) *sz = s;
	return true;
}
bool BaseIO::Read(short& val, unsigned int* sz, bool hidden)
{
	unsigned int s = sizeof(short);
	if(!Read(reinterpret_cast<void*>(&val), &s))
	{
		if(sz!=0) *sz = s;
		return false;
	}
	if(sz!=0) *sz = s;
	return true;
}

bool BaseIO::Write(const unsigned short& val, unsigned int* sz, const char* name, bool hidden)
{
	unsigned int s = sizeof(unsigned short);
	if(!Write(reinterpret_cast<const void*>(&val), &s, name))
	{
		if(sz!=0) *sz = s;
		return false;
	}
	if(sz!=0) *sz = s;
	return true;
}
bool BaseIO::Read(unsigned short& val, unsigned int* sz, bool hidden)
{
	unsigned int s = sizeof(unsigned short);
	if(!Read(reinterpret_cast<void*>(&val), &s))
	{
		if(sz!=0) *sz = s;
		return false;
	}
	if(sz!=0) *sz = s;
	return true;
}

bool BaseIO::Write(const int& val, unsigned int* sz, const char* name, bool hidden)
{
	unsigned int s = sizeof(int);
	if(!Write(reinterpret_cast<const void*>(&val), &s, name))
	{
		if(sz!=0) *sz = s;
		return false;
	}
	if(sz!=0) *sz = s;
	return true;
}
bool BaseIO::Read(int& val, unsigned int* sz, bool hidden)
{
	unsigned int s = sizeof(int);
	if(!Read(reinterpret_cast<void*>(&val), &s))
	{
		if(sz!=0) *sz = s;
		return false;
	}
	if(sz!=0) *sz = s;
	return true;
}

bool BaseIO::Write(const unsigned int& val, unsigned int* sz, const char* name, bool hidden)
{
	unsigned int s = sizeof(unsigned int);
	if(!Write(reinterpret_cast<const void*>(&val), &s, name))
	{
		if(sz!=0) *sz = s;
		return false;
	}
	if(sz!=0) *sz = s;
	return true;
}
bool BaseIO::Read(unsigned int& val, unsigned int* sz, bool hidden)
{
	unsigned int s = sizeof(unsigned int);
	if(!Read(reinterpret_cast<void*>(&val), &s))
	{
		if(sz!=0) *sz = s;
		return false;
	}
	if(sz!=0) *sz = s;
	return true;
}

bool BaseIO::Write(const long& val, unsigned int* sz, const char* name, bool hidden)
{
	unsigned int s = sizeof(long);
	if(!Write(reinterpret_cast<const void*>(&val), &s, name))
	{
		if(sz!=0) *sz = s;
		return false;
	}
	if(sz!=0) *sz = s;
	return true;
}
bool BaseIO::Read(long& val, unsigned int* sz, bool hidden)
{
	unsigned int s = sizeof(long);
	if(!Read(reinterpret_cast<void*>(&val), &s))
	{
		if(sz!=0) *sz = s;
		return false;
	}
	if(sz!=0) *sz = s;
	return true;
}

bool BaseIO::Write(const unsigned long& val, unsigned int* sz, const char* name, bool hidden)
{
	unsigned int s = sizeof(unsigned long);
	if(!Write(reinterpret_cast<const void*>(&val), &s, name))
	{
		if(sz!=0) *sz = s;
		return false;
	}
	if(sz!=0) *sz = s;
	return true;
}
bool BaseIO::Read(unsigned long& val, unsigned int* sz, bool hidden)
{
	unsigned int s = sizeof(unsigned long);
	if(!Read(reinterpret_cast<void*>(&val), &s))
	{
		if(sz!=0) *sz = s;
		return false;
	}
	if(sz!=0) *sz = s;
	return true;
}

bool BaseIO::Write(const long long& val, unsigned int* sz, const char* name, bool hidden)
{
	unsigned int s = sizeof(long long);
	if(!Write(reinterpret_cast<const void*>(&val), &s, name))
	{
		if(sz!=0) *sz = s;
		return false;
	}
	if(sz!=0) *sz = s;
	return true;
}
bool BaseIO::Read(long long& val, unsigned int* sz, bool hidden)
{
	unsigned int s = sizeof(long long);
	if(!Read(reinterpret_cast<void*>(&val), &s))
	{
		if(sz!=0) *sz = s;
		return false;
	}
	if(sz!=0) *sz = s;
	return true;
}

bool BaseIO::Write(const unsigned long long& val, unsigned int* sz, const char* name, bool hidden)
{
	unsigned int s = sizeof(unsigned long long);
	if(!Write(reinterpret_cast<const void*>(&val), &s, name))
	{
		if(sz!=0) *sz = s;
		return false;
	}
	if(sz!=0) *sz = s;
	return true;
}
bool BaseIO::Read(unsigned long long& val, unsigned int* sz, bool hidden)
{
	unsigned int s = sizeof(unsigned long long);
	if(!Read(reinterpret_cast<void*>(&val), &s))
	{
		if(sz!=0) *sz = s;
		return false;
	}
	if(sz!=0) *sz = s;
	return true;
}

bool BaseIO::Write(const float& val, unsigned int* sz, const char* name, bool hidden)
{
	unsigned int s = sizeof(float);
	if(!Write(reinterpret_cast<const void*>(&val), &s, name))
	{
		if(sz!=0) *sz = s;
		return false;
	}
	if(sz!=0) *sz = s;
	return true;
}
bool BaseIO::Read(float& val, unsigned int* sz, bool hidden)
{
	unsigned int s = sizeof(float);
	if(!Read(reinterpret_cast<void*>(&val), &s))
	{
		if(sz!=0) *sz = s;
		return false;
	}
	if(sz!=0) *sz = s;
	return true;
}

bool BaseIO::Write(const double& val, unsigned int* sz, const char* name, bool hidden)
{
	unsigned int s = sizeof(double);
	if(!Write(reinterpret_cast<const void*>(&val), &s, name))
	{
		if(sz!=0) *sz = s;
		return false;
	}
	if(sz!=0) *sz = s;
	return true;
}
bool BaseIO::Read(double& val, unsigned int* sz, bool hidden)
{
	unsigned int s = sizeof(double);
	if(!Read(reinterpret_cast<void*>(&val), &s))
	{
		if(sz!=0) *sz = s;
		return false;
	}
	if(sz!=0) *sz = s;
	return true;
}

bool BaseIO::Write(const char* val, unsigned int* sz, const char* name, bool hidden)
{
	unsigned int s = 0;
	unsigned int size = 0;
	unsigned int write;
	string str = val;
	size = str.size();
	Write(size, &write);
	if(write == 0) return false;
	s = size * sizeof(char);
	if(!Write(reinterpret_cast<const void*>(val), &s, name))
	{
		if(sz!=0) *sz = s;
		return false;
	}
	if(sz!=0) *sz = s;
	return true;
}

bool BaseIO::Write(const wchar_t* val, unsigned int* sz, const char* name, bool hidden)
{
	unsigned int s = 0;
	unsigned int size = 0;
	unsigned int write;
	wstring str = val;
	size = str.size();
	Write(size, &write);
	if(write == 0) return false;
	s = sizeof(unsigned short) * size;

	unsigned short* tchr = new unsigned short[size];
	ConvertWcharTo16Bit(tchr, val, size);

	if(!Write(reinterpret_cast<const void*>(tchr), &s, name))
	{
		delete [] tchr;
		if(sz!=0) *sz = s;
		return false;
	}
	delete [] tchr;
	if(sz!=0) *sz = s;
	return true;
}

bool BaseIO::Write(const string& val, unsigned int* sz, const char* name, bool hidden)
{
	return Write(val.c_str(), sz, name);
}

bool BaseIO::Read(string& val, unsigned int* sz, bool hidden)
{
	unsigned int s = 0;
	unsigned int size = 0;
	unsigned int read;
	size = 0;
	Read(size, &read);
	if(read == 0) return false;
	read = (size);
	char* ch = new char[read + 1];
	s = sizeof(char) * read;
	if(!Read(reinterpret_cast<void*>(ch), &s))
	{
		delete [] ch;
		return false;
	}
	ch[read] = '\0';
	val = ch;
	delete [] ch;
	if(sz!=0) *sz = s;
	return true;
}

bool BaseIO::Write(const wstring& val, unsigned int* sz, const char* name, bool hidden)
{
	return Write(val.c_str(), sz, name);
}

bool BaseIO::Read(wstring& val, unsigned int* sz, bool hidden)
{
	unsigned int s = 0;
	unsigned int size = 0;
	unsigned int read;
	size = 0;
	Read(size, &read);
	if(read == 0) return false;
	read = (size);
	wchar_t* ch = new wchar_t[read + 1];
	s = sizeof(unsigned short) * read;
	unsigned short* chw = new unsigned short[read + 1];
	if(!Read(reinterpret_cast<void*>(chw), &s))
	{
		delete [] chw;
		delete [] ch;
		return false;
	}
	chw[read] = L'\0';
	Convert16BitWchar(ch, chw, read + 1);
	delete [] chw;
	val = ch;
	delete [] ch;
	if(sz!=0) *sz = s;
	return true;
}

BaseIO::BaseIO( ResourceManager* mgr )
{
	m_mgr = mgr;
}