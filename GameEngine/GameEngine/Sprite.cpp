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

unsigned int SpriteFrameSet::AddFrame(float delay, RectangleI rect)
{
	frames.push_back(SpriteFrame(delay, rect));
	return frames.size()-1;
}

unsigned int SpriteFrameSet::AddFrame(float delay, int left, int top, int right, int bottom)
{
	frames.push_back(SpriteFrame(delay, left, top, right, bottom));
	return frames.size()-1;
}

void SpriteFrameSet::RemoveFrame(unsigned int index)
{
	frames.erase(frames.begin() + index);
}

unsigned int SpriteFrameSet::GetFrameCount()
{
	return frames.size();
}

bool SpriteFrameSet::LoadFromFile(ResourceManager* mgr)
{
	Variant str, str2;
	bool autogen = false;
	bool good = false;
	SpriteFrame frame;
	ZipFile fs;
	if(!fs.Open(GetFileName())) return false;
	str << fs;
	unsigned int ssz = 0;
	fs.Read(autogen, &ssz);
	mgr->GetResource(str, RES_TYPE_IMAGE, &image);
	
	while(1)
	{
		if(!fs.Read(frame.delay, &ssz)) break;;
		ssz = sizeof(RectangleI);
		if(!fs.Read((void*)&frame.rect, &ssz)) break;
		good = true;
		if(frame.delay == 0 && frame.rect.left == 0 && frame.rect.right == 0 && frame.rect.top == 0 && frame.rect.bottom == 0)
		{
			break;
		}
		if(autogen)
		{
			AutoGenerateGrid(frame.delay, Size(frame.rect.right, frame.rect.bottom));
		}
		else
		{
			frames.push_back(frame);
		}
	}


	while(1)
	{
		Variant str3;
		//str2.ClearString();
		//str2 << fs;
		if(!str3.Read(&fs)) break;
		if(str3.GetType()!=Variant::String) break;

		SpriteData d;
		d.frame.clear();
		GetStringArray(d.frame, (const char*)str3, ',');
		data.push_back(d);
		/*stringstream s(str4);
		SpriteData d;
		string tmp;
		int i;
		while(getline(s, tmp, ','))
		{
			i = atoi(tmp.c_str());
			d.frame.push_back(i);
		}
		data.push_back(d);*/		
	}

	/*SpriteData d;
	d.frame.push_back(0);
	data.push_back(d);*/

	fs.Close();
	return good;
}

SpriteData* SpriteFrameSet::GetSpriteData(unsigned int i)
{
	return &data[i];
}

void SpriteFrameSet::AddSpriteData(SpriteData& d)
{
	data.push_back(d);
}

void SpriteFrameSet::RemoveSpriteData(unsigned int i)
{
	data.erase(data.begin()+i);
}

void SpriteFrameSet::Release()
{
	data.clear();
	frames.clear();
	image = Resource();
}

void* SpriteFrameSet::GetResourcePtr()
{
	return this;
}

IResource* SpriteFrameSet::Clone()
{
	return new SpriteFrameSet();
}

void SpriteFrameSet::RegisterLoader(ResourceManager* mgr)
{
	mgr->RegisterLoader(RES_TYPE_IMAGESET, new SpriteFrameSet());
}

unsigned int SpriteFrameSet::GetDataCount()
{
	return data.size();
}

void SpriteFrameSet::SetImage(Resource img)
{
	image = img;
}

Resource SpriteFrameSet::GetImage()
{
	return image;
}

SpriteFrame* SpriteFrameSet::GetSpriteFrame(unsigned int index)
{
	if(index>=frames.size()) return 0;
	return &frames[index];
}

void SpriteFrameSet::AutoGenerateGrid(float delay, Size blocksize)
{
	if(delay==0 ||  blocksize.width == 0 ||  blocksize.height == 0) return;
	frames.clear();
	Size size = image.GetPtr<ImageResource>()->GetImageSize();
	int tiledx = size.width / blocksize.width;
	int tiledy = size.height / blocksize.height;
	for(int i = 0; i<tiledy; i++)
	{
		for(int f = 0; f<tiledx; f++)
		{
			frames.push_back(SpriteFrame(delay, f*blocksize.width, i*blocksize.height, (f+1)*blocksize.width, (i+1)*blocksize.height));
		}
	}
}

// Sprite
Sprite::Sprite()
{
	index = 0;
	offset = 0;
	pause = true;
	animtype = Forward;
	data = 0;
	img = 0;
	frames = 0;
	direction = 0;
	tdelay = 0;
	m_drawbolt = false;
	mirrorx = false;
	mirrory = false;
	animspeed = 1.0f;

	anglesprite = false;
	resetangle = false;
	startind = 0;
	endind = 10;
	recmessages = false;
	parent = 0;
}

void Sprite::PostFinishedMessage()
{
	if(recmessages && parent!=0)
	{
		parent->SpriteAnimationFinished();
	}
}

void Sprite::Reset()
{
	offset = 0;
	tdelay = 0;
	pause = true;
	PostFinishedMessage();
}

void Sprite::Resume()
{
	pause = false;
}

void Sprite::Pause()
{
	pause = true;
}

void Sprite::SetIndex(unsigned int i)
{
	if(frames!=0 && i < frames->GetDataCount())
	{
		index = i;
		data = frames->GetSpriteData(index);
		Reset();
	}
	else 
	{
		Pause();
		index = i;
		data = 0;
		unsigned int off = offset;
		Reset();
		SetOffset(off);
	}
}

unsigned  int Sprite::GetIndex() const
{
	return index;
}

void Sprite::SetAnimationType(Sprite::AnimationType type)
{
	if(data==0 || frames==0) return;
	animtype = type;
	if(type == Forward)
	{
		direction = 0;
		offset = 0;
	}
	else if(type == Backward)
	{
		direction = 1;
		if(data!=0)
		this->offset = data->frame.size() - 1;
		if(this->offset < 0) this->offset = 0;
	}
	else if(type == Loop) 
	{
		direction = 0;
		offset = 0;
	}
}

Sprite::AnimationType Sprite::GetAnimationType() const
{
	return animtype;
}

void Sprite::SetOffset(unsigned int offset)
{
	if(data!=0 && data->frame.size() > offset)
	this->offset = offset;
	else 
	{
		Pause();
		this->offset = offset;
		data = 0;
	}
}

unsigned int Sprite::GetOffset() const
{
	return offset;
}

void Sprite::SetImageSet(Resource img)
{
	if(img.IsValid())
	{
		if(img->GetType()==RES_TYPE_IMAGESET)
		{
			image = img;
			this->frames = image.GetPtr<SpriteFrameSet>();
			if( frames != 0) this->img = frames->GetImage().GetPtr<ImageResource>();		
			SetIndex(0);
		}
		else if(img->GetType()==RES_TYPE_IMAGE)
		{
			image = img;
			this->img = image.GetPtr<ImageResource>();
			frames = 0;
			SetIndex(0);
			offset = this->img->GetImageSize().width;
		}
		else
		{
			image = 0;	
			this->frames = 0;
			this->img = 0;		
			SetIndex(0);
		}
	}
	else
	{
		image = 0;	
		this->frames = 0;
		this->img = 0;		
		SetIndex(0);
	}
}

Resource Sprite::GetImageSet() const
{
	return image;
}

void Sprite::DrawArea(C_Renderer* r, const RectangleF& srcr, const RectangleF& dest, unsigned int color, float angle, float layer)
{
	if(data!=0 && frames!=0 && img!=0 && img->GetResourcePtr()!=0)
	{
		if(anglesprite)
		{
			unsigned int ind = (unsigned int)( startind + (endind - startind) * (angle / (C_PI*2)) );
			if(ind!=index)
			{
				SetIndex(ind);
				if(!(data!=0 && frames!=0)) return;
			}
			if(resetangle) angle = 0;
		}

		RectangleI src = frames->GetSpriteFrame(data->frame[offset])->rect;
		int iwidth = src.right - src.left;
		int iheight = src.bottom - src.top;
		int ileft = src.left;
		int itop = src.top;

		src.left = (int)(ileft + (iwidth) * srcr.left);
		src.top = (int)(itop + (iheight) * srcr.left);
		src.right = (int)(ileft + (iwidth) * srcr.right);
		src.bottom = (int)(itop + (iheight) * srcr.bottom);

		if(mirrorx)
		{
			int tmp = src.left;
			src.left = src.right;
			src.right = tmp;
		}
		if(mirrory)
		{
			int tmp = src.top;
			src.top = src.bottom;
			src.bottom = tmp;
		}

		if(m_drawbolt)
		{
			RectangleF rc = dest;
			PointF translate;
			translate.left = (rc.right - rc.left) / 2.0f;
			translate.top = 0;			
			r->DrawImageTranslated(img, translate, src, dest, color, angle, layer);
		}
		else r->DrawImage(img, src, dest, color, angle, layer); 
	}
	else if(img!=0 && img->GetResourcePtr()!=0)
	{
		if(anglesprite)
		{
			unsigned int ind = (unsigned int)( startind + (endind - startind) * (angle / (C_PI*2)) );
			if(ind!=index)
			{
				SetIndex(ind);
				if(!(data!=0 && frames!=0)) return;
			}
			if(resetangle) angle = 0;
		}

		Size sz = img->GetImageSize();
		unsigned int imgrc = 0;
		unsigned int px = 0;
		unsigned int py = 0;

		if(offset!=0)
		{
			imgrc = sz.width / offset;
			sz.width = offset;
			sz.height = offset;

			py = (unsigned int)floor(index / (float)imgrc);
			px = index % imgrc;
			py *= offset;
			px *= offset;
		}

		RectangleI src(px, py, px + sz.width, py + sz.height);

		int iwidth = src.right - src.left;
		int iheight = src.bottom - src.top;
		int ileft = src.left;
		int itop = src.top;

		src.left = (int)(ileft + (iwidth) * srcr.left);
		src.top = (int)(itop + (iheight) * srcr.left);
		src.right = (int)(ileft + (iwidth) * srcr.right);
		src.bottom = (int)(itop + (iheight) * srcr.bottom);

		if(mirrorx)
		{
			int tmp = src.left;
			src.left = src.right;
			src.right = tmp;
		}
		if(mirrory)
		{
			int tmp = src.top;
			src.top = src.bottom;
			src.bottom = tmp;
		}

		r->DrawImage(img, src, dest, color, angle, layer); 
	}
}

void Sprite::Draw(C_Renderer* r, const RectangleF& dest, unsigned int color, float angle, float layer)
{
	if(data!=0 && frames!=0 && img!=0 && img->GetResourcePtr()!=0)
	{
		if(anglesprite)
		{
			unsigned int ind = (unsigned int)( startind + (endind - startind) * (angle / (C_PI*2)) );
			if(ind!=index)
			{
				SetIndex(ind);
				if(!(data!=0 && frames!=0)) return;
			}
			if(resetangle) angle = 0;
		}

		RectangleI src = frames->GetSpriteFrame(data->frame[offset])->rect;
		if(mirrorx)
		{
			int tmp = src.left;
			src.left = src.right;
			src.right = tmp;
		}
		if(mirrory)
		{
			int tmp = src.top;
			src.top = src.bottom;
			src.bottom = tmp;
		}

		if(m_drawbolt)
		{
			RectangleF rc = dest;
			PointF translate;
			translate.left = (rc.right - rc.left) / 2.0f;
			translate.top = 0;			
			r->DrawImageTranslated(img, translate, src, dest, color, angle, layer);
		}
		else r->DrawImage(img, src, dest, color, angle, layer); 
	}
	else if(img!=0 && img->GetResourcePtr()!=0)
	{
		if(anglesprite)
		{
			unsigned int ind = (unsigned int)( startind + (endind - startind) * (angle / (C_PI*2)) );
			if(ind!=index)
			{
				SetIndex(ind);
				if(!(data!=0 && frames!=0)) return;
			}
			if(resetangle) angle = 0;
		}

		Size sz = img->GetImageSize();
		unsigned int imgrc = 0;
		unsigned int px = 0;
		unsigned int py = 0;

		if(offset!=0)
		{
			imgrc = sz.width / offset;
			sz.width = offset;
			sz.height = offset;

			py = (unsigned int)floor(index / (float)imgrc);
			px = index % imgrc;
			py *= offset;
			px *= offset;
		}

		RectangleI src(px, py, px + sz.width, py + sz.height);
		if(mirrorx)
		{
			int tmp = src.left;
			src.left = src.right;
			src.right = tmp;
		}
		if(mirrory)
		{
			int tmp = src.top;
			src.top = src.bottom;
			src.bottom = tmp;
		}
		r->DrawImage(img, src, dest, color, angle, layer); 
	}
}

void Sprite::DrawTransformed(C_Renderer* r, const PointF& transform, const RectangleF& dest, unsigned int color, float angle, float layer)
{
	if(data!=0 && frames!=0 && img!=0 && img->GetResourcePtr()!=0)
	{
		if(anglesprite)
		{
			unsigned int ind = (unsigned int)( startind + (endind - startind) * (angle / (C_PI*2)) );
			if(ind!=index)
			{
				SetIndex(ind);
				if(!(data!=0 && frames!=0)) return;
			}
			if(resetangle) angle = 0;
		}

		RectangleI src = frames->GetSpriteFrame(data->frame[offset])->rect;
		if(mirrorx)
		{
			int tmp = src.left;
			src.left = src.right;
			src.right = tmp;
		}
		if(mirrory)
		{
			int tmp = src.top;
			src.top = src.bottom;
			src.bottom = tmp;
		}
		r->DrawImageTranslated(img, transform, src, dest, color, angle, layer);
	}
	else if(img!=0 && img->GetResourcePtr()!=0)
	{
		if(anglesprite)
		{
			unsigned int ind = (unsigned int)( startind + (endind - startind) * (angle / (C_PI*2)) );
			if(ind!=index)
			{
				SetIndex(ind);
				if(!(data!=0 && frames!=0)) return;
			}
			if(resetangle) angle = 0;
		}

		Size sz = img->GetImageSize();
		unsigned int imgrc = 0;
		unsigned int px = 0;
		unsigned int py = 0;

		if(offset!=0)
		{
			imgrc = sz.width / offset;
			sz.width = offset;
			sz.height = offset;

			py = (unsigned int)floor(index / (float)imgrc);
			px = index % imgrc;
			py *= offset;
			px *= offset;
		}

		RectangleI src(px, py, px + sz.width, py + sz.height);
		if(mirrorx)
		{
			int tmp = src.left;
			src.left = src.right;
			src.right = tmp;
		}
		if(mirrory)
		{
			int tmp = src.top;
			src.top = src.bottom;
			src.bottom = tmp;
		}

		r->DrawImageTranslated(img, transform, src, dest, color, angle, layer); 
	}
}

void Sprite::Next()
{
	if(offset == (data->frame.size() - 1))
	{
		if(animtype == PingPong)
		{
			direction = 1;
		}
		else if(animtype == Loop)
		{
			offset = 0;
			return;
		}
		else if(animtype == Forward)
		{
			tdelay = 0;
			pause = true;
			Fnished();
			PostFinishedMessage();
			return;
		}
	}
	else if(offset == 0)
	{
		if(animtype == PingPong)
		{
			direction = 0;
		}
		else if(animtype == Backward)
		{
			tdelay = 0;
			pause = true;
			Fnished();
			PostFinishedMessage();
			return;
		}
	}

	if(direction == 0)
	{
		++offset;
	}
	else
	{
		--offset;
		if(offset == -1) offset = 0;
	}
}

void Sprite::Update(float delta)
{
	delta *= animspeed;
	if(!pause)  tdelay += delta;
	while(tdelay>0)
	{
		if(pause) break;
		if(data==0 || frames==0)
		{
			tdelay = 0;
			Pause();
			Fnished();
			PostFinishedMessage();
			break;
		}
		if(data->frame.size() <= offset)
		{
			tdelay = 0;
			Pause();
			Fnished();
			PostFinishedMessage();
			break;
		}
		float delay = frames->GetSpriteFrame(data->frame[offset])->delay;
		if(delay == 0) break;

		if(tdelay - delay < 0)
		{
			break;
		}

		tdelay -= delay;
		Next();	
		Step();
	}
}

void Sprite::OnLost()
{
}

void* Sprite::GetResourcePtr()
{
	return this;
}

RectangleI* Sprite::GetSpriteRect(unsigned int index)
{
	if(frames==0 || frames->GetFrameCount() <= index) return 0;
	return &(frames->GetSpriteFrame(index)->rect);	
}

bool Sprite::LoadFromFile( ResourceManager* mgr )
{
	vector<wstring> lst;
	wstring str2 = GetFileName();
	wstringstream s(str2);
	s << str2;
	SpriteData d;
	wstring tmp;

	while(getline(s, tmp, L','))
	{
		lst.push_back(tmp);
	}
	
	if(lst.size()<4) return false;

	char t[250];
	WideCharToMultiByte(t, lst[0].c_str(), 250);
	Resource rc;
	mgr->GetResource(t, RES_TYPE_IMAGESET, &rc);
	SetImageSet(rc);

	wchar_t* stop;
	AnimationType t_animtype;
	unsigned int t_index = 0;
	unsigned int t_offset = 0;
	t_index = wcstol(lst[1].c_str(), &stop, 10);
	t_offset = wcstol(lst[2].c_str(), &stop, 10);
	t_animtype = (AnimationType)wcstol(lst[3].c_str(), &stop, 10);
	SetOffset(t_offset);
	SetIndex(t_index);	
	SetAnimationType(t_animtype);
	if(lst.size()>4)
	{
		long b = 0;
		wcstol(lst[4].c_str(), &stop, 10);
		if(b==0)
		{
			Reset();
			Pause();
			return true;
		}
	}
	Resume();
	return true;
}

void Sprite::Save( BaseIO& str, const char* name )
{
	string n = name;
	n += " imageset;help=Sprite imageset resource Name.;type=spriteresource;default=6";
	str.Write(&image, 0, n.c_str());
	AnimationType t_animtype = animtype;
	unsigned int t_index = index;
	unsigned int t_offset = offset;
	n = name;
	n += " Animation index;help=Index number of the animation for sprite.";
	str.Write(t_index, 0, n.c_str());
	n = name;
	n += " Offset;help=Offset of the frame for currect animation index.";
	str.Write(t_offset, 0, n.c_str());
	n = name;
	n += " Animation type;help=animation type.\n (values : Forward - 0, Backward - 1, Loop - 2, PingPong - 3);type=list;list=Forward,Backward,Loop,PingPong";
	str.Write((int)t_animtype, 0, n.c_str());
	n = name;
	n += " Pause Animation;help=Pause animation.";
	str.Write(pause, 0, n.c_str());

	n = name;
	n += " Draw Bolt;help=Draw Bolt.";
	str.Write(m_drawbolt, 0, n.c_str());

	n = name;
	n += " MirrorX;help=MirrorX.";
	str.Write(mirrorx, 0, n.c_str());

	n = name;
	n += " MirrorY;help=MirrorY.";
	str.Write(mirrory, 0, n.c_str());

	n = name;
	n += " XSpeed;help=Animation speed modifier.";
	str.Write(animspeed, 0, n.c_str());

	n = name;
	n += " Angle Sprite";
	str.Write(anglesprite, 0, n.c_str());

	n = name;
	n += " Reset Angle";
	str.Write(resetangle, 0, n.c_str());

	n = name;
	n += " StartIndex";
	str.Write(startind, 0, n.c_str());

	n = name;
	n += " EndIndex";
	str.Write(endind, 0, n.c_str());

	n = name;
	n += " RecMessage";
	str.Write(recmessages, 0, n.c_str());
}

const Sprite& Sprite::Copy(const Sprite& val)
{
	SetImageSet(val.GetImageSet());
	bool p = val.pause;
	m_drawbolt = val.m_drawbolt;
	SetIndex(val.GetIndex());
	SetOffset(val.GetOffset());
	SetAnimationType(val.GetAnimationType());
	mirrorx = val.mirrorx;
	mirrory = val.mirrory;
	animspeed = val.animspeed;
	Reset();
	if(p) Pause();
	else Resume();

	anglesprite = val.anglesprite;
	startind = val.startind;
	endind = val.endind;
	resetangle = val.resetangle;

	recmessages = val.recmessages;

	return *this;
}

Sprite::Sprite(const Sprite& val)
{
	Copy(val);
}

const Sprite& Sprite::operator=(const Sprite& val)
{
	return Copy(val);
}

void Sprite::Load( BaseIO& str )
{
	Resource rc;
	str.Read(&rc);
	SetImageSet(rc);

	int t_animtype;
	unsigned int t_index = 0;
	unsigned int t_offset = 0;
	str.Read(t_index);
	str.Read(t_offset);
	str.Read(t_animtype);
	bool p = true;
	str.Read(p);
	str.Read(m_drawbolt);

	str.Read(mirrorx);
	str.Read(mirrory);
	str.Read(animspeed);

	SetIndex(t_index);
	SetOffset(t_offset);
	SetAnimationType((AnimationType)t_animtype);
	tdelay = 0;
	pause = true;
	if(p) Pause();
	else Resume();

	str.Read(anglesprite);
	str.Read(resetangle);
	str.Read(startind);
	str.Read(endind);
	str.Read(recmessages);
}

IResource* Sprite::Clone()
{
	return new Sprite();
}

void Sprite::OnReset()
{
}

void Sprite::Release()
{
}

void Sprite::RegisterLoader( ResourceManager* mgr )
{
	mgr->RegisterLoader(RES_TYPE_SPRITE, new Sprite());
}

//Angle Sprite
void Sprite::SetAngleSprite(bool val)
{
	this->anglesprite = val;
}

bool Sprite::GetAngleSprite()
{
	return anglesprite;
}

void Sprite::SetIndexRange(unsigned int start, unsigned int end)
{
	startind = start;
	endind = end;
}

unsigned int Sprite::GetStartIndex()
{
	return startind;
}

unsigned int Sprite::GetEndIndex()
{
	return endind;
}