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

#ifdef USE_DIRECTSOUND8

class C_SoundPlayer
{
protected:
	LPDIRECTSOUNDBUFFER dsb;
	bool releasebuff;
	Resource rc;
	bool m_playing;

public:
	C_SoundPlayer();
	~C_SoundPlayer();
	virtual void SetBuffer(Resource buff);
	virtual void Play(bool loop = true);
	virtual void Stop();	

	virtual void SetVolume(long volume);
	virtual long GetVolume();

	virtual void SetBuffer(LPDIRECTSOUNDBUFFER buff);
	virtual LPDIRECTSOUNDBUFFER GetBuffer();

	virtual void ReleaseBuffer();

	virtual bool IsPlaying();

	virtual void Save( BaseIO& str, const char* name = "name=Sound" );
	virtual void Load( BaseIO& str );
};

#else

class C_SoundPlayer
{
protected:
	ALuint buff;
	ALuint source;
	Resource rc;
	bool m_playing;
	bool releasebuff;

public:
	C_SoundPlayer();
	virtual ~C_SoundPlayer();
	virtual void SetBuffer(Resource res);
	virtual void Play(bool loop = true);
	virtual void Stop();	

	virtual void SetVolume(long volume);
	virtual long GetVolume();

	virtual void SetBuffer(ALuint* buff);
	virtual ALuint* GetBuffer();

	virtual void ReleaseBuffer();

	virtual bool IsPlaying();

	virtual void Save( BaseIO& str, const char* name = "name=Sound" );
	virtual void Load( BaseIO& str );
};

#endif

class C_MusicPlayer : public C_SoundPlayer
{
private:
	wstring filename;
	ZipFile2 file;
	unsigned int avgbytes;
	bool updateb1;
	bool updateb2;
#ifdef USE_DIRECTSOUND8
#else
	ALuint buff2;
#endif

#ifdef USE_OGG
	oggFile oggf;
#endif

public:
	C_MusicPlayer();
	~C_MusicPlayer();
	void SetBuffer(Resource buff);

#ifdef USE_DIRECTSOUND8
	void SetBuffer(LPDIRECTSOUNDBUFFER buff);
	LPDIRECTSOUNDBUFFER GetBuffer();
#else
	void SetBuffer(ALuint* buff);
	ALuint* GetBuffer();
	virtual void ReleaseBuffer();
	virtual void Play(bool loop = false);
#endif

	void Update(float delta);
	virtual void Play(bool loop = true);
};