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

#ifdef USE_DIRECTSOUND8
C_SoundPlayer::C_SoundPlayer()
{
	dsb = 0;
	releasebuff = false;
	m_playing = false;
}

C_SoundPlayer::~C_SoundPlayer()
{
	ReleaseBuffer();
}

void C_SoundPlayer::ReleaseBuffer()
{
	if(dsb!=0 && releasebuff)
	{
		dsb->Release();
		dsb = 0;
	}
}

bool C_SoundPlayer::IsPlaying()
{
	return m_playing;
}

void C_SoundPlayer::SetBuffer(Resource buff)
{
	ReleaseBuffer();

	if(buff.IsValid() && buff->GetType()&RES_TYPE_SOUND)
	{
		#ifdef USE_DIRECTSOUND8
			if(buff.GetPtr<SoundResource>()->GetSoundDevice()==0) return;
			if(buff.GetPtr<SoundResource>()->GetSoundDevice()->GetDevice()==0) return;
		#endif
		buff.GetPtr<SoundResource>()->GetSoundDevice()->GetDevice()->DuplicateSoundBuffer(CAST_RES(LPDIRECTSOUNDBUFFER, buff), &dsb);
		releasebuff = true;
	}
}

void C_SoundPlayer::SetBuffer(LPDIRECTSOUNDBUFFER buff)
{
	ReleaseBuffer();

	if(dsb!=0)
	{
		dsb->Release();
		dsb = 0;
	}

	dsb = buff;
	releasebuff = false;
}

LPDIRECTSOUNDBUFFER C_SoundPlayer::GetBuffer()
{
	return dsb;
}

void C_SoundPlayer::Play(bool loop)
{
	if(!Game->settings.m_ensound)
	{
		return;
	}
	if(dsb!=0)
	{
		SetVolume(Game->settings.m_svol);
		dsb->Play(0, 0, loop ? DSBPLAY_LOOPING : 0);
		if(loop) m_playing = true;
	}
}

void C_SoundPlayer::Stop()
{
	if(dsb!=0)
	{
		dsb->Stop();
		m_playing = false;
	}
}

void C_SoundPlayer::SetVolume(long volume)
{
	if(dsb!=0)
	{
		dsb->SetVolume(DSBVOLUME_MIN + volume);
	}
}

long C_SoundPlayer::GetVolume()
{
	long volume = 0;
	if(dsb!=0)
	{
		dsb->GetVolume(&volume);
	}
	return volume;
}

void C_SoundPlayer::Save( BaseIO& str, const char* name )
{
	string n = name;
	n += " sound;help=Sound resource Name.;type=soundresource";
	str.Write(&rc, 0, n.c_str());	
}

void C_SoundPlayer::Load( BaseIO& str )
{
	str.Read(&rc);
	SetBuffer(rc);
}
#else

C_SoundPlayer::C_SoundPlayer()
{
	buff = 0;
	source = 0;
	m_playing = false;
	releasebuff = false;
}

C_SoundPlayer::~C_SoundPlayer()
{
	ReleaseBuffer();
	if(source!=0)
	{
		alDeleteSources(1, &source);
		source = 0;
	}
}

void C_SoundPlayer::ReleaseBuffer()
{
	
}

bool C_SoundPlayer::IsPlaying()
{
	return m_playing;
}

void C_SoundPlayer::SetBuffer(Resource res)
{
	ReleaseBuffer();

	if(res.IsValid() && res->GetType()&RES_TYPE_SOUND)
	{
		buff = *(CAST_RES(ALuint*, res));
	}

	if(source==0) alGenSources(1, &source);
	if(source==0) return;
	alSourcei(source, AL_BUFFER, buff);
}

void C_SoundPlayer::SetBuffer(ALuint* buff)
{
	ReleaseBuffer();
	this->buff = *buff;

	if(source==0) alGenSources(1, &source);
	if(source==0) return;
	if(buff!=0) alSourcei(source, AL_BUFFER, this->buff);
}

ALuint* C_SoundPlayer::GetBuffer()
{
	return &buff;
}

void C_SoundPlayer::Play(bool loop)
{
	if(source!=0 && buff!=0)
	{
		alSourcei(source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
		alSourcePlay(source);
		SetVolume(Game->m_svol);
		if(loop) m_playing = true;
	}
}

void C_SoundPlayer::Stop()
{
	if(source!=0)
	{
		alSourceStop(source);
		m_playing = false;
	}
}

void C_SoundPlayer::SetVolume(long volume)
{
	if(source!=0)
	{
		alSourcef(source, AL_GAIN, 1.0f - (abs(Game->m_svol) / MAX_VOL));
	}
}

long C_SoundPlayer::GetVolume()
{
	float vol = 0;
	if(source!=0)
	{
		alGetSourcef(source, AL_GAIN, &vol);
		vol = (1.0f - vol) * MAX_VOL;
	}
	return (long)vol;
}

void C_SoundPlayer::Save( BaseIO& str, const char* name )
{
	string n = name;
	n += " sound;help=Sound resource Name.;type=soundresource";
	str.Write(&rc, 0, n.c_str());	
}

void C_SoundPlayer::Load( BaseIO& str )
{
	str.Read(&rc);
	SetBuffer(rc);
}

#endif

//music

C_MusicPlayer::C_MusicPlayer() : C_SoundPlayer()
{
#ifdef USE_DIRECTSOUND8
#else
	buff = 0;
	buff2 = 0;
	source = 0;
#endif
	releasebuff = false;
	m_playing = false;
	avgbytes = 0;
	updateb1 = false;
	updateb2 = false;
}

C_MusicPlayer::~C_MusicPlayer()
{
	oggf.CloseFile();
	file.Close();
	ReleaseBuffer();
}

#if !defined(USE_DIRECTSOUND8)
void C_MusicPlayer::ReleaseBuffer()
{
	if(releasebuff)
	{
		if(buff!=0) 
		{
			alDeleteBuffers(1, &buff);
			buff = 0;
		}
		if(buff2!=0) 
		{
			alDeleteBuffers(1, &buff2);
			buff2 = 0;
		}
	}
	C_SoundPlayer::ReleaseBuffer();
}
#endif

#if !defined(USE_OGG)
bool ReadCycleBuffer(BaseIO* file, char* buff, unsigned int size, const wchar_t* filename)
{
	unsigned int read = 0;
	unsigned int sz = size;
	bool wrapped = false;

	while((read+sz)<=size)
	{
		if(!file->Read((void*)buff, &sz))
		{
			if(!wrapped)
			{
				file->Close();
				file->Open(filename, BaseIO::Access_Read);
				WaveHeader hd;
				unsigned int szz = sizeof(WaveHeader);
				file->Read((void*)&hd, &szz);
				wrapped = true;
			}
			else return false;
		}

		read += sz;

		if(read<size)
		{
			file->Close();
			file->Open(filename, BaseIO::Access_Read);
			WaveHeader hd;
			unsigned int szz = sizeof(WaveHeader);
			file->Read((void*)&hd, &szz);
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
#endif

void C_MusicPlayer::SetBuffer(Resource buff)
{
	ReleaseBuffer();

	if(buff.IsValid() && buff->GetType()&RES_TYPE_SOUND)
	{
		rc = buff;

#ifdef USE_DIRECTSOUND8
		if(buff.GetPtr<SoundResource>()->GetSoundDevice()==0) return;
		if(buff.GetPtr<SoundResource>()->GetSoundDevice()->GetDevice()==0) return;
#endif

		filename = rc->GetFileName();

		file.Close();
		if(!file.Open(filename.c_str())) return;
		
		//wave file
#ifdef USE_OGG
		if(!oggf.OpenFile(&file)) return;
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

		DSBUFFERDESC desc;
		ZeroMemory(&desc, sizeof(DSBUFFERDESC));
		desc.dwFlags |= DSBCAPS_CTRLVOLUME;

		desc.dwSize = sizeof(desc);
		desc.lpwfxFormat = &wfmt;
		desc.dwBufferBytes = wfmt.nAvgBytesPerSec * 4;
#else
		if(source==0) alGenSources(1, &source);
		if(source==0) return;

		ALuint format;
		
		format = (vi->channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
		alGenBuffers(1, &(this->buff));
		alGenBuffers(1, &buff2);
#endif
#else
		WaveHeader header;
		unsigned int sz = sizeof(WaveHeader);
		if(!file.Read((void*)&header, &sz))
		{
			file.Close();
			return;
		}

		if(memcmp(header.chunkid, "RIFF", 4)!=0 || memcmp(header.format, "WAVE", 4)!=0 || header.audioformat != 1)
		{
			file.Close();
			return;
		}

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
		desc.dwBufferBytes = wfmt.nAvgBytesPerSec * 4;
#endif

#ifdef USE_DIRECTSOUND8
		DWORD size = 0;
		buff.GetPtr<SoundResource>()->GetSoundDevice()->GetDevice()->CreateSoundBuffer(&desc, &dsb, 0);
		char* bf;
		if(Game) dsb->SetVolume(Game->settings.m_mvol);
		dsb->Lock(0, wfmt.nAvgBytesPerSec * 2, (LPVOID*)&bf, &size, 0, 0, DSBLOCK_ENTIREBUFFER);
#ifdef USE_OGG
		oggf.ReadCycleBuffer(&file, bf, size, filename.c_str());
#else
		ReadCycleBuffer(&file, bf, size, filename.c_str());
#endif
		dsb->Unlock((LPVOID)bf, size, NULL,NULL);
		updateb1 = false;
		updateb2 = true;
		avgbytes = wfmt.nAvgBytesPerSec * 4;
#else
		avgbytes = vi->rate * vi->channels * 2 * 8;
		unsigned int size = avgbytes/2;
		char* bf = new char[avgbytes/2];
		char* bf2 = new char[avgbytes/2];
#ifdef USE_OGG
		oggf.ReadCycleBuffer(&file, bf, size, filename.c_str());
		oggf.ReadCycleBuffer(&file, bf2, size, filename.c_str());
#else
		ReadCycleBuffer(&file, bf, size, filename.c_str());
		ReadCycleBuffer(&file, bf2, size, filename.c_str());
#endif
		alBufferData((this->buff),format,bf,size,vi->rate);
		alBufferData(buff2,format,bf2,size,vi->rate);
		alSourceQueueBuffers(source, 1, &(this->buff));
		alSourceQueueBuffers(source, 1, &(this->buff2));
		delete [] bf;
		delete [] bf2;
#endif
		releasebuff = true;
	}
}

#ifdef USE_DIRECTSOUND8
void C_MusicPlayer::SetBuffer(LPDIRECTSOUNDBUFFER buff)
{

}

LPDIRECTSOUNDBUFFER C_MusicPlayer::GetBuffer()
{
	return 0;
}
#else
void C_MusicPlayer::SetBuffer(ALuint* buff)
{
}

ALuint* C_MusicPlayer::GetBuffer()
{
	return 0;
}

void C_MusicPlayer::Play(bool loop)
{
	C_SoundPlayer::Play(false);
}
#endif

void C_MusicPlayer::Update(float delta)
{
#ifdef USE_DIRECTSOUND8
	if(!Game->settings.GetMusic())
	{
		Stop();
		return;
	}

	if(dsb==0) return;
	if(releasebuff != true) return;	

	DWORD pos;
	dsb->GetCurrentPosition(&pos, NULL);
	if(pos > 0 && pos < avgbytes / 2)
	{
		updateb1 = false;
		if(!updateb2)
		{
			char* bf = 0;
			DWORD size = 0;
			dsb->Lock(avgbytes/2, avgbytes/2, (LPVOID*)&bf, &size, 0, 0, 0);
#ifdef USE_OGG
			oggf.ReadCycleBuffer(&file, bf, size, filename.c_str());
#else
			ReadCycleBuffer(&file, bf, size, filename.c_str());
#endif
			dsb->Unlock((LPVOID)bf, size, NULL,NULL);
			updateb2 = true;
		}
	}
	else if(pos > avgbytes / 2 && pos < avgbytes)
	{
		updateb2 = false;
		if(!updateb1)
		{
			char* bf = 0;
			DWORD size = 0;
			dsb->Lock(0, avgbytes/2, (LPVOID*)&bf, &size, 0, 0, 0);
#ifdef USE_OGG
			oggf.ReadCycleBuffer(&file, bf, size, filename.c_str());
#else
			ReadCycleBuffer(&file, bf, size, filename.c_str());
#endif
			dsb->Unlock((LPVOID)bf, size, NULL,NULL);
			updateb1 = true;
		}
	}
#else
	ALint process;
	alGetSourcei(source, AL_BUFFERS_PROCESSED, &process);
	while(process>0)
	{
		alSourceUnqueueBuffers(source, 1,  &(this->buff));

		unsigned int size = avgbytes/2;
		char* bf = new char[avgbytes/2];
#ifdef USE_OGG
		oggf.ReadCycleBuffer(&file, bf, size, filename.c_str());
#else
		ReadCycleBuffer(&file, bf, size, filename.c_str());
#endif
		ALuint format = 0;
		format = (oggf.GetInfo()->channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
		alBufferData((this->buff),format,bf,size,oggf.GetInfo()->rate);
		alSourceQueueBuffers(source, 1, &(this->buff));
		delete [] bf;
		--process;
	}
#endif
}

void C_MusicPlayer::Play(bool loop)
{
	if(!Game->settings.GetMusic())
	{
		return;
	}
	if(dsb!=0)
	{
		SetVolume(Game->settings.m_mvol);
		dsb->Play(0, 0, true);
		if(loop) m_playing = true;
	}
}