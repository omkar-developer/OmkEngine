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

ImageResource::ImageResource()
{
	texture = 0;
}

Size ImageResource::GetImageSize()
{
	if(texture!=0)
	{
		D3DSURFACE_DESC desc;
		texture->GetLevelDesc(0, &desc);
		return Size(desc.Width, desc.Height);
	}
	return Size(0, 0);
}

bool ImageResource::LoadFromFile(ResourceManager* mgr)
{
	if(texture!=0)Release();
	if(mgr->GetDevice()!=0)
	{
		ZipFile file;
		if(!file.Open(this->GetFileName())) return false;

		unsigned int sz = (unsigned int)file.GetSize();
		char* buff = new char[sz];
		if(!file.Read((void*)buff, &sz))
		{
			delete [] buff;
			return false;
		}

		if(SUCCEEDED(D3DXCreateTextureFromFileInMemory(mgr->GetDevice()->GetDevice(), (LPCVOID)buff, sz, &texture)))
		{
			delete [] buff;
			return true;
		}
	}
	return false;
}

void ImageResource::Release()
{
	if(texture!=0)
	{
		texture->Release();
		texture = 0;
	}
}

void* ImageResource::GetResourcePtr()
{
	return texture;
}

IResource* ImageResource::Clone()
{	
	return new ImageResource();
}

void ImageResource::OnReset()
{
}

void ImageResource::OnLost()
{
}

void RegisterImageLoader(ResourceManager* mgr)
{
	mgr->RegisterLoader(RES_TYPE_IMAGE, new ImageResource());
}

//sound

#ifdef USE_OGG

size_t readfunc(void *ptr, size_t size, size_t nmemb, void *datasource)
{
	BaseIO* file = (BaseIO*)datasource;
	unsigned int sz = nmemb * size;
	file->Read(ptr, &sz);
	return sz;
}

oggFile::oggFile()
{
	vi = 0;
	opened = false;
}

vorbis_info* oggFile::GetInfo()
{
	if(opened) return vi;
	else return 0;
}

bool oggFile::OpenFile(BaseIO* file)
{
	CloseFile();

	ov_callbacks callb;
	callb.read_func = readfunc;
	callb.close_func = 0;
	callb.seek_func = 0;
	callb.tell_func = 0;

	if(ov_open_callbacks(file, &vf, 0, 0, callb)<0)
	{
		opened = false;
		return false;
	}

	vi = ov_info(&vf, -1);
	if(vi==0)
	{
		opened = false;
		ov_clear(&vf);
		return false;
	}

	ogg_int64_t len = ov_pcm_total(&vf, -1);
	if((len * 2)>MAXSBUFF)
	{
		ov_clear(&vf);
		opened = false;
		return false;
	}

	opened = true;
	return true;
}

void oggFile::CloseFile()
{
	if(opened == true)
	{
		opened = false;
		ov_clear(&vf);
	}
}

bool oggFile::ReadCycleBuffer(BaseIO* file, char* buff, unsigned int size, const wchar_t* filename)
{
	unsigned int read = 0;
	unsigned int sz = size;
	bool wrapped = false;

	while((read+sz)<=size)
	{
		if(!ReadBuffer(buff, &sz))
		{
			if(!wrapped)
			{
				file->Close();
				file->Open(filename, BaseIO::Access_Read);
				CloseFile();
				OpenFile(file);
				wrapped = true;
			}
			else return false;
		}

		read += sz;

		if(read<size)
		{
			file->Close();
			file->Open(filename, BaseIO::Access_Read);
			CloseFile();
			OpenFile(file);
			buff += read;
			sz = size - sz;
		}
		else
		{
			break;
		}
	}

	return true;
}

bool oggFile::ReadBuffer(char* ptr, unsigned int* size)
{
	if(size==0) return false;
	unsigned int pos = 0;
	int bread = 1;
	int sec = 0;
	while(bread && pos < *size)
	{
		bread = ov_read(&vf, ptr+pos, *size - pos, 0, 2, 1, &sec);
		if(bread==0)
		{
			if(pos!=0) break;
			else return false;
		}
		pos+=bread;
	}
	*size = pos;
	return true;
}
#endif

C_SoundDevice* SoundResource::GetSoundDevice()
{
	return ds;
}

bool SoundResource::LoadFromFile(ResourceManager* mgr)
{
#ifdef USE_DIRECTSOUND8
	if(dsb!=0)Release();
#else
	if(buff!=0)Release();
#endif

	if(mgr->GetSoundDevice()!=0 && mgr->GetSoundDevice()->GetDevice()!=0)
	{
		ZipFile file;
		if(!file.Open(GetFileName())) return false;

		if(file.GetSize()>MAXSBUFF)
		{
			file.Close();
			ds = mgr->GetSoundDevice();
			return true;
		}

#ifdef USE_OGG
		oggFile oggf;
		
		if(!oggf.OpenFile(&file))
		{
			file.Close();
			ds = mgr->GetSoundDevice();
			return false;
		}

		vorbis_info* vi = oggf.GetInfo();

#ifdef USE_DIRECTSOUND8
		WAVEFORMATEX wfmt;
		ZeroMemory(&wfmt, sizeof(WAVEFORMATEX));

		wfmt.cbSize = sizeof(WAVEFORMATEX);
		wfmt.wBitsPerSample = 16;
		wfmt.nChannels = vi->channels;
		wfmt.nSamplesPerSec = vi->rate;
		wfmt.wFormatTag = 1;
		wfmt.nBlockAlign = 2*vi->channels;
		wfmt.nAvgBytesPerSec = 2*wfmt.nSamplesPerSec*wfmt.nChannels;
#else
		ALuint format;
		
		format = (vi->channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
		alGenBuffers(1, &buff);
#endif

		char* tbuff = new char[MAXSBUFF];
		unsigned int size = MAXSBUFF;

		if(!oggf.ReadBuffer(tbuff, &size))
		{
			delete [] tbuff;
			oggf.CloseFile();
			file.Close();
			return false;
		}
		
		oggf.CloseFile();
		file.Close();

#ifdef USE_DIRECTSOUND8
		DSBUFFERDESC desc;
		ZeroMemory(&desc, sizeof(DSBUFFERDESC));
		desc.dwFlags |= DSBCAPS_CTRLVOLUME;

		desc.dwSize = sizeof(desc);
		desc.lpwfxFormat = &wfmt;
		desc.dwBufferBytes = size;
		mgr->GetSoundDevice()->GetDevice()->CreateSoundBuffer(&desc, &dsb, 0);
		char* bf;
		DWORD size2 = size;
		dsb->Lock(0, DSBSIZE_MAX, (LPVOID*)&bf, &size2, 0, 0, DSBLOCK_ENTIREBUFFER);
		memcpy(bf, tbuff, size2);
		dsb->Unlock((LPVOID)bf, size2, NULL,NULL);
#else
		alBufferData(buff,format,tbuff,size,vi->rate);
#endif
		delete [] tbuff;
		ds = mgr->GetSoundDevice();
		return true;

#else
		WaveHeader header;
		unsigned int sz = sizeof(WaveHeader);
		if(!file.Read((void*)&header, &sz))
		{
			file.Close();
			return false;
		}

		if(memcmp(header.chunkid, "RIFF", 4)!=0 || memcmp(header.format, "WAVE", 4)!=0 || header.audioformat != 1)
		{
			file.Close();
			return false;
		}

		if(header.datasize > MAXSBUFF)
		{
			ds = mgr->GetSoundDevice();
			file.Close();
			return false;
		}

		char* tbuff = new char[header.datasize];
		sz = header.datasize;
		if(!file.Read((void*)tbuff, &sz))
		{
			delete [] tbuff;
			file.Close();
			return false;
		}

		file.Close();

#ifdef USE_DIRECTSOUND8
		DWORD size = 0;
		WAVEFORMATEX wfmt;
		ZeroMemory(&wfmt, sizeof(WAVEFORMATEX));

		wfmt.cbSize = sizeof(WAVEFORMATEX);
		wfmt.wBitsPerSample = header.bitspersample;
		wfmt.nChannels = header.chnnels;
		wfmt.nSamplesPerSec = header.samplerate;
		wfmt.wFormatTag = 1;
		wfmt.nBlockAlign = 2*header.chnnels;
		wfmt.nAvgBytesPerSec = 2*wfmt.nSamplesPerSec*wfmt.nChannels;

		DSBUFFERDESC desc;
		ZeroMemory(&desc, sizeof(DSBUFFERDESC));
		desc.dwFlags |= DSBCAPS_CTRLVOLUME;
		desc.dwSize = sizeof(desc);
		desc.lpwfxFormat = &wfmt;
		desc.dwBufferBytes = header.datasize;

		mgr->GetSoundDevice()->GetDevice()->CreateSoundBuffer(&desc, &dsb, 0);
		char* bf;
		dsb->Lock(0, header.datasize, (LPVOID*)&bf, &size, 0, 0, DSBLOCK_ENTIREBUFFER);
		memcpy(bf, tbuff, size);
		dsb->Unlock((LPVOID)bf, size, NULL,NULL);
		ds = mgr->GetSoundDevice();
#else
		ALuint format;
		
		if(header.bitspersample == 8)
		{
			format = (header.chnnels == 2) ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
		}
		else if(header.bitspersample == 16)
		{
			format = (header.chnnels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
		}

		alGenBuffers(1, &buff);

		alBufferData(buff,format,tbuff,header.datasize,header.samplerate);
#endif

		delete [] tbuff;
		return true;
#endif
	}
	return false;
}

void SoundResource::Release()
{
#ifdef USE_DIRECTSOUND8
	if(dsb!=0)
	{
		dsb->Release();
		dsb=0;
	}
#else
	if(buff!=0)
	{
		alDeleteBuffers(1, &buff);
		buff=0;
	}
#endif
}

void SoundResource::OnReset()
{
}

void SoundResource::OnLost()
{
}

SoundResource::SoundResource()
{
#ifdef USE_DIRECTSOUND8
	dsb = 0;
#else
	buff = 0;
#endif
	ds = 0;
}

void* SoundResource::GetResourcePtr()
{
#ifdef USE_DIRECTSOUND8
	return dsb;
#else
	return &buff;
#endif
}

IResource* SoundResource::Clone()
{
	return new SoundResource();
}

void RegisterSoundLoader(ResourceManager* mgr)
{
	mgr->RegisterLoader(RES_TYPE_SOUND, new SoundResource());
}

//font

bool FontResource::LoadFromFile(ResourceManager* mgr)
{
#ifdef USE_BITMAPFONT
	imgset = 0;
#else
	if(font!=0)Release();
#endif

	vector<wstring> lst;
	wstring str;
	wstringstream ss(GetFileName());
	while(getline(ss, str, L','))
	{
		lst.push_back(str);
	}

#ifdef ANDROID
	char tmp[256];
	if(lst.size()!=3) return false;
	wchar_t* stop;
	unsigned int size= 0;

	WideCharToMultiByte(tmp, lst[0].c_str(), 256);

	size = atol(tmp);

	__android_log_print(ANDROID_LOG_VERBOSE, "native", "Loading font >> %d", size);

	unsigned int type = 0;

	WideCharToMultiByte(tmp, lst[1].c_str(), 256);
	type = atol(tmp);

#else
	if(lst.size()!=3) return false;
	wchar_t* stop;
	unsigned int size= 0;
	size = wcstol(lst[0].c_str(), &stop, 10);

	unsigned int type = 0;
	type = wcstol(lst[1].c_str(), &stop, 10);

	unsigned int bold = FW_NORMAL;
	if(type&FONT_BOLD) bold = FW_BOLD;

	BOOL italic = FALSE;
	if(type&FONT_ITALIC) italic = TRUE;
#endif

#ifdef USE_BITMAPFONT
	fsize = size;

	char t[250];
	WideCharToMultiByte(t, lst[2].c_str(), 250);

	Resource rc;
	mgr->GetResource(t, RES_TYPE_SPRITE, &rc);

	if(rc.IsValid())
	{
		imgset = rc;
		return true;
	}

	return false;
#else
	if(mgr->GetDevice()!=0)
	{
		if(SUCCEEDED(D3DXCreateFont(mgr->GetDevice()->GetDevice(), size, 0, bold, 0, italic, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, lst[2].c_str(), &font)))
		{
			return true;
		}
	}
	return false;
#endif
}

void FontResource::Release()
{
#ifdef USE_BITMAPFONT
	imgset = 0;
#else
	if(font!=0)
	{
		font->Release();
		font = 0;
	}
#endif
}

FontResource::FontResource()
{
#ifdef USE_BITMAPFONT
	imgset = 0;
	fsize = 0;
#else
	font = 0;
#endif
}

void* FontResource::GetResourcePtr()
{
#ifdef USE_BITMAPFONT
	return imgset->GetResourcePtr();
#else
	return font;
#endif
}

unsigned int FontResource::GetSize()
{
#ifdef USE_BITMAPFONT	
	return fsize;
#else
	return 0;
#endif
}

IResource* FontResource::Clone()
{
	return new FontResource();
}

void RegisterFontLoader(ResourceManager* mgr)
{
	mgr->RegisterLoader(RES_TYPE_FONT, new FontResource());
}

void FontResource::OnReset()
{
#ifdef USE_BITMAPFONT	
#else
	if(font!=0) font->OnResetDevice();
#endif
}

void FontResource::OnLost()
{
#ifdef USE_BITMAPFONT
#else
	if(font!=0) font->OnLostDevice();
#endif
}