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

//Animation types
#define ANIM_TYPE_POSX		1
#define ANIM_TYPE_POSY		2
#define ANIM_TYPE_SCALEX	3
#define ANIM_TYPE_SCALEY	4
#define ANIM_TYPE_BCOLOR	5
#define ANIM_TYPE_ROTATION	6
#define ANIM_TYPE_CPOSX		7
#define ANIM_TYPE_CPOSY		8
#define ANIM_TYPE_CZOOM		9

class AnimationKey
{
public:
	Variant val;
	unsigned int frame;

	AnimationKey();

	AnimationKey(unsigned int frame);

	bool operator<(const AnimationKey& str) const
	{
		return frame < str.frame;
	}

	bool operator>(const AnimationKey& str) const
	{
		return frame > str.frame;
	}

	void Save( BaseIO& str );
	void Load( BaseIO& str );
};

class AnimationTimeline;

class ObjectAnimations
{
private:
	ObjectConnection obj;
	map<int, set<AnimationKey> > animations;

public:
	AnimationTimeline* parent;
	void AddKey(int type, Variant val, unsigned int frame);
	void DeleteKey(unsigned int frame);
	void MoveToFrame(unsigned int frame);

	void Save( BaseIO& str );
	void Load( BaseIO& str );

	friend class AnimationTimeline;
};

class AnimationTimeline : public GUIProgress
{
private:
	float cframe;
	unsigned int maxframes;
	map<DrawableObject*, ObjectAnimations> objects;
	vector<ObjectAnimations> tmpanims;
	bool paused;
	DrawableObject* sel;

public:	
	unsigned int fps;
	unsigned int m_markercol;
	int animtype;

	AnimationTimeline( ResourceManager* mgr = 0, unsigned int ID = 0, RectangleF const& location = RectangleF(0,0,100,100), Resource sprite = 0, unsigned int backcolor = 0xFFFFFFFF, float rotation = 0, bool visible = true, unsigned int flags = 0, ObjectsContainer* parent = 0, bool update = true );
	~AnimationTimeline();

	void SetMaxFrames(unsigned int frames);

	void Play(int animtype);
	void Play();
	void Pause();
	void Stop();
	
	//General functions
	virtual void Update( float delta );
	virtual Object* Clone();

	virtual void Draw(C_Renderer& rnd);
	virtual void DrawBoundingBox(C_Renderer& rnd);
	virtual void OnValueChanged();
	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );	
	virtual void PostLoadAll();

	void AddPositionKeys(DrawableObject* obj);
	void AddScaleKeys(DrawableObject* obj);
	void AddBColorKeys(DrawableObject* obj);
	void AddRotationKeys(DrawableObject* obj);
	void AddCPositionKeys(DrawableObject* obj);
	void AddCZoomKeys(DrawableObject* obj);

	friend class ObjectAnimations;
};