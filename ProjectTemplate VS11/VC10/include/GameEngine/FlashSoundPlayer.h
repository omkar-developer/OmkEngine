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

struct Mixer_Source
{
	unsigned int cpos;
	unsigned int size;
	short* data;
	float gain;

	Mixer_Source();
	Mixer_Source(short* data, unsigned int size, float gain = 1.0f);
};

class C_AudioMixer
{
private:
	list<Mixer_Source> source;

public:
	wstring fname;
	float musicgain;
	bool musicplaying;
	oggFile* file;
	BaseIO* fl;

	C_AudioMixer();

	void AddSource(const Mixer_Source& src);
	void GetData(float* data, unsigned int size);
	void ClearAll();
};

extern C_AudioMixer m_mixer;

class C_SoundPlayer
{
protected:
	Resource rc;
	bool m_playing;
	bool releasebuff;
	vector<short>* sbuff;
	long volume;

public:
	C_SoundPlayer();
	virtual ~C_SoundPlayer();
	virtual void SetBuffer(Resource res);
	virtual void Play(bool loop = true);
	virtual void Stop();	

	virtual void SetVolume(long volume);
	virtual long GetVolume();

	virtual void ReleaseBuffer();

	virtual bool IsPlaying();

	virtual void Save( BaseIO& str, const char* name = "name=Sound" );
	virtual void Load( BaseIO& str );
};

class C_MusicPlayer : public C_SoundPlayer
{
private:
	wstring filename;
	ZipFile2 file;
	//Resource rc;

#ifdef USE_OGG
	oggFile oggf;
#endif

public:
	C_MusicPlayer();
	~C_MusicPlayer();
	virtual void SetBuffer(Resource res);

	virtual void ReleaseBuffer();
	virtual void Play(bool loop = false);
	virtual void Stop();

	virtual void SetVolume(long volume);
	virtual long GetVolume();

	void Update(float delta){};

	virtual void Save( BaseIO& str, const char* name = "name=Sound" );
	virtual void Load( BaseIO& str );
};