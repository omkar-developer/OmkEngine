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

void ObjectBuilder::RegisterBuilder(const char* name, Object* obj)
{
	if(m_type_list.find(name)!=m_type_list.end())
	{
		delete m_type_list[name];
	}
	m_type_list[name] = obj;
}

bool ObjectBuilder::RemoveBuilder(const char* name)
{
	map<string, Object*>::iterator it = m_type_list.find(name);
	if(it!=m_type_list.end())
	{
		delete it->second;
		m_type_list.erase(it);
		return true;
	}
	return false;
}
	
void ObjectBuilder::ClearBuilders()
{
	for(map<string, Object*>::iterator it = m_type_list.begin(); it!=m_type_list.end(); it++)
	{
		delete it->second;
	}
	m_type_list.clear();
}

void TransformVertex2DInv(RectangleF& pt)
{
	ObjectsContainer* m_parent = Game->GetMainContainer();

	if(m_parent!=0)
	{
		//Transform
		pt.left += m_parent->GetCamera().m_pos.left;
		pt.top += m_parent->GetCamera().m_pos.top;
		pt.right += m_parent->GetCamera().m_pos.left;
		pt.bottom += m_parent->GetCamera().m_pos.top;
	}
}

Object* ObjectBuilder::BuildObject(const char* name)
{
	map<string, Object*>::iterator it = m_type_list.find(name);
	if(it!=m_type_list.end())
	{
		if(it->second)
		{
			Object* obj = it->second->Clone();
			if(obj!=0) 
			{
				obj->name = name;
				obj->SetID(AutoID);

#ifdef USEBOX2DPHYSICS
				if(obj->GetType()&OBJECT_TYPE_PHYOBJECT)
				{
					RectangleF oarea = ((PhyObject*)obj)->GetObjectArea();
					TransformVertex2DInv(oarea);
					((PhyObject*)obj)->SetObjectArea(oarea);
				}
#endif
				++AutoID;
			}
			return obj;
		}
	}
	return 0;
}

ObjectBuilder::ObjectBuilder()
{
	AutoID = 100;
}

ObjectBuilder::~ObjectBuilder()
{
	ClearBuilders();
}

void ObjectBuilder::GetBuildersList(vector<string>& lst)
{
	lst.clear();
	for(map<string, Object*>::iterator it = m_type_list.begin(); it!=m_type_list.end(); it++)
	{
		lst.push_back(it->first);
	}
}

void ObjectBuilder::RegisterDefaultBuilders(ResourceManager* m_mgr, C_Renderer* renderer)
{
	RegisterBuilder("Drawable Object", new DrawableObject());
	RegisterBuilder("AnimableObject", new AnimableObject());
	RegisterBuilder("Objects Container", new ObjectsContainer(m_mgr));
	RegisterBuilder("Text", new GUIText(m_mgr));
	RegisterBuilder("Button", new GUIButton(m_mgr));
	GUIButton* btn = new GUIButton(m_mgr);
	btn->m_flags |= OBJECT_Flag_BTypeCheck | OBJECT_Flag_NoBackground;
	RectangleF area = btn->GetObjectArea();
	area.bottom = area.top + 40; 
	area.right = area.left + 200;
	btn->SetObjectArea (area);
	btn->SetCheckBoxSize(40);
	RegisterBuilder("CheckBox", btn);

	btn = new GUIButton(m_mgr);
	btn->m_flags |= OBJECT_Flag_BTypeRadio | OBJECT_Flag_NoBackground;
	area = btn->GetObjectArea();
	area.bottom = area.top + 40; 
	area.right = area.left + 200;
	btn->SetObjectArea (area);
	btn->SetCheckBoxSize(40);
	RegisterBuilder("Radio button", btn);

	btn = new GUIButton(m_mgr);
	btn->m_flags |= OBJECT_Flag_BTypePush;
	area = btn->GetObjectArea();
	area.bottom = area.top + 40; 
	area.right = area.left + 200;
	btn->SetObjectArea (area);
	RegisterBuilder("Push button", btn);

	RegisterBuilder("EditBox", new GUIEdit(renderer, m_mgr));

	GUIProgress* prog = new GUIProgress(m_mgr);
	RegisterBuilder("Progressbar", prog);

	GUILoadProgress* prog2 = new GUILoadProgress(m_mgr);
	RegisterBuilder("LoadProgress", prog2);

	prog = new GUIProgress(m_mgr);
	prog->m_flags |= OBJECT_Flag_PTypeSlider;
	RegisterBuilder("Slider", prog);

	prog = new GUIProgress(m_mgr);
	prog->m_flags |= OBJECT_Flag_PTypeSlider | OBJECT_Flag_PTypeVertical;
	RegisterBuilder("SliderV", prog);

	RegisterBuilder("Circle Menu", new GUICircleMenu(m_mgr));
	RegisterBuilder("Particle Engine", new ParticleEngine());
	RegisterBuilder("Timer", new TimerObject());
	RegisterBuilder("Tiles", new Tiles());
	RegisterBuilder("Animation Timeline", new AnimationTimeline(m_mgr, 10));
	RegisterBuilder("Table", new GUITable(m_mgr));
	RegisterBuilder("TextEffects", new GUITextEffects(m_mgr));
	RegisterBuilder("ImageScroller", new ImageScroller());
	RegisterBuilder("FunctionButton", new FunctionButton(m_mgr));
	RegisterBuilder("Dial", new Dial());

#ifdef USEBOX2DPHYSICS
	RegisterBuilder("PhysicsObject", new PhysicsObject(Game->GetPhysicsWorld(), false));
	RegisterBuilder("PhysicsJoint", new PhyJoint(Game->GetPhysicsWorld()));
#endif
}

//Object
const string& Object::GetName()
{
	return name;
}

int Object::GetType()
{
	return m_type;
}

bool Object::GetUpdate()
{
	return m_update;
}

void Object::SetUpdate(bool val)
{
	m_update = val;
}

ObjectsContainer* Object::GetParent()
{
	return m_parent;
}

void Object::Save( BaseIO& str )
{
	str.Write(name, 0, "name=ObjectType;help=dont modify this value.");
	m_objver = OBJ_VERSION;
	str.Write(m_objver, 0, "name=ObjectVersion", true);
	str.Write(m_ID, 0, "name=ObjectID;help=ObjectID, Must be more than 1000.;category=Object");
	str.Write(m_objname, 0, "name=Object Name;help=Object Name");
	str.Write(m_flags, 0 , "name=Flags;help=flags.");
	str.Write(m_objtype, 0 , "name=Object Type");
	str.Write(m_update, 0, "name=Enable update;help=Enable update.");

	str.Write((unsigned int)extrafloat.size(), 0, 0, true);

	for(map<string, float>::iterator i = extrafloat.begin(); i!=extrafloat.end(); i++)
	{
		str.Write(i->first, 0, 0, true);
		str.Write(i->second, 0, ("name=Prop_" + i->first).c_str());
	}
}

string Object::ReadType(BaseIO& str)
{
	string type;
	str.Read(type);
	return type;
}

void Object::Load( BaseIO& str )
{
	str.Read(m_objver, 0, true);
	str.Read(m_ID);
	str.Read(m_objname);
	str.Read(m_flags);
	str.Read(m_objtype);
	str.Read(m_update);

	unsigned int sz = 0;
	str.Read(sz, 0, true);
	if(sz == 0)
	{
		for(map<string, float>::iterator i = extrafloat.begin(); i!=extrafloat.end(); i++)
		{
			string st;
			float data;
			str.Read(st, 0, true);
			str.Read(data);
			i->second = data;
		}
	}
	else
	{
		for(unsigned int i = 0; i<sz; i++)
		{
			string st;
			float data;
			str.Read(st);
			str.Read(data);
			extrafloat[st] = data;
		}
	}
}

Object::Object( unsigned int ID, unsigned int flags, ObjectsContainer* parent, bool update )
{
	m_type = OBJECT_TYPE_OBJECT;
	m_ID = ID;
	m_flags = flags;

	m_parent = parent;
	m_update = update;
	m_objver = OBJ_VERSION;
	m_objtype = 0;
}

void Object::SetID(unsigned int val)
{
	m_ID = val;
}

unsigned int Object::GetID()
{
	return m_ID;
}

unsigned int Object::GetObjectVersion()
{
	return m_objver;
}

void Object::SetProperty(const char* name, float val)
{
	extrafloat[name] = val;
}

float Object::GetProperty(const char* name)
{
	return extrafloat[name];
}

void Object::ClearProperties()
{
	extrafloat.clear();
}

//Drawable Object
void DrawableObject::ReleaseSelected()
{
	if(m_parent!=0)
	{
		if(m_parent->Selected==this)
		{
			m_parent->Selected = 0;
		}
	}
}

void DrawableObject::ReleaseEditor()
{
	if(m_parent!=0)
	{
		if(m_parent->GetEditorObject()==this)
		{
			m_parent->ClearEditorObject();
		}
	}
}

Object* DrawableObject::Clone()
{
	DrawableObject* obj = new DrawableObject();
	*obj = *this;
	return obj;
}

RectangleF& DrawableObject::GetObjectArea()
{
	return m_location;
}

void DrawableObject::SetObjectArea(const RectangleF& val)
{
	m_location = val;
	if(m_parent) m_parent->Callback(this, OBJECT_Message_Resize, 0, 0);
}

float DrawableObject::GetAngle()
{
	return m_rotation;
}

void DrawableObject::SetAngle(float val)
{
	m_rotation = val;
}

bool DrawableObject::GetVisible()
{
	return !(m_flags&OBJECT_Flag_Invisible);
}

void DrawableObject::SetVisible(bool val)
{
	if(val==false) m_flags |= OBJECT_Flag_Invisible;
	else m_flags &= ~OBJECT_Flag_Invisible;
}

unsigned int DrawableObject::GetBackColor()
{
	return m_backcolor;
}

void DrawableObject::SetBackColor(unsigned int val)
{
	m_backcolor = val;
}

Sprite& DrawableObject::GetSprite()
{
	return m_sprite;
}

void DrawableObject::SetSprite(Sprite const& val)
{
	m_sprite = val;
}

void DrawableObject::SpriteAnimationFinished()
{
	if(m_parent) m_parent->Callback(this, OBJECT_Message_SpriteFinished, 1, 0);
}

DrawableObject::DrawableObject( unsigned int ID, RectangleF const& location, Resource sprite, unsigned int backcolor, float rotation, bool visible, unsigned int flags, ObjectsContainer* parent, bool update ) : Object( ID, flags, parent, update )
{
	m_type |= OBJECT_TYPE_DRAWABLE;
	m_location = location;
	if (sprite!=0) m_sprite = *(sprite.GetPtr<Sprite>());
	m_backcolor = backcolor;
	m_rotation = rotation;
	SetVisible(visible);
	m_layer = 0.5f;
	m_sprite.Pause();
	m_depth = 1.0f;
	m_sprite.parent = this;
}

DrawableObject::DrawableObject(const DrawableObject& obj)
{
	m_type |= OBJECT_TYPE_DRAWABLE;
	m_update = obj.m_update;
	name = obj.name;
	m_ID = obj.m_ID;
	m_objver = obj.m_objver;
	m_parent = 0;
	m_flags = obj.m_flags;

	m_location = obj.m_location;
	m_rotation = obj.m_rotation;
	m_sprite = obj.m_sprite;
	m_sprite.parent = this;
	m_editable = obj.m_editable;
	m_layer = obj.m_layer;
	m_backcolor = obj.m_backcolor;
	m_depth = obj.m_depth;
	m_objtype = obj.m_objtype;
	extrafloat = obj.extrafloat;
}

const DrawableObject& DrawableObject::operator=(const DrawableObject& obj)
{
	m_update = obj.m_update;
	name = obj.name;
	m_ID = obj.m_ID;
	m_objver = obj.m_objver;
	m_flags = obj.m_flags;

	m_location = obj.m_location;
	m_rotation = obj.m_rotation;
	m_sprite = obj.m_sprite;
	m_editable = obj.m_editable;
	m_layer = obj.m_layer;
	m_backcolor = obj.m_backcolor;
	m_depth = obj.m_depth;
	m_objtype = obj.m_objtype;
	extrafloat = obj.extrafloat;
	return *this;
}

float DrawableObject::GetLayer()
{
	return m_layer;
}

void DrawableObject::SetLayer(float val)
{
	m_layer = val;
}

void DrawableObject::SetCapture()
{
	if(m_parent!=0)
	{
		m_parent->Capture = this;
		m_parent->SetCapture();
	}
}

void DrawableObject::ReleaseCapture()
{
	if(GetType()&OBJECT_TYPE_CONTAINER)
	{
		if(((ObjectsContainer*)this)->HoldCapture) return;
	}
	if(m_parent!=0)
	{
		m_parent->Capture = 0;
		m_parent->ReleaseCapture();
	}
}

void DrawableObject::Delete()
{
	if(m_parent!=0 && !(m_type & OBJECT_TYPE_EDITOR))
	{
		m_parent->RemoveObject(this);
	}
}

void DrawableObject::QueueForDelete()
{
	if(m_parent!=0 && !(m_type & OBJECT_TYPE_EDITOR))
	{
		m_parent->AddToDeleteQueue(this);
	}
}

bool DrawableObject::IsEditing()
{
	if(m_parent!=0 && !(m_type & OBJECT_TYPE_EDITOR))
	{
		return m_parent->GetEditable();	
	}
	return false;
}

bool DrawableObject::IsCaptured()
{
	if(m_parent!=0 && !(m_type & OBJECT_TYPE_EDITOR))
	{
		if(m_parent->Capture == this)
		{
			return true;
		}	
	}
	return false;
}

bool DrawableObject::IsDrawTop()
{
	if(m_parent!=0 && !(m_type & OBJECT_TYPE_EDITOR))
	{
		if(m_parent->DrawTop == this)
		{
			return true;
		}	
	}
	return false;
}

bool DrawableObject::IsSelected()
{
	if(m_parent!=0 && !(m_type & OBJECT_TYPE_EDITOR))
	{
		if(m_parent->Selected == this)
		{
			return true;
		}	
	}
	return false;
}

void DrawableObject::SetSelected()
{
	if(m_parent!=0 && !(m_type & OBJECT_TYPE_EDITOR))
	{
		m_parent->Selected = this;
		m_parent->SetSelected();
		m_parent->SetEditorObject(this);
	}
}

void DrawableObject::SetDrawTop()
{
	if(m_parent!=0)
	{
		m_parent->DrawTop = this;
		m_parent->SetDrawTop();
	}
}

void DrawableObject::ReleaseDrawTop()
{
	if(m_parent!=0)
	{
		m_parent->DrawTop = 0;
		m_parent->ReleaseDrawTop();
	}
}

bool DrawableObject::GetSelected()
{
	if(m_parent!=0)
	{
		if(m_parent->Selected == this) return true;
	}
	return false;
}

void DrawableObject::Process( unsigned int Msg, unsigned int Param1, void* Param2 )
{
	switch(Msg)
	{
	case OBJECT_Message_LMouseDown:
		SetSelected();
		break;
	}
}

void DrawableObject::Update( float delta )
{
	if(m_update) m_sprite.Update(delta);
}

void DrawableObject::Save( BaseIO& str )
{
	Object::Save(str);
	m_sprite.Save(str, "category=DrawableObject;name=Sprite");
	//str.Write(m_visible);
	str.Write(m_backcolor, 0, "name=Background color;help=Background color.;type=color");
	str.Write(GetAngle(), 0, "name=Rotation;help=Rotation angle of the object in radians.;type=angle");
	str.Write(GetLayer(), 0, "name=Layer;help=layer number of the object.");
	//str.Write(m_extra, 0, );
	str.Write(GetObjectArea().left, 0, "name=Position : Left;help=Position x.");
	str.Write(GetObjectArea().top, 0, "name=Position : Top;help=Position y.");
	str.Write(GetObjectArea().right, 0, "name=Position : Right;help=Position right.");
	str.Write(GetObjectArea().bottom, 0, "name=Position : Bottom;help=Position bottom.");
	str.Write(m_depth, 0, "name=ParallaxDepth");
}

void DrawableObject::Load( BaseIO& str )
{
	Object::Load(str);
	m_sprite.Load(str);
	//str.Read(m_visible);
	str.Read(m_backcolor);
	float rot=0;
	str.Read(rot);
	SetAngle(rot);
	//str.Read(m_extra);
	float layer = 0.0f;
	str.Read(layer);
	SetLayer(layer);
	RectangleF r;
	str.Read(r.left);
	str.Read(r.top);
	str.Read(r.right);
	str.Read(r.bottom);
	SetObjectArea(r);
	str.Read(m_depth);
}

bool DrawableObject::Init()
{
	// No Init required
	return true;
}

RectangleF DrawableObject::ToRelativePos(RectangleF const& pos, bool transform)
{
	RectangleF rt = pos;
	RectangleF rect = GetAbsolutePos();

	if(transform && GetType()&OBJECT_TYPE_CONTAINER)
	{
		rt.left += ((ObjectsContainer*)this)->GetCamera().m_pos.left / m_depth;
		rt.top += ((ObjectsContainer*)this)->GetCamera().m_pos.top / m_depth;
		rt.right += ((ObjectsContainer*)this)->GetCamera().m_pos.left / m_depth;
		rt.bottom += ((ObjectsContainer*)this)->GetCamera().m_pos.top / m_depth;

		rt.left /= ((ObjectsContainer*)this)->GetCamera().m_zoom / m_depth;
		rt.top /= ((ObjectsContainer*)this)->GetCamera().m_zoom / m_depth;
		rt.right /= ((ObjectsContainer*)this)->GetCamera().m_zoom / m_depth;
		rt.bottom /= ((ObjectsContainer*)this)->GetCamera().m_zoom / m_depth;
	}
	
	rt.left -= rect.left;
	rt.top -= rect.top;
	rt.right -= rect.left;
	rt.bottom -= rect.top;

	if(GetType()&OBJECT_TYPE_CONTAINER)
	{
		rt.left /= ((ObjectsContainer*)this)->GetCamera().m_scale;
		rt.top /= ((ObjectsContainer*)this)->GetCamera().m_scale;
		rt.right /= ((ObjectsContainer*)this)->GetCamera().m_scale;
		rt.bottom /= ((ObjectsContainer*)this)->GetCamera().m_scale;
	}

	if(m_parent!=0)
	{
		if(m_flags & OBJECT_Flag_Tansform && !transform)
		{
			rt.left += m_parent->GetCamera().m_pos.left / m_depth;
			rt.top += m_parent->GetCamera().m_pos.top / m_depth;
			rt.right += m_parent->GetCamera().m_pos.left / m_depth;
			rt.bottom += m_parent->GetCamera().m_pos.top / m_depth;

			rt.left /= m_parent->GetCamera().m_zoom / m_depth;
			rt.top /= m_parent->GetCamera().m_zoom / m_depth;
			rt.right /= m_parent->GetCamera().m_zoom / m_depth;
			rt.bottom /= m_parent->GetCamera().m_zoom / m_depth;
		}

		rt.left /= m_parent->GetCamera().m_scale;
		rt.top /= m_parent->GetCamera().m_scale;
		rt.right /= m_parent->GetCamera().m_scale;
		rt.bottom /= m_parent->GetCamera().m_scale;
	}

	return rt;
}

RectangleF DrawableObject::GetAbsolutePos(const RectangleF* pos)
{
	RectangleF rt(0,0,0,0);
	RectangleF rt2 = GetObjectArea();
	if(pos!=0) rt2 = (*pos);

	if(GetType()&OBJECT_TYPE_CONTAINER)
	{
		rt2.left *= ((ObjectsContainer*)this)->GetCamera().m_scale;
		rt2.top *= ((ObjectsContainer*)this)->GetCamera().m_scale;
		rt2.right *= ((ObjectsContainer*)this)->GetCamera().m_scale;
		rt2.bottom *= ((ObjectsContainer*)this)->GetCamera().m_scale;
	}

	if(m_parent!=0)
	{
		if(m_parent->m_parent!=0)
		{
			rt2.left = (rt2.left * m_parent->m_parent->GetCamera().m_scale);
			rt2.top = (rt2.top * m_parent->m_parent->GetCamera().m_scale);
			rt2.right = (rt2.right * m_parent->m_parent->GetCamera().m_scale);
			rt2.bottom = (rt2.bottom * m_parent->m_parent->GetCamera().m_scale);
		}

		rt.left = (rt2.left * m_parent->GetCamera().m_scale) + m_parent->GetAbsolutePos().left;
		rt.top = (rt2.top * m_parent->GetCamera().m_scale) + m_parent->GetAbsolutePos().top;
		rt.right = (rt2.right * m_parent->GetCamera().m_scale) + m_parent->GetAbsolutePos().left;
		rt.bottom = (rt2.bottom * m_parent->GetCamera().m_scale) + m_parent->GetAbsolutePos().top;

		if(m_flags & OBJECT_Flag_Tansform)
		{
			rt.left *= m_parent->GetCamera().m_zoom * m_depth;
			rt.top *= m_parent->GetCamera().m_zoom * m_depth;
			rt.right *= m_parent->GetCamera().m_zoom * m_depth;
			rt.bottom *= m_parent->GetCamera().m_zoom * m_depth;

			rt.left -= m_parent->GetCamera().m_pos.left * m_depth;
			rt.top -= m_parent->GetCamera().m_pos.top * m_depth;
			rt.right -= m_parent->GetCamera().m_pos.left * m_depth;
			rt.bottom -= m_parent->GetCamera().m_pos.top * m_depth;
		}
	}
	else
	{
		return rt2;
	}
	return rt;
}

void DrawableObject::Draw( C_Renderer& rnd )
{
	m_sprite.Draw(&rnd, GetAbsolutePos(), m_backcolor, GetAngle(), GetLayer());
}

bool DrawableObject::TestPoint(const PointI& pt)
{
	return PointI::Intersect(pt, GetAbsolutePos());
}

void DrawableObject::MouseDown( unsigned short button, unsigned short buttons, PointI const& pt )
{
	if(button == KEY_LBUTTON)
	{
		SetSelected();
	}
}

void DrawableObject::DrawBoundingBox(C_Renderer& rnd)
{
	RectangleF r = GetAbsolutePos();
	PointF v[4];
	v[0].left = r.left;
	v[0].top = r.top;

	v[1].left = r.right;
	v[1].top = r.top;

	v[2].left = r.right;
	v[2].top = r.bottom;

	v[3].left = r.left;
	v[3].top = r.bottom;
	rnd.DrawPolygon(v, 4, 0xFFFFFFFF);
}

void DrawableObject::ClearConnections()
{
	for(set<ObjectConnection*>::iterator i = connections.begin(); i!= connections.end(); i++)
	{
		(*i)->Disconnected((*i)->cobj);
		(*i)->cobj = 0;
	}
	connections.clear();
}

DrawableObject::~DrawableObject()
{
	ClearConnections();
}

// Object Container

void ObjectsContainer::AddToDeleteQueue(Object* val)
{
	DeleteQueue.insert(val);
}

void ObjectsContainer::ProcessDeleteQueue()
{
	for(set<Object*>::iterator i = DeleteQueue.begin(); i != DeleteQueue.end(); ++i)
	{
		RemoveObject(*i);
	}
	DeleteQueue.clear();
	OnDeleteQueue();
}

Camera2D& ObjectsContainer::GetCamera()
{
	return m_Camera;
}

void ObjectsContainer::CenterCamera()
{
	RectangleF ab = GetAbsolutePos();
	float cwidth = 0, cheight = 0;
	cwidth = (m_Camera.m_area.right - m_Camera.m_area.left) / 2.0f;
	cheight = (m_Camera.m_area.bottom - m_Camera.m_area.top) / 2.0f;

	float vwidth = 0, vheight = 0;
	vwidth = (ab.right - ab.left) / 2.0f;
	vheight = (ab.bottom - ab.top) / 2.0f;

	m_location.left = cwidth - vwidth / m_Camera.m_scale;
	m_location.right = m_location.left + vwidth / m_Camera.m_scale * 2.0f;
	m_location.top = cheight - vheight / m_Camera.m_scale;
	m_location.bottom = m_location.top + vheight / m_Camera.m_scale * 2.0f;
}

void ObjectsContainer::SetupCamera(const Camera2D& camera)
{
	m_Camera = camera;
	m_Camera.m_size.width = (unsigned int)(camera.m_area.right - camera.m_area.left);
	m_Camera.m_size.height = (unsigned int)(camera.m_area.bottom - camera.m_area.top);
	AdjustCamera();
}

void ObjectsContainer::AdjustSize(C_Device& dev)
{
	Size sz = dev.GetBackBufferSize();
	if(sz.height == 0 || sz.width == 0) return;

	float vwidth = 0, vheight = 0;
	//vwidth = m_location.right - m_location.left;
	//vheight = m_location.bottom - m_location.top;
	vwidth = (float)m_Camera.m_size.width;
	vheight = (float)m_Camera.m_size.height;

	float wscale = sz.width / vwidth;
	float hscale = sz.height / vheight;
	float scale = (wscale < hscale) ? wscale : hscale;
	m_Camera.m_scale = scale;

	float locwidth = m_location.right - m_location.left;
	float locheight = m_location.bottom - m_location.top;
	
	m_location.left = (sz.width/2.0f) / scale - vwidth/2.0f;
	m_location.top = (sz.height/2.0f) / scale - vheight/2.0f;
	m_location.right = m_location.left + locwidth;
	m_location.bottom = m_location.top + locheight;

	RectangleF view;
	view.left = (sz.width/2.0f) / scale - vwidth/2.0f;
	view.top = (sz.height/2.0f) / scale - vheight/2.0f;
	view.right = view.left + vwidth;
	view.bottom = view.top + vheight;
	view.left *= scale;
	view.top *= scale;
	view.right *= scale;
	view.bottom *= scale;
	dev.SetView(view);

#ifdef ANDROID
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthof(view.left, view.right, view.bottom, view.top, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
#endif

	//m_Camera.m_area.right = m_Camera.m_area.left + (float)sz.width;
	//m_Camera.m_area.bottom = m_Camera.m_area.top + (float)sz.height;

	//CenterCamera();
}

void ObjectsContainer::AdjustCamera()
{
	/*float cwidth = 0, cheight = 0;
	cwidth = m_Camera.m_area.right - m_Camera.m_area.left;
	cheight = m_Camera.m_area.bottom - m_Camera.m_area.top;

	RectangleF ab = GetAbsolutePos();
	float vwidth = 0, vheight = 0;
	vwidth = ab.right - ab.left;
	vheight = ab.bottom - ab.top;

	if(cwidth >= vwidth)
	{
		m_location.left = cwidth / 2.0f - vwidth / m_Camera.m_scale / 2.0f;
		m_location.right = m_location.left + vwidth / m_Camera.m_scale;

		m_Camera.m_area.left = 0;
		m_Camera.m_area.right = m_Camera.m_area.left + cwidth;
	}
	else
	{
		if(m_Camera.m_area.left <= ab.left) m_Camera.m_area.left = ab.left;
		else if(m_Camera.m_area.right >= ab.right) m_Camera.m_area.left = ab.right - cwidth;
		else m_location.left = -m_Camera.m_area.left / m_Camera.m_scale;
		
		m_Camera.m_area.right = m_Camera.m_area.left + cwidth;
		m_location.right = m_location.left + vwidth / m_Camera.m_scale;
	}

	if(cheight >= vheight)
	{
		m_location.top = cheight / 2.0f - vheight / m_Camera.m_scale / 2.0f;
		m_location.bottom = m_location.top + vheight / m_Camera.m_scale;

		m_Camera.m_area.top = 0;
		m_Camera.m_area.bottom = m_Camera.m_area.top + cheight;
	}
	else
	{
		if(m_Camera.m_area.top <= ab.top) m_Camera.m_area.top = ab.top;
		else if(m_Camera.m_area.bottom >= ab.bottom) m_Camera.m_area.top = ab.bottom - cheight;
		else m_location.top = -m_Camera.m_area.top  / m_Camera.m_scale;

		m_Camera.m_area.bottom = m_Camera.m_area.top + cheight;
		m_location.bottom = m_location.top + vheight  / m_Camera.m_scale;
	}*/

	if(m_Camera.m_zoom < 1.0f)
	{
		m_Camera.m_zoom = 1.0f;
	}

	if(m_Camera.m_zoom > m_Camera.m_maxzoom)
	{
		m_Camera.m_zoom = m_Camera.m_maxzoom;
	}

	RectangleF rct = GetAbsolutePos();

	float limitwidth = m_Camera.m_max.width * m_Camera.m_scale * m_Camera.m_zoom + (rct.right - rct.left) * m_Camera.m_zoom - m_Camera.m_size.width * m_Camera.m_scale;
	float limitheight = m_Camera.m_max.height * m_Camera.m_scale * m_Camera.m_zoom + (rct.bottom - rct.top) * m_Camera.m_zoom - m_Camera.m_size.height * m_Camera.m_scale;

	if(m_Camera.m_pos.left > limitwidth)
	{
		m_Camera.m_pos.left = limitwidth;
	}

	if(m_Camera.m_pos.top > limitheight)
	{
		m_Camera.m_pos.top = limitheight;
	}

	if(m_Camera.m_pos.left < 0)
	{
		m_Camera.m_pos.left = 0;
	}

	if(m_Camera.m_pos.top < 0)
	{
		m_Camera.m_pos.top = 0;
	}
}

void ObjectsContainer::FocusCameraOn(const PointF& val, const PointF& offset)
{
	//RectangleF rct = ((DrawableObject*)val)->GetAbsolutePos();

	m_Camera.m_pos.left = ((val.left) - offset.left) * m_Camera.m_scale * m_Camera.m_zoom;
	m_Camera.m_pos.top = ((val.top) - offset.top) * m_Camera.m_scale * m_Camera.m_zoom;

	AdjustCamera();
}

void ObjectsContainer::MoveCamera(float x, float y)
{
	m_Camera.m_pos.left += x;
	m_Camera.m_pos.top += y;

	AdjustCamera();
	//TODO: Create new camera limit variable
}

void ObjectsContainer::Zoom(float factor)
{
	m_Camera.m_zoom *= factor;

	m_Camera.m_pos.left /= m_Camera.m_scale;
	m_Camera.m_pos.top /= m_Camera.m_scale;
	AdjustCamera();
}

void ObjectsContainer::ShowAll()
{
	for(list<Object*>::iterator i = Objects.begin(); i!=Objects.end(); i++)
	{
		(*i)->m_flags &= ~OBJECT_Flag_Invisible;
	}
}

void ObjectsContainer::HideAll()
{
	for(list<Object*>::iterator i = Objects.begin(); i!=Objects.end(); i++)
	{
		(*i)->m_flags |= OBJECT_Flag_Invisible;
	}
}

void ObjectsContainer::ScaleAll(float scale)
{
	RectangleF r = GetObjectArea();
	r.left *= scale;
	r.top *= scale;
	r.right *= scale;
	r.bottom *= scale;
	SetObjectArea(r);

	for(list<Object*>::iterator i = Objects.begin(); i!=Objects.end(); i++)
	{
		if((*i)->GetType()&OBJECT_TYPE_CONTAINER)
		{
			((ObjectsContainer*) (*i))->ScaleAll(scale);
		}
		else if((*i)->GetType()&OBJECT_TYPE_DRAWABLE)
		{
			r = ((DrawableObject*) (*i))->GetObjectArea();
			r.left *= scale;
			r.top *= scale;
			r.right *= scale;
			r.bottom *= scale;
			((DrawableObject*) (*i))->SetObjectArea(r);
		}
	}
}

bool ObjectsContainer::GetEditable()
{
	return m_editable;
}

void ObjectsContainer::SetEditable(bool val)
{
	m_editable = val;
	if(!m_editable && m_editor) m_editor->ClearObject();
}

void ObjectsContainer::Save( BaseIO& str )
{
	DrawableObject::Save(str);
	str.Write(m_Camera.m_max.width, 0, "name=MaxExWidth;category=Camera");
	str.Write(m_Camera.m_max.height, 0, "name=MaxExHeight");
	str.Write(m_Camera.m_maxzoom, 0, "name=MaxZoom");
	str.Write(m_Camera.m_pos.left, 0, "name=CamPosLeft");
	str.Write(m_Camera.m_pos.top, 0, "name=CamPosTop");
	str.Write(m_Camera.m_zoom, 0, "name=CamZoom");
}

void ObjectsContainer::Load( BaseIO& str )
{
	DrawableObject::Load(str);
	str.Read(m_Camera.m_max.width);
	str.Read(m_Camera.m_max.height);
	str.Read(m_Camera.m_maxzoom);
	str.Read(m_Camera.m_pos.left);
	str.Read(m_Camera.m_pos.top);
	str.Read(m_Camera.m_zoom);
}

void ObjectsContainer::SaveAll( BaseIO& str )
{
	this->Save(str);
	

	unsigned int size = this->GetSize() - 1;
	str.Write(size);
	for(list<Object*>::iterator i = Objects.begin(); i!=Objects.end(); i++)
	{
		if((*i)->GetType()&OBJECT_TYPE_CONTAINER)
		{
			if((*i)->GetName() != "") ((ObjectsContainer*) (*i))->SaveAll(str);
		}
		else if(!((*i)->GetType()&OBJECT_TYPE_EDITOR))
		{
			if((*i)->GetName() != "") (*i)->Save(str);
		}
	}
}

void ObjectsContainer::LoadAll( BaseIO& str, ObjectBuilder& builder, bool editor, bool readtype, bool append )
{
	string n = "";
	if(readtype)
	{
		n = Object::ReadType(str);
		if(n=="") return;
	}

	if(append==true)
	{
		ObjectsContainer* tmpcon = new ObjectsContainer(this->m_resmanager);
		tmpcon->Load(str);
		delete tmpcon;
	}
	else
	{
		this->Load(str);
	}

	unsigned int size = 0;
	str.Read(size);
	if(size==0) return;

	for(unsigned int i = 0; i < size; i++)
	{
		n = "";
		n = Object::ReadType(str);
		if(n=="") continue;
		Object* obj = builder.BuildObject(n.c_str());
		if(obj->GetType()&OBJECT_TYPE_CONTAINER)
		{
			((ObjectsContainer*) obj)->LoadAll(str, builder, editor, false);
			if(editor) ((ObjectsContainer*) obj)->SetEditable(editor);

#ifdef USESCRIPTING
			((ObjectsContainer*) obj)->Callback.RegisterSingleHandler<Script, &Script::Callback>(&Game->m_script);
#endif
		}
		else obj->Load(str);
		AddObject(obj);
	}

	for(list<Object*>::iterator i = Objects.begin(); i!=Objects.end(); i++)
	{
		(*i)->PostLoadAll();
	}

	AdjustCamera();
}

void ObjectsContainer::UpdateEditor()
{
	if(m_editor) m_editor->UpdateSize();
}

Object* ObjectsContainer::Clone()
{
	return new ObjectsContainer(m_resmanager);
}

DrawableObject* ObjectsContainer::GetEditorObject()
{
	if(m_editor!=0)
	{
		return m_editor->GetObject();
	}
	return 0;
}

DrawableObject* ObjectsContainer::GetEditorObjectRecursive()
{
	if(m_editor!=0)
	{
		DrawableObject* obj2 = 0;
		DrawableObject* obj = m_editor->GetObject();
		if(obj!=0 && obj->GetType()&OBJECT_TYPE_CONTAINER)
		{
			obj2 = ((ObjectsContainer*)obj)->GetEditorObjectRecursive();
		}
		if(obj2==0)
			return obj;
		else
			return obj2;
	}
	return 0;
}

void ObjectsContainer::SetEditorObject(DrawableObject* obj)
{
	if(m_editor!=0 && m_editable)
	{
		m_editor->SetObject(obj);
		Callback(obj, OBJECT_Message_SelEdit, 0, 0);
		Selected = m_editor;
		SetSelected();
	}
}

void ObjectsContainer::Clean()
{
	for(list<Object*>::iterator i = Objects.begin(); i!=Objects.end(); i++)
	{
		if((*i)==m_editor) continue;
		Callback((*i), OBJECT_Message_Release, 0, 0);
		if(Selected==(*i)) Selected =0;
		if((*i)->GetType()&OBJECT_TYPE_DRAWABLE)
		{
			((DrawableObject*)(*i))->ReleaseCapture();
			((DrawableObject*)(*i))->ReleaseDrawTop();
			((DrawableObject*)(*i))->ReleaseSelected();
			((DrawableObject*)(*i))->ReleaseEditor();
		}

		(*i)->Release();
		delete (*i);
	}
}

Object* ObjectsContainer::GetObjectByID( unsigned int id )
{
	for(list<Object*>::iterator i = Objects.begin(); i!=Objects.end(); i++)
	{
		if((*i)->GetID() == id) return (*i);
	}
	return 0;
}

list<Object*>::iterator ObjectsContainer::GetObjectInteratorBegin()
{
	return Objects.begin();
}

list<Object*>::iterator ObjectsContainer::GetObjectInteratorEnd()
{
	return Objects.end();
}

void ObjectsContainer::AddObject( Object* obj )
{
	if(obj==0) return;
	if(obj->m_parent==0)
	{
		obj->m_parent = this;
		Objects.push_back(obj);
		obj->it = Objects.end();
		--obj->it;
		if(m_editable && obj->GetType()&OBJECT_TYPE_DRAWABLE) SetEditorObject((DrawableObject*)obj);
		//ReleaseEditor();
	}
}

void ObjectsContainer::RemoveObject( unsigned int Index )
{
	if(Index>=Objects.size()) return;
	list<Object*>::iterator i = Objects.begin();
	advance(i, Index);
	if(Selected==(*i)) Selected =0;
	if((*i)->GetType()&OBJECT_TYPE_DRAWABLE)
	{
		if(Capture == *i) ((DrawableObject*)(*i))->ReleaseCapture();
		if(DrawTop == *i) ((DrawableObject*)(*i))->ReleaseDrawTop();
		if(Selected == *i) ((DrawableObject*)(*i))->ReleaseSelected();
		((DrawableObject*)(*i))->ReleaseEditor();
	}

	if((*i)->GetType()&OBJECT_TYPE_CONTAINER)
	{
		((ObjectsContainer*)(*i))->Clear();
	}
	(*i)->Release();
	delete (*i);
	Objects.erase(i);
}

void ObjectsContainer::RemoveObject( Object* ctrl )
{
	list<Object*>::iterator i = ctrl->it;
	if(Selected==(*i)) Selected =0;
	if((*i)->GetType()&OBJECT_TYPE_DRAWABLE)
	{
		if(Capture == *i) ((DrawableObject*)(*i))->ReleaseCapture();
		if(DrawTop == *i) ((DrawableObject*)(*i))->ReleaseDrawTop();
		if(Selected == *i) ((DrawableObject*)(*i))->ReleaseSelected();
		((DrawableObject*)(*i))->ReleaseEditor();
	}

	/*if((*i)->GetType()&OBJECT_TYPE_CONTAINER)
	{
		((ObjectsContainer*)(*i))->Clear();
	}*/

	(*i)->Release();
	delete (*i);
	Objects.erase(i);
}

Object* ObjectsContainer::GetObject( unsigned int Index )
{
	if(Index>=Objects.size()) return 0;
	list<Object*>::iterator i = Objects.begin();
	advance(i, Index);
	return (*i);
}

void ObjectsContainer::Clear(bool full)
{
	if(!Objects.empty())
	{
		m_editor->SetObject(0);
		Selected = 0;
		Capture = 0;
		DrawTop = 0;
		Clean();
		Objects.clear();
		if(full && m_editor != 0)
		{
			delete m_editor;
			m_editor = 0;
		}
		else
		{
			m_editor->m_parent = 0;
			ObjectsContainer::AddObject(m_editor);
			Selected = 0;
			Capture = 0;
			DrawTop = 0;
		}
	}
}

void ObjectsContainer::QueueClear()
{
	if(!Objects.empty())
	{
		m_editor->SetObject(0);
		Selected = 0;
		Capture = 0;
		DrawTop = 0;

		for(list<Object*>::iterator i = Objects.begin(); i!=Objects.end(); i++)
		{
			if((*i)==m_editor) continue;
			if(Selected==(*i)) Selected =0;
			(*i)->QueueForDelete();
		}
	}
}

unsigned int ObjectsContainer::GetSize()
{
	return Objects.size();
}

void ObjectsContainer::Update( float delta )
{
	ProcessDeleteQueue();
	
	DrawableObject::Update(delta);
	for(list<Object*>::iterator i = Objects.begin(); i!=Objects.end(); i++)
	{
		(*i)->Update(delta);
	}
}

void ObjectsContainer::Process( unsigned int Msg, unsigned int Param1, void* Param2 )
{	
	DrawableObject::Process(Msg, Param1, Param2);
	Callback(this, Msg, Param1, Param2);
}

Object* ObjectsContainer::GetIntersect( PointI const& Param2 )
{
	if(Capture!=0)
	{
		return Capture;
	}

	if(Selected!=0)
	{
		if(!(Selected->m_flags&OBJECT_Flag_Disabled) && !(Selected->m_flags&OBJECT_Flag_Invisible))
		{
			if(PointI::Intersect(Param2, Selected->GetAbsolutePos()))
			{				
				return Selected;					
			}
		}
	}

	for(list<Object*>::iterator i = Objects.begin(); i!=Objects.end(); i++)
	{
		if(!((*i)->m_flags&OBJECT_Flag_Disabled) && !((*i)->m_flags&OBJECT_Flag_Invisible))
		{
			if((*i)->GetType()&OBJECT_TYPE_DRAWABLE)
			if(PointI::Intersect(Param2, ((DrawableObject*)(*i))->GetAbsolutePos()))
			{
				return (*i);
			}
		}
	}
	return 0;
}

void ObjectsContainer::Release()
{
	DrawableObject::Release();
}

ObjectsContainer::~ObjectsContainer()
{
	DisconnectInput();
	Clear(true);
	//if(m_editor) delete m_editor;
}

bool ObjectsContainer::Init()
{
	DrawableObject::Init();
	bool flag = true;
	for(list<Object*>::iterator i = Objects.begin(); i!=Objects.end(); i++)
	{
		if((*i)->Init()==false) flag = false;
		Callback((*i), OBJECT_Message_Init, 0, 0);
	}
	return flag;
}

void ObjectsContainer::Draw( C_Renderer& rnd )
{
	DrawableObject::Draw(rnd);

	for(list<Object*>::iterator i = Objects.begin(); i!=Objects.end(); i++)
	{
		if((*i)==Capture) continue;
		if((*i)==Selected) continue;
		if((*i)==DrawTop) continue;
		if(!((*i)->m_flags&OBJECT_Flag_Invisible))
		{
			(*i)->Draw(rnd);
			Callback((*i), OBJECT_Message_Draw, 0, 0);
		}
	}

	if(Selected!=0)
	{
		if(!(Selected->m_flags&OBJECT_Flag_Invisible))
		{
			Selected->Draw(rnd);
			Callback(Selected, OBJECT_Message_Draw, 0, 0);
		}
	}

	if(Capture!=0 && Selected != Capture)
	{
		if(!(Capture->m_flags&OBJECT_Flag_Invisible))
		{
			Capture->Draw(rnd);
			Callback(Capture, OBJECT_Message_Draw, 0, 0);
		}
	}

	if(DrawTop!=0 && Selected!=DrawTop && Capture!=DrawTop)
	{
		if(!(DrawTop->m_flags&OBJECT_Flag_Invisible))
		{
			DrawTop->Draw(rnd);
			Callback(DrawTop, OBJECT_Message_Draw, 0, 0);
		}
	}
}

void ObjectsContainer::DrawBoundingBox(C_Renderer& rnd)
{
	if(!m_drawboundingbox) return;

	DrawableObject::DrawBoundingBox(rnd);

	RectangleF r = m_Camera.m_area;

	PointF v[4];
	v[0].left = 0;
	v[0].top = 0;

	v[1].left = r.right - r.left;
	v[1].top = 0;

	v[2].left = r.right - r.left;
	v[2].top = r.bottom - r.top;

	v[3].left = 0;
	v[3].top = r.bottom - r.top;
	rnd.DrawPolygon(v, 4, 0xFF00FFFF);

	for(list<Object*>::iterator i = Objects.begin(); i!=Objects.end(); i++)
	{
		if((*i)->GetType()&OBJECT_TYPE_DRAWABLE)
		{
			((DrawableObject*)(*i))->DrawBoundingBox(rnd);
		}
	}
}

bool ObjectsContainer::GetMouseProcess()
{
	return m_clicked;
}

void ObjectsContainer::MouseDown( unsigned short button, unsigned short buttons, PointI const& pt )
{
	/*if(m_parent==0)
	{
		const_cast<PointI*>(&pt)->left += Game->GetDevice()->GetViewTransform().left;
		const_cast<PointI*>(&pt)->top += Game->GetDevice()->GetViewTransform().top;
	}*/
	DrawableObject::MouseDown( button, buttons, pt);

	if(!(this->m_flags&OBJECT_Flag_Disabled) && !(this->m_flags&OBJECT_Flag_Invisible)) 
	{
		if(this->m_parent==0) Callback(this, OBJECT_Message_MouseDown, button, const_cast<PointI*>(&pt));
		/*if(m_editor && m_editor->GetObject()!=0)
		{
			if(PointI::Intersect(pt, m_editor->GetObject()->GetAbsolutePos()))
			{
				Callback(m_editor->GetObject(), OBJECT_Message_MouseDown, button, &pt);
				if(m_editor->GetObject()!=0) m_editor->GetObject()->MouseDown(button, buttons, pt);			
				return;
			}
		}*/

		if(Capture!=0)
		{
			Callback(Capture, OBJECT_Message_MouseDown, button, const_cast<PointI*>(&pt));
			if(Capture!=0) Capture->MouseDown(button, buttons, pt);		
			m_clicked = true;
			return;
		}

		if(Selected!=0)
		{
			if(!(Selected->m_flags&OBJECT_Flag_Disabled) && !(Selected->m_flags&OBJECT_Flag_Invisible))
			{
				if((Selected)->TestPoint(pt))
				{
					Callback(Selected, OBJECT_Message_MouseDown, button, const_cast<PointI*>(&pt));
					Selected->MouseDown(button, buttons, pt);
					m_clicked = true;
					return;
				}
			}
		}

		m_clicked = false;

		for(list<Object*>::iterator i = Objects.begin(); i!=Objects.end(); i++)
		{
			if((*i)->GetType()&OBJECT_TYPE_DRAWABLE && ((!((*i)->m_flags&OBJECT_Flag_Disabled) && !((*i)->m_flags&OBJECT_Flag_Invisible)) || m_editable))
			{
				if(((DrawableObject*)(*i))->TestPoint(pt))
				{
					m_clicked = true;
					if(m_editable)
					{
						if(!((*i)->m_type & OBJECT_TYPE_EDITOR))
						{
							Selected = ((DrawableObject*)(*i));
							SetSelected();
							SetEditorObject(((DrawableObject*)(*i)));
							if(button==KEY_LBUTTON) 
							{
								break;
							}
						}
						else
						{
							continue;
						}
					}
					else
					{
						Callback((*i), OBJECT_Message_MouseDown, button, const_cast<PointI*>(&pt));
						((DrawableObject*)(*i))->MouseDown(button, buttons, pt);
					}
					if(!(((DrawableObject*) (*i))->m_flags & OBJECT_Flag_IgnoreMouse) && !m_editable) break;
				}
			}
		}
		
		if(m_clicked == false && m_editable)
		{
			ClearEditorObject();
		}
	}
}

void ObjectsContainer::ClearEditorObject()
{
	if(!m_editor) return;
	Callback(this, OBJECT_Message_SelClear, 0, 0);
	if(m_editor->GetObject() && m_editor->GetObject()->GetType()&OBJECT_TYPE_CONTAINER)
	{
		((ObjectsContainer*)m_editor->GetObject())->ClearEditorObject();
	}
	m_editor->ClearObject();
}

void ObjectsContainer::MouseUp( unsigned short button, unsigned short buttons, PointI const& pt )
{
	/*if(m_parent==0)
	{
		const_cast<PointI*>(&pt)->left += Game->GetDevice()->GetViewTransform().left;
		const_cast<PointI*>(&pt)->top += Game->GetDevice()->GetViewTransform().top;
	}*/

	DrawableObject::MouseUp( button, buttons, pt);

	if(!(this->m_flags&OBJECT_Flag_Disabled) && !(this->m_flags&OBJECT_Flag_Invisible)) 
	{
		if(this->m_parent==0) Callback(this, OBJECT_Message_MouseUp, button, const_cast<PointI*>(&pt));

		/*if(m_editor && m_editor->GetObject()!=0)
		{
			if(PointI::Intersect(pt, m_editor->GetObject()->GetAbsolutePos()))
			{
				Callback(m_editor->GetObject(), OBJECT_Message_MouseUp, button, &pt);
				if(m_editor->GetObject()!=0) m_editor->GetObject()->MouseUp(button, buttons, pt);			
				return;
			}
		}*/

		if(Capture!=0)
		{
			Callback(Capture, OBJECT_Message_MouseUp, button, const_cast<PointI*>(&pt));
			if(Capture!=0) Capture->MouseUp(button, buttons, pt);			
			return;
		}

		if(Selected!=0)
		{
			if(!(Selected->m_flags&OBJECT_Flag_Disabled) && !(Selected->m_flags&OBJECT_Flag_Invisible))
			{
				if((Selected)->TestPoint(pt))
				{
					Callback(Selected, OBJECT_Message_MouseUp, button, const_cast<PointI*>(&pt));
					Selected->MouseUp(button, buttons, pt);					
					return;
				}
			}
		}

		for(list<Object*>::iterator i = Objects.begin(); i!=Objects.end(); i++)
		{
			if((*i)->GetType()&OBJECT_TYPE_DRAWABLE && !((*i)->m_flags&OBJECT_Flag_Disabled) && !((*i)->m_flags&OBJECT_Flag_Invisible))
			{
				if(((DrawableObject*)(*i))->TestPoint(pt))
				{
					Callback((*i), OBJECT_Message_MouseUp, button, const_cast<PointI*>(&pt));
					((DrawableObject*)(*i))->MouseUp(button, buttons, pt);	
					if(!(((DrawableObject*) (*i))->m_flags & OBJECT_Flag_IgnoreMouse)) break;
				}
			}
		}
	}
}

void ObjectsContainer::MouseMove( PointI const& pt )
{
	/*if(m_parent==0)
	{
		const_cast<PointI*>(&pt)->left += Game->GetDevice()->GetViewTransform().left;
		const_cast<PointI*>(&pt)->top += Game->GetDevice()->GetViewTransform().top;
	}*/

	DrawableObject::MouseMove( pt);

	if(!(this->m_flags&OBJECT_Flag_Disabled) && !(this->m_flags&OBJECT_Flag_Invisible)) 
	{
		if(this->m_parent==0) Callback(this, OBJECT_Message_MouseMove, 0, const_cast<PointI*>(&pt));

		/*if(m_editor && m_editor->GetObject()!=0)
		{
			if(PointI::Intersect(pt, m_editor->GetObject()->GetAbsolutePos()))
			{
				Callback(m_editor->GetObject(), OBJECT_Message_MouseMove, 0, &pt);
				if(m_editor->GetObject()!=0) m_editor->GetObject()->MouseMove(pt);			
				return;
			}
		}*/

		if(Capture!=0)
		{
			Callback(Capture, OBJECT_Message_MouseMove, 0, const_cast<PointI*>(&pt));
			if(Capture!=0) Capture->MouseMove(pt);			
			return;
		}

		if(Selected!=0)
		{
			if(!(Selected->m_flags&OBJECT_Flag_Disabled) && !(Selected->m_flags&OBJECT_Flag_Invisible))
			{
				if((Selected)->TestPoint(pt))
				{
					Callback(Selected, OBJECT_Message_MouseMove, 0, const_cast<PointI*>(&pt));
					Selected->MouseMove(pt);					
					return;
				}
			}
		}

		for(list<Object*>::iterator i = Objects.begin(); i!=Objects.end(); i++)
		{
			if((*i)->GetType()&OBJECT_TYPE_DRAWABLE && !((*i)->m_flags&OBJECT_Flag_Disabled) && !((*i)->m_flags&OBJECT_Flag_Invisible))
			{
				if(((DrawableObject*)(*i))->TestPoint(pt))
				{
					Callback((*i), OBJECT_Message_MouseMove, 0, const_cast<PointI*>(&pt));
					((DrawableObject*)(*i))->MouseMove(pt);
					if(!(((DrawableObject*) (*i))->m_flags & OBJECT_Flag_IgnoreMouse)) break;
				}
			}
		}
	}
}

void ObjectsContainer::MouseWheel( int wheel, unsigned short buttons, PointI const& pt)
{
	/*if(m_parent==0)
	{
		const_cast<PointI*>(&pt)->left += Game->GetDevice()->GetViewTransform().left;
		const_cast<PointI*>(&pt)->top += Game->GetDevice()->GetViewTransform().top;
	}*/

#ifdef _DEBUG
	if(wheel == 1) Zoom(1.23f);
	else Zoom(0.83f);
#endif
	DrawableObject::MouseWheel( wheel, buttons, pt);

	if(!(this->m_flags&OBJECT_Flag_Disabled) && !(this->m_flags&OBJECT_Flag_Invisible)) 
	{
		if(this->m_parent==0) Callback(this, OBJECT_Message_MouseScroll, wheel, const_cast<PointI*>(&pt));
		
		/*if(m_editor && m_editor->GetObject()!=0)
		{
			if(PointI::Intersect(pt, m_editor->GetObject()->GetAbsolutePos()))
			{
				Callback(m_editor->GetObject(), OBJECT_Message_MouseScroll, wheel, &pt);
				if(m_editor->GetObject()!=0) m_editor->GetObject()->MouseWheel(wheel, buttons, pt);		
				return;
			}
		}*/
		
		if(Capture!=0)
		{
			Callback(Capture, OBJECT_Message_MouseScroll, wheel, const_cast<PointI*>(&pt));
			if(Capture!=0) Capture->MouseWheel(wheel, buttons, pt);			
			return;
		}

		if(Selected!=0)
		{
			if(!(Selected->m_flags&OBJECT_Flag_Disabled) && !(Selected->m_flags&OBJECT_Flag_Invisible))
			{
				if((Selected)->TestPoint(pt))
				{
					Callback(Selected, OBJECT_Message_MouseScroll, wheel, const_cast<PointI*>(&pt));
					Selected->MouseWheel(wheel, buttons, pt);					
					return;
				}
			}
		}

		for(list<Object*>::iterator i = Objects.begin(); i!=Objects.end(); i++)
		{
			if((*i)->GetType()&OBJECT_TYPE_DRAWABLE && !((*i)->m_flags&OBJECT_Flag_Disabled) && !((*i)->m_flags&OBJECT_Flag_Invisible))
			{
				if(((DrawableObject*)(*i))->TestPoint(pt))
				{
					Callback((*i), OBJECT_Message_MouseScroll, wheel, const_cast<PointI*>(&pt));
					((DrawableObject*)(*i))->MouseWheel(wheel, buttons, pt);
					if(!(((DrawableObject*) (*i))->m_flags & OBJECT_Flag_IgnoreMouse)) break;
				}
			}
		}
	}
}

void ObjectsContainer::KeyDown( unsigned int key )
{
	DrawableObject::KeyDown(key);

	if(key == KEY_J)
	{
		MoveCamera(5, 0);
		//AdjustCamera();
	}
	else if(key == KEY_L)
	{
		MoveCamera(-5, 0);
		//AdjustCamera();
	}
	else if(key == KEY_I)
	{
		MoveCamera(0, 5);
		//AdjustCamera();
	}
	else if(key == KEY_K) 
	{
		MoveCamera(0, -5);
		//AdjustCamera();
	}
	
	Callback(this, OBJECT_Message_KeyDown, key, 0);
	if(!(this->m_flags&OBJECT_Flag_Disabled) && !(this->m_flags&OBJECT_Flag_Invisible)) Callback(this, OBJECT_Message_KeyDown, key, 0);
	{
		if(Selected!=0)
		{
			if(!(Selected->m_flags&OBJECT_Flag_Disabled) && !(Selected->m_flags&OBJECT_Flag_Invisible))
			{
				Callback(Selected, OBJECT_Message_KeyDown, key, 0);
				Selected->KeyDown(key);				
				return;
			}
		}
	}
}

void ObjectsContainer::KeyUp( unsigned int key )
{
	DrawableObject::KeyUp(key);
	Callback(this, OBJECT_Message_KeyUp, key, 0);
	if(!(this->m_flags&OBJECT_Flag_Disabled) && !(this->m_flags&OBJECT_Flag_Invisible)) Callback(this, OBJECT_Message_KeyUp, key, 0);
	{
		if(Selected!=0)
		{
			if(!(Selected->m_flags&OBJECT_Flag_Disabled) && !(Selected->m_flags&OBJECT_Flag_Invisible))
			{
				Callback(Selected, OBJECT_Message_KeyUp, key, 0);
				Selected->KeyUp(key);				
				return;
			}
		}
	}
}

void ObjectsContainer::KeyChar( wchar_t key )
{
	DrawableObject::KeyChar(key);
	Callback(this, OBJECT_Message_KeyUp, key, 0);
	if(!(this->m_flags&OBJECT_Flag_Disabled) && !(this->m_flags&OBJECT_Flag_Invisible)) Callback(this, OBJECT_Message_KeyChar, key, 0);
	{
		if(Selected!=0)
		{
			if(!(Selected->m_flags&OBJECT_Flag_Disabled) && !(Selected->m_flags&OBJECT_Flag_Invisible))
			{
				Callback(Selected, OBJECT_Message_KeyChar, key, 0);
				Selected->KeyChar(key);				
				return;
			}
		}
	}
}

ObjectsContainer::ObjectsContainer(ResourceManager* mgr, unsigned int ID, const RectangleF& location, Resource sprite, unsigned int backcolor, float rotatio, bool visible, unsigned int flags, ObjectsContainer* parent, bool update ) : DrawableObject(ID, location, sprite, backcolor, rotatio, visible, flags, parent, update)
{
	m_drawboundingbox = false;
	m_type|=OBJECT_TYPE_CONTAINER;
	m_win = 0;
	HoldCapture = false;
	Capture = 0;
	Selected = 0;
	DrawTop = 0;
	m_resmanager = mgr;
	m_editable = false;
	
	Resource res;
	mgr->GetResource("rectangle", RES_TYPE_SPRITE, &res);
	Sprite* spr = res.GetPtr<Sprite>();
	if(spr!=0)
	{
		m_editor = new ObjectEditor();
		m_editor->SetSprite(*spr);
		m_editor->SetID(100);
		AddObject(m_editor);
	}
	else
	{
		m_editor = 0;
	}	
}

void ObjectsContainer::AutoConnectInput( C_Window* window )
{
	DisconnectInput();
	window->MouseDown.RegisterHandler<ObjectsContainer, &ObjectsContainer::MouseDown>(this);
	window->MouseUp.RegisterHandler<ObjectsContainer, &ObjectsContainer::MouseUp>(this);
	window->MouseMove.RegisterHandler<ObjectsContainer, &ObjectsContainer::MouseMove>(this);
	window->MouseWheel.RegisterHandler<ObjectsContainer, &ObjectsContainer::MouseWheel>(this);

	window->KeyDown.RegisterHandler<ObjectsContainer, &ObjectsContainer::KeyDown>(this);
	window->KeyUp.RegisterHandler<ObjectsContainer, &ObjectsContainer::KeyUp>(this);
	window->KeyChar.RegisterHandler<ObjectsContainer, &ObjectsContainer::KeyChar>(this);
	m_win = window;
}

void ObjectsContainer::DisconnectInput()
{
	if(m_win==0) return;
	m_win->MouseDown.DisconnectHandler<ObjectsContainer, &ObjectsContainer::MouseDown>(this);
	m_win->MouseUp.DisconnectHandler<ObjectsContainer, &ObjectsContainer::MouseUp>(this);
	m_win->MouseMove.DisconnectHandler<ObjectsContainer, &ObjectsContainer::MouseMove>(this);
	m_win->MouseWheel.DisconnectHandler<ObjectsContainer, &ObjectsContainer::MouseWheel>(this);

	m_win->KeyDown.DisconnectHandler<ObjectsContainer, &ObjectsContainer::KeyDown>(this);
	m_win->KeyUp.DisconnectHandler<ObjectsContainer, &ObjectsContainer::KeyUp>(this);
	m_win->KeyChar.DisconnectHandler<ObjectsContainer, &ObjectsContainer::KeyChar>(this);
}

// ObjectEditor

ObjectEditor::EditMode ObjectEditor::GetIntersect(PointI const& pt)
{
	RectangleF Left, Top, Right, Bottom, Center;
	Center = Left = Top = Right = Bottom = GetAbsolutePos();

	Center.left += m_space;
	Center.top += m_space;
	Center.right -= m_space;
	Center.bottom -= m_space;
	if(PointI::Intersect(pt, Center)) return C;

	Left.right = Left.left + m_space;
	Top.bottom = Top.top + m_space;
	Right.left = Right.right - m_space;
	Bottom.top = Bottom.bottom - m_space;
	bool li = false, ti = false, ri = false, bi = false;
	if(PointI::Intersect(pt, Left)) li = true;
	if(PointI::Intersect(pt, Top)) ti = true;
	if(PointI::Intersect(pt, Right)) ri = true;
	if(PointI::Intersect(pt, Bottom)) bi = true;

	if(li && ti) return LT;
	else if(li && bi) return LB;
	else if(ri && ti) return RT;
	else if(ri && bi) return RB;
	else if(li) return L;
	else if(ri) return R;
	else if(ti) return T;
	else if(bi) return B;
	return None;
}

ObjectEditor::ObjectEditor(unsigned int ID, const RectangleF& location, Resource sprite, unsigned int backcolor, float rotatio, bool visible, unsigned int flags, ObjectsContainer* parent, bool update ) : DrawableObject(ID, location, sprite, backcolor, rotatio, visible, flags, parent, update)
{
	m_edit = None;
	m_object = 0;
	m_space = 6;
	m_mousedown = false;
	m_oldpos = PointI(0,0);
	m_type |= OBJECT_TYPE_EDITOR;
	m_flags |= OBJECT_Flag_Invisible;
	m_snaptogrid = true;
	m_gridsize = 5.0f;
}

void ObjectEditor::UpdateSize()
{
	if(m_object == 0) return;
	m_location = m_object->GetObjectArea();
	m_location.left -= m_space;
	m_location.top -= m_space;
	m_location.right += m_space;
	m_location.bottom += m_space;
}

void ObjectEditor::UpdateObjectSize()
{
	if(m_object == 0) return;
	RectangleF rect;
	rect.left = m_location.left + m_space;
	rect.top = m_location.top + m_space;
	rect.right = m_location.right - m_space;
	rect.bottom = m_location.bottom - m_space;
	m_object->SetObjectArea(rect);
}

void ObjectEditor::ClearObject()
{
	m_object = 0;
	m_flags |= OBJECT_Flag_Invisible;
}

void ObjectEditor::Process( unsigned int Msg, unsigned int Param1, void* Param2 )
{
	DrawableObject::Process( Msg, Param1, Param2 );
}

void ObjectEditor::Update( float delta )
{
	DrawableObject::Update(delta);
}

bool ObjectEditor::Init()
{
	return DrawableObject::Init();
}

void ObjectEditor::Draw(C_Renderer& rnd)
{
	RectangleF rf;
	rf = GetAbsolutePos();
	m_sprite.Draw(&rnd, rf, 0x000800FF, 0.0f, 0.0f);

	//
	rf = GetAbsolutePos();
	rf.right = rf.left + (m_space);
	m_sprite.Draw(&rnd, rf, 0x4FFFFFFF, 0.0f, 0.0f);

	rf = GetAbsolutePos();
	rf.bottom = rf.top + (m_space);
	m_sprite.Draw(&rnd, rf, 0x4FFFFFFF, 0.0f, 0.0f);

	rf = GetAbsolutePos();
	rf.top = rf.bottom - (m_space);
	m_sprite.Draw(&rnd, rf, 0x4FFFFFFF, 0.0f, 0.0f);

	rf = GetAbsolutePos();
	rf.left = rf.right - (m_space);
	m_sprite.Draw(&rnd, rf, 0x4FFFFFFF, 0.0f, 0.0f);
	
	//
	rf = GetAbsolutePos();
	rf.right = rf.left + (m_space);
	rf.bottom = rf.top + (m_space);
	m_sprite.Draw(&rnd, rf, 0xFFFFFFFF, 0.0f, 0.0f);

	rf = GetAbsolutePos();
	rf.left = rf.right - (m_space);
	rf.bottom = rf.top + (m_space);
	m_sprite.Draw(&rnd, rf, 0xFFFFFFFF, 0.0f, 0.0f);

	rf = GetAbsolutePos();
	rf.top = rf.bottom - (m_space);
	rf.right = rf.left + (m_space);
	m_sprite.Draw(&rnd, rf, 0xFFFFFFFF, 0.0f, 0.0f);

	rf = GetAbsolutePos();
	rf.left = rf.right - (m_space);
	rf.top = rf.bottom - (m_space);
	m_sprite.Draw(&rnd, rf, 0xFFFFFFFF, 0.0f, 0.0f);

	/*rnd.DrawSolidCircle(PointF(GetAbsolutePos().left + (m_space/2.0f), GetAbsolutePos().top + (m_space/2.0f)), m_space/2.0f, 0xFFFFFFFF);
	rnd.DrawSolidCircle(PointF(GetAbsolutePos().right - (m_space/2.0f), GetAbsolutePos().bottom - (m_space/2.0f)), m_space/2.0f, 0xFFFFFFFF);
	rnd.DrawSolidCircle(PointF(GetAbsolutePos().left + (m_space/2.0f), GetAbsolutePos().bottom - (m_space/2.0f)), m_space/2.0f, 0xFFFFFFFF);
	rnd.DrawSolidCircle(PointF(GetAbsolutePos().right - (m_space/2.0f), GetAbsolutePos().top + (m_space/2.0f)), m_space/2.0f, 0xFFFFFFFF);*/
}

void ObjectEditor::Save( BaseIO& str )
{
}

void ObjectEditor::Load( BaseIO& str )
{
}

//Input
void ObjectEditor::MouseDown(unsigned short button, unsigned short buttons, PointI const& pt)
{
	if(m_object!=0 && m_object->GetType()&OBJECT_TYPE_CONTAINER)
	{
		((ObjectsContainer*)m_object)->MouseDown(button, buttons, pt);
		if(((ObjectsContainer*)m_object)->GetIntersect(pt))
		{
			return;
		}
	}
	else
	{
		m_object->MouseDown(button, buttons, pt);
	}

	m_edit = GetIntersect(pt);
	if(m_edit!=None && button == KEY_LBUTTON)
	{
		m_mousedown = true;
		SetCapture();

		RectangleF r;
		r.left = floor(pt.left / m_gridsize) * m_gridsize;
		r.top = floor(pt.top / m_gridsize) * m_gridsize;
		r = ToRelativePos(r, (m_flags & OBJECT_Flag_Tansform) ? true : false);
		m_oldpos.left = (int)r.left;
		m_oldpos.top = (int)r.top;
	}
}

void ObjectEditor::MouseUp(unsigned short button, unsigned short buttons, PointI const& pt)
{	
	if(!m_mousedown && m_object!=0 && m_object->GetType()&OBJECT_TYPE_CONTAINER)
	{
		if(((ObjectsContainer*)m_object)->GetIntersect(pt))
		{
			((ObjectsContainer*)m_object)->MouseUp(button, buttons, pt);
			return;
		}
	}
	m_mousedown = false;
	ReleaseCapture();
}

void ObjectEditor::MouseMove(PointI const& pt)
{
	if(!m_mousedown && m_object!=0 && m_object->GetType()&OBJECT_TYPE_CONTAINER)
	{
		if(((ObjectsContainer*)m_object)->GetIntersect(pt))
		{
			((ObjectsContainer*)m_object)->MouseMove(pt);
			return;
		}
	}

	if(m_mousedown==true)
	{
		RectangleF r;
		r.left = floor(pt.left / m_gridsize) * m_gridsize;
		r.top = floor(pt.top / m_gridsize) * m_gridsize;
		if(m_parent!=0) r = m_parent->ToRelativePos(r, (m_flags & OBJECT_Flag_Tansform) ? true : false);

		const float d = m_space * 2.0f + 1.0f;
		if(m_edit==L)
		{
			if(r.left<m_location.right-d) m_location.left = r.left;
		}
		else if(m_edit==T)
		{
			if(r.top<m_location.bottom-d) m_location.top = r.top;
		}
		else if(m_edit==R)
		{
			if(r.left>m_location.left+d) m_location.right = r.left;
		}
		else if(m_edit==B)
		{
			if(r.top>m_location.top+d) m_location.bottom = r.top;
		}
		else if(m_edit==LT)
		{
			if(r.left<m_location.right-d) m_location.left = r.left;
			if(r.top<m_location.bottom-d) m_location.top = r.top;
		}
		else if(m_edit==RT)
		{
			if(r.left>m_location.left+d) m_location.right = r.left;
			if(r.top<m_location.bottom-d) m_location.top = r.top;
		}
		else if(m_edit==LB)
		{
			if(r.left<m_location.right-d) m_location.left = r.left;
			if(r.top>m_location.top+d) m_location.bottom = r.top;
		}
		else if(m_edit==RB)
		{
			if(r.left>m_location.left+d) m_location.right = r.left;
			if(r.top>m_location.top+d) m_location.bottom = r.top;
		}
		else if(m_edit==C)
		{
			float width = m_location.right - m_location.left, height = m_location.bottom - m_location.top;
			m_location.left = r.left - m_oldpos.left;
			m_location.top = r.top - m_oldpos.top;
			m_location.right = m_location.left + width;
			m_location.bottom = m_location.top + height;
		}
		UpdateObjectSize();
		UpdateSize();
	}
}

void ObjectEditor::MouseWheel(int, unsigned short, PointI const&)
{
}

void ObjectEditor::KeyDown(unsigned int key)
{
	if(m_object) m_object->KeyDown(key);
	if(Game->GetWindow()->KeyState(KEY_RCONTROL))
	{
		float width = m_location.right - m_location.left;
		float height = m_location.bottom - m_location.top;

		if(key==KEY_D)
		{
			m_location.right += 1.0f;
		}
		else if(key==KEY_A)
		{
			m_location.right -= 1.0f;
		}
		else if(key==KEY_W)
		{
			m_location.bottom -= 1.0f;
		}
		else if(key==KEY_S)
		{
			m_location.bottom += 1.0f;
		}
	}
	else if(Game->GetWindow()->KeyState(KEY_LCONTROL))
	{
		float width = m_location.right - m_location.left;
		float height = m_location.bottom - m_location.top;

		if(key==KEY_D)
		{
			m_location.left += 1.0f;
		}
		else if(key==KEY_A)
		{
			m_location.left -= 1.0f;
		}
		else if(key==KEY_W)
		{
			m_location.top -= 1.0f;
		}
		else if(key==KEY_S)
		{
			m_location.top += 1.0f;
		}
		else if(key==KEY_P)
		{
			if(Game->timeline && m_object)
			{
				Game->timeline->AddPositionKeys(m_object);
			}
		}
		else if(key==KEY_X)
		{
			if(Game->timeline && m_object)
			{
				Game->timeline->AddScaleKeys(m_object);
			}
		}
		else if(key==KEY_K)
		{
			if(Game->timeline && m_object)
			{
				Game->timeline->AddBColorKeys(m_object);
			}
		}
		else if(key==KEY_R)
		{
			if(Game->timeline && m_object)
			{
				Game->timeline->AddRotationKeys(m_object);
			}
		}
		else if(key==KEY_C)
		{
			if(Game->timeline)
			{
				Game->timeline->AddCPositionKeys(0);
			}
		}
		else if(key==KEY_Z)
		{
			if(Game->timeline)
			{
				Game->timeline->AddCZoomKeys(0);
			}
		}
	}
	else
	{
		float width = m_location.right - m_location.left;
		float height = m_location.bottom - m_location.top;

		if(key==KEY_D)
		{
			m_location.left += 1.0f;
			m_location.right = m_location.left + width;
		}
		else if(key==KEY_A)
		{
			m_location.left -= 1.0f;
			m_location.right = m_location.left + width;
		}
		else if(key==KEY_W)
		{
			m_location.top -= 1.0f;
			m_location.bottom = m_location.top + height;
		}
		else if(key==KEY_S)
		{
			m_location.top += 1.0f;
			m_location.bottom = m_location.top + height;
		}
	}

	UpdateObjectSize();
	UpdateSize();
}

void ObjectEditor::KeyUp(unsigned int key)
{
	if(m_object) m_object->KeyUp(key);
}

void ObjectEditor::KeyChar(wchar_t)
{

}

void ObjectEditor::SetObject(DrawableObject* val)
{
	m_object = val;
	if(m_object!=0)
	{
		if(m_object->m_flags & OBJECT_Flag_Tansform)
		{
			m_flags |= OBJECT_Flag_Tansform;
		}
		else
		{
			m_flags &= ~OBJECT_Flag_Tansform;
		}
	}
	UpdateSize();
	if(m_object!=0) m_flags &= ~OBJECT_Flag_Invisible;
	else m_flags |= OBJECT_Flag_Invisible;
}

DrawableObject* ObjectEditor::GetObject()
{
	return m_object;
}

void ObjectEditor::SetSpace(int val)
{
	m_space = val;
}

int ObjectEditor::GetSpace()
{
	return m_space;
}

//text
void GUIText::Process(unsigned int Msg, unsigned int Param1, void* Param2)
{
	AnimableObject::Process(Msg, Param1, Param2);
}

GUIText::GUIText( ResourceManager* mgr, unsigned int ID, RectangleF const& location, Resource sprite, unsigned int backcolor, float rotation, bool visible, unsigned int flags, ObjectsContainer* parent, bool update) : AnimableObject( ID, location, sprite, backcolor, rotation, visible, flags, parent, update)
{
	m_mgr = mgr;
	m_Font=0;
	if(mgr!=0) 
	{
		mgr->GetResource("DefaultFont", RES_TYPE_FONT, &m_Font);
	}
	m_DrawStyle=FONT_STYLE_CENTERALIGN | FONT_STYLE_VCENTERALIGN;
	m_Color=0xFFFFFFFF;
	m_ShadowColor = 0xFF000000;
	m_type|=OBJECT_TYPE_TEXT;
	m_Text=L"Text";
	m_Shadow=true;
	m_fscale = 1.0f;
}

void GUIText::Release()
{
	AnimableObject::Release();
}

//Resource GUIText::GetFont()
//{
//	return m_Font;
//}
//
//void GUIText::SetFont(Resource val)
//{
//	if(val->GetType()==RES_TYPE_FONT)
//	m_Font = val;
//}
//
//unsigned int GUIText::GetDrawStyle()
//{
//	return m_DrawStyle;
//}
//
//void GUIText::SetDrawStyle(unsigned int val)
//{
//	m_DrawStyle = val;
//}
//
//UIWString& GUIText::GetText()
//{
//	return m_Text;
//}
//
//void GUIText::SetText(UIWString const& val)
//{
//	m_Text = val;
//}
//
//unsigned int GUIText::GetColor()
//{
//	return m_Color;
//}
//
//void GUIText::SetColor(unsigned int val)
//{
//	m_Color = val;
//}
//
//unsigned int GUIText::GetShadowColor()
//{
//	return m_ShadowColor;
//}
//
//void GUIText::SetShadowColor(unsigned int val)
//{
//	m_ShadowColor = val;
//}
//
//bool GUIText::GetShadow()
//{
//	return m_Shadow;
//}
//
//void GUIText::SetShadow(bool val)
//{
//	m_Shadow = val;
//}
//
//RectangleF& GUIText::GetTextPadding()
//{
//	return m_TextPadding;
//}
//
//void GUIText::SetTextPadding(RectangleF const& val)
//{
//	m_TextPadding = val;
//}

void GUIText::Update( float delta )
{
	AnimableObject::Update(delta);
}

bool GUIText::Init()
{
	AnimableObject::Init();
	return true;
}

void GUIText::Draw(C_Renderer& rnd)
{
	RectangleF Pos = GetAbsolutePos();

	if(!(m_flags&OBJECT_Flag_NoBackground)) AnimableObject::Draw(rnd);

	Pos.left += m_TextPadding.left;
	Pos.right -= m_TextPadding.right;
	Pos.top += m_TextPadding.top;
	Pos.bottom -= m_TextPadding.bottom;

	RectangleI r;
	r.left = (int)Pos.left;
	r.top = (int)Pos.top;
	r.right = (int)Pos.right;
	r.bottom = (int)Pos.bottom;

	float scale = m_fscale;
	if(m_parent)
	{
		scale *= m_parent->GetCamera().m_scale;

		if(m_parent->m_parent!=0)
		{
			scale *= m_parent->m_parent->GetCamera().m_scale;
		}
	}

	if(m_Shadow)
	{
		r.left = (int)(r.left + 2 * scale);
		r.top = (int)(r.top + 2 * scale);
		rnd.DrawText(m_Font, m_Text.GetPtr(), &r, m_ShadowColor, m_DrawStyle, scale);

		r.left = (int)Pos.left;
		r.top = (int)Pos.top;
		r.right = (int)Pos.right;
		r.bottom = (int)Pos.bottom;
	}
	
	rnd.DrawText(m_Font, m_Text.GetPtr(), &r, m_Color, m_DrawStyle, scale);
	//rnd.DrawText(m_Font, m_Text.GetPtr(), r.left, r.top, r.right, r.bottom, m_Color, m_DrawStyle);
}

void GUIText::Save( BaseIO& str )
{
	unsigned int dt = 0;
	if(m_DrawStyle&FONT_STYLE_LEFTALIGN) dt |= 1;
	if(m_DrawStyle&FONT_STYLE_SINGLELINE) dt |= 128;
	if(m_DrawStyle&FONT_STYLE_RIGHTALIGN) dt |= 2;
	if(m_DrawStyle&FONT_STYLE_CENTERALIGN) dt |= 4;
	if(m_DrawStyle&FONT_STYLE_VCENTERALIGN) dt |= 8;
	if(m_DrawStyle&FONT_STYLE_TOPALIGN) dt |= 16;
	if(m_DrawStyle&FONT_STYLE_BOTTOMALIGN) dt |= 32;
	if(m_DrawStyle&FONT_STYLE_MULTILINE) dt |= 64;

	AnimableObject::Save(str);
	str.Write(&m_Font, 0, "name=Font;help=Font resource.;category=GUIText;default=2");

	str.Write(dt, 0, "name=Draw Style;help=Draw style;type=flags;list=Left align,Right align,Center align,Vertical center align,Top align,Bottom align,Multiline,Single line");
	str.Write(m_Text, 0, "name=Text;help=Text.");
	str.Write(m_Color, 0, "name=Font Color;help=Font color.;type=color");
	str.Write(m_fscale, 0, "name=Font Scale;help=Font scale.");
	str.Write(m_Shadow, 0, "name=Text Shadow;help=Text shadow.");
	str.Write(m_ShadowColor, 0, "name=Shadow Color;help=Shadow color.;type=color");
	str.Write(m_TextPadding.left, 0, "name=Padding left;help=Left padding.");
	str.Write(m_TextPadding.top, 0, "name=Padding top;help=Top padding.");
	str.Write(m_TextPadding.right, 0, "name=Padding right;help=Right padding.");
	str.Write(m_TextPadding.bottom, 0, "name=Padding bottom;help=Bottom padding.");
}

void GUIText::Load( BaseIO& str )
{	
	AnimableObject::Load(str);

	unsigned int dt = 0;
	wstring text;
	str.Read(&m_Font);
	str.Read(dt);
	str.Read(text);
	m_Text = text.c_str();
	str.Read(m_Color);
	str.Read(m_fscale);
	str.Read(m_Shadow);
	str.Read(m_ShadowColor);
	str.Read(m_TextPadding.left);
	str.Read(m_TextPadding.top);
	str.Read(m_TextPadding.right);
	str.Read(m_TextPadding.bottom);

	m_DrawStyle = 0;
	if(dt&1) m_DrawStyle |= FONT_STYLE_LEFTALIGN;
	if(dt&128) m_DrawStyle |= FONT_STYLE_SINGLELINE;
	if(dt&2) m_DrawStyle |= FONT_STYLE_RIGHTALIGN;
	if(dt&4) m_DrawStyle |= FONT_STYLE_CENTERALIGN;
	if(dt&8) m_DrawStyle |= FONT_STYLE_VCENTERALIGN;
	if(dt&16) m_DrawStyle |= FONT_STYLE_TOPALIGN;
	if(dt&32) m_DrawStyle |= FONT_STYLE_BOTTOMALIGN;
	if(dt&64) m_DrawStyle |= FONT_STYLE_MULTILINE;
}

Object* GUIText::Clone()
{
	/*GUIText* txt = new GUIText();
	txt->m_Font = m_Font;*/
	GUIText* obj = new GUIText();
	*obj = *this;
	return obj;
}

//Button
GUIButton::GUIButton( ResourceManager* mgr, unsigned int ID, RectangleF const& location, Resource sprite, unsigned int backcolor, float rotation, bool visible, unsigned int flags, ObjectsContainer* parent, bool update ) : GUIText( mgr, ID, location, sprite, backcolor, rotation, visible, flags, parent, update )
{
	if(mgr)
	{
		Resource res;
		mgr->GetResource("checkback", RES_TYPE_SPRITE, &res);
		if(res.IsValid()) 
		{
			m_CheckBack = *(res.GetPtr<Sprite>());
		}

		Resource res2;
		mgr->GetResource("checkcheck", RES_TYPE_SPRITE, &res2);
		if(res2.IsValid()) 
		{
			m_CheckCheck = *(res2.GetPtr<Sprite>());
		}

		Resource res3;
		mgr->GetResource("radioback", RES_TYPE_SPRITE, &res3);
		if(res3.IsValid()) 
		{
			m_RadioBack = *(res3.GetPtr<Sprite>());
		}

		Resource res4;
		mgr->GetResource("radiocheck", RES_TYPE_SPRITE, &res4);
		if(res4.IsValid())
		{
			m_RadioCheck = *(res4.GetPtr<Sprite>());
		}

		Resource res5;
		mgr->GetResource("buttonback", RES_TYPE_SPRITE, &res5);
		if(res5.IsValid()) 
		{
			m_sprite = *(res5.GetPtr<Sprite>());
		}

		m_Normal  = m_sprite;
		m_MouseOver = m_sprite;
		m_Disabled = m_sprite;

		//test
		/*Resource res6;
		mgr->GetResource("tooltip", RES_TYPE_SPRITE, &res6);
		if(res6.IsValid()) 
		{
			m_ToolTip = *(res6.GetPtr<Sprite>());
		}*/
	}

	m_maxttimer = 0.5f;
	m_ttimer = 0;
	m_type|=OBJECT_TYPE_BUTTON;
	state = GUIButton_State_Normal;
	prevstate = GUIButton_State_Normal;
	m_PressedColor=0xFF0F0F0F;
	m_NormalColor=0xDFFFFFFF;
	m_MouseOverColor=0xFFFFFFFF;
	m_DColor=0xFF101010;
	m_AnimationSpeed=0.0025f;
	CurrentAniPos=0.9f;
	CurrentAniPos2=0.9f;
	m_CheckedColor=0xFFFFFFFF;
	m_CCheckColor=0xFFFFFFFF;
	m_Animate = true;
	m_CheckBoxSize = 50;
	m_Shadow = false;
	m_Color = 0xFF000000;
	Checked=false;
	m_twidth = 128.0f;
	m_theight = 100.0f;
}

void GUIButton::Process(unsigned int Msg, unsigned int Param1, void* Param2)
{
	GUIText::Process(Msg, Param1, Param2);
}
	
void GUIButton::Release()
{
	GUIText::Release();
}

//General functions
void GUIButton::SetCheckBoxSize(unsigned int val)
{
	m_CheckBoxSize = val;
	if(m_flags&OBJECT_Flag_BTypeCheck || m_flags&OBJECT_Flag_BTypeRadio) m_TextPadding.left = (float)m_CheckBoxSize + 5;
}

unsigned int GUIButton::GetCheckBoxSize()
{
	return m_CheckBoxSize;
}

void GUIButton::Update( float delta )
{
	GUIText::Update( delta );

	if(GetState()==GUIButton_State_MouseOver)
	{
		m_ttimer += delta;
		if(m_ttimer>m_maxttimer)
		{
			m_ttimer = m_maxttimer;
		}
	}

	delta = delta * 1000.0f;

	if(CurrentAniPos==1 && CurrentAniPos2==1) return;
	if(m_Animate==true)
	{
		CurrentAniPos += m_AnimationSpeed * delta;
		CurrentAniPos2 += m_AnimationSpeed * delta;
		if(CurrentAniPos>=1) CurrentAniPos = 1;
		if(CurrentAniPos2>=1) CurrentAniPos2 = 1;
	}
	else
	{
		CurrentAniPos=1;
		CurrentAniPos2=1;
	}

	//Animate
	ColorARGB32 first;
	ColorARGB32 second;

	if(this->GetPrevState()==GUIButton_State_MouseOver) first = this->m_MouseOverColor;
	else if(this->GetPrevState()==GUIButton_State_Normal) first = this->m_NormalColor;
	else if(this->GetPrevState()==GUIButton_State_Pressed) first = this->m_PressedColor;

	if(this->GetState()==GUIButton_State_MouseOver) second = this->m_MouseOverColor;
	else if(this->GetState()==GUIButton_State_Normal) second = this->m_NormalColor;
	else if(this->GetState()==GUIButton_State_Pressed) second = this->m_PressedColor;

	ColorARGB32 dt;
	dt.SetA((unsigned char) (first.GetA() + (second.GetA() - first.GetA()) * (this->GetAnimationPos())));
	dt.SetR((unsigned char) (first.GetR() + (second.GetR() - first.GetR()) * (this->GetAnimationPos())));
	dt.SetG((unsigned char) (first.GetG() + (second.GetG() - first.GetG()) * (this->GetAnimationPos())));
	dt.SetB((unsigned char) (first.GetB() + (second.GetB() - first.GetB()) * (this->GetAnimationPos())));
	this->m_backcolor = dt;

	if(this->GetChecked()==true)
	{
		first = this->m_CheckedColor;
		second = this->m_CheckedColor;
		first.SetA(0);

		dt.SetA((unsigned char) (first.GetA() + (second.GetA() - first.GetA()) * (this->GetAnimationPos2())));
		dt.SetR((unsigned char) (first.GetR() + (second.GetR() - first.GetR()) * (this->GetAnimationPos2())));
		dt.SetG((unsigned char) (first.GetG() + (second.GetG() - first.GetG()) * (this->GetAnimationPos2())));
		dt.SetB((unsigned char) (first.GetB() + (second.GetB() - first.GetB()) * (this->GetAnimationPos2())));
		this->m_CCheckColor = dt;
	}
	else
	{
		first = this->m_CheckedColor;
		second = this->m_CheckedColor;
		second.SetA(0);

		dt.SetA((unsigned char) (first.GetA() + (second.GetA() - first.GetA()) * (this->GetAnimationPos2())));
		dt.SetR((unsigned char) (first.GetR() + (second.GetR() - first.GetR()) * (this->GetAnimationPos2())));
		dt.SetG((unsigned char) (first.GetG() + (second.GetG() - first.GetG()) * (this->GetAnimationPos2())));
		dt.SetB((unsigned char) (first.GetB() + (second.GetB() - first.GetB()) * (this->GetAnimationPos2())));
		this->m_CCheckColor = dt;
	}
}

bool GUIButton::Init()
{
	return GUIText::Init();
}

void GUIButton::Draw(C_Renderer& rnd)
{
	RectangleF Pos = GetAbsolutePos();

	unsigned int checksize = m_CheckBoxSize;
	if(m_parent) checksize = (unsigned int)(checksize * m_parent->GetCamera().m_scale);

	Pos.right = Pos.left + checksize;
	Pos.bottom = Pos.top + checksize;

	RectangleI r;
	r.left = (int)Pos.left;
	r.top = (int)Pos.top;
	r.right = (int)Pos.right;
	r.bottom = (int)Pos.bottom;

	if(GetState()==GUIButton_State_MouseOver)
	{
		m_sprite = m_MouseOver;
	}
	else
	{
		m_sprite = m_Normal;
	}

	if(m_flags&OBJECT_Flag_Disabled)
	{
		m_sprite = m_Disabled;
		m_backcolor = m_DColor;
	}
	
	if(m_flags&OBJECT_Flag_BTypeCheck)
	{
		m_CheckBack.Draw(&rnd, Pos, m_backcolor, m_rotation, GetLayer());
		m_CheckCheck.Draw(&rnd, Pos, m_CCheckColor, m_rotation, GetLayer());


		GUIText::Draw(rnd);		
	}
	else if(m_flags&OBJECT_Flag_BTypeRadio)
	{
		m_RadioBack.Draw(&rnd, Pos, m_backcolor, m_rotation, GetLayer());
		m_RadioCheck.Draw(&rnd, Pos, m_CCheckColor, m_rotation, GetLayer());

		GUIText::Draw(rnd);
	}
	else
	{
		GUIText::Draw(rnd);
	}

	if(GetState()==GUIButton_State_MouseOver)
	{
		if(m_ttimer>=m_maxttimer)
		{
			Pos.left = (float)m_mousept.left;
			Pos.top = (float)m_mousept.top + 10.0f;
			Pos.right = Pos.left + m_twidth;
			Pos.bottom = Pos.top + m_theight;

			if(m_parent)
			{
				RectangleF loc = Game->GetMainContainer()->GetAbsolutePos();
				/*if(m_parent->GetParent()) loc = m_parent->GetParent()->GetAbsolutePos();
				else loc = m_parent->GetAbsolutePos();*/

				if(Pos.right > loc.right)
				{
					Pos.left -= m_twidth;
					Pos.right = Pos.left + m_twidth;
				}

				if(Pos.bottom > loc.bottom)
				{
					Pos.top -= m_theight;
					Pos.bottom = Pos.top + m_theight;
				}
			}

			m_ToolTip.Draw(&rnd, Pos, 0xFFFFFFFF, 0.0f, GetLayer());
		}
	}
}

void GUIButton::Save( BaseIO& str )
{
	GUIText::Save(str);
	
	m_CheckBack.Save(str, "category=GUIButton;name=Checkbox back");
	m_CheckCheck.Save(str, "name=Checkbox check");
	m_RadioBack.Save(str, "name=Radio Back");
	m_RadioCheck.Save(str, "name=Radio check");
	m_ToolTip.Save(str, "name=Tool tip");
	m_MouseOver.Save(str, "name=Mouse over");
	m_Normal.Save(str, "name=Mouse Normal");

	str.Write(m_PressedColor, 0, "name=Button pressed color;help=color of the button when it is presseed.;type=color");
	str.Write(m_NormalColor, 0, "name=Button normal color;help=color of the button when it is at normal state.;type=color");
	str.Write(m_MouseOverColor, 0, "name=Button mouse over color;help=color of the button when mouse pointer is over it.;type=color");
	str.Write(m_AnimationSpeed, 0, "name=Animation speed;help=Animation speed of the button transition.");
	str.Write(m_CheckedColor, 0, "name=Checked color;help=Button checked color.;type=color");
	str.Write(m_CCheckColor, 0, "name=CChecked color;help=Button checked color.;type=color");
	str.Write(m_Animate, 0, "name=Animate button;help=Animate button.");
	str.Write(m_CheckBoxSize, 0, "name=Checkbox size;help=checkbox size.");
	str.Write(Checked, 0, "name=Checked;help=checkbox checked.");
	str.Write(&sound, 0, "name=Select Sound;help=SelectSound.;default=3");
}

void GUIButton::Load( BaseIO& str )
{
	GUIText::Load(str);

	m_CheckBack.Load(str);
	m_CheckCheck.Load(str);
	m_RadioBack.Load(str);
	m_RadioCheck.Load(str);
	m_ToolTip.Load(str);
	m_MouseOver.Load(str);
	m_Normal.Load(str);

	str.Read(m_PressedColor);
	str.Read(m_NormalColor);
	str.Read(m_MouseOverColor);
	str.Read(m_AnimationSpeed);
	str.Read(m_CheckedColor);
	str.Read(m_CCheckColor);
	str.Read(m_Animate);

	unsigned int csize = 0;
	str.Read(csize);
	SetCheckBoxSize(csize);

	bool checked = false;
	str.Read(checked);
	SetChecked(checked);

	str.Read(&sound);
	splayer.SetBuffer(sound);
}

void GUIButton::SetState(unsigned int st)
{
	prevstate = state;
	state = st;
	if(CurrentAniPos < 0.3) CurrentAniPos =  0;
	else CurrentAniPos = 1-CurrentAniPos;
	if(state == GUIButton_State_MouseOver)
	{
		splayer.Play(false);
	}
}

unsigned int GUIButton::GetPrevState()
{
	return prevstate;
}

unsigned int GUIButton::GetState()
{
	return state;
}

float GUIButton::GetAnimationPos()
{
	return CurrentAniPos;
}

float GUIButton::GetAnimationPos2()
{
	return CurrentAniPos2;
}

void GUIButton::SetChecked(bool chk)
{
	Checked = chk;
	CurrentAniPos2 = 1-CurrentAniPos2;
	OnChecked();
	if(m_parent!=0)
	{
		((ObjectsContainer*)m_parent)->Callback(this, OBJECT_Message_BSetChecked, 0, 0);
	}
}

bool GUIButton::GetChecked()
{
	return Checked;
}

//Input
void GUIButton::MouseDown(unsigned short button, unsigned short buttons, PointI const& pt)
{
	GUIText::MouseDown(button, buttons, pt);

	m_ttimer = 0;

	if(button == KEY_LBUTTON)
	{
		if(m_flags&OBJECT_Flag_BTypePush)
		{
			if(state == GUIButton_State_Pressed)
			{
				SetState(GUIButton_State_Normal);
				SetCapture();
			}
			else
			{
				SetState(GUIButton_State_Pressed);
				ReleaseCapture();
			}
		}
		else if(m_flags&OBJECT_Flag_BTypeCheck)
		{
			if(Checked)
			{
				SetChecked(false);
			}
			else
			{
				SetChecked(true);
			}
		}
		else if(m_flags&OBJECT_Flag_BTypeRadio)
		{
			if(!Checked) SetChecked(true);
		}
		else
		{
			SetState(GUIButton_State_Pressed);
			SetCapture();
		}
	}
}

void GUIButton::MouseUp(unsigned short button, unsigned short buttons, PointI const& pt)
{
	GUIText::MouseUp(button, buttons, pt);
	if(button == KEY_LBUTTON)
	{
		if(!(m_flags&OBJECT_Flag_BTypePush || m_flags&OBJECT_Flag_BTypeCheck || m_flags&OBJECT_Flag_BTypeRadio))
		{
			if(PointI::Intersect(pt, GetAbsolutePos()))
			{
				//SetState(GUIButton_State_MouseOver);
				SetState(GUIButton_State_Normal);
				ReleaseCapture();
				if(m_parent!=0)
				{
					((ObjectsContainer*)m_parent)->Callback(this, OBJECT_Message_LMouseClick, 0, 0);	
					OnClick();									
				}
			}
			else
			{
				SetState(GUIButton_State_Normal);
				ReleaseCapture();
			}
		}
		else
		{
			/*if(state == GUIButton_State_MouseOver)
			{
				SetState(GUIButton_State_Normal);
			}*/
			ReleaseCapture();
		}
	}
}

void GUIButton::MouseMove(PointI const& pt)
{
	GUIText::MouseMove(pt);

	if(state==GUIButton_State_Pressed) 
	{
		/*if(m_parent!=0)
		{
			((ObjectsContainer*)m_parent)->Callback(this, OBJECT_Message_BMouseNormal, 0, 0);
		}*/
		return;
	}
	if(state==GUIButton_State_MouseOver)
	{
		m_mousept = pt;
		if(!PointI::Intersect(pt, GetAbsolutePos()))
		{
			ReleaseCapture();
			SetState(GUIButton_State_Normal);
			if(m_parent!=0)
			{
				((ObjectsContainer*)m_parent)->Callback(this, OBJECT_Message_BMouseNormal, 0, 0);
			}
		}
		else
		{
			SetCapture();
		}
	}
	else
	{
		SetState(GUIButton_State_MouseOver);
		SetCapture();
		if(m_parent!=0)
		{
			((ObjectsContainer*)m_parent)->Callback(this, OBJECT_Message_BMouseOver, 0, 0);
		}
		m_ttimer = 0;
		m_mousept = pt;
	}
}

void GUIButton::MouseWheel(int delta, unsigned short buttons, PointI const& pt)
{
	GUIText::MouseWheel(delta, buttons, pt);
}

void GUIButton::KeyDown(unsigned int key)
{
	GUIText::KeyDown(key);
}

void GUIButton::KeyUp(unsigned int key)
{
	GUIText::KeyUp(key);
}

void GUIButton::KeyChar(wchar_t c)
{
	GUIText::KeyChar(c);
}

Object* GUIButton::Clone()
{
	GUIButton* btn = new GUIButton();
	*btn = *this;
	return btn;
}

//Edit
unsigned int GUIEdit::GetTextLength(unsigned int start, unsigned int end)
{
	if(m_renderer==0) return 0;
	if(end==0) end= m_Text.Length() - 1 - start;
	if(start + end > m_Text.Length() - 1) end= m_Text.Length() - 1 - start;

	RectangleI rc(0,0,0,0);
	RectangleI r2;
	unsigned int spwidth=0;
	UIWString str;

	unsigned int st=0;

	str = L"0 0";

	r2.left = 0;
	r2.right = 0;
	r2.bottom = 0;
	r2.top = 0;

	float scale = 1.0f;
	if(m_parent)
	{
		scale = m_parent->GetCamera().m_scale;
	}

	m_renderer->CalculateRect(m_Font, &r2, str.GetPtr(), 0, 1, m_DrawStyle, scale);
	r2.left = (int)(r2.left * scale);
	r2.top = (int)(r2.top * scale);
	r2.right = (int)(r2.right * scale);
	r2.bottom = (int)(r2.bottom * scale);

	st = r2.right;

	m_renderer->CalculateRect(m_Font, &r2, str.GetPtr(), 0, 3, m_DrawStyle, scale);
	r2.left = (int)(r2.left * scale);
	r2.top = (int)(r2.top * scale);
	r2.right = (int)(r2.right * scale);
	r2.bottom = (int)(r2.bottom * scale);

	spwidth = r2.right - st*2;

	m_renderer->CalculateRect(m_Font, &rc, m_Text.GetPtr(), start, end, m_DrawStyle, scale);
	rc.left = (int)(rc.left * scale);
	rc.top = (int)(rc.top * scale);
	rc.right = (int)(rc.right * scale);
	rc.bottom = (int)(rc.bottom * scale);

	for(unsigned int i=1; i<=end-start; i++)
	{
		if(end-start-i>=0)
		{
			if(m_Text[end-start-i]==L' ') rc.right += spwidth;
			else break;
		}
	}

	return rc.right;
}

unsigned int GUIEdit::GetDrawFrom()
{
	return DrawFrom;
}

unsigned int GUIEdit::GetPosFromPoint(RectangleI r1, PointI* pt)
{
	if(m_renderer==0) return 0;
	RectangleI r2;
	unsigned int i=0;
	unsigned int spwidth=0;
	UIWString str;

	unsigned int st=0;

	unsigned int width=0;

	str = L"0 0";

	r2.left = 0;
	r2.right = 0;
	r2.bottom = 0;
	r2.top = 0;

	float scale = 1.0f;
	if(m_parent)
	{
		scale = m_parent->GetCamera().m_scale;
	}

	m_renderer->CalculateRect(m_Font, &r2, str.GetPtr(), 0, 1, m_DrawStyle, scale);
	r2.left = (int)(r2.left * scale);
	r2.top = (int)(r2.top * scale);
	r2.right = (int)(r2.right * scale);
	r2.bottom = (int)(r2.bottom * scale);

	st = r2.right;

	m_renderer->CalculateRect(m_Font, &r2, str.GetPtr(), 0, 3, m_DrawStyle, scale);
	r2.left = (int)(r2.left * scale);
	r2.top = (int)(r2.top * scale);
	r2.right = (int)(r2.right * scale);
	r2.bottom = (int)(r2.bottom * scale);

	spwidth = r2.right - st*2;

	r2 = r1;

	for(i=DrawFrom; i<m_Text.Length()-1; i++)
	{
		if(m_Text[i]==L' ')  r2.right = r2.left + spwidth;
		else m_renderer->CalculateRect(m_Font, &r2, m_Text.GetPtr(), i, 1, m_DrawStyle, scale);

		r2.left = (int)(r2.left * scale);
		r2.top = (int)(r2.top * scale);
		r2.right = (int)(r2.right * scale);
		r2.bottom = (int)(r2.bottom * scale);

		width = r2.right - r2.left;

		if(PointI::Intersect(*(pt), RectangleF((float)r2.left, (float)r2.top, (float)r2.right - width/3, (float)r2.bottom)))
		{
			return i;
		}
		else if(PointI::Intersect(*(pt), RectangleF((float)r2.left, (float)r2.top, (float)r2.right, (float)r2.bottom)))
		{
			return i+1;
		}
		r2.left = r2.right;
	}
	return ERR_CONST;
}

bool GUIEdit::GetRectFromPos(RectangleI* rect, unsigned int pos)
{
	if(!m_renderer) return 0;
	RectangleI r1;
	r1 = *rect;

	RectangleI r2;
	unsigned int spwidth=0;
	UIWString str;

	unsigned int st=0;

	str = L"0 0";

	r2.left = 0;
	r2.right = 0;
	r2.bottom = 0;
	r2.top = 0;

	float scale = 1.0f;
	if(m_parent)
	{
		scale = m_parent->GetCamera().m_scale;
	}

	m_renderer->CalculateRect(m_Font, &r2, str.GetPtr(), 0, 1, m_DrawStyle, scale);
	r2.left = (int)(r2.left * scale);
	r2.top = (int)(r2.top * scale);
	r2.right = (int)(r2.right * scale);
	r2.bottom = (int)(r2.bottom * scale);

	st = r2.right;

	m_renderer->CalculateRect(m_Font, &r2, str.GetPtr(), 0, 3, m_DrawStyle, scale);
	r2.left = (int)(r2.left * scale);
	r2.top = (int)(r2.top * scale);
	r2.right = (int)(r2.right * scale);
	r2.bottom = (int)(r2.bottom * scale);

	spwidth = r2.right - st*2;

	m_renderer->CalculateRect(m_Font, &r1, str.GetPtr(), 0, 1, m_DrawStyle, scale);
	r1.left = (int)(r1.left * scale);
	r1.top = (int)(r1.top * scale);
	r1.right = (int)(r1.right * scale);
	r1.bottom = (int)(r1.bottom * scale);

	rect->top = r1.top;
	rect->bottom = r1.bottom;	

	if(pos<=GetDrawFrom()) return false;
	r1 = *rect;
	m_renderer->CalculateRect(m_Font, &r1, m_Text.GetPtr(), GetDrawFrom(), pos - GetDrawFrom(), m_DrawStyle, scale);
	r1.left = (int)(r1.left * scale);
	r1.top = (int)(r1.top * scale);
	r1.right = (int)(r1.right * scale);
	r1.bottom = (int)(r1.bottom * scale);
	
	for(unsigned int i=1; i<=m_SelStart; i++)
	{
		if(pos-i>=0)
		{
			if(m_Text[pos-i]==L' ') r1.right += spwidth;
			else break;
		}
	}

	rect->left = r1.right;
	return true;
}

bool GUIEdit::AdjustPosition(unsigned int pos)
{
	RectangleI r1, r2;
	r1.left = (int)GetAbsolutePos().left;
	r1.top = (int)GetAbsolutePos().top;
	r1.right = (int)GetAbsolutePos().right;
	r1.bottom = (int)GetAbsolutePos().bottom;

	r1.left += (int)m_TextPadding.left;
	r1.top += (int)m_TextPadding.top;
	r1.right -= (int)m_TextPadding.right;
	r1.bottom -= (int)m_TextPadding.bottom;
	r2=r1;

	GetRectFromPos(&r2, pos);

	if(r2.left >= r1.right)
	{
		if(DrawFrom+1<m_Text.Length()-1)
		{
			DrawFrom += 1;
			return true;
		}
		else return false;
	}
	else if(r2.left <= r1.left)
	{
		if((int)DrawFrom-1>=0)
		{
			DrawFrom -= 1;
			return true;
		}
		else return false;
	}
	else
	{
		return false;
	}
	return false;
}

GUIEdit::GUIEdit( C_Renderer* renderer, ResourceManager* mgr, unsigned int ID, RectangleF const& location, Resource sprite, unsigned int backcolor, float rotation, bool visible, unsigned int flags, ObjectsContainer* parent, bool update ) : GUIText( mgr, ID, location, sprite, backcolor, rotation, visible, flags, parent, update )
{
	if(mgr)
	{
		Resource res;
		mgr->GetResource("editback2", RES_TYPE_SPRITE, &res);
		if(res.IsValid()) 
		{
			EditBack = *(res.GetPtr<Sprite>());
		}

		Resource res5;
		mgr->GetResource("editback", RES_TYPE_SPRITE, &res5);
		if(res5.IsValid()) 
		{
			m_sprite = *(res5.GetPtr<Sprite>());
		}
	}

	m_renderer = renderer;
	m_type|=OBJECT_TYPE_EDIT;
	Selecting=false;
	m_location.bottom = m_location.top + 50;
	m_location.right = m_location.left + 150;
	m_DrawStyle = FONT_STYLE_LEFTALIGN | FONT_STYLE_VCENTERALIGN | FONT_STYLE_SINGLELINE;
	DrawFrom=m_SelStart=m_SelLength=ETime=0;
	m_SelColor=0xFF005FCF;
	m_CColor=0xFF000000;
	m_Color=0xFF000000;
	m_Bk2Color=0xFFFFFFFF;
	m_MouseBounds = 20;
	m_MaxLength = 100;
	m_TextPadding = RectangleF(10,5,10,5);
	SelectionStart=0;
	m_CurVisible=true;
	m_BlinkSpeed = 500;
}

void GUIEdit::Release()
{
	GUIText::Release();
}

void GUIEdit::Process(unsigned int Msg, unsigned int Param1, void* Param2)
{
	GUIText::Process(Msg, Param1, Param2);
}

//General functions
void GUIEdit::Update( float delta )
{
	GUIText::Update(delta);
	ETime += (unsigned int)(delta * 1000.0f);
	if(ETime>m_BlinkSpeed)
	{
		if(m_CurVisible)m_CurVisible=false;
		else m_CurVisible=true;
		ETime = 0;
	}
}

bool GUIEdit::Init()
{
	return GUIText::Init();
}

void GUIEdit::Draw(C_Renderer& rnd)
{
	RectangleF Pos = GetAbsolutePos();
	if(!(m_flags&OBJECT_Flag_NoBackground))	DrawableObject::Draw(rnd);

	Pos.left += m_TextPadding.left;
	Pos.top += m_TextPadding.top;
	Pos.right -= m_TextPadding.right;
	Pos.bottom -= m_TextPadding.bottom;

	EditBack.Draw(&rnd, Pos, m_Bk2Color, m_rotation, GetLayer());

	RectangleI rect2;
	rect2.left = (int)Pos.left;
	rect2.top = (int)Pos.top;
	rect2.right = (int)Pos.right;
	rect2.bottom = (int)Pos.bottom;

	float scale = 1.0f;
	if(m_parent)
	{
		scale = m_parent->GetCamera().m_scale;
	}

	if(m_SelLength >0)
	{
		RectangleI rect3;
		rect3.left = (int)Pos.left;
		rect3.top = (int)Pos.top;
		rect3.right = (int)Pos.right;
		rect3.bottom = (int)Pos.bottom;

		//1
		GetRectFromPos(&rect3, m_SelStart);

		//2nd
		GetRectFromPos(&rect2, m_SelStart+m_SelLength);		

		if(rect2.left < rect3.right)
		rect3.right = rect2.left;
		
		//end
		rect2.left = (int)Pos.left;
		rect2.top = (int)Pos.top;
		rect2.right = (int)Pos.right;
		rect2.bottom = (int)Pos.bottom;

		EditBack.Draw(&rnd, RectangleF((float)rect3.left, (float)rect3.top, (float)rect3.right, (float)rect3.bottom), m_SelColor, m_rotation, GetLayer());
	}

	rnd.DrawText(m_Font, m_Text.GetPtr() + GetDrawFrom(), &rect2, m_Color, m_DrawStyle, scale);
		
	if(m_CurVisible && GetSelected() && !m_SelLength)
	if(GetDrawFrom()<=m_SelStart)
	{	
		GetRectFromPos(&rect2, m_SelStart);

		/*PointF vec[2];
		vec[0].left = (float)rect2.left;
		vec[1].left = (float)rect2.left;
		vec[0].top = (float)rect2.top;
		vec[1].top = (float)rect2.bottom;
		rnd.DrawLine(vec[0], vec[1], m_CColor);*/
		EditBack.Draw(&rnd, RectangleF((float)rect2.left, (float)rect2.top, (float)rect2.left+2, (float)rect2.bottom), m_CColor, 0, GetLayer());
	}
}

void GUIEdit::Save( BaseIO& str )
{
	GUIText::Save(str);

	EditBack.Save(str, "category=GUIEdit;name=Edit background.");
	str.Write(m_MaxLength, 0, "name=Max length;help=Max number of characters edit of can have.");
	str.Write(m_SelColor, 0, "name=Selection color;help=Selection color;type=color");
	str.Write(m_CColor, 0, "name=Cursor color;help=Color of the cursor.;type=color");
	str.Write(m_Bk2Color, 0, "name=Back2 color;help=Second background color.;type=color");
	str.Write(m_BlinkSpeed, 0, "name=Blink speed;help=Cursor blink speed.");
	str.Write(m_MouseBounds, 0, "name=Mouse bounds;help=Mouse bounds used for auto text clipping using mouse cursor.");
	str.Write(m_SelStart, 0, "name=Selection start;help=Text selection start.");
	str.Write(m_SelLength, 0, "name=Selection length;help=Text selection length.");
}

void GUIEdit::Load( BaseIO& str )
{
	GUIText::Load(str);

	EditBack.Load(str);
	str.Read(m_MaxLength);
	str.Read(m_SelColor);
	str.Read(m_CColor);
	str.Read(m_Bk2Color);
	str.Read(m_BlinkSpeed);
	str.Read(m_MouseBounds);
	str.Read(m_SelStart);
	str.Read(m_SelLength);
}

Object* GUIEdit::Clone()
{
	GUIEdit* obj = new GUIEdit();
	*obj = *this;
	return obj;
}

//Input
void GUIEdit::MouseDown(unsigned short button, unsigned short buttons, PointI const& pt)
{
	GUIText::MouseDown(button, buttons, pt);
	if(button == KEY_LBUTTON)
	{
		RectangleI r1;
		r1.left = (int)GetAbsolutePos().left;
		r1.top = (int)GetAbsolutePos().top;
		r1.right = (int)GetAbsolutePos().right;
		r1.bottom = (int)GetAbsolutePos().bottom;

		r1.left += (int)m_TextPadding.left;
		r1.top += (int)m_TextPadding.top;
		r1.right -= (int)m_TextPadding.right;
		r1.bottom -= (int)m_TextPadding.bottom;
		Selecting = true;
		m_SelLength = 0;
		SetCapture();

		unsigned int tmp = GetPosFromPoint(r1, const_cast<PointI*>(&pt));
		if(tmp!=ERR_CONST)			
		m_SelStart = tmp;	

		if(PointI::Intersect(pt, RectangleF((float)r1.left, (float)r1.top, (float)r1.left+m_MouseBounds, (float)r1.bottom)))
		{
			if((int)DrawFrom - 1>=0) DrawFrom -= 1;
		}
		else if(PointI::Intersect(pt, RectangleF((float)r1.right-m_MouseBounds, (float)r1.top, (float)r1.right, (float)r1.bottom)))
		{
			if((int)(GetTextLength(DrawFrom, 0)+m_MouseBounds)+r1.left>r1.right)
			DrawFrom += 1;
		}
		SelectionStart = m_SelStart;
		if(m_parent!=0)
		{
			((ObjectsContainer*)m_parent)->Callback(this, OBJECT_Message_EDRedrawText, 0, 0);
		}
	}
}

void GUIEdit::MouseUp(unsigned short button, unsigned short buttons, PointI const& pt)
{
	GUIText::MouseUp(button, buttons, pt);
	ReleaseCapture();
	Selecting = false;
	if(m_parent!=0)
	{
		((ObjectsContainer*)m_parent)->Callback(this, OBJECT_Message_EDRedrawText, 0, 0);
	}
}

void GUIEdit::MouseMove(PointI const& pt)
{
	GUIText::MouseMove(pt);
	if(Selecting)
	{
		RectangleI r1;
		r1.left = (int)GetAbsolutePos().left;
		r1.top = (int)GetAbsolutePos().top;
		r1.right = (int)GetAbsolutePos().right;
		r1.bottom = (int)GetAbsolutePos().bottom;

		r1.left += (int)m_TextPadding.left;
		r1.top += (int)m_TextPadding.top;
		r1.right -= (int)m_TextPadding.right;
		r1.bottom -= (int)m_TextPadding.bottom;

		if(pt.left > r1.right) return;
		if(pt.left < r1.left) return;

		unsigned int tmp = GetPosFromPoint(r1, const_cast<PointI*>(&pt));
		if(tmp!=ERR_CONST)
		{
			if(tmp<=SelectionStart)
			{
				m_SelLength = SelectionStart - tmp;
				m_SelStart = tmp;
			}
			else
			{
				m_SelLength = tmp - SelectionStart;
			}
		}

		if(PointI::Intersect(pt, RectangleF((float)r1.left, (float)r1.top, (float)r1.left+m_MouseBounds, (float)r1.bottom)))
		{
			if((int)DrawFrom - 1>=0) DrawFrom -= 1;
		}
		else if(PointI::Intersect(pt, RectangleF((float)r1.right-m_MouseBounds, (float)r1.top, (float)r1.right, (float)r1.bottom)))
		{
			if((int)(GetTextLength(DrawFrom, 0)+m_MouseBounds)+r1.left>r1.right)
			DrawFrom += 1;
		}
	}
	if(m_parent!=0)
	{
		((ObjectsContainer*)m_parent)->Callback(this, OBJECT_Message_EDRedrawText, 0, 0);
	}
}

void GUIEdit::MouseWheel(int wheel, unsigned short buttons, PointI const& pt)
{
	GUIText::MouseWheel(wheel, buttons, pt);
}

void GUIEdit::DeleteChar(int val)
{
	if(m_SelStart<m_Text.Length()-1 && val==0)
	{
		if(m_SelLength==0)
		{
			m_Text.DeleteString(m_SelStart, 1);
		}
		else
		{
			m_Text.DeleteString(m_SelStart, m_SelLength);
			m_SelLength = 0;
		}
	}
	else if(m_SelStart>0 && val==1)
	{
		if(m_SelLength==0)
		{
			m_Text.DeleteString(m_SelStart-1, 1);
			m_SelStart -= 1;
		}
		else
		{
			m_Text.DeleteString(m_SelStart, m_SelLength);
			m_SelLength = 0;
		}
	}
	while(AdjustPosition(m_SelStart)){}
	if(m_parent!=0)
	{
		((ObjectsContainer*)m_parent)->Callback(this, OBJECT_Message_EDRedrawText, 0, 0);
	}
	if(m_parent!=0)
	{
		((ObjectsContainer*)m_parent)->Callback(this, OBJECT_Message_EDTextChange, 0, 0);
	}
}

void GUIEdit::MoveCursor(int val)
{
	if(m_SelLength>0)
	{
		if(val>0)
		{
			m_SelStart = m_SelStart+m_SelLength;
			m_SelLength = 0;
		}
		else
		{
			m_SelLength = 0;
		}
		while(AdjustPosition(m_SelStart)){}
	}
	else
	{
		if(m_SelStart + val>=0 && (int)m_SelStart + val<(int)m_Text.Length())
		{
			m_SelStart += val;
			while(AdjustPosition(m_SelStart)){}
		}
	}
	if(m_parent!=0)
	{
		((ObjectsContainer*)m_parent)->Callback(this, OBJECT_Message_EDRedrawText, 0, 0);
	}
}

void GUIEdit::KeyDown(unsigned int key)
{
	GUIText::KeyDown(key);
	if(key==KEY_DELETE)
	{
		DeleteChar(0);
	}
	else if(key==KEY_BACK)
	{
		DeleteChar(1);
	}
	else if(key==KEY_LEFT)
	{
		MoveCursor(-1);
	}
	else if(key==KEY_RIGHT)
	{
		MoveCursor(1);		
	}
}

void GUIEdit::KeyUp(unsigned int key)
{
	GUIText::KeyUp(key);
}

void GUIEdit::KeyChar(wchar_t key)
{
	GUIText::KeyChar(key);
	if(key!=0x08 && key!=0x0A && key!=0x1B && key!=0x0D && key!=0x09)
	{
		if(m_Text.Length()>=m_MaxLength) return;
		m_Text.DeleteString(m_SelStart, m_SelLength);
		m_SelLength = 0;
		m_Text.InsertString(key, m_SelStart);
		m_SelStart += 1;
		while(AdjustPosition(m_SelStart)){}
		if(m_parent!=0)
		{
			((ObjectsContainer*)m_parent)->Callback(this, OBJECT_Message_EDRedrawText, 0, 0);
		}
		if(m_parent!=0)
		{
			((ObjectsContainer*)m_parent)->Callback(this, OBJECT_Message_EDTextChange, 0, 0);
		}
	}
}

// ProgressBar
float GUIProgress::GetMin()
{
	return Min;
}

float GUIProgress::GetMax()
{
	return Max;
}

float GUIProgress::GetValue()
{
	return Value;
}

void GUIProgress::SetMin(float val)
{
	if(Max>val) Min=val;
	if(Value<Min) Value=Min;
}

void GUIProgress::SetMax(float val)
{
	if(val>Min) Max=val;
	if(Value>Max) Value=Max;
}

void GUIProgress::SetValue(float val)
{
	val = max(Min, val);
	val = min(Max, val);
	if(val>=Min && val<=Max)
	{
		Value = val;
		if(m_parent!=0)
		{
			((ObjectsContainer*)m_parent)->Callback(this, OBJECT_Message_PValueChanged, (unsigned int)GetValue(), 0);
			OnValueChanged();
		}

		if(imgprogress)
		{
			float mul = (GetValue()-GetMin())/(GetMax()-GetMin());
			int indsize = endind - startind;
			unsigned int ind = (unsigned int)(startind + indsize * mul);
			m_progressback.SetIndex(ind);
		}
	}
}

GUIProgress::GUIProgress( ResourceManager* mgr, unsigned int ID, RectangleF const& location, Resource sprite, unsigned int backcolor, float rotation, bool visible, unsigned int flags, ObjectsContainer* parent, bool update) : DrawableObject( ID, location, sprite, backcolor, rotation, visible, flags, parent, update )
{
	if(mgr)
	{
		Resource res;
		mgr->GetResource("progress", RES_TYPE_SPRITE, &res);
		if(res.IsValid()) 
		{
			m_progressback = *(res.GetPtr<Sprite>());
		}

		Resource res5;
		mgr->GetResource("progressback", RES_TYPE_SPRITE, &res5);
		if(res5.IsValid()) 
		{
			m_sprite = *(res5.GetPtr<Sprite>());
		}
	}

	m_type|=OBJECT_TYPE_PROGRESS;
	Min=0;
	Max=100;
	Value=0;
	m_Region = RectangleF(10,10,10,10);
	m_ProgressColor=0xFF001FCF;
	m_Bk2Color=0xFFFFFF;
	m_ThumbSize=20;
	Selecting=0;
	m_MoveSize=1;
	clipping = false;
	imgprogress = false;
	startind  = 0; 
	endind = 21;
}

void GUIProgress::Release()
{
	DrawableObject::Release();
}

void GUIProgress::Process(unsigned int Msg, unsigned int Param1, void* Param2)
{
	DrawableObject::Process(Msg, Param1, Param2);
}

//General functions
void GUIProgress::Update( float delta )
{
	DrawableObject::Update(delta);
}

bool GUIProgress::Init()
{
	return DrawableObject::Init();
}

void GUIProgress::Draw(C_Renderer& rnd)
{
	//DrawableObject::Draw(rnd);

	RectangleF Pos = GetAbsolutePos();
	
	if(m_flags&OBJECT_Flag_PTypeSlider)
	{
		if(!(m_flags&OBJECT_Flag_NoBackground)) DrawableObject::Draw(rnd);
		
		Pos.left += m_Region.left;
		Pos.top += m_Region.top;
		Pos.right -= m_Region.right;
		Pos.bottom -= m_Region.bottom;

		m_progressback.Draw(&rnd, Pos, m_Bk2Color, m_rotation, GetLayer());

		float mul = (GetValue()-GetMin())/(GetMax()-GetMin());
		if(m_flags&OBJECT_Flag_PTypeVertical  && m_flags&OBJECT_Flag_PTypeInv)
		{
			Pos.top = (Pos.top + (Pos.bottom - Pos.top) * (mul)) - m_ThumbSize / 2;
			Pos.bottom = Pos.top + m_ThumbSize;
			Pos.left -= m_Region.left;
			Pos.right += m_Region.right;
		}
		else if(m_flags&OBJECT_Flag_PTypeVertical)
		{
			Pos.top = (Pos.top + (Pos.bottom - Pos.top) * (1-mul)) - m_ThumbSize / 2;
			Pos.bottom = Pos.top + m_ThumbSize;
			Pos.left -= m_Region.left;
			Pos.right += m_Region.right;
		}
		else
		{
			Pos.left = (Pos.left + (Pos.right - Pos.left) * mul) - m_ThumbSize / 2;
			Pos.right = Pos.left + m_ThumbSize;
			Pos.top -= m_Region.top;
			Pos.bottom += m_Region.bottom;
		}

		m_progressback.Draw(&rnd, Pos, m_ProgressColor, m_rotation, GetLayer());
	}
	else
	{
		if(!(m_flags&OBJECT_Flag_NoBackground)) DrawableObject::Draw(rnd);

		Pos.left += m_Region.left;
		Pos.top += m_Region.top;
		Pos.right -= m_Region.right;
		Pos.bottom -= m_Region.bottom;		

		if(imgprogress)
		{
			m_progressback.Draw(&rnd, Pos, m_ProgressColor, m_rotation, GetLayer());
		}
		else
		{
			if(!(m_flags&OBJECT_Flag_NoBackground))
			{
				if(m_flags&OBJECT_Flag_PTypeVertical)
				{
					m_progressback.Draw(&rnd, Pos, m_ProgressColor, m_rotation, GetLayer());
				}
				else
				{
					m_progressback.Draw(&rnd, Pos, m_Bk2Color, m_rotation, GetLayer());
				}
			}

			float mul = (GetValue()-GetMin())/(GetMax()-GetMin());
			if(m_flags&OBJECT_Flag_PTypeVertical)
			{
				Pos.bottom = (Pos.top + (Pos.bottom - Pos.top) * (1-mul));
			}
			else
			{
				Pos.right = (Pos.left + (Pos.right - Pos.left) * mul);
			}

			if(m_flags&OBJECT_Flag_PTypeVertical)
			{
				if(clipping)
				{
					RectangleF pos2;

					pos2.left = 0;
					pos2.top = 0;
					pos2.right = 1;
					pos2.bottom = (1-mul);

					m_progressback.DrawArea(&rnd, pos2, Pos, m_Bk2Color, m_rotation, GetLayer());
				}
				else
				{
					m_progressback.Draw(&rnd, Pos, m_Bk2Color, m_rotation, GetLayer());
				}
			}
			else
			{
				if(clipping)
				{
					RectangleF pos2;

					pos2.left = 0;
					pos2.top = 0;
					pos2.right = mul;
					pos2.bottom = 1;

					m_progressback.DrawArea(&rnd, pos2, Pos, m_ProgressColor, m_rotation, GetLayer());
				}
				else
				{
					m_progressback.Draw(&rnd, Pos, m_ProgressColor, m_rotation, GetLayer());
				}
			}
		}
	}
}

void GUIProgress::Save( BaseIO& str )
{
	DrawableObject::Save(str);

	m_progressback.Save(str, "category=GUIProgress;name=Progressbar");
	str.Write(m_Region.left, 0, "name=Region left;help=Left region.");
	str.Write(m_Region.top, 0, "name=Region top;help=Top region.");
	str.Write(m_Region.right, 0, "name=Region right;help=Right region.");
	str.Write(m_Region.bottom, 0, "name=Region bottom;help=Bottom region.");

	str.Write(m_ProgressColor, 0, "name=Progress color;help=Progress color.;type=color");
	str.Write(m_Bk2Color, 0, "name=Back2 color;help=Second background color.;type=color");
	str.Write(m_ThumbSize, 0, "name=Thumb size;help=Thumb size.");
	str.Write(m_MoveSize, 0, "name=Move size;help=Move size.");

	str.Write(GetMin(), 0, "name=Min;help=Min value.");
	str.Write(GetMax(), 0, "name=Max;help=Max value.");
	str.Write(GetValue(), 0, "name=Value;help=Current value.");
	str.Write(clipping, 0, "name=Clipping");

	str.Write(imgprogress, 0, "name=Image Progress");
	str.Write(startind, 0, "name=Start Index");
	str.Write(endind, 0, "name=End Index");
}

void GUIProgress::Load( BaseIO& str )
{
	DrawableObject::Load(str);

	m_progressback.Load(str);
	str.Read(m_Region.left);
	str.Read(m_Region.top);
	str.Read(m_Region.right);
	str.Read(m_Region.bottom);

	str.Read(m_ProgressColor);
	str.Read(m_Bk2Color);
	str.Read(m_ThumbSize);
	str.Read(m_MoveSize);

	float Mn = 0, Mx = 0, Vl = 0;
	str.Read(Mn);
	str.Read(Mx);
	str.Read(Vl);
	str.Read(clipping);
	str.Read(imgprogress);
	str.Read(startind);
	str.Read(endind);

	SetMax(Mx);
	SetMin(Mn);
	SetValue(Vl);	
}

Object* GUIProgress::Clone()
{
	GUIProgress* obj = new GUIProgress();
	*obj = *this;
	return obj;
}

//Input
void GUIProgress::MouseDown(unsigned short button, unsigned short buttons, PointI const& pt)
{
	DrawableObject::MouseDown(button, buttons, pt);
	if(m_flags&OBJECT_Flag_PTypeSlider)
	{
		RectangleF r1;
		float tmp;
		int tmp2;

		r1 = GetAbsolutePos();
		r1.left += m_Region.left;
		r1.top += m_Region.top;
		r1.right -= m_Region.right;
		r1.bottom -= m_Region.bottom;

		tmp = (GetValue()-GetMin())/(GetMax()-GetMin());

		if(m_flags&OBJECT_Flag_PTypeVertical && m_flags&OBJECT_Flag_PTypeInv)
		{
			tmp2 = int(r1.bottom - (r1.bottom - r1.top) * (1-tmp));

			if(pt.top >= tmp2 - (int)m_ThumbSize / 2 && pt.top <= tmp2 + (int)m_ThumbSize / 2)
			{
				Selecting = true;
				SetCapture();
			}
			else if(pt.top < tmp2)
			{
				SetValue(Value-m_MoveSize);
			}
			else if(pt.top > tmp2)
			{
				SetValue(Value+m_MoveSize);
			}
		}
		else if(m_flags&OBJECT_Flag_PTypeVertical)
		{
			tmp2 = int(r1.bottom - (r1.bottom - r1.top) * tmp);

			if(pt.top >= tmp2 - (int)m_ThumbSize / 2 && pt.top <= tmp2 + (int)m_ThumbSize / 2)
			{
				Selecting = true;
				SetCapture();
			}
			else if(pt.top < tmp2)
			{
				SetValue(Value+m_MoveSize);
			}
			else if(pt.top > tmp2)
			{
				SetValue(Value-m_MoveSize);
			}
		}
		else
		{
			tmp2 = int(r1.left + (r1.right - r1.left) * tmp);

			if(pt.left >= tmp2 - (int)m_ThumbSize / 2 && pt.left <= tmp2 + (int)m_ThumbSize / 2)
			{
				Selecting = true;
				SetCapture();
			}
			else if(pt.left < tmp2)
			{
				SetValue(Value-m_MoveSize);
			}
			else if(pt.left > tmp2)
			{
				SetValue(Value+m_MoveSize);
			}
		}
	}
}

void GUIProgress::MouseUp(unsigned short button, unsigned short buttons, PointI const& pt)
{
	DrawableObject::MouseUp(button, buttons, pt);
	ReleaseCapture();
	Selecting=false;
}

void GUIProgress::MouseMove(PointI const& pt)
{
	DrawableObject::MouseMove(pt);
	if(m_flags&OBJECT_Flag_PTypeSlider)
	{
		RectangleF r1;
		float tmp, tmp3;
		int tmp2;

		if(Selecting)
		{
			r1 = GetAbsolutePos();
			r1.left += m_Region.left;
			r1.top += m_Region.top;
			r1.right -= m_Region.right;
			r1.bottom -= m_Region.bottom;

			if(m_flags&OBJECT_Flag_PTypeVertical && m_flags&OBJECT_Flag_PTypeInv)
			{
				if(pt.top >= r1.top - m_Region.top && pt.top <= r1.top)
				{
					Value = Min;
				}
				else if(pt.top >= r1.bottom && pt.top <= r1.bottom + m_Region.bottom)
				{
					Value = Max;
				}
				else if(pt.top > r1.top && pt.top < r1.bottom)
				{
					tmp = (r1.bottom - pt.top) / (float)(r1.bottom-r1.top);
					if(m_flags&OBJECT_Flag_PTypeDecimal) SetValue(((Max-Min)*(1-tmp))+Min);
					else
					{
						tmp3 = ((Max-Min)*(1-tmp))+Min;
						tmp2 = int(tmp3);
						if(tmp3-tmp2>0.5) tmp2+=1;
						SetValue((float)tmp2);
					}					 
				}
			}
			else if(m_flags&OBJECT_Flag_PTypeVertical)
			{
				if(pt.top >= r1.top - m_Region.top && pt.top <= r1.top)
				{
					Value = Max;
				}
				else if(pt.top >= r1.bottom && pt.top <= r1.bottom + m_Region.bottom)
				{
					Value = Min;
				}
				else if(pt.top > r1.top && pt.top < r1.bottom)
				{
					tmp = (r1.bottom - pt.top) / (float)(r1.bottom-r1.top);
					if(m_flags&OBJECT_Flag_PTypeDecimal) SetValue(((Max-Min)*tmp)+Min);
					else
					{
						tmp3 = ((Max-Min)*tmp)+Min;
						tmp2 = int(tmp3);
						if(tmp3-tmp2>0.5) tmp2+=1;
						SetValue((float)tmp2);
					}					 
				}
			}
			else
			{
				if(pt.left >= r1.left - m_Region.left && pt.left <= r1.left)
				{
					Value = Min;
				}
				else if(pt.left >= r1.right && pt.left <= r1.right + m_Region.right)
				{
					Value = Max;
				}
				else if(pt.left > r1.left && pt.left < r1.right)
				{
					tmp = (pt.left - r1.left) / (float)(r1.right-r1.left);
					if(m_flags&OBJECT_Flag_PTypeDecimal) SetValue(((Max-Min)*tmp)+Min);
					else
					{
						tmp3 = ((Max-Min)*tmp)+Min;
						tmp2 = int(tmp3);
						if(tmp3-tmp2>0.5) tmp2+=1;
						SetValue((float)tmp2);
					}					 
				}
			}
		}
	}
}

void GUIProgress::MouseWheel(int wheel, unsigned short buttons, PointI const& pt)
{
	MouseWheel(wheel, buttons, pt);
	int tmp2 =  wheel;
	if(m_flags&OBJECT_Flag_PTypeInv) tmp2 = -tmp2;
	if(m_flags&OBJECT_Flag_PTypeDecimal)
	SetValue(GetValue() + tmp2/100.0f);
	else SetValue(GetValue() + tmp2);
}

void GUIProgress::KeyDown(unsigned int key)
{
	DrawableObject::KeyDown(key);
}

void GUIProgress::KeyUp(unsigned int key)
{
	DrawableObject::KeyUp(key);
}

void GUIProgress::KeyChar(wchar_t key)
{
	DrawableObject::KeyChar(key);
}

// Circle Menu
GUICircleMenu::GUICircleMenu( ResourceManager* mgr, unsigned int ID, const RectangleF& location, Resource sprite, unsigned int backcolor, float rotation, bool visible, unsigned int flags, ObjectsContainer* parent, bool update ) : ObjectsContainer( mgr, ID, location, sprite, backcolor, rotation, visible, flags, parent, update )
{
	m_type |= OBJECT_TYPE_CIRCLEMENU;
	m_radius = 100;
	m_sangle = 0;
}
	
void GUICircleMenu::AddObject(Object* obj)
{
	ObjectsContainer::AddObject(obj);
	Arrange();
}

void GUICircleMenu::Arrange()
{
	unsigned int count = Objects.size();
	list<Object*>::iterator it = Objects.begin();

	float width = GetAbsolutePos().right - GetAbsolutePos().left;
	float height = GetAbsolutePos().bottom - GetAbsolutePos().top;

	width /= 2.0f;
	height /= 2.0f;

	RectangleF rf;

	for(float a = 0; a<=C_PI*2, it != Objects.end(); a += C_PI*2 / (count - 1), it++)
	{
		if((*it)->GetType()&OBJECT_TYPE_DRAWABLE)
		{
			DrawableObject* obj = (DrawableObject*)(*it);
			rf = obj->GetObjectArea();
			
			float owidth = rf.right - rf.left;
			float oheight = rf.bottom - rf.top;

			rf.left = (width + cos(a+m_sangle) * m_radius) - owidth/2.0f;
			rf.top = (height + sin(a+m_sangle) * m_radius) - oheight/2.0f;
			rf.right = rf.left + owidth;
			rf.bottom = rf.top + oheight;
			obj->SetObjectArea(rf);
		}
	}
}

float GUICircleMenu::GetRadius()
{
	return m_radius;
}

void GUICircleMenu::SetRadius(float val)
{
	m_radius = val;
	Arrange();
}

float GUICircleMenu::GetStartAngle()
{
	return m_sangle;
}

void GUICircleMenu::SetStartAngle(float val)
{
	m_sangle = val;
	Arrange();
}

void GUICircleMenu::SetParent(ObjectsContainer* val)
{
	m_parent = val;
}

void GUICircleMenu::Save( BaseIO& str )
{
	ObjectsContainer::Save(str);
	str.Write(GetRadius(), 0, "name=Circle radius;help=Circle radius.;category=GUICircleMenu");
	str.Write(GetStartAngle(), 0, "name=Start Angle;help=Start angle for arrangement of controls.");
}

void GUICircleMenu::Load( BaseIO& str )
{
	ObjectsContainer::Load(str);
	float radius, angle;
	str.Read(radius);
	str.Read(angle);	
	SetRadius(radius);
	SetStartAngle(angle);
}

Object* GUICircleMenu::Clone()
{
	return new GUICircleMenu(m_resmanager);
}

AnimableObject::AnimableObject(unsigned int ID, RectangleF const& location, Resource sprite, unsigned int backcolor, float rotation, bool visible, unsigned int flags, ObjectsContainer* parent, bool update) : DrawableObject(ID, location, sprite, backcolor, rotation, visible, flags, parent, update)
{
	m_type |= OBJECT_TYPE_ANIMABLE;
	m_current = 0;
	m_delay = 0;
	m_speed = 0;
	m_animtype = 0;
	m_playtype = 0;
	m_sloop = false;
	m_cpos = 0;
	m_rspos = 0.0f;
	m_repos = C_PI * 2.0f;
	m_scolor = 0x00000000;
	m_ecolor = 0xFFFFFFFF;
	m_scale = 1.0f;
	m_scalefrom = 0.0f;
	m_scaleto = 50.0f;
}

void AnimableObject::Draw(C_Renderer& rnd)
{
	RectangleF rect = GetAbsolutePos();
	rect.left -= m_scale;
	rect.top -= m_scale;
	rect.right += m_scale;
	rect.bottom += m_scale;
	m_sprite.Draw(&rnd, rect, m_backcolor, GetAngle(), GetLayer());
}

void AnimableObject::Reset()
{
	if(m_playtype==1) m_cpos = 1.0f;
	else m_cpos = 0.0f;
	m_current = 0;
	m_player.Stop();
}

void AnimableObject::Update( float delta )
{
	DrawableObject::Update(delta);
	if(m_current<m_delay) m_current += delta;

	float scale = 1.0f;
	if(m_parent)
	{
		scale = m_parent->GetCamera().m_scale;
	}

	if(m_animtype&1)
	{
		PointF pt;
		pt.left = m_spos.left + (m_epos.left - m_spos.left) * m_cpos;
		pt.top = m_spos.top + (m_epos.top - m_spos.top) * m_cpos;
		float width = GetObjectArea().right - GetObjectArea().left;
		float height = GetObjectArea().bottom - GetObjectArea().top;
		SetObjectArea(RectangleF(pt.left, pt.top, pt.left + width, pt.top + height));
	}
	if(m_animtype&2)
	{
		SetAngle(m_rspos + (m_repos - m_rspos) * m_cpos);
	}
	if(m_animtype&4)
	{
		ColorARGB32 c1(m_scolor);
		ColorARGB32 c2(m_ecolor);
		PointF pt;
		pt.left = m_rspos + (m_repos - m_rspos) * m_cpos;
		m_backcolor = ColorARGB32((unsigned char)(c1.GetA() + (c2.GetA() -  c1.GetA()) * m_cpos), (unsigned char)(c1.GetR() + (c2.GetR() -  c1.GetR()) * m_cpos), (unsigned char)(c1.GetG() + (c2.GetG() -  c1.GetG()) * m_cpos), (unsigned char)(c1.GetB() + (c2.GetB() -  c1.GetB()) * m_cpos));
	}
	if(m_animtype&8)
	{
		m_scale = m_scalefrom * scale + (m_scaleto * scale - m_scalefrom * scale) * m_cpos;			
	}
	else
	{
		m_scale = 1.0f;
	}		

	if(m_current>=m_delay)
	{
		if(m_cpos==0)
		{
			m_player.Stop();
			m_player.Play(m_sloop);
		}

		if(m_playtype == 0 && m_cpos < 1.0f)
		{
			m_cpos += m_speed * delta;

			if(m_pingpong && m_cpos > 1.0f)
			{
				float tmp = m_cpos;
				while(tmp > 1.0f) tmp -= 1.0f;
				m_cpos = 1.0f - tmp;
				m_playtype = 1;
				if(m_animtype&16)
				{
					Delete();
				}
			}
			else if(m_cpos > 1.0f)
			{
				m_cpos = 1.0f;
			}
		}
		else if(m_playtype == 1 && m_cpos > 0)
		{
			m_cpos -= m_speed * delta;

			if(m_pingpong && m_cpos < 0.0f)
			{
				float tmp = m_cpos;
				while(tmp < 0.0f) tmp += 1.0f;
				m_cpos = 1.0f - tmp;
				m_playtype = 0;
			}
		}
		else if(m_playtype == 2 && m_cpos < 1.0f)
		{
			m_cpos += m_speed * delta;
			while(m_cpos > 1.0f)
			{
				m_cpos -= 1.0f;
				m_current = 0.0f;
			}
		}
	}
}

void AnimableObject::Save( BaseIO& str )
{
	DrawableObject::Save(str);
	str.Write(m_delay, 0, "name=Delay;help=start delay.;category=AnimableObject");
	str.Write(m_speed, 0, "name=Speed;help=Speed.");
	str.Write(m_spos.left, 0, "name=Start X;help=start X.");
	str.Write(m_spos.top, 0, "name=Start Y;help=start Y.");
	str.Write(m_epos.left, 0, "name=End X;help=End X.");
	str.Write(m_epos.top, 0, "name=End Y;help=End Y.");
	str.Write(m_animtype, 0, "name=Animation Type;help=Animation type 1 - position, 2 - rotation, 4 - color, 8 - scale.;type=flags;list=position,rotation,color,scale");
	str.Write(m_playtype, 0, "name=Play type;help=Play type 0 - forward, 1 - backward, 2 - loop.;type=list;list=forward,backward,loop");
	str.Write(m_sloop, 0, "name=Loop sound;help=Loop sound.");
	str.Write(m_pingpong, 0, "name=Pingpong;help=Pingpong.");
	str.Write(m_rspos, 0, "name=Rotation start;help=Rotation start.");
	str.Write(m_repos, 0, "name=Rotation end;help=Rotation end.");
	str.Write(m_scolor, 0, "name=Color start;help=Color end.;type=color");
	str.Write(m_ecolor, 0, "name=Color end;help=Color end.;type=color");
	str.Write(m_scalefrom, 0, "name=Scale from;help=Scale from.");
	str.Write(m_scaleto, 0, "name=Scale to;help=Scale to.");
	str.Write(&m_sound, 0, "name=Sound resource;default=3");
}

void AnimableObject::Load( BaseIO& str )
{
	DrawableObject::Load(str);
	str.Read(m_delay);
	str.Read(m_speed);
	str.Read(m_spos.left);
	str.Read(m_spos.top);
	str.Read(m_epos.left);
	str.Read(m_epos.top);
	str.Read(m_animtype);
	str.Read(m_playtype);
	str.Read(m_sloop);
	str.Read(m_pingpong);
	str.Read(m_rspos);
	str.Read(m_repos);
	str.Read(m_scolor);
	str.Read(m_ecolor);
	str.Read(m_scalefrom);
	str.Read(m_scaleto);
	str.Read(&m_sound);
	m_player.Stop();
	m_player.SetBuffer(m_sound);
	Reset();
}

Object* AnimableObject::Clone()
{
	AnimableObject* obj = new AnimableObject();
	*obj = *this;
	return obj;
}

//Particle emitter
ParticleEmitter::ParticleEmitter(unsigned int ID, RectangleF const& location, Resource sprite, unsigned int backcolor, float rotation, bool visible, unsigned int flags, ObjectsContainer* parent, bool update) : AnimableObject(ID, location, sprite, backcolor, rotation, visible, flags, parent, update)
{
	m_time = 0;
	m_activated = false;
	m_paricledelay = 3.0f;
}

void ParticleEmitter::Update( float delta )
{
	AnimableObject::Update(delta);

	if(!m_activated) return;

	m_time += delta;

	if(m_paricledelay <= m_time)
	{
		if(m_parent)
		{
			RectangleF objar = GetObjectArea();
			RectangleF rf;

			rf.left = rand() * objar.right + objar.left;
			rf.top = rand() * objar.bottom + objar.top;
			rf.right = rf.left + 20;
			rf.bottom = rf.top + 20;

			AnimableObject* aobj = new AnimableObject(123456);
			aobj->SetObjectArea(rf);
			aobj->SetBackColor(m_backcolor);
			aobj->SetSprite(m_sprite);
			aobj->m_animtype = 1 | 16;
			aobj->m_delay = m_delay;
			aobj->m_speed = 0.3f;
			aobj->m_playtype = m_playtype;
			aobj->m_spos.left = rf.left;
			aobj->m_spos.top = rf.top;
			aobj->m_epos.left = rf.left + m_epos.left;
			aobj->m_epos.top = rf.left + m_epos.top;

			m_parent->AddObject(aobj);
		}
		m_time = 0;
	}
}

void ParticleEmitter::Save( BaseIO& str )
{
	AnimableObject::Save(str);
	str.Write(m_activated, 0, "name=Activated");
}

void ParticleEmitter::Load( BaseIO& str )
{
	AnimableObject::Load(str);
	str.Read(m_activated);
}

Object* ParticleEmitter::Clone()
{
	return new ParticleEmitter();
}

GUILoadProgress::GUILoadProgress( ResourceManager* mgr, unsigned int ID, RectangleF const& location, Resource sprite, unsigned int backcolor, float rotation, bool visible, unsigned int flags, ObjectsContainer* parent, bool update ) : GUIProgress(mgr, ID, location, sprite, backcolor, rotation, visible, flags, parent, update)
{
}

void GUILoadProgress::Update( float delta )
{
	if(Game!=0)
	{
		SetValue((float)Game->m_loadst);
	}
}

Object* GUILoadProgress::Clone()
{
	GUILoadProgress* obj = new GUILoadProgress();
	*obj = *this;
	return obj;
}

//particle
Particle::Particle()
{
	color = 0;
	ctime = 0.0f;
	ttime = 0.0f;

	scale = 0.0f;
	rotation = 0.0f;

	posx = 0;
	posy = 0;

	velx = 0;
	vely = 0;
}

//particle engine
ParticleEngine::ParticleEngine(unsigned int ID, RectangleF const& location, Resource sprite, unsigned int backcolor, float rotation, bool visible, unsigned int flags, ObjectsContainer* parent, bool update) : AnimableObject(ID, location, sprite, backcolor, rotation, visible, flags, parent, update)
{
	m_type |= OBJECT_TYPE_PARTICLEEN;
	m_startdelay = 0.0f;
	m_tmptime = 0.0f;
	m_activated = 0.0f;
	startcolor = ColorARGB32(255,255,255,255);
	endcolor = ColorARGB32(0,0,0,0);;
	angularvel = 0.0f;
	scalefreq = 0.0f;
	m_emission = 10;

	m_cetime = 0.0f;
	m_maxtime = 0.05f;

	velxstart = -20;
	velxend = 20;

	velystart = -80;
	velyend = -80;

	randomx = true;
	randomy = false;

	m_ttimemax = 4;
	m_ttimemin = 0.5f;
	randomtime = true;

	particlesizestart = 5;
	particlesizeend = 5;
	randomsize = false;

	forcex = 0;
	forcey = 0;

	randomxpos = true;
	randomypos = false;

	relativevel = false;

	m_emittortype = EMITTOR_TYPE_PLANE;

	circleradstart = 10;
	circleradend = 20;
	cianglestart = 0;
	ciangleend = C_PI * 2;
	randomradius = false;
	cirelativevel = true;
	gravitycenter = false;
	m_emitlistonly = false;
}

void PointTransform(float& vecx, float& vecy, float tx, float ty, float angle, float scalex, float scaley)
{
	float sin0 = sinf(angle);
	float cos0 = cosf(angle);
	float x = 0, y = 0, tmx = 0, tmy = 0;

	tmx = vecx - tx;
	tmy = vecy - ty;

	tmx *= scalex;
	tmy *= scaley;

	x = (tmx * cos0 - tmy * sin0) + tx;
	y = (tmx * sin0 + tmy * cos0) + ty; 

	vecx = x;
	vecy = y;
}

void ParticleEngine::EmitParticles()
{
	EmitParticles(GetAbsolutePos());
}

void ParticleEngine::EmitParticles(float x, float y, float size)
{
	size /= 2.0f;
	RectangleF rct(x - size, y - size, x + size, y + size);
	EmitParticles(GetAbsolutePos(&rct));
}

void ParticleEngine::EmitParticles(const RectangleF& rect)
{
	RectangleF re = rect;

	float hwidth = re.right - re.left;
	float hheight = re.bottom - re.top;
	float hleft = re.left;
	float htop = re.top;

	float m_cscale = 1.0f;
	if(m_parent) m_cscale = m_parent->GetCamera().m_scale;

	for(int i = 0; i<m_emission; i++)
	{
		float posx = hleft; 
		float posy = htop;
		float cangle = 0;

		if(m_emittortype == EMITTOR_TYPE_PLANE)
		{
			if(randomxpos && hwidth) posx = hleft + rand() % (int)hwidth;
			if(randomypos && hheight) posy = htop + rand() % (int)hheight;
		}
		else if(m_emittortype == EMITTOR_TYPE_CIRCLE)
		{
			float rn = ((float)rand() / RAND_MAX);
			cangle = cianglestart + rn * (ciangleend - cianglestart);
			int rad = circleradend;
			if(randomradius && (circleradend - circleradstart)) rad = circleradstart + rand() % (circleradend - circleradstart);
			rad = (int)(rad * m_cscale);

			posx = (hleft+(hwidth/2.0f)) + cos(cangle) * rad;
			posy = (htop+(hheight/2.0f)) + sin(cangle) * rad;
		}

		Particle p;
		p.color = m_backcolor;
		p.posx = posx;
		p.posy = posy;

		PointTransform(p.posx, p.posy, hleft+(hwidth/2.0f), htop+(hheight/2.0f), m_rotation, m_scale, m_scale);

		if(randomx && (velxend - velxstart)) p.velx = (float)(velxstart + rand() % (velxend - velxstart));
		else p.velx = (float)(velxstart);

		if(randomy && (velyend - velystart)) p.vely = (float)(velystart + rand() % (velyend - velystart));
		else p.vely = (float)(velystart);

		if(cirelativevel) PointTransform(p.velx, p.vely, 0, 0, cangle, 1, 1);
		if(relativevel) PointTransform(p.velx, p.vely, 0, 0, m_rotation, m_scale, m_scale);

		if(randomtime && m_ttimemax >= 0.5f) p.ttime = (float)(m_ttimemin + rand() % (int)m_ttimemax);
		else p.ttime = m_ttimemax;

		if(randomsize && (particlesizeend - particlesizestart)) p.scale = (float)(particlesizestart + rand() % (int)(particlesizeend - particlesizestart)) * m_cscale;
		else p.scale = particlesizeend * m_cscale;
		particles.insert(particles.begin(), p);
	}
}

void ParticleEngine::AddEmitRect(const RectangleF& rect, float life)
{
	EmitItem itm;
	itm.pos = rect;
	itm.m_life = life;
	emitlist.push_back(itm);
}

void ParticleEngine::AddEmitRect(float x, float y, float life, float size)
{
	EmitItem itm;
	itm.pos.left = x - size;
	itm.pos.top = y - size;
	itm.pos.right = x + size;
	itm.pos.bottom = y + size;
	itm.m_life = life;
	emitlist.push_back(itm);
}

void ParticleEngine::ClearEmitRect()
{
	emitlist.clear();
}

void ParticleEngine::Update( float delta )
{
	AnimableObject::Update(delta);
	if(!m_activated) return;

	m_tmptime += delta;
	if(m_tmptime>=m_startdelay)
	{
		m_tmptime = m_startdelay;

		m_cetime += delta;

		float m_cscale = 1.0f;
		PointF center(0,0);

		if(m_parent) m_cscale = m_parent->GetCamera().m_scale;

		if(m_maxtime > 0.01f)
		while(m_cetime>=m_maxtime)
		{
			m_cetime -= m_maxtime;
			if(emitlist.size()>0)
			{
				for(vector<EmitItem>::iterator i = emitlist.begin(); i!=emitlist.end(); )
				{
					i->m_ctime += delta;
					EmitParticles(GetAbsolutePos(&i->pos));
					if(i->m_ctime >= i->m_life) emitlist.erase(i++);
					else ++i;
				}
			}
			else if(!m_emitlistonly) EmitParticles();
		}

		RectangleF re = GetAbsolutePos();
		center.left = re.left + (re.right - re.left) / 2.0f;
		center.top = re.top + (re.bottom - re.top) / 2.0f;

		for(list<Particle>::iterator i = particles.begin(); i != particles.end(); i++)
		{
			if(gravitycenter)
			{
				PointF diff;
				diff.left = center.left - i->posx;
				diff.top = center.top - i->posy;

				float d = diff.left * diff.left + diff.top * diff.top;

				if ( !(d == 1.0  || d == 0.0) )
				{
					d = fast_rsqrt(d);
					diff.left *= d;
					diff.top *= d;
				}

				i->velx += (forcex * diff.left * m_cscale * delta);
				i->vely += (forcey * diff.top * m_cscale * delta);
			}
			else
			{
				i->velx += (forcex * m_cscale * delta);
				i->vely += (forcey * m_cscale * delta);
			}
			i->posx += i->velx  * m_cscale * delta;
			i->posy += i->vely  * m_cscale * delta;
			i->scale += scalefreq  * m_cscale * delta;
			i->ctime += delta;
			i->rotation += angularvel * delta;

			ColorARGB32 c1(startcolor);
			ColorARGB32 c2(endcolor);
			float crtime = i->ctime / i->ttime;
			i->color = ColorARGB32((unsigned char)(c1.GetA() + (c2.GetA() -  c1.GetA()) * crtime), (unsigned char)(c1.GetR() + (c2.GetR() -  c1.GetR()) * crtime), (unsigned char)(c1.GetG() + (c2.GetG() -  c1.GetG()) * crtime), (unsigned char)(c1.GetB() + (c2.GetB() -  c1.GetB()) * crtime));
			if(i->ctime >= i->ttime)
			{
				i = particles.erase(i);
				if(i==particles.end()) break;
			}
		}
	}
}

void ParticleEngine::Draw(C_Renderer& rnd)
{
	AnimableObject::Draw(rnd);
	for(list<Particle>::iterator i = particles.begin(); i != particles.end(); i++)
	{
		m_psprite.Draw(&rnd, RectangleF(i->posx - i->scale, i->posy - i->scale, i->posx + i->scale, i->posy + i->scale), i->color, i->rotation, m_layer);
	}
}

void ParticleEngine::Save( BaseIO& str )
{
	AnimableObject::Save(str);
	str.Write(m_activated, 0, "name=Activated;category=Particle Engine");
	m_psprite.Save(str, "name=Particle Sprite");
	str.Write(m_emission, 0, "name=Emission");
	str.Write(scalefreq, 0, "name=scale frequency");
	str.Write(angularvel, 0, "name=anguler velocity");
	str.Write(startcolor, 0, "name=Start Color;type=color");
	str.Write(endcolor, 0, "name=End Color;type=color");
	str.Write(m_maxtime,0, "name=Emission Delay");
	str.Write(velxstart, 0, "name=Velocity X start");
	str.Write(velxend, 0, "name=Velocity X end");
	str.Write(velystart, 0, "name=Velocity Y start");
	str.Write(velyend, 0, "name=Velocity Y end");
	str.Write(randomx, 0, "name=Random X Vel");
	str.Write(randomy, 0, "name=Random Y Vel");
	str.Write(m_ttimemax, 0, "name=Max life");
	str.Write(m_ttimemin, 0, "name=Min life");
	str.Write(randomtime, 0, "name=Random life");
	str.Write(particlesizestart, 0, "name=Particle Size Start");
	str.Write(particlesizeend, 0, "name=Particle Size End");
	str.Write(randomsize, 0, "name=Random Size");
	str.Write(forcex, 0, "name=Force X");
	str.Write(forcey, 0, "name=Force Y");
	str.Write(randomxpos, 0, "name=Random X Pos");
	str.Write(randomypos, 0, "name=Random Y Pos");
	str.Write(relativevel, 0, "name=Relative Velocity");
	str.Write(circleradstart, 0, "name=Circle radius start");
	str.Write(circleradend, 0, "name=Circle radius end");
	str.Write(cianglestart, 0, "name=Circle angle start");
	str.Write(ciangleend, 0, "name=Circle angle end");
	str.Write(randomradius, 0, "name=Random radius");
	str.Write(m_emittortype, 0, "name=Emitter type;type=list;list=Plane,Circle");
	str.Write(cirelativevel, 0, "name=Relative circle velocity");
	str.Write(gravitycenter, 0, "name=Gravity Center");
	str.Write(m_emitlistonly, 0, "name=EmitListOnly");
	
}

void ParticleEngine::Load( BaseIO& str )
{
	AnimableObject::Load(str);
	str.Read(m_activated);
	m_psprite.Load(str);
	str.Read(m_emission);
	str.Read(scalefreq);
	str.Read(angularvel);
	str.Read(startcolor);
	str.Read(endcolor);
	str.Read(m_maxtime);
	str.Read(velxstart);
	str.Read(velxend);
	str.Read(velystart);
	str.Read(velyend);
	str.Read(randomx);
	str.Read(randomy);
	str.Read(m_ttimemax);
	str.Read(m_ttimemin);
	str.Read(randomtime);
	str.Read(particlesizestart);
	str.Read(particlesizeend);
	str.Read(randomsize);
	str.Read(forcex);
	str.Read(forcey);
	str.Read(randomxpos);
	str.Read(randomypos);
	str.Read(relativevel);
	str.Read(circleradstart);
	str.Read(circleradend);
	str.Read(cianglestart);
	str.Read(ciangleend);
	str.Read(randomradius);
	str.Read(m_emittortype);
	str.Read(cirelativevel);
	str.Read(gravitycenter);
	str.Read(m_emitlistonly);
}

Object* ParticleEngine::Clone()
{
	return new ParticleEngine();
}

//timer

TimerObject::TimerObject(unsigned int ID, RectangleF const& location, Resource sprite, unsigned int backcolor, float rotation, bool visible, unsigned int flags, ObjectsContainer* parent, bool update) : DrawableObject(ID, location, sprite, backcolor, rotation, visible, flags, parent, update)
{
	m_type |= OBJECT_TYPE_TIMER;
	m_flags |= OBJECT_Flag_Disabled | OBJECT_Flag_Invisible;
	m_ctime = 0.0f;
	m_started = false;
	m_interval = 0.5f;
	m_loop = false;
}

void TimerObject::StartTimer()
{
	StopTimer();
	m_started = true;
}

void TimerObject::StopTimer()
{
	m_started = false;
	m_ctime = 0.0f;
}

void TimerObject::PauseTimer()
{
	m_started = false;
}

void TimerObject::Update( float delta )
{
	DrawableObject::Update(delta);

	if(!m_started || IsEditing()) return;

	m_ctime += delta;
	while(m_ctime >= m_interval && m_started)
	{
		if(!m_loop) StopTimer();
		m_ctime -= m_interval;
		OnInterval();
		if(m_parent) m_parent->Callback(this, OBJECT_Message_OnTimer, 0, 0);
	}
}

void TimerObject::Save( BaseIO& str )
{
	DrawableObject::Save(str);
	str.Write(m_started, 0, "name=Started;category=Timer");
	str.Write(m_interval, 0, "name=Interval");
	str.Write(m_loop, 0, "name=Loop");
	str.Write(m_ctime, 0, "name=Current Time;category=Data");
}

void TimerObject::Load( BaseIO& str )
{
	DrawableObject::Load(str);
	str.Read(m_started);
	str.Read(m_interval);
	str.Read(m_loop);
	str.Read(m_ctime);
	if(!m_started) StopTimer();
}

Object* TimerObject::Clone()
{
	return new TimerObject();
}

// Tiles
Tiles::Tiles(unsigned int ID, RectangleF const& location, Resource sprite, unsigned int backcolor, float rotation, bool visible, unsigned int flags, ObjectsContainer* parent, bool update) : DrawableObject(ID, location, sprite, backcolor, rotation, visible, flags, parent, update)
{
	twidth = 64.0f;
	theight = 64.0f;
	spacex = 0;
	spacey = 0;
	tilescx = 1;
	tilescy = 1;
	relative = true;
	editing = false;
	cindex = 0;
	cangle = 0;
}

void Tiles::Update( float delta )
{
	DrawableObject::Update(delta);
}

void Tiles::Save( BaseIO& str )
{
	DrawableObject::Save(str);
	str.Write(twidth, 0, "name=Tile Width;category=Tiles");
	str.Write(theight, 0, "name=Tile Height");
	str.Write(spacex, 0, "name=Space X");
	str.Write(spacey, 0, "name=Space Y");
	str.Write(tilescx, 0, "name=Tile Col count");
	str.Write(tilescy, 0, "name=Tile Row count");

	string st;
	stringstream s;
	for(unsigned int i = 0; i<data.size(); i++)
	{
		s << data[i];
		if(i!=data.size()-1) s << ",";
	}
	st = s.str();
	str.Write(st, 0, "name=Tiles data;help=Tiles data.");

	string st2;
	stringstream s2;
	for(unsigned int i = 0; i<angles.size(); i++)
	{
		s2 << angles[i];
		if(i!=angles.size()-1) s2 << ",";
	}
	st2 = s2.str();
	str.Write(st2, 0, "name=Tiles Angles;help=Tiles Angles.");
}

void Tiles::Load( BaseIO& str )
{
	DrawableObject::Load(str);
	str.Read(twidth);
	str.Read(theight);
	str.Read(spacex);
	str.Read(spacey);
	str.Read(tilescx);
	str.Read(tilescy);

	string st;
	str.Read(st);
	data.clear();
	GetStringArray(data, st.c_str(), ',');

	if(data.size()<(tilescx*tilescy))
	{
		unsigned int diff = (tilescx*tilescy) - data.size();
		for(unsigned int i = 0; i<diff; i++)
		{
			data.push_back(0);
		}
	}

	string st2;
	str.Read(st2);
	angles.clear();
	GetStringArray(angles, st2.c_str(), ',');

	if(angles.size()<(tilescx*tilescy))
	{
		unsigned int diff = (tilescx*tilescy) - angles.size();
		for(unsigned int i = 0; i<diff; i++)
		{
			angles.push_back(0);
		}
	}
}

void Tiles::Draw(C_Renderer& rnd)
{
	if(IsEditing()) DrawableObject::Draw(rnd);
	for(unsigned int y = 0; y < tilescy; y++)
	{
		for(unsigned int x = 0; x < tilescx; x++)
		{
			unsigned int p = y * tilescy + x;
			float angle = 0;
			if(data.size() > p) 
			{
				m_sprite.SetIndex(data[p]);
			}
			if(angles.size() > p)
			{
				angle = angles[p];
			}
			RectangleF des(x * (spacex + twidth), y * (spacey + theight), (x * (spacex + twidth)) + twidth, (y * (spacey + theight)) + theight);
			if(relative)
			{
				RectangleF ps = GetObjectArea();
				des.left += ps.left;
				des.top += ps.top;
				des.right += ps.left;
				des.bottom += ps.top;
			}
			m_sprite.Draw(&rnd, GetAbsolutePos(&des), 0xFFFFFFFF, angle, GetLayer());
		}
	}
}

void Tiles::MouseDown(unsigned short button, unsigned short buttons, PointI const& pt)
{
	DrawableObject::MouseDown(button, buttons, pt);

	bool found = false;
	for(unsigned int y = 0; y < tilescy; y++)
	{
		for(unsigned int x = 0; x < tilescx; x++)
		{
			RectangleF des(x * (spacex + twidth), y * (spacey + theight), (x * (spacex + twidth)) + twidth, (y * (spacey + theight)) + theight);
			if(relative)
			{
				RectangleF ps = GetObjectArea();
				des.left += ps.left;
				des.top += ps.top;
				des.right += ps.left;
				des.bottom += ps.top;
			}
			if(PointI::Intersect(pt, des))
			{
				unsigned int p = y * tilescy + x;
				if(m_parent) m_parent->Callback(this, OBJECT_Message_TileClicked, p, 0);
				found = true;
				break;
			}
		}
		if(found) break;
	}

#ifndef __AVM2__
	if(!IsEditing()) return;
	if(editing && button == KEY_LBUTTON)
	{
		found = false;
		for(unsigned int y = 0; y < tilescy; y++)
		{
			for(unsigned int x = 0; x < tilescx; x++)
			{
				RectangleF des(x * (spacex + twidth), y * (spacey + theight), (x * (spacex + twidth)) + twidth, (y * (spacey + theight)) + theight);
				if(relative)
				{
					RectangleF ps = GetObjectArea();
					des.left += ps.left;
					des.top += ps.top;
					des.right += ps.left;
					des.bottom += ps.top;
				}
				if(PointI::Intersect(pt, des))
				{
					unsigned int p = y * tilescy + x;
					if(data.size() > p) 
					{
						data[p] = cindex;
					}
					else 
					{
						data[p] = 0;
					}

					if(angles.size() > p)
					{
						angles[p] = cangle;
					}
					else
					{
						angles[p] = 0;
					}
					found = true;
					break;
				}
			}
			if(found) break;
		}
	}
	else if(button == KEY_RBUTTON)
	{
		editing = false;
		ReleaseCapture();
	}
#endif
}

void Tiles::KeyDown(unsigned int key)
{
	DrawableObject::KeyDown(key);
	if(!IsEditing()) return;
	if(editing)
	{
		if( key >= KEY_0 && key <= KEY_9 )
		{
			tmpstr += (char)key;
		}
		else if(key == KEY_SPACE)
		{
			cindex = atoi(tmpstr.c_str());
			tmpstr = "";
			m_sprite.SetIndex(cindex);
		}
		else if( key == KEY_E )
		{
			editing = false;
			ReleaseCapture();
		}
		else if( key == KEY_F)
		{
			cangle = C_PI;
		}
		else if( key == KEY_H)
		{
			cangle = 0;
		}
		else if( key == KEY_T)
		{
			cangle = C_PI + C_PI / 2.0f;
		}
		else if( key == KEY_G)
		{
			cangle = C_PI / 2.0f;
		}
	}
	else if( key == KEY_E )
	{
		editing = true;
		cindex = 0;
		cangle = 0;
		SetCapture();
	}
}

Object* Tiles::Clone()
{
	return new Tiles();
}

// Object Connection
ObjectConnection::ObjectConnection()
{
	cobj = 0;
	ConID = 0;
}

ObjectConnection::ObjectConnection(DrawableObject* cobj)
{
	this->cobj = cobj;
	if(cobj!=0) ConID = cobj->m_ID;
	if(cobj!=0) AddConnection(*cobj);
}

ObjectConnection::~ObjectConnection()
{
	Disconnect();
}

void ObjectConnection::UpdateConnection()
{
	if(ConID==0) return;
	if(cobj!=0 && cobj->m_ID == ConID) return;
	Disconnect();
	cobj = (DrawableObject*)Game->GetMainContainer()->GetObjectByID(ConID);
	if(cobj!=0) AddConnection(*cobj);
}

void ObjectConnection::Disconnect()
{
	if(cobj!=0)
	{
		DeleteConnection((*cobj));
		Disconnected(cobj);
		cobj = 0;
	}
}

const DrawableObject* ObjectConnection::GetConstObject() const
{
	return cobj;
}

DrawableObject* ObjectConnection::GetObject()
{
	return cobj;
}

bool ObjectConnection::IsValid()
{
	if(cobj==0) return false;
	return true;
}

void ObjectConnection::SetObject(DrawableObject* obj)
{
	Disconnect();
	if(obj==0)
	{
		cobj = 0;
	}
	else
	{
		cobj = obj;
		ConID = obj->m_ID;
		AddConnection(*cobj);
	}
}

void ObjectConnection::SetConnectionIDNU(unsigned int ID)
{
	ConID = ID;
}

void ObjectConnection::SetConnectionID(unsigned int ID)
{
	ConID = ID;
	UpdateConnection();
}

unsigned int ObjectConnection::GetConnectionID()
{
	return ConID;
}

void ObjectConnection::AddConnection(DrawableObject& obj)
{
	obj.connections.insert(this);
}

void ObjectConnection::DeleteConnection(DrawableObject& obj)
{
	obj.connections.erase(this);
}

void ObjectConnection::Copy(const ObjectConnection& cpy)
{
	SetObject(cpy.cobj);
}

ObjectConnection::ObjectConnection(const ObjectConnection& cpy)
{
	cobj = 0;
	ConID = cpy.ConID;
	Copy(cpy);
}

const ObjectConnection& ObjectConnection::operator=(const ObjectConnection& cpy)
{
	Copy(cpy);
	return *this;
}

// Grid Container
GUITable::GUITable( ResourceManager* mgr, unsigned int ID, const RectangleF& location, Resource sprite, unsigned int backcolor, float rotation, bool visible, unsigned int flags, ObjectsContainer* parent, bool update ) : ObjectsContainer( mgr, ID, location, sprite, backcolor, rotation, visible, flags, parent, update )
{
	twidth = 64.0f;
	theight = 64.0f;
	spacex = 0;
	spacey = 0;
	tilescx = 1;
}
	
void GUITable::AddObject(Object* obj)
{
	ObjectsContainer::AddObject(obj);
	Arrange();
}

void GUITable::SetTileSpace(float spacex, float spacey)
{
	if(spacex==0 && spacey==0) return;

	if(spacex==0)
	{
		this->spacex = spacey;
		this->spacey = spacey;
	}
	else if(spacey==0)
	{
		this->spacex = spacex;
		this->spacey = spacex;
	}
	else
	{
		this->spacex = spacex;
		this->spacey = spacey;
	}
	Arrange();
}

void GUITable::SetTileSize(float width, float height)
{
	if(width==0 && height==0) return;

	if(width==0)
	{
		twidth = height;
		theight = height;
	}
	else if(height==0)
	{
		twidth = width;
		theight = width;
	}
	else
	{
		twidth = width;
		theight = height;
	}
	Arrange();
}

void GUITable::SetColumns(unsigned int count)
{
	if(count != 0)
	{
		tilescx = count;
		Arrange();
	}
}

PointF GUITable::GetTileSize()
{
	return PointF(twidth, theight);
}

PointF GUITable::GetTileSpace()
{
	return PointF(spacex, spacey);
}

unsigned int GUITable::GetColumns()
{
	return tilescx;
}

void GUITable::Arrange()
{
	list<Object*>::iterator i = Objects.begin();
	bool flag = false;

	for(unsigned int y = 0; y < Objects.size(); y++)
	{
		for(unsigned int x = 0; x < tilescx; x++)
		{
			if(i!=Objects.end())
			{
				RectangleF des(x * (spacex + twidth), y * (spacey + theight), (x * (spacex + twidth)) + twidth, (y * (spacey + theight)) + theight);
				
				if((*i)->GetType()&OBJECT_TYPE_EDITOR)
				{
					++i;
					if(i==Objects.end()) 
					{
						flag = true;
						break;
					}
				}				

				if((*i)->GetType()&OBJECT_TYPE_DRAWABLE)
				{
					((DrawableObject*)(*i))->SetObjectArea(des);
				}		
				++i;
			}
			else
			{
				flag = true;
				break;
			}			
		}
		if(flag) break;
	}
}

void GUITable::SetParent(ObjectsContainer* val)
{
	m_parent = val;
}

void GUITable::Save( BaseIO& str )
{
	ObjectsContainer::Save(str);
	str.Write(twidth, 0, "name=Tile Width;category=GUI Table");
	str.Write(theight, 0, "name=Tile Height");
	str.Write(spacex, 0, "name=Space X");
	str.Write(spacey, 0, "name=Space Y");
	str.Write(tilescx, 0, "name=Tile Col count");
}

void GUITable::Load( BaseIO& str )
{
	ObjectsContainer::Load(str);
	str.Read(twidth);
	str.Read(theight);
	str.Read(spacex);
	str.Read(spacey);
	str.Read(tilescx);
	Arrange();
}

void GUITable::PostLoadAll()
{
	Arrange();
}

Object* GUITable::Clone()
{
	return new GUITable(m_resmanager);
}

//Text Effects

GUITextEffects::GUITextEffects(ResourceManager* mgr, unsigned int ID, RectangleF const& location, Resource sprite, unsigned int backcolor, float rotation, bool visible, unsigned int flags, ObjectsContainer* parent, bool update) : GUIText(mgr, ID, location, sprite, backcolor, rotation, visible, flags, parent, update)
{
	ctime = 0;
	speed = 2.0f;
	param1 = 0.6f;
	param2 = 10.0f;
	param3 = 0.0f;
	etype = SinCurve;
	loop = true;
}

//General functions
void GUITextEffects::Update( float delta )
{
	GUIText::Update(delta);
	ctime += speed * delta;
	if(loop)
	{
		while(ctime>1.0f) ctime -= 1.0f;
	}
	else
	{
		if(ctime>1.0f) ctime = 1.0f;
	}
}

void GUITextEffects::Draw(C_Renderer& rnd)
{
	RectangleF Pos = GetAbsolutePos();

	if(!(m_flags&OBJECT_Flag_NoBackground)) AnimableObject::Draw(rnd);

	Pos.left += m_TextPadding.left;
	Pos.right -= m_TextPadding.right;
	Pos.top += m_TextPadding.top;
	Pos.bottom -= m_TextPadding.bottom;

	float scale = m_fscale;
	if(m_parent)
	{
		scale *= m_parent->GetCamera().m_scale;

		if(m_parent->m_parent!=0)
		{
			scale *= m_parent->m_parent->GetCamera().m_scale;
		}
	}

	for(unsigned int i = 0; i<m_Text.Length(); i++)
	{
		RectangleI r;
		r.left = (int)Pos.left;
		r.top = (int)Pos.top;
		r.right = (int)Pos.right;
		r.bottom = (int)Pos.bottom;

		unsigned int tcolor = m_Color;

		if(etype==SinCurve) r.top = (int)(r.top + sin(i/param1 + ctime * C_PI * 2) * param2 * scale);
		else if(etype==HSinCurve) r.left = (int)(r.left + sin(i/param1 + ctime * C_PI * 2) * param2 * scale);
		else if(etype==Linear) r.top = (int)(r.top + (i/param1 + (ctime - 0.5f) * 5) * param2 * scale);
		else if(etype==Colors) tcolor = ColorARGB32((unsigned char)(sin(i/param1 + ctime * C_PI * 2) * 255), (unsigned char)(sin(i/param1 + ctime * C_PI * 2) * 255), (unsigned char)(sin(i/param1 + ctime * C_PI * 2) * 255), (unsigned char)(sin(i/param1 + ctime * C_PI * 2) * 255));
		else if(etype==Typing) 
		{
			if(i>(ctime*m_Text.Length())) break;
		}

		if(m_Shadow)
		{
			r.left = (int)(r.left + 2 * scale);
			r.top = (int)(r.top + 2 * scale);
			rnd.DrawText(m_Font, m_Text.GetPtr(), &r, m_ShadowColor, m_DrawStyle, scale, i);

			r.left = (int)Pos.left;
			r.top = (int)Pos.top;
			r.right = (int)Pos.right;
			r.bottom = (int)Pos.bottom;

			if(etype==SinCurve) r.top = (int)(r.top + sin(i/param1 + ctime * C_PI * 2) * param2 * scale);
			else if(etype==HSinCurve) r.left = (int)(r.left + sin(i/param1 + ctime * C_PI * 2) * param2 * scale);
			else if(etype==Linear) r.top = (int)(r.top + (i/param1 + (ctime - 0.5f) * 5) * param2 * scale);
		}

		rnd.DrawText(m_Font, m_Text.GetPtr(), &r, tcolor, m_DrawStyle, scale, i);
	}
}

void GUITextEffects::Save( BaseIO& str )
{
	GUIText::Save(str);
	str.Write(speed, 0, "name=Effect Speed;category=Text Effects");
	str.Write((int)etype, 0, "name=EffectType;type=list;list=SinCurve,HSinCurve,Linear,Colors,Typing");
	str.Write(param1, 0, "name=Param1");
	str.Write(param2, 0, "name=Param2");
	str.Write(param2, 0, "name=Param3");
	str.Write(loop, 0, "name=Loop");
}

void GUITextEffects::Load( BaseIO& str )
{
	GUIText::Load(str);
	int t;
	str.Read(speed);
	str.Read(t);
	str.Read(param1);
	str.Read(param2);
	str.Read(param3);
	str.Read(loop);
	etype = (EffecTtype)t;
	ctime = 0;
}

Object* GUITextEffects::Clone()
{
	GUITextEffects* obj = new GUITextEffects();
	*obj = *this;
	return obj;
}

//Image Scroller
ImageScroller::ImageScroller(unsigned int ID, RectangleF const& location, Resource sprite, unsigned int backcolor, float rotation, bool visible, unsigned int flags, ObjectsContainer* parent, bool update) : DrawableObject(ID, location, sprite, backcolor, rotation, visible, flags, parent, update)
{
	m_type |= OBJECT_TYPE_IMAGESCROLL;
	cursel = 0;
	transpos = 1;
	rev = false;
}

void ImageScroller::Next()
{
	if((unsigned int)(cursel+1) < indices.size())
	{
		++cursel;
	}
	else
	{
		cursel = 0;
	}

	transpos = 0;
	rev = false;
}

void ImageScroller::Previous()
{
	if(cursel-1 < 0)
	{
		cursel = indices.size() - 1;
		if(cursel < 0) cursel = 0;
	}
	else
	{
		--cursel;
	}

	transpos = 0;
	rev = true;
}
	
void ImageScroller::MouseDown(unsigned short button, unsigned short buttons, PointI const& pt)
{
	DrawableObject::MouseDown(button, buttons, pt);
}

void ImageScroller::Update( float delta )
{
	DrawableObject::Update(delta);
	if(transpos<1.0f) 
	{
		transpos += delta;
		if(transpos>1) transpos = 1.0f;
	}
}

void ImageScroller::Draw(C_Renderer& rnd)
{
	if(indices.size()==0 || indices.size()<=(unsigned int)cursel) return;

	if(transpos<1.0f)
	{
		int backsel = 0;
		if(rev==false)
		{
			backsel = cursel - 1;
		}
		else
		{
			backsel = cursel + 1;
		}

		if(backsel < 0) backsel = indices.size() - 1;
		else if((unsigned int)backsel >= indices.size()) backsel = 0;

		ColorARGB32 col(m_backcolor);
		col.SetA((unsigned char)(col.GetA()*(1.0f-transpos)));
		m_sprite.SetIndex(indices[backsel]);
		m_sprite.Draw(&rnd, GetAbsolutePos(), col, GetAngle(), GetLayer());

		col = m_backcolor;
		col.SetA((unsigned char)(col.GetA()*transpos));
		m_sprite.SetIndex(indices[cursel]);
		m_sprite.Draw(&rnd, GetAbsolutePos(), col, GetAngle(), GetLayer());
	}
	else
	{
		m_sprite.SetIndex(indices[cursel]);
		m_sprite.Draw(&rnd, GetAbsolutePos(), m_backcolor, GetAngle(), GetLayer());
	}
}

void ImageScroller::Save( BaseIO& str )
{
	DrawableObject::Save(str);
	str.Write(cursel, 0, "name=Selection;category=Image Scroller");

	string st;
	stringstream s;
	for(unsigned int i = 0; i<indices.size(); i++)
	{
		s << indices[i];
		if(i!=indices.size()-1) s << ",";
	}
	st = s.str();
	str.Write(st, 0, "name=Images");
}

void ImageScroller::Load( BaseIO& str )
{
	DrawableObject::Load(str);
	str.Read(cursel);

	indices.clear();
	string st;
	str.Read(st);
	GetStringArray(indices, st.c_str(), ',');
}

Object* ImageScroller::Clone()
{
	ImageScroller* obj = new ImageScroller();
	*obj = *this;
	return obj;
}

//Function button
FunctionButton::FunctionButton( ResourceManager* mgr, unsigned int ID, RectangleF const& location, Resource sprite, unsigned int backcolor, float rotation, bool visible, unsigned int flags, ObjectsContainer* parent, bool update ) : GUIButton( mgr, ID, location, sprite, backcolor, rotation, visible, flags, parent, update )
{
	m_targetid = 100;
	m_function = ImgScrollNext;
}

 void FunctionButton::OnClick()
 {
	 if(m_function == ImgScrollNext)
	 {
		 Object* obj = Game->GetMainContainer()->GetObjectByID(m_targetid);
		 if(obj!=0 && obj->GetType()&OBJECT_TYPE_IMAGESCROLL)
		 {
			((ImageScroller*)obj)->Next();
		 }
	 }
	 else if(m_function == ImgScrollPrevious)
	 {
		 Object* obj = Game->GetMainContainer()->GetObjectByID(m_targetid);
		 if(obj!=0 && obj->GetType()&OBJECT_TYPE_IMAGESCROLL)
		 {
			((ImageScroller*)obj)->Previous();
		 }
	 }
 }

void FunctionButton::Save( BaseIO& str )
{
	GUIButton::Save(str);
	str.Write(m_targetid, 0, "name=TargetID;category=Function Button");
	str.Write((int)m_function, 0, "name=Function;type=list;list=ImageScrollerNext,ImageScrollerPrev");
}

void FunctionButton::Load( BaseIO& str )
{
	GUIButton::Load(str);
	str.Read(m_targetid);
	int fn;
	str.Read(fn);
	m_function = (Function)fn;
}

Object* FunctionButton::Clone()
{
	FunctionButton* obj = new FunctionButton();
	*obj = *this;
	return obj;
}

//Dial
Dial::Dial(unsigned int ID, RectangleF const& location, Resource sprite, unsigned int backcolor, float rotation, bool visible, unsigned int flags, ObjectsContainer* parent, bool update) : DrawableObject(ID, location, sprite, backcolor, rotation, visible, flags, parent, update)
{
	m_type |= OBJECT_TYPE_DIAL;
	m_value = 25;
	m_max = 100;
	m_min = 0;
	m_radius = 80;
	m_dotsize = 20;
	m_dotcolor = 0xFFFFFFFF;
	m_startangle = 0.0f;
	m_autodotangle = false;
	m_eangle = C_PI * 2;
	m_mdown = false;
}

void Dial::Draw(C_Renderer& rnd)
{
	DrawableObject::Draw(rnd);

	float scale = 1.0f;
	if(m_parent) scale = m_parent->GetCamera().m_scale;

	float nval = (m_value - m_min) / (m_max - m_min);
	float ang = (m_startangle + (m_eangle - m_startangle) * nval);

	PointF pt;
	RectangleF absu = GetAbsolutePos();
	float midx = absu.left + (absu.right - absu.left) / 2.0f;
	float midy = absu.top + (absu.bottom - absu.top) / 2.0f;

	float pointx = midx + cos(ang) * m_radius * scale;
	float pointy = midy + sin(ang) * m_radius * scale;

	RectangleF rect;
	rect.left = pointx - m_dotsize * scale / 2.0f;
	rect.top = pointy - m_dotsize * scale / 2.0f;
	rect.right = pointx + m_dotsize * scale / 2.0f;
	rect.bottom = pointy + m_dotsize * scale / 2.0f;

	if(!m_autodotangle) m_dot.Draw(&rnd, rect, m_dotcolor, 0.0f, GetLayer());
	else m_dot.Draw(&rnd, rect, m_dotcolor, ang, GetLayer());
}

void Dial::MouseDown(unsigned short button, unsigned short buttons, PointI const& pt)
{
	m_mdown = true;
	SetCapture();
}

void Dial::MouseUp(unsigned short button, unsigned short buttons, PointI const& pt)
{
	m_mdown = false;
	ReleaseCapture();
}

void Dial::MouseMove(PointI const& pt)
{
	if(m_mdown)
	{
		RectangleF absu = GetAbsolutePos();
		float midx = absu.left + (absu.right - absu.left) / 2.0f;
		float midy = absu.top + (absu.bottom - absu.top) / 2.0f;

		PointF dir;
		dir.left = pt.left - midx;
		dir.top = pt.top - midy;
		float length = sqrt((dir.left * dir.left) + (dir.top * dir.top));
		dir.left /= length;
		dir.top /= length;

		float ang = acos(dir.left);
		if(dir.top < 0) ang = C_PI + (C_PI - ang);

		if(ang < m_startangle) ang = m_startangle;
		else if(ang > m_eangle) ang = m_eangle;

		ang = ang - m_startangle;
		//ang = WrapAngle(ang, abs(m_eangle));		

		float nang = ang / (fabs((m_eangle - m_startangle)));

		m_value = (float)int(WrapAngle(m_min + (m_max - m_min) * (nang), m_max));
	}
}

void Dial::Save( BaseIO& str )
{
	DrawableObject::Save(str);
	str.Write(m_value, 0, "name=Value;category=Dial;help=Dial Value");
	str.Write(m_min, 0, "name=Min");
	str.Write(m_max, 0, "name=Max");
	str.Write(m_radius, 0, "name=Radius");
	str.Write(m_dotsize, 0, "name=Dot Size");
	str.Write(m_dotcolor, 0, "name=Dot Color;type=color");
	str.Write(m_autodotangle, 0, "name=AutoDotAngle");
	str.Write(m_startangle, 0, "name=Start Angle");
	str.Write(m_eangle, 0, "name=End Angle");
	m_dot.Save(str, "name=Dot Image");
}

void Dial::Load( BaseIO& str )
{
	DrawableObject::Load(str);
	str.Read(m_value);
	str.Read(m_min);
	str.Read(m_max);
	str.Read(m_radius);
	str.Read(m_dotsize);
	str.Read(m_dotcolor);
	str.Read(m_autodotangle);
	str.Read(m_startangle);
	str.Read(m_eangle);
	m_dot.Load(str);
}

Object* Dial::Clone()
{
	Dial* d = new Dial();
	*d = *this;
	return d;
}

//Sound player
SoundPlayer::SoundPlayer(unsigned int ID, RectangleF const& location, Resource sprite, unsigned int backcolor, float rotation, bool visible, unsigned int flags, ObjectsContainer* parent, bool update) : DrawableObject(ID, location, sprite, backcolor, rotation, visible, flags, parent, update)
{
	m_type |= OBJECT_TYPE_SOUNDPLAYER;
	m_flags |= OBJECT_Flag_Disabled | OBJECT_Flag_Invisible;
	m_sound = 0;
}

void SoundPlayer::Start(bool loop)
{
	player.Play(loop);
}

void SoundPlayer::Stop()
{
	player.Stop();
}

bool SoundPlayer::isPlaying()
{
	return player.IsPlaying();
}

void SoundPlayer::Save( BaseIO& str )
{
	DrawableObject::Save(str);
	str.Write(m_sound, 0, "name=SoundResource");
}

void SoundPlayer::SetResource(const char* name)
{
	Game->GetResourceManager()->GetResource(name, RES_TYPE_SOUND, &m_sound);
	player.SetBuffer(m_sound);
}

void SoundPlayer::SetVolume(long val)
{
	player.SetVolume(val);
}

long SoundPlayer::GetVolume()
{
	return player.GetVolume();
}

void SoundPlayer::Load( BaseIO& str )
{
	DrawableObject::Load(str);
	str.Read(m_sound);
	player.SetBuffer(m_sound);
}

Object* SoundPlayer::Clone()
{
	SoundPlayer* nplayer = new SoundPlayer();
	*nplayer = *this;
	return nplayer;
}