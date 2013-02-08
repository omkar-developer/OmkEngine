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

/** Color Constants \addtogroup ColorConstants
* \{
*/
#define COLOR_BLACK		0xff000000 ///<Black color.
#define COLOR_WHITE		0xffffffff ///<White color.
#define COLOR_GREEN		0xff00ff00 ///<Green color.
#define COLOR_BLUE		0xff0000ff ///<Blue color.
#define COLOR_RED		0xffff0000 ///<Red color.
#define COLOR_GRAY		0xff808080 ///<Gray Color.
#define COLOR_AQUA		0xff00ffff ///<Aqua color.
#define COLOR_YELLOW	0xffffff00 ///<Yellow color.
#define COLOR_CYAN		0xff00ffff ///<Cyan color.
/** \} */

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
#define OBJECT_TYPE_PHYSICSOBJ	0x8000	///<Physics object type.
#define OBJECT_TYPE_SAVE		0x10000 ///<Save config object type.
#define OBJECT_TYPE_PHYJOINT	0x20000 ///<Physics Joint object type.
#define OBJECT_TYPE_SOUNDPLAYER	0x40000 ///<Sound player object type.
/// \}

/** Object Messages \addtogroup ObjectMessages
* \{
*/
#define OBJECT_Message_Draw					0 ///<Sent when drawing object (not used).
#define OBJECT_Message_LMouseDown			1 ///<Sent when left mouse button is down.
#define OBJECT_Message_LMouseUp				2 ///<Sent when left mouse button is up.
#define OBJECT_Message_MouseMove			3 ///<Sent when mouse cursor is moved.
#define OBJECT_Message_LMouseClick			4 ///<Sent when left mouse button is clicked.
#define OBJECT_Message_KeyState				5 ///<Sent when key state is changed.
#define OBJECT_Message_Release				7 ///<Sent when object is being released.
#define OBJECT_Message_EDAddChar			8 ///<Sent to add new character to the editbox (first param is char).
#define OBJECT_Message_EDDelete				9 ///<Sent to delete character from editbox (first param -1 is delete back char, 1 is next character).
#define OBJECT_Message_EDMove				10 ///<Sent when caret in editbox is moved.
#define OBJECT_Message_EDTextChange			11 ///<Sent when text in editbox is changed.
#define OBJECT_Message_EDRedrawText			12 ///<Sent when text in editbox needs to be redrawn.
//#define OBJECT_Message_BUpdateAnimation		13
#define OBJECT_Message_PValueChanged		14 ///<Sent when progress value changed.
#define OBJECT_Message_LSelectionChanged	15 ///<Sent when list selection changed (not used).
#define OBJECT_Message_BSetChecked			16 ///<Sent when checkbox/optionbutton check status is changed.
#define OBJECT_Message_CSelectionChanged	17 ///<Sent Combobox selection changed (not used).
#define OBJECT_Message_LSelected			18 ///<Sent List selection (not used.)
#define OBJECT_Message_CnUpdatePositions	19 ///<Not used.
#define OBJECT_Message_RMouseDown			20 ///<Sent when mouse right button is down.
#define OBJECT_Message_RMouseUp				21 ///<Sent when mouse right button is up.
#define OBJECT_Message_MouseDown			23 ///<Sent when mouse button is down (not used).
#define OBJECT_Message_MouseUp				24 ///<Sent when when mouse button is up.
#define OBJECT_Message_MouseScroll			25 ///<Sent on mouse scroll (first param is amount of scroll).
#define OBJECT_Message_BMouseOver			26 ///<Sent when Button mouse over.
#define OBJECT_Message_BMouseNormal			27 ///<Sent when Button mouse normal.
#define OBJECT_Message_Init					28 ///<Not used.
#define OBJECT_Message_KeyDown				29 ///<Sent when keyboard key is down.
#define OBJECT_Message_KeyUp				30 ///<Sent when keyboard key is up.
#define OBJECT_Message_KeyChar				31 ///<Sent when pressed key is character.
#define OBJECT_Message_SelEdit				32 ///<Sent when object is selected for editing.
#define OBJECT_Message_SelClear				33 ///<Sent when selectio is cleared.
#define OBJECT_Message_Resize				34 ///<Sent when window is resized.
#define OBJECT_Message_OnTimer				35 ///<Sent on timer tick.
#define OBJECT_Message_Collision			36 ///<Sent on object collision.
#define OBJECT_Message_SpriteFinished		37 ///<Sent when Sprite animation finished.
#define OBJECT_Message_Update				38 ///<Sent when sent every frame.
#define OBJECT_Message_TileClicked			39 ///<Sent when tile is clicked from tiles control.
#define OBJECT_Message_EndContact			40 ///<Sent when overlapping object is resolved.
/** \} */

/** Object Flags \addtogroup ObjectFlags Object Flags
* \{
*/
//Object Flags
//Control Flags
#define OBJECT_Flag_Invisible			1 ///<Makes object invisible.
#define OBJECT_Flag_Disabled			2 ///<Disables object to object does not receive any input.
#define OBJECT_Flag_NoBackground		4 ///<Sets background sprite invisible.
#define OBJECT_Flag_Tansform			8 ///<Sets object to be transformable when camera is moved.
#define OBJECT_Flag_IgnoreMouse			16 ///<Ignores mouse input and passes it on to next object.
#define OBJECT_Flag_ChangeByVelocity	32 ///<Changes physics objects rotation using velocity direction.
#define OBJECT_Flag_BTypePush			2097152 ///<Sets button type to push button.
#define OBJECT_Flag_BTypeCheck			4194304 ///<Sets button type to check box.
#define OBJECT_Flag_BTypeRadio			8388608 ///<Sets button type to radio button.
#define OBJECT_Flag_PTypeVertical		2097152 ///<Sets Progress type to be vertical.
#define OBJECT_Flag_PTypeSlider			4194304 ///<Sets Progress type to be slider.
#define OBJECT_Flag_PTypeDecimal		8388608 ///<Allows progress to have floating point values.
#define OBJECT_Flag_PTypeInv			16777216 ///<Inverts progress view.
#define OBJECT_Flag_LItemSelected		2097152 ///<Not used
#define OBJECT_Flag_LTypeMultiSelect	4194304 ///<Not used
#define OBJECT_Flag_ETypePass			2097152 ///<Not used
/** \} */

/** Button States \addtogroup ButtonStates Button States
* \{
*/
//Button States
#define GUIButton_State_Normal		0 ///< Button has normal state.
#define GUIButton_State_Pressed		1 ///< Button has pressed state.
#define GUIButton_State_MouseOver	2 ///< Button has mouse over state.
/** \} */

//class ObjectsContainer;
class ObjectsContainer;
class ObjectBuilder;

#define ERR_CONST 4294967295u

/**
 * Contains camera configuration.
 */
struct Camera2D
{
	RectangleF m_area; ///< Area of the camera.
	PointF m_pos; ///< Camera position.
	Size m_size; ///< Camera size.
	float m_scale; ///< Camera scale, value 1 is no scale (used for adjusting camera with screen resolution).
	float m_zoom; ///< Camera Zoom, value 1 is no zoom.
	float m_maxzoom; ///< Camera's maximum zoom value.
	Size m_max; ///< Max pixels camera can move right/bottom.

	/**
	 * Constructor.
	 * \param rect Area of the camera.
	 * \param scale Camera scale, value 1 is no scale (used for adjusting camera with screen resolution).
	 * \param zoom Camera Zoom, value 1 is no zoom.
	 */
	Camera2D(const RectangleF& rect, float scale = 1.0f, float zoom = 1.0f)
	{
		m_area = rect;
		m_scale = scale;
		m_zoom = zoom;
		m_maxzoom = 2.0f;
	}

	/**
	 * Default Constructor.
	 */
	Camera2D()
	{
		m_scale = 1.0f;
		m_zoom = 1.0f;
		m_maxzoom = 2.0f;
	}
};

class Object;
class DrawableObject;

/**
 * Connection to a object which will be destroyed automatically when connected object is destroyed.
 */
class ObjectConnection
{
private:
	DrawableObject* cobj; ///< Pointer to the connected object.
	unsigned int ConID; ///< ID of the connection object or object being connected.

	/**
	 * Informs connected object of connection.
	 * \param obj Object to be informed.
	 */
	void AddConnection(DrawableObject& obj);

	/**
	 * Informs connected object of disconnection.
	 * \param obj Object to be informed.
	 */
	void DeleteConnection(DrawableObject& obj);

public:
	/**
	 * Copies specified object settings to this object.
	 * \param cpy object to be copied.
	 */
	void Copy(const ObjectConnection& cpy);

	/**
	 * Copy constructor.
	 * \param cpy Object to be copied.
	 */
	ObjectConnection(const ObjectConnection& cpy);

	/**
	 * Assignment operator for copying specified object settings to this object.
	 * \param cpy Object to be copied.
	 * \return Reference to this object.
	 */
	const ObjectConnection& operator=(const ObjectConnection& cpy);

	/**
	 * Default constructor.
	 */
	ObjectConnection();

	/**
	 * Constructor to construct object using specified connection object.
	 * \param cobj Object to be connected.
	 */
	ObjectConnection(DrawableObject* cobj);

	/**
	 * Destructor disconnects objects.
	 */
	virtual ~ObjectConnection();

	/**
	 * Updates Connection according to the specified object ID if object is already connected this function does nothing.
	 */
	void UpdateConnection();

	/**
	 * Disconnects connected object.
	 */
	void Disconnect();

	/**
	 * Set ID of object to be connected.
	 * \param ID ID of the object which is being connected.
	 */
	void SetConnectionID(unsigned int ID);

	/**
	 * Returns current connection ID.
	 * \return current connection ID.
	 */
	unsigned int GetConnectionID();

	/**
	 * Returns connected object if no object is connected this returns null.
	 * \return connected object if no object is connected this returns null.
	 */
	DrawableObject* GetObject();

	/**
	 * Sets connection with specified object.
	 * \param obj Pointer to the object to be connected.
	 */
	void SetObject(DrawableObject* obj);

	/**
	 * Sets ID of the object to be connected later by calling UpdateConnection().
	 * \param ID ID of the object to be connected.
	 */
	void SetConnectionIDNU(unsigned int ID);

	/**
	 * Returns const connected object if no object is connected this returns null.
	 * \return const connected object if no object is connected this returns null.
	 */
	const DrawableObject* GetConstObject() const;

	/**
	 * Tells if connection is valid.
	 * \return true if connection to the object is valid otherwise false.
	 */
	bool IsValid();

	/**
	 * Compares ObjectConnections objects.
	 * \param str Second object to be compared with this object.
	 * \return true if current object pointer value is less otherwise false.
	 */
	bool operator<(const ObjectConnection& str) const
	{
		return cobj < str.cobj;
	}

	/**
	 * Signaled when object connected with this object is destroyed.
	 */
	Signal1<DrawableObject*> Disconnected;


	friend class DrawableObject; ///< Friend DrawableObject class.
};

/**
 * Abstract base object class (all other objects are derived from this class).
 */
class Object
{
protected:
	list<Object*>::iterator it; ///< This object's iterator from parent container.
	bool m_update; ///< Whether to allow update on this object.
	int m_type; ///< Type of this object, combination of \link ObjectTypes Object Types \endlink.
	string name; ///< Name of this object.
	unsigned int m_ID; ///< ID of this object.
	unsigned int m_objver; ///< Object version of this object (automatically placed).

public:
	map<string, float> extrafloat; ///< Used to store extra properties dynamically made using object editor or script.
	ObjectsContainer* m_parent; ///< Parent of this object.
	unsigned int m_flags; ///< Flags for this object. combination of \link ObjectFlags Object Flags \endlink
	string m_objname; ///< Object name used when registering this object to builder when this object is placed in Objects.cfg file.
	unsigned int m_objtype; ///< Type number of this object (used to make copies of same object with different no/type).

	/**
	 * Sets dynamically created property (if this property is not defined then new one is created by specified name).
	 * \param name Name of the property to set.
	 * \param val Value of the the property.
	 */
	void SetProperty(const char* name, float val);

	/**
	 * Returns property value specified by name.
	 * \param name Name of the property to retrieve value.
	 * \return Value belongs to name specified.
	 */
	float GetProperty(const char* name);

	/**
	 * Clears(Deletes) all dynamic properties.
	 */
	void ClearProperties();

	/**
	 * Reads type of the object in BaseIO object (this function is used by file loader to determine object type before reading its all data).
	 * \param str Reference to BaseIO.
	 * \return Object type of (\link ObjectTypes Object Types \endlink) of the current object in BaseIO file.
	 */
	static string ReadType(BaseIO& str);

	/**
	 * Returns type of this object values can be combination of one of these \link ObjectTypes Object Types \endlink .
	 * \return type of this object used to determine object hierarchy/type.
	 */
	int GetType();

	/**
	 * Constructor (these parameters are mostly filled by objects builder).
	 * \param ID Identifier number of this object (this number is used to find/identify this object).
	 * \param flags Flags for this object can be combination of these \link ObjectTypes Object Types \endlink .
	 * \param parent Parent of this object.
	 * \param update Whether to allow this object to receive update messages.
	 */
	Object( unsigned int ID = 0, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true );

	/**
	 * Sets ID of this object.
	 * \param val ID value to be set for this object.
	 */
	void SetID(unsigned int val);

	/**
	 * Returns ID of this object.
	 * \return ID of this object.
	 */
	unsigned int GetID();

	/**
	 * Returns version of this object (this is used to determine by which version this object was loaded, when saving version is updates to newer version).
	 * \return version of this object.
	 */
	unsigned int GetObjectVersion();

	/**
	 * Gets name of this object.
	 * \return name of this object used when object builder built this object.
	 */
	const string& GetName();

	/**
	 * Queues this object for deleting to delete it when next game loop cycle is started.
	 */
	virtual void QueueForDelete()=0;

	/**
	 * Returns enabled state of the update for this object.
	 * \return enabled state of update.
	 */
	virtual bool GetUpdate();

	/**
	 * Sets update enabled state of this object.
	 * \param val true to enable updating, false to disable updating.
	 */
	virtual void SetUpdate(bool val);

	/**
	 * Called to do initializing stuff for this object (better to do in constructor).
	 * \return true if initialization was successful.
	 */
	virtual bool Init()=0;

	/**
	 *	Called when object is being destroyed.
	 */
	virtual void Release()=0;

	/**
	 * Called to make object update itself using time specified in delta.
	 * \param delta Number of seconds passed after last frame.
	 */
	virtual void Update( float delta )=0;

	/**
	 * Called when object need to be drawn.
	 * \param rnd Renderer to use for drawing this object.
	 */
	virtual void Draw(C_Renderer& rnd)=0;

	/**
	 * Clones this object using new keyword (cast it to type of this or base object).
	 * \warning delete must be called to release this object unless object is handled by a container.
	 * \return Cloned object.
	 */
	virtual Object* Clone()=0;

	/**
	 * virtual destructor does nothing.
	 */
	virtual ~Object(){};

	/**
	 * Returns parent container of this object.
	 * \return Pointer to the parent of this object (null if object has no parent).
	 */
	ObjectsContainer* GetParent();

	/**
	 * Used to save this object using specified IO object.
	 * \param str BaseIO object using which this object has to be saved.
	 */
	virtual void Save( BaseIO& str );

	/**
	 * Used to load this object using specified IO object.
	 * \param str BaseIO object using which this object has to be loaded.
	 */
	virtual void Load( BaseIO& str );

	/**
	 * This function is called when all objects are loaded.
	 * \note this function can be used to update connection of ObjectConnection so this object can find connected object as this function is called after all objects are loaded.
	 */
	virtual void PostLoadAll(){};

	/** this is called to process messages (currently not used).
	* \param Msg Object Message from \link ObjectMessages Messages \endlink .
	* \param Param1 First parameter.
	* \param Param2 Second parameter.
	**/
	virtual void Process( unsigned int Msg, unsigned int Param1, void* Param2 ) = 0;

	friend class ObjectsContainer;
	friend class ObjectBuilder;
};

/**
 * This can draw basic sprite (All other drawable objects can be safely derived from this class).
 */
class DrawableObject : public Object
{
private:
	set<ObjectConnection*> connections; ///< List to store objects linked to this object.
	friend void ObjectConnection::AddConnection(DrawableObject& obj); ///< Used by ObjectConnection to inform of connection.
	friend void ObjectConnection::DeleteConnection(DrawableObject& obj); ///< Used by ObjectConnection to inform of disconnection.

	/**
	 * private copy constructor to prevent copying.
	 * \param obj Not used.
	 */
	DrawableObject(const DrawableObject& obj);

protected:
	RectangleF m_location; ///< Drawing area allocated for this object.
	float m_rotation; ///< Rotation of this object.
	Sprite m_sprite; ///< Sprite to be drawn with this object draw function.
	bool m_editable; ///< Whether this object is in editing mode.
	float m_layer; ///< Current layer of the object which is used to sort drawing of objects (0 layer will be drawn at top and 1 will be at back).

public:	
	unsigned int m_backcolor; ///< Color to be used to to draw this object's sprite.
	float m_depth; ///< Depth Multiplier value used with transformable object to simulate parallax effect when camera moves.

	/** Constructor (these parameters are mostly filled by objects builder).
	 * \param ID Identifier number of this object (this number is used to find/identify this object).
	 * \param location Drawing area of this object.
	 * \param sprite Sprite resource to draw with this object.
	 * \param backcolor Sprite color.
	 * \param rotation Rotation angle of this object in radian.
	 * \param visible Whether object is visible and receives draw messages.
	 * \param flags Flags for this object can be combination of these \link ObjectTypes Object Types \endlink .
	 * \param parent Parent of this object.
	 * \param update Whether to allow this object to receive update messages.
	 */
	DrawableObject( unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true );

	/** virtual destructor */
	virtual ~DrawableObject();
	
	/**
	 * Assignment operator to copy other objects settings.
	 * \param obj Reference to the object from which settings to be copied to this object.
	 * \return Reference to this object.
	 */
	const DrawableObject& operator=(const DrawableObject& obj);

	/**
	 * Called to draw bounding boxes around objects draw area.
	 * \param rnd Renderer to use for drawing boxes.
	 */
	virtual void DrawBoundingBox(C_Renderer& rnd);

	virtual void QueueForDelete();

	/**
	 * Directly deletes this object. (QueueForDelete() function is preferred way to delete instead of this).
	 */
	void Delete();

	/**
	 * Clears/Disconnects all other objects connected to this object using ObjectConnection.
	 */
	void ClearConnections();

	//properties
	/**
	 * Returns current draw area/position of the object.
	 * \return current draw area/position of the object.
	 */
	virtual RectangleF& GetObjectArea();

	/**
	 * Sets draw area of this object.
	 * \param val Dimensions to be set for this object's draw area.
	 */
	virtual void SetObjectArea(const RectangleF& val);

	/**
	 * Return current rotation angle of this object.
	 * \return current rotation angle of this object in radian (angle starts from right side of the center)..
	 */
	virtual float GetAngle();

	/**
	 * Sets angle/rotation of this object in radian.
	 * \param val rotation angle in radian (angle starts from right side of the center).
	 */
	virtual void SetAngle(float val);

	/**
	 * Gets Visible state of this object.
	 * \return Visible state of this object.
	 */
	virtual bool GetVisible();

	/**
	 * Gets Visible state of this object.
	 * \param val true to make object visible, false to make it invisible.
	 */
	virtual void SetVisible(bool val);

	/**
	 * Color of the sprite.
	 * \return Color of the sprite.
	 */
	virtual unsigned int GetBackColor();

	/**
	 * Sets sprite color.
	 * \param val Sprite color.
	 */
	virtual void SetBackColor(unsigned int val);

	/**
	 * Gets background sprite.
	 * \return reference background sprite object.
	 */
	virtual Sprite& GetSprite();

	/**
	 * Sets sprite to be used when drawing this object.
	 * \param val Sprite to be used when drawing this object.
	 */
	virtual void SetSprite(Sprite const& val);

	/**
	 * Tests for the point if its inside this object.
	 * \param pt Point to be tested.
	 * \return true if point is inside otherwise false.
	 */
	virtual bool TestPoint(const PointI& pt);

	/**
	 * Gets objects layer (layer is used to short objects drawing, 0 is topmost and 1 is back most).
	 * \return objects layer.
	 */
	float GetLayer();

	/**
	 * Sets objects layer (layer is used to short objects drawing, 0 is topmost and 1 is back most).
	 * \param val objects layer.
	 */
	void SetLayer(float val);

	// Sets the Capture.
	/**
	 * Gets relative position of the provided rectangle.
	 * \param pos Rectangle to be converted to this objects relative position.
	 * \param transform If camera transformation should be used.
	 * \return Transformed rectangle.
	 */
	RectangleF ToRelativePos(RectangleF const& pos, bool transform = false);

	/**
	 * Gets absolute position of the provided rectangle ( if rectangle is 0 this returns this objects absolute position ).
	 * \param pos Pointer to the rectangle to be converted to the absolute position of this object.
	 * \return Absolute position of this object if pos was null otherwise transformed pos rectangle.
	 */
	RectangleF GetAbsolutePos(const RectangleF* pos = 0);

	/**
	 * Sets mouse capture so this object received all mouse events even if cursor is out this objects area.
	 * \note ReleaseCapture() must be called to release mouse when done with capture process otherwise other object will not receive mouse events till ReleaseCapture() is called.
	 */
	void SetCapture();

	/**
	 * Releases mouse input capture.
	 */
	void ReleaseCapture();

	/**
	 * Sets this object selected to it receives all key events.
	 */
	void SetSelected();

	/**
	 * Is this object is selected.
	 * \return true if this object is selected otherwise false.
	 */
	bool IsSelected();

	/**
	 * Is mouse capture is set for this object.
	 * \return true if mouse capture is set for this object otherwise false.
	 */
	bool IsCaptured();

	/**
	 * Is this object is set to draw on top of all other objects.
	 * \return true if this object is set to draw on top of all other objects otherwise false.
	 */
	bool IsDrawTop();

	/**
	 * Is this object is in editing mode.
	 * \return true if this object is in editing mode otherwise false.
	 */
	bool IsEditing();

	/**
	 * Sets this object to be drawn on top of all other objects.
	 */
	void SetDrawTop();

	/**
	 * Resets draw order of this object to previous one.
	 */
	void ReleaseDrawTop();

	/**
	 * Gets if this objects is selected or not to receive all key inputs.
	 * \return true if this objects is selected otherwise false.
	 */
	bool GetSelected();

	/**
	 * Unselects this object.
	 */
	void ReleaseSelected();

	/**
	 * Gets object out of the editing mode.
	 */
	void ReleaseEditor();
	virtual void Release(){};

	virtual void Process( unsigned int Msg, unsigned int Param1, void* Param2 );

	virtual void Update( float delta );
	virtual bool Init();
	virtual void Draw(C_Renderer& rnd);

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual Object* Clone();

	/**
	 * This function is called when sprite animation is finished.
	 */
	virtual void SpriteAnimationFinished();

	//Input
	/**
	 * Called when mouse button is pressed when pointer is on this object.
	 * \param button Number of the button pressed 0 for left 1 for right 1 < for other buttons.
	 * \param buttons Currently pressed buttons (planning to use this for touch markers).
	 * \param pt Cursor's absolute position.
	 */
	virtual void MouseDown(unsigned short button, unsigned short buttons, PointI const& pt);

	/**
	 * Called when mouse button is released when pointer is on this object.
	 * \param button Number of the button pressed 0 for left 1 for right 1 < for other buttons.
	 * \param buttons Currently pressed buttons (planning to use this for touch markers).
	 * \param pt Cursor's absolute position.
	 */
	virtual void MouseUp(unsigned short button, unsigned short buttons, PointI const& pt){};

	/**
	 * Called mouse pointer us moved on this object.
	 * \param pt Cursor's absolute position.
	 */
	virtual void MouseMove(PointI const& pt){};

	/**
	 * Called when mouse scroller is used.
	 * \param wheel scroll amount.
	 * \param buttons buttons Currently pressed buttons (planning to use this for touch markers).
	 * \param pt Cursor's absolute position.
	 */
	virtual void MouseWheel(int wheel, unsigned short buttons, PointI const& pt){};

	/**
	 * Called when key is pressed.
	 * \param key Key code of the key pressed.
	 */
	virtual void KeyDown(unsigned int key){};

	/**
	 * Called when key is released.
	 * \param key Key code of the key released.
	 */
	virtual void KeyUp(unsigned int key){};

	/**
	 * Called when pressed key is character key.
	 * \param key Char code of the key pressed.
	 */
	virtual void KeyChar(wchar_t key){};

	friend class ObjectConnection;
};

/**
 * Draws sprite with realtime position, rotation, color animation effects.
 */
class AnimableObject : public DrawableObject
{
protected:
	float m_current; ///< Time passed from when object was created.
	float m_cpos; ///< Current animation position.
	float m_scale; ///< Current scale.

public:
	float m_delay; ///< Time delay for starting animation.
	float m_speed; ///< Speed of the animation must be less than or equal to 1.
	PointF m_spos; ///< Scale start.
	PointF m_epos; ///< Scale end.
	float m_rspos; ///< Rotation start position.
	float m_repos; ///< Rotation end position.
	unsigned int m_scolor; ///< Start color.
	unsigned int m_ecolor; ///< End color.
	int m_animtype; ///< Animation type (1 - position, 2 - Rotation, 4 - Color, 8 - Scale) .
	int m_playtype; ///< Play type (0 - Forward, 1 - Backward, 2 - loop).
	Resource m_sound;  ///< Sound played when animation is strated.
	bool m_sloop; ///< Whether to loop animation.
	bool m_pingpong; ///< Whether to loop sound.
	C_SoundPlayer m_player; ///< Sound player object to play sound.
	float m_scalefrom; ///< Scale start.
	float m_scaleto; ///< Scale end.

	/** \copydoc DrawableObject::DrawableObject(unsigned int, RectangleF const&, Resource, unsigned int, float, bool, unsigned int, ObjectsContainer*, bool) */
	AnimableObject(unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true);

	virtual void Update( float delta );

	virtual void Draw(C_Renderer& rnd);

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	/**
	 * Resets all animations.
	 */
	void Reset();

	virtual Object* Clone();
};

/**
 * Draws text and background sprite.
 */
class GUIText : public AnimableObject
{
public:
	Resource m_Font; ///< Font resource used to draw text.
	unsigned int m_DrawStyle; ///< Font alignment. combination of (FONT_STYLE_LEFTALIGN, FONT_STYLE_SINGLELINE, FONT_STYLE_RIGHTALIGN, FONT_STYLE_CENTERALIGN, FONT_STYLE_VCENTERALIGN).
	UIWString m_Text; ///< Text to be drawn.
	unsigned int m_Color; ///< Text color.
	bool m_Shadow; ///< Whether to drop shadow when drawing text.
	RectangleF m_TextPadding; ///< Padding to be used for text draw area.
	unsigned int m_ShadowColor; ///< Shadow color.
	float m_fscale; ///< Font scale 1 is no scale.

	ResourceManager* m_mgr; ///< Resource manager used with this object.

	/**
	 * Default Constructor (all these parameters are automatically populated by object builder unless you are registering object itself).
	 * \param mgr Link to the resource manager can use Game->GetResourceManager();
	 * \param ID ID of this object
	 * \param location Position of this object.
	 * \param sprite Background sprite used with this object.
	 * \param backcolor Color of the sprite.
	 * \param rotation Rotation of this object.
	 * \param visible true if object is visible else false.
	 * \param flags Flags for this object.
	 * \param parent parent of this object.
	 * \param update whether to enable update function or not.
	 */
	GUIText( ResourceManager* mgr = 0, unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true);

	/**
	 * Virtual destructor.
	 */
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

/**
 * Provides object/interface to get click events from user.
 */
class GUIButton :  public GUIText
{
protected:
	unsigned int state; ///< Current pressed state of this button.
	unsigned int prevstate; ///< Previous state of this button.
    float CurrentAniPos; ///< Current color start animation position.
	float CurrentAniPos2; ///< Current color end animation position.
	bool Checked; ///< If button if checkbox or radio button this tells its checked state.
	unsigned int m_CheckBoxSize; ///< If type of button is checkbox or radio defined in flags this defined checkbox size.
	float m_ttimer; ///< Timer used for tool tip animation.
	PointI m_mousept; ///< Mouse cursor position used to show tool tip.
	Resource sound; ///< Sound resource to play sound when button has been clicked.
	C_SoundPlayer splayer; ///< Sound player to play sound when button is clicked.

public:
	unsigned int m_PressedColor; ///< Color to set when button is pressed.
	unsigned int m_NormalColor; ///< Button normally has this color.
	unsigned int m_MouseOverColor; ///< Color to set when mouse cursor is over this button.
	unsigned int m_CheckedColor; ///< Color to set for check symbol.
	unsigned int m_CCheckColor; ///< Temporary Color to set for check symbol.
	unsigned int m_DColor; ///< Color to set when button is disabled.
	bool m_Animate; ///< Whether to enable color transition effect.
	float m_AnimationSpeed; ///< Color transition animation speed.

	Sprite m_CheckBack; ///< Sprite used to draw checkbox's box.
	Sprite m_CheckCheck;///< Sprite used to draw checkbox's check symbol.
	Sprite m_RadioBack;///< Sprite used to draw radiobutton's box.
	Sprite m_RadioCheck;///< Sprite used to draw radiobutton's check symbol.
	Sprite m_ToolTip;///< Sprite to show as tool tip.
	Sprite m_MouseOver; ///< Sprite to draw when mouse cursor is over this button.
	Sprite m_Normal; ///< Sprite to draw when button is in normal state.
	Sprite m_Disabled; ///< Sprite to draw when button is disabled.

	float m_maxttimer; ///< Tool tip will be shown after this amount of time in seconds.
	float m_twidth; ///< Width of the the tooltip  sprite.
	float m_theight; ///< Height of the the tooltip  sprite.

	/**
	 * Set button state from \link ButtonStates \endlink
	 * \param st Button state from \link ButtonStates \endlink .
	 */
	virtual void SetState(unsigned int st);

	/**
	 * Gets button previous state.
	 * \return button previous state, one of the \link ButtonStates \endlink .
	 */
	unsigned int GetPrevState();

	/**
	 * Gets current button state.
	 * \return button current state, one of the \link ButtonStates \endlink .
	 */
	unsigned int GetState();

	/**
	 * Gets current animation position for button's background sprite color transition animation.
	 * \return current animation position for button's background sprite color transition animation.
	 */
	float GetAnimationPos();

	/**
	 * Gets current animation position for button's check symbol sprite color transition animation.
	 * \return current animation position for button's check symbol sprite color transition animation.
	 */
	float GetAnimationPos2();
	
	/**
	 * Sets button's checked state.
	 * \param chk true to set button checked otherwise false.
	 */
	virtual void SetChecked(bool chk);

	/**
	 * Gets buttons checked state.
	 * \return true if button is checked otherwise false.
	 */
	bool GetChecked();

	/**
	 * Sets Checkbox size.
	 * \param val Checkbox size in pixels.
	 */
	void SetCheckBoxSize(unsigned int val);

	/**
	 * Gets checkbox size in pixels.
	 * \return Checkbox size in pixels.
	 */
	unsigned int GetCheckBoxSize();

	/** \copydoc GUIText::GUIText(ResourceManager*, unsigned int, RectangleF const&, Resource, unsigned int backcolor, float rotation, bool, unsigned int, ObjectsContainer* parent, bool) */
	GUIButton( ResourceManager* mgr = 0, unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true );

	virtual void Process(unsigned int Msg, unsigned int Param1, void* Param2);
	virtual void Release();

	//General functions
	virtual void Update( float delta );
	virtual bool Init();
	virtual void Draw(C_Renderer& rnd);

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	/** Called on button click. */
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

	/** Called when buttons check state changed */
	virtual void OnChecked(){};
};

/**
 * Edit box used to get text input from the user.
 */
class GUIEdit : public GUIText
{
protected:
	unsigned int DrawFrom; ///< Offset in text from where to start drawing text.
	unsigned int ETime; ///< Used to show caret blink animation.
	bool Selecting; ///< Whether is selecting mode.
	unsigned int SelectionStart; ///< From where selection starts.
	bool m_CurVisible; ///< Whether caret is visible.

public:
	Sprite EditBack; ///< Edit box's background sprite.
	C_Renderer* m_renderer; ///< Renderer used to draw this object.
	unsigned int m_MaxLength; ///< Used to restrict max character input.
	unsigned int m_SelColor; ///< Background color for showing selected text.
	unsigned int m_CColor; ///< Color of the Caret.
	unsigned int m_Bk2Color; ///< Edit box's background color.
	unsigned int m_BlinkSpeed; ///< Amount of time to show/hide caret.
	unsigned int m_MouseBounds; ///< Mouse bounds used for auto text clipping using mouse cursor.
	unsigned int m_SelStart; ///< Text selection start.
	unsigned int m_SelLength; ///< Text selection length.

	/**
	 * Calculates text length in pixels.
	 * \param start Selection start.
	 * \param end Selection End.
	 * \return Length of the selected text in pixels.
	 */
	unsigned int GetTextLength(unsigned int start, unsigned int end);

	/**
	 * Gets from where text starts drawing.
	 * \return offset from where text starts drawing.
	 */
	unsigned int GetDrawFrom();

	/**
	 * Returns position(offset) of the text at specified point.
	 * \param r1 Provide rectangle area where text is being drawn.
	 * \param pt Point to check intersection for.
	 * \return position(offset) of the text at specified point if not found then returns ERRCONST (4294967295u).
	 */
	unsigned int GetPosFromPoint(RectangleI r1, PointI* pt);

	/**
	 * Returns rectangle containing size and position at specified offset of text.
	 * \param r1 Pointer to the rectangle structure to receive size and position.
	 * \param pos Position(Offset) of the text.
	 * \return true if operation was successful otherwise false.
	 */
	bool GetRectFromPos(RectangleI* r1, unsigned int pos);

	/**
	 * Auto scrolls text to clip text outside drawing area.
	 * \param pos offset of the text to view.
	 * \return true of operation was successful otherwise false.
	 */
	bool AdjustPosition(unsigned int pos);

	/**
	 * Deletes character from current caret position.
	 * \param val 0 to delete char after caret, 1 to delete char at back of the caret (if there text is selected then selected text will be deleted instead).
	 */
	void DeleteChar(int val);

	/**
	 * Moves caret to specified position (char offset).
	 * \param val Offset of the text where to move caret.
	 */
	void MoveCursor(int val);

	/** Default Constructor (all these parameters are automatically populated by object builder unless you are registering object itself).
	 * \param renderer Renderer used to calculate rendered character sizes.
	 * \param mgr Link to the resource manager can use Game->GetResourceManager();
	 * \param ID ID of this object
	 * \param location Position of this object.
	 * \param sprite Background sprite used with this object.
	 * \param backcolor Color of the sprite.
	 * \param rotation Rotation of this object.
	 * \param visible true if object is visible else false.
	 * \param flags Flags for this object.
	 * \param parent parent of this object.
	 * \param update whether to enable update function or not.
	 */
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

/**
 * Shows progress using progressbar or image progress, can also be used as slider by setting slider type in flags.
 */
class GUIProgress : public DrawableObject
{
protected:
	float Min; ///< Minimum value.
	float Max; ///< Maximum value.
	float Value; ///< Current value.
	bool Selecting; ///< Whether thumb is grabbed by cursor.

public:
	float GetMin();	///< Returns minimum value this object can have.
	float GetMax(); ///< Returns maximum value this object can have.
	float GetValue(); ///< Returns object's current value.

	void SetMin(float val);	///< Sets minimum value this object can have.
	void SetMax(float val); ///< Sets maximum value this object can have.
	void SetValue(float val); ///< Sets this objects progress/thumb value.

	Sprite m_progressback; ///< Sprite to be shown for progress.
	RectangleF m_Region; ///< Adds padding for progress view/sprite.
	unsigned int m_ProgressColor; ///< Color of the progress.
	unsigned int m_Bk2Color; ///< Background color of the progress.
	unsigned int m_ThumbSize; ///< Size of the thumb for slider when slider type is set in flags.
	unsigned int m_MoveSize; ///< How much to move when clicked on silder.
	bool clipping; ///< If true Shows progress as clipped image.
	bool imgprogress; ///< if true only uses images to view progress.
	unsigned int startind; ///< Start animation index of the sprite to show progress using images.
	unsigned int endind; ///< End Animation index of the sprite to show progress using images.

	/** \copydoc GUIText::GUIText(ResourceManager*, unsigned int, RectangleF const&, Resource, unsigned int backcolor, float rotation, bool, unsigned int, ObjectsContainer* parent, bool) */
	GUIProgress( ResourceManager* mgr = 0, unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true );

	virtual void Release();

	virtual void Process(unsigned int Msg, unsigned int Param1, void* Param2);

	//General functions
	/**
	 * Called when value/progress of this object is changed.
	 */
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

/**
 * Automatically shows current resource loading progress.
 */
class GUILoadProgress : public GUIProgress
{
protected:

public:
	/** \copydoc GUIText::GUIText(ResourceManager*, unsigned int, RectangleF const&, Resource, unsigned int backcolor, float rotation, bool, unsigned int, ObjectsContainer* parent, bool) */
	GUILoadProgress( ResourceManager* mgr = 0, unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true );

	//General functions
	virtual void Update( float delta );
	virtual Object* Clone();
};

/**
 * Used as helper to edit objects in editing mode.
 */
class ObjectEditor : public DrawableObject
{
protected:
	/**
	 * Edit modes (used to define which editing handle is selected).
	 */
	enum EditMode
	{
		None,///< None
		C,   ///< Center
		L,   ///< Left
		T,   ///< Top
		R,   ///< Right
		B,   ///< Bottom
		RB,  ///< RightBottom
		RT,  ///< RightTop
		LB,  ///< LeftBottom
		LT   ///< LeftTop
	};

	EditMode m_edit; ///< Current editing mode.
	bool m_mousedown; ///< If mouse button is currently pressed.
	PointI m_oldpos; ///< Old position of the cursor where mouse button was pressed.

protected:
	DrawableObject* m_object; ///< Object currently being edited.
	int m_space; ///< Amount of empty space between editing object and editor.

	/**
	 * Returns which editing handle is under specified point.
	 * \param pt Point to use for checking intersection.
	 * \return Editing handle which is under this point.
	 */
	EditMode GetIntersect(PointI const& pt);

public:
	bool m_snaptogrid; ///< If enabled snaps position to grid created using m_gridsize.
	float m_gridsize; ///< Used to snap positions using this amount.

	/**
	 * Sets currently editing object to null.
	 */
	void ClearObject();

	/**
	 * Sets current editing object.
	 * \param val Pointer to the object to be edited.
	 */
	void SetObject(DrawableObject* val);

	/**
	 * Gets current editing object.
	 */
	DrawableObject* GetObject();

	/**
	 * Sets amount of space between editing object and editor.
	 * \param val Amount of space between editing object and editor.
	 */
	void SetSpace(int val);

	/**
	 * Gets amount of space between editing object and editor.
	 * \return Amount of space between editing object and editor.
	 */
	int GetSpace();

	/**
	 * Synchronizes editor's position and size with editing object's position and size.
	 */
	void UpdateSize();

	/**
	 * Synchronizes editing object's position and size with editor's position and size.
	 */
	void UpdateObjectSize();

	/** \copydoc DrawableObject::DrawableObject(unsigned int, RectangleF const&, Resource, unsigned int, float, bool, unsigned int, ObjectsContainer*, bool) */
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

/**
 * Container for objects (manages objects).
 */
class ObjectsContainer : public DrawableObject
{
private:
	bool m_clicked; ///< Whether click event happened on any of children.

protected:
	list<Object*> Objects; ///< List of objects.
	set<Object*> DeleteQueue; ///< Delete queue used for queuing objects for deletion when process delete queue is called.

	void Clean(); ///< Cleans/Deletes all objects.
	C_Window* m_win; ///< Window used when disconnecting input signals.
	ObjectEditor* m_editor; ///< Object editor object used as helper object for editing other objects.
	ResourceManager* m_resmanager; ///< Resource manager used by this object to load resources etc.
	Camera2D m_Camera; ///< Camera settings.

public:
	bool m_drawboundingbox; ///< Whether to draw bounding boxes around objects.

	list<Object*>::iterator GetObjectInteratorBegin(); ///< Returns object list's begin iterator.
	list<Object*>::iterator GetObjectInteratorEnd(); ///< Returns object list's end iterator.

	/**
	 * Adds sepecified object to the delete queue for deletion when ProcessDeleteQueue() is called.
	 * \param val Pointer to the object to be added to the delete queue.
	 */
	void AddToDeleteQueue(Object* val);

	/**
	 * Clears delete queue and deletes all objects in delete queue.
	 */
	void ProcessDeleteQueue();

	/**
	 * Setups camera using camera settings.
	 * \param camera camera settings to be used to setup camera for this container.
	 */
	void SetupCamera(const Camera2D& camera);

	/**
	 * Adjusts/resizes this container according to provided device's draw buffer size.
	 * \param dev Reference to the device used to get screen buffer size.
	 */
	void AdjustSize(C_Device& dev);

	/**
	 * Adjusts area of this object according to camera settings.
	 */
	void AdjustCamera();

	/**
	 * Centers camera.
	 */
	void CenterCamera();

	/**
	 * Moves camera by specified amount.
	 * \param x X position to move.
	 * \param y Y position to move.
	 */
	void MoveCamera(float x, float y);

	/**
	 * Zooms by specified factor.
	 * \param factor Camera is zoomed by this amount.
	 */
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

	/** \copydoc GUIText::GUIText(ResourceManager*, unsigned int, RectangleF const&, Resource, unsigned int backcolor, float rotation, bool, unsigned int, ObjectsContainer* parent, bool) */
	ObjectsContainer( ResourceManager* mgr, unsigned int ID = 0, const RectangleF& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true );

	/**
	 * Tells whether mouse click has been processed by any of child objects.
	 * \return true if click has been processed by any of child object otherwise false.
	 */
	bool GetMouseProcess();

	/**
	 * Sets all objects visible.
	 */
	void ShowAll();

	/**
	 * Sets all objects invisible.
	 */
	void HideAll();

	/**
	 * Scales all objects by specified amount.
	 * \param scale scales all objects by specified amount.
	 */
	void ScaleAll(float scale);

	/**
	 * Sets objects being editor to null.
	 */
	void ClearEditorObject();

	/**
	 * Sets specified object for editing.
	 * \param obj Pointer to the object to set for editing.
	 */
	void SetEditorObject(DrawableObject* obj);

	/**
	 * Finds object by id and returns its pointer.
	 * \param id ID of the object to find.
	 * \return Object with same ID as specified if found otherwise null.
	 */
	virtual Object* GetObjectByID(unsigned int id);

	/**
	 * Adds new object to this container.
	 * \note Use new operator on provided object (delete operator will be automatically used on this object).
	 * \param obj Pointer to the object to be added to this container.
	 */
	virtual void AddObject(Object* obj);

	/**
	 * Removes object by its index.
	 * \param Index Index of the object in the list.
	 */
	virtual void RemoveObject(unsigned int Index);

	/**
	 * Removes object specified by its pointer.
	 * \param ctrl Pointer to the object to be deleted.
	 */
	virtual void RemoveObject(Object* ctrl);

	/**
	 * Returns object by its index in list.
	 * \param Index Object's index in list.
	 * \return
	 */
	virtual Object* GetObject(unsigned int Index);

	/**
	 * Clears all container and deletes all objects in this container.
	 * \param full if true Editor object is also deleted, if false Editor object is not deleted.
	 */
	virtual void Clear(bool full = false);

	/**
	 * Queues all of the objects for deletion.
	 */
	virtual void QueueClear();

	virtual void Update(float delta);

	/**
	 * Tells whether this container is in editing mode.
	 * \return true if container is in editing mode otherwise false.
	 */
	virtual bool GetEditable();

	/**
	 * Sets editing state of this container.
	 * \param val Editing state of this container to set.
	 */
	virtual void SetEditable(bool val);

	/**
	 * Returns pointer to the object being edited by editor object.
	 * \return pointer to the object being edited by editor object.
	 */
	DrawableObject* GetEditorObject();

	/**
	 * Recursively searches for object being edited even in child objects who are containers.
	 * \return pointer to the object being edited by editor object.
	 */
	DrawableObject* GetEditorObjectRecursive();

	/**
	 * Returns number of objects are in this container.
	 * \return number of objects are in this container.
	 */
	unsigned int GetSize();

	virtual void Process(unsigned int Msg, unsigned int Param1, void* Param2);

	/**
	 * Returns object which contains this point in its drawing area.
	 * \param Param2 Point to check for.
	 * \return Null if none of objects in this container contains specified point otherwise returns first object which contains this point in its draw area.
	 */
	Object* GetIntersect(PointI const& Param2);

	/**
	 * Updates editor size according to its selected(editing) object.
	 */
	void UpdateEditor();

	virtual Object* Clone();

	virtual void Release();
	virtual bool Init();
	virtual void Draw(C_Renderer& rnd);
	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	/**
	 * Saves all controls in this container using provided BaseIO.
	 * \param str IO object to be used for saving objects.
	 */
	virtual void SaveAll( BaseIO& str );

	/**
	 * Loads all controls into this container using provided BaseIO (current objects are not cleared/deleted when adding newly loaded objects).
	 * \param str IO object to be used for loading objects
	 * \param builder Builder to be used for build loaded object types.
	 * \param editor Whether to load container objects in edit mode.
	 * \param readtype Whether to read type of this object first before loading.
	 * \param append Whether to append all loaded objects to this object without clearing all settings of this container.
	 */
	virtual void LoadAll( BaseIO& str, ObjectBuilder& builder, bool editor, bool readtype = true, bool append = false);

	virtual void MouseDown(unsigned short button, unsigned short buttons, PointI const& pt);
	virtual void MouseUp(unsigned short button, unsigned short buttons, PointI const& pt);
	virtual void MouseMove(PointI const& pt);
	virtual void MouseWheel(int wheel, unsigned short buttons, PointI const& pt);

	virtual void KeyDown(unsigned int key);
	virtual void KeyUp(unsigned int key);
	virtual void KeyChar(wchar_t key);

	/**
	 * Auto connects mouse/keyboard input signals from provided window to this container.
	 * \param window Window used to connecting input signals.
	 */
	void AutoConnectInput(C_Window* window);

	/**
	 * Disconnects all mouse/keyboard signals.
	 */
	void DisconnectInput();

	/**
	 * Virtual container destructor deletes/deallocates memory of all objects it owns.
	 */
	virtual ~ObjectsContainer();
};

/**
 * Animated buttons container which arranges all its item in a circle.
 */
class GUICircleMenu : public ObjectsContainer
{
protected:
	float m_radius; ///< Radius of the circle.
	float m_sangle; ///< Start angle for starting to arrange items from angle's position on circle.

public:
	/** \copydoc GUIText::GUIText(ResourceManager*, unsigned int, RectangleF const&, Resource, unsigned int backcolor, float rotation, bool, unsigned int, ObjectsContainer* parent, bool) */
	GUICircleMenu( ResourceManager* mgr, unsigned int ID = 0, const RectangleF& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true );
	
	void AddObject(Object* obj);

	/** Arranges all objects positions into circle */
	void Arrange();

	/**
	 * Gets radius of the circle used for arranging items.
	 * \return radius of the circle used for arranging items.
	 */
	float GetRadius();

	/**
	 * Sets radius of the circle used for arranging all objects.
	 * \param val radius of the circle used for arranging all objects.
	 */
	void SetRadius(float val);

	/**
	 * Gets start angle of the circle from where arranging of item will start.
	 * \return start angle of the circle from where arranging of item will start.
	 */
	float GetStartAngle();

	/**
	 * Sets start angle of the circle from where arranging of item will start.
	 * \param val start angle of the circle from where arranging of item will start.
	 */
	void SetStartAngle(float val);

	/**
	 * Sets parent of this object.
	 * \param val Parent container.
	 */
	void SetParent(ObjectsContainer* val);

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual Object* Clone();
};

/**
 * Container which can arrange all its objects in a table of rows and columns.
 */
class GUITable : public ObjectsContainer
{
protected:
	float twidth; ///< Width of the tiles.
	float theight; ///< Height of the tiles.
	float spacex; ///< X space between tiles.
	float spacey; ///< Y space between tiles.
	unsigned int tilescx; ///< Tiles column count.

public:
	/** \copydoc GUIText::GUIText(ResourceManager*, unsigned int, RectangleF const&, Resource, unsigned int backcolor, float rotation, bool, unsigned int, ObjectsContainer* parent, bool) */
	GUITable( ResourceManager* mgr, unsigned int ID = 0, const RectangleF& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true );
	
	void AddObject(Object* obj);

	/** Arranges all objects positions/sizes into table */
	void Arrange();

	/** Sets parent container of this container */
	void SetParent(ObjectsContainer* val);

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	/**
	 * Sets tile size.
	 * \param width Tiles width.
	 * \param height Tiles height.
	 */
	void SetTileSize(float width, float height);

	/**
	 * Sets amount of space between tiles.
	 * \param spacex X space between tiles.
	 * \param spacey Y space between tiles.
	 */
	void SetTileSpace(float spacex, float spacey);

	/**
	 * Sets number of columns.
	 * \param count Number of columns.
	 */
	void SetColumns(unsigned int count);

	/**
	 * Returns tiles size.
	 * \return Tiles size.
	 */
	PointF GetTileSize();

	/**
	 * Gets current space between tiles.
	 * \return Space between tiles.
	 */
	PointF GetTileSpace();

	/**
	 * Gets number of columns in table.
	 * \return number of columns in table.
	 */
	unsigned int GetColumns();

	virtual void PostLoadAll();

	virtual Object* Clone();
};

/**
 * Patricle structure/configuration.
 */
struct Particle
{
	float posx; ///< Particle's X position.
	float posy; ///< Particle's Y position.
	float velx; ///< Particle's X velocity.
	float vely; ///< Particle's Y velocity.
	unsigned int color; ///< Particle's color.

	float ctime; ///< Time in seconds passed since particle was created.
	float ttime; ///< Max particle time/life in seconds.

	float scale; ///< Particle's scale.
	float rotation; ///< Particle's rotation.

	Particle(); ///< Default constructor initializing all to 0.
};

/**
 * Emiter item used to add emission positions.
 */
struct EmitItem
{
	RectangleF pos; ///< Emission area.
	float m_ctime; ///< Time in seconds passed since emission object was created.
	float m_life; ///< Emission items life in seconds.

	/**
	 * Contructor of the emission item initialized ctime to 0 and life to 1.
	 */
	EmitItem()
	{
		m_ctime = 0;
		m_life = 1;
	}
};

/** \addtogroup EmitterTypes
* \{
*/
#define EMITTOR_TYPE_PLANE	0 ///< Plane shaped emitter.
#define EMITTOR_TYPE_CIRCLE	1 ///< Circle shaped emitter.
/** \} */

/**
 * Particle engine generated particle effects.
 */
class ParticleEngine : public AnimableObject
{
protected:
	vector<EmitItem> emitlist; ///< List of emitter items.
	list<Particle> particles; ///< List of particles.

public:
	Sprite m_psprite; ///< Sprites used for drawing particles.

	int m_emission; ///< Number of particles to emit each cycle.

	float m_startdelay; ///< Startup delay.
	float m_tmptime; ///< Temporary time used for start delay.
	bool m_activated; ///< Whether particle emission is enabled.
	bool m_emitlistonly; ///< Emmit only emission items but not particles.

	unsigned int startcolor; ///< Color of the particle when its created.
	unsigned int endcolor; ///< Color of the particle when its being destroyed.
	float scalefreq; ///< Scale frequency used to scale particle by this amount each cycle.
	float angularvel; ///< Angular velocity of the particle

	float m_cetime; ///< Time in seconds passed since last emission.
	float m_maxtime; ///< Time in seconds for emission interval.

	int velxstart; ///< Paticle's minimum X velocity when its created.
	int velxend; ///< Paticle's maximum X velocity when its created.

	int velystart; ///< Paticle's minimum Y velocity when its created.
	int velyend; ///< Paticle's maximum Y velocity when its created.

	bool randomx; ///< Whether to randomize particle's X velocities at start using start and end velocity.
	bool randomy; ///< Whether to randomize particle's Y velocities at start using start and end velocity.

	float m_ttimemax; ///< Maximum life particle can have.
	float m_ttimemin; ///< Minimum life particle can have.
	bool randomtime; ///< Whether to randomize particle's life using min and max particle life.

	int particlesizestart; ///< Minimum particle size.
	int particlesizeend; ///< Maximum particle size
	bool randomsize; ///< Randomize particle sizes using min and max particle sizes.

	float forcex; ///< X force applied of the particles.
	float forcey; ///< Y force applied of the particles.

	int m_emittortype; ///< Type of the emitter to use can be one of \link EmitterTypes \endlink .

	bool randomxpos; ///< Randomize X position of the particles at emission.
	bool randomypos; ///< Randomize Y position of the particles at emission.

	bool relativevel; ///< Whether to use velocity relative to emitters current position and rotation.

	int circleradstart; ///< Minimum circle radius for circle shaped emitter.
	int circleradend; ///< Maximum circle radius for circle shaped emitter.
	float cianglestart; ///< Start angle of the circle emission.
	float ciangleend; ///< End angle of the circle emission.
	bool randomradius; ///< Whether to randomize circle emitter's size using min and max circle radius.

	bool cirelativevel; ///< Whether to use velocity relative to circle's rotation.
	bool gravitycenter; ///< Whether to add gravity at the center of the emitter (forcex and forcey will be used for computation of the force).

public:
	/** \copydoc DrawableObject::DrawableObject(unsigned int, RectangleF const&, Resource, unsigned int, float, bool, unsigned int, ObjectsContainer*, bool) */
	ParticleEngine(unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true);

	/**
	 * Emits particles from specified area.
	 * \param rect Emission rectangle area.
	 */
	void EmitParticles(const RectangleF& rect);

	/**
	 * Emits particles using specified settings.
	 */
	void EmitParticles();

	/**
	 * Emits particles in specified area.
	 * \param x X position from where to emit particles.
	 * \param y Y position from where to emit particles.
	 * \param size Size of the area.
	 */
	void EmitParticles(float x, float y, float size = 1.0f);

	/**
	 * Adds emission rectangle to do emission in this rectangle area for specified time in seconds.
	 * \param rect Emission rectangle area.
	 * \param life Life of this emission in seconds.
	 */
	void AddEmitRect(const RectangleF& rect, float life);

	/**
	 * Adds emission area to do emission in this square area for specified time in seconds.
	 * \param x X position of the square.
	 * \param y Y position of the square.
	 * \param life Life of this emission in seconds.
	 * \param size Size of the square.
	 */
	void AddEmitRect(float x, float y, float life, float size = 1.0f);

	/**
	 * Clears all emission rectangles.
	 */
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

/**
 * Timer object which invokes signals and object messages at specified interval.
 */
class TimerObject : public DrawableObject
{
private:
	float m_ctime; ///< Time passed since last signal.
	bool m_started; ///< Weather timer is started.

public:
	float m_interval; ///< Interval in seconds.
	bool m_loop; ///< Weather to keep signaling in specified interval.

	Signal0 evt_Action; ///< This is signaled when timer riches its interval time.

	/** \copydoc DrawableObject::DrawableObject(unsigned int, RectangleF const&, Resource, unsigned int, float, bool, unsigned int, ObjectsContainer*, bool) */
	TimerObject(unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true);

	void StartTimer(); ///< Starts Timer.
	void StopTimer(); ///< Stops timer.
	void PauseTimer(); ///< Pauses timer.

	///Called when timer riches interval time.
	virtual void OnInterval() {evt_Action();};

	virtual void Update( float delta );

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual Object* Clone();
};

/**
 * Renders tiles.
 */
class Tiles : public DrawableObject
{
private:
	bool editing; ///< Does tiles are in editing mode.
	string tmpstr; ///< temporary string used for getting number inout from user.
	int cindex; ///< Currently selected sprite index.
	float cangle; ///< Currently selected tile angle.

public:
	float twidth; ///< Width of the tiles.
	float theight; ///< Height of the tiles.
	float spacex; ///< X space between tiles
	float spacey; ///< Y space between tiles
	unsigned int tilescx; ///< Number of columns.
	unsigned int tilescy; ///< Number of rows.
	bool relative; ///< if true draws tiles relative to this objects position.
	vector<int> data; ///< Tile sprite indexes.
	vector<float> angles; ///< Tile sprite angles.

public:
	/** \copydoc DrawableObject::DrawableObject(unsigned int, RectangleF const&, Resource, unsigned int, float, bool, unsigned int, ObjectsContainer*, bool) */
	Tiles(unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true);
	
	virtual void MouseDown(unsigned short button, unsigned short buttons, PointI const& pt);
	virtual void KeyDown(unsigned int key);

	virtual void Update( float delta );
	virtual void Draw(C_Renderer& rnd);

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual Object* Clone();
};

/**
 * Renders texts with varius effects.
 */
class GUITextEffects : public GUIText
{
protected:
	float ctime; ///< Current animation time.

public:
	/**
	 * Type of the text effect.
	 */
	enum EffecTtype
	{
		SinCurve = 0,///< Sin Curve
		HSinCurve,   ///< Horizontal Sin Curve
		Linear,      ///< Linear
		Colors,      ///< Colors
		Typing       ///< Typing
	};

	float speed; ///< Speed of the animation.
	float param1; ///< Parameter used for the animation tuning.
	float param2; ///< Parameter used for the animation tuning.
	float param3; ///< Parameter used for the animation tuning.
	EffecTtype etype; ///< Effect Type.
	bool loop; ///< Whether to keep looping the animation.

	/** \copydoc GUIText::GUIText(ResourceManager*, unsigned int, RectangleF const&, Resource, unsigned int backcolor, float rotation, bool, unsigned int, ObjectsContainer* parent, bool) */
	GUITextEffects( ResourceManager* mgr = 0, unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true );

	//General functions
	virtual void Update( float delta );
	virtual void Draw(C_Renderer& rnd);

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual Object* Clone();
};

/**
 * Used to view image slide show or show images 1 by 1.
 */
class ImageScroller : public DrawableObject
{
private:
	int cursel; ///< Currently selected sprite index.
	float transpos; ///< Current color transition position.

public:
	vector<int> indices; ///< List of animation indexes used with object.
	bool rev; ///< Whether selecting previous image.

public:
	/** \copydoc DrawableObject::DrawableObject(unsigned int, RectangleF const&, Resource, unsigned int, float, bool, unsigned int, ObjectsContainer*, bool) */
	ImageScroller(unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true);
	
	/**
	 * Transits to next image in list.
	 */
	void Next();

	/**
	 * Transits to previous image in list.
	 */
	void Previous();

	virtual void MouseDown(unsigned short button, unsigned short buttons, PointI const& pt);

	virtual void Update( float delta );
	virtual void Draw(C_Renderer& rnd);

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual Object* Clone();
};

/**
 * Gives some functions by its specified function.
 */
class FunctionButton : public GUIButton
{
private:

public:
	/**
	 * Buttons functions.
	 */
	enum Function
	{
		ImgScrollNext,   ///< Image Scroller Next
		ImgScrollPrevious///< Image Scroller Previous
	};

	unsigned int m_targetid; ///< ID of the target object.
	Function m_function; ///< Function of this button.

	/** \copydoc GUIText::GUIText(ResourceManager*, unsigned int, RectangleF const&, Resource, unsigned int backcolor, float rotation, bool, unsigned int, ObjectsContainer* parent, bool) */
	FunctionButton( ResourceManager* mgr = 0, unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true );

	virtual void OnClick();

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual Object* Clone();
};

/**
 * Dial control.
 */
class Dial : public DrawableObject
{
private:
	bool m_mdown; ///< Whether mouse button is pressed on this object.

public:
	float m_startangle; ///< From where to start circle.
	float m_value; ///< Current value of the dial.
	float m_max; ///< Maximum value of the dial.
	float m_min; ///< Minimum value of the dial.

	float m_radius; ///< Radius of the dial.
	float m_dotsize; ///< Size of the dot in the dial.

	bool m_autodotangle; ///< Whether to update dot angle according to its position.

	float m_eangle; ///< Where to end circle.

	unsigned int m_dotcolor; ///< Color of the dot sprite.

	Sprite m_dot; ///< Sprite used to draw dot.

	/** \copydoc DrawableObject::DrawableObject(unsigned int, RectangleF const&, Resource, unsigned int, float, bool, unsigned int, ObjectsContainer*, bool) */
	Dial(unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true);

	virtual void Draw(C_Renderer& rnd);

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual void MouseDown(unsigned short button, unsigned short buttons, PointI const& pt);
	virtual void MouseUp(unsigned short button, unsigned short buttons, PointI const& pt);
	virtual void MouseMove(PointI const& pt);

	virtual Object* Clone();
};

/**
 * Plays specified sounds.
 */
class SoundPlayer : public DrawableObject
{
protected:
	Resource m_sound; ///< Sound resource to play.
	C_SoundPlayer player; ///< Player to play soubd.

public:
	/** \copydoc DrawableObject::DrawableObject(unsigned int, RectangleF const&, Resource, unsigned int, float, bool, unsigned int, ObjectsContainer*, bool) */
	SoundPlayer(unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true);

	/**
	 * Starts playing sound.
	 * \param loop Whether to loop sound.
	 */
	void Start(bool loop = false);

	/**
	 * Stops playing current sound.
	 */
	void Stop();

	/**
	 * Does sound is playing.
	 * \return true if sound is playing otherwise false.
	 */
	bool isPlaying();

	/**
	 * Sets sound resource to be played.
	 * \param name Name of the sound resource.
	 */
	void SetResource(const char* name);

	/**
	 * Sets sound volume.
	 * \param val sound volume (0 - 10k).
	 */
	void SetVolume(long val);

	/**
	 * Gets Sound volume (0 - 10k).
	 */
	long GetVolume();

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual Object* Clone();
};

/**
 * Builder class used to build/create objects.
 */
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
