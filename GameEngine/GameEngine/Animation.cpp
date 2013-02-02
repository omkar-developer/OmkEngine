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

AnimationKey::AnimationKey()
{
	frame = 0;
}

AnimationKey::AnimationKey(unsigned int frame)
{
	this->frame = frame;
}

void AnimationKey::Save( BaseIO& str )
{
	val.Write(&str, true);
	str.Write(frame, 0, 0, true);
}

void AnimationKey::Load( BaseIO& str )
{
	val.Read(&str, true);
	str.Read(frame, 0, true);
}

// ObjectAnimation
void ObjectAnimations::AddKey(int type, Variant val, unsigned int frame)
{
	AnimationKey key;
	key.frame = frame;
	key.val = val;
	animations[type].insert(key);
}

void ObjectAnimations::DeleteKey(unsigned int frame)
{
	AnimationKey key;
	key.frame = frame;

	for(map<int, set<AnimationKey> >::iterator i = animations.begin(); i!=animations.end(); i++)
	{
		i->second.erase(key);
	}
}

void ObjectAnimations::MoveToFrame(unsigned int frame)
{
	Variant sval;
	Variant eval;
	unsigned int sframe, eframe;

	for(map<int, set<AnimationKey> >::iterator i = animations.begin(); i!=animations.end(); i++)
	{
		if(i->second.size()==0) continue;
		set<AnimationKey>::iterator end, start;
		end = i->second.upper_bound(frame);
		start = end;
		--start;
		
		if(end==i->second.end())
		{
			sval = start->val;
			sframe = start->frame;
			eval = start->val;
			eframe = start->frame;
		}
		else
		{
			sval = start->val;
			sframe = start->frame;

			eval = end->val;
			eframe = end->frame;
		}

		unsigned int tframes = eframe - sframe;
		if(tframes==0) tframes = 1;
		float t = (frame - sframe) / (float)tframes;

		if(sval.GetType() == Variant::Int && eval.GetType() == Variant::Int)
		{
			int s = sval;
			int e = eval;
			int c = (int)(s + (e - s) * t);
		}
		else if(sval.GetType() == Variant::Float && eval.GetType() == Variant::Float)
		{
			float s = sval;
			float e = eval;
			float c = s + (e - s) * t;

			if(i->first == ANIM_TYPE_POSX)
			{
				if(!obj.IsValid()) return;
				RectangleF& pos = obj.GetObject()->GetObjectArea();
				float width = pos.right - pos.left;
				pos.left = c;
				pos.right = pos.left + width;
			}
			else if(i->first == ANIM_TYPE_POSY)
			{
				if(!obj.IsValid()) return;
				RectangleF& pos = obj.GetObject()->GetObjectArea();
				float height = pos.bottom - pos.top;
				pos.top = c;
				pos.bottom = pos.top + height;
			}
			else if(i->first == ANIM_TYPE_SCALEX)
			{
				if(!obj.IsValid()) return;
				RectangleF& pos = obj.GetObject()->GetObjectArea();
				pos.bottom = pos.top + c;
			}
			else if(i->first == ANIM_TYPE_SCALEY)
			{
				if(!obj.IsValid()) return;
				RectangleF& pos = obj.GetObject()->GetObjectArea();
				pos.bottom = pos.top + c;
			}
			else if(i->first == ANIM_TYPE_ROTATION)
			{
				if(!obj.IsValid()) return;
				obj.GetObject()->SetAngle(c);
			}
			else if(i->first == ANIM_TYPE_CPOSX)
			{
				Game->GetMainContainer()->GetCamera().m_pos.left = c;
				Game->GetMainContainer()->AdjustCamera();
			}
			else if(i->first == ANIM_TYPE_CPOSY)
			{
				Game->GetMainContainer()->GetCamera().m_pos.top = c;
				Game->GetMainContainer()->AdjustCamera();
			}
			else if(i->first == ANIM_TYPE_CZOOM)
			{
				Game->GetMainContainer()->GetCamera().m_zoom = c;
				Game->GetMainContainer()->AdjustCamera();
			}
		}
		else if(sval.GetType() == Variant::UInt && eval.GetType() == Variant::UInt)
		{
			unsigned int s = sval;
			unsigned int e = eval;
			int c = (unsigned int)(s + (e - s) * t);
			if(i->first == ANIM_TYPE_BCOLOR)
			{
				if(!obj.IsValid()) return;
				ColorARGB32 CA(s), CB(e);
				ColorARGB32 CE((unsigned char)(CA.GetA() + (CB.GetA() - CA.GetA()) * t), (unsigned char)(CA.GetR() + (CB.GetR() - CA.GetR()) * t), (unsigned char)(CA.GetG() + (CB.GetG() - CA.GetG()) * t), (unsigned char)(CA.GetB() + (CB.GetB() - CA.GetB()) * t));
				obj.GetObject()->m_backcolor = CE;
			}
		}
	}
}

void ObjectAnimations::Save( BaseIO& str )
{
	str.Write(obj.GetConnectionID(), 0, 0, true);
	str.Write((unsigned int)animations.size(), 0, 0, true);
	for(map<int, set<AnimationKey> >::iterator i = animations.begin(); i!=animations.end(); i++)
	{
		int k = i->first;
		str.Write(k, 0, 0, true);

		str.Write((unsigned int)i->second.size(), 0, 0, true);
		for(set<AnimationKey>::iterator i2 = i->second.begin(); i2 != i->second.end(); i2++)
		{
			AnimationKey key = (*i2);
			key.Save(str);
		}
	}
}

void ObjectAnimations::Load( BaseIO& str )
{
	unsigned int id = 0;
	str.Read(id, 0, true);
	obj.SetConnectionID(id);
	unsigned int sz = 0;
	str.Read(sz, 0, true);

	for(unsigned int i = 0; i<sz; i++)
	{
		int k = 0;
		str.Read(k, 0, true);

		unsigned int sz2 = 0;
		str.Read(sz2, 0, true);
		for(unsigned int i2 = 0; i2<sz2; i2++)
		{
			AnimationKey key;
			key.Load(str);
			animations[k].insert(key);
		}
	}
}

//Animation Timeline
AnimationTimeline::AnimationTimeline( ResourceManager* mgr, unsigned int ID, RectangleF const& location, Resource sprite, unsigned int backcolor, float rotation, bool visible, unsigned int flags, ObjectsContainer* parent, bool update ) : GUIProgress(mgr, ID, location, sprite, backcolor, rotation, visible, flags, parent, update)
{
	m_type |= OBJECT_TYPE_ANITIMELINE;
	m_flags |= OBJECT_Flag_PTypeSlider;
	paused = true;
	cframe = 0;
	maxframes = 300;
	fps = 20;
	m_markercol = 0xFFFF000F;
	sel = 0;
	animtype = 0;
	SetMax((float)maxframes);
	if(ID != 10)
	{
		if(Game->timeline != 0)
		{
			QueueForDelete();
		}
		else
		{
			Game->timeline = this;
		}
	}
}

AnimationTimeline::~AnimationTimeline()
{
	Game->timeline = 0;
}

void AnimationTimeline::SetMaxFrames(unsigned int frames)
{
	maxframes = frames;
	SetMax((float)maxframes);
}

void AnimationTimeline::Draw(C_Renderer& rnd)
{
	GUIProgress::Draw(rnd);
}

void AnimationTimeline::DrawBoundingBox(C_Renderer& rnd)
{
	GUIProgress::DrawBoundingBox(rnd);
	DrawableObject* sel2 = Game->GetMainContainer()->Selected;
	map<DrawableObject*, ObjectAnimations>::iterator i = objects.end();
	if(sel2!=0)
	{
		 i = objects.find(sel2);
		 if(i==objects.end())
		 {
			 if(sel==0) return;
			 i = objects.find(sel);
			 if(i==objects.end()) return;
		 }
		 else
		 {
			 sel = sel2;
		 }
	}
	else
	{
		if(sel==0) return;
		i = objects.find(sel);
		if(i==objects.end()) return;
	}

	for(map<int, set<AnimationKey> >::iterator f = i->second.animations.begin(); f!=i->second.animations.end(); f++)
	{
		for(set<AnimationKey>::iterator f2 = f->second.begin(); f2 != f->second.end(); f2++)
		{
			RectangleF Pos = GetAbsolutePos();
			Pos.left += m_Region.left;
			Pos.top += m_Region.top;
			Pos.right -= m_Region.right;
			Pos.bottom -= m_Region.bottom;

			float mul = ((float)f2->frame-GetMin())/(GetMax()-GetMin());
			if(m_flags&OBJECT_Flag_PTypeVertical  && m_flags&OBJECT_Flag_PTypeInv)
			{
				Pos.top = (Pos.top + (Pos.bottom - Pos.top) * (mul)) - 2;
				Pos.bottom = Pos.top + 4;
				Pos.left -= m_Region.left;
				Pos.right += m_Region.right;
			}
			else if(m_flags&OBJECT_Flag_PTypeVertical)
			{
				Pos.top = (Pos.top + (Pos.bottom - Pos.top) * (1-mul)) - 2;
				Pos.bottom = Pos.top + 4;
				Pos.left -= m_Region.left;
				Pos.right += m_Region.right;
			}
			else
			{
				Pos.left = (Pos.left + (Pos.right - Pos.left) * mul) - 2;
				Pos.right = Pos.left + 4;
				Pos.top -= m_Region.top;
				Pos.bottom += m_Region.bottom;
			}

			//m_progressback.Draw(&rnd, Pos, m_markercol, m_rotation, GetLayer());
			rnd.DrawLine(PointF(Pos.left, Pos.top), PointF(Pos.left, Pos.bottom), m_markercol);
		}
	}
}

void AnimationTimeline::OnValueChanged()
{
	cframe = GetValue();

	unsigned int frame = (unsigned int)cframe;
	for(map<DrawableObject*, ObjectAnimations>::iterator i = objects.begin(); i!= objects.end(); i++)
	{
		i->second.MoveToFrame(frame);
	}
}

void AnimationTimeline::Save( BaseIO& str )
{
	GUIProgress::Save(str);
	str.Write(paused, 0, "name=Paused;category=Animation Timeline");
	str.Write(cframe, 0, "name=CurrentFrame");
	str.Write(maxframes, 0, "name=Max Frames");
	str.Write(fps, 0, "name=FPS");
	str.Write(animtype, 0, "name=AType;type=list;list=Forward,Backward");
	str.Write(m_markercol, 0, "name=Merker Color;type=color");

	for(map<DrawableObject*, ObjectAnimations>::iterator i = objects.begin(); i!= objects.end();)
	{
		if(!i->second.obj.IsValid())
		{
			objects.erase(i++);
			if(i==objects.end()) break;
		}
		else
		{
			++i;
		}
	}

	str.Write((unsigned int) objects.size(), 0, 0, true);

	for(map<DrawableObject*, ObjectAnimations>::iterator i = objects.begin(); i!= objects.end(); i++)
	{
		i->second.Save(str);
	}
}

void AnimationTimeline::Load( BaseIO& str )
{
	GUIProgress::Load(str);
	str.Read(paused);
	str.Read(cframe);
	str.Read(maxframes);
	str.Read(fps);
	str.Read(animtype);
	str.Read(m_markercol);
	SetMaxFrames(maxframes);

	unsigned int count = 0;
	str.Read(count, 0, true);

	tmpanims.clear();
	for(unsigned int i = 0; i<count; i++)
	{
		ObjectAnimations anims;
		anims.Load(str);
		tmpanims.push_back(anims);
	}
	SetValue(cframe);
}

void AnimationTimeline::PostLoadAll()
{
	for(unsigned int i = 0; i<tmpanims.size(); i++)
	{
		tmpanims[i].obj.UpdateConnection();
		if(tmpanims[i].obj.GetObject()!=0)
		{
			objects[tmpanims[i].obj.GetObject()] = tmpanims[i];
		}
	}
	tmpanims.clear();
}

void AnimationTimeline::Play(int animtype)
{
	this->animtype = animtype;
	paused = false;
}

void AnimationTimeline::Play()
{
	paused = false;
}

void AnimationTimeline::Pause()
{
	paused = true;
}

void AnimationTimeline::Stop()
{
	if(animtype == 0)
	{
		cframe = 0;
	}
	else
	{
		cframe = (float)maxframes - 1;
	}
	paused = true;
}

void AnimationTimeline::Update( float delta )
{
	if(paused) return;
	if(IsEditing()) return;

	if(animtype==0)
	{
		cframe += delta * fps;
		while(cframe>maxframes) cframe -= (float)maxframes;
	}
	else if(animtype==1)
	{
		cframe -= delta * fps;
		while(cframe<0) cframe += (float)maxframes;
	}
	SetValue(cframe);
}

Object* AnimationTimeline::Clone()
{
	AnimationTimeline* obj = new AnimationTimeline();
	*obj = *this;
	return obj;
}

void AnimationTimeline::AddPositionKeys(DrawableObject* obj)
{
	map<DrawableObject*, ObjectAnimations>::iterator i = objects.find(obj);
	if(i==objects.end())
	{
		objects[obj].obj.SetObject(obj);
	}

	objects[obj].AddKey(ANIM_TYPE_POSX, obj->GetObjectArea().left, (unsigned int)cframe);
	objects[obj].AddKey(ANIM_TYPE_POSY, obj->GetObjectArea().top, (unsigned int)cframe);
}

void AnimationTimeline::AddScaleKeys(DrawableObject* obj)
{
	map<DrawableObject*, ObjectAnimations>::iterator i = objects.find(obj);
	if(i==objects.end())
	{
		objects[obj].obj.SetObject(obj);
	}

	objects[obj].AddKey(ANIM_TYPE_SCALEX, obj->GetObjectArea().right - obj->GetObjectArea().left, (unsigned int)cframe);
	objects[obj].AddKey(ANIM_TYPE_SCALEY, obj->GetObjectArea().bottom - obj->GetObjectArea().top, (unsigned int)cframe);
}

void AnimationTimeline::AddBColorKeys(DrawableObject* obj)
{
	map<DrawableObject*, ObjectAnimations>::iterator i = objects.find(obj);
	if(i==objects.end())
	{
		objects[obj].obj.SetObject(obj);
	}

	objects[obj].AddKey(ANIM_TYPE_BCOLOR, obj->m_backcolor, (unsigned int)cframe);
}

void AnimationTimeline::AddRotationKeys(DrawableObject* obj)
{
	map<DrawableObject*, ObjectAnimations>::iterator i = objects.find(obj);
	if(i==objects.end())
	{
		objects[obj].obj.SetObject(obj);
	}

	objects[obj].AddKey(ANIM_TYPE_ROTATION, obj->GetAngle(), (unsigned int)cframe);
}

void AnimationTimeline::AddCPositionKeys(DrawableObject* obj)
{
	DrawableObject* obj2 = 0;
	objects[obj2].obj.SetObject(obj2);
	objects[obj2].AddKey(ANIM_TYPE_CPOSX, Game->GetMainContainer()->GetCamera().m_pos.left, (unsigned int)cframe);
	objects[obj2].AddKey(ANIM_TYPE_CPOSY, Game->GetMainContainer()->GetCamera().m_pos.top, (unsigned int)cframe);
}

void AnimationTimeline::AddCZoomKeys(DrawableObject* obj)
{
	DrawableObject* obj2 = 0;
	objects[obj2].obj.SetObject(obj2);
	objects[obj2].AddKey(ANIM_TYPE_CZOOM, Game->GetMainContainer()->GetCamera().m_zoom, (unsigned int)cframe);
}