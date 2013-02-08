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

/**
 * Abstract base input output class.
 * New IO class can be created using this base class and implementing only read and write functions for void*.\n
 * for other read and write functions simply use (using BaseIO::Write; using BaseIO::Read;) in derived class.\n
 * Example for making new IO class :
 * ~~~~~~~~~~~~~~~~{.cpp}
 * class FileIO : public BaseIO
 * {
 * 	File file;
 *
 * public:
 * 	FileIO(ResourceManager* mgr = 0) : BaseIO(mgr) {};
 * 	~FileIO(){};
 *
 * 	virtual bool IsGood()
 * 	{
 * 		return true;
 * 	}
 *
 * 	virtual bool Open(const wchar_t* filename, Access access, CreationFlags flags = Create_OpenAlways, ShareMode share= Share_None)
 * 	{
 * 		file.open(filename);
 * 		return true;
 * 	}
 *
 * 	virtual void Close() {};
 * 	virtual bool Seek(long long val, SeekMode mode, long long* newdest = 0) {rturn true;}
 * 	virtual long long GetPosition() { return 0; }
 * 	virtual long long GetSize() {return 0;}
 *
 * 	virtual bool Write(const void* val, unsigned int* sz = 0, const char* name = 0);
 * 	{
 * 		return file.write(val, sz);
 * 	}
 *
 * 	virtual bool Read(void* val, unsigned int* sz = 0)
 * 	{
 * 		return file.read(val, sz);
 * 	}
 *
 * 	using BaseIO::Write;
 * 	using BaseIO::Read;
 * };
 * ~~~~~~~~~~~~~~~~
 */
class BaseIO
{
public:
	/**
	 * IO Access permissions.
	 */
	enum Access
	{
		Access_Read = 0x01, ///< Read access.
		Access_Write = 0x02,///< Write access.
		Access_ALL = 0x04,  ///< Read and write access.
	};

	/**
	 * IO Creation flags for creating new IO or using new one or deleting old etc.
	 * \note currently usable only for physical files.
	 */
	enum CreationFlags
	{
		Create_OpenExisting = 0x10,///< Open existing file (IO).
		Create_OpenAlways = 0x20,  ///< If file does not exists creates new one else opens old.
		Create_CreateAlways = 0x40,///< Delete old file if exists and create new one.
	};

	/**
	 * Seek mode for moving current IO read/write position.
	 */
	enum SeekMode
	{
		Seek_Set = 0x01,    ///< Set position from beginning.
		Seek_Current = 0x02,///< Set position relative to current position.
		Seek_End = 0x03     ///< Set position relative to end.
	};

	/**
	 * Share mode for sharing IO with other IO.
	 */
	enum ShareMode 
	{ 
		Share_None = 0x00,                                  ///< Don't share with other IOs/softwares.
		Share_Read = 0x01,                                  ///< Read only access to other IOs/softwares.
		Share_Write = 0x02,                                 ///< Write only access to other IOs/softwares.
		Share_Delete = 0x04,                                ///< Allows other IOs/softwares to delete this file.
		Share_All = Share_Read | Share_Write | Share_Delete ///< Full access permission for other IOs/softwares.
	};

protected:
	ResourceManager* m_mgr; ///< Pointer to the resource manager used to load resources used in IO.

public:
	/**
	 * Default constructor.
	 * \param mgr Pointer to the resource manager which will be used to load resources used by this IO.
	 */
	BaseIO(ResourceManager* mgr);

	/**
	 * Opens file.
	 * \param filename Name of the file to open.
	 * \param access Open with these access permissions.
	 * \param flags Open with these file creation flags.
	 * \param share Sharing permissions.
	 * \return true if file is successfully opened else false.
	 */
	virtual bool Open(const wchar_t* filename, Access access, CreationFlags flags = Create_OpenAlways, ShareMode share = Share_None) = 0;

	/**
	 * Closes file handle.
	 */
	virtual void Close() = 0;

	/**
	 * Sets write and read position to new destination.
	 * \param val Where to set position.
	 * \param mode Which seek mode to use.
	 * \param newdest Returns new read and write position (can be set to null if don't want to get new position).
	 * \return true if setting of new position was successful else false.
	 */
	virtual bool Seek(long long val, SeekMode mode, long long* newdest = 0) = 0;

	/**
	 * Returns current read/write position.
	 */
	virtual long long GetPosition() = 0;

	/**
	 * return file size in bytes.
	 */
	virtual long long GetSize() = 0;

	/**
	 * Write specified value using this IO object.
	 * \param val Value
	 * \param sz Size of the value object to write also returns number of bytes written, this can be set to null.
	 * \param name Name of the value/proper currently used only for showing property name in editor use it like "name=property name".
	 * \param hidden Whether this value is visible in properties editor.
	 * \return true if value was written successfully else false.
	 */
	virtual bool Write(const bool& val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);

	/**
	 * Read specified value using this IO object.
	 * \param val Reference to the value object, read value will be stored in this object.
	 * \param sz Size of the value object to read also returns number of bytes read, this can be set to null.
	 * \param hidden Whether this value was visible(written) in properties editor.
	 * \return true if value read operation was successfully else false.
	 */
	virtual bool Read(bool& val, unsigned int* sz = 0, bool hidden = false);

	/** \copydoc BaseIO::Write(const bool&, unsigned int*, const char*, bool) */
	virtual bool Write(const char& val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);
	/** \copydoc BaseIO::Read(bool&, unsigned int*, bool) */
	virtual bool Read(char& val, unsigned int* sz = 0, bool hidden = false);

	/** \copydoc BaseIO::Write(const bool&, unsigned int*, const char*, bool) */
	virtual bool Write(const wchar_t& val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);
	/** \copydoc BaseIO::Read(bool&, unsigned int*, bool) */
	virtual bool Read(wchar_t& val, unsigned int* sz = 0, bool hidden = false);

	/** \copydoc BaseIO::Write(const bool&, unsigned int*, const char*, bool) */
	virtual bool Write(const short& val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);
	/** \copydoc BaseIO::Read(bool&, unsigned int*, bool) */
	virtual bool Read(short& val, unsigned int* sz = 0, bool hidden = false);

	/** \copydoc BaseIO::Write(const bool&, unsigned int*, const char*, bool) */
	virtual bool Write(const unsigned short& val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);
	/** \copydoc BaseIO::Read(bool&, unsigned int*, bool) */
	virtual bool Read(unsigned short& val, unsigned int* sz = 0, bool hidden = false);

	/** \copydoc BaseIO::Write(const bool&, unsigned int*, const char*, bool) */
	virtual bool Write(const int& val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);
	/** \copydoc BaseIO::Read(bool&, unsigned int*, bool) */
	virtual bool Read(int& val, unsigned int* sz = 0, bool hidden = false);

	/** \copydoc BaseIO::Write(const bool&, unsigned int*, const char*, bool) */
	virtual bool Write(const unsigned int& val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);
	/** \copydoc BaseIO::Read(bool&, unsigned int*, bool) */
	virtual bool Read(unsigned int& val, unsigned int* sz = 0, bool hidden = false);

	/** \copydoc BaseIO::Write(const bool&, unsigned int*, const char*, bool) */
	virtual bool Write(const long& val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);
	/** \copydoc BaseIO::Read(bool&, unsigned int*, bool) */
	virtual bool Read(long& val, unsigned int* sz = 0, bool hidden = false);

	/** \copydoc BaseIO::Write(const bool&, unsigned int*, const char*, bool) */
	virtual bool Write(const unsigned long& val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);
	/** \copydoc BaseIO::Read(bool&, unsigned int*, bool) */
	virtual bool Read(unsigned long& val, unsigned int* sz = 0, bool hidden = false);

	/** \copydoc BaseIO::Write(const bool&, unsigned int*, const char*, bool) */
	virtual bool Write(const long long& val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);
	/** \copydoc BaseIO::Read(bool&, unsigned int*, bool) */
	virtual bool Read(long long& val, unsigned int* sz = 0, bool hidden = false);

	/** \copydoc BaseIO::Write(const bool&, unsigned int*, const char*, bool) */
	virtual bool Write(const unsigned long long& val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);
	/** \copydoc BaseIO::Read(bool&, unsigned int*, bool) */
	virtual bool Read(unsigned long long& val, unsigned int* sz = 0, bool hidden = false);

	/** \copydoc BaseIO::Write(const bool&, unsigned int*, const char*, bool) */
	virtual bool Write(const float& val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);
	/** \copydoc BaseIO::Read(bool&, unsigned int*, bool) */
	virtual bool Read(float& val, unsigned int* sz = 0, bool hidden = false);

	/** \copydoc BaseIO::Write(const bool&, unsigned int*, const char*, bool) */
	virtual bool Write(const double& val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);
	/** \copydoc BaseIO::Read(bool&, unsigned int*, bool) */
	virtual bool Read(double& val, unsigned int* sz = 0, bool hidden = false);

	/** \copydoc BaseIO::Write(const bool&, unsigned int*, const char*, bool) */
	virtual bool Write(const char* val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);

	/** \copydoc BaseIO::Write(const bool&, unsigned int*, const char*, bool) */
	virtual bool Write(const wchar_t* val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);

	/** \copydoc BaseIO::Write(const bool&, unsigned int*, const char*, bool) */
	virtual bool Write(const string& val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);
	/** \copydoc BaseIO::Read(bool&, unsigned int*, bool) */
	virtual bool Read(string& val, unsigned int* sz = 0, bool hidden = false);

	/** \copydoc BaseIO::Write(const bool&, unsigned int*, const char*, bool) */
	virtual bool Write(const wstring& val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);
	/** \copydoc BaseIO::Read(bool&, unsigned int*, bool) */
	virtual bool Read(wstring& val, unsigned int* sz = 0, bool hidden = false);

	/**
	 * Write specified value using this IO object (Implement this function in derived class to get all functionality of BaseIO).
	 * \param val Pointer to the value to write.
	 * \param sz Size of the value object to write also returns number of bytes written, this can't be set to null.
	 * \param name Name of the value/proper currently used only for showing property name in editor use it like "name=property name".
	 * \return true if value was written successfully else false.
	 */
	virtual bool Write(const void* val, unsigned int* sz = 0, const char* name = 0) = 0;
	/**
	 * Read specified value using this IO object.
	 * \param val Pointer to the value object, read value will be stored in this object.
	 * \param sz Size of the value object to read also returns number of bytes read, this can be set to null.
	 * \return true if value read operation was successfully else false.
	 */
	virtual bool Read(void* val, unsigned int* sz = 0) = 0;

	/** \copydoc BaseIO::Write(const bool&, unsigned int*, const char*, bool) */
	virtual bool Write(Resource* val, unsigned int* sz = 0, const char* name = 0, bool hidden = false);
	/** \copydoc BaseIO::Read(bool&, unsigned int*, bool) */
	virtual bool Read(Resource* val, unsigned int* sz = 0, bool hidden = false);

	/**
	 * Returns true if IO object is still usable else false.
	 * \return true if IO object is still usable else false.
	 */
	virtual bool IsGood() = 0;

	/**
	 * Virtual destructor.
	 */
	virtual ~BaseIO(){}
};
