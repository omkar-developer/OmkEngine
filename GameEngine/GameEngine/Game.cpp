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

/// Game Objects

GameWorld* Game = 0;

#ifdef __AVM2__
#else
#ifdef ANDROID
#else
BOOL DirectoryExists(LPCTSTR szPath)
{
  DWORD dwAttrib = GetFileAttributes(szPath);

  return (dwAttrib != INVALID_FILE_ATTRIBUTES && 
         (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}
#endif
#endif

GameWorld::GameWorld(const wchar_t* title, int winwidth, int winheight, bool fullscreen)
{
#ifdef ANDROID
	clostthread = false;
	pthread_mutex_init(&mtx, 0);
#else 
	ZipFile::OpenZipFile(ZIPRESOURCEFILE);
	ZipFile2::OpenZipFile(ZIPRESOURCEFILE);
#ifndef __AVM2__
	wchar_t spath[MAX_PATH];
	SHGetFolderPath(0, CSIDL_PERSONAL, NULL, 0, spath);
	m_savedir = spath;
	m_savedir += L"/";
	m_savedir += SAVEDIRECTORY;

	if(!DirectoryExists(m_savedir.c_str()))
	{
		CreateDirectory(m_savedir.c_str(), 0);
	}
#else
	m_savedir += L"/local/";
	m_savedir = SAVEDIRECTORY;
#endif
#endif
	timeline = 0;
#ifndef USEBOX2DPHYSICS
	m_paused = false;
#endif
	m_lopped = false;
	m_closing= false;
	m_processdq = false;
	m_lvlbutton = true;
	GameStarted = false;
	CurrentLevel = 0;
	UnlockLevel = 0;
	CurrentGroup = 0;
	m_loadst = 0;
	lvl_timer = 0;
	m_tdelta = 0;
	m_resourcesloaded = false;
	m_allloaded = false;

	win = new C_Window(title, winwidth, winheight, fullscreen);
	win->SetVisible(true);
	dev = new C_Device(win, fullscreen);
	render = new C_Renderer(dev);
	builder = new ObjectBuilder();
	sdevice.Init(win);

	mgr.SetDevice(dev);
	mgr.SetSoundDevice(&sdevice);

	// Register Loaders
	RegisterImageLoader(&mgr);
	RegisterFontLoader(&mgr);
	RegisterSoundLoader(&mgr);
	SpriteFrameSet::RegisterLoader(&mgr);
	Sprite::RegisterLoader(&mgr);

	mgr.PrepareResources(RESOURCEFILE);
	mgr.LoadResourcesByGroup(1);
	mgr.GetResource("DefaultFont", RES_TYPE_FONT, &res);

	tm.Start();

#ifdef USEBOX2DPHYSICS
	m_DebugDraw = false;
#endif

	ShowFPS = false;
	DrawBoxes = false;
	Game = this;

	builder->RegisterDefaultBuilders(&mgr, render);
	obc = (ObjectsContainer*)builder->BuildObject("Objects Container");
	obc->AutoConnectInput(win);
	obc->SetObjectArea(RectangleF(0.0f, 0.0f, (float)winwidth, (float)winheight));
	obc->m_drawboundingbox = true;
	obc->SetLayer(1.0);
	obc->SetupCamera(Camera2D(RectangleF(0, 0, (float)winwidth, (float)winheight), 1.0f));
	obc->GetCamera().m_max = Size(800, 100);

#ifndef ANDROID
	mgr.OnLoadingStatusChanged.RegisterSingleHandler<GameWorld, &GameWorld::OnLoading>(this);
#endif
	obc->OnDeleteQueue.RegisterSingleHandler<GameWorld, &GameWorld::OnDeleteQueue>(this);
	dev->OnDeviceReset.RegisterSingleHandler<ResourceManager, &ResourceManager::OnReset>(&mgr);

#if defined(ANDROID) || defined(__AVM2__)
	dev->SetBackBufferSize(Size(winwidth, winheight));
	obc->AdjustSize(*Game->GetDevice());
#ifdef __AVM2__
	glViewport(0, 0, winwidth, winheight);
#endif
	glViewport(0, 0, winwidth, winheight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
#ifdef ANDROID
	glOrthof(0,winwidth,winheight,0, -1, 1);
#else
	glOrtho(0,winwidth,winheight,0, -10, 10);
#endif
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
#endif

	ZipFile filel(GetResourceManager());
	filel.Open(LOADINGFILE);
	obc->LoadAll(filel, *builder, false);
	filel.Close();

	Game->GetMainContainer()->AdjustSize(*Game->GetDevice());
	m_init = true;
}

#ifdef USEBOX2DPHYSICS
PhyWorld* GameWorld::GetPhysicsWorld()
{
	return &m_world;
}
#endif

void GameWorld::FinalizeInit()
{
	C_Timer ctm;
	ctm.Start();
	
#ifndef ANDROID
		obc->Clear();
		mgr.OnLoadingStatusChanged.DisconnectHandler<GameWorld, &GameWorld::OnLoading>(this);
#endif

		builder->RegisterBuilder("TextViewer", new TextViewer(&mgr));
		builder->RegisterBuilder("ViewProgress", new ProgressViewer(&mgr));
		builder->RegisterBuilder("LoadButton", new LoadButton(&mgr));
		builder->RegisterBuilder("SoundButton", new SoundButton(&mgr));
		builder->RegisterBuilder("SaveConfig", new SaveConfig(&mgr));

		RegisterObjects(builder);		

#ifdef USEBOX2DPHYSICS
		m_debugdraw.SetRenderer(render);
		m_world.SetDebugDraw(&m_debugdraw);
		m_debugdraw.AppendFlags(b2Draw::e_aabbBit);
		m_debugdraw.AppendFlags(b2Draw::e_centerOfMassBit);
		m_debugdraw.AppendFlags(b2Draw::e_jointBit);
		m_debugdraw.AppendFlags(b2Draw::e_pairBit);
		m_debugdraw.AppendFlags(b2Draw::e_shapeBit);
#endif
	
		ZipFile file(GetResourceManager());
		C_File cfile(GetResourceManager());

		//global
		wstring sepath;
		sepath = m_savedir;
#ifndef __AVM2__
		sepath += L"/";
#endif
		sepath += SETTINGSFILE;
		cfile.Open(sepath.c_str(), BaseIO::Access_Read);
		settings.Load(cfile);
		cfile.Close();
		//

#ifdef USESCRIPTING
		RegisterScriptObjects(&m_script);
		obc->Callback.RegisterSingleHandler<Script, &Script::Callback>(&m_script);
		//m_script.Open(SCRIPTFILE);
		//m_script.Execute();		
#endif	
		Resource sres;
		mgr.GetResource(MENUMUSIC, RES_TYPE_SOUND, &sres);
		splayer.SetBuffer(sres); 
		splayer.Play();

#ifdef USEBOX2DPHYSICS
		//Objects 
		ObjectsContainer* config = (ObjectsContainer*)builder->BuildObject("Objects Container");
		ZipFile file4(GetResourceManager());
		file4.Open(OBJECTSCONFIG, BaseIO::Access_Read);
		config->LoadAll(file4, *builder, false);
		file4.Close();

		list<Object*>::iterator f = config->GetObjectInteratorBegin();

		for(;f!=config->GetObjectInteratorEnd(); f++)
		{
			if((*f)->GetType()&OBJECT_TYPE_PHYSICSOBJ)
			{
				if((*f)->m_objname.size() > 0) builder->RegisterBuilder((*f)->m_objname.c_str(), ((PhysicsObject*)(*f))->BuilderClone());
			}
			else if((*f)->m_objname.size() > 0) builder->RegisterBuilder((*f)->m_objname.c_str(), ((DrawableObject*)(*f))->Clone());
		}

		delete config;
		m_world.ClearAllObjects();
#endif
		//
}

bool GameWorld::GetAllLoaded()
{
	return m_allloaded;
}

bool GameWorld::InitSynch()
{
	if(!m_resourcesloaded)
	{
		if(!mgr.GetSynchLoading())
		{
			mgr.ResetSynchLoader(0);
		}
		else
		{
			if(mgr.LoadResourcesByGroupSynch())
			{
				m_resourcesloaded = true;
				m_init = false;
				FinalizeInit();
				return true;
			}
		}		
	}
	else if(!m_allloaded)
	{
		m_allloaded = true;

		Initialize();
		Game->LoadFile(MAINMENUFILE, 0, false, 0, 0, 0, SCRIPTFILE, WINFILE);
		Game->GetMainContainer()->AdjustSize(*Game->GetDevice());
		Game->Unpause();
		return true;
	}
	return false;
}

void GameWorld::Init()
{
	if(m_init)
	{
		m_init = false;
		
		mgr.LoadResourcesByGroup(0);
		FinalizeInit();

		Game->LoadFile(MAINMENUFILE, 0, false, 0, 0, 0, SCRIPTFILE, WINFILE);
		Game->GetMainContainer()->AdjustSize(*Game->GetDevice());
		Game->Unpause();
		m_resourcesloaded = true;
		m_allloaded = true;
	}
}

void GameWorld::QuitApp()
{
#ifdef ANDROID
	__android_log_print(ANDROID_LOG_VERBOSE, "native", "Stopping . . . . .");
	pthread_mutex_lock(&Game->mtx);
	Game->clostthread = true;
	pthread_mutex_unlock(&Game->mtx);
	__android_log_print(ANDROID_LOG_VERBOSE, "native", "Waiting on Thread . . . . .");
	pthread_join(Game->thread1, 0);
	ZipFile2::CloseZipFile();
	ZipFile::CloseZipFile();
	delete Game;
	Game = 0;
	__android_log_print(ANDROID_LOG_VERBOSE, "native", "Stopped . . . . .");
#else
	Game->GetWindow()->Close();
#endif
	m_closing = true;
}

void GameWorld::EnableMusic(bool val)
{
	Game->settings.SetMusic(val);
	if(val)
	{
		splayer.Play(false);
	}
	else
	{
		splayer.Stop();
	}
}

void GameWorld::EnableSound(bool val)
{
	Game->settings.m_ensound = val;
}

void GameWorld::OnLoading(int status)
{
	m_loadst = status;
	Loop();
#ifdef ANDROID
	//eglSwapBuffers( eglGetCurrentDisplay(), eglGetCurrentSurface( EGL_DRAW ) );
#endif
#ifdef __AVM2__
	inline_as3(
		"import GLS3D.GLAPI;"
		"CModule.serviceUIRequests();"
		"GLAPI.instance.context.present();"
		);
#endif
}

void GameWorld::OnDeleteQueue()
{
	if(m_processdq)
	{
		m_processdq = false;

		if(m_file.size()>1 && m_file[0] == L':')
		{
			wstring fname = m_savedir;
			fname += L"/";
			fname += &m_file[1];

			C_File file(Game->GetResourceManager());

			if(!file.Open(fname.c_str(), BaseIO::Access_Read, BaseIO::Create_OpenExisting)) return;

			GetMainContainer()->LoadAll(file, *Game->GetBuilder(), false);

			file.Close();
		}
		else
		{
			ZipFile file(Game->GetResourceManager());

			if(!file.Open(m_file.c_str(), BaseIO::Access_Read, BaseIO::Create_OpenExisting)) return;

			GetMainContainer()->LoadAll(file, *Game->GetBuilder(), false);

			file.Close();
		}

		ZipFile file(Game->GetResourceManager());
		if(m_lvlbutton)
		{
			if(file.Open(HUDFILE, BaseIO::Access_Read, BaseIO::Create_OpenExisting))
			{
				Game->GetMainContainer()->LoadAll(file, *Game->GetBuilder(), false, true, true);
				file.Close();
			}
		}

		if(CurrentLevel>0)
		{
			lvl_timer = 0;
			XPGain = 0;
			Stars = 0;
		}

#ifdef USESCRIPTING
		if(m_sfile.size()>0) Game->m_script.Open(m_sfile.c_str());
#endif

		GameStarted = false;
		Game->GetMainContainer()->AdjustSize(*Game->GetDevice());

		ResetTimer();
	}
}

bool GameWorld::LoadFile(const wchar_t* filename, unsigned int group, bool levelbtn, int clevel, int ulevel, Resource sound, const wchar_t* scriptfile, const wchar_t* winfile)
{
	if(wcscmp(filename, L"")==0) return false;

	Unpause();
	m_sfile = scriptfile;

	if(levelbtn) 
	{
		CurrentLevel = clevel;
		UnlockLevel = ulevel;
		if(wcscmp(winfile, L"")==0) m_wingamefile = WINFILE;
		else m_wingamefile = winfile;
	}
	else
	{
		CurrentLevel = 0;
		UnlockLevel = 0;
		//m_wingamefile = L"";
	}

	if(CurrentGroup!=0) GetResourceManager()->ReleaseResourceGroup(CurrentGroup);
	GetResourceManager()->LoadResourcesByGroup(group);
	CurrentGroup = group;

	if(sound.IsValid())
	{
		GetSoundPlayer()->Stop();
		GetSoundPlayer()->SetBuffer(sound);
		GetSoundPlayer()->Play();
	}

	GetMainContainer()->QueueClear();

	m_file = filename;
	m_lvlbutton = levelbtn;
	m_processdq = true;
	return true;
}

void GameWorld::ResetAllData()
{
	CurrentLevel = 0;
	UnlockLevel = 0;
	CurrentGroup = 0;
}

void GameWorld::TogglePause()
{
#ifdef USEBOX2DPHYSICS
	GetPhysicsWorld()->SetUpdate(!GetPhysicsWorld()->GetUpdate());
#else
	m_paused = !m_paused;
#endif
}

void GameWorld::Pause()
{
#ifdef USEBOX2DPHYSICS
	GetPhysicsWorld()->SetUpdate(false);
#else
	m_paused = true;
#endif
}

void GameWorld::Unpause()
{
	ResetTimer();
#ifdef USEBOX2DPHYSICS
	GetPhysicsWorld()->SetUpdate(true);
#else
	m_paused = false;
#endif
}

bool GameWorld::GetPaused()
{
#ifdef USEBOX2DPHYSICS
	return GetPhysicsWorld()->GetUpdate();
#else
	return m_paused;
#endif
}

void GameWorld::Win()
{
	TXP += XPGain;
	settings.XP += TXP;

	int clvl = 0;
	for(unsigned int i = 0; i<MAX_LEVELS; i++)
	{
		if(((int)pow(2.0f, (float)i))&CurrentLevel)
		{
			clvl = i+1;
			break;
		}
	}

	if(clvl<MAX_LEVELS) 
	{
		if(settings.m_score[clvl] < TXP) settings.m_score[clvl] = TXP;
		if(settings.m_stars[clvl] < 0) settings.m_stars[clvl] = 0;
		else Stars = settings.m_stars[CurrentLevel];
		if(settings.m_time[clvl]==0 || settings.m_time[clvl] > lvl_timer)  settings.m_time[clvl] = lvl_timer;
	}

	settings.m_levels |= UnlockLevel;
	LoadFile(m_wingamefile.c_str());
	SaveSettings();
	GameStarted = false;
}

void GameWorld::Loose()
{
	LoadFile(LOOSEFILE);
	GameStarted = false;
}

void GameWorld::SaveSettings()
{
	//global
	C_File file(&mgr);
	wstring sepath = m_savedir;
	sepath += L"/";
	sepath += SETTINGSFILE;
	file.Open(sepath.c_str(), BaseIO::Access_Write, BaseIO::Create_CreateAlways);
	settings.Save(file);
	file.Close();
	//
}

GameWorld::~GameWorld()
{
	Release();
	SaveSettings();
	splayer.ReleaseBuffer();
	mgr.ReleaseAllResources();
	delete obc;	
	delete builder;
	delete render;
	delete dev;
	delete win;
	ZipFile::CloseZipFile();
	ZipFile2::CloseZipFile();
}

void GameWorld::SetMusicVolume(long val)
{
	settings.m_mvol = val;
	splayer.SetVolume(val);
}

void GameWorld::SetSoundVolume(long val)
{
	settings.m_svol = val;
}

C_MusicPlayer* GameWorld::GetSoundPlayer()
{
	return &splayer;
}

C_Window* GameWorld::GetWindow()
{
	return win;
}

C_Device* GameWorld::GetDevice()
{
	return dev;
}

C_Renderer* GameWorld::GetRenderer()
{
	return render;
}

ObjectsContainer* GameWorld::GetMainContainer()
{
	return obc;
}

ObjectBuilder* GameWorld::GetBuilder()
{
	return builder;
}

ResourceManager* GameWorld::GetResourceManager()
{
	return &mgr;
}

void GameWorld::Loop(unsigned int BackColor)
{ 
	wstringstream str(ios::in | ios::out);
#ifdef USE_SINGLETHREADLOADING
	InitSynch();
#endif
	if(dev->CheckDevice())
	{
		dev->Clear(BackColor);
		dev->StartDrawing();
		render->StartDrawing();
		obc->Draw(*render);

		if(ShowFPS)
		{
			str.str(L"");
			str << "FPS " << tm.FPS();
			render->DrawText(res, str.str().c_str(), 10, 10, 700, 50);
		}

		render->EndDrawing();
#ifdef USEBOX2DPHYSICS
		if(m_DebugDraw) m_world.DrawDebug();
#endif
		if(DrawBoxes) obc->DrawBoundingBox(*render);
		dev->EndDrawing();
		dev->Present();
	}
	float res = tm.Reset();
	if(res>2.0f) res = 2.0f;
	m_tdelta = res;
#ifdef USESCRIPTING
	m_script.Callback(GetMainContainer(), OBJECT_Message_Update, 0, 0);
#endif
#ifdef ANDROID
#else
#ifdef USESTREAMING
	splayer.Update(res);
#endif
#endif
	obc->Update(res);
#ifdef USEBOX2DPHYSICS
	m_world.Update(res);
#endif

	if(GameStarted)
	{
		lvl_timer += res;
	}
}

void GameWorld::ResetTimer()
{
	tm.Reset();
}

void GameWorld::UpdateMusicBuffer()
{
#ifdef USESTREAMING
	//splayer.Update(0);
#endif
}


//Text Viewer
TextViewer::TextViewer(ResourceManager* mgr, unsigned int ID, RectangleF const& location, Resource sprite, unsigned int backcolor, float rotation, bool visible, unsigned int flags, ObjectsContainer* parent, bool update) : GUIText(mgr, ID, location, sprite, backcolor, rotation, visible, flags, parent, update)
{
	m_tmp = 0;
	m_viewtype = 0;
}

void TextViewer::Save( BaseIO& str )
{
	GUIText::Save(str);
	str.Write(m_viewtype, 0, "name=View Type;help=2-Xp,5-level,6-ulevell,11-xpgain,23-txp,24-stars,25-hscore,26-paused,>100-others");
}

void TextViewer::Load( BaseIO& str )
{
	GUIText::Load(str);
	str.Read(m_viewtype);
}

Object* TextViewer::Clone()
{
	return new TextViewer(m_mgr);
}

void TextViewer::Update( float delta )
{
	if(m_viewtype == 0)
	{
		m_Text = L"";
	}
	else if(m_viewtype == 2)
	{
		m_Text = Game->settings.XP;
	}
	else if(m_viewtype == 5)
	{
		m_Text = Game->CurrentLevel;
	}
	else if(m_viewtype == 6)
	{
		m_Text = Game->UnlockLevel;
	}
	else if(m_viewtype == 11)
	{
		m_Text = Game->XPGain;
	}
	else if(m_viewtype == 23)
	{
#ifdef USE_USEBOX2DPHYSICS
		m_Text = m_tmp;
		if(m_tmp+(SCOREUPDATE_FREQ/Game->GetPhysicsWorld()->GetFrequency()) < Game->TXP)
		{
			m_tmp += (int)(SCOREUPDATE_FREQ/Game->GetPhysicsWorld()->GetFrequency());
		}
		else
		{
			m_tmp = Game->TXP;
		}
#else
		m_Text = m_tmp;
		float ufreq = Game->TXP / SCOREUPDATE_FREQ;
		if(m_tmp+(ufreq) < Game->TXP)
		{
			m_tmp += (int)(ufreq);
		}
		else
		{
			m_tmp = Game->TXP;
		}
#endif
	}
	else if(m_viewtype == 24)
	{
		if(Game->Stars == 1)
		{
			m_Text = L"*";
		}
		else if(Game->Stars == 2)
		{
			m_Text = L"*\n*";
		}
		else if(Game->Stars == 3)
		{
			m_Text = L"*\n*\n*";
		}
		else
		{
			m_Text = L"";
		}
	}
	else if(m_viewtype == 25)
	{
		m_Text =  L"";
		if(Game->CurrentLevel < MAX_LEVELS)
		{
			if(Game->settings.m_score[Game->CurrentLevel] == Game->TXP)
			{
				m_Text =  L"=>> NEW HIGH SCORE <<=";
			}
		}
	}
	else if(m_viewtype == 26)
	{
		if(Game->GetPaused() || Game->CurrentLevel == 100)
		{
			m_Text =  L"";
			m_flags |= OBJECT_Flag_Disabled;
			m_flags |= OBJECT_Flag_Invisible;
		}
		else
		{
			m_Text =  L"* PAUSED *";
			m_flags &= ~OBJECT_Flag_Disabled;
			m_flags &= ~OBJECT_Flag_Invisible;
		}
	}
	else if(m_viewtype >= 100)
	{
		DisplayMoreTexts(m_Text, m_viewtype);
	}
}

//global settings
GlobalSettings::GlobalSettings()
{
	XP = 0;
	m_levels = 1;
	m_ensound = true;
	m_enmusic = true;
	m_mvol = 10000;
	m_svol = 10000;
	memset(m_score, 0, sizeof(int) * MAX_LEVELS);
	memset(m_stars, 0, sizeof(char) * MAX_LEVELS);
	memset(m_time, 0, sizeof(float) * MAX_LEVELS);
}

bool GlobalSettings::GetMusic()
{
	bool me = false;
#ifdef ANDROID
	pthread_mutex_lock(&Game->mtx);
#endif
	me = m_enmusic;
#ifdef ANDROID
	pthread_mutex_unlock(&Game->mtx);
#endif
	return me;
}

void GlobalSettings::SetMusic(bool val)
{
#ifdef ANDROID
	pthread_mutex_lock(&Game->mtx);
#endif
	m_enmusic = val;
#ifdef ANDROID
	pthread_mutex_unlock(&Game->mtx);
#endif
}

void GlobalSettings::Save( BaseIO& str )
{
	str.Write(m_levels, 0, "name=Levels;help=Unlocked Levels.");
	str.Write(XP);
	str.Write(m_ensound);
	str.Write(GetMusic());

	for(unsigned int i = 0; i<MAX_LEVELS; i++)
	{
		str.Write(m_score[i]);
	}

	for(unsigned int i = 0; i<MAX_LEVELS; i++)
	{
		str.Write(m_stars[i]);
	}

	for(unsigned int i = 0; i<MAX_LEVELS; i++)
	{
		str.Write(m_time[i]);
	}

	str.Write(m_mvol);
	str.Write(m_svol);

	str.Write((unsigned int)extraint.size());

	for(map<string, int>::iterator i = extraint.begin(); i!=extraint.end(); i++)
	{
		str.Write(i->first);
		str.Write(i->second);
	}

	str.Write((unsigned int)extrafloat.size());

	for(map<string, float>::iterator i = extrafloat.begin(); i!=extrafloat.end(); i++)
	{
		str.Write(i->first);
		str.Write(i->second);
	}
}

void GlobalSettings::Load( BaseIO& str )
{
	str.Read(m_levels);
	str.Read(XP);
	str.Read(m_ensound);
	str.Read(m_enmusic);

	for(unsigned int i = 0; i<MAX_LEVELS; i++)
	{
		str.Read(m_score[i]);
	}

	for(unsigned int i = 0; i<MAX_LEVELS; i++)
	{
		str.Read(m_stars[i]);
	}

	for(unsigned int i = 0; i<MAX_LEVELS; i++)
	{
		str.Read(m_time[i]);
	}

	str.Read(m_mvol);
	str.Read(m_svol);

	unsigned int sz = 0;
	str.Read(sz);

	for(unsigned int i = 0; i<sz; i++)
	{
		string st;
		int data;
		str.Read(st);
		str.Read(data);
		extraint[st] = data;
	}

	sz = 0;
	str.Read(sz);

	for(unsigned int i = 0; i<sz; i++)
	{
		string st;
		float data;
		str.Read(st);
		str.Read(data);
		extrafloat[st] = data;
	}
}

//Load Button
LoadButton::LoadButton(ResourceManager* mgr, unsigned int ID, RectangleF const& location, Resource sprite, unsigned int backcolor, float rotation, bool visible, unsigned int flags, ObjectsContainer* parent, bool update) : GUIButton(mgr, ID, location, sprite, backcolor, rotation, visible, flags, parent, update)
{
	m_lvl = 0;
	m_unlocklvl = 0;
	m_layernum = 0;
	m_lvlbutton = 0;
	m_resgroup = 0;
}

void LoadButton::Update( float delta )
{
	GUIButton::Update(delta);
	if(m_lvl !=0 && !(m_lvl&Game->settings.m_levels))
	{
		m_flags |= OBJECT_Flag_Disabled;
	}
	else
	{
		m_flags &= ~OBJECT_Flag_Disabled;
	}
}

void LoadButton::Save( BaseIO& str )
{
	GUIButton::Save(str);
	str.Write(m_filename, 0, "name=File name;help=Filename.");
	str.Write(m_endgame, 0, "name=WinGame File;help=Win Game File.");
	str.Write(m_script, 0, "name=Script File;help=Script File.");
	str.Write(m_lvl, 0, "name=Level;help=Level.");
	str.Write(m_unlocklvl, 0, "name=Unlock Level;help=Unlocked Level.");
	str.Write(m_layernum, 0, "name=Layer no;help=Layer number.");
	str.Write(m_lvlbutton, 0, "name=Level button;help=Level button.");
	str.Write(m_resgroup, 0, "name=Resource group;help=Resource group.");
	str.Write(&sound, 0, "name=Music;help=Music.");
}

void LoadButton::Load( BaseIO& str )
{
	GUIButton::Load(str);
	str.Read(m_filename);
	str.Read(m_endgame);
	str.Read(m_script);
	str.Read(m_lvl);
	str.Read(m_unlocklvl);
	str.Read(m_layernum);
	str.Read(m_lvlbutton);
	str.Read(m_resgroup);
	str.Read(&sound);
}

void LoadButton::OnClick()
{
	if(IsEditing()) return;
	Game->LoadFile(m_filename.c_str(), m_resgroup, m_lvlbutton, m_lvl, m_unlocklvl, sound, m_script.c_str(), m_endgame.c_str());
}

Object* LoadButton::Clone()
{
	return new LoadButton(m_mgr);
}

SoundButton::SoundButton(ResourceManager* mgr, unsigned int ID, RectangleF const& location, Resource sprite, unsigned int backcolor, float rotation, bool visible, unsigned int flags, ObjectsContainer* parent, bool update) : GUIButton(mgr, ID, location, sprite, backcolor, rotation, visible, flags, parent, update)
{
	m_music = false;
	m_flags |= OBJECT_Flag_BTypePush;
}

void SoundButton::UpdateState()
{
	if(Game==0) return;
	if(m_music)
	{
		if(Game->settings.GetMusic())
		{
			SetState(GUIButton_State_Pressed);
		}
		else
		{
			SetState(GUIButton_State_Normal);
		}
	}
	else
	{
		if(Game->settings.m_ensound)
		{
			SetState(GUIButton_State_Pressed);
		}
		else
		{
			SetState(GUIButton_State_Normal);
		}
	}
}

void SoundButton::Update( float delta )
{
	GUIButton::Update(delta);
}

void SoundButton::SetState(unsigned int st)
{
	if(st==GUIButton_State_Pressed)
	{
		if(Game==0) return;
		if(m_music)
		{
			Game->EnableMusic(true);
		}
		else
		{
			Game->EnableSound(true);
		}
	}
	else if(st==GUIButton_State_MouseOver && GetState()==GUIButton_State_Pressed)
	{
		if(m_music)
		{
			Game->EnableMusic(false);
		}
		else
		{
			Game->EnableSound(false);
		}
	}
	else if(st==GUIButton_State_Normal && GetState()==GUIButton_State_Pressed)
	{
		if(m_music)
		{
			Game->EnableMusic(false);
		}
		else
		{
			Game->EnableSound(false);
		}
	}
	GUIButton::SetState(st);
}

Object* SoundButton::Clone()
{
	return new SoundButton();
}

void SoundButton::Save( BaseIO& str )
{
	GUIButton::Save(str);
	str.Write(m_music, 0, "name=Music");
}

void SoundButton::Load( BaseIO& str )
{
	GUIButton::Load(str);
	str.Read(m_music);
	UpdateState();
}

//progress viewer
ProgressViewer::ProgressViewer(ResourceManager* mgr, unsigned int ID, RectangleF const& location, Resource sprite, unsigned int backcolor, float rotation, bool visible, unsigned int flags, ObjectsContainer* parent, bool update) : GUIProgress(mgr, ID, location, sprite, backcolor, rotation, visible, flags, parent, update)
{
	m_viewtype = 0;
}

void ProgressViewer::Save( BaseIO& str )
{
	GUIProgress::Save(str);
	str.Write(m_viewtype, 0, "name=View Type");
}

void ProgressViewer::Load( BaseIO& str )
{
	GUIProgress::Load(str);
	str.Read(m_viewtype);
}

Object* ProgressViewer::Clone()
{
	return new ProgressViewer();
}

void ProgressViewer::Update( float delta )
{
	float progress = 0.0f;
	DisplayMoreProgress(progress, m_viewtype);
	SetValue(progress);
}

//Save

SaveConfig::SaveConfig(ResourceManager* mgr, unsigned int ID, RectangleF const& location, Resource sprite, unsigned int backcolor, float rotation, bool visible, unsigned int flags, ObjectsContainer* parent, bool update) : GUIButton(mgr, ID, location, sprite, backcolor, rotation, visible, flags, parent, update)
{
	m_type |= OBJECT_TYPE_SAVE;
	m_flags |= OBJECT_Flag_Disabled | OBJECT_Flag_BTypePush;
	m_effectID = 0;
	m_upgradecost = 0;
	m_proc = false;
}

void SaveConfig::UpgradeLinks(bool val)
{
	if(!m_parent) return;
	list<Object*>::iterator li = m_parent->GetObjectInteratorBegin();
	for(; li!=m_parent->GetObjectInteratorEnd(); li++)
	{
		if((*li) == this) continue;
		if((*li)->GetType()&OBJECT_TYPE_SAVE)
		for(unsigned int i = 0; i<m_links.size(); i++)
		{
			if((*li)->GetID()==m_links[i])
			{
				if(!val)
				{
					((SaveConfig*)(*li))->m_flags |= OBJECT_Flag_Disabled;
					((SaveConfig*)(*li))->SetEnableEffect(false);
				}
				else 
				{
					((SaveConfig*)(*li))->m_flags &= ~OBJECT_Flag_Disabled;
					((SaveConfig*)(*li))->SetEnableEffect(false);
				}
			}
		}
	}
}

bool SaveConfig::GetEnableEffect()
{
	if(GetState()==GUIButton_State_Pressed)
	{
		return true;;
	}
	else
	{
		return false;
	}
}

void SaveConfig::SetEnableEffect(bool val)
{
	m_proc = true;
	if(val)
	{
		SetState(GUIButton_State_Pressed);
#ifdef USESCRIPTING
		SetStoreInt("U_" + ename, 1);
#endif
	}
	else
	{
		SetState(GUIButton_State_Normal);
#ifdef USESCRIPTING
		SetStoreInt("U_" + ename, 0);
#endif
	}
}

void SaveConfig::Save( BaseIO& str )
{
	m_loading = true;
	GUIButton::Save(str);
	str.Write(GetEnableEffect(), 0, "name=Enable Effect.;category=Effect Config");
	str.Write(m_effectID, 0, "name=EffectID.");
	str.Write(m_upgradecost, 0, "name=Upgrade Cost.");

	string st;
	stringstream s;
	for(unsigned int i = 0; i<m_links.size(); i++)
	{
		s << m_links[i];
		if(i!=m_links.size()-1) s << ",";
	}
	st = s.str();

	str.Write(st, 0, "name=Links;help=Tech tree links.");
	str.Write(ename, 0, "name=Unlock Name");
	m_loading = false;
}

void SaveConfig::Load( BaseIO& str )
{
	m_loading = true;
	bool m_enableeffect = false;
	GUIButton::Load(str);
	str.Read(m_enableeffect);
	str.Read(m_effectID);
	str.Read(m_upgradecost);

	string st;
	str.Read(st);
	m_links.clear();
	GetStringArray(m_links, st.c_str(), ',');
	str.Read(ename);
	SetEnableEffect(m_enableeffect);
	m_loading = false;
}

bool SaveConfig::Init()
{
	UpgradeLinks(GetEnableEffect());
	return true;
}

Object* SaveConfig::Clone()
{
	SaveConfig* obj = new SaveConfig(m_mgr);
	*obj = *this;
	return obj;
}

void SaveConfig::SetState(unsigned int st)
{
	if(m_loading || IsEditing())
	{
		GUIButton::SetState(st);
		return;
	}
	/*if(!m_proc && (GetState()==GUIButton_State_Pressed && st==GUIButton_State_Normal))
	{
		if(Game->settings.XP >= m_buildcost)
		{
			Game->settings.XP -= m_buildcost;
			GUIButton::SetState(GUIButton_State_Pressed);
		}
		return;
	}*/
	m_proc = false;
	
	if(st==GUIButton_State_Pressed)
	{
		if(Game->settings.XP >= m_upgradecost)
		{
			GUIButton::SetState(st);
			UpgradeLinks(true);
			Game->settings.XP -= m_upgradecost;
		}
		return;
	}
	else if(st==GUIButton_State_MouseOver && GetState()==GUIButton_State_Pressed)
	{
		GUIButton::SetState(GUIButton_State_Normal);
		UpgradeLinks(false);
		Game->settings.XP += m_upgradecost;
		return;
	}
	else if(st==GUIButton_State_Normal && GetState()==GUIButton_State_Pressed)
	{
		GUIButton::SetState(GUIButton_State_Normal);
		UpgradeLinks(false);
		Game->settings.XP += m_upgradecost;
		return;
	}

	GUIButton::SetState(st);
}
