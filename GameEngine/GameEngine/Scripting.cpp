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

#ifdef USESCRIPTING

PointI Script::m_clkpoint = PointI();

ScriptRectangle::ScriptRectangle()
{
	ref = 1;
	x = 0;
	y = 0;
	r = 0;
	b = 0;
}

void ScriptRectangle::addref()
{
	ref += 1;
}

void ScriptRectangle::release()
{
	ref -= 1;
	if(ref<=0)
	{
		delete this;
	}
}

ScriptRectangle* reactanglefactory()
{
	return new ScriptRectangle();
}

ScriptRectangle* reactanglefactory2(float x, float y, float r, float b)
{
	ScriptRectangle* rc = new ScriptRectangle();
	rc->x = x;
	rc->y = y;
	rc->r = r;
	rc->b = b;
	return rc;
}

//Point
ScriptPoint::ScriptPoint()
{
	ref = 1;
	x = 0;
	y = 0;
}

void ScriptPoint::addref()
{
	ref += 1;
}

void ScriptPoint::release()
{
	ref -= 1;
	if(ref<=0)
	{
		delete this;
	}
}

ScriptPoint* pointfactory()
{
	return new ScriptPoint();
}

ScriptPoint* pointfactory2(float x, float y)
{
	ScriptPoint* rc = new ScriptPoint();
	rc->x = x;
	rc->y = y;
	return rc;
}
//

ScriptObject::ScriptObject()
{
	ref = 1;
	obj = 0;
}

void ScriptObject::addref()
{
	ref += 1;
}

void ScriptObject::release()
{
	ref -= 1;
	if(ref<=0)
	{
		delete this;
	}
}

ScriptObject* ScriptObject::operator=(ScriptObject* o)
{
	obj = o->obj;
	o->release();
	return this;
}

unsigned int ScriptObject::GetID()
{
	if(obj!=0)
	{
		return obj->GetID();
	}
	return 0; 
}

unsigned int ScriptObject::GetObjectType()
{
	if(obj!=0)
	{
		return obj->m_objtype;
	}
	return 0;
}

void ScriptObject::SetObjectType(unsigned int val)
{
	if(obj!=0)
	{
		obj->m_objtype = val;
	}
}

string ScriptObject::GetObjectName()
{
	if(obj!=0)
	{
		return obj->m_objname;
	}
	return 0;
}

void ScriptObject::SetObjectName(string name)
{
	if(obj!=0)
	{
		obj->m_objname = name;
	}
}

void ScriptObject::SetProperty(string name, float val)
{
	if(obj!=0)
	{
		obj->SetProperty(name.c_str(), val);
	}
}

float ScriptObject::GetProperty(string name)
{
	if(obj!=0)
	{
		return obj->GetProperty(name.c_str());
	}
	return 0;
}

void ScriptObject::ClearProperties()
{
	if(obj!=0)
	{
		obj->ClearProperties();
	}
}

void ScriptObject::Show()
{
	if(obj==0) return;
	obj->m_flags &= ~OBJECT_Flag_Invisible;
}

void ScriptObject::Hide()
{
	if(obj==0) return;
	obj->m_flags |= OBJECT_Flag_Invisible;
}

void ScriptObject::Enable()
{
	if(obj==0) return;
	obj->m_flags &= ~OBJECT_Flag_Disabled;
}

void ScriptObject::Disable()
{
	if(obj==0) return;
	obj->m_flags |= OBJECT_Flag_Disabled;
}

void ScriptObject::SetText(string text)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_TEXT)) return;
	((GUIText*)obj)->m_Text = text.c_str();
}

string ScriptObject::GetText()
{
	if(obj==0) return "";
	if(!(obj->GetType()&OBJECT_TYPE_TEXT)) return "";
	string str;
	wstring str2 = (wstring)(((GUIText*)obj)->m_Text);
	for(unsigned int i = 0; i<str2.size(); i++)
	{
		str += (char)str2[i];
	}

	return str;
}

void ScriptObject::SetColor(unsigned int color)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_DRAWABLE)) return;
	((DrawableObject*)obj)->SetBackColor(color);
}

unsigned int ScriptObject::GetColor()
{
	if(obj==0) return 0;
	if(!(obj->GetType()&OBJECT_TYPE_DRAWABLE)) return 0;
	return ((DrawableObject*)obj)->GetBackColor();
}

void ScriptObject::SetRotation(float val)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_DRAWABLE)) return;
	((DrawableObject*)obj)->SetAngle(val);
}

float ScriptObject::GetRotation()
{
	if(obj==0) return 0;
	if(!(obj->GetType()&OBJECT_TYPE_DRAWABLE)) return 0;
	return ((DrawableObject*)obj)->GetAngle();
}

void ScriptObject::SetCapture()
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_DRAWABLE)) return;
	((DrawableObject*)obj)->SetCapture();
	if(obj->GetType()&OBJECT_TYPE_CONTAINER) ((ObjectsContainer*)obj)->HoldCapture = true;
}

bool ScriptObject::GetCapture()
{
	if(obj==0) return false;
	if(!(obj->GetType()&OBJECT_TYPE_DRAWABLE)) return false;
	return ((DrawableObject*)obj)->IsCaptured();
}

void ScriptObject::ReleaseCapture()
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_DRAWABLE)) return;
	if(obj->GetType()&OBJECT_TYPE_CONTAINER) ((ObjectsContainer*)obj)->HoldCapture = false;
	((DrawableObject*)obj)->ReleaseCapture();
}

void ScriptObject::SetValue(int text)
{
	if(obj==0) return;
	if(obj->GetType()&OBJECT_TYPE_PROGRESS)
	{
		((GUIProgress*)obj)->SetValue((float)text);
	}
	else if(obj->GetType()&OBJECT_TYPE_PROGRESS)
	{
		if(text==0) ((GUIButton*)obj)->SetChecked(false);
		else ((GUIButton*)obj)->SetChecked(true);
	}	
}

int ScriptObject::GetValue()
{
	if(obj==0) return 0;
	if(obj->GetType()&OBJECT_TYPE_PROGRESS)
	{
		return (int)((GUIProgress*)obj)->GetValue();
	}
	else if(obj->GetType()&OBJECT_TYPE_BUTTON)
	{
		if(((GUIButton*)obj)->GetChecked()) return 1;
		else return 0;
	}
	return 0;
}

void ScriptObject::SetMax(float val)
{
	if(obj==0) return;
	if(obj->GetType()&OBJECT_TYPE_PROGRESS)
	{
		((GUIProgress*)obj)->SetMax(val);
	}
}

float ScriptObject::GetMax()
{
	if(obj==0) return 0;
	if(obj->GetType()&OBJECT_TYPE_PROGRESS)
	{
		return ((GUIProgress*)obj)->GetMax();
	}
	return 0;
}

void ScriptObject::SetMin(float val)
{
	if(obj==0) return;
	if(obj->GetType()&OBJECT_TYPE_PROGRESS)
	{
		((GUIProgress*)obj)->SetMin(val);
	}
}

float ScriptObject::GetMin()
{
	if(obj==0) return 0;
	if(obj->GetType()&OBJECT_TYPE_PROGRESS)
	{
		return ((GUIProgress*)obj)->GetMin();
	}
	return 0;
}

void ScriptObject::PlaySprite()
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_DRAWABLE)) return;
	((DrawableObject*)obj)->GetSprite().Resume();
}

void ScriptObject::SetSpriteIndex(unsigned int ind)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_DRAWABLE)) return;
	((DrawableObject*)obj)->GetSprite().SetIndex(ind);
}

void ScriptObject::MirrorSpriteX(bool val)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_DRAWABLE)) return;
	((DrawableObject*)obj)->GetSprite().mirrorx = val;
}

void ScriptObject::MirrorSpriteY(bool val)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_DRAWABLE)) return;
	((DrawableObject*)obj)->GetSprite().mirrory = val;
}

void ScriptObject::PauseSprite()
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_DRAWABLE)) return;
	((DrawableObject*)obj)->GetSprite().Pause();
}

void ScriptObject::StopSprite()
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_DRAWABLE)) return;
	((DrawableObject*)obj)->GetSprite().Reset();
}

void ScriptObject::Play(int animtype)
{
	if(obj==0) return;
	if((obj->GetType()&OBJECT_TYPE_ANITIMELINE))
	{
		((AnimationTimeline*)obj)->Play(animtype);
	}
	else if((obj->GetType()&OBJECT_TYPE_SOUNDPLAYER))
	{
		((SoundPlayer*)obj)->Start(animtype?true:false);
	}
}

void ScriptObject::Play1()
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_ANITIMELINE)) return;
	((AnimationTimeline*)obj)->Play();
}

void ScriptObject::Pause()
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_ANITIMELINE)) return;
	((AnimationTimeline*)obj)->Pause();
}

void ScriptObject::Stop()
{
	if(obj==0) return;
	if((obj->GetType()&OBJECT_TYPE_ANITIMELINE))
	{
		((AnimationTimeline*)obj)->Stop();
	}
	else if((obj->GetType()&OBJECT_TYPE_SOUNDPLAYER))
	{
		((SoundPlayer*)obj)->Stop();
	}
}

bool ScriptObject::isPlaying()
{
	if(obj==0) return false;
	if(!(obj->GetType()&OBJECT_TYPE_SOUNDPLAYER)) return false;
	return ((SoundPlayer*)obj)->isPlaying();
}

void ScriptObject::SetSoundResource(string name)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_SOUNDPLAYER)) return;
	((SoundPlayer*)obj)->SetResource(name.c_str());
}

void ScriptObject::SetVolume(int val)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_SOUNDPLAYER)) return;
	((SoundPlayer*)obj)->SetVolume(val);
}

int ScriptObject::GetVolume()
{
	if(obj==0) return 0;
	if(!(obj->GetType()&OBJECT_TYPE_SOUNDPLAYER)) return 0;
	return ((SoundPlayer*)obj)->GetVolume();
}

int ScriptObject::GetType()
{
	if(obj==0) return 0;
	return obj->GetType();
}

void ScriptObject::QueueForDelete()
{
	if(obj==0) return;
	return obj->QueueForDelete();
}

void ScriptObject::Draw()
{
	if(obj==0) return;
	obj->Draw(*Game->GetRenderer());
}

unsigned int ScriptObject::GetFlags()
{
	if(obj==0) return 0;
	return obj->m_flags;
}

void ScriptObject::SetFlags(unsigned int val)
{
	if(obj==0) return;
	obj->m_flags = val;
}

ScriptRectangle* ScriptObject::GetObjectArea()
{
	ScriptRectangle* rct = new ScriptRectangle();
	if(obj==0) return rct;
	if(!(obj->GetType()&OBJECT_TYPE_DRAWABLE)) return rct;
	RectangleF rc = ((DrawableObject*)obj)->GetObjectArea();
	rct->x = rc.left;
	rct->y = rc.top;
	rct->r = rc.right;
	rct->b = rc.bottom;
	return rct;
}

ScriptRectangle* ScriptObject::GetAbsolutePosition()
{
	ScriptRectangle* rct = new ScriptRectangle();
	if(obj==0) return rct;
	if(!(obj->GetType()&OBJECT_TYPE_DRAWABLE)) return rct;
	RectangleF rc = ((DrawableObject*)obj)->GetAbsolutePos();
	rct->x = rc.left;
	rct->y = rc.top;
	rct->r = rc.right;
	rct->b = rc.bottom;
	return rct;
}

void ScriptObject::SetObjectArea(ScriptRectangle* val)
{
	RectangleF rc;
	rc.left = val->x;
	rc.top = val->y;
	rc.right = val->r;
	rc.bottom = val->b;
	val->release();
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_DRAWABLE)) return;	
	((DrawableObject*)obj)->SetObjectArea(rc);
}

void ScriptObject::SetSprite(string name, int index, int animtype, bool paused)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_DRAWABLE)) return;
	Resource res;
	Game->GetResourceManager()->GetResource(name.c_str(), RES_TYPE_IMAGESET, &res);
	Sprite spr;
	spr.SetImageSet(res);
	spr.SetIndex(index);
	spr.SetAnimationType((Sprite::AnimationType)animtype);
	spr.Reset();
	if(paused) spr.Pause();
	else spr.Resume();
	((DrawableObject*)obj)->SetSprite(spr);
}

void ScriptObject::SetLayer(float val)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_DRAWABLE)) return;
	((DrawableObject*)obj)->SetLayer(val);
}

float ScriptObject::GetLayer()
{
	if(obj==0) return 0;
	if(!(obj->GetType()&OBJECT_TYPE_DRAWABLE)) return 0.0f;
	return ((DrawableObject*)obj)->GetLayer();
}

void ScriptObject::StartTimer()
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_TIMER)) return;
	((TimerObject*)obj)->StartTimer();
}

void ScriptObject::PauseTimer()
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_TIMER)) return;
	((TimerObject*)obj)->PauseTimer();
}

void ScriptObject::StopTimer()
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_TIMER)) return;
	((TimerObject*)obj)->StopTimer();
}

void ScriptObject::EmitParticles1(ScriptPoint* pt)
{
	PointF pt2(pt->x, pt->y);
	pt->release();
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_PARTICLEEN)) return;
	((ParticleEngine*)obj)->EmitParticles(pt2.left, pt2.top);
}

void ScriptObject::EmitParticles2(float x, float y, float size)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_PARTICLEEN)) return;
	((ParticleEngine*)obj)->EmitParticles(x, y, size);
}

void ScriptObject::AddEmitRect(ScriptRectangle* pt, float life)
{
	RectangleF rct(pt->x, pt->y, pt->r, pt->b);
	pt->release();
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_PARTICLEEN)) return;
	((ParticleEngine*)obj)->AddEmitRect(rct, life);
}

void ScriptObject::AddEmitRect2(float x, float y, float life, float size)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_PARTICLEEN)) return;
	((ParticleEngine*)obj)->AddEmitRect(x, y, life, size);
}

void ScriptObject::ClearEmitRect()
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_PARTICLEEN)) return;
	((ParticleEngine*)obj)->ClearEmitRect();
}

void ScriptObject::SetDelay(float delay)
{
	if(obj==0) return;
	if(obj->GetType()&OBJECT_TYPE_ANIMABLE)
	{
		((AnimableObject*)obj)->m_delay  = delay;
	}
#ifdef USEBOX2DPHYSICS
	else if(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)
	{
		((PhysicsObject*)obj)->m_delay  = delay;
	}  
#endif // USEBOX2DPHYSICS

}

void ScriptObject::SetSpeed(float speed)
{
	if(obj==0) return;
	if(obj->GetType()&OBJECT_TYPE_ANIMABLE)
	{
		((AnimableObject*)obj)->m_speed  = speed;
	}
#ifdef USEBOX2DPHYSICS
	else if(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)
	{
		((PhysicsObject*)obj)->m_speed  = speed;
	}  
#endif // USEBOX2DPHYSICS

}

void ScriptObject::SetAnimatiomType(int type)
{
	if(obj==0) return;
	if(obj->GetType()&OBJECT_TYPE_ANIMABLE)
	{
		((AnimableObject*)obj)->m_animtype = type;
		((AnimableObject*)obj)->Reset();
	}
#ifdef USEBOX2DPHYSICS
	else if(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)
	{
		((PhysicsObject*)obj)->m_animtype = type;
		((PhysicsObject*)obj)->Reset();
	}  
#endif // USEBOX2DPHYSICS

}

void ScriptObject::SetPlayType(int type)
{
	if(obj==0) return;
	if(obj->GetType()&OBJECT_TYPE_ANIMABLE)
	{
		((AnimableObject*)obj)->m_playtype = type;
		((AnimableObject*)obj)->Reset();
	}
#ifdef USEBOX2DPHYSICS
	else if(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)
	{
		((PhysicsObject*)obj)->m_playtype = type;
		((PhysicsObject*)obj)->Reset();
	}  
#endif // USEBOX2DPHYSICS

}

void ScriptObject::SetPingPong(bool val)
{
	if(obj==0) return;
	if(obj->GetType()&OBJECT_TYPE_ANIMABLE)
	{
		((AnimableObject*)obj)->m_pingpong = val;
		((AnimableObject*)obj)->Reset();
	}
#ifdef USEBOX2DPHYSICS
	else if(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)
	{
		((PhysicsObject*)obj)->m_pingpong = val;
		((PhysicsObject*)obj)->Reset();
	}  
#endif // USEBOX2DPHYSICS

}

void ScriptObject::SetPosAnim(ScriptPoint* start, ScriptPoint* end)
{
	if(obj==0)
	{
		start->release();
		end->release();
		return;
	}
	if((obj->GetType()&OBJECT_TYPE_ANIMABLE))
	{
		((AnimableObject*)obj)->m_animtype |= 1;
		((AnimableObject*)obj)->m_spos.left = start->x;
		((AnimableObject*)obj)->m_spos.top = start->y;
		((AnimableObject*)obj)->m_epos.left = end->x;
		((AnimableObject*)obj)->m_epos.top = end->y;
		((AnimableObject*)obj)->Reset();
	}
#ifdef USEBOX2DPHYSICS
	else if((obj->GetType()&OBJECT_TYPE_PHYSICSOBJ))
	{
		((PhysicsObject*)obj)->m_animtype |= 1;
		((PhysicsObject*)obj)->m_spos.left = start->x;
		((PhysicsObject*)obj)->m_spos.top = start->y;
		((PhysicsObject*)obj)->m_epos.left = end->x;
		((PhysicsObject*)obj)->m_epos.top = end->y;
		((PhysicsObject*)obj)->Reset();
	}  
#endif // USEBOX2DPHYSICS
	start->release();
	end->release();
}

void ScriptObject::SetRotationAnim(float start, float end)
{
	if(obj==0) return;
	if(obj->GetType()&OBJECT_TYPE_ANIMABLE)
	{
		((AnimableObject*)obj)->m_animtype |= 2;
		((AnimableObject*)obj)->m_rspos = start;
		((AnimableObject*)obj)->m_repos = end;
		((AnimableObject*)obj)->Reset();
	}
#ifdef USEBOX2DPHYSICS
	else if(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)
	{
		((PhysicsObject*)obj)->m_animtype |= 2;
		((PhysicsObject*)obj)->m_rspos = start;
		((PhysicsObject*)obj)->m_repos = end;
		((PhysicsObject*)obj)->Reset();
	}  
#endif // USEBOX2DPHYSICS

}

void ScriptObject::SetColorAnim(unsigned int start, unsigned int end)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_ANIMABLE)) return;
	((AnimableObject*)obj)->m_animtype |= 4;
	((AnimableObject*)obj)->m_scolor = start;
	((AnimableObject*)obj)->m_ecolor = end;
	((AnimableObject*)obj)->Reset();
}

void ScriptObject::SetScaleAnim(float start, float end)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_ANIMABLE)) return;
	((AnimableObject*)obj)->m_animtype |= 8;
	((AnimableObject*)obj)->m_scalefrom = start;
	((AnimableObject*)obj)->m_scaleto = end;
	((AnimableObject*)obj)->Reset();
}

float ScriptObject::GetDelay()
{
	if(obj==0) return 0;
	if(obj->GetType()&OBJECT_TYPE_ANIMABLE)
	{
		return ((AnimableObject*)obj)->m_delay;
	}
#ifdef USEBOX2DPHYSICS
	else if(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)
	{
		return ((PhysicsObject*)obj)->m_delay;
	}  
#endif // USEBOX2DPHYSICS

	return 0.0f;
}

float ScriptObject::GetSpeed()
{
	if(obj==0) return 0;
	if(obj->GetType()&OBJECT_TYPE_ANIMABLE)
	{
		return ((AnimableObject*)obj)->m_speed;
	}
#ifdef USEBOX2DPHYSICS
	else if(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)
	{
		return ((PhysicsObject*)obj)->m_speed;
	}  
#endif // USEBOX2DPHYSICS

	 return 0.0f;
}

int ScriptObject::GetAnimatiomType()
{
	if(obj==0) return 0;
	if(obj->GetType()&OBJECT_TYPE_ANIMABLE)
	{
		return ((AnimableObject*)obj)->m_animtype;
	}
#ifdef USEBOX2DPHYSICS
	else if(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)
	{
		return ((PhysicsObject*)obj)->m_animtype;
	}  
#endif // USEBOX2DPHYSICS

	return 0;
}

int ScriptObject::GetPlayType()
{
	if(obj==0) return 0;
	if(obj->GetType()&OBJECT_TYPE_ANIMABLE)
	{
		return ((AnimableObject*)obj)->m_playtype;
	}
#ifdef USEBOX2DPHYSICS
	else if(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)
	{
		return ((PhysicsObject*)obj)->m_playtype;
	}  
#endif // USEBOX2DPHYSICS

	return 0;
}

bool ScriptObject::GetPingPong()
{
	if(obj==0) return false;
	if(obj->GetType()&OBJECT_TYPE_ANIMABLE)
	{
		return ((AnimableObject*)obj)->m_pingpong;
	}
#ifdef USEBOX2DPHYSICS
	else if(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)
	{
		return ((PhysicsObject*)obj)->m_pingpong;
	}  
#endif // USEBOX2DPHYSICS

	return false;
}

ScriptPoint* ScriptObject::GetPosAnimStart()
{
	ScriptPoint* pt = new ScriptPoint();
	pt->x = 0;
	pt->y = 0;

	if(obj==0) return pt;
	if(obj->GetType()&OBJECT_TYPE_ANIMABLE)
	{
		pt->x = ((AnimableObject*)obj)->m_spos.left;
		pt->y = ((AnimableObject*)obj)->m_spos.top;
	}
#ifdef USEBOX2DPHYSICS
	else if(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)
	{
		pt->x = ((PhysicsObject*)obj)->m_spos.left;
		pt->y = ((PhysicsObject*)obj)->m_spos.top;
	}  
#endif // USEBOX2DPHYSICS

	return pt;
}

ScriptPoint* ScriptObject::GetPosAnimEnd()
{
	ScriptPoint* pt = new ScriptPoint();
	pt->x = 0;
	pt->y = 0;

	if(obj==0) return pt;
	if(obj->GetType()&OBJECT_TYPE_ANIMABLE)
	{
		pt->x = ((AnimableObject*)obj)->m_epos.left;
		pt->y = ((AnimableObject*)obj)->m_epos.top;
	}
#ifdef USEBOX2DPHYSICS
	else if(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)
	{
		pt->x = ((PhysicsObject*)obj)->m_epos.left;
		pt->y = ((PhysicsObject*)obj)->m_epos.top;
	}  
#endif // USEBOX2DPHYSICS

	return pt;
}

float ScriptObject::GetRotationAnimStart()
{
	if(obj==0) return 0;
	if(obj->GetType()&OBJECT_TYPE_ANIMABLE)
	{
		return ((AnimableObject*)obj)->m_rspos;
	}
#ifdef USEBOX2DPHYSICS
	else if(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)
	{
		return ((PhysicsObject*)obj)->m_rspos;
	}
#endif
	return 0.0f;
}

float ScriptObject::GetRotationAnimEnd()
{
	if(obj==0) return 0;
	if(obj->GetType()&OBJECT_TYPE_ANIMABLE)
	{
		return ((AnimableObject*)obj)->m_repos;
	}
#ifdef USEBOX2DPHYSICS
	else if(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)
	{
		return ((PhysicsObject*)obj)->m_repos;
	}
#endif
	return 0.0f;
}

unsigned int ScriptObject::GetColorAnimStart()
{
	if(obj==0) return 0;
	if(!(obj->GetType()&OBJECT_TYPE_ANIMABLE)) return 0;
	return ((AnimableObject*)obj)->m_scolor;
}

unsigned int ScriptObject::GetColorAnimEnd()
{
	if(obj==0) return 0;
	if(!(obj->GetType()&OBJECT_TYPE_ANIMABLE)) return 0;
	return ((AnimableObject*)obj)->m_ecolor;
}

float ScriptObject::GetScaleAnimStart()
{
	if(obj==0) return 0;
	if(!(obj->GetType()&OBJECT_TYPE_ANIMABLE)) return 0;
	return ((AnimableObject*)obj)->m_scalefrom;
}

float ScriptObject::GetScaleAnimEnd()
{
	if(obj==0) return 0;
	if(!(obj->GetType()&OBJECT_TYPE_ANIMABLE)) return 0;
	return ((AnimableObject*)obj)->m_scaleto;
}

#ifdef USEBOX2DPHYSICS

void ScriptObject::SetLimits(float limit1, float limit2)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_PHYJOINT)) return;
	((PhyJoint*)obj)->SetLimits(limit1, limit2);
}

void ScriptObject::EnableLimits(bool val)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_PHYJOINT)) return;
	((PhyJoint*)obj)->EnableLimits(val);
}

void ScriptObject::EnableMotor(bool val)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_PHYJOINT)) return;
	((PhyJoint*)obj)->EnableMotor(val);
}

void ScriptObject::SetMotorSpeed(bool val)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_PHYJOINT)) return;
	((PhyJoint*)obj)->SetMotorSpeed(val);
}

void ScriptObject::SetMotorPower(bool val)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_PHYJOINT)) return;
	((PhyJoint*)obj)->SetMotorPower(val);
}

void ScriptObject::SetDampingRatio(float val)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_PHYJOINT)) return;
	((PhyJoint*)obj)->SetDampingRatio(val);
}

void ScriptObject::SetFrequency(float val)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_PHYJOINT)) return;
	((PhyJoint*)obj)->SetFrequency(val);
}

bool ScriptObject::IsLimitEnabled()
{
	if(obj==0) return false;
	if(!(obj->GetType()&OBJECT_TYPE_PHYJOINT)) return false;
	return ((PhyJoint*)obj)->IsLimitEnabled();
}

bool ScriptObject::IsMotorEnabled()
{
	if(obj==0) return false;
	if(!(obj->GetType()&OBJECT_TYPE_PHYJOINT)) return false;
	return ((PhyJoint*)obj)->IsMotorEnabled();
}

float ScriptObject::GetLimit1()
{
	if(obj==0) return 0;
	if(!(obj->GetType()&OBJECT_TYPE_PHYJOINT)) return 0;
	return ((PhyJoint*)obj)->GetLimit1();
}

float ScriptObject::GetLimit2()
{
	if(obj==0) return 0;
	if(!(obj->GetType()&OBJECT_TYPE_PHYJOINT)) return 0;
	return ((PhyJoint*)obj)->GetLimit2();
}

float ScriptObject::GetMotorPower()
{
	if(obj==0) return 0;
	if(!(obj->GetType()&OBJECT_TYPE_PHYJOINT)) return 0;
	return ((PhyJoint*)obj)->GetMotorPower();
}

float ScriptObject::GetMootorSpeed()
{
	if(obj==0) return 0;
	if(!(obj->GetType()&OBJECT_TYPE_PHYJOINT)) return 0;
	return ((PhyJoint*)obj)->GetMootorSpeed();
}

float ScriptObject::GetDampingRatio()
{
	if(obj==0) return 0;
	if(!(obj->GetType()&OBJECT_TYPE_PHYJOINT)) return 0;
	return ((PhyJoint*)obj)->GetDampingRatio();
}

float ScriptObject::GetFrequency()
{
	if(obj==0) return 0;
	if(!(obj->GetType()&OBJECT_TYPE_PHYJOINT)) return 0;
	return ((PhyJoint*)obj)->GetFrequency();
}

short ScriptObject::GetTeam()
{
	if(obj==0) return 0;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return 0;
	return ((PhysicsObject*)obj)->GetTeam();
}

short ScriptObject::GetTeamMask()
{
	if(obj==0) return 0;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return 0;
	return ((PhysicsObject*)obj)->GetTeamMask();
}

void ScriptObject::SetTeam(short val, short mask)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return;
	((PhysicsObject*)obj)->SetTeam(val, mask);
}

void ScriptObject::SetSensor(bool val)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return;
	((PhysicsObject*)obj)->SetSensor(val);
}

bool ScriptObject::GetSensor()
{
	if(obj==0) return false;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return false;
	return ((PhysicsObject*)obj)->GetSensor();
	return false;
}

void ScriptObject::SetBodyType(int val)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return;
	((PhysicsObject*)obj)->SetBodyType(val);
}

int ScriptObject::GetBodyType()
{
	if(obj==0) return 0;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return 0;
	return ((PhysicsObject*)obj)->GetBodyType();
	return 0;
}

void ScriptObject::SetBodyShape(int val)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return;
	((PhysicsObject*)obj)->SetBodyShape(val);
}

int ScriptObject::GetBodyShape()
{
	if(obj==0) return 0;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return 0;
	return ((PhysicsObject*)obj)->GetBodyShape();
	return 0;
}

void ScriptObject::SetRestitution(float val)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return;
	((PhysicsObject*)obj)->SetRestitution(val);
}

void ScriptObject::SetFriction(float val)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return;
	((PhysicsObject*)obj)->SetFriction(val);
}

float ScriptObject::GetRestitution()
{
	if(obj==0) return 0;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return 0;
	return ((PhysicsObject*)obj)->GetRestitution();
	return 0;
}

float ScriptObject::GetFriction()
{
	if(obj==0) return 0;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return 0;
	return ((PhysicsObject*)obj)->GetFriction();
	return 0;
}

void ScriptObject::SetVelocity(ScriptPoint* vel)
{
	PointF pt(vel->x, vel->y);
	vel->release();
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return;
	((PhysicsObject*)obj)->SetVelocity(pt);
}

ScriptPoint* ScriptObject::GetVelocity()
{
	ScriptPoint* pt = new ScriptPoint();
	pt->x = 0;
	pt->y = 0;

	if(obj==0) return pt;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return pt;
	PointF pt2 = ((PhysicsObject*)obj)->GetVelocity();
	pt->x = pt2.left;
	pt->y = pt2.top;
	return pt;
}

void ScriptObject::ApplyForce(ScriptPoint* vel)
{
	PointF pt(vel->x, vel->y);
	vel->release();
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return;
	((PhysicsObject*)obj)->ApplyForce(pt);
}

void ScriptObject::ApplyImpulse(ScriptPoint* vel)
{
	PointF pt(vel->x, vel->y);
	vel->release();
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return;
	((PhysicsObject*)obj)->ApplyImpulse(pt);
}

void ScriptObject::ApplyTorque(float val)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return;
	((PhysicsObject*)obj)->ApplyTorque(val);
}

void ScriptObject::ApplyAngImpulse(float vel)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return;
	((PhysicsObject*)obj)->ApplyAngImpulse(vel);
}

void ScriptObject::SetTransform(ScriptPoint* vel)
{
	PointF pt(vel->x, vel->y);
	vel->release();
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return;
	((PhysicsObject*)obj)->SetObjectTransform(pt);
}

ScriptPoint* ScriptObject::GetTransform()
{
	ScriptPoint* pt = new ScriptPoint();
	pt->x = 0;
	pt->y = 0;

	if(obj==0) return pt;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return pt;
	PointF pt2 = ((PhysicsObject*)obj)->GetObjectTransform();
	pt->x = pt2.left;
	pt->y = pt2.top;
	return pt;
}

void ScriptObject::SetPhyAngle(float val)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return;
	((PhysicsObject*)obj)->SetObjectAngle(val);
}

float ScriptObject::GetPhyAngle()
{
	if(obj==0) return 0;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return 0;
	return ((PhysicsObject*)obj)->GetObjectAngle();
}

void ScriptObject::SetDensity(float val)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return;
	((PhysicsObject*)obj)->SetDensity(val);
}

float ScriptObject::GetDensity()
{
	if(obj==0) return 0;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return 0;
	return ((PhysicsObject*)obj)->GetDensity();
}

void ScriptObject::SetGravityScale(float val)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return;
	((PhysicsObject*)obj)->SetGravityScale(val);
}

void ScriptObject::SetAngularDamping(float val)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return;
	((PhysicsObject*)obj)->SetGravityScale(val);
}

void ScriptObject::SetLinearDamping(float val)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return;
	((PhysicsObject*)obj)->SetGravityScale(val);
}

float ScriptObject::GetGravityScale()
{
	if(obj==0) return 0;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return 0;
	return ((PhysicsObject*)obj)->GetGravityScale();
}

float ScriptObject::GetAngularDamping()
{
	if(obj==0) return 0;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return 0;
	return ((PhysicsObject*)obj)->GetAngularDamping();
}

float ScriptObject::GetLinearDamping()
{
	if(obj==0) return 0;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return 0;
	return ((PhysicsObject*)obj)->GetLinearDamping();
}

void ScriptObject::LockRotation(bool val)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return;
	((PhysicsObject*)obj)->LockRotation(val);
}

bool ScriptObject::isFixedRotation()
{
	if(obj==0) return false;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return false;
	return ((PhysicsObject*)obj)->isFixedRotation();
}

void ScriptObject::SetAngularVelocity(float val)
{
	if(obj==0) return;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return;
	((PhysicsObject*)obj)->SetAngularVelocity(val);
}

float ScriptObject::GetAngularVelocity()
{
	if(obj==0) return 0;
	if(!(obj->GetType()&OBJECT_TYPE_PHYSICSOBJ)) return 0;
	return ((PhysicsObject*)obj)->GetAngularVelocity();
}

void SetGravity(ScriptPoint* vel)
{
	Game->GetPhysicsWorld()->GetWorld().SetGravity(b2Vec2(vel->x, vel->y));
	vel->release();
}

ScriptPoint* GetGravity()
{
	ScriptPoint* pt = new ScriptPoint();
	pt->x = Game->GetPhysicsWorld()->GetWorld().GetGravity().x;
	pt->y = Game->GetPhysicsWorld()->GetWorld().GetGravity().y;
	return pt;
}
#endif

ScriptObject* objectfactory()
{
	return new ScriptObject();
}

//Global functionds

void SetXP(int val)
{
	Game->settings.XP = val;
}

int GetXP()
{
	return Game->settings.XP;
}

void SetLevels(unsigned int val)
{
	Game->settings.m_levels = val;
}

unsigned int GetLevels()
{
	return Game->settings.m_levels;
}

int GetCurrentLevel()
{
	return Game->CurrentLevel;
}

void SetCurrentLevel(int val)
{
	Game->CurrentLevel = val;
}

ScriptObject* GetObjectByID(unsigned int ID)
{
	ScriptObject* obj = new ScriptObject();
	obj->obj = Game->GetMainContainer()->GetObjectByID(ID);
	return obj;
}

ScriptObject* GetBackground()
{
	ScriptObject* obj = new ScriptObject();
	obj->obj = Game->GetMainContainer();
	return obj;
}

void Win()
{
	Game->Win();
}

void Loose()
{
	Game->Loose();
}

void TogglePause()
{
	Game->TogglePause();
}

void Pause()
{
	Game->Pause();
}

void Unpause()
{
	Game->Unpause();
}

void ResetData()
{
	Game->ResetAllData();
}

bool LoadFile(string filename, unsigned int resgroup, bool lvlbutton, int clvl, int ulvl, string snd, string script, string win)
{
	UIWString file = filename.c_str();
	UIWString scriptfile = script.c_str();
	UIWString winfile = win.c_str();

	Resource sound;
	Game->GetResourceManager()->GetResource(snd.c_str(), RES_TYPE_SOUND, &sound);
	return Game->LoadFile(file, resgroup, lvlbutton, clvl, ulvl, sound, scriptfile, winfile);
}

bool SaveFile(string filename)
{
	UIWString sepath = Game->m_savedir.c_str();
	sepath += L"/";
	sepath += filename.c_str();

	C_File file(Game->GetResourceManager());
	if(file.Open(sepath.GetPtr(), BaseIO::Access_ALL))
	{
		Game->GetMainContainer()->SaveAll(file);
		file.Close();
		return true;
	}
	return false;
}

void PlayMusic(string name, bool loop)
{
	Resource sound;
	Game->GetResourceManager()->GetResource(name.c_str(), RES_TYPE_SOUND, &sound);
	Game->GetSoundPlayer()->SetBuffer(sound);
	Game->GetSoundPlayer()->Play(loop);
}

int GetMusicVolume()
{
	return Game->settings.m_mvol;
}

int GetSoundVolume()
{
	return Game->settings.m_svol;
}

void SetMusicVolume(int val)
{
	Game->SetMusicVolume(val);
}

void SetSoundVolume(int val)
{
	Game->SetSoundVolume(val);
}

void Exit()
{
	Game->QuitApp();
}

void SaveSettings()
{
	Game->SaveSettings();
}

int GetStoreInt(string str)
{
	map<string, int>::iterator i = Game->settings.extraint.find(str);
	if(i!=Game->settings.extraint.end())
	{
		return i->second;
	}
	return -1;
}

float GetStoreFloat(string str)
{
	map<string, float>::iterator i = Game->settings.extrafloat.find(str);
	if(i!=Game->settings.extrafloat.end())
	{
		return i->second;
	}
	return -1;
}

void SetStoreInt(string str, int val)
{
	Game->settings.extraint[str] = val;
}

void SetStoreFloat(string str, float val)
{
	Game->settings.extrafloat[str] = val;
}

void EraseStoreInt(string str)
{
	Game->settings.extraint.erase(str);
}

void EraseStoreFloat(string str)
{
	Game->settings.extrafloat.erase(str);
}

bool ExistsStoreInt(string str)
{
	return (Game->settings.extraint.find(str) != Game->settings.extraint.end());
}

bool ExistsStoreFloat(string str)
{
	return (Game->settings.extrafloat.find(str) != Game->settings.extrafloat.end());
}

void SetXPGain(int val)
{
	Game->XPGain = val;
}

int GetXPGain()
{
	return Game->XPGain;
}

void SetStars(char val)
{
	Game->Stars = val;
}

char GetStars()
{
	return Game->Stars;
}

void IncreaseXPGain(int val)
{
	Game->XPGain += val;
}

void CreateSprite(ScriptRectangle* rct, string name, int index, int animtype, bool paused)
{
	Resource res;
	Game->GetResourceManager()->GetResource(name.c_str(), RES_TYPE_IMAGESET, &res);
	Sprite spr;
	spr.SetImageSet(res);
	spr.SetIndex(index);
	spr.SetAnimationType((Sprite::AnimationType)animtype);
	spr.Reset();
	if(paused) spr.Pause();
	else spr.Resume();

	AnimableObject* obj = (AnimableObject*)Game->GetBuilder()->BuildObject("AnimableObject");
	if(obj==0) 
	{
		rct->release();
		return;
	}
	RectangleF rc(rct->x, rct->y, rct->r, rct->b);
	obj->SetObjectArea(rc);
	((AnimableObject*)obj)->SetSprite(spr);
	Game->GetMainContainer()->AddObject(obj);
	rct->release();
}

ScriptObject* CreateObject(string name, unsigned int ID, ScriptRectangle* rct)
{
	ScriptObject* objr = new ScriptObject();
	objr->obj = 0;
	DrawableObject* obj = (DrawableObject*)Game->GetBuilder()->BuildObject(name.c_str());
	if(obj==0) return objr;
	RectangleF rc(rct->x, rct->y, rct->r, rct->b);
	obj->SetObjectArea(rc);
	//((AnimableObject*)obj)->SetSprite(spr);
	Game->GetMainContainer()->AddObject(obj);

	objr->obj = obj;
	rct->release();
	return objr;
}

unsigned int GetMainID()
{
	return Game->GetMainContainer()->GetID();
}

ScriptPoint* GetMousePoint()
{
	ScriptPoint* pt = new ScriptPoint();
	pt->x = (float)Script::m_clkpoint.left;
	pt->y = (float)Script::m_clkpoint.top;
	return pt;
}

ScriptPoint* RelativePoint(ScriptPoint* pt)
{
	PointF pt2(pt->x, pt->y);
	float scale = 1.0f;
	RectangleF absu = ((ObjectsContainer*)Game->GetMainContainer())->GetAbsolutePos();
	RectangleF obarea = ((ObjectsContainer*)Game->GetMainContainer())->GetObjectArea();

	scale = ((ObjectsContainer*)Game->GetMainContainer())->GetCamera().m_scale;

	pt2.left = pt2.left + obarea.left;
	pt2.top = pt2.top + obarea.top;
	pt2.left = pt2.left / scale;
	pt2.top = pt2.top / scale;
	pt2.top = pt2.top - absu.top;
	pt2.left = pt2.left - absu.left;

	ScriptPoint* pt3 = new ScriptPoint();
	pt3->x = pt2.left;
	pt3->y = pt2.top;
	pt->release();
	return pt3;
}

void CPrint(string val)
{
	printf(val.c_str());
}

float GetTimeDelta()
{
	return Game->m_tdelta;
}

ScriptPoint* GetDirection(ScriptPoint* val, ScriptPoint* val2)
{
	ScriptPoint* pt3 = new ScriptPoint();
	pt3->x = val2->x - val->x;
	pt3->y = val2->y - val->y;
	float d = (pt3->x * pt3->x) + (pt3->y * pt3->y);
	d = sqrt(d);
	if(d>0)
	{
		pt3->x /= d;
		pt3->y /= d;
	}
	val->release();
	val2->release();
	return pt3;
}

float GetDistance(ScriptPoint* val)
{
	float d = (val->x * val->x) + (val->y * val->y);
	val->release();
	return sqrt(d);
}

float GetSquaredDistance(ScriptPoint* val)
{
	float sdist = (val->x * val->x) + (val->y * val->y);
	val->release();
	return sdist;
}

float GetDistance2(ScriptPoint* val, ScriptPoint* val2)
{
	ScriptPoint* pt3 = new ScriptPoint();
	pt3->x = val2->x - val->x;
	pt3->y = val2->y - val->y;
	float d = (pt3->x * pt3->x) + (pt3->y * pt3->y);
	val->release();
	val2->release();
	return sqrt(d);
}

float GetSquaredDistance2(ScriptPoint* val, ScriptPoint* val2)
{
	ScriptPoint* pt3 = new ScriptPoint();
	pt3->x = val2->x - val->x;
	pt3->y = val2->y - val->y;
	val->release();
	val2->release();
	return (pt3->x * pt3->x) + (pt3->y * pt3->y);
}

float GetAngle(ScriptPoint* val)
{
	float ang = acos(val->x);
	if(val->y < 0) ang = C_PI + (C_PI - ang);
	val->release();
	return ang;
}

void FocusCamera(ScriptObject* obj, float offx, float offy)
{
	if(obj->obj==0) return;
	RectangleF pos = ((DrawableObject*)obj->obj)->GetObjectArea();
	if(offx == 0 && offy == 0)
	{
		offx = (Game->GetMainContainer()->GetObjectArea().right - Game->GetMainContainer()->GetObjectArea().left) / 2.0f;
		offy = (Game->GetMainContainer()->GetObjectArea().bottom - Game->GetMainContainer()->GetObjectArea().top) / 2.0f;
	}
	if(Game->GetMainContainer()!=obj->obj) Game->GetMainContainer()->FocusCameraOn(PointF(pos.left + (pos.right - pos.left) / 2.0f, pos.top + (pos.bottom - pos.top) / 2.0f), PointF(offx, offy));
	obj->release();
}

void FocusCamera2(float x, float y, float offx, float offy)
{
	if(offx == 0 && offy == 0)
	{
		offx = (Game->GetMainContainer()->GetObjectArea().right - Game->GetMainContainer()->GetObjectArea().left) / 2.0f;
		offy = (Game->GetMainContainer()->GetObjectArea().bottom - Game->GetMainContainer()->GetObjectArea().top) / 2.0f;
	}
	Game->GetMainContainer()->FocusCameraOn(PointF(x, y), PointF(offx, offy));
}

void MoveCamera(float x, float y)
{
	Game->GetMainContainer()->MoveCamera(x, y);
}

void ZoomCamera(float val)
{
	Game->GetMainContainer()->Zoom(val);
}

float GetCameraZoom()
{
	return Game->GetMainContainer()->GetCamera().m_zoom;
}

void MessageCallback(const asSMessageInfo *msg, void *param)
{
	ScriptError err;
	err.col = msg->col;
	err.row = msg->row;
	err.type = msg->type;
	err.message = msg->message;

	((Script*)param)->errors[msg->row]=err;
#ifndef ANDROID
	printf(msg->message);
	printf(" (line:%d, col:%d)", msg->row, msg->col);
	printf("\n");
	//MessageBoxA(0, str, "error", MB_OK);
#endif

#ifdef ANDROID
	__android_log_print(ANDROID_LOG_VERBOSE, "native", "Script compilation error >> %s", msg->message);
#endif
}

//void MyGenericFunction(asIScriptGeneric *gen)
//{
//  // Extract the arguments
//  int arg0              = gen->GetArgDWord(0);
//  float arg1            = gen->GetArgFloat(1);
//  asIScriptObject *arg2 = reinterpret_cast<asIScriptObject*>(gen->GetArgObject(2));
//  
//  // Call the real function
//  asIScriptObject *ret = MyFunction(arg0, arg1, arg2);
//  
//  // Set the return value
//  gen->SetReturnObject(ret);
//}

Script::Script()
{
	engine = 0;
	mainfn = 0;
	funid = 0;
	fid = 0;
	callbackfn = 0;

	engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	if(engine==0) return;

	RegisterScriptArray(engine, true);
	RegisterStdString(engine);
	RegisterScriptMath(engine);

	if( strstr(asGetLibraryOptions(), "AS_MAX_PORTABILITY") )
	{
		engine->RegisterObjectType("Rectangle", 0, asOBJ_REF);
		engine->RegisterObjectBehaviour("Rectangle", asBEHAVE_FACTORY, "Rectangle@ f()", WRAP_FN(reactanglefactory), asCALL_GENERIC);
		engine->RegisterObjectBehaviour("Rectangle", asBEHAVE_FACTORY, "Rectangle@ f(float x, float y, float r, float b)", WRAP_FN(reactanglefactory2), asCALL_GENERIC);
		engine->RegisterObjectBehaviour("Rectangle", asBEHAVE_ADDREF, "void f()", WRAP_MFN(ScriptRectangle, addref), asCALL_GENERIC);
		engine->RegisterObjectBehaviour("Rectangle", asBEHAVE_RELEASE, "void f()", WRAP_MFN(ScriptRectangle, release), asCALL_GENERIC);
		engine->RegisterObjectProperty("Rectangle", "float Left", asOFFSET(ScriptRectangle, x));
		engine->RegisterObjectProperty("Rectangle", "float Top", asOFFSET(ScriptRectangle, y));
		engine->RegisterObjectProperty("Rectangle", "float Right", asOFFSET(ScriptRectangle, r));
		engine->RegisterObjectProperty("Rectangle", "float Bottom", asOFFSET(ScriptRectangle, b));

		engine->RegisterObjectType("Point", 0, asOBJ_REF);
		engine->RegisterObjectBehaviour("Point", asBEHAVE_FACTORY, "Point@ f()", WRAP_FN(pointfactory), asCALL_GENERIC);
		engine->RegisterObjectBehaviour("Point", asBEHAVE_FACTORY, "Point@ f(float x, float y)", WRAP_FN(pointfactory2), asCALL_GENERIC);
		engine->RegisterObjectBehaviour("Point", asBEHAVE_ADDREF, "void f()", WRAP_MFN(ScriptPoint,addref), asCALL_GENERIC);
		engine->RegisterObjectBehaviour("Point", asBEHAVE_RELEASE, "void f()", WRAP_MFN(ScriptPoint,release), asCALL_GENERIC);
		engine->RegisterObjectProperty("Point", "float x", asOFFSET(ScriptPoint, x));
		engine->RegisterObjectProperty("Point", "float y", asOFFSET(ScriptPoint, y));

		engine->RegisterObjectType("Object", 0, asOBJ_REF);
		engine->RegisterObjectBehaviour("Object", asBEHAVE_FACTORY, "Object@ f()", WRAP_FN(objectfactory), asCALL_GENERIC);
		engine->RegisterObjectBehaviour("Object", asBEHAVE_ADDREF, "void f()", WRAP_MFN(ScriptObject,addref), asCALL_GENERIC);
		engine->RegisterObjectBehaviour("Object", asBEHAVE_RELEASE, "void f()", WRAP_MFN(ScriptObject,release), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "Object@ opAssign(Object@ obj)", WRAP_MFN_PR(ScriptObject, operator=, (ScriptObject*), ScriptObject*), asCALL_GENERIC);
	
		engine->RegisterObjectMethod("Object", "uint GetID()", WRAP_MFN(ScriptObject, GetID), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void Show()", WRAP_MFN(ScriptObject, Show), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void Hide()", WRAP_MFN(ScriptObject, Hide), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void Enable()", WRAP_MFN(ScriptObject, Enable), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void Disable()", WRAP_MFN(ScriptObject, Disable), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void SetText(string)", WRAP_MFN(ScriptObject, SetText), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "string GetText()", WRAP_MFN(ScriptObject, GetText), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void SetColor(uint)", WRAP_MFN(ScriptObject, SetColor), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "uint GetColor()", WRAP_MFN(ScriptObject, GetColor), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void SetRotation(float)", WRAP_MFN(ScriptObject, SetRotation), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "float GetRotation()", WRAP_MFN(ScriptObject, GetRotation), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "int GetType()", WRAP_MFN(ScriptObject, GetType), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void QueueForDelete()", WRAP_MFN(ScriptObject, QueueForDelete), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void Draw()", WRAP_MFN(ScriptObject, Draw), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "uint GetFlags()", WRAP_MFN(ScriptObject, GetFlags), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void SetFlags(uint)", WRAP_MFN(ScriptObject, SetFlags), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "Rectangle@ GetObjectArea()", WRAP_MFN(ScriptObject, GetObjectArea), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "Rectangle@ GetAbsolutePosition()", WRAP_MFN(ScriptObject, GetAbsolutePosition), asCALL_GENERIC);

		engine->RegisterObjectMethod("Object", "void SetObjectArea(Rectangle@)", WRAP_MFN(ScriptObject, SetObjectArea), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void SetSprite(string name, int index, int animtype = 0, bool paused = false)", WRAP_MFN(ScriptObject, SetSprite), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void SetLayer(float)", WRAP_MFN(ScriptObject, SetLayer), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "float GetLayer()", WRAP_MFN(ScriptObject, GetLayer), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void SetValue(int)", WRAP_MFN(ScriptObject, SetValue), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "int GetValue()", WRAP_MFN(ScriptObject, GetValue), asCALL_GENERIC);

		engine->RegisterObjectMethod("Object", "void SetMax(float val)", WRAP_MFN(ScriptObject, SetMax), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "float GetMax()", WRAP_MFN(ScriptObject, GetMax), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void SetMin(float val)", WRAP_MFN(ScriptObject, SetMin), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "float GetMin()", WRAP_MFN(ScriptObject, GetMin), asCALL_GENERIC);

		engine->RegisterObjectMethod("Object", "void SetCapture()", WRAP_MFN(ScriptObject, SetCapture), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void ReleaseCapture()", WRAP_MFN(ScriptObject, ReleaseCapture), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "bool GetCapture()", WRAP_MFN(ScriptObject, GetCapture), asCALL_GENERIC);

		engine->RegisterObjectMethod("Object", "void StartTimer()", WRAP_MFN(ScriptObject, StartTimer), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void StopTimer()", WRAP_MFN(ScriptObject, StopTimer), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void PauseTimer()", WRAP_MFN(ScriptObject, PauseTimer), asCALL_GENERIC);

		engine->RegisterObjectMethod("Object", "void Play(int animtype)", WRAP_MFN(ScriptObject, Play), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void Play()", WRAP_MFN(ScriptObject, Play1), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void Pause()", WRAP_MFN(ScriptObject, Pause), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void Stop()", WRAP_MFN(ScriptObject, Stop), asCALL_GENERIC);

		engine->RegisterObjectMethod("Object", "void SetDelay(float delay)", WRAP_MFN(ScriptObject, SetDelay), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void SetSpeed(float speed)", WRAP_MFN(ScriptObject, SetSpeed), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void SetAnimatiomType(int type)", WRAP_MFN(ScriptObject, SetAnimatiomType), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void SetPlayType(int type)", WRAP_MFN(ScriptObject, SetPlayType), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void SetPingPong(bool val)", WRAP_MFN(ScriptObject, SetPingPong), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void SetPosAnim(Point@ start, Point@ end)", WRAP_MFN(ScriptObject, SetPosAnim), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void SetRotationAnim(float start, float end)", WRAP_MFN(ScriptObject, SetRotationAnim), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void SetColorAnim(uint start, uint end)", WRAP_MFN(ScriptObject, SetColorAnim), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void SetScaleAnim(float start, float end)", WRAP_MFN(ScriptObject, SetScaleAnim), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "float GetDelay()", WRAP_MFN(ScriptObject, GetDelay), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "float GetSpeed()", WRAP_MFN(ScriptObject, GetSpeed), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "int GetAnimatiomType()", WRAP_MFN(ScriptObject, GetAnimatiomType), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "int GetPlayType()", WRAP_MFN(ScriptObject, GetPlayType), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "bool GetPingPong()", WRAP_MFN(ScriptObject, GetPingPong), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "Point@ GetPosAnimStart()", WRAP_MFN(ScriptObject, GetPosAnimStart), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "Point@ GetPosAnimEnd()", WRAP_MFN(ScriptObject, GetPosAnimEnd), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "float GetRotationAnimStart()", WRAP_MFN(ScriptObject, GetRotationAnimStart), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "float GetRotationAnimEnd()", WRAP_MFN(ScriptObject, GetRotationAnimEnd), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "uint GetColorAnimStart()", WRAP_MFN(ScriptObject, GetColorAnimStart), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "uint GetColorAnimEnd()", WRAP_MFN(ScriptObject, GetColorAnimEnd), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "float GetScaleAnimStart()", WRAP_MFN(ScriptObject, GetScaleAnimStart), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "float GetScaleAnimEnd()", WRAP_MFN(ScriptObject, GetScaleAnimEnd), asCALL_GENERIC);		

		engine->RegisterObjectMethod("Object", "void PlaySprite()", WRAP_MFN(ScriptObject, PlaySprite), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void SetSpriteIndex(uint ind)", WRAP_MFN(ScriptObject, SetSpriteIndex), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void MirrorSpriteX(bool val)", WRAP_MFN(ScriptObject, MirrorSpriteX), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void MirrorSpriteY(bool val)", WRAP_MFN(ScriptObject, MirrorSpriteY), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void PauseSprite()", WRAP_MFN(ScriptObject, PauseSprite), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void StopSprite()", WRAP_MFN(ScriptObject, StopSprite), asCALL_GENERIC);	

		engine->RegisterObjectMethod("Object", "void EmitParticles(Point@)", WRAP_MFN(ScriptObject, EmitParticles1), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void EmitParticles(float x, float y, float size = 1)", WRAP_MFN(ScriptObject, EmitParticles2), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void AddEmitRect(Rectangle@, float life = 1)", WRAP_MFN(ScriptObject, AddEmitRect), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void AddEmitRect(float x, float y, float life, float size = 1)", WRAP_MFN(ScriptObject, AddEmitRect2), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void ClearEmitRect()", WRAP_MFN(ScriptObject, ClearEmitRect), asCALL_GENERIC);

		engine->RegisterObjectMethod("Object", "uint GetObjectType()", WRAP_MFN(ScriptObject, GetObjectType), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void SetObjectType(uint val)", WRAP_MFN(ScriptObject, SetObjectType), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "string GetObjectName()", WRAP_MFN(ScriptObject, GetObjectName), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void SetObjectName(string name)", WRAP_MFN(ScriptObject, SetObjectName), asCALL_GENERIC);

		engine->RegisterObjectMethod("Object", "void SetProperty(string name, float val)", WRAP_MFN(ScriptObject, SetProperty), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "float GetProperty(string name)", WRAP_MFN(ScriptObject, GetProperty), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void ClearProperties()", WRAP_MFN(ScriptObject, ClearProperties), asCALL_GENERIC);

		engine->RegisterObjectMethod("Object", "bool isPlaying()", WRAP_MFN(ScriptObject, isPlaying), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void SetSoundResource(string name)", WRAP_MFN(ScriptObject, SetSoundResource), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void SetVolume(int val)", WRAP_MFN(ScriptObject, SetVolume), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "int GetVolume()", WRAP_MFN(ScriptObject, GetVolume), asCALL_GENERIC);

#ifdef USEBOX2DPHYSICS
		engine->RegisterObjectMethod("Object", "int16 GetTeam()", WRAP_MFN(ScriptObject, GetTeam), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "int16 GetTeamMask()", WRAP_MFN(ScriptObject, GetTeamMask), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void SetTeam(int16 val, int16 mask)", WRAP_MFN(ScriptObject, SetTeam), asCALL_GENERIC);

		engine->RegisterObjectMethod("Object", "void SetSensor(bool val)", WRAP_MFN(ScriptObject, SetSensor), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "bool GetSensor()", WRAP_MFN(ScriptObject, GetSensor), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void SetBodyType(int val)", WRAP_MFN(ScriptObject, SetBodyType), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "int GetBodyType()", WRAP_MFN(ScriptObject, GetBodyType), asCALL_GENERIC);

		engine->RegisterObjectMethod("Object", "void SetVelocity(Point@ vel)", WRAP_MFN(ScriptObject, SetVelocity), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "Point@ GetVelocity()", WRAP_MFN(ScriptObject, GetVelocity), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void ApplyForce(Point@ vel)", WRAP_MFN(ScriptObject, ApplyForce), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void ApplyImpulse(Point@ vel)", WRAP_MFN(ScriptObject, ApplyImpulse), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void ApplyTorque(float val)", WRAP_MFN(ScriptObject, ApplyTorque), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void ApplyAngularImpulse(float vel)", WRAP_MFN(ScriptObject, ApplyAngImpulse), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void SetTransform(Point@ vel)", WRAP_MFN(ScriptObject, SetTransform), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "Point@ GetTransform()", WRAP_MFN(ScriptObject, GetTransform), asCALL_GENERIC);

		engine->RegisterObjectMethod("Object", "void SetRestitution(float val)", WRAP_MFN(ScriptObject, SetRestitution), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void SetFriction(float val)", WRAP_MFN(ScriptObject, SetFriction), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "float GetRestitution()", WRAP_MFN(ScriptObject, GetRestitution), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "float GetFriction()", WRAP_MFN(ScriptObject, GetFriction), asCALL_GENERIC);

		engine->RegisterObjectMethod("Object", "void SetBodyShape(int val)", WRAP_MFN(ScriptObject, SetBodyShape), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "int GetBodyShape()", WRAP_MFN(ScriptObject, GetBodyShape), asCALL_GENERIC);

		engine->RegisterObjectMethod("Object", "void SetPhyAngle(float val)", WRAP_MFN(ScriptObject, SetPhyAngle), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "float GetPhyAngle()", WRAP_MFN(ScriptObject, GetPhyAngle), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void SetGravityScale(float val)", WRAP_MFN(ScriptObject, SetGravityScale), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void SetAngularDamping(float val)", WRAP_MFN(ScriptObject, SetAngularDamping), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void SetLinearDamping(float val)", WRAP_MFN(ScriptObject, SetLinearDamping), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "float GetGravityScale()", WRAP_MFN(ScriptObject, GetGravityScale), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "float GetAngularDamping()", WRAP_MFN(ScriptObject, GetAngularDamping), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "float GetLinearDamping()", WRAP_MFN(ScriptObject, GetLinearDamping), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void LockRotation(bool val)", WRAP_MFN(ScriptObject, LockRotation), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void SetDensity(float val)", WRAP_MFN(ScriptObject, SetDensity), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "float GetDensity()", WRAP_MFN(ScriptObject, GetDensity), asCALL_GENERIC);

		engine->RegisterObjectMethod("Object", "void SetLimits(float limit1, float limit2)", WRAP_MFN(ScriptObject, SetLimits), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void EnableLimits(bool val)", WRAP_MFN(ScriptObject, EnableLimits), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void EnableMotor(bool val)", WRAP_MFN(ScriptObject, EnableMotor), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void SetMotorSpeed(bool val)", WRAP_MFN(ScriptObject, SetMotorSpeed), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void SetMotorPower(bool val)", WRAP_MFN(ScriptObject, SetMotorPower), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void SetDampingRatio(float val)", WRAP_MFN(ScriptObject, SetDampingRatio), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void SetFrequency(float val)", WRAP_MFN(ScriptObject, SetFrequency), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "bool IsLimitEnabled()", WRAP_MFN(ScriptObject, IsLimitEnabled), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "bool IsMotorEnabled()", WRAP_MFN(ScriptObject, IsMotorEnabled), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "float GetLimit1()", WRAP_MFN(ScriptObject, GetLimit1), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "float GetLimit2()", WRAP_MFN(ScriptObject, GetLimit2), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "float GetMotorPower()", WRAP_MFN(ScriptObject, GetMotorPower), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "float GetMootorSpeed()", WRAP_MFN(ScriptObject, GetMootorSpeed), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "float GetDampingRatio()", WRAP_MFN(ScriptObject, GetDampingRatio), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "float GetFrequency()", WRAP_MFN(ScriptObject, GetFrequency), asCALL_GENERIC);

		engine->RegisterObjectMethod("Object", "bool IsFixedRotation()", WRAP_MFN(ScriptObject, isFixedRotation), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "void SetAngularVelocity(float val)", WRAP_MFN(ScriptObject, SetAngularVelocity), asCALL_GENERIC);
		engine->RegisterObjectMethod("Object", "float GetAngularVelocity()", WRAP_MFN(ScriptObject, GetAngularVelocity), asCALL_GENERIC);

		engine->RegisterGlobalFunction("void SetGravity(Point@)", WRAP_FN(SetGravity), asCALL_GENERIC);
		engine->RegisterGlobalFunction("Point@ GetGravity()", WRAP_FN(GetGravity), asCALL_GENERIC);
#endif

		engine->RegisterGlobalFunction("Point@ RelativePoint(Point@)", WRAP_FN(RelativePoint), asCALL_GENERIC);
		engine->RegisterGlobalFunction("Point@ GetMousePoint()", WRAP_FN(GetMousePoint), asCALL_GENERIC);
		engine->RegisterGlobalFunction("void SetStars(int8)", WRAP_FN(SetStars), asCALL_GENERIC);
		engine->RegisterGlobalFunction("int8 GetStars()", WRAP_FN(GetStars), asCALL_GENERIC);
		engine->RegisterGlobalFunction("void SetXPGain(int)", WRAP_FN(SetXPGain), asCALL_GENERIC);
		engine->RegisterGlobalFunction("int GetXPGain()", WRAP_FN(GetXPGain), asCALL_GENERIC);
		engine->RegisterGlobalFunction("void IncreaseXPGain(int)", WRAP_FN(IncreaseXPGain), asCALL_GENERIC);
		engine->RegisterGlobalFunction("void SetXP(int)", WRAP_FN(SetXP), asCALL_GENERIC);
		engine->RegisterGlobalFunction("int GetXP()", WRAP_FN(GetXP), asCALL_GENERIC);
		engine->RegisterGlobalFunction("void SetLevels(uint)", WRAP_FN(SetLevels), asCALL_GENERIC);
		engine->RegisterGlobalFunction("uint GetLevels()", WRAP_FN(GetLevels), asCALL_GENERIC);
		engine->RegisterGlobalFunction("Object@ GetObject(uint)", WRAP_FN(GetObjectByID), asCALL_GENERIC);
		engine->RegisterGlobalFunction("void Win()", WRAP_FN(Win), asCALL_GENERIC);
		engine->RegisterGlobalFunction("void Loose()", WRAP_FN(Loose), asCALL_GENERIC);
		engine->RegisterGlobalFunction("void TogglePause()", WRAP_FN(TogglePause), asCALL_GENERIC);
		engine->RegisterGlobalFunction("void ResetData()", WRAP_FN(ResetData), asCALL_GENERIC);
		engine->RegisterGlobalFunction("void Pause()", WRAP_FN(Pause), asCALL_GENERIC);
		engine->RegisterGlobalFunction("void Unpause()", WRAP_FN(Unpause), asCALL_GENERIC);
		engine->RegisterGlobalFunction("bool LoadFile(string filename, uint resgroup = 0, bool lvlbutton = false, int clvl = 0, int ulvl = 0, string snd  = \"\", string script = \"Script.as\", string win = \"win.cfg\")", WRAP_FN(LoadFile), asCALL_GENERIC);
		engine->RegisterGlobalFunction("void Exit()", WRAP_FN(Exit), asCALL_GENERIC);
		engine->RegisterGlobalFunction("void PlayMusic(string name, bool loop = true)", WRAP_FN(PlayMusic), asCALL_GENERIC);
		engine->RegisterGlobalFunction("void SetMusicVolume(int val)", WRAP_FN(SetMusicVolume), asCALL_GENERIC);
		engine->RegisterGlobalFunction("void SetSoundVolume(int val)", WRAP_FN(SetSoundVolume), asCALL_GENERIC);
		engine->RegisterGlobalFunction("int GetMusicVolume()", WRAP_FN(GetMusicVolume), asCALL_GENERIC);
		engine->RegisterGlobalFunction("int GetSoundVolume()", WRAP_FN(GetSoundVolume), asCALL_GENERIC);
		engine->RegisterGlobalFunction("void CreateSprite(Rectangle@, string name, int index, int animtype = 0, bool paused = false)", WRAP_FN(CreateSprite), asCALL_GENERIC);
		engine->RegisterGlobalFunction("void SetCurrentLevel(int)", WRAP_FN(SetCurrentLevel), asCALL_GENERIC);
		engine->RegisterGlobalFunction("int GetCurrentLevel()", WRAP_FN(GetCurrentLevel), asCALL_GENERIC);
		engine->RegisterGlobalFunction("void SaveSettings()", WRAP_FN(SaveSettings), asCALL_GENERIC);
		engine->RegisterGlobalFunction("uint GetMainID()", WRAP_FN(GetMainID), asCALL_GENERIC);
		engine->RegisterGlobalFunction("Object@ GetBackground()", WRAP_FN(GetBackground), asCALL_GENERIC);
		engine->RegisterGlobalFunction("Object@ CreateObject(string name, uint ID, Rectangle@ rct)", WRAP_FN(CreateObject), asCALL_GENERIC);
		engine->RegisterGlobalFunction("bool SaveFile(string filename)", WRAP_FN(SaveFile), asCALL_GENERIC);

		engine->RegisterGlobalFunction("void Print(string val)", WRAP_FN(CPrint), asCALL_GENERIC);

		engine->RegisterGlobalFunction("int GetStoreInt(string str)", WRAP_FN(GetStoreInt), asCALL_GENERIC);
		engine->RegisterGlobalFunction("float GetStoreFloat(string str)", WRAP_FN(GetStoreFloat), asCALL_GENERIC);
		engine->RegisterGlobalFunction("void SetStoreInt(string str, int val)", WRAP_FN(SetStoreInt), asCALL_GENERIC);
		engine->RegisterGlobalFunction("void SetStoreFloat(string str, float val)", WRAP_FN(SetStoreFloat), asCALL_GENERIC);
		engine->RegisterGlobalFunction("void EraseStoreInt(string str)", WRAP_FN(EraseStoreInt), asCALL_GENERIC);
		engine->RegisterGlobalFunction("void EraseStoreFloat(string str)", WRAP_FN(EraseStoreFloat), asCALL_GENERIC);
		engine->RegisterGlobalFunction("bool ExistsStoreInt(string str)", WRAP_FN(ExistsStoreInt), asCALL_GENERIC);
		engine->RegisterGlobalFunction("bool ExistsStoreFloat(string str)", WRAP_FN(ExistsStoreFloat), asCALL_GENERIC);

		engine->RegisterGlobalFunction("float GetTimeDelta()", WRAP_FN(GetTimeDelta), asCALL_GENERIC);
		engine->RegisterGlobalFunction("Point@ GetDirection(Point@ val, Point@ val2)", WRAP_FN(GetDirection), asCALL_GENERIC);
		engine->RegisterGlobalFunction("float GetAngle(Point@ val)", WRAP_FN(GetAngle), asCALL_GENERIC);
		engine->RegisterGlobalFunction("float GetDistance(Point@ val)", WRAP_FN(GetDistance), asCALL_GENERIC);
		engine->RegisterGlobalFunction("float GetSquaredDistance(Point@ val)", WRAP_FN(GetSquaredDistance), asCALL_GENERIC);
		engine->RegisterGlobalFunction("float GetDistance(Point@ val, Point@ val1)", WRAP_FN(GetDistance2), asCALL_GENERIC);
		engine->RegisterGlobalFunction("float GetSquaredDistance(Point@ val, Point@ val1)", WRAP_FN(GetSquaredDistance2), asCALL_GENERIC);
		engine->RegisterGlobalFunction("void FocusCamera(Object@ obj, float offx = 0, float offy = 0)", WRAP_FN(FocusCamera), asCALL_GENERIC);
		engine->RegisterGlobalFunction("void FocusCamera(float x, float y, float offx = 0, float offy = 0)", WRAP_FN(FocusCamera2), asCALL_GENERIC);
		engine->RegisterGlobalFunction("void MoveCamera(float x, float y)", WRAP_FN(MoveCamera), asCALL_GENERIC);
		engine->RegisterGlobalFunction("void ZoomCamera(float val)", WRAP_FN(ZoomCamera), asCALL_GENERIC);
		engine->RegisterGlobalFunction("float GetCameraZoom()", WRAP_FN(GetCameraZoom), asCALL_GENERIC);

		engine->RegisterGlobalFunction("void OpenURL(string str)", WRAP_FN(OpenURL), asCALL_GENERIC);		
	}
	else
	{
		engine->RegisterObjectType("Rectangle", 0, asOBJ_REF);
		engine->RegisterObjectBehaviour("Rectangle", asBEHAVE_FACTORY, "Rectangle@ f()", asFUNCTION(reactanglefactory), asCALL_CDECL);
		engine->RegisterObjectBehaviour("Rectangle", asBEHAVE_FACTORY, "Rectangle@ f(float x, float y, float r, float b)", asFUNCTION(reactanglefactory2), asCALL_CDECL);
		engine->RegisterObjectBehaviour("Rectangle", asBEHAVE_ADDREF, "void f()", asMETHOD(ScriptRectangle,addref), asCALL_THISCALL);
		engine->RegisterObjectBehaviour("Rectangle", asBEHAVE_RELEASE, "void f()", asMETHOD(ScriptRectangle,release), asCALL_THISCALL);
		engine->RegisterObjectProperty("Rectangle", "float Left", asOFFSET(ScriptRectangle, x));
		engine->RegisterObjectProperty("Rectangle", "float Top", asOFFSET(ScriptRectangle, y));
		engine->RegisterObjectProperty("Rectangle", "float Right", asOFFSET(ScriptRectangle, r));
		engine->RegisterObjectProperty("Rectangle", "float Bottom", asOFFSET(ScriptRectangle, b));

		engine->RegisterObjectType("Point", 0, asOBJ_REF);
		engine->RegisterObjectBehaviour("Point", asBEHAVE_FACTORY, "Point@ f()", asFUNCTION(pointfactory), asCALL_CDECL);
		engine->RegisterObjectBehaviour("Point", asBEHAVE_FACTORY, "Point@ f(float x, float y)", asFUNCTION(pointfactory2), asCALL_CDECL);
		engine->RegisterObjectBehaviour("Point", asBEHAVE_ADDREF, "void f()", asMETHOD(ScriptPoint,addref), asCALL_THISCALL);
		engine->RegisterObjectBehaviour("Point", asBEHAVE_RELEASE, "void f()", asMETHOD(ScriptPoint,release), asCALL_THISCALL);
		engine->RegisterObjectProperty("Point", "float x", asOFFSET(ScriptPoint, x));
		engine->RegisterObjectProperty("Point", "float y", asOFFSET(ScriptPoint, y));

		engine->RegisterObjectType("Object", 0, asOBJ_REF);
		engine->RegisterObjectBehaviour("Object", asBEHAVE_FACTORY, "Object@ f()", asFUNCTION(objectfactory), asCALL_CDECL);
		engine->RegisterObjectBehaviour("Object", asBEHAVE_ADDREF, "void f()", asMETHOD(ScriptObject,addref), asCALL_THISCALL);
		engine->RegisterObjectBehaviour("Object", asBEHAVE_RELEASE, "void f()", asMETHOD(ScriptObject,release), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "Object@ opAssign(Object@ obj)", asMETHODPR(ScriptObject, operator=, (ScriptObject*), ScriptObject*), asCALL_THISCALL);

		engine->RegisterObjectMethod("Object", "uint GetID()", asMETHOD(ScriptObject, GetID), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void Show()", asMETHOD(ScriptObject, Show), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void Hide()", asMETHOD(ScriptObject, Hide), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void Enable()", asMETHOD(ScriptObject, Enable), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void Disable()", asMETHOD(ScriptObject, Disable), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void SetText(string)", asMETHOD(ScriptObject, SetText), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "string GetText()", asMETHOD(ScriptObject, GetText), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void SetColor(uint)", asMETHOD(ScriptObject, SetColor), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "uint GetColor()", asMETHOD(ScriptObject, GetColor), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void SetRotation(float)", asMETHOD(ScriptObject, SetRotation), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "float GetRotation()", asMETHOD(ScriptObject, GetRotation), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "int GetType()", asMETHOD(ScriptObject, GetType), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void QueueForDelete()", asMETHOD(ScriptObject, QueueForDelete), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void Draw()", asMETHOD(ScriptObject, Draw), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "uint GetFlags()", asMETHOD(ScriptObject, GetFlags), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void SetFlags(uint)", asMETHOD(ScriptObject, SetFlags), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "Rectangle@ GetObjectArea()", asMETHOD(ScriptObject, GetObjectArea), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "Rectangle@ GetAbsolutePosition()", asMETHOD(ScriptObject, GetAbsolutePosition), asCALL_THISCALL);

		engine->RegisterObjectMethod("Object", "void SetObjectArea(Rectangle@)", asMETHOD(ScriptObject, SetObjectArea), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void SetSprite(string name, int index, int animtype = 0, bool paused = false)", asMETHOD(ScriptObject, SetSprite), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void SetLayer(float)", asMETHOD(ScriptObject, SetLayer), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "float GetLayer()", asMETHOD(ScriptObject, GetLayer), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void SetValue(int)", asMETHOD(ScriptObject, SetValue), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "int GetValue()", asMETHOD(ScriptObject, GetValue), asCALL_THISCALL);

		engine->RegisterObjectMethod("Object", "void SetMax(float val)", asMETHOD(ScriptObject, SetMax), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "float GetMax()", asMETHOD(ScriptObject, GetMax), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void SetMin(float val)", asMETHOD(ScriptObject, SetMin), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "float GetMin()", asMETHOD(ScriptObject, GetMin), asCALL_THISCALL);

		engine->RegisterObjectMethod("Object", "void SetCapture()", asMETHOD(ScriptObject, SetCapture), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void ReleaseCapture()", asMETHOD(ScriptObject, ReleaseCapture), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "bool GetCapture()", asMETHOD(ScriptObject, GetCapture), asCALL_THISCALL);

		engine->RegisterObjectMethod("Object", "void StartTimer()", asMETHOD(ScriptObject, StartTimer), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void StopTimer()", asMETHOD(ScriptObject, StopTimer), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void PauseTimer()", asMETHOD(ScriptObject, PauseTimer), asCALL_THISCALL);

		engine->RegisterObjectMethod("Object", "void Play(int animtype)", asMETHOD(ScriptObject, Play), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void Play()", asMETHOD(ScriptObject, Play1), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void Pause()", asMETHOD(ScriptObject, Pause), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void Stop()", asMETHOD(ScriptObject, Stop), asCALL_THISCALL);

		engine->RegisterObjectMethod("Object", "void SetDelay(float delay)", asMETHOD(ScriptObject, SetDelay), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void SetSpeed(float speed)", asMETHOD(ScriptObject, SetSpeed), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void SetAnimatiomType(int type)", asMETHOD(ScriptObject, SetAnimatiomType), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void SetPlayType(int type)", asMETHOD(ScriptObject, SetPlayType), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void SetPingPong(bool val)", asMETHOD(ScriptObject, SetPingPong), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void SetPosAnim(Point@ start, Point@ end)", asMETHOD(ScriptObject, SetPosAnim), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void SetRotationAnim(float start, float end)", asMETHOD(ScriptObject, SetRotationAnim), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void SetColorAnim(uint start, uint end)", asMETHOD(ScriptObject, SetColorAnim), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void SetScaleAnim(float start, float end)", asMETHOD(ScriptObject, SetScaleAnim), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "float GetDelay()", asMETHOD(ScriptObject, GetDelay), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "float GetSpeed()", asMETHOD(ScriptObject, GetSpeed), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "int GetAnimatiomType()", asMETHOD(ScriptObject, GetAnimatiomType), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "int GetPlayType()", asMETHOD(ScriptObject, GetPlayType), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "bool GetPingPong()", asMETHOD(ScriptObject, GetPingPong), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "Point@ GetPosAnimStart()", asMETHOD(ScriptObject, GetPosAnimStart), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "Point@ GetPosAnimEnd()", asMETHOD(ScriptObject, GetPosAnimEnd), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "float GetRotationAnimStart()", asMETHOD(ScriptObject, GetRotationAnimStart), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "float GetRotationAnimEnd()", asMETHOD(ScriptObject, GetRotationAnimEnd), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "uint GetColorAnimStart()", asMETHOD(ScriptObject, GetColorAnimStart), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "uint GetColorAnimEnd()", asMETHOD(ScriptObject, GetColorAnimEnd), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "float GetScaleAnimStart()", asMETHOD(ScriptObject, GetScaleAnimStart), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "float GetScaleAnimEnd()", asMETHOD(ScriptObject, GetScaleAnimEnd), asCALL_THISCALL);		

		engine->RegisterObjectMethod("Object", "void PlaySprite()", asMETHOD(ScriptObject, PlaySprite), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void SetSpriteIndex(uint ind)", asMETHOD(ScriptObject, SetSpriteIndex), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void MirrorSpriteX(bool val)", asMETHOD(ScriptObject, MirrorSpriteX), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void MirrorSpriteY(bool val)", asMETHOD(ScriptObject, MirrorSpriteY), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void PauseSprite()", asMETHOD(ScriptObject, PauseSprite), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void StopSprite()", asMETHOD(ScriptObject, StopSprite), asCALL_THISCALL);	

		engine->RegisterObjectMethod("Object", "void EmitParticles(Point@)", asMETHOD(ScriptObject, EmitParticles1), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void EmitParticles(float x, float y, float size = 1)", asMETHOD(ScriptObject, EmitParticles2), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void AddEmitRect(Rectangle@, float life = 1)", asMETHOD(ScriptObject, AddEmitRect), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void AddEmitRect(float x, float y, float life, float size = 1)", asMETHOD(ScriptObject, AddEmitRect2), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void ClearEmitRect()", asMETHOD(ScriptObject, ClearEmitRect), asCALL_THISCALL);

		engine->RegisterObjectMethod("Object", "uint GetObjectType()", asMETHOD(ScriptObject, GetObjectType), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void SetObjectType(uint val)", asMETHOD(ScriptObject, SetObjectType), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "string GetObjectName()", asMETHOD(ScriptObject, GetObjectName), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void SetObjectName(string name)", asMETHOD(ScriptObject, SetObjectName), asCALL_THISCALL);

		engine->RegisterObjectMethod("Object", "void SetProperty(string name, float val)", asMETHOD(ScriptObject, SetProperty), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "float GetProperty(string name)", asMETHOD(ScriptObject, GetProperty), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void ClearProperties()", asMETHOD(ScriptObject, ClearProperties), asCALL_THISCALL);

		engine->RegisterObjectMethod("Object", "bool isPlaying()", asMETHOD(ScriptObject, isPlaying), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void SetSoundResource(string name)", asMETHOD(ScriptObject, SetSoundResource), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void SetVolume(int val)", asMETHOD(ScriptObject, SetVolume), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "int GetVolume()", asMETHOD(ScriptObject, GetVolume), asCALL_THISCALL);

#ifdef USEBOX2DPHYSICS
		engine->RegisterObjectMethod("Object", "int16 GetTeam()", asMETHOD(ScriptObject, GetTeam), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "int16 GetTeamMask()", asMETHOD(ScriptObject, GetTeamMask), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void SetTeam(int16 val, int16 mask)", asMETHOD(ScriptObject, SetTeam), asCALL_THISCALL);

		engine->RegisterObjectMethod("Object", "void SetSensor(bool val)", asMETHOD(ScriptObject, SetSensor), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "bool GetSensor()", asMETHOD(ScriptObject, GetSensor), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void SetBodyType(int val)", asMETHOD(ScriptObject, SetBodyType), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "int GetBodyType()", asMETHOD(ScriptObject, GetBodyType), asCALL_THISCALL);

		engine->RegisterObjectMethod("Object", "void SetVelocity(Point@ vel)", asMETHOD(ScriptObject, SetVelocity), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "Point@ GetVelocity()", asMETHOD(ScriptObject, GetVelocity), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void ApplyForce(Point@ vel)", asMETHOD(ScriptObject, ApplyForce), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void ApplyImpulse(Point@ vel)", asMETHOD(ScriptObject, ApplyImpulse), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void ApplyTorque(float val)", asMETHOD(ScriptObject, ApplyTorque), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void ApplyAngularImpulse(float vel)", asMETHOD(ScriptObject, ApplyAngImpulse), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void SetTransform(Point@ vel)", asMETHOD(ScriptObject, SetTransform), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "Point@ GetTransform()", asMETHOD(ScriptObject, GetTransform), asCALL_THISCALL);

		engine->RegisterObjectMethod("Object", "void SetRestitution(float val)", asMETHOD(ScriptObject, SetRestitution), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void SetFriction(float val)", asMETHOD(ScriptObject, SetFriction), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "float GetRestitution()", asMETHOD(ScriptObject, GetRestitution), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "float GetFriction()", asMETHOD(ScriptObject, GetFriction), asCALL_THISCALL);

		engine->RegisterObjectMethod("Object", "void SetBodyShape(int val)", asMETHOD(ScriptObject, SetBodyShape), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "int GetBodyShape()", asMETHOD(ScriptObject, GetBodyShape), asCALL_THISCALL);

		engine->RegisterObjectMethod("Object", "void SetPhyAngle(float val)", asMETHOD(ScriptObject, SetPhyAngle), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "float GetPhyAngle()", asMETHOD(ScriptObject, GetPhyAngle), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void SetGravityScale(float val)", asMETHOD(ScriptObject, SetGravityScale), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void SetAngularDamping(float val)", asMETHOD(ScriptObject, SetAngularDamping), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void SetLinearDamping(float val)", asMETHOD(ScriptObject, SetLinearDamping), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "float GetGravityScale()", asMETHOD(ScriptObject, GetGravityScale), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "float GetAngularDamping()", asMETHOD(ScriptObject, GetAngularDamping), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "float GetLinearDamping()", asMETHOD(ScriptObject, GetLinearDamping), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void LockRotation(bool val)", asMETHOD(ScriptObject, LockRotation), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void SetDensity(float val)", asMETHOD(ScriptObject, SetDensity), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "float GetDensity()", asMETHOD(ScriptObject, GetDensity), asCALL_THISCALL);

		engine->RegisterObjectMethod("Object", "void SetLimits(float limit1, float limit2)", asMETHOD(ScriptObject, SetLimits), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void EnableLimits(bool val)", asMETHOD(ScriptObject, EnableLimits), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void EnableMotor(bool val)", asMETHOD(ScriptObject, EnableMotor), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void SetMotorSpeed(bool val)", asMETHOD(ScriptObject, SetMotorSpeed), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void SetMotorPower(bool val)", asMETHOD(ScriptObject, SetMotorPower), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void SetDampingRatio(float val)", asMETHOD(ScriptObject, SetDampingRatio), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void SetFrequency(float val)", asMETHOD(ScriptObject, SetFrequency), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "bool IsLimitEnabled()", asMETHOD(ScriptObject, IsLimitEnabled), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "bool IsMotorEnabled()", asMETHOD(ScriptObject, IsMotorEnabled), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "float GetLimit1()", asMETHOD(ScriptObject, GetLimit1), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "float GetLimit2()", asMETHOD(ScriptObject, GetLimit2), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "float GetMotorPower()", asMETHOD(ScriptObject, GetMotorPower), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "float GetMootorSpeed()", asMETHOD(ScriptObject, GetMootorSpeed), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "float GetDampingRatio()", asMETHOD(ScriptObject, GetDampingRatio), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "float GetFrequency()", asMETHOD(ScriptObject, GetFrequency), asCALL_THISCALL);

		engine->RegisterObjectMethod("Object", "bool IsFixedRotation()", asMETHOD(ScriptObject, isFixedRotation), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "void SetAngularVelocity(float val)", asMETHOD(ScriptObject, SetAngularVelocity), asCALL_THISCALL);
		engine->RegisterObjectMethod("Object", "float GetAngularVelocity()", asMETHOD(ScriptObject, GetAngularVelocity), asCALL_THISCALL);

		engine->RegisterGlobalFunction("void SetGravity(Point@)", asFUNCTION(SetGravity), asCALL_CDECL);
		engine->RegisterGlobalFunction("Point@ GetGravity()", asFUNCTION(GetGravity), asCALL_CDECL);
#endif

		engine->RegisterGlobalFunction("Point@ RelativePoint(Point@)", asFUNCTION(RelativePoint), asCALL_CDECL);
		engine->RegisterGlobalFunction("Point@ GetMousePoint()", asFUNCTION(GetMousePoint), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetStars(int8)", asFUNCTION(SetStars), asCALL_CDECL);
		engine->RegisterGlobalFunction("int8 GetStars()", asFUNCTION(GetStars), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetXPGain(int)", asFUNCTION(SetXPGain), asCALL_CDECL);
		engine->RegisterGlobalFunction("int GetXPGain()", asFUNCTION(GetXPGain), asCALL_CDECL);
		engine->RegisterGlobalFunction("void IncreaseXPGain(int)", asFUNCTION(IncreaseXPGain), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetXP(int)", asFUNCTION(SetXP), asCALL_CDECL);
		engine->RegisterGlobalFunction("int GetXP()", asFUNCTION(GetXP), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetLevels(uint)", asFUNCTION(SetLevels), asCALL_CDECL);
		engine->RegisterGlobalFunction("uint GetLevels()", asFUNCTION(GetLevels), asCALL_CDECL);
		engine->RegisterGlobalFunction("Object@ GetObject(uint)", asFUNCTION(GetObjectByID), asCALL_CDECL);
		engine->RegisterGlobalFunction("void Win()", asFUNCTION(Win), asCALL_CDECL);
		engine->RegisterGlobalFunction("void Loose()", asFUNCTION(Loose), asCALL_CDECL);
		engine->RegisterGlobalFunction("void TogglePause()", asFUNCTION(TogglePause), asCALL_CDECL);
		engine->RegisterGlobalFunction("void ResetData()", asFUNCTION(ResetData), asCALL_CDECL);
		engine->RegisterGlobalFunction("void Pause()", asFUNCTION(Pause), asCALL_CDECL);
		engine->RegisterGlobalFunction("void Unpause()", asFUNCTION(Unpause), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool LoadFile(string filename, uint resgroup = 0, bool lvlbutton = false, int clvl = 0, int ulvl = 0, string snd  = \"\", string script = \"Script.as\", string win = \"win.cfg\")", asFUNCTION(LoadFile), asCALL_CDECL);
		engine->RegisterGlobalFunction("void Exit()", asFUNCTION(Exit), asCALL_CDECL);
		engine->RegisterGlobalFunction("void PlayMusic(string name, bool loop = true)", asFUNCTION(PlayMusic), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetMusicVolume(int val)", asFUNCTION(SetMusicVolume), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetSoundVolume(int val)", asFUNCTION(SetSoundVolume), asCALL_CDECL);
		engine->RegisterGlobalFunction("int GetMusicVolume()", asFUNCTION(GetMusicVolume), asCALL_CDECL);
		engine->RegisterGlobalFunction("int GetSoundVolume()", asFUNCTION(GetSoundVolume), asCALL_CDECL);
		engine->RegisterGlobalFunction("void CreateSprite(Rectangle@, string name, int index, int animtype = 0, bool paused = false)", asFUNCTION(CreateSprite), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetCurrentLevel(int)", asFUNCTION(SetCurrentLevel), asCALL_CDECL);
		engine->RegisterGlobalFunction("int GetCurrentLevel()", asFUNCTION(GetCurrentLevel), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SaveSettings()", asFUNCTION(SaveSettings), asCALL_CDECL);
		engine->RegisterGlobalFunction("uint GetMainID()", asFUNCTION(GetMainID), asCALL_CDECL);
		engine->RegisterGlobalFunction("Object@ GetBackground()", asFUNCTION(GetBackground), asCALL_CDECL);
		engine->RegisterGlobalFunction("Object@ CreateObject(string name, uint ID, Rectangle@ rct)", asFUNCTION(CreateObject), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool SaveFile(string filename)", asFUNCTION(SaveFile), asCALL_CDECL);

		engine->RegisterGlobalFunction("void Print(string val)", asFUNCTION(CPrint), asCALL_CDECL);

		engine->RegisterGlobalFunction("int GetStoreInt(string str)", asFUNCTION(GetStoreInt), asCALL_CDECL);
		engine->RegisterGlobalFunction("float GetStoreFloat(string str)", asFUNCTION(GetStoreFloat), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetStoreInt(string str, int val)", asFUNCTION(SetStoreInt), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetStoreFloat(string str, float val)", asFUNCTION(SetStoreFloat), asCALL_CDECL);
		engine->RegisterGlobalFunction("void EraseStoreInt(string str)", asFUNCTION(EraseStoreInt), asCALL_CDECL);
		engine->RegisterGlobalFunction("void EraseStoreFloat(string str)", asFUNCTION(EraseStoreFloat), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool ExistsStoreInt(string str)", asFUNCTION(ExistsStoreInt), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool ExistsStoreFloat(string str)", asFUNCTION(ExistsStoreFloat), asCALL_CDECL);

		engine->RegisterGlobalFunction("float GetTimeDelta()", asFUNCTION(GetTimeDelta), asCALL_CDECL);
		engine->RegisterGlobalFunction("Point@ GetDirection(Point@ val, Point@ val2)", asFUNCTION(GetDirection), asCALL_CDECL);
		engine->RegisterGlobalFunction("float GetAngle(Point@ val)", asFUNCTION(GetAngle), asCALL_CDECL);
		engine->RegisterGlobalFunction("float GetDistance(Point@ val)", asFUNCTION(GetDistance), asCALL_CDECL);
		engine->RegisterGlobalFunction("float GetSquaredDistance(Point@ val)", asFUNCTION(GetSquaredDistance), asCALL_CDECL);
		engine->RegisterGlobalFunction("float GetDistance(Point@ val, Point@ val1)", asFUNCTION(GetDistance2), asCALL_CDECL);
		engine->RegisterGlobalFunction("float GetSquaredDistance(Point@ val, Point@ val1)", asFUNCTION(GetSquaredDistance2), asCALL_CDECL);
		engine->RegisterGlobalFunction("void FocusCamera(Object@ obj, float offx = 0, float offy = 0)", asFUNCTION(FocusCamera), asCALL_CDECL);
		engine->RegisterGlobalFunction("void FocusCamera(float x, float y, float offx = 0, float offy = 0)", asFUNCTION(FocusCamera2), asCALL_CDECL);
		engine->RegisterGlobalFunction("void MoveCamera(float x, float y)", asFUNCTION(MoveCamera), asCALL_CDECL);
		engine->RegisterGlobalFunction("void ZoomCamera(float val)", asFUNCTION(ZoomCamera), asCALL_CDECL);
		engine->RegisterGlobalFunction("float GetCameraZoom()", asFUNCTION(GetCameraZoom), asCALL_CDECL);

		engine->RegisterGlobalFunction("void OpenURL(string str)", asFUNCTION(OpenURL), asCALL_CDECL);
	}

	mainfn = engine->CreateContext();
	callbackfn = engine->CreateContext();
}

asIScriptEngine* Script::GetEngine()
{
	return engine;
}

void Script::Callback (Object* obj, unsigned int Msg, unsigned int Param1, void* Param2)
{
	//if(Msg == OBJECT_Message_MouseDown)
	//{
	//	PointI* pt = (PointI*)Param2;
	//	RectangleF pos = ((DrawableObject*)obj)->GetObjectArea();
	//	if(Game->GetMainContainer()!=obj) Game->GetMainContainer()->FocusCameraOn(PointF(pos.left + (pos.right - pos.left) / 2.0f, pos.top + (pos.bottom - pos.top) / 2.0f), PointF(100, 240));
	//}

	if(callbackfn==0 || funid == 0) return;

	if(Msg==OBJECT_Message_MouseDown || Msg==OBJECT_Message_MouseUp || Msg==OBJECT_Message_MouseMove)
	{
		m_clkpoint = *((PointI*)(Param2));
	}

	sobj.obj = obj;

	callbackfn->Unprepare();
	callbackfn->Prepare(funid);

	callbackfn->SetArgObject(0, &sobj);
	callbackfn->SetArgDWord(1, Msg);
	callbackfn->SetArgDWord(2, Param1);
	callbackfn->Execute();
}

bool Script::Compile(const char* fname, bool execute)
{
	m_curcode = fname;
	errors.clear();

	asIScriptModule* mod = engine->GetModule("mod", asGM_ALWAYS_CREATE);

	string str2;
	stringstream sstr;
	sstr << "const int KEY_BACK = ";
	sstr << KEY_BACK;
	sstr << ";";
	sstr << "const int KEY_ESC = ";
	sstr << KEY_ESCAPE;
	sstr << ";";

	AddConstant(sstr, KEY_CONTROL);
	AddConstant(sstr, KEY_ENTER);
	AddConstant(sstr, KEY_HOME);
	AddConstant(sstr, KEY_END);

	AddConstant(sstr, KEY_LEFT);
	AddConstant(sstr, KEY_RIGHT);
	AddConstant(sstr, KEY_UP);
	AddConstant(sstr, KEY_DOWN);
	AddConstant(sstr, KEY_F1);
	AddConstant(sstr, KEY_F2);
	AddConstant(sstr, KEY_F3);
	AddConstant(sstr, KEY_F4);
	AddConstant(sstr, KEY_F5);
	AddConstant(sstr, KEY_F6);
	AddConstant(sstr, KEY_F7);
	AddConstant(sstr, KEY_F8);
	AddConstant(sstr, KEY_F9);
	AddConstant(sstr, KEY_F10);
	AddConstant(sstr, KEY_F11);
	AddConstant(sstr, KEY_F12);
	AddConstant(sstr, KEY_INSERT);
	AddConstant(sstr, KEY_PRINTSCR);
	AddConstant(sstr, KEY_DELETE);
	AddConstant(sstr, KEY_TAB);
	
	AddConstant(sstr, OBJECT_Message_BMouseNormal);
	AddConstant(sstr, OBJECT_Message_BMouseOver);
	AddConstant(sstr, OBJECT_Message_BSetChecked);
	AddConstant(sstr, OBJECT_Message_CSelectionChanged);
	AddConstant(sstr, OBJECT_Message_Draw);
	AddConstant(sstr, OBJECT_Message_EDTextChange);
	AddConstant(sstr, OBJECT_Message_GameLoose);
	AddConstant(sstr, OBJECT_Message_GameWin);
	AddConstant(sstr, OBJECT_Message_GameStep);
	AddConstant(sstr, OBJECT_Message_Init);
	AddConstant(sstr, OBJECT_Message_KeyDown);
	AddConstant(sstr, OBJECT_Message_KeyState);
	AddConstant(sstr, OBJECT_Message_KeyUp);
	AddConstant(sstr, OBJECT_Message_LMouseClick);
	AddConstant(sstr, OBJECT_Message_LMouseDown);
	AddConstant(sstr, OBJECT_Message_LMouseUp);
	AddConstant(sstr, OBJECT_Message_MouseDown);
	AddConstant(sstr, OBJECT_Message_MouseMove);
	AddConstant(sstr, OBJECT_Message_MouseUp);
	AddConstant(sstr, OBJECT_Message_MouseScroll);
	AddConstant(sstr, OBJECT_Message_OnTimer);
	AddConstant(sstr, OBJECT_Message_PValueChanged);
	AddConstant(sstr, OBJECT_Message_Release);
	AddConstant(sstr, OBJECT_Message_Resize);
	AddConstant(sstr, OBJECT_Message_RMouseDown);
	AddConstant(sstr, OBJECT_Message_RMouseUp);
	AddConstant(sstr, OBJECT_Message_SelClear);
	AddConstant(sstr, OBJECT_Message_SelEdit);
	AddConstant(sstr, OBJECT_Message_SpriteFinished);
	AddConstant(sstr, OBJECT_Message_Collision);
	AddConstant(sstr, OBJECT_Message_Update);
	AddConstant(sstr, OBJECT_Message_TileClicked);
	AddConstant(sstr, OBJECT_Message_EndContact);

	AddConstant(sstr, COLOR_BLACK);
	AddConstant(sstr, COLOR_WHITE);
	AddConstant(sstr, COLOR_GREEN);
	AddConstant(sstr, COLOR_BLUE);
	AddConstant(sstr, COLOR_RED);
	AddConstant(sstr, COLOR_GRAY);
	AddConstant(sstr, COLOR_AQUA);
	AddConstant(sstr, COLOR_YELLOW);
	AddConstant(sstr, COLOR_CYAN);
	RegisterConstants(sstr);

	str2 = sstr.str();

	int r = mod->AddScriptSection("scriptg", str2.c_str(), str2.size());
	if(r<0)
	{
		return false;
	}

	r = mod->AddScriptSection("script", m_curcode.c_str(), m_curcode.size());
	if(r<0)
	{
		return false;
	}

#ifdef WIN32
		engine->SetMessageCallback(asFUNCTION(MessageCallback), this, asCALL_CDECL);
#endif
	r = mod->Build();

	if(r<0)
	{
		return false;
	}

	if(mainfn==0) return false;

	fid = engine->GetModule("mod")->GetFunctionByName("main");
	if(fid<0)
	{
		return false;
	}

	mainfn->Unprepare();

	r=mainfn->Prepare(fid);
	if(r<0)
	{
		return false;
	}

	if(callbackfn==0) return false;

	funid = engine->GetModule("mod")->GetFunctionByName("callback");
	if(funid<0)
	{
		return false;
	}

	callbackfn->Unprepare();

	r=callbackfn->Prepare(funid);
	if(r<0)
	{
		return false;
	}

	if(execute) Execute();

	return true;
}

asIScriptModule* Script::GetModule()
{
	return engine->GetModule("mod");
}

bool Script::Open(const wchar_t* fname)
{
	char* tmp = new char[4096];
	//fstream file(fname, ios_base::in);
	WideCharToMultiByte(tmp, fname, 4096);
	vector<string> txts;
	GetStringArray(txts, tmp, '.');

	ZipFile file;

	if(!file.Open(fname))
	{
		delete [] tmp;
		return false;
	}

	m_curcode = "";
	unsigned int sz = 0;
	bool compiledfile = false;

	if(txts.size()>1)
	{
		string ext = txts[txts.size()-1];

		if(iequals(ext, "asc"))
		{
			compiledfile = true;
		}
		else
		{
			while(1)
			{
				sz = 4095;
				if(!file.Read((void*)tmp, &sz))
				{
					break;
				}
				tmp[sz] = '\0';
				m_curcode += tmp;
			}
			file.Close();
			delete [] tmp;
		}
	}
	else
	{
		while(1)
		{
			sz = 4095;
			if(!file.Read((void*)tmp, &sz))
			{
				break;
			}
			tmp[sz] = '\0';
			m_curcode += tmp;
		}
		file.Close();
		delete [] tmp;
	}

	errors.clear();

	asIScriptModule* mod = engine->GetModule("mod", asGM_ALWAYS_CREATE);

#ifdef __AVM2__
	mod->Build();
#endif

	if(!compiledfile)
	{
		string str2;
		stringstream sstr;
		sstr << "const int KEY_BACK = ";
		sstr << KEY_BACK;
		sstr << ";";
		sstr << "const int KEY_ESC = ";
		sstr << KEY_ESCAPE;
		sstr << ";";

		AddConstant(sstr, KEY_CONTROL);
		AddConstant(sstr, KEY_ENTER);
		AddConstant(sstr, KEY_HOME);
		AddConstant(sstr, KEY_END);

		AddConstant(sstr, KEY_LEFT);
		AddConstant(sstr, KEY_RIGHT);
		AddConstant(sstr, KEY_UP);
		AddConstant(sstr, KEY_DOWN);
		AddConstant(sstr, KEY_F1);
		AddConstant(sstr, KEY_F2);
		AddConstant(sstr, KEY_F3);
		AddConstant(sstr, KEY_F4);
		AddConstant(sstr, KEY_F5);
		AddConstant(sstr, KEY_F6);
		AddConstant(sstr, KEY_F7);
		AddConstant(sstr, KEY_F8);
		AddConstant(sstr, KEY_F9);
		AddConstant(sstr, KEY_F10);
		AddConstant(sstr, KEY_F11);
		AddConstant(sstr, KEY_F12);
		AddConstant(sstr, KEY_INSERT);
		AddConstant(sstr, KEY_PRINTSCR);
		AddConstant(sstr, KEY_DELETE);
		AddConstant(sstr, KEY_TAB);

		AddConstant(sstr, OBJECT_Message_BMouseNormal);
		AddConstant(sstr, OBJECT_Message_BMouseOver);
		AddConstant(sstr, OBJECT_Message_BSetChecked);
		AddConstant(sstr, OBJECT_Message_CSelectionChanged);
		AddConstant(sstr, OBJECT_Message_Draw);
		AddConstant(sstr, OBJECT_Message_EDTextChange);
		AddConstant(sstr, OBJECT_Message_GameLoose);
		AddConstant(sstr, OBJECT_Message_GameWin);
		AddConstant(sstr, OBJECT_Message_GameStep);
		AddConstant(sstr, OBJECT_Message_Init);
		AddConstant(sstr, OBJECT_Message_KeyDown);
		AddConstant(sstr, OBJECT_Message_KeyState);
		AddConstant(sstr, OBJECT_Message_KeyUp);
		AddConstant(sstr, OBJECT_Message_LMouseClick);
		AddConstant(sstr, OBJECT_Message_LMouseDown);
		AddConstant(sstr, OBJECT_Message_LMouseUp);
		AddConstant(sstr, OBJECT_Message_MouseDown);
		AddConstant(sstr, OBJECT_Message_MouseMove);
		AddConstant(sstr, OBJECT_Message_MouseUp);
		AddConstant(sstr, OBJECT_Message_MouseScroll);
		AddConstant(sstr, OBJECT_Message_OnTimer);
		AddConstant(sstr, OBJECT_Message_PValueChanged);
		AddConstant(sstr, OBJECT_Message_Release);
		AddConstant(sstr, OBJECT_Message_Resize);
		AddConstant(sstr, OBJECT_Message_RMouseDown);
		AddConstant(sstr, OBJECT_Message_RMouseUp);
		AddConstant(sstr, OBJECT_Message_SelClear);
		AddConstant(sstr, OBJECT_Message_SelEdit);
		AddConstant(sstr, OBJECT_Message_SpriteFinished);
		AddConstant(sstr, OBJECT_Message_Collision);
		AddConstant(sstr, OBJECT_Message_Update);
		AddConstant(sstr, OBJECT_Message_TileClicked);
		AddConstant(sstr, OBJECT_Message_EndContact);

		AddConstant(sstr, COLOR_BLACK);
		AddConstant(sstr, COLOR_WHITE);
		AddConstant(sstr, COLOR_GREEN);
		AddConstant(sstr, COLOR_BLUE);
		AddConstant(sstr, COLOR_RED);
		AddConstant(sstr, COLOR_GRAY);
		AddConstant(sstr, COLOR_AQUA);
		AddConstant(sstr, COLOR_YELLOW);
		AddConstant(sstr, COLOR_CYAN);

		RegisterConstants(sstr);

		str2 = sstr.str();

		int r = mod->AddScriptSection("scriptg", str2.c_str(), str2.size());
		if(r<0)
		{
			return false;
		}

		r = mod->AddScriptSection("script", m_curcode.c_str(), m_curcode.size());
		if(r<0)
		{
			return false;
		}

#ifdef WIN32
		engine->SetMessageCallback(asFUNCTION(MessageCallback), this, asCALL_CDECL);
#endif
		r = mod->Build();

		if(r<0)
		{
			return false;
		}
	}
	else
	{
		BinaryStream bs(&file);
#ifdef WIN32
		engine->SetMessageCallback(asFUNCTION(MessageCallback), this, asCALL_CDECL);
#endif
		int r = mod->LoadByteCode(&bs);
		file.Close();
		delete [] tmp;
		if(r<0)
		{
			return false;
		}
	}

	if(mainfn==0) return false;

	fid = engine->GetModule("mod")->GetFunctionByName("main");
	if(fid<0)
	{
		return false;
	}
	
	mainfn->Unprepare();

	int r=mainfn->Prepare(fid);
	if(r<0)
	{
		return false;
	}

	if(callbackfn==0) return false;

	funid = engine->GetModule("mod")->GetFunctionByName("callback");
	if(funid<0)
	{
		return false;
	}

	callbackfn->Unprepare();

	r=callbackfn->Prepare(funid);
	if(r<0)
	{
		return false;
	}

	Execute();

	return true;
}

string Script::GetFunctionNames(set<string, icompstrs>& arr)
{
	asUINT count = engine->GetGlobalFunctionCount();
	string ret;
	arr.clear();

	for (unsigned int i = 0; i < count; i++)
	{
		if(i!=0) ret += " ";
		asIScriptFunction* fun = engine->GetGlobalFunctionByIndex(i);
		ret += fun->GetName();
	}
	if(count>0) ret += " ";

	count = engine->GetGlobalPropertyCount();
	for (unsigned int i = 0; i < count; i++)
	{
		if(i!=0) ret += " ";
		const char* name;
		engine->GetGlobalPropertyByIndex(i, &name);
		ret += name;
	}
	if(count>0) ret += " ";

	count = engine->GetObjectTypeCount();
	for (unsigned int i = 0; i < count; i++)
	{
		asIObjectType* fun = engine->GetObjectTypeByIndex(i);

		if(i!=0) ret += " ";
		ret += fun->GetName();

		asUINT count2 = fun->GetMethodCount();
		for (unsigned int i2 = 0; i2 < count2; i2++)
		{
			asIScriptFunction* fun2 = fun->GetMethodByIndex(i2);
			string str;
			str = fun2->GetName();
			arr.insert(str); 
		}

		count2 = fun->GetPropertyCount();
		for (unsigned int i2 = 0; i2 < count2; i2++)
		{
			const char* name;
			fun->GetProperty(i2, &name);
			arr.insert(name); 
		}
	}
	if(count>0) ret += " ";

	asIScriptModule* mod = engine->GetModule("mod", asGM_ONLY_IF_EXISTS);
	if(mod!=0)
	{
		count = mod->GetFunctionCount();
		for (unsigned int i = 0; i < count; i++)
		{
			if(i!=0) ret += " ";
			asIScriptFunction* fun = mod->GetFunctionByIndex(i);
			ret += fun->GetName();
		}
		if(count>0) ret += " ";

		count = mod->GetGlobalVarCount();
		for (unsigned int i = 0; i < count; i++)
		{
			if(i!=0) ret += " ";
			const char* name;
			mod->GetGlobalVar(i, &name);
			ret += name;
		}
		if(count>0) ret += " ";

		count = mod->GetObjectTypeCount();
		for (unsigned int i = 0; i < count; i++)
		{
			asIObjectType* fun = mod->GetObjectTypeByIndex(i);

			if(i!=0) ret += " ";
			ret += fun->GetName();

			asUINT count2 = fun->GetMethodCount();
			for (unsigned int i2 = 0; i2 < count2; i2++)
			{
				asIScriptFunction* fun2 = fun->GetMethodByIndex(i2);
				string str;
				str = fun2->GetName();
				arr.insert(str); 
			}

			count2 = fun->GetPropertyCount();
			for (unsigned int i2 = 0; i2 < count2; i2++)
			{
				const char* name;
				fun->GetProperty(i2, &name);
				arr.insert(name); 
			}
		}
	}
	return ret;
}

string Script::GetClassNames()
{
	asUINT count = engine->GetGlobalFunctionCount();
	string ret;

	count = engine->GetObjectTypeCount();
	for (unsigned int i = 0; i < count; i++)
	{
		if(i!=0) ret += " ";
		asIObjectType* fun = engine->GetObjectTypeByIndex(i);
		ret += fun->GetName();
	}
	if(count>0) ret += " ";

	asIScriptModule* mod = engine->GetModule("mod", asGM_ONLY_IF_EXISTS);
	if(mod!=0)
	{
		count = mod->GetObjectTypeCount();
		for (unsigned int i = 0; i < count; i++)
		{
			if(i!=0) ret += " ";
			asIObjectType* fun = mod->GetObjectTypeByIndex(i);
			ret += fun->GetName();
		}
	}
	return ret;
}

void Script::PopulateWithHelp(map<string, string>& m_help)
{
	asUINT count = engine->GetGlobalFunctionCount();
	m_help.clear();

	for (unsigned int i = 0; i < count; i++)
	{
		asIScriptFunction* fun = engine->GetGlobalFunctionByIndex(i);
		m_help[fun->GetName()] = fun->GetDeclaration();
	}

	count = engine->GetObjectTypeCount();
	for (unsigned int i = 0; i < count; i++)
	{
		asIObjectType* fun = engine->GetObjectTypeByIndex(i);

		asUINT count2 = fun->GetMethodCount();
		for (unsigned int i2 = 0; i2 < count2; i2++)
		{
			asIScriptFunction* fun2 = fun->GetMethodByIndex(i2);
			m_help[fun2->GetName()] = fun2->GetDeclaration();
		}
	}

	asIScriptModule* mod = engine->GetModule("mod", asGM_ONLY_IF_EXISTS);
	if(mod!=0)
	{
		count = mod->GetFunctionCount();
		for (unsigned int i = 0; i < count; i++)
		{
			asIScriptFunction* fun = mod->GetFunctionByIndex(i);
			m_help[fun->GetName()] = fun->GetDeclaration();
		}

		count = mod->GetObjectTypeCount();
		for (unsigned int i = 0; i < count; i++)
		{
			asIObjectType* fun = mod->GetObjectTypeByIndex(i);

			asUINT count2 = fun->GetMethodCount();
			for (unsigned int i2 = 0; i2 < count2; i2++)
			{
				asIScriptFunction* fun2 = fun->GetMethodByIndex(i2);
				m_help[fun2->GetName()] = fun2->GetDeclaration();
			}
		}
	}
}

bool Script::Execute()
{
	if(mainfn==0 || fid==0) return false;
	mainfn->Unprepare();

	int r=mainfn->Prepare(fid);
	if(r<0)
	{
		return false;
	}
	r = mainfn->Execute();
	if(r!=asEXECUTION_FINISHED)
	{
		return false;
	}
	return true;
}

Script::~Script()
{
	if(callbackfn!=0) callbackfn->Release();
	if(mainfn!=0) mainfn->Release();
	if(engine!=0) engine->Release();
}

//IO
BinaryStream::BinaryStream(BaseIO* bio)
{
	m_io = bio;
}

void BinaryStream::Read(void *ptr, asUINT size)
{
	m_io->Read(ptr, &size);
}

void BinaryStream::Write(const void *ptr, asUINT size)
{
	m_io->Write(ptr, &size);
}
//
#endif