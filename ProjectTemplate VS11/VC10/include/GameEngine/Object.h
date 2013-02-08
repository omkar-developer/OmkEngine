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

#define OBJ_VERSION 01ul

#define COLOR_BLACK		0xff000000
#define COLOR_WHITE		0xffffffff
#define COLOR_GREEN		0xff00ff00
#define COLOR_BLUE		0xff0000ff
#define COLOR_RED		0xffff0000
#define COLOR_GRAY		0xff808080
#define COLOR_AQUA		0xff00ffff
#define COLOR_YELLOW	0xffffff00
#define COLOR_CYAN		0xff00ffff

/** \addtogroup ObjectTypes
* \{
*/
#define OBJECT_TYPE_DRAWABLE	0x2		///<Drawable object type.
#define OBJECT_TYPE_OBJECT		0x1		///<Abstract base object type.
#define OBJECT_TYPE_CONTAINER	0x4		///<Object Container type.
#define OBJECT_TYPE_EDITOR		0x8		///<Object Editor type.
#define OBJECT_TYPE_TEXT		0x10	///<Text object type.
#define OBJECT_TYPE_BUTTON		0x20	///<Button object type.
#define OBJECT_TYPE_EDIT		0x40	///<Edit object type.
#define OBJECT_TYPE_PROGRESS	0x80	///<Progressbar object type.
#define OBJECT_TYPE_CIRCLEMENU	0x100	///<Circle menu object type.
#define OBJECT_TYPE_TIMER		0x200	///<Timer object type.
#define OBJECT_TYPE_IMAGESCROLL	0x400	///<Image Scroller object type.
#define OBJECT_TYPE_DIAL		0x800	///<Dial object type.
#define OBJECT_TYPE_PARTICLEEN	0x1000	///<Particle engine object type.
#define OBJECT_TYPE_ANITIMELINE	0x2000	///<Animation timeline object type.
#define OBJECT_TYPE_ANIMABLE	0x4000	///<Animable object type.
#define OBJECT_TYPE_PHYSICSOBJ	0x8000	///<Animable object type.
#define OBJECT_TYPE_SAVE		0x10000
#define OBJECT_TYPE_PHYJOINT	0x20000
#define OBJECT_TYPE_SOUNDPLAYER	0x40000
/// \}

/** \addtogroup ObjectMessages
* \{
*/
#define OBJECT_Message_Draw					0
#define OBJECT_Message_LMouseDown			1
#define OBJECT_Message_LMouseUp				2
#define OBJECT_Message_MouseMove			3
#define OBJECT_Message_LMouseClick			4
#define OBJECT_Message_KeyState				5
#define OBJECT_Message_Release				7
#define OBJECT_Message_EDAddChar			8
#define OBJECT_Message_EDDelete				9
#define OBJECT_Message_EDMove				10
#define OBJECT_Message_EDTextChange			11
#define OBJECT_Message_EDRedrawText			12
//#define OBJECT_Message_BUpdateAnimation		13
#define OBJECT_Message_PValueChanged		14
#define OBJECT_Message_LSelectionChanged	15
#define OBJECT_Message_BSetChecked			16 
#define OBJECT_Message_CSelectionChanged	17
#define OBJECT_Message_LSelected			18
#define OBJECT_Message_CnUpdatePositions	19
#define OBJECT_Message_RMouseDown			20
#define OBJECT_Message_RMouseUp				21
#define OBJECT_Message_MouseDown			23
#define OBJECT_Message_MouseUp				24
#define OBJECT_Message_MouseScroll			25
#define OBJECT_Message_BMouseOver			26
#define OBJECT_Message_BMouseNormal			27
#define OBJECT_Message_Init					28
#define OBJECT_Message_KeyDown				29
#define OBJECT_Message_KeyUp				30
#define OBJECT_Message_KeyChar				31
#define OBJECT_Message_SelEdit				32
#define OBJECT_Message_SelClear				33
#define OBJECT_Message_Resize				34
#define OBJECT_Message_OnTimer				35
#define OBJECT_Message_Collision			36
#define OBJECT_Message_SpriteFinished		37
#define OBJECT_Message_Update				38
#define OBJECT_Message_TileClicked			39
#define OBJECT_Message_EndContact			40
/** \} */
//Object Flags
//Control Flags
#define OBJECT_Flag_Invisible			1 //0
#define OBJECT_Flag_Disabled			2 //1
#define OBJECT_Flag_NoBackground		4 //2
#define OBJECT_Flag_Tansform			8 //3
#define OBJECT_Flag_IgnoreMouse			16 //4
#define OBJECT_Flag_ChangeByVelocity	32 //5
#define OBJECT_Flag_BTypePush			2097152 //21
#define OBJECT_Flag_BTypeCheck			4194304 //22
#define OBJECT_Flag_BTypeRadio			8388608 //23
#define OBJECT_Flag_PTypeVertical		2097152 //21
#define OBJECT_Flag_PTypeSlider			4194304 //22
#define OBJECT_Flag_PTypeDecimal		8388608 //23
#define OBJECT_Flag_PTypeInv			16777216 //24
#define OBJECT_Flag_LItemSelected		2097152 //21
#define OBJECT_Flag_LTypeMultiSelect	4194304 //22
#define OBJECT_Flag_ETypePass			2097152 //21

//Button States
#define GUIButton_State_Normal		0
#define GUIButton_State_Pressed		1
#define GUIButton_State_MouseOver	2

//class ObjectsContainer;
class ObjectsContainer;
class ObjectBuilder;

#define ERR_CONST 4294967295u
/**
 Object. 
 */

struct Camera2D
{
	RectangleF m_area;
	PointF m_pos;
	Size m_size;
	float m_scale;
	float m_zoom;
	float m_maxzoom;
	Size m_max;

	Camera2D(const RectangleF& rect, float scale = 1.0f, float zoom = 1.0f)
	{
		m_area = rect;
		m_scale = scale;
		m_zoom = zoom;
		m_maxzoom = 2.0f;
	}

	Camera2D()
	{
		m_scale = 1.0f;
		m_zoom = 1.0f;
		m_maxzoom = 2.0f;
	}
};

class Object;
class DrawableObject;

class ObjectConnection
{
private:
	DrawableObject* cobj;
	unsigned int ConID;
	void AddConnection(DrawableObject& obj);
	void DeleteConnection(DrawableObject& obj);

public:
	void Copy(const ObjectConnection& cpy);
	ObjectConnection(const ObjectConnection& cpy);
	const ObjectConnection& operator=(const ObjectConnection& cpy);
	ObjectConnection();
	ObjectConnection(DrawableObject* cobj);

	virtual ~ObjectConnection();
	void UpdateConnection();
	void Disconnect();
	void SetConnectionID(unsigned int ID);
	unsigned int GetConnectionID();
	DrawableObject* GetObject();
	void SetObject(DrawableObject* obj);
	void SetConnectionIDNU(unsigned int ID);
	const DrawableObject* GetConstObject() const;
	bool IsValid();

	bool operator<(const ObjectConnection& str) const
	{
		return cobj < str.cobj;
	}

	Signal1<DrawableObject*> Disconnected;

	friend class DrawableObject;
};

class Object
{
protected:
	list<Object*>::iterator it;
	bool m_update;
	protected:
	int m_type;
	string name;
	unsigned int m_ID;
	unsigned int m_objver;

public:
	map<string, float> extrafloat;
	ObjectsContainer* m_parent;
	unsigned int m_flags;
	string m_objname;
	unsigned int m_objtype;

	void SetProperty(const char* name, float val);
	float GetProperty(const char* name);
	void ClearProperties();

	static string ReadType(BaseIO& str);

	int GetType();

	Object( unsigned int ID = 0, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true );

	void SetID(unsigned int val);
	unsigned int GetID();

	unsigned int GetObjectVersion();

	const string& GetName();

	virtual void QueueForDelete()=0;

	virtual bool GetUpdate();
	virtual void SetUpdate(bool val);

	virtual bool Init()=0;
	virtual void Release()=0;
	virtual void Update( float delta )=0;
	virtual void Draw(C_Renderer& rnd)=0;
	virtual Object* Clone()=0;
	virtual ~Object(){};

	ObjectsContainer* GetParent();

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );
	virtual void PostLoadAll(){};

	/** brief.  
	* \param Msg Object Message.
	* \link ObjectMessages Messages \endlink
	* \param Param1 First parameter.
	* \param Param2 Second parameter.
	**/
	virtual void Process( unsigned int Msg, unsigned int Param1, void* Param2 ) = 0;

	friend class ObjectsContainer;
	friend class ObjectBuilder;
};

class DrawableObject : public Object
{
private:
	set<ObjectConnection*> connections;
	friend void ObjectConnection::AddConnection(DrawableObject& obj);
	friend void ObjectConnection::DeleteConnection(DrawableObject& obj);
	DrawableObject(const DrawableObject& obj);

protected:
	RectangleF m_location;
	float m_rotation;
	Sprite m_sprite;
	bool m_editable;
	float m_layer;

public:	
	unsigned int m_backcolor;
	float m_depth;

	DrawableObject( unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true );
	virtual ~DrawableObject();
	
	const DrawableObject& operator=(const DrawableObject& obj);

	virtual void DrawBoundingBox(C_Renderer& rnd);

	virtual void QueueForDelete();
	void Delete();
	void ClearConnections();

	//properties
	virtual RectangleF& GetObjectArea();
	virtual void SetObjectArea(const RectangleF& val);

	virtual float GetAngle();
	virtual void SetAngle(float val);

	virtual bool GetVisible();
	virtual void SetVisible(bool val);

	virtual unsigned int GetBackColor();
	virtual void SetBackColor(unsigned int val);

	virtual Sprite& GetSprite();
	virtual void SetSprite(Sprite const& val);

	virtual bool TestPoint(const PointI& pt);

	float GetLayer();
	void SetLayer(float val);

	// Sets the Capture.
	RectangleF ToRelativePos(RectangleF const& pos, bool transform = false);
	RectangleF GetAbsolutePos(const RectangleF* pos = 0);
	void SetCapture();
	void ReleaseCapture();
	void SetSelected();
	bool IsSelected();
	bool IsCaptured();
	bool IsDrawTop();
	bool IsEditing();
	void SetDrawTop();
	void ReleaseDrawTop();
	bool GetSelected();
	void ReleaseSelected();
	void ReleaseEditor();
	virtual void Release(){};

	virtual void Process( unsigned int Msg, unsigned int Param1, void* Param2 );

	virtual void Update( float delta );
	virtual bool Init();
	virtual void Draw(C_Renderer& rnd);

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual Object* Clone();

	virtual void SpriteAnimationFinished();

	//Input
	virtual void MouseDown(unsigned short button, unsigned short buttons, PointI const& pt);
	virtual void MouseUp(unsigned short button, unsigned short buttons, PointI const& pt){};
	virtual void MouseMove(PointI const& pt){};
	virtual void MouseWheel(int wheel, unsigned short buttons, PointI const& pt){};

	virtual void KeyDown(unsigned int key){};
	virtual void KeyUp(unsigned int key){};
	virtual void KeyChar(wchar_t key){};

	friend class ObjectConnection;
};

class AnimableObject : public DrawableObject
{
protected:
	float m_current;
	float m_cpos;
	float m_scale;

public:
	float m_delay;
	float m_speed;
	PointF m_spos;
	PointF m_epos;
	float m_rspos;
	float m_repos;
	unsigned int m_scolor;
	unsigned int m_ecolor;
	int m_animtype;
	int m_playtype;
	Resource m_sound;
	bool m_sloop;
	bool m_pingpong;
	C_SoundPlayer m_player;
	float m_scalefrom;
	float m_scaleto;

	AnimableObject(unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true);

	virtual void Update( float delta );

	virtual void Draw(C_Renderer& rnd);

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	void Reset();

	virtual Object* Clone();
};

class GUIText : public AnimableObject
{
public:
	Resource m_Font;
	unsigned int m_DrawStyle;
	UIWString m_Text;
	unsigned int m_Color;
	bool m_Shadow;
	RectangleF m_TextPadding;
	unsigned int m_ShadowColor;
	float m_fscale;

	ResourceManager* m_mgr;

	GUIText( ResourceManager* mgr = 0, unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true);
	virtual ~GUIText(){};
	virtual void Release();
	virtual void Process(unsigned int Msg, unsigned int Param1, void* Param2);
	
	//Getset
	//Resource GetFont();
	//void SetFont(Resource val);

	//unsigned int GetDrawStyle();
	//void SetDrawStyle(unsigned int val);

	//UIWString& GetText();
	//void SetText(UIWString const& val);

	//unsigned int GetColor();
	//void SetColor(unsigned int val);

	//unsigned int GetShadowColor();
	//void SetShadowColor(unsigned int val);

	//bool GetShadow();
	//void SetShadow(bool val);

	//RectangleF& GetTextPadding();
	//void SetTextPadding(RectangleF const& val);

	//General functions
	virtual void Update( float delta );
	virtual bool Init();
	virtual void Draw(C_Renderer& rnd);

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual Object* Clone();
	//Input
};

class GUIButton :  public GUIText
{
protected:
	unsigned int state;
	unsigned int prevstate;
    float CurrentAniPos;
	float CurrentAniPos2;
	bool Checked;
	unsigned int m_CheckBoxSize;
	float m_ttimer;
	PointI m_mousept;
	Resource sound;
	C_SoundPlayer splayer;

public:
	unsigned int m_PressedColor;
	unsigned int m_NormalColor;
	unsigned int m_MouseOverColor;
	unsigned int m_CheckedColor;
	unsigned int m_CCheckColor;
	unsigned int m_DColor;
	bool m_Animate;
	float m_AnimationSpeed;

	Sprite m_CheckBack;
	Sprite m_CheckCheck;
	Sprite m_RadioBack;
	Sprite m_RadioCheck;
	Sprite m_ToolTip;
	Sprite m_MouseOver;
	Sprite m_Normal;
	Sprite m_Disabled;

	float m_maxttimer;
	float m_twidth;
	float m_theight;

	virtual void SetState(unsigned int st);
	unsigned int GetPrevState();
	unsigned int GetState();
	float GetAnimationPos();
	float GetAnimationPos2();
	
	virtual void SetChecked(bool chk);
	bool GetChecked();

	void SetCheckBoxSize(unsigned int val);
	unsigned int GetCheckBoxSize();

	GUIButton( ResourceManager* mgr = 0, unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true );

	virtual void Process(unsigned int Msg, unsigned int Param1, void* Param2);
	virtual void Release();

	//General functions
	virtual void Update( float delta );
	virtual bool Init();
	virtual void Draw(C_Renderer& rnd);

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual void OnClick(){};

	virtual Object* Clone();

	//Input
	virtual void MouseDown(unsigned short button, unsigned short buttons, PointI const& pt);
	virtual void MouseUp(unsigned short button, unsigned short buttons, PointI const& pt);
	virtual void MouseMove(PointI const& pt);
	virtual void MouseWheel(int wheel, unsigned short buttons, PointI const& pt);

	virtual void KeyDown(unsigned int key);
	virtual void KeyUp(unsigned int key);
	virtual void KeyChar(wchar_t key);
	virtual void OnChecked(){};
};

class GUIEdit : public GUIText
{
protected:
	unsigned int DrawFrom;
	unsigned int ETime;
	bool Selecting;
	unsigned int SelectionStart;
	bool m_CurVisible;

public:
	Sprite EditBack;
	C_Renderer* m_renderer;
	unsigned int m_MaxLength;
	unsigned int m_SelColor;
	unsigned int m_CColor;
	unsigned int m_Bk2Color;
	unsigned int m_BlinkSpeed;
	unsigned int m_MouseBounds;
	unsigned int m_SelStart;
	unsigned int m_SelLength;

	unsigned int GetTextLength(unsigned int start, unsigned int end);
	unsigned int GetDrawFrom();

	unsigned int GetPosFromPoint(RectangleI r1, PointI* pt);
	bool GetRectFromPos(RectangleI* r1, unsigned int pos);
	bool AdjustPosition(unsigned int pos);
	void DeleteChar(int val);
	void MoveCursor(int val);

	GUIEdit( C_Renderer* renderer = 0, ResourceManager* mgr = 0, unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true );

	virtual void Release();

	virtual void Process(unsigned int Msg, unsigned int Param1, void* Param2);

	//General functions
	virtual void Update( float delta );
	virtual bool Init();
	virtual void Draw(C_Renderer& rnd);

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual Object* Clone();

	//Input
	virtual void MouseDown(unsigned short button, unsigned short buttons, PointI const& pt);
	virtual void MouseUp(unsigned short button, unsigned short buttons, PointI const& pt);
	virtual void MouseMove(PointI const& pt);
	virtual void MouseWheel(int wheel, unsigned short buttons, PointI const& pt);

	virtual void KeyDown(unsigned int key);
	virtual void KeyUp(unsigned int key);
	virtual void KeyChar(wchar_t key);
};

class GUIProgress : public DrawableObject
{
protected:
	float Min, Max, Value;
	bool Selecting;

public:
	float GetMin();	
	float GetMax();
	float GetValue();

	void SetMin(float val);	
	void SetMax(float val);
	void SetValue(float val);

	Sprite m_progressback;
	RectangleF m_Region;
	unsigned int m_ProgressColor;
	unsigned int m_Bk2Color;
	unsigned int m_ThumbSize;
	unsigned int m_MoveSize;
	bool clipping;
	bool imgprogress;
	unsigned int startind, endind;

	GUIProgress( ResourceManager* mgr = 0, unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true );

	virtual void Release();

	virtual void Process(unsigned int Msg, unsigned int Param1, void* Param2);

	//General functions
	virtual void OnValueChanged(){};
	virtual void Update( float delta );
	virtual bool Init();
	virtual void Draw(C_Renderer& rnd);

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual Object* Clone();

	//Input
	virtual void MouseDown(unsigned short button, unsigned short buttons, PointI const& pt);
	virtual void MouseUp(unsigned short button, unsigned short buttons, PointI const& pt);
	virtual void MouseMove(PointI const& pt);
	virtual void MouseWheel(int wheel, unsigned short buttons, PointI const& pt);

	virtual void KeyDown(unsigned int key);
	virtual void KeyUp(unsigned int key);
	virtual void KeyChar(wchar_t key);
};

class GUILoadProgress : public GUIProgress
{
protected:

public:
	GUILoadProgress( ResourceManager* mgr = 0, unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true );

	//General functions
	virtual void Update( float delta );
	virtual Object* Clone();
};

class ObjectEditor : public DrawableObject
{
private:
	enum EditMode
	{
		None,
		C,
		L,
		T,
		R,
		B,
		RB,
		RT,
		LB,
		LT
	};

	EditMode m_edit;
	bool m_mousedown;
	PointI m_oldpos;

protected:
	DrawableObject* m_object;
	int m_space;

	EditMode GetIntersect(PointI const& pt);

public:
	bool m_snaptogrid;
	float m_gridsize;

	void ClearObject();

	void SetObject(DrawableObject* val);
	DrawableObject* GetObject();

	void SetSpace(int val);
	int GetSpace();

	void UpdateSize();
	void UpdateObjectSize();

	ObjectEditor( unsigned int ID = 0, const RectangleF& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true );

	virtual void Release(){};

	virtual void Process( unsigned int Msg, unsigned int Param1, void* Param2 );

	virtual void Update( float delta );
	virtual bool Init();
	virtual void Draw(C_Renderer& rnd);

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual Object* Clone(){return 0;};

	//Input
	virtual void MouseDown(unsigned short button, unsigned short buttons, PointI const& pt);
	virtual void MouseUp(unsigned short button, unsigned short buttons, PointI const& pt);
	virtual void MouseMove(PointI const& pt);
	virtual void MouseWheel(int wheel, unsigned short buttons, PointI const& pt);

	virtual void KeyDown(unsigned int key);
	virtual void KeyUp(unsigned int key);
	virtual void KeyChar(wchar_t key);
};

class ObjectsContainer : public DrawableObject
{
private:
	bool m_clicked;

protected:
	list<Object*> Objects;
	set<Object*> DeleteQueue;

	void Clean();
	C_Window* m_win;
	ObjectEditor* m_editor;
	ResourceManager* m_resmanager;
	Camera2D m_Camera;

public:
	bool m_drawboundingbox;

	list<Object*>::iterator GetObjectInteratorBegin();
	list<Object*>::iterator GetObjectInteratorEnd();

	void AddToDeleteQueue(Object* val);
	void ProcessDeleteQueue();

	void SetupCamera(const Camera2D& camera);
	void AdjustSize(C_Device& dev);
	void AdjustCamera();
	void CenterCamera();
	void MoveCamera(float x, float y);
	void Zoom(float factor);
	void FocusCameraOn(const PointF& val, const PointF& offset);
	Camera2D& GetCamera();

	virtual void DrawBoundingBox(C_Renderer& rnd);
	Signal0 OnDeleteQueue;

	bool HoldCapture;
	DrawableObject* Capture;
	DrawableObject* Selected;
	DrawableObject* DrawTop;

	Signal4<Object*, unsigned int, unsigned int, void*> Callback;

	ObjectsContainer( ResourceManager* mgr, unsigned int ID = 0, const RectangleF& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true );

	bool GetMouseProcess();

	void ShowAll();
	void HideAll();

	void ScaleAll(float scale);

	void ClearEditorObject();
	void SetEditorObject(DrawableObject* obj);
	virtual Object* GetObjectByID(unsigned int id);
	virtual void AddObject(Object* obj);
	virtual void RemoveObject(unsigned int Index);
	virtual void RemoveObject(Object* ctrl);
	virtual Object* GetObject(unsigned int Index);
	virtual void Clear(bool full = false);
	virtual void QueueClear();
	virtual void Update(float delta);
	virtual bool GetEditable();
	virtual void SetEditable(bool val);
	DrawableObject* GetEditorObject();
	DrawableObject* GetEditorObjectRecursive();
	unsigned int GetSize();
	virtual void Process(unsigned int Msg, unsigned int Param1, void* Param2);
	Object* GetIntersect(PointI const& Param2);

	void UpdateEditor();

	virtual Object* Clone();

	virtual void Release();
	virtual bool Init();
	virtual void Draw(C_Renderer& rnd);
	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );
	virtual void SaveAll( BaseIO& str );
	virtual void LoadAll( BaseIO& str, ObjectBuilder& builder, bool editor, bool readtype = true, bool append = false);

	virtual void MouseDown(unsigned short button, unsigned short buttons, PointI const& pt);
	virtual void MouseUp(unsigned short button, unsigned short buttons, PointI const& pt);
	virtual void MouseMove(PointI const& pt);
	virtual void MouseWheel(int wheel, unsigned short buttons, PointI const& pt);

	virtual void KeyDown(unsigned int key);
	virtual void KeyUp(unsigned int key);
	virtual void KeyChar(wchar_t key);

	void AutoConnectInput(C_Window* window);
	void DisconnectInput();
	virtual ~ObjectsContainer();
};

class GUICircleMenu : public ObjectsContainer
{
protected:
	float m_radius;
	float m_sangle;

public:
	GUICircleMenu( ResourceManager* mgr, unsigned int ID = 0, const RectangleF& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true );
	
	void AddObject(Object* obj);
	void Arrange();

	float GetRadius();
	void SetRadius(float val);

	float GetStartAngle();
	void SetStartAngle(float val);

	void SetParent(ObjectsContainer* val);

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual Object* Clone();
};

class GUITable : public ObjectsContainer
{
protected:
	float twidth;
	float theight;
	float spacex;
	float spacey;
	unsigned int tilescx;

public:
	GUITable( ResourceManager* mgr, unsigned int ID = 0, const RectangleF& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true );
	
	void AddObject(Object* obj);
	void Arrange();

	void SetParent(ObjectsContainer* val);

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	void SetTileSize(float width, float height);
	void SetTileSpace(float spacex, float spacey);
	void SetColumns(unsigned int count);

	PointF GetTileSize();
	PointF GetTileSpace();
	unsigned int GetColumns();

	virtual void PostLoadAll();

	virtual Object* Clone();
};

struct Particle
{
	float posx, posy;
	float velx, vely;
	unsigned int color;

	float ctime;
	float ttime;

	float scale;
	float rotation;

	Particle();
};

struct EmitItem
{
	RectangleF pos;
	float m_ctime;
	float m_life;

	EmitItem()
	{
		m_ctime = 0;
		m_life = 1;
	}
};

#define EMITTOR_TYPE_PLANE	0
#define EMITTOR_TYPE_CIRCLE	1

class ParticleEngine : public AnimableObject
{
protected:
	vector<EmitItem> emitlist;
	list<Particle> particles;

	Sprite m_psprite;

	int m_emission;

	float m_startdelay;
	float m_tmptime;
	bool m_activated;
	bool m_emitlistonly;

	unsigned int startcolor;
	unsigned int endcolor;
	float scalefreq;
	float angularvel;

	float m_cetime;
	float m_maxtime;

	int velxstart;
	int velxend;

	int velystart;
	int velyend;

	bool randomx;
	bool randomy;

	float m_ttimemax;
	float m_ttimemin;
	bool randomtime;

	int particlesizestart;
	int particlesizeend;
	bool randomsize;

	float forcex;
	float forcey;

	int m_emittortype;

	bool randomxpos;
	bool randomypos;

	bool relativevel;

	int circleradstart;
	int circleradend;
	float cianglestart;
	float ciangleend;
	bool randomradius;

	bool cirelativevel;
	bool gravitycenter;

public:
	ParticleEngine(unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true);

	void EmitParticles(const RectangleF& rect);
	void EmitParticles();
	void EmitParticles(float x, float y, float size = 1.0f);

	void AddEmitRect(const RectangleF& rect, float life);
	void AddEmitRect(float x, float y, float life, float size = 1.0f);
	void ClearEmitRect();

	virtual void Update( float delta );
	virtual void Draw(C_Renderer& rnd);

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual Object* Clone();
};

class ParticleEmitter : public AnimableObject
{
protected:
	float m_time;
	bool m_activated;
	float m_paricledelay;

public:
	ParticleEmitter(unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true);

	virtual void Update( float delta );

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual Object* Clone();
};

class TimerObject : public DrawableObject
{
private:
	float m_ctime;
	bool m_started;

public:
	float m_interval;
	bool m_loop;

	Signal0 evt_Action;

	TimerObject(unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true);

	void StartTimer();
	void StopTimer();
	void PauseTimer();

	virtual void OnInterval() {evt_Action();};

	virtual void Update( float delta );

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual Object* Clone();
};

class Tiles : public DrawableObject
{
private:
	bool editing;
	string tmpstr;
	int cindex;
	float cangle;

	float twidth;
	float theight;
	float spacex;
	float spacey;
	unsigned int tilescx;
	unsigned int tilescy;
	bool relative;
	vector<int> data;
	vector<float> angles;

public:
	Tiles(unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true);
	
	virtual void MouseDown(unsigned short button, unsigned short buttons, PointI const& pt);
	virtual void KeyDown(unsigned int key);

	virtual void Update( float delta );
	virtual void Draw(C_Renderer& rnd);

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual Object* Clone();
};

class GUITextEffects : public GUIText
{
protected:
	float ctime;

public:
	enum EffecTtype
	{
		SinCurve = 0,
		HSinCurve,
		Linear,
		Colors,
		Typing
	};

	float speed;
	float param1;
	float param2;
	float param3;
	EffecTtype etype;
	bool loop;

	GUITextEffects( ResourceManager* mgr = 0, unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true );

	//General functions
	virtual void Update( float delta );
	virtual void Draw(C_Renderer& rnd);

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual Object* Clone();
};

class ImageScroller : public DrawableObject
{
private:
	int cursel;
	float transpos;
	vector<int> indices;
	bool rev;

public:
	ImageScroller(unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true);
	
	void Next();
	void Previous();

	virtual void MouseDown(unsigned short button, unsigned short buttons, PointI const& pt);

	virtual void Update( float delta );
	virtual void Draw(C_Renderer& rnd);

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual Object* Clone();
};

class FunctionButton : public GUIButton
{
private:

public:
	enum Function
	{
		ImgScrollNext,
		ImgScrollPrevious
	};

	unsigned int m_targetid;
	Function m_function;

	FunctionButton( ResourceManager* mgr = 0, unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true );

	virtual void OnClick();

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual Object* Clone();
};

class Dial : public DrawableObject
{
private:
	bool m_mdown;

public:
	float m_startangle;
	float m_value;
	float m_max;
	float m_min;

	float m_radius;
	float m_dotsize;

	bool m_autodotangle;

	float m_eangle;

	unsigned int m_dotcolor;

	Sprite m_dot;

	Dial(unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true);

	virtual void Draw(C_Renderer& rnd);

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual void MouseDown(unsigned short button, unsigned short buttons, PointI const& pt);
	virtual void MouseUp(unsigned short button, unsigned short buttons, PointI const& pt);
	virtual void MouseMove(PointI const& pt);

	virtual Object* Clone();
};

class SoundPlayer : public DrawableObject
{
private:
	Resource m_sound;
	C_SoundPlayer player;

public:
	SoundPlayer(unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true);

	void Start(bool loop = false);
	void Stop();
	bool isPlaying();

	void SetResource(const char* name);

	void SetVolume(long val);
	long GetVolume();

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual Object* Clone();
};

//// Object builder
class ObjectBuilder
{
protected:
	map<string, Object*> m_type_list; ///< List of all codecs for each type of object.
	unsigned int AutoID; ///< Auto Generated ID

public:
	// object builder
	/** \name Object Builder
	\{ */
	/** Registers builder for the specified object name.
	\param name Name of a builder to be registered.
	\param obj Pointer to the Object builder being registered.
	\note new keyword must be used on the object pointer being registered, this object builder will be automatically deleted using delete keyword when the object builder is destructed or that builder is replaced by a new one.*/
	void RegisterBuilder(const char* name, Object* obj);

	/** This method removes the object builder specified by the type.  
	\param name Name of a builder to be registered.
	\return true if the builder is removed successfully, otherwise false. */
	bool RemoveBuilder(const char* name);
	
	/** Clears all builders. */
	void ClearBuilders();

	/** builds new Object.  
	\param name Name of the resource.
	\return Valid new object if object building was successful, otherwise 0.
	*/
	Object* BuildObject(const char* name);
	/** \} */

	// resourcemanager
	/** Constructor. */
	ObjectBuilder();

	/** Destructor. */
	virtual ~ObjectBuilder();

	/** Gets object builders list.
	\param lst Vector list object to be populated by object builder names.
	*/
	void GetBuildersList(vector<string>& lst);

	/** Registers all default builders, this function can be overrided to register more builders in derived class.
	\param m_mgr Resource manager used when building objects.
	\param renderer Renderer used when building objects.
	*/
	virtual void RegisterDefaultBuilders(ResourceManager* m_mgr, C_Renderer* renderer = 0);
};