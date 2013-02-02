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

unzFile ZipFile::m_file = 0;
unz_file_info ZipFile::m_info;

ZipFile::ZipFile(ResourceManager* mgr) : BaseIO(mgr)
{
	//m_file = 0;
}

ZipFile::~ZipFile()
{
	Close();
}

unzFile ZipFile::GetHandle()
{
	return m_file;
}

bool ZipFile::IsGood()
{
	if(m_file==0) return false;
	return true;
}

void ZipFile::GetFilesList(set<string>& list)
{
	if(m_file==0) return;
	if(unzGoToFirstFile(m_file))
	{
		return;
	}

	while(1)
	{
		unz_file_info info;
		char tmp[256];
		unzGetCurrentFileInfo(m_file, &info, tmp, 256, 0, 0, 0, 0);
		list.insert(tmp);

		if(unzGoToNextFile(m_file))
		{
			return;
		}
	}
}

bool ZipFile::Open(const wchar_t* filename, Access access, CreationFlags flags, ShareMode share)
{
	if(m_file==0) return false;
	Close();

	char tmp[256];
	WideCharToMultiByte(tmp, filename, 256);

	string s;

#ifdef ANDROID
	s = "assets/";
#endif

	s += tmp;

	if(unzLocateFile(m_file, s.c_str(), 1))
	{
		return false;
	}
	
	unzGetCurrentFileInfo(m_file, &m_info, tmp, 256, 0, 0, 0, 0);
	if(unzOpenCurrentFile(m_file)!=UNZ_OK) return false;
	return true;
}

void ZipFile::Close()
{
	if(m_file!=0) 
	{
		unzCloseCurrentFile(m_file);
	}
}

bool ZipFile::Seek(long long val, SeekMode mode, long long* newdest)
{
	//not implimented
	return false;
}

long long ZipFile::GetPosition()
{
	if(m_file!=0) 
	{
		/*unz_file_pos pos;
		unzGetFilePos(m_file, &pos);
		uLong pos2 = unzGetOffset(m_file);
		unzCloseCurrentFile(m_file);
		unzOpenCurrentFile(m_file);

		C_Timer tm;
		tm.Start();
		char buff;
		for(unsigned int i = 0; i<10000000; i++)
		{
			unzReadCurrentFile(m_file, &buff, 1);
		}
		float endt = tm.Reset();

		pos2 = unztell(m_file);
		uLong pos3 = unzGetOffset(m_file);
		return pos2 - pos.pos_in_zip_directory;*/
		return unztell(m_file);
	}
	return 0;
}

long long ZipFile::GetSize()
{
	if(m_file!=0) 
	{
		return m_info.uncompressed_size;
	}
	return 0;
}

bool ZipFile::Write(const void* val, unsigned int* sz, const char* name)
{
	if(m_file==0) return false;
	if(sz==0) return false;
	int s = 0;
	//not implimented
	*sz = s;
	return false;
}

bool ZipFile::Read(void* val, unsigned int* sz)
{
	if(m_file==0) return false;
	if(sz==0) return false;
	int s = 0;
	s = unzReadCurrentFile(m_file, (voidp)val, *sz);
	if(s<=0)
	{
		if(sz!=0) *sz = 0;
		return false;
	}
	*sz = s;
	return true;
}

bool ZipFile::eof()
{
	if(m_file==0) return false;
	return (0>=unzeof(m_file));
}

bool ZipFile::OpenZipFile(const wchar_t* filename)
{
	CloseZipFile();
	char tmp[256];
	WideCharToMultiByte(tmp, filename, 256);
	m_file = unzOpen(tmp);
	if(m_file==0) return false;
	return true;
}

bool ZipFile::OpenZipFile(const char* filename)
{
	CloseZipFile();
	m_file = unzOpen(filename);
	if(m_file==0) return false;
	return true;
}

void ZipFile::CloseZipFile()
{
	if(m_file!=0)
	{
		unzClose(m_file);
		m_file = 0;
	}
}



//zipfile2

unzFile ZipFile2::m_file = 0;
unz_file_info ZipFile2::m_info;

ZipFile2::ZipFile2(ResourceManager* mgr) : BaseIO(mgr)
{
	//m_file = 0;
}

ZipFile2::~ZipFile2()
{
	Close();
}

unzFile ZipFile2::GetHandle()
{
	return m_file;
}

bool ZipFile2::IsGood()
{
	if(m_file==0) return false;
	return true;
}

bool ZipFile2::Open(const wchar_t* filename, Access access, CreationFlags flags, ShareMode share)
{
	if(m_file==0) return false;
	Close();

	char tmp[256];
	WideCharToMultiByte(tmp, filename, 256);

	string s;

#ifdef ANDROID
	s = "assets/";
#endif

	s += tmp;

	if(unzLocateFile(m_file, s.c_str(), 1))
	{
		return false;
	}

	unzGetCurrentFileInfo(m_file, &m_info, tmp, 256, 0, 0, 0, 0);
	if(unzOpenCurrentFile(m_file)!=UNZ_OK) return false;
	return true;
}

void ZipFile2::Close()
{
	if(m_file!=0) 
	{
		unzCloseCurrentFile(m_file);
	}
}

bool ZipFile2::Seek(long long val, SeekMode mode, long long* newdest)
{
	//not implimented
	return false;
}

long long ZipFile2::GetPosition()
{
	if(m_file!=0) 
	{
		/*unz_file_pos pos;
		unzGetFilePos(m_file, &pos);
		uLong pos2 = unzGetOffset(m_file);
		unzCloseCurrentFile(m_file);
		unzOpenCurrentFile(m_file);

		C_Timer tm;
		tm.Start();
		char buff;
		for(unsigned int i = 0; i<10000000; i++)
		{
			unzReadCurrentFile(m_file, &buff, 1);
		}
		float endt = tm.Reset();

		pos2 = unztell(m_file);
		uLong pos3 = unzGetOffset(m_file);
		return pos2 - pos.pos_in_zip_directory;*/
		return unztell(m_file);
	}
	return 0;
}

long long ZipFile2::GetSize()
{
	if(m_file!=0) 
	{
		return m_info.uncompressed_size;
	}
	return 0;
}

bool ZipFile2::Write(const void* val, unsigned int* sz, const char* name)
{
	if(m_file==0) return false;
	if(sz==0) return false;
	int s = 0;
	//not implimented
	*sz = s;
	return false;
}

bool ZipFile2::Read(void* val, unsigned int* sz)
{
	if(m_file==0) return false;
	if(sz==0) return false;
	int s = 0;
	s = unzReadCurrentFile(m_file, (voidp)val, *sz);
	if(s<=0)
	{
		if(sz!=0) *sz = 0;
		return false;
	}
	*sz = s;
	return true;
}

bool ZipFile2::eof()
{
	if(m_file==0) return false;
	return (0>=unzeof(m_file));
}

bool ZipFile2::OpenZipFile(const wchar_t* filename)
{
	CloseZipFile();
	char tmp[256];
	WideCharToMultiByte(tmp, filename, 256);
	m_file = unzOpen(tmp);
	if(m_file==0) return false;
	return true;
}

bool ZipFile2::OpenZipFile(const char* filename)
{
	CloseZipFile();
	m_file = unzOpen(filename);
	if(m_file==0) return false;
	return true;
}

void ZipFile2::CloseZipFile()
{
	if(m_file!=0)
	{
		unzClose(m_file);
		m_file = 0;
	}
}