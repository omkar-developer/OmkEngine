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
 
#include "CDialogs.h"
#ifndef USE_FSTREAM
#ifdef WIN32

C_File::C_File(ResourceManager* mgr) : BaseIO(mgr)
{
	m_handle = INVALID_HANDLE_VALUE;
}

C_File::~C_File()
{
	Close();
}

HANDLE C_File::GetHandle()
{
	return m_handle;
}

bool C_File::IsGood()
{
	if(m_handle==INVALID_HANDLE_VALUE) return false;
	return true;
}

bool C_File::Open(const wchar_t* filename, Access access, CreationFlags flags, ShareMode share)
{
	Close();

	DWORD accesst = 0;
	DWORD sharemode = 0;
	DWORD options = 0;

	if(access&Access_Read) accesst |= GENERIC_READ;
	if(access&Access_Write) accesst |= GENERIC_WRITE;
	if(access&Access_ALL) accesst |= GENERIC_ALL;

	if(flags&Create_CreateAlways) options = CREATE_ALWAYS;
	else if(flags&Create_OpenAlways) options = OPEN_ALWAYS;
	else if(flags&Create_OpenExisting) options = OPEN_EXISTING;
	
	m_handle = CreateFile(filename, accesst, sharemode, NULL, options, FILE_ATTRIBUTE_NORMAL, NULL);
	if(m_handle == INVALID_HANDLE_VALUE) return false;
	return true;
}

void C_File::Close()
{
	if(m_handle!=INVALID_HANDLE_VALUE) 
	{
		CloseHandle(m_handle);
		m_handle = INVALID_HANDLE_VALUE;
	}
}

bool C_File::Seek(long long val, SeekMode mode, long long* newdest)
{
	if(m_handle!=INVALID_HANDLE_VALUE) 
	{
		LARGE_INTEGER dest, ndest;
		ndest.QuadPart = 0;
		dest.QuadPart = val;

		unsigned int mmode = 0;
		if(mode==Seek_Set) mmode = FILE_BEGIN;
		else if(mode==Seek_Current) mmode = FILE_CURRENT;
		else if(mode==Seek_End) mmode = FILE_END;

		if(SetFilePointerEx(m_handle, dest, &ndest, mmode))
		{
			if(newdest!=0) *newdest = ndest.QuadPart;
			return true;
		}
	}
	return false;
}

long long C_File::GetPosition()
{
	if(m_handle!=INVALID_HANDLE_VALUE) 
	{
		LARGE_INTEGER dest, ndest;
		ndest.QuadPart = 0;
		dest.QuadPart = 0;

		unsigned int mmode = 0;
		mmode = FILE_CURRENT;

		if(SetFilePointerEx(m_handle, dest, &ndest, mmode))
		{
			return ndest.QuadPart;
		}
	}
	return 0;
}

long long C_File::GetSize()
{
	if(m_handle!=INVALID_HANDLE_VALUE) 
	{
		LARGE_INTEGER sz;
		sz.QuadPart = 0;
		if(GetFileSizeEx(m_handle, &sz))
		{
			return sz.QuadPart;
		}
	}
	return 0;
}

bool C_File::Write(const void* val, unsigned int* sz, const char* name)
{
	if(sz==0) return false;
	DWORD s = 0;
	if(!WriteFile(m_handle, reinterpret_cast<const void*>(val), *sz, &s, NULL))
	{
		if(sz!=0) *sz = s;
		return false;
	}
	*sz = s;
	return true;
}

bool C_File::Read(void* val, unsigned int* sz)
{
	if(sz==0) return false;
	DWORD s = 0;
	if(!ReadFile(m_handle, reinterpret_cast<void*>(val), *sz, &s, NULL))
	{
		if(sz!=0) *sz = s;
		return false;
	}
	*sz = s;
	return true;
}

#endif
#else

C_File::C_File(ResourceManager* mgr) : BaseIO(mgr)
{
	
}

C_File::~C_File()
{
	Close();
}

fstream& C_File::GetHandle()
{
	return m_handle;
}

bool C_File::IsGood()
{
	if(!m_handle.good()) return false;
	return true;
}

bool C_File::Open(const wchar_t* filename, Access access, CreationFlags flags, ShareMode share)
{
	Close();

	int accesst = 0;

	if(access&Access_Read) accesst |= ios::in;
	if(access&Access_Write) accesst |= ios::out;
	if(access&Access_ALL) accesst |= ios::in | ios::out;

	if(flags&Create_CreateAlways) accesst |= ios::trunc;
	
	char tmp[256];
	WideCharToMultiByte(tmp, filename, 256);
	m_handle.open(tmp, accesst | ios::binary);
		
	if(!m_handle.good()) return false;
	return true;
}

void C_File::Close()
{
	m_handle.close();
}

bool C_File::Seek(long long val, SeekMode mode, long long* newdest)
{
	if(m_handle.good()) 
	{
		ios_base::seekdir mmode = 0;
		if(mode==Seek_Set) mmode = ios::beg;
		else if(mode==Seek_Current) mmode = ios::cur;
		else if(mode==Seek_End) mmode = ios::end;

		m_handle.seekp(m_handle.tellg(), ios::beg);

		m_handle.seekg(val, mode);
		m_handle.seekp(val, mode);

		if(!m_handle.fail())
		{
			if(newdest!=0) *newdest = m_handle.tellg();
			return true;
		}
	}
	return false;
}

long long C_File::GetPosition()
{
	return m_handle.tellg();
}

long long C_File::GetSize()
{
	long pos = (long)m_handle.tellg();
	m_handle.seekg(0, ios::end);
	long end = (long)m_handle.tellg();
	m_handle.seekg(pos, ios::beg);
	return end;
}

bool C_File::Write(const void* val, unsigned int* sz, const char* name)
{
	if(sz==0) return false;
	unsigned int s = 0;

	unsigned int ps = (unsigned int)m_handle.tellp();
	m_handle.write((char*)val, *sz);
	m_handle.seekg(m_handle.tellp(), ios::beg);
	s = (unsigned int)m_handle.tellp() - ps;

	if(m_handle.fail())
	{		
		s = *sz;
		if(sz!=0) *sz = s;
		return false;
	}
	*sz = s;
	return true;
}

bool C_File::Read(void* val, unsigned int* sz)
{
	if(sz==0) return false;
	unsigned int s = 0;

	unsigned int ps = (unsigned int)m_handle.tellg();
	m_handle.read((char*)val, *sz);
	m_handle.seekp(m_handle.tellg(), ios::beg);
	s = (unsigned int)m_handle.tellg() - ps;

	if(m_handle.fail())
	{		
		s = *sz;
		if(sz!=0) *sz = s;
		return false;
	}
	*sz = s;
	return true;
}

#endif