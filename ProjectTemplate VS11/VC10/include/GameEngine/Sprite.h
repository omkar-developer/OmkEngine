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

class DrawableObject;

struct SpriteFrame
{
	float delay;
	RectangleI rect;

	SpriteFrame()

	{
		delay = 0.0f;
		rect.left = 0;
		rect.top = 0;
		rect.right = 0;
		rect.bottom = 0;
	}

	SpriteFrame(float delay, RectangleI& rect)
	{
		this->delay = delay;
		this->rect = rect;
	}

	SpriteFrame(float delay, int left, int top, int right, int bottom)
	{
		this->delay = delay;
		this->rect.left = left;
		this->rect.top = top;
		this->rect.right = right;
		this->rect.bottom = bottom;
	}
};

struct SpriteData
{
	vector<int> frame;
};

class SpriteFrameSet : public IResource
{
private:
	vector<SpriteData> data;
	vector<SpriteFrame> frames;
	Resource image;

protected:
	virtual bool LoadFromFile(ResourceManager* mgr);
	virtual void Release();
	virtual void OnReset(){};
	virtual void OnLost(){};

public:
	void SetImage(Resource img);
	Resource GetImage();

	unsigned int AddFrame(float delay, RectangleI rect);
	unsigned int AddFrame(float delay, int left, int top, int right, int bottom);
	void RemoveFrame(unsigned int index);
	unsigned int GetFrameCount();
	SpriteFrame* GetSpriteFrame(unsigned int index);
	unsigned int GetDataCount();
	SpriteData* GetSpriteData(unsigned int i);
	void AddSpriteData(SpriteData& d);
	void RemoveSpriteData(unsigned int i);

	void AutoGenerateGrid(float delay, Size blocksize);

	virtual void* GetResourcePtr();	
	virtual IResource* Clone();
	static void RegisterLoader(ResourceManager* mgr);
};

class Sprite : public IResource
{
public:
	enum AnimationType
	{
		Forward,
		Backward,
		Loop,
		PingPong
	};

private:
	float tdelay;
	unsigned int direction;
	unsigned int index;
	unsigned int offset;
	bool pause;
	AnimationType animtype;
	SpriteData* data;
	ImageResource* img;
	SpriteFrameSet* frames;
	Resource image;	
	unsigned int startind, endind;

protected:
	void Next();

public:
	bool mirrorx;
	bool mirrory;
	float animspeed;
	bool anglesprite;
	bool recmessages;

	DrawableObject* parent;
	bool resetangle;
	bool m_drawbolt;

	void SetImageSet(Resource img);
	Resource GetImageSet() const;

	Sprite();

	void SetAngleSprite(bool val);
	bool GetAngleSprite();

	void SetIndexRange(unsigned int start, unsigned int end);
	unsigned int GetStartIndex();
	unsigned int GetEndIndex();

	void Reset();
	void Resume();
	void Pause();
	void Draw(C_Renderer* r, const RectangleF& dest, unsigned int color, float angle, float layer);
	void DrawTransformed(C_Renderer* r, const PointF& transform, const RectangleF& dest, unsigned int color, float angle, float layer);
	void DrawArea(C_Renderer* r, const RectangleF& src, const RectangleF& dest, unsigned int color, float angle, float layer);

	void Update(float delta);

	void SetIndex(unsigned int i);
	unsigned  int GetIndex() const;
	void SetAnimationType(AnimationType type);
	AnimationType GetAnimationType() const;
	void SetOffset(unsigned int offset);
	unsigned int GetOffset() const;
	RectangleI* GetSpriteRect(unsigned int index);

	virtual void OnLost();
	virtual void* GetResourcePtr();
	virtual bool LoadFromFile( ResourceManager* mgr );
	virtual IResource* Clone();
	virtual void OnReset();
	virtual void Release();

	void PostFinishedMessage();

	static void RegisterLoader(ResourceManager* mgr);

	virtual void Save( BaseIO& str, const char* name = "name=Sprite" );
	virtual void Load( BaseIO& str );

	//events
	Signal0 Fnished;
	Signal0 Step;

	//operators
	const Sprite& Copy(const Sprite& val);
	Sprite(const Sprite& val);
	const Sprite& operator=(const Sprite& val);
};