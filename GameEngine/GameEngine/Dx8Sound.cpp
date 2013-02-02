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

C_SoundDevice::C_SoundDevice()
{
	ds = 0;
}

C_SoundDevice::~C_SoundDevice()
{
	if(ds!=0)
	{
		ds->Release();
		ds = 0;
	}	
}

bool C_SoundDevice::Init(C_Window* win)
{
	if(win==0) return false;
	if(SUCCEEDED(DirectSoundCreate8(NULL, &ds, 0)))
	{
		ds->Initialize(NULL);
		if(ds->SetCooperativeLevel(win->GetHandle(), DSSCL_NORMAL)!=DS_OK)
		{
			ds->Release();
			ds = 0;
			return false;
		}
		window = win;
		return true;
	}

	return false;
}

LPDIRECTSOUND8 C_SoundDevice::GetDevice()
{
	return ds;
}

#else

C_SoundDevice::C_SoundDevice()
{
	device = 0;
	context = 0;
}

C_SoundDevice::~C_SoundDevice()
{
	alcMakeContextCurrent(0);

	if(context!=0)
	{
		alcDestroyContext(context);
	}

	if(device!=0)
	{
        alcCloseDevice(device);
	}	
}

bool C_SoundDevice::Init(C_Window* win)
{
	if(win==0) return false;
	const ALint context_attribs[] = { ALC_FREQUENCY, 44100, 0 };
	device = alcOpenDevice(0);
	if(device==0) return false;
	context = alcCreateContext(device, context_attribs);	
	alcMakeContextCurrent(context);
	return true;
}

ALCdevice* C_SoundDevice::GetDevice()
{
	return device;
}

#endif