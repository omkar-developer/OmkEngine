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

class PropertiesIO : public BaseIO
{
private:
	wxPropertyGridManager* pmgr;
	wxPGVIterator it;
	long long pos;

public:
	PropertiesIO(ResourceManager* mgr = 0);
	~PropertiesIO();

	wxPropertyGridManager* GetPropertyGridManager();
	void SetPropertyGridManager(wxPropertyGridManager* val);
	virtual bool IsGood();

	virtual bool Open(const wchar_t* filename = 0, Access access = Access_ALL, CreationFlags flags = Create_OpenAlways, ShareMode share= Share_None);
	virtual void Close();
	virtual bool Seek(long long val, SeekMode mode, long long* newdest = 0);
	virtual long long GetPosition();
	virtual long long GetSize();

	virtual bool Write(const bool& val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);
	virtual bool Read(bool& val, unsigned int* sz = 0, bool hidden = false);

	virtual bool Write(const char& val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);
	virtual bool Read(char& val, unsigned int* sz = 0, bool hidden = false);

	virtual bool Write(const wchar_t& val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);
	virtual bool Read(wchar_t& val, unsigned int* sz = 0, bool hidden = false);

	virtual bool Write(const short& val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);
	virtual bool Read(short& val, unsigned int* sz = 0, bool hidden = false);

	virtual bool Write(const unsigned short& val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);
	virtual bool Read(unsigned short& val, unsigned int* sz = 0, bool hidden = false);

	virtual bool Write(const int& val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);
	virtual bool Read(int& val, unsigned int* sz = 0, bool hidden = false);

	virtual bool Write(const unsigned int& val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);
	virtual bool Read(unsigned int& val, unsigned int* sz = 0, bool hidden = false);

	virtual bool Write(const long& val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);
	virtual bool Read(long& val, unsigned int* sz = 0, bool hidden = false);

	virtual bool Write(const unsigned long& val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);
	virtual bool Read(unsigned long& val, unsigned int* sz = 0, bool hidden = false);

	virtual bool Write(const long long& val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);
	virtual bool Read(long long& val, unsigned int* sz = 0, bool hidden = false);

	virtual bool Write(const unsigned long long& val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);
	virtual bool Read(unsigned long long& val, unsigned int* sz = 0, bool hidden = false);

	virtual bool Write(const float& val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);
	virtual bool Read(float& val, unsigned int* sz = 0, bool hidden = false);

	virtual bool Write(const double& val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);
	virtual bool Read(double& val, unsigned int* sz = 0, bool hidden = false);

	virtual bool Write(const char* val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);
	virtual bool Write(const wchar_t* val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);

	virtual bool Write(const string& val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);
	virtual bool Read(string& val, unsigned int* sz = 0, bool hidden = false);

	virtual bool Write(const wstring& val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);
	virtual bool Read(wstring& val, unsigned int* sz = 0, bool hidden = false);

	virtual bool Write(const void* val, unsigned int* sz = 0, const char* name = 0);
	virtual bool Read(void* val, unsigned int* sz = 0);

	virtual bool Write(Resource* val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);
	virtual bool Read(Resource* val, unsigned int* sz = 0, bool hidden = false);
};