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

C_AudioMixer m_mixer;

Mixer_Source::Mixer_Source()
{
	data = 0;
	cpos = 0;
	gain = 1.0f;
	size = 0;
}

Mixer_Source::Mixer_Source(short* data, unsigned int size, float gain)
{
	cpos = 0;
	this->data = data;
	this->gain = gain;
	this->size = size;
}

C_AudioMixer::C_AudioMixer()
{
	musicgain = 1.0f;
	musicplaying = true;
	file = 0;
	fl = 0;
}

void C_AudioMixer::AddSource(const Mixer_Source& src)
{
	source.push_back(src);
}

void C_AudioMixer::GetData(float* data, unsigned int size)
{
	memset(data, 0, size * sizeof(float));	

	if(musicplaying)
	{
		if(file!=0 && fl!=0)
		{			
			short* tdata = new short[size];
			if(file->ReadCycleBuffer(fl, (char*)tdata, size * sizeof(short), fname.c_str()))
			{
				unsigned int multiplier = 1;
				if(file->GetInfo()->channels == 1) multiplier *= 2;
				if(file->GetInfo()->rate == 22050) multiplier *= 2;
				else if(file->GetInfo()->rate == 11025) multiplier *= 4;

				for(unsigned int i = 0; i<size; i++)
				{
					for(unsigned int f = 0; f < multiplier; f++)
					{
						data[i] = (tdata[i] / 32768.0f) * musicgain;
					}
				}
			}
			delete [] tdata;
		}
	}

	for(list<Mixer_Source>::iterator it = source.begin(); it != source.end();)
	{
		unsigned int maxsz = it->size - it->cpos;
		bool erase = true;

		if(maxsz > size)
		{
			maxsz = it->cpos + size;
			erase = false;
		}
		else
		{
			maxsz = it->size;
		}

		for(unsigned int p = it->cpos; p < maxsz;  p++)
		{
			data[(p-it->cpos)] += (it->data[p] * it->gain) / 32768.0f;
			if(data[(p-it->cpos)] > 1.0f) data[(p-it->cpos)] = 1.0f;
			else if(data[(p-it->cpos)] < -1.0f) data[(p-it->cpos)] = -1.0f;
		}

		if(erase)
		{
			source.erase(it++);
		}
		else
		{
			it->cpos = maxsz;
			++it;
		}
	}
}

void C_AudioMixer::ClearAll()
{
	source.clear();
}

C_SoundPlayer::C_SoundPlayer()
{
	m_playing = false;
	releasebuff = false;
	volume = MAX_VOL;
}

C_SoundPlayer::~C_SoundPlayer()
{
	ReleaseBuffer();
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
	rc = res;
	if(res.IsValid() && res->GetType()&RES_TYPE_SOUND)
	{
		sbuff = (CAST_RES(vector<short>*, res));
	}
}

void C_SoundPlayer::Play(bool loop)
{
	if(Game->settings.m_ensound)
	{
		if(rc.IsValid())
		{
			m_mixer.AddSource(Mixer_Source(&(*sbuff)[0], sbuff->size(), volume / MAX_VOL));
			m_playing = true;
		}
	}
}

void C_SoundPlayer::Stop()
{

}

void C_SoundPlayer::SetVolume(long volume)
{
	volume = volume;
}

long C_SoundPlayer::GetVolume()
{
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

//music

C_MusicPlayer::C_MusicPlayer()
{

}

C_MusicPlayer::~C_MusicPlayer()
{
	Stop();
	oggf.CloseFile();
	file.Close();
}

void C_MusicPlayer::Stop()
{
	m_mixer.musicplaying = false;
	m_mixer.file = 0;
	m_mixer.fl = 0;	
	m_playing = false;
}

void C_MusicPlayer::SetVolume(long volume)
{
	C_SoundPlayer::SetVolume(volume);
	m_mixer.musicgain = volume / MAX_VOL;
}

long C_MusicPlayer::GetVolume()
{
	return C_SoundPlayer::GetVolume();
}

void C_MusicPlayer::Play(bool loop)
{
	if(!Game->settings.m_ensound)
	{
		return;
	}
	if(filename!=L"")
	{
		char tmp[256];
		WideCharToMultiByte(tmp, filename.c_str(), 256);
		m_mixer.musicplaying = true;
		m_mixer.musicgain = volume / MAX_VOL;		
		if(rc.IsValid())
		{
			m_mixer.fname = rc->GetFileName();
		}
		m_mixer.file = &oggf;
		m_mixer.fl = &file;
		m_playing = true;
	}
}

void C_MusicPlayer::ReleaseBuffer()
{
	
}

void C_MusicPlayer::Save( BaseIO& str, const char* name )
{
	string n = name;
	n += " sound;help=Sound resource Name.;type=soundresource";
	str.Write(&rc, 0, n.c_str());
}

void C_MusicPlayer::Load( BaseIO& str )
{
	str.Read(&rc);
	SetBuffer(rc);
}

void C_MusicPlayer::SetBuffer(Resource buff)
{
	ReleaseBuffer();

	if(buff.IsValid() && buff->GetType()&RES_TYPE_SOUND)
	{
		rc = buff;
		filename = rc->GetFileName();
		m_mixer.fname = rc->GetFileName();
		
		oggf.CloseFile();
		file.Close();

		file.Open(filename.c_str());
		oggf.OpenFile(&file);

		m_mixer.fl = &file;
		m_mixer.file = &oggf;
		//wave file
	}
}