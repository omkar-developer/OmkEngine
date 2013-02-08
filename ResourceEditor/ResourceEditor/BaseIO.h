class BaseIO
{
public:
	enum Access
	{
		Access_Read = 0x01,
		Access_Write = 0x02,
		Access_ALL = 0x04,
	};

	enum CreationFlags
	{
		Create_OpenExisting = 0x10,
		Create_OpenAlways = 0x20,
		Create_CreateAlways = 0x40,
	};

	enum SeekMode
	{
		Seek_Set = 0x01,
		Seek_Current = 0x02,
		Seek_End = 0x03
	};

	enum ShareMode 
	{ 
		Share_None = 0x00,
		Share_Read = 0x01,
		Share_Write = 0x02, 
		Share_Delete = 0x04, 
		Share_All = Share_Read | Share_Write | Share_Delete 
	};

protected:
	ResourceManager* m_mgr;

public:
	BaseIO(ResourceManager* mgr);
	virtual bool Open(const wchar_t* filename, Access access, CreationFlags flags = Create_OpenAlways, ShareMode share = Share_None) = 0;
	virtual void Close() = 0;
	virtual bool Seek(long long val, SeekMode mode, long long* newdest = 0) = 0;
	virtual long long GetPosition() = 0;
	virtual long long GetSize() = 0;

	virtual bool Write(const bool& val, unsigned int* sz = 0, const char* name = 0);
	virtual bool Read(bool& val, unsigned int* sz = 0);

	virtual bool Write(const char& val, unsigned int* sz = 0, const char* name = 0);
	virtual bool Read(char& val, unsigned int* sz = 0);

	virtual bool Write(const wchar_t& val, unsigned int* sz = 0, const char* name = 0);
	virtual bool Read(wchar_t& val, unsigned int* sz = 0);

	virtual bool Write(const short& val, unsigned int* sz = 0, const char* name = 0);
	virtual bool Read(short& val, unsigned int* sz = 0);

	virtual bool Write(const unsigned short& val, unsigned int* sz = 0, const char* name = 0);
	virtual bool Read(unsigned short& val, unsigned int* sz = 0);

	virtual bool Write(const int& val, unsigned int* sz = 0, const char* name = 0);
	virtual bool Read(int& val, unsigned int* sz = 0);

	virtual bool Write(const unsigned int& val, unsigned int* sz = 0, const char* name = 0);
	virtual bool Read(unsigned int& val, unsigned int* sz = 0);

	virtual bool Write(const long& val, unsigned int* sz = 0, const char* name = 0);
	virtual bool Read(long& val, unsigned int* sz = 0);

	virtual bool Write(const unsigned long& val, unsigned int* sz = 0, const char* name = 0);
	virtual bool Read(unsigned long& val, unsigned int* sz = 0);

	virtual bool Write(const long long& val, unsigned int* sz = 0, const char* name = 0);
	virtual bool Read(long long& val, unsigned int* sz = 0);

	virtual bool Write(const unsigned long long& val, unsigned int* sz = 0, const char* name = 0);
	virtual bool Read(unsigned long long& val, unsigned int* sz = 0);

	virtual bool Write(const float& val, unsigned int* sz = 0, const char* name = 0);
	virtual bool Read(float& val, unsigned int* sz = 0);

	virtual bool Write(const double& val, unsigned int* sz = 0, const char* name = 0);
	virtual bool Read(double& val, unsigned int* sz = 0);

	virtual bool Write(const char* val, unsigned int* sz = 0, const char* name = 0);

	virtual bool Write(const wchar_t* val, unsigned int* sz = 0, const char* name = 0);

	virtual bool Write(const string& val, unsigned int* sz = 0, const char* name = 0);
	virtual bool Read(string& val, unsigned int* sz = 0);

	virtual bool Write(const wstring& val, unsigned int* sz = 0, const char* name = 0);
	virtual bool Read(wstring& val, unsigned int* sz = 0);

	virtual bool Write(const void* val, unsigned int* sz = 0, const char* name = 0) = 0;
	virtual bool Read(void* val, unsigned int* sz = 0) = 0;

	virtual bool Write(Resource* val, unsigned int* sz = 0, const char* name = 0);
	virtual bool Read(Resource* val, unsigned int* sz = 0);

	virtual bool IsGood() = 0;
	virtual ~BaseIO(){}
};