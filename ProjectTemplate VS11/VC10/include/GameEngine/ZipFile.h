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

class ZipFile : public BaseIO
{
private:
	static unzFile m_file;
	static unz_file_info m_info;

public:
	ZipFile(ResourceManager* mgr = 0);
	virtual ~ZipFile();

	unzFile GetHandle();
	virtual bool IsGood();

	virtual bool Open(const wchar_t* filename, Access access = BaseIO::Access_ALL, CreationFlags flags = Create_OpenAlways, ShareMode share= Share_None);
	virtual void Close();
	virtual bool Seek(long long val, SeekMode mode, long long* newdest = 0);
	virtual long long GetPosition();
	virtual long long GetSize();

	virtual bool Write(const void* val, unsigned int* sz = 0, const char* name = 0);
	virtual bool Read(void* val, unsigned int* sz = 0);

	using BaseIO::Write;
	using BaseIO::Read;

	static bool OpenZipFile(const wchar_t* filename);
	static bool OpenZipFile(const char* filename);
	static void CloseZipFile();
	bool eof();

	static void GetFilesList(set<string>& list);
};


class ZipFile2 : public BaseIO
{
private:
	static unzFile m_file;
	static unz_file_info m_info;

public:
	ZipFile2(ResourceManager* mgr = 0);
	~ZipFile2();

	unzFile GetHandle();
	virtual bool IsGood();

	virtual bool Open(const wchar_t* filename, Access access = BaseIO::Access_ALL, CreationFlags flags = Create_OpenAlways, ShareMode share= Share_None);
	virtual void Close();
	virtual bool Seek(long long val, SeekMode mode, long long* newdest = 0);
	virtual long long GetPosition();
	virtual long long GetSize();

	virtual bool Write(const void* val, unsigned int* sz = 0, const char* name = 0);
	virtual bool Read(void* val, unsigned int* sz = 0);

	using BaseIO::Write;
	using BaseIO::Read;

	static bool OpenZipFile(const wchar_t* filename);
	static bool OpenZipFile(const char* filename);
	static void CloseZipFile();
	bool eof();
};