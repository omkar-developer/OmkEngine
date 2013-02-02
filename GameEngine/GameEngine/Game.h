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

#define OBJECT_TYPE_GAMEOBJECT	0x400000		///<Abstract weapon object type.

#define OBJECT_Message_GameWin				1003
#define OBJECT_Message_GameLoose			1004
#define OBJECT_Message_GameStep				1005

#define MAX_LEVELS 20

#ifndef SCRIPTFILE
#define SCRIPTFILE L"Script.as"
#endif

#ifndef SETTINGSFILE
#define SETTINGSFILE L"settings.cfg"
#endif

#ifndef HUDFILE
#define HUDFILE L"hud.cfg"
#endif

#ifndef WINFILE
#define WINFILE L"win.cfg"
#endif

#ifndef LOOSEFILE
#define LOOSEFILE L"loose.cfg"
#endif

#ifndef ZIPRESOURCEFILE
#define ZIPRESOURCEFILE L"Resources.zip"
#endif

#ifndef SAVEDIRECTORY
#define SAVEDIRECTORY L"NewSavedGames"
#endif

#ifndef RESOURCEFILE
#define RESOURCEFILE L"resource.r"
#endif

#ifndef LOADINGFILE
#define LOADINGFILE L"Loading.cfg"
#endif

#ifndef MENUMUSIC
#define MENUMUSIC "menumusicsound"
#endif

#ifndef MAINMENUFILE
#define MAINMENUFILE L"mainmenu.lvl"
#endif

#ifndef OBJECTSCONFIG
#define OBJECTSCONFIG L"objects.cfg"
#endif

#ifndef SCOREUPDATE_FREQ
#define SCOREUPDATE_FREQ 500.0f
#endif

class TextViewer : public GUIText
{
protected:
	unsigned int m_viewtype;
	int m_tmp;

public:
	TextViewer(ResourceManager* mgr = 0, unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true);

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual Object* Clone();

	virtual void Update( float delta );
};

class ProgressViewer : public GUIProgress
{
protected:
	unsigned int m_viewtype;

public:
	ProgressViewer(ResourceManager* mgr = 0, unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true);

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual Object* Clone();

	virtual void Update( float delta );
};

class GlobalSettings
{
protected:
	bool m_enmusic;

public:
	unsigned int m_levels;
	int XP;
	bool m_ensound;

	long m_mvol;
	long m_svol;

	int m_score[MAX_LEVELS];
	char m_stars[MAX_LEVELS];
	float m_time[MAX_LEVELS];

	map<string, int> extraint;
	map<string, float> extrafloat;

	GlobalSettings();

	bool GetMusic();
	void SetMusic(bool val);
	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );
};

class LoadButton : public GUIButton
{
protected:
	wstring m_filename;
	unsigned int m_lvl;
	unsigned int m_unlocklvl;
	unsigned int m_layernum;
	bool m_lvlbutton;
	Resource sound;
	unsigned int m_resgroup;
	wstring m_endgame;
	wstring m_script;

public:
	LoadButton(ResourceManager* mgr = 0, unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true);

	virtual void Update( float delta );

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual void OnClick();

	virtual Object* Clone();
};

class SoundButton : public GUIButton
{
protected:
	bool m_music;
	
public:
	SoundButton(ResourceManager* mgr = 0, unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true);

	virtual void Update( float delta );

	virtual Object* Clone();

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	void SetState(unsigned int st);

	void UpdateState();
};

//SaveConfig
class SaveConfig : public GUIButton
{
protected:
	//bonuses
	bool m_proc;
	bool m_loading;
	string ename;

public:
	int m_effectID;
	int m_upgradecost;
	vector<int> m_links;

	SaveConfig(ResourceManager* mgr = 0, unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true);

	void UpgradeLinks(bool val = false);
	virtual bool Init();

	virtual void SetState(unsigned int st);
	bool GetEnableEffect();
	void SetEnableEffect(bool val);

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual Object* Clone();
};

class GameWorld
{
private:
	C_Window* win;
	C_Device* dev;
	C_SoundDevice sdevice;
	C_Renderer* render;
	C_Timer tm;
	ObjectsContainer* obc;
	ObjectBuilder* builder;
	ResourceManager mgr;

#ifdef USEBOX2DPHYSICS
	PhyWorld m_world;
	DebugDraw m_debugdraw;
#endif

	Resource res;

#ifndef USEBOX2DPHYSICS
	bool m_paused;
#endif

	bool m_processdq;
	wstring m_file;
	bool m_lvlbutton;
	wstring m_sfile;

	bool m_allloaded;

	void FinalizeInit();
	
public:
	AnimationTimeline* timeline;

	bool m_init;
	bool m_lopped;
	bool m_closing;
	wstring m_savedir;

#ifdef USESTREAMING
	C_MusicPlayer splayer;
#else
	C_SoundPlayer splayer;
#endif	

#ifdef ANDROID
	pthread_t thread1;
	bool clostthread;
	pthread_mutex_t mtx;
#endif

#ifdef USESCRIPTING
	Script m_script;
#endif

	bool GameStarted;

#ifdef USEBOX2DPHYSICS
	bool m_DebugDraw;
#endif

	bool ShowFPS;
	bool DrawBoxes;

	int CurrentLevel;
	int UnlockLevel;
	unsigned int CurrentGroup;

	int XPGain;
	int TXP;
	char Stars;
	//global
	GlobalSettings settings;
	wstring m_wingamefile;

	//Loading status
	int m_loadst;

	float lvl_timer;

	float m_tdelta;

	bool m_resourcesloaded;
	
	GameWorld(const wchar_t* title, int winwidth, int winheight, bool fullscreen = false);
	virtual ~GameWorld();
	
	void OnDeleteQueue();
	void SetMusicVolume(long val);
	void SetSoundVolume(long val);
	void ResetAllData();
	void TogglePause();
	void Pause();
	void Unpause();
	bool GetPaused();
	void Win();
	void Loose();
	bool LoadFile(const wchar_t* filename, unsigned int group = 0, bool levelbtn = false, int clevel = 0, int ulevel = 0, Resource sound = 0, const wchar_t* scriptfile = SCRIPTFILE, const wchar_t* winfile = WINFILE);
	
	C_Window* GetWindow();
	C_Device* GetDevice();
	C_Renderer* GetRenderer();
	ObjectsContainer* GetMainContainer();
	ObjectBuilder* GetBuilder();
	ResourceManager* GetResourceManager();

#ifdef USEBOX2DPHYSICS
	PhyWorld* GetPhysicsWorld();
#endif

	C_MusicPlayer* GetSoundPlayer();
	void ResetTimer();

	void Loop(unsigned int BackColor = 0xFF000000);
	void UpdateMusicBuffer();

	//void Callback(Object* obj, unsigned int Msg, unsigned int Param1, void* Param2);
	void OnLoading(int status);
	void QuitApp();
	void Init();
	bool InitSynch();

	void EnableMusic(bool val);
	void EnableSound(bool val);
	void SaveSettings();

	bool GetAllLoaded();

	//virtual functions
	virtual void Initialize() = 0;
	virtual void RegisterObjects(ObjectBuilder* v_builder) = 0;
	virtual void Release(){};

#ifdef USESCRIPTING
	virtual void RegisterScriptObjects(Script* engine) = 0;
#endif
};

extern GameWorld* GetGameAPI(const wchar_t* title, int winwidth, int winheight, bool fullscreen = false);
extern void DisplayMoreTexts(UIWString& txt, unsigned int id);
extern void DisplayMoreProgress(float& progress, unsigned int id);

extern GameWorld* Game;