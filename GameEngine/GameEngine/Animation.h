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

/** Identifiers for Animation type.
\addtogroup AnimationTypes Animation Types
\{ */
#define ANIM_TYPE_POSX		1 ///< Left (X) position animation (will move object along x axis).
#define ANIM_TYPE_POSY		2 ///< Top (Y) position animation (will move object along y axis).
#define ANIM_TYPE_SCALEX	3 ///< Width scaling animation.
#define ANIM_TYPE_SCALEY	4 ///< Height scaling animation.
#define ANIM_TYPE_BCOLOR	5 ///< Background (sprite color) transition animation.
#define ANIM_TYPE_ROTATION	6 ///< Object Rotation animation.
#define ANIM_TYPE_CPOSX		7 ///< Camera X position animation (will move camera along x axis).
#define ANIM_TYPE_CPOSY		8 ///< Camera Y position animation (will move camera along y axis).
#define ANIM_TYPE_CZOOM		9 ///< Camera Zoom in / Zoom out animation.
/** \} */

/**
 * Class holding frame and value of a animation.
 */
class AnimationKey
{
public:
	Variant val; ///< Variant object type stores value for this key at specified frame.
	unsigned int frame; ///< Stores frame number at which value of this key should be same as the value specified by this key.

	/**
	 * Default constructor initializing all to 0.
	 */
	AnimationKey();

	/**
	 * Constructor creating key using frame number.
	 * \param frame Key will be initialized using this frame number.
	 */
	AnimationKey(unsigned int frame);

	/**
	 * Compares frame times.
	 * \param str Reference to the second key to compare this key with.
	 * \return true if frame number of this key is smaller than frame number of the key provided in parameters else false.
	 */
	bool operator<(const AnimationKey& str) const
	{
		return frame < str.frame;
	}

	/**
	 * Compares frame times.
	 * \param str Reference to the second key to compare this key with.
	 * \return true if frame number of this key is larger than frame number of the key provided in parameters else false.
	 */
	bool operator>(const AnimationKey& str) const
	{
		return frame > str.frame;
	}

	/**
	 * Key IO used to save key frame and value using specified IO object.
	 * \param str Reference to a BaseIO object.
	 */
	void Save( BaseIO& str );

	/**
	 * Key IO used to load key frame and value using specified IO object.
	 * \param str Reference to a BaseIO object.
	 */
	void Load( BaseIO& str );
};

class AnimationTimeline;

/**
 * Stores list of keys used by specified object.
 */
class ObjectAnimations
{
private:
	ObjectConnection obj; ///< Link to a object.
	map<int, set<AnimationKey> > animations; ///< List of the keys and animation types for object specified by this class.

public:
	AnimationTimeline* parent; ///< Pointer to the AnimationTimeline object which this object belongs to.

	/**
	 * Adds key to the list.
	 * \note If key with same frame number already exists it will be replaced by new one.
	 * \param type Type of the key, can be combination of [Animation Types](\ref AnimationTypes).
	 * \param val Value of the key, can be any value like (int, float, string) supported by Variant.
	 * \param frame Frame number at which value of this key should be value provided by val parameter.
	 */
	void AddKey(int type, Variant val, unsigned int frame);

	/**
	 * Deletes key at specified frame.
	 * \note If key doesn't exists at specified frame this function will do nothing.
	 * \param frame Frame number of the key being deleted.
	 */
	void DeleteKey(unsigned int frame);

	/**
	 * Moves current frame to specified frame and computes keys for that frame.
	 * \param frame Frame at which current frame should move.
	 */
	void MoveToFrame(unsigned int frame);

	/**
	 * Used to save this object values using specified IO object.
	 * \param str Reference to a BaseIO object.
	 */
	void Save( BaseIO& str );

	/**
	 * Used to save this object values using specified IO object.
	 * \param str Reference to a BaseIO object.
	 */
	void Load( BaseIO& str );

	/**
	 * Friend class animation timeline.
	 */
	friend class AnimationTimeline;
};

/**
 * (Key frame animation class) animates objects by computing their keys at current frame.
 * This class is used to animate objects using their key animations.
 * \n Simple use :
 * ~~~~~~~~~~{.cpp}
 * AnimationTimeline timeline;
 * timeline.AddPositionKeys(&obj); //provide pointer to the object being animated.
 * timeline.fps = 30; // sets frames to be advanced per second.
 * timeline.animtype = 0; // sets forward animation type.
 * timeline.Play(); // Finally set object in play mode so when Update function is called time advances and object is animated.
 * ~~~~~~~~~~
 */
class AnimationTimeline : public GUIProgress
{
private:
	float cframe; ///< Current frame.
	unsigned int maxframes; ///< Last animation frame.
	map<DrawableObject*, ObjectAnimations> objects; ///< List of object animations.
	vector<ObjectAnimations> tmpanims; ///< List of temporary animations.
	bool paused; ///< pause state of the animations.
	DrawableObject* sel; ///< current selected drawable object.

public:	
	unsigned int fps; ///< Play rate in frames per sec.
	unsigned int m_markercol; ///< Marker color.
	int animtype; ///< Animation type 0 Forward, 1 Backward.

	/**
	 * Default Constructor (all these params are automatically populated by object builder).
	 * \param mgr Link to the resource manager can use Game->GetResourceManager();
	 * \param ID ID of this object
	 * \param location Position of this object.
	 * \param sprite Background sprite used with this object.
	 * \param backcolor Color of the sprite.
	 * \param rotation Rotation of this object.
	 * \param visible true if object is visible else false.
	 * \param flags Flags or this object.
	 * \param parent parent of this object.
	 * \param update whether to enable update function or not.
	 */
	AnimationTimeline( ResourceManager* mgr = 0, unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true );

	/**
	 * Destructor just sets current gameworld timeline to null can be retrieved by Game->timeline.
	 */
	~AnimationTimeline();

	/**
	 * Sets animation length.
	 * \param frames Total number of frames for this animation.
	 */
	void SetMaxFrames(unsigned int frames);

	/**
	 * Sets play state to playing so next time Update function is called time is advanced and objects are animated.
	 * \param animtype Animation type 0 is Forward and 1 is Backward.
	 */
	void Play(int animtype);

	/**
	 * Sets play state to playing so next time Update function is called time is advanced and objects are animated.
	 */
	void Play();

	/**
	 * Sets play state to paused so calling Update function has no effect.
	 */
	void Pause();

	/**
	 * Sets play state to paused and resets animation frame to beginning.
	 */
	void Stop();
	
	/**
	 * Advances time (animates objects).
	 * \note if play state is paused this function has no effect.
	 * \param delta Time to advance animation in seconds.
	 */
	virtual void Update( float delta );

	/**
	 * Clones current object.
	 * \return AnimationTimeline object.
	 */
	virtual Object* Clone();

	virtual void Draw(C_Renderer& rnd);
	virtual void DrawBoundingBox(C_Renderer& rnd);
	virtual void OnValueChanged();
	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );	
	virtual void PostLoadAll();

	/**
	 * Adds position animation for specified object.
	 * \param obj Object for which animation is defined.
	 */
	void AddPositionKeys(DrawableObject* obj);

	/**
	 * Adds scaling animation for specified object.
	 * \param obj Object for which animation is defined.
	 */
	void AddScaleKeys(DrawableObject* obj);

	/**
	 * Adds backcolor (sprite color) animation for specified object.
	 * \param obj Object for which animation is defined.
	 */
	void AddBColorKeys(DrawableObject* obj);

	/**
	 * Adds rotation animation for specified object.
	 * \param obj Object for which animation is defined.
	 */
	void AddRotationKeys(DrawableObject* obj);

	/**
	 * Adds camera position animation for specified object.
	 * \param obj Object for which animation is defined.
	 */
	void AddCPositionKeys(DrawableObject* obj);

	/**
	 * Adds camera zoom animation for specified object.
	 * \param obj Object for which animation is defined.
	 */
	void AddCZoomKeys(DrawableObject* obj);

	/**
	 * friend class ObjectAnimations.
	 */
	friend class ObjectAnimations;
};
