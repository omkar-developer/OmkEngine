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

#define FONT_STYLE_LEFTALIGN	0
#define FONT_STYLE_RIGHTALIGN	2
#define FONT_STYLE_CENTERALIGN	1
#define FONT_STYLE_VCENTERALIGN	4
#define FONT_STYLE_TOPALIGN		0
#define FONT_STYLE_BOTTOMALIGN	8
#define FONT_STYLE_MULTILINE	16
#define FONT_STYLE_SINGLELINE	32

struct WaveHeader
{
	char chunkid[4];
	unsigned int chunksize;
	char format[4];
	char subchunkid[4];
	unsigned int subchunksize;
	unsigned short audioformat;
	unsigned short chnnels;
	unsigned int samplerate;
	unsigned int byterate;
	unsigned short blockalign;
	unsigned short bitspersample;
	char data[4];
	unsigned int datasize;
};

// Image
class ImageResource : public IResource
{
private:
	GLuint texture;
	Size m_size;
	char* data;

protected:
	virtual bool LoadFromFile(ResourceManager* mgr);
	virtual void Release();
	virtual void OnReset();
	virtual void OnLost();

public:
	ImageResource();
	virtual void* GetResourcePtr();	
	virtual IResource* Clone();

	Size GetImageSize();
};

void RegisterImageLoader(ResourceManager* mgr);


// Font
class FontResource : public IResource
{
private:
#ifdef USE_BITMAPFONT
	Resource imgset;
	unsigned int fsize;
#else
	ID3DXFont* font;
#endif

protected:
	virtual bool LoadFromFile(ResourceManager* mgr);
	virtual void Release();
	virtual void OnReset();
	virtual void OnLost();

public:
	FontResource();
	virtual void* GetResourcePtr();	
	virtual IResource* Clone();
	unsigned int GetSize();
};

void RegisterFontLoader(ResourceManager* mgr);

#ifdef USE_OGG
class oggFile
{
private:
	OggVorbis_File vf;
	vorbis_info* vi;
	bool opened;

public:
	oggFile();
	vorbis_info* GetInfo();
	bool OpenFile(BaseIO* file);
	void CloseFile();

	bool ReadCycleBuffer(BaseIO* file, char* buff, unsigned int size, const wchar_t* filename);
	bool ReadBuffer(char* ptr, unsigned int* size);
};
#endif

class SoundResource : public IResource
{
private:
#ifdef USE_DIRECTSOUND8
	LPDIRECTSOUNDBUFFER dsb;
#else
	ALuint buff;
#endif

	C_SoundDevice* ds;

protected:
	virtual bool LoadFromFile(ResourceManager* mgr);
	virtual void Release();
	virtual void OnReset();
	virtual void OnLost();

public:
	SoundResource();
	virtual void* GetResourcePtr();	
	virtual IResource* Clone();
	C_SoundDevice* GetSoundDevice();
};

void RegisterSoundLoader(ResourceManager* mgr);