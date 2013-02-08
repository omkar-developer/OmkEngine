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

/** Identifier for object type. \addtogroup ObjectTypes
* \{
*/
#define OBJECT_TYPE_GAMEOBJECT	0x400000		///<Abstract weapon object type.
/** \} */

/** \addtogroup ObjectMessages
* \{
*/
#define OBJECT_Message_GameWin				1003 ///<This message is sent after Win or when Win() called from script.
#define OBJECT_Message_GameLoose			1004 ///<This message is sent after Loose or when Loose() called from script.
#define OBJECT_Message_GameStep				1005 ///<Message sent at each game step (loop).
/** \} */

/** \addtogroup GlobalConstants
* \{
*/
#ifndef MAX_LEVELS
#define MAX_LEVELS 20 ///<Max number of levels this game has used to create array to store score etc for each level (must be less than 32).
#endif

#ifndef SCRIPTFILE
#define SCRIPTFILE L"Script.as" ///<Path of the script file to be loaded at start.
#endif

#ifndef SETTINGSFILE
#define SETTINGSFILE L"settings.cfg" ///<Path name of the settings files where all game seetings are stored.
#endif

#ifndef HUDFILE
#define HUDFILE L"hud.cfg" ///<Path name to the GUI/Interface file for levels.
#endif

#ifndef WINFILE
#define WINFILE L"win.cfg" ///< Default path to the interface file which will be shown on game win.
#endif

#ifndef LOOSEFILE
#define LOOSEFILE L"loose.cfg" ///<Default path to the interface file which will be shown on game loose.
#endif

#ifndef ZIPRESOURCEFILE
#define ZIPRESOURCEFILE L"Resources.zip" ///<Path to resources zip file where all resources file are stores.
#endif

#ifndef SAVEDIRECTORY
#define SAVEDIRECTORY L"NewSavedGames" ///<Path to directory where settings files are saved.
#endif

#ifndef RESOURCEFILE
#define RESOURCEFILE L"resource.r" ///<Name/Path of the resource file where all resource configurations are stored (this file is created using resourceeditor).
#endif

#ifndef LOADINGFILE
#define LOADINGFILE L"Loading.cfg" ///<Name/Path of the interface shown when loading at start.
#endif

#ifndef MENUMUSIC
#define MENUMUSIC "menumusicsound" ///<Resource name of the music file played on main menu.
#endif

#ifndef MAINMENUFILE
#define MAINMENUFILE L"mainmenu.lvl" ///<Path of the interface file in resources zip file which will be loaded for main menu.
#endif

#ifndef OBJECTSCONFIG
#define OBJECTSCONFIG L"objects.cfg" ///<Path of the objects files which are added to componants list.
#endif

#ifndef SCOREUPDATE_FREQ
#define SCOREUPDATE_FREQ 500.0f ///<Frequency for text animation showed in scores view.
#endif
/** \} */

/**
 *	Views texts according to provided view type.
 *	Default view types :\n
 *	2-Xp shows current xp,\n
 *	5-level Shows current level number,\n
 *	6-ulevell Shows which level will be unlocked after winning current level,\n
 *	11-xpgain Shows Accumulated XP for current level,\n
 *	23-txp Shows total XP,\n
 *	24-stars Shows starts accuired for current level,\n
 *	25-hscore Shows High score text here if current score was more than previous otherwise this is empty,\n
 *	26-paused Shows PAUSED if game is paused otherwise empty,\n
 *	>100-user defined texts by implementing DisplayMoreTexts function.
 */
class TextViewer : public GUIText
{
protected:
	unsigned int m_viewtype; ///< Which text to view is stored here.
	int m_tmp; ///< Just a temporary variable used to number counting text effect.

public:
	/** \copydoc GUIText::GUIText(ResourceManager*, unsigned int, RectangleF const&, Resource, unsigned int backcolor, float rotation, bool, unsigned int, ObjectsContainer* parent, bool) */
	TextViewer(ResourceManager* mgr = 0, unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true);

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual Object* Clone();

	virtual void Update( float delta );
};

/**
 * Views progress data for provided type/view number.
 * Default view numbers : None\n
 * implement DisplayMoreProgress to display progress for specified types.
 */
class ProgressViewer : public GUIProgress
{
protected:
	unsigned int m_viewtype; ///< Type/Number of the data to view in progress.

public:
	/** \copydoc GUIText::GUIText(ResourceManager*, unsigned int, RectangleF const&, Resource, unsigned int backcolor, float rotation, bool, unsigned int, ObjectsContainer* parent, bool) */
	ProgressViewer(ResourceManager* mgr = 0, unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true);

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual Object* Clone();

	virtual void Update( float delta );
};

/**
 * Saves/Loads settings and levels stats like scores/stars for each level etc (instance of this class is automatically managed by GameWorld class).
 */
class GlobalSettings
{
protected:
	bool m_enmusic; ///<Music enabled state.

public:
	unsigned int m_levels; ///<Currently locked and unlocked levels state.
	int XP; ///<Total XP acquired by the user.
	bool m_ensound; ///<Sound effects are enabled state.

	long m_mvol; ///<Music volume
	long m_svol; ///<Sound volume

	int m_score[MAX_LEVELS]; ///<Array of scores containing score of each level.
	char m_stars[MAX_LEVELS]; ///<Array of scores containing stars of each level.
	float m_time[MAX_LEVELS]; ///<Array of scores containing time required to complete each level.

	map<string, int> extraint; ///<Map of int values to be stored as settings.
	map<string, float> extrafloat; ///<Map of float values to be stored as settings.

	GlobalSettings(); ///< Default constructor.

	/**
	 * Return true if music is enabled otherwise false.
	 * \return true if music is enabled otherwise false.
	 */
	bool GetMusic();

	/**
	 * Sets music enable state.
	 * \param val if true music is enabled, if false music is disabled.
	 */
	void SetMusic(bool val);

	/** \copydoc Object::Save(BaseIO&) */
	virtual void Save( BaseIO& str );
	/** \copydoc Object::Load(BaseIO&) */
	virtual void Load( BaseIO& str );
};

/**
 * Used to load specified interface/level file when user clicks this button.
 */
class LoadButton : public GUIButton
{
protected:
	wstring m_filename; ///< Path name of the file in resources to be loaded (path is relative to resources).
	unsigned int m_lvl; ///< Current level to set when this is a level button.
	unsigned int m_unlocklvl; ///< Unlock level to set when loaded level is completed(won) by user.
	unsigned int m_layernum; ///< Currently not used.
	bool m_lvlbutton; ///< Whether this button is used to load level or interface (if level button all m_lvl, m_unlocklevel values are set).
	Resource sound; ///< Music resource to be played when specified file is loaded.
	unsigned int m_resgroup; ///< Resource group to be loaded when loading this file. (this group is automatically released after level is completed).
	wstring m_endgame; ///< Path name of the interface file to be loaded when level is completed by winning.
	wstring m_script; ///< Path name of the script file to be loaded with this interface/level.

public:
	/** \copydoc GUIText::GUIText(ResourceManager*, unsigned int, RectangleF const&, Resource, unsigned int backcolor, float rotation, bool, unsigned int, ObjectsContainer* parent, bool) */
	LoadButton(ResourceManager* mgr = 0, unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true);

	virtual void Update( float delta );

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual void OnClick();

	virtual Object* Clone();
};

/**
 * This button is used to turn on/off sound/music.
 */
class SoundButton : public GUIButton
{
protected:
	bool m_music; ///< If true this button manages music otherwise sound.
	
public:
	/** \copydoc GUIText::GUIText(ResourceManager*, unsigned int, RectangleF const&, Resource, unsigned int backcolor, float rotation, bool, unsigned int, ObjectsContainer* parent, bool) */
	SoundButton(ResourceManager* mgr = 0, unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true);

	virtual void Update( float delta );

	virtual Object* Clone();

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	/**
	 * Set current sound/music enabled state.
	 * \param st sound/music enabled state.
	 */
	void SetState(unsigned int st);

	/**
	 * Synchronizes music/sound state with current button pressed state.
	 */
	void UpdateState();
};

/**
 * Used to save/manage configurations/effects like making unlockable powers ( If button is pressed effect is enabled in not effect is disabled ).
 * Use GetStoreInt(ename) function to retrieve current state of this effect, function returns 1 if this effect is enabled otherwise false.
 */
class SaveConfig : public GUIButton
{
protected:
	//bonuses
	bool m_proc; ///< Currently not used but tells if effect has been set or not.
	bool m_loading; ///< Whether load has been processed (used for synchronization when loading).
	string ename; ///< Name of the effect/configuration used to set/save effect in file by this name (GetStoreInt(ename) retrieves effect state).

public:
	int m_effectID; ///< Effect ID.
	int m_upgradecost; ///< Amount of XP is required to enable this effect.
	vector<int> m_links; ///< Links to other effect/SaveConfig buttons by their IDs (if this effect is disabled all effects linked to this effect are automatically disabled).

	/** \copydoc GUIText::GUIText(ResourceManager*, unsigned int, RectangleF const&, Resource, unsigned int backcolor, float rotation, bool, unsigned int, ObjectsContainer* parent, bool) */
	SaveConfig(ResourceManager* mgr = 0, unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true);

	/**
	 * Updates all linked effects with current state of this effect (this is automatically called when button is enabled or disabled).
	 * \param val true to enable all linked effects, false to disable all linked effects.
	 */
	void UpgradeLinks(bool val = false);

	/**
	 * Initializes and synchronizes all linked effects with current effect.
	 * \return always true.
	 */
	virtual bool Init();

	virtual void SetState(unsigned int st);

	/**
	 * Returns true if effect is enabled otherwise false.
	 * \return true if effect is enabled otherwise false.
	 */
	bool GetEnableEffect();

	/**
	 * Sets effect enabled state.
	 * \param val true to enable effect otherwise false.
	 */
	void SetEnableEffect(bool val);

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual Object* Clone();
};

/**
 * Main class containing whole game world objects, resources, windows, devices, renderer and their operations.
 * This object initializes game world environment and loads resources/files to be used in game.\n
 * This object also sets current global variable named Game to this newly created game world.
 */
class GameWorld
{
private:
	C_Window* win; ///< Pointer to the current window.
	C_Device* dev; ///< Pointer to the current video device object.
	C_SoundDevice sdevice; ///< Pointer to the current SoundDevice object.
	C_Renderer* render; ///< Pointer to the current renderer.
	C_Timer tm; ///< Timer used to synch this game world/rendering according to the time.
	ObjectsContainer* obc; ///< Pointer to the root objects container.
	ObjectBuilder* builder; ///< Pointer to the builder object used to build all object in this game world.
	ResourceManager mgr; ///< Resource manager.

#ifdef USEBOX2DPHYSICS
	PhyWorld m_world; ///< Physics world/Objects manager.
	DebugDraw m_debugdraw; ///< Debug drawing state.
#endif

	Resource res; ///< Temporary resource holder for font to show FPS.

#ifndef USEBOX2DPHYSICS
	bool m_paused; ///< Game Paused state.
#endif

	bool m_processdq; ///< Set to true to process delete queue o next loop.
	wstring m_file; ///< Currently loaded file.
	bool m_lvlbutton; ///< Not used.
	wstring m_sfile; ///< Game settings save/load file path.

	bool m_allloaded; ///< Resource loading state.

	/**
	 * Called to finalize initialization when all initializing is done.
	 */
	void FinalizeInit();
	
public:
	AnimationTimeline* timeline; ///< Current Timeline object 0 if there are none.

	bool m_init; ///< Weather Initializing has been done.
	bool m_lopped; ///< Used by Android to determine first loop.
	bool m_closing; ///< Used by Android to determine if application is closing and need to destroyed.
	wstring m_savedir; ///< Game settings save/load directory path.

#ifdef USESTREAMING
	C_MusicPlayer splayer; ///< Sound player object used to play music.
#else
	C_SoundPlayer splayer; ///< Sound player object used to play music.
#endif	

#ifdef ANDROID
	pthread_t thread1; ///< thread used on Android to decode music stream (deprecated).
	bool clostthread; ///< Whether to close music stream decoder thread (deprecated).
	pthread_mutex_t mtx; ///< Mutex for music stream decoder threading (deprecated).
#endif

#ifdef USESCRIPTING
	Script m_script; ///< Script engine used for current game world.
#endif

	bool GameStarted; ///< Whether game has been started.

#ifdef USEBOX2DPHYSICS
	bool m_DebugDraw; ///< Whether to enable debug draw for all objects.
#endif

	bool ShowFPS; ///< Whether to show FPS at top or not.
	bool DrawBoxes; ///< Whether to draw boxes around every object.

	int CurrentLevel; ///< Current level number.
	int UnlockLevel; ///< Level to be unlocked if user wins current level.
	unsigned int CurrentGroup; ///< Currently loaded resource group which will be released when new file is loaded with different resource group.

	int XPGain;  ///< XP for current level if user wins this XP is added to the total XP.
	int TXP;  ///< Total XP acquired by user.
	char Stars; ///< Total number of stars acquired for current level.
	//global
	GlobalSettings settings; ///< Manages game settings.
	wstring m_wingamefile; ///< This file will be loaded after user wins current level.

	//Loading status
	int m_loadst; ///< Used for current loading status.

	float lvl_timer; ///< Used to count level time.

	float m_tdelta; ///< Stores current time delta in seconds.

	bool m_resourcesloaded; ///< Resource load state;
	
	/**
	 * GameWorld constructor prepares this object and opens base resource files.
	 * \param title Title of the game/window.
	 * \param winwidth Window width.
	 * \param winheight Window height.
	 * \param fullscreen Whether application should run in full screen mode or not.
	 */
	GameWorld(const wchar_t* title, int winwidth, int winheight, bool fullscreen = false);

	/**
	 * Virtual Game World destructor releases all resources and files loaded by this game world object.
	 */
	virtual ~GameWorld();
	
	/** Called to process Delete queue (delete queue id list of objects to be deleted) */
	void OnDeleteQueue();

	/**
	 * Sets music volume.
	 * \param val music volume to set.
	 */
	void SetMusicVolume(long val);

	/**
	 * Sets sound volume.
	 * \param val sound volume to set.
	 */
	void SetSoundVolume(long val);

	/**
	 * Clears all object and resets all data for this object.
	 */
	void ResetAllData();

	/**
	 * Toggles pause state.
	 */
	void TogglePause();

	/**
	 * Pauses game to prevent world update on call to Update().
	 */
	void Pause();

	/**
	 * Resumes game so that update function is processed again.
	 */
	void Unpause();

	/**
	 * Returns current pause state of the game.
	 * \return current pause state of the game.
	 */
	bool GetPaused();

	/**
	 * Called to set win for current level.
	 */
	void Win();

	/**
	 * Called to set loose status for current level.
	 */
	void Loose();

	/**
	 * Used to load interface/level file.
	 * \param filename Filename to load (pathname for file in resources zip file).
	 * \param group Resource group to load with this file.
	 * \param levelbtn Set true to load level and set current level and unlocklevel to values specified (also loads HUD/level Interface file).
	 * \param clevel If levelbtn is true this value is set to current level.
	 * \param ulevel If levelbtn is true this value is set to unlock level which will be unlocked if user wins current level.
	 * \param sound music resource to play when this file is loaded.
	 * \param scriptfile Path of the script file to be loaded with this this file.
	 * \param winfile Path of the file to be loaded when user wins current level.
	 * \return
	 */
	bool LoadFile(const wchar_t* filename, unsigned int group = 0, bool levelbtn = false, int clevel = 0, int ulevel = 0, Resource sound = 0, const wchar_t* scriptfile = SCRIPTFILE, const wchar_t* winfile = WINFILE);
	
	/**
	 * Returns pointer to the current window used by this game world object.
	 * \return pointer to the current window used by this game world object.
	 */
	C_Window* GetWindow();

	/**
	 * Returns pointer to the current video device used by this game world object.
	 * \return pointer to the current video device used by this game world object.
	 */
	C_Device* GetDevice();

	/**
	 * Returns pointer to the current renderer object used by this game world object.
	 * \return pointer to the current renderer object used by this game world object.
	 */
	C_Renderer* GetRenderer();

	/**
	 * Returns pointer to the root objects container used by this game world object.
	 * \return pointer to the root objects container used by this game world object.
	 */
	ObjectsContainer* GetMainContainer();

	/**
	 * Returns pointer to the objects builder used by this game world object.
	 * \return pointer to the objects builder used by this game world object.
	 */
	ObjectBuilder* GetBuilder();

	/**
	 * Returns pointer to the resource manager used by this game world object.
	 * \return pointer to the resource manager used by this game world object.
	 */
	ResourceManager* GetResourceManager();

#ifdef USEBOX2DPHYSICS
	/**
	 * Returns pointer to the physics objects container/manager used by this game world object.
	 * \return pointer to the physics objects container/manager used by this game world object.
	 */
	PhyWorld* GetPhysicsWorld();
#endif

	/**
	 * Returns pointer to the sound device used by this game world object.
	 * \return pointer to the sound device used by this game world object.
	 */
	C_MusicPlayer* GetSoundPlayer();
	void ResetTimer();

	/**
	 * Called to process and render all objects.
	 * \param BackColor Background color to be used when clearing scene.
	 */
	void Loop(unsigned int BackColor = 0xFF000000);

	/**
	 * Updates music buffer (deprecated).
	 */
	void UpdateMusicBuffer();

	//void Callback(Object* obj, unsigned int Msg, unsigned int Param1, void* Param2);
	/**
	 * Called to give resource loading status.
	 * \param status current resource loading progress in percents.
	 */
	void OnLoading(int status);

	/**
	 * Quits application and releases all resources.
	 */
	void QuitApp();

	/**
	 * Initializes this object by loading all resources and files in one call.
	 */
	void Init();

	/**
	 * Enables resources to be loaded 1 by 1 each game loop so this function is called repeatedly till all resources are loaded.
	 * \return true if all initializing was done else false.
	 */
	bool InitSynch();

	/**
	 * Sets enable state of music.
	 * \param val Music enabled state.
	 */
	void EnableMusic(bool val);

	/**
	 * Sets enable state of sound effects.
	 * \param val Sound effects enabled state.
	 */
	void EnableSound(bool val);

	/**
	 * Saves all game settings like scores, XP etc.
	 */
	void SaveSettings();

	/**
	 * Returns true if everything is loaded.
	 * \return true if everything is loaded.
	 */
	bool GetAllLoaded();

	//virtual functions
	/**
	 * Called to do additional initialization.
	 */
	virtual void Initialize() = 0;

	/**
	 * Called to register additional objects.
	 * \param v_builder register new objects using this builder.
	 */
	virtual void RegisterObjects(ObjectBuilder* v_builder) = 0;

	/**
	 * Called when game world is about to be destroyed.
	 */
	virtual void Release(){};

#ifdef USESCRIPTING
	/**
	 * Called to register additional script interface.
	 */
	virtual void RegisterScriptObjects(Script* engine) = 0;
#endif
};

/** \addtogroup GlobalFunctions
* \{
*/

/**
 * Implement this to return your own derived GameWorld object.
 * \param title Tells title to be used for window.
 * \param winwidth Window width.
 * \param winheight Window Height.
 * \param fullscreen Whether application is fullscreen or not.
 * \return Return pointer to the derived game object created using new keyword (this will be automatically deleted after its use).
 */
extern GameWorld* GetGameAPI(const wchar_t* title, int winwidth, int winheight, bool fullscreen = false);

/**
 * Implement this function to write more texts using text viewer.
 * \param txt Set text to be written for given id.
 * \param id ID of the text to write.
 */
extern void DisplayMoreTexts(UIWString& txt, unsigned int id);

/**
 * Implement this function to show more progresses using progress viewer.
 * \param progress Progress value for current id in percents.
 * \param id ID of the progress value being retrieved.
 */
extern void DisplayMoreProgress(float& progress, unsigned int id);

/** \} */

/**
 * Global Game object initialized after making first GameWorld object.
 */
extern GameWorld* Game;
