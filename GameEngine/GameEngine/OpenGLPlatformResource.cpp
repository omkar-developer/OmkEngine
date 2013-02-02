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

void IORead(png_structp pstr, png_bytep data, png_size_t length)
{
	ZipFile* p = (ZipFile*)png_get_io_ptr(pstr);
	unsigned int sz = length;
	p->Read((void*)data, &sz);
}

bool CreateTextureFromFile(const wchar_t* filename, char** data2, GLuint* tex, Size* size)
{
	ZipFile file;
	file.Open(filename);
	png_byte psign[8];
	unsigned int sz = 8;
	/*char* data = 0;*/
	if(!file.Read((void*)psign, &sz))
	{
		file.Close();
		return false;
	}

	if(png_sig_cmp(psign, 0, 8)!=0)
	{
		file.Close();
		return false;
	}

	png_structp pstr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	if(pstr==0)
	{
		file.Close();
		return false;
	}

	png_infop pinfo = png_create_info_struct(pstr);
	if(pinfo==0)
	{
		png_destroy_read_struct(&pstr, 0, 0);
	}

	png_bytep* rows = 0;

	if(setjmp(png_jmpbuf(pstr)))
	{
		file.Close();
		png_destroy_read_struct(&pstr, &pinfo, 0);
		if(rows!=0) delete [] rows;
		if(*data2!=0) 
		{
			delete [] *data2;
			*data2 = 0;
		}

		return false;
	}

	png_set_read_fn(pstr, (png_voidp)&file, IORead);

	png_set_sig_bytes(pstr, 8);
	png_read_info(pstr, pinfo);

	png_uint_32 imgWidth =  png_get_image_width(pstr, pinfo);
    png_uint_32 imgHeight = png_get_image_height(pstr, pinfo);

	png_uint_32 bitdepth   = png_get_bit_depth(pstr, pinfo);
	png_uint_32 channels   = png_get_channels(pstr, pinfo);

	png_uint_32 color_type = png_get_color_type(pstr, pinfo);

	switch (color_type) 
	{
        case PNG_COLOR_TYPE_PALETTE:
            png_set_palette_to_rgb(pstr);
            channels = 3;           
            break;
        case PNG_COLOR_TYPE_GRAY:
            if (bitdepth < 8)
            png_set_expand_gray_1_2_4_to_8(pstr);
            bitdepth = 8;
            break;
    }

    if (png_get_valid(pstr, pinfo, PNG_INFO_tRNS)) 
	{
        png_set_tRNS_to_alpha(pstr);
        channels+=1;
    }

	rows = new png_bytep[imgHeight];
	*data2 = new char[imgWidth * imgHeight * bitdepth * channels / 8];
	const unsigned int stride = imgWidth * bitdepth * channels / 8;

	for (size_t i = 0; i < imgHeight; i++) 
	{
        png_uint_32 q = (imgHeight- i - 1) * stride;
        rows[i] = (png_bytep)(*data2) + q;
    }

	png_read_image(pstr, rows);

	delete [] rows;
	png_destroy_read_struct(&pstr, &pinfo,0);
	file.Close();

	glGenTextures(1, tex);
	glBindTexture(GL_TEXTURE_2D, *tex);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, (*data2));
	//delete [] data;
	if(size!=0)
	{
		size->width = imgWidth;
		size->height = imgHeight;
	}
	return true;
}

ImageResource::ImageResource()
{
	texture = 0;
	data = 0;
}

Size ImageResource::GetImageSize()
{
	if(texture!=0)
	{
		return m_size;
	}
	return Size(0, 0);
}

bool ImageResource::LoadFromFile(ResourceManager* mgr)
{
	Release();
	if(CreateTextureFromFile(GetFileName(), &data, &texture, &m_size))
	{
		return true;
	}

	return false;
}

void ImageResource::Release()
{
	if(texture!=0)
	{
		glDeleteTextures(1, &texture);
		texture = 0;
	}
	if(data!=0)
	{
		delete [] data;
		data = 0;
	}

	char tmp[250];
	WideCharToMultiByte(tmp, GetFileName(), 250);
	__android_log_print(ANDROID_LOG_VERBOSE, "native", "Realeased Resource File >> %s", tmp);
}

void* ImageResource::GetResourcePtr()
{
	return &texture;
}

IResource* ImageResource::Clone()
{	
	return new ImageResource();
}

void ImageResource::OnReset()
{
	texture = 0;
	if(data==0)
	{
		char tmp[256];
		WideCharToMultiByte(tmp, GetFileName(), 256);
		if(CreateTextureFromFile(GetFileName(), &data, &texture, &m_size))
		{
			__android_log_print(ANDROID_LOG_VERBOSE, "native", "Texture Reset : %s", tmp);
		}	
	}
	else
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.width, m_size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
}

void ImageResource::OnLost()
{
	if(texture!=0)
	{
		glDeleteTextures(1, &texture);
		texture = 0;
	}
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
		bread = 0;
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

	char tmp[256];
	if(lst.size()!=3) return false;
	const wchar_t* stop = L"\0";
	unsigned int size= 0;

	WideCharToMultiByte(tmp, lst[0].c_str(), 256);

	size = atol(tmp);

	__android_log_print(ANDROID_LOG_VERBOSE, "native", "Loading font >> %d", size);

	unsigned int type = 0;

	WideCharToMultiByte(tmp, lst[1].c_str(), 256);
	type = atol(tmp);

	/*unsigned int bold = FW_NORMAL;
	if(type&FONT_BOLD) bold = FW_BOLD;

	BOOL italic = FALSE;
	if(type&FONT_ITALIC) italic = TRUE;*/

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