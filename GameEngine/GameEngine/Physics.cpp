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

#ifdef USEBOX2DPHYSICS
//Debug Draw
void DebugDraw::SetRenderer(C_Renderer* renderer)
{
	m_rnd = renderer;
}

C_Renderer* DebugDraw::GetRenderer()
{
	return m_rnd;
}

DebugDraw::DebugDraw(C_Renderer* renderer)
{
	m_rnd = renderer;
}

void TransformVertex2D(PointF& pt)
{
	ObjectsContainer* m_parent = Game->GetMainContainer();

	if(m_parent!=0)
	{
		pt.left = (pt.left * PIXM * m_parent->GetCamera().m_scale) + m_parent->GetAbsolutePos().left;
		pt.top = (pt.top * PIXM * m_parent->GetCamera().m_scale) + m_parent->GetAbsolutePos().top;
		
		//Transform
		pt.left *= m_parent->GetCamera().m_zoom;
		pt.top *= m_parent->GetCamera().m_zoom;

		pt.left -= m_parent->GetCamera().m_pos.left;
		pt.top -= m_parent->GetCamera().m_pos.top;
	}
}

void TransformVertex1D(float& pt)
{
	ObjectsContainer* m_parent = Game->GetMainContainer();

	if(m_parent!=0)
	{
		pt = (pt * PIXM * m_parent->GetCamera().m_scale);
		
		//Transform
		pt *= m_parent->GetCamera().m_zoom;
	}
}

void DebugDraw::DrawPolygon (const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
{
	if(m_rnd==0) return;
	PointF verts[VBMAX];
	for(int i = 0; i<VBMAX, i<vertexCount; i++)
	{
		verts[i].left = vertices[i].x;
		verts[i].top = vertices[i].y;
		TransformVertex2D(verts[i]);
	}
	m_rnd->DrawPolygon(verts, vertexCount, ColorARGB32(0xAA, (unsigned char)(255 * color.r), (unsigned char)(255 * color.g), (unsigned char)(255 * color.b)));
}

void DebugDraw::DrawSolidPolygon (const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
{
	if(m_rnd==0) return;
	PointF verts[VBMAX];
	for(int i = 0; i<VBMAX, i<vertexCount; i++)
	{
		verts[i].left = vertices[i].x;
		verts[i].top = vertices[i].y;
		TransformVertex2D(verts[i]);
	}
	m_rnd->DrawSolidPolygon(verts, vertexCount, ColorARGB32(0x8F, (unsigned char)(255 * color.r), (unsigned char)(255 * color.g), (unsigned char)(255 * color.b)));
	m_rnd->DrawPolygon(verts, vertexCount, ColorARGB32(0xFF, (unsigned char)(255 * color.r), (unsigned char)(255 * color.g), (unsigned char)(255 * color.b)));
}

void DebugDraw::DrawCircle (const b2Vec2 &center, float32 radius, const b2Color &color)
{
	if(m_rnd==0) return;
	PointF pt(center.x, center.y);
	TransformVertex2D(pt);	
	TransformVertex1D(radius);
	m_rnd->DrawCircle(pt, radius, ColorARGB32(0xAA, (unsigned char)(255 * color.r), (unsigned char)(255 * color.g), (unsigned char)(255 * color.b)));
}

void DebugDraw::DrawSolidCircle (const b2Vec2 &center, float32 radius, const b2Vec2 &axis, const b2Color &color)
{
	if(m_rnd==0) return;
	PointF pt(center.x, center.y);
	TransformVertex2D(pt);	
	TransformVertex1D(radius);
	m_rnd->DrawSolidCircle(pt, radius, ColorARGB32(0x8F, (unsigned char)(255 * color.r), (unsigned char)(255 * color.g), (unsigned char)(255 * color.b)));
	m_rnd->DrawCircle(pt, radius, ColorARGB32(0xFF, (unsigned char)(255 * color.r), (unsigned char)(255 * color.g), (unsigned char)(255 * color.b)));
}

void DebugDraw::DrawSegment (const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color)
{
	if(m_rnd==0) return;
	PointF verts[2];

	verts[0].left = p1.x;
	verts[0].top = p1.y;
	TransformVertex2D(verts[0]);

	verts[1].left = p2.x;
	verts[1].top = p2.y;
	TransformVertex2D(verts[1]);

	m_rnd->DrawLine(verts[0], verts[1], ColorARGB32(0xAA, (unsigned char)(255 * color.r), (unsigned char)(255 * color.g), (unsigned char)(255 * color.b)));
}

void DebugDraw::DrawTransform (const b2Transform &xf)
{
	if(m_rnd==0) return;
	PointF verts[2];

	verts[0].left = xf.p.x;
	verts[0].top = xf.p.y;
	TransformVertex2D(verts[0]);

	verts[1].left = verts[0].left + xf.q.c * 10.0f;
	verts[1].top = verts[0].top + xf.q.s * 10.0f;

	m_rnd->DrawLine(verts[0], verts[1], 0xFFFFFFFF);

	PointF pix(xf.p.x, xf.p.y);
	TransformVertex2D(pix);

	verts[1].left = verts[0].left + 5;
	verts[1].top = pix.top;
	m_rnd->DrawLine(verts[0], verts[1], 0xFFFF0000);

	verts[1].left = pix.left;
	verts[1].top = verts[0].top - 5;

	m_rnd->DrawLine(verts[0], verts[1], 0xFF0000FF);
}


//collision handler
void CollisionHandler::BeginContact (b2Contact *contact)
{
	b2Fixture* fixa = contact->GetFixtureA();
	b2Fixture* fixb = contact->GetFixtureB();
	b2Body* bodya = fixa->GetBody();
	b2Body* bodyb = fixb->GetBody();
	PhyObject* poa = (PhyObject*)bodya->GetUserData();
	PhyObject* pob = (PhyObject*)bodyb->GetUserData();
	poa->BeginContact(contact, pob);
	pob->BeginContact(contact, poa);
}

void CollisionHandler::EndContact (b2Contact *contact)
{
	b2Fixture* fixa = contact->GetFixtureA();
	b2Fixture* fixb = contact->GetFixtureB();
	b2Body* bodya = fixa->GetBody();
	b2Body* bodyb = fixb->GetBody();
	PhyObject* poa = (PhyObject*)bodya->GetUserData();
	PhyObject* pob = (PhyObject*)bodyb->GetUserData();
	poa->EndContact(contact, pob);
	pob->EndContact(contact, poa);
}

void CollisionHandler::PreSolve (b2Contact *contact, const b2Manifold *oldManifold)
{
	b2Fixture* fixa = contact->GetFixtureA();
	b2Fixture* fixb = contact->GetFixtureB();
	b2Body* bodya = fixa->GetBody();
	b2Body* bodyb = fixb->GetBody();
	PhyObject* poa = (PhyObject*)bodya->GetUserData();
	PhyObject* pob = (PhyObject*)bodyb->GetUserData();
	poa->PreSolve(contact, oldManifold, pob);
	pob->PreSolve(contact, oldManifold, poa);
}

void CollisionHandler::PostSolve (b2Contact *contact, const b2ContactImpulse *impulse)
{
	b2Fixture* fixa = contact->GetFixtureA();
	b2Fixture* fixb = contact->GetFixtureB();
	b2Body* bodya = fixa->GetBody();
	b2Body* bodyb = fixb->GetBody();
	PhyObject* poa = (PhyObject*)bodya->GetUserData();
	PhyObject* pob = (PhyObject*)bodyb->GetUserData();
	poa->PostSolve(contact, impulse, pob);
	pob->PostSolve(contact, impulse, poa);
}

//world
PhyWorld::PhyWorld(const b2Vec2& gravity) : m_world(gravity)
{
	m_prev = 0;
#ifdef ANDROID
	m_frequency = 20;
	m_viterations = 4;
	m_piterations = 2;
#else
	m_frequency = 60;
	m_viterations = 8;
	m_piterations = 3;
#endif
	m_update = false;
	RestoreContactListener();
}

b2World& PhyWorld::GetWorld()
{
	return m_world;
}

void PhyWorld::DrawDebug()
{
	m_world.DrawDebugData();
}

void PhyWorld::SetDebugDraw(b2Draw* val)
{
	m_world.SetDebugDraw(val);
}

void PhyWorld::ChangeContactListener(b2ContactListener* callback)
{
	m_world.SetContactListener(callback);
}

void PhyWorld::RestoreContactListener()
{
	m_world.SetContactListener(&m_colhandler);
}

void PhyWorld::Update(float delta)
{
	if(!m_update) return;
	float iterations = delta * m_frequency;
	int it = (int)floor(iterations + m_prev);
	m_prev = (iterations + m_prev) - floor(iterations + m_prev);
	if(it>MAXPHYINTERATIONS) it = MAXPHYINTERATIONS;
	for(int i = 0; i<it; i++)
	{
		ProcessDeleteQueue();

		//for(list<PhyObject*>::iterator i = m_updatelist.begin(); i!=m_updatelist.end(); i++)
		//{
		//	(*i)->PhyPreStep();
		//}

		for(list<PhyObject*>::iterator i = m_updatelist.begin(); i!=m_updatelist.end(); i++)
		{
			(*i)->PhyStep(1.0f/m_frequency);
		}

		m_world.Step(1.0f/m_frequency, m_viterations, m_piterations);
	}
}

void PhyWorld::AddToDeleteQueue(PhyObject* val)
{
	m_DeleteList.insert(val);
}

void PhyWorld::ClearAllObjects()
{
	m_DeleteList.clear();
	m_updatelist.clear();
}

void PhyWorld::ProcessDeleteQueue()
{
	for(set<PhyObject*>::iterator i = m_DeleteList.begin(); i != m_DeleteList.end(); ++i)
	{
		DeleteFromUpdateList(*i);
		(*i)->Delete();
		if(i==m_DeleteList.end()) return;
	}

	m_DeleteList.clear();
}

void PhyWorld::AddToUpdateList(PhyObject* val)
{
	m_updatelist.push_back(val);
	val->it = m_updatelist.end();
	--val->it;
	val->m_validit = true;
}

void PhyWorld::DeleteFromUpdateList(PhyObject* val)
{
	if(val->m_validit && val->it != m_updatelist.end())
	{
		m_updatelist.erase(val->it);
		val->it = m_updatelist.end();
		val->m_validit = false;
	}
}

void PhyWorld::ClearUpdateList()
{
	m_updatelist.clear();
}

bool PhyWorld::GetUpdate()
{
	return m_update;
}

void PhyWorld::SetUpdate(bool val)
{
	m_update = val;
}

float PhyWorld::GetFrequency()
{
	return m_frequency;
}

void PhyWorld::SetFrequency(float val)
{
	m_frequency = val;
}

int PhyWorld::GetVelocityIterations()
{
	return m_viterations;
}

void PhyWorld::SetVelocityIterations(int val)
{
	m_viterations = val;
}

int PhyWorld::GetPositionIterations()
{
	return m_piterations;
}

void PhyWorld::SetPositionIterations(int val)
{
	m_piterations = val;
}

//phy object
PhyObject::PhyObject(PhyWorld* world) : DrawableObject()
{
	m_type |= OBJECT_TYPE_PHYOBJECT;
	m_world = world;
	m_body = 0;
	m_validit = false;
}

void PhyObject::QueueForDelete()
{
	if(m_world)
	{
		m_world->AddToDeleteQueue(this);
	}
}

PhyObject::~PhyObject()
{
	ClearConnections();
	//if(m_world) m_world->DeleteFromUpdateList(this);
	if(m_body != 0 && m_world != 0)
	{
		m_world->GetWorld().DestroyBody(m_body);
		m_body = 0;
	}
}

b2Body* PhyObject::GetBody()
{
	return m_body;
}

RectangleF& PhyObject::GetObjectArea()
{
	if(m_body!=0)
	{
		b2Vec2 pos = m_body->GetPosition();
		m_location.left = pos.x * PIXM - 10;
		m_location.right = pos.x  * PIXM + 10;
		m_location.top = pos.y * PIXM - 10;
		m_location.bottom = pos.y  * PIXM + 10;
	}
	return m_location;
}

void PhyObject::SetObjectArea(const RectangleF& val)
{
	if(m_body!=0)
	{
		b2Vec2 pos;
		pos.x = (val.left + 10) / PIXM;
		pos.y = (val.top + 10) / PIXM;
		m_body->SetTransform(pos, m_body->GetAngle());
	}
}

float PhyObject::GetAngle()
{
	if(m_body!=0)
	{
		return m_body->GetAngle();
	}
	return 0;
}

void PhyObject::SetAngle(float val)
{
	if(m_body!=0)
	{
		m_body->SetTransform(m_body->GetPosition(), val);
	}
}

void PhyObject::Process( unsigned int Msg, unsigned int Param1, void* Param2 )
{
	DrawableObject::Process(Msg, Param1, Param2);
}

void PhyObject::Update( float delta )
{
	DrawableObject::Update(delta);
}

//joint
PhyJoint::PhyJoint(PhyWorld* world)
{
	m_type |= OBJECT_TYPE_PHYJOINT;
	m_world = world;
	m_joint = 0;
	m_jointtype = 0;
	m_refangle = 0;
	m_collidecon = false;
	m_enablelimit = false;
	m_limit1 = 0;
	m_limit2 = 0;
	m_enablemotor = false;
	m_motortorque = 20;
	m_motorspeed = C_PI * 2;
	m_editing = false;
	m_b1anchor.x = 0;
	m_b1anchor.y = 0;
	m_b2anchor.x = 0;
	m_b2anchor.y = 0;
	m_jointaxis.x = 0;
	m_jointaxis.y = -1;
	m_jointaxis2.x = 0;
	m_jointaxis2.y = -1;
	m_frequency = 0;
	m_dampingratio = 0;
}

PhyJoint::PhyJoint(PhyWorld* m_world, int type)
{
	m_type |= OBJECT_TYPE_PHYJOINT;
	this->m_world = m_world;
	m_joint = 0;
	m_jointtype = 0;
	m_refangle = 0;
	m_collidecon = false;
	m_enablelimit = false;
	m_limit1 = 0;
	m_limit2 = 0;
	m_enablemotor = false;
	m_motortorque = 20;
	m_motorspeed = C_PI * 2;
	m_editing = false;
	m_b1anchor.x = 0;
	m_b1anchor.y = 0;
	m_b2anchor.x = 0;
	m_b2anchor.y = 0;
	m_jointaxis.x = 0;
	m_jointaxis.y = -1;
	m_jointaxis2.x = 0;
	m_jointaxis2.y = -1;
	m_frequency = 0;
	m_dampingratio = 0;
	obj1.Disconnected.RegisterHandler<PhyJoint, &PhyJoint::Disconnected>(this);
	obj2.Disconnected.RegisterHandler<PhyJoint, &PhyJoint::Disconnected>(this);
}

PhyJoint::~PhyJoint()
{
	if(m_world && m_joint)
	{
		m_world->GetWorld().DestroyJoint(m_joint);
		m_joint = 0;
	}
}

void PhyJoint::Disconnected(DrawableObject* obj)
{
	if(m_world && m_joint)
	{
		m_world->GetWorld().DestroyJoint(m_joint);
		m_joint = 0;
	}
}

const PhyJoint& PhyJoint::operator=(const PhyJoint& val)
{
	DrawableObject::operator=(val);
	m_jointtype = val.m_jointtype;
	m_refangle = val.m_refangle;
	m_collidecon = val.m_collidecon;
	m_enablelimit = val.m_enablelimit;
	m_limit1 = val.m_limit1;
	m_limit2 = val.m_limit2;
	m_enablemotor = val.m_enablemotor;
	m_motortorque = val.m_motortorque;
	m_motorspeed = val.m_motorspeed;
	m_b1anchor = val.m_b1anchor;
	m_b2anchor = val.m_b2anchor;
	m_jointaxis = val.m_jointaxis;
	m_frequency = val.m_frequency;
	m_dampingratio = val.m_dampingratio;
	m_jointaxis2 = val.m_jointaxis2;
	UpdateJoint();
	return *this;
}

void PhyJoint::MouseDown(unsigned short button, unsigned short buttons, PointI const& pt)
{
	DrawableObject::MouseDown(button, buttons, pt);
	if(m_editing)
	{
		RectangleF pos;		

		if(button == KEY_LBUTTON)
		{
			float hx = (obj1.GetObject()->GetObjectArea().right - obj1.GetObject()->GetObjectArea().left);
			float hy = (obj1.GetObject()->GetObjectArea().bottom - obj1.GetObject()->GetObjectArea().top);
			pos.left = (float)pt.left;
			pos.top = (float)pt.top;

			RectangleF rel = obj1.GetObject()->ToRelativePos(pos);
			rel.left /= hx; 
			rel.top /= hy;
			hx /= PIXM;
			hy /= PIXM;
			rel.left = rel.left * hx * 2 * PIXM - hx * PIXM;
			rel.top = rel.top * hy * 2 * PIXM - hy * PIXM;
			rel.left /= PIXM;
			rel.top /= PIXM;

			if(m_joint)
			{
				m_b1anchor.x = rel.left / 2.0f;
				m_b1anchor.y = rel.top / 2.0f;
				UpdateJoint();
			}
		}
		else if(button == KEY_RBUTTON)
		{
			float hx = (obj2.GetObject()->GetObjectArea().right - obj2.GetObject()->GetObjectArea().left);
			float hy = (obj2.GetObject()->GetObjectArea().bottom - obj2.GetObject()->GetObjectArea().top);
			pos.left = (float)pt.left;
			pos.top = (float)pt.top;

			RectangleF rel = obj2.GetObject()->ToRelativePos(pos);
			rel.left /= hx; 
			rel.top /= hy;
			hx /= PIXM;
			hy /= PIXM;
			rel.left = rel.left * hx * 2 * PIXM - hx * PIXM;
			rel.top = rel.top * hy * 2 * PIXM - hy * PIXM;
			rel.left /= PIXM;
			rel.top /= PIXM;

			if(m_joint)
			{
				m_b2anchor.x = rel.left / 2.0f;
				m_b2anchor.y = rel.top / 2.0f;
				UpdateJoint();
			}
		}
		else
		{
			float hx = (obj1.GetObject()->GetObjectArea().right - obj1.GetObject()->GetObjectArea().left);
			float hy = (obj1.GetObject()->GetObjectArea().bottom - obj1.GetObject()->GetObjectArea().top);
			pos.left = (float)pt.left;
			pos.top = (float)pt.top;

			RectangleF rel = obj1.GetObject()->ToRelativePos(pos);
			rel.left /= hx; 
			rel.top /= hy;
			hx /= PIXM;
			hy /= PIXM;
			rel.left = rel.left * hx * 2 * PIXM - hx * PIXM;
			rel.top = rel.top * hy * 2 * PIXM - hy * PIXM;
			rel.left /= PIXM;
			rel.top /= PIXM;

			if(m_joint)
			{
				m_jointaxis.x = rel.left / 2.0f;
				m_jointaxis.y = rel.top / 2.0f;
				m_jointaxis.Normalize();
				UpdateJoint();
			}
		}
		ReleaseCapture();
		m_editing = false;
	}
}

void PhyJoint::KeyDown(unsigned int key)
{
	DrawableObject::KeyDown(key);
	if(!IsEditing()) return;
	if(key == KEY_E)
	{
		if(obj1.IsValid() && obj2.IsValid())
		{
			m_editing = true;
			SetCapture();
		}
	}
}

b2Joint* PhyJoint::GetJoint()
{
	return m_joint;
}

void PhyJoint::UpdateJoint()
{
	if(obj1.IsValid() && obj2.IsValid())
	{
		if(m_jointtype==8 && obj1.GetObject()->GetType()&OBJECT_TYPE_PHYJOINT && obj2.GetObject()->GetType()&OBJECT_TYPE_PHYJOINT)
		{
			PhyJoint* o1 = (PhyJoint*)obj1.GetObject();
			PhyJoint* o2 = (PhyJoint*)obj2.GetObject();

			if(o1->obj1.IsValid() && o2->obj1.IsValid() && o1->GetJoint()!=0 && o2->GetJoint()!=0)
			{
				if(o1->obj1.GetObject()->GetType()&OBJECT_TYPE_PHYSICSOBJ && o2->obj1.GetObject()->GetType()&OBJECT_TYPE_PHYSICSOBJ)
				{
					if(m_joint) m_world->GetWorld().DestroyJoint(m_joint);

					b2GearJointDef jointdef;
					jointdef.bodyA = ((PhysicsObject*) o1->obj1.GetObject())->GetBody();
					jointdef.bodyB = ((PhysicsObject*) o2->obj1.GetObject())->GetBody();
					jointdef.collideConnected = m_collidecon;
					jointdef.joint1 = o1->GetJoint();
					jointdef.joint2 = o2->GetJoint();
					jointdef.ratio = m_dampingratio;
					m_joint = m_world->GetWorld().CreateJoint(&jointdef);
				}
			}
		}
		else if(obj1.GetObject()->GetType()&OBJECT_TYPE_PHYSICSOBJ && obj2.GetObject()->GetType()&OBJECT_TYPE_PHYSICSOBJ)
		{
			if(m_joint) m_world->GetWorld().DestroyJoint(m_joint);
			if(m_jointtype == 0)
			{
				b2RevoluteJointDef jointdef;
				jointdef.bodyA = ((PhysicsObject*) obj1.GetObject())->GetBody();
				jointdef.bodyB = ((PhysicsObject*) obj2.GetObject())->GetBody();
				jointdef.collideConnected = m_collidecon;
				jointdef.localAnchorA = m_b1anchor;
				jointdef.localAnchorB = m_b2anchor;
				jointdef.referenceAngle = m_refangle;
				jointdef.enableLimit = m_enablelimit;
				jointdef.lowerAngle = m_limit1;
				jointdef.upperAngle = m_limit2;
				jointdef.enableMotor = m_enablemotor;
				jointdef.maxMotorTorque = m_motortorque;
				jointdef.motorSpeed = m_motorspeed;
				m_joint = m_world->GetWorld().CreateJoint(&jointdef);
			}
			else if(m_jointtype == 1)
			{
				b2PrismaticJointDef jointdef;
				jointdef.bodyA = ((PhysicsObject*) obj1.GetObject())->GetBody();
				jointdef.bodyB = ((PhysicsObject*) obj2.GetObject())->GetBody();
				jointdef.collideConnected = m_collidecon;
				jointdef.localAnchorA = m_b1anchor;
				jointdef.localAnchorB = m_b2anchor;
				jointdef.referenceAngle = m_refangle;
				jointdef.enableLimit = m_enablelimit;
				jointdef.lowerTranslation = m_limit1;
				jointdef.upperTranslation = m_limit2;
				jointdef.enableMotor = m_enablemotor;
				jointdef.maxMotorForce = m_motortorque;
				jointdef.localAxisA = m_jointaxis;
				jointdef.motorSpeed = m_motorspeed;
				m_joint = m_world->GetWorld().CreateJoint(&jointdef);
			}
			else if(m_jointtype == 2)
			{
				b2DistanceJointDef jointdef;
				jointdef.bodyA = ((PhysicsObject*) obj1.GetObject())->GetBody();
				jointdef.bodyB = ((PhysicsObject*) obj2.GetObject())->GetBody();
				jointdef.collideConnected = m_collidecon;
				jointdef.localAnchorA = m_b1anchor;
				jointdef.localAnchorB = m_b2anchor;
				jointdef.frequencyHz = m_frequency;
				jointdef.dampingRatio = m_dampingratio;
				jointdef.length = m_limit1;	
				m_joint = m_world->GetWorld().CreateJoint(&jointdef);
			}
			else if(m_jointtype == 3)
			{
				b2PulleyJointDef jointdef;
				jointdef.bodyA = ((PhysicsObject*) obj1.GetObject())->GetBody();
				jointdef.bodyB = ((PhysicsObject*) obj2.GetObject())->GetBody();
				jointdef.collideConnected = m_collidecon;
				jointdef.localAnchorA = m_b1anchor;
				jointdef.localAnchorB = m_b2anchor;
				jointdef.groundAnchorA = m_jointaxis;
				jointdef.groundAnchorB = m_jointaxis2;
				jointdef.ratio = m_dampingratio;
				jointdef.lengthA = m_limit1;
				jointdef.lengthB = m_limit1;
				m_joint = m_world->GetWorld().CreateJoint(&jointdef);
			}
			else if(m_jointtype == 4)
			{
				b2WheelJointDef jointdef;
				jointdef.bodyA = ((PhysicsObject*) obj1.GetObject())->GetBody();
				jointdef.bodyB = ((PhysicsObject*) obj2.GetObject())->GetBody();
				jointdef.collideConnected = m_collidecon;
				jointdef.localAnchorA = m_b1anchor;
				jointdef.localAnchorB = m_b2anchor;
				jointdef.frequencyHz = m_frequency;
				jointdef.dampingRatio = m_dampingratio;
				jointdef.enableMotor = m_enablemotor;
				jointdef.maxMotorTorque = m_motortorque;
				jointdef.localAxisA = m_jointaxis;
				jointdef.motorSpeed = m_motorspeed;
				m_joint = m_world->GetWorld().CreateJoint(&jointdef);
			}
			else if(m_jointtype == 5)
			{
				b2WeldJointDef jointdef;
				jointdef.bodyA = ((PhysicsObject*) obj1.GetObject())->GetBody();
				jointdef.bodyB = ((PhysicsObject*) obj2.GetObject())->GetBody();
				jointdef.collideConnected = m_collidecon;
				jointdef.localAnchorA = m_b1anchor;
				jointdef.localAnchorB = m_b2anchor;
				jointdef.frequencyHz = m_frequency;
				jointdef.dampingRatio = m_dampingratio;
				jointdef.referenceAngle = m_refangle;
				m_joint = m_world->GetWorld().CreateJoint(&jointdef);
			}
			else if(m_jointtype == 6)
			{
				b2RopeJointDef jointdef;
				jointdef.bodyA = ((PhysicsObject*) obj1.GetObject())->GetBody();
				jointdef.bodyB = ((PhysicsObject*) obj2.GetObject())->GetBody();
				jointdef.collideConnected = m_collidecon;
				jointdef.localAnchorA = m_b1anchor;
				jointdef.localAnchorB = m_b2anchor;
				jointdef.maxLength = m_limit1;
				m_joint = m_world->GetWorld().CreateJoint(&jointdef);
			}
			else if(m_jointtype == 7)
			{
				b2FrictionJointDef jointdef;
				jointdef.bodyA = ((PhysicsObject*) obj1.GetObject())->GetBody();
				jointdef.bodyB = ((PhysicsObject*) obj2.GetObject())->GetBody();
				jointdef.collideConnected = m_collidecon;
				jointdef.localAnchorA = m_b1anchor;
				jointdef.localAnchorB = m_b2anchor;
				jointdef.maxForce = m_limit1;
				jointdef.maxTorque = m_limit2;
				m_joint = m_world->GetWorld().CreateJoint(&jointdef);
			}
		}
	}
}

void PhyJoint::SetLimits(float limit1, float limit2)
{
	if(m_joint)
	{
		m_limit1 = limit1;
		m_limit2 = limit2;
		if(m_joint->GetType() == e_distanceJoint)
		{
			b2DistanceJoint* joint = (b2DistanceJoint*)m_joint;
			joint->SetLength(limit1);
		}
		else if(m_joint->GetType() == e_frictionJoint)
		{
			b2FrictionJoint* joint = (b2FrictionJoint*)m_joint;
			joint->SetMaxForce(limit1);
			joint->SetMaxTorque(limit2);
		}
		else if(m_joint->GetType() == e_gearJoint)
		{
			b2GearJoint* joint = (b2GearJoint*)m_joint;
			
		}
		else if(m_joint->GetType() == e_prismaticJoint)
		{
			b2PrismaticJoint* joint = (b2PrismaticJoint*)m_joint;
			joint->SetLimits(limit1, limit2);
		}
		else if(m_joint->GetType() == e_pulleyJoint)
		{
			b2PulleyJoint* joint = (b2PulleyJoint*)m_joint;

		}
		else if(m_joint->GetType() == e_revoluteJoint)
		{
			b2RevoluteJoint* joint = (b2RevoluteJoint*)m_joint;
			joint->SetLimits(limit1, limit2);
		}
		else if(m_joint->GetType() == e_ropeJoint)
		{
			b2RopeJoint* joint = (b2RopeJoint*)m_joint;
			joint->SetMaxLength(limit1);
		}
		else if(m_joint->GetType() == e_weldJoint)
		{
			b2WeldJoint* joint = (b2WeldJoint*)m_joint;
			
		}
		else if(m_joint->GetType() == e_wheelJoint)
		{
			b2WheelJoint* joint = (b2WheelJoint*)m_joint;
		}
	}
}

void PhyJoint::EnableLimits(bool val)
{
	if(m_joint)
	{
		m_enablelimit = val;
		if(m_joint->GetType() == e_distanceJoint)
		{
			b2DistanceJoint* joint = (b2DistanceJoint*)m_joint;
		}
		else if(m_joint->GetType() == e_frictionJoint)
		{
			b2FrictionJoint* joint = (b2FrictionJoint*)m_joint;
		}
		else if(m_joint->GetType() == e_gearJoint)
		{
			b2GearJoint* joint = (b2GearJoint*)m_joint;			
		}
		else if(m_joint->GetType() == e_prismaticJoint)
		{
			b2PrismaticJoint* joint = (b2PrismaticJoint*)m_joint;
			joint->EnableLimit(val);
		}
		else if(m_joint->GetType() == e_pulleyJoint)
		{
			b2PulleyJoint* joint = (b2PulleyJoint*)m_joint;
		}
		else if(m_joint->GetType() == e_revoluteJoint)
		{
			b2RevoluteJoint* joint = (b2RevoluteJoint*)m_joint;
			joint->EnableLimit(val);
		}
		else if(m_joint->GetType() == e_ropeJoint)
		{
			b2RopeJoint* joint = (b2RopeJoint*)m_joint;
		}
		else if(m_joint->GetType() == e_weldJoint)
		{
			b2WeldJoint* joint = (b2WeldJoint*)m_joint;			
		}
		else if(m_joint->GetType() == e_wheelJoint)
		{
			b2WheelJoint* joint = (b2WheelJoint*)m_joint;
		}
	}
}

void PhyJoint::EnableMotor(bool val)
{
	if(m_joint)
	{
		m_enablemotor = val;
		if(m_joint->GetType() == e_distanceJoint)
		{
			b2DistanceJoint* joint = (b2DistanceJoint*)m_joint;
		}
		else if(m_joint->GetType() == e_frictionJoint)
		{
			b2FrictionJoint* joint = (b2FrictionJoint*)m_joint;			
		}
		else if(m_joint->GetType() == e_gearJoint)
		{
			b2GearJoint* joint = (b2GearJoint*)m_joint;
		}
		else if(m_joint->GetType() == e_prismaticJoint)
		{
			b2PrismaticJoint* joint = (b2PrismaticJoint*)m_joint;
			joint->EnableMotor(val);
		}
		else if(m_joint->GetType() == e_pulleyJoint)
		{
			b2PulleyJoint* joint = (b2PulleyJoint*)m_joint;
		}
		else if(m_joint->GetType() == e_revoluteJoint)
		{
			b2RevoluteJoint* joint = (b2RevoluteJoint*)m_joint;
			joint->EnableMotor(val);
		}
		else if(m_joint->GetType() == e_ropeJoint)
		{
			b2RopeJoint* joint = (b2RopeJoint*)m_joint;
		}
		else if(m_joint->GetType() == e_weldJoint)
		{
			b2WeldJoint* joint = (b2WeldJoint*)m_joint;			
		}
		else if(m_joint->GetType() == e_wheelJoint)
		{
			b2WheelJoint* joint = (b2WheelJoint*)m_joint;
			joint->EnableMotor(val);
		}
	}
}

void PhyJoint::SetMotorSpeed(bool val)
{
	if(m_joint)
	{
		m_motorspeed = val;
		if(m_joint->GetType() == e_distanceJoint)
		{
			b2DistanceJoint* joint = (b2DistanceJoint*)m_joint;
		}
		else if(m_joint->GetType() == e_frictionJoint)
		{
			b2FrictionJoint* joint = (b2FrictionJoint*)m_joint;			
		}
		else if(m_joint->GetType() == e_gearJoint)
		{
			b2GearJoint* joint = (b2GearJoint*)m_joint;
		}
		else if(m_joint->GetType() == e_prismaticJoint)
		{
			b2PrismaticJoint* joint = (b2PrismaticJoint*)m_joint;
			joint->SetMotorSpeed(val);
		}
		else if(m_joint->GetType() == e_pulleyJoint)
		{
			b2PulleyJoint* joint = (b2PulleyJoint*)m_joint;
		}
		else if(m_joint->GetType() == e_revoluteJoint)
		{
			b2RevoluteJoint* joint = (b2RevoluteJoint*)m_joint;
			joint->SetMotorSpeed(val);
		}
		else if(m_joint->GetType() == e_ropeJoint)
		{
			b2RopeJoint* joint = (b2RopeJoint*)m_joint;
		}
		else if(m_joint->GetType() == e_weldJoint)
		{
			b2WeldJoint* joint = (b2WeldJoint*)m_joint;			
		}
		else if(m_joint->GetType() == e_wheelJoint)
		{
			b2WheelJoint* joint = (b2WheelJoint*)m_joint;
			joint->SetMotorSpeed(val);
		}
	}
}

void PhyJoint::SetMotorPower(bool val)
{
	if(m_joint)
	{
		m_motortorque = val;
		if(m_joint->GetType() == e_distanceJoint)
		{
			b2DistanceJoint* joint = (b2DistanceJoint*)m_joint;
		}
		else if(m_joint->GetType() == e_frictionJoint)
		{
			b2FrictionJoint* joint = (b2FrictionJoint*)m_joint;			
		}
		else if(m_joint->GetType() == e_gearJoint)
		{
			b2GearJoint* joint = (b2GearJoint*)m_joint;
		}
		else if(m_joint->GetType() == e_prismaticJoint)
		{
			b2PrismaticJoint* joint = (b2PrismaticJoint*)m_joint;
			joint->SetMaxMotorForce(val);
		}
		else if(m_joint->GetType() == e_pulleyJoint)
		{
			b2PulleyJoint* joint = (b2PulleyJoint*)m_joint;
		}
		else if(m_joint->GetType() == e_revoluteJoint)
		{
			b2RevoluteJoint* joint = (b2RevoluteJoint*)m_joint;
			joint->SetMaxMotorTorque(val);
		}
		else if(m_joint->GetType() == e_ropeJoint)
		{
			b2RopeJoint* joint = (b2RopeJoint*)m_joint;
		}
		else if(m_joint->GetType() == e_weldJoint)
		{
			b2WeldJoint* joint = (b2WeldJoint*)m_joint;			
		}
		else if(m_joint->GetType() == e_wheelJoint)
		{
			b2WheelJoint* joint = (b2WheelJoint*)m_joint;
			joint->SetMaxMotorTorque(val);
		}
	}
}

void PhyJoint::SetDampingRatio(float val)
{
	if(m_joint)
	{
		m_dampingratio = val;
		if(m_joint->GetType() == e_distanceJoint)
		{
			b2DistanceJoint* joint = (b2DistanceJoint*)m_joint;
			joint->SetDampingRatio(val);
		}
		else if(m_joint->GetType() == e_frictionJoint)
		{
			b2FrictionJoint* joint = (b2FrictionJoint*)m_joint;		
		}
		else if(m_joint->GetType() == e_gearJoint)
		{
			b2GearJoint* joint = (b2GearJoint*)m_joint;
			joint->SetRatio(val);
		}
		else if(m_joint->GetType() == e_prismaticJoint)
		{
			b2PrismaticJoint* joint = (b2PrismaticJoint*)m_joint;
		}
		else if(m_joint->GetType() == e_pulleyJoint)
		{
			b2PulleyJoint* joint = (b2PulleyJoint*)m_joint;
		}
		else if(m_joint->GetType() == e_revoluteJoint)
		{
			b2RevoluteJoint* joint = (b2RevoluteJoint*)m_joint;
		}
		else if(m_joint->GetType() == e_ropeJoint)
		{
			b2RopeJoint* joint = (b2RopeJoint*)m_joint;
		}
		else if(m_joint->GetType() == e_weldJoint)
		{
			b2WeldJoint* joint = (b2WeldJoint*)m_joint;
			joint->SetDampingRatio(val);
		}
		else if(m_joint->GetType() == e_wheelJoint)
		{
			b2WheelJoint* joint = (b2WheelJoint*)m_joint;
			joint->SetSpringDampingRatio(val);
		}
	}
}

void PhyJoint::SetFrequency(float val)
{
	if(m_joint)
	{
		m_frequency = val;
		if(m_joint->GetType() == e_distanceJoint)
		{
			b2DistanceJoint* joint = (b2DistanceJoint*)m_joint;
			joint->SetFrequency(val);
		}
		else if(m_joint->GetType() == e_frictionJoint)
		{
			b2FrictionJoint* joint = (b2FrictionJoint*)m_joint;		
		}
		else if(m_joint->GetType() == e_gearJoint)
		{
			b2GearJoint* joint = (b2GearJoint*)m_joint;
		}
		else if(m_joint->GetType() == e_prismaticJoint)
		{
			b2PrismaticJoint* joint = (b2PrismaticJoint*)m_joint;
		}
		else if(m_joint->GetType() == e_pulleyJoint)
		{
			b2PulleyJoint* joint = (b2PulleyJoint*)m_joint;
		}
		else if(m_joint->GetType() == e_revoluteJoint)
		{
			b2RevoluteJoint* joint = (b2RevoluteJoint*)m_joint;
		}
		else if(m_joint->GetType() == e_ropeJoint)
		{
			b2RopeJoint* joint = (b2RopeJoint*)m_joint;
		}
		else if(m_joint->GetType() == e_weldJoint)
		{
			b2WeldJoint* joint = (b2WeldJoint*)m_joint;
			joint->SetFrequency(val);
		}
		else if(m_joint->GetType() == e_wheelJoint)
		{
			b2WheelJoint* joint = (b2WheelJoint*)m_joint;
			joint->SetSpringFrequencyHz(val);
		}
	}
}

bool PhyJoint::IsLimitEnabled()
{
	return m_enablelimit;
}

bool PhyJoint::IsMotorEnabled()
{
	return m_enablemotor;
}

float PhyJoint::GetLimit1()
{
	return m_limit1;
}

float PhyJoint::GetLimit2()
{
	return m_limit2;
}

float PhyJoint::GetMotorPower()
{
	return m_motortorque;
}

float PhyJoint::GetMootorSpeed()
{
	return m_motorspeed;
}

float PhyJoint::GetDampingRatio()
{
	return m_dampingratio;
}

float PhyJoint::GetFrequency()
{
	return m_frequency;
}

RectangleF& PhyJoint::GetObjectArea()
{
	return DrawableObject::GetObjectArea();
}

void PhyJoint::SetObjectArea(const RectangleF& val)
{
	DrawableObject::SetObjectArea(val);
}

void PhyJoint::Update( float delta )
{
	DrawableObject::Update(delta);
}

void PhyJoint::PostLoadAll()
{
	DrawableObject::PostLoadAll();
	obj1.UpdateConnection();
	obj2.UpdateConnection();
	UpdateJoint();
}

void PhyJoint::Save( BaseIO& str )
{
	DrawableObject::Save(str);
	str.Write(m_jointtype, 0, "name=JointType;category=PhysicsJoint;type=list;list=Revolute,Prismatic,Distance,Pully,Wheel,Weld,Rope,Friction,Gear");
	str.Write(obj1.GetConnectionID(), 0, "name=Object1ID");
	str.Write(obj2.GetConnectionID(), 0, "name=Object2ID");

	str.Write(m_b1anchor.x, 0, "name=Obj1AnchorX");
	str.Write(m_b1anchor.y, 0, "name=Obj1AnchorY");
	str.Write(m_b2anchor.x, 0, "name=Obj2AnchorX");
	str.Write(m_b2anchor.y, 0, "name=Obj2AnchorY");
	str.Write(m_jointaxis.x, 0, "name=JointAxisX");
	str.Write(m_jointaxis.y, 0, "name=jointAxisY");
	str.Write(m_jointaxis2.x, 0, "name=JointAxisX2");
	str.Write(m_jointaxis2.y, 0, "name=jointAxisY2");

	str.Write(m_collidecon, 0, "name=Collide Connected");
	str.Write(m_enablelimit, 0, "name=Enable Limit");
	str.Write(m_limit1, 0, "name=Limit1");
	str.Write(m_limit2, 0, "name=Limit2");
	str.Write(m_refangle, 0, "name=Referance Angle");
	str.Write(m_enablemotor, 0, "name=Enable Motor");
	str.Write(m_motortorque, 0, "name=Motor Torque");
	str.Write(m_motorspeed, 0, "name=Motor Speed");

	str.Write(m_frequency, 0, "name=Frequency");
	str.Write(m_dampingratio, 0, "name=DampingRatio");
}

void PhyJoint::Load( BaseIO& str )
{
	DrawableObject::Load(str);
	str.Read(m_jointtype);
	unsigned int tmp = 0;
	str.Read(tmp);
	obj1.SetConnectionID(tmp);
	tmp = 0;
	str.Read(tmp);
	obj2.SetConnectionID(tmp);

	str.Read(m_b1anchor.x);
	str.Read(m_b1anchor.y);
	str.Read(m_b2anchor.x);
	str.Read(m_b2anchor.y);
	str.Read(m_jointaxis.x);
	str.Read(m_jointaxis.y);
	str.Read(m_jointaxis2.x);
	str.Read(m_jointaxis2.y);

	str.Read(m_collidecon);
	str.Read(m_enablelimit);
	str.Read(m_limit1);
	str.Read(m_limit2);
	str.Read(m_refangle);
	str.Read(m_enablemotor);
	str.Read(m_motortorque);
	str.Read(m_motorspeed);

	str.Read(m_frequency);
	str.Read(m_dampingratio);
	UpdateJoint();
}

Object* PhyJoint::Clone()
{
	PhyJoint* jnt = new PhyJoint(m_world, 0);
	*jnt = *this;
	return jnt;
}

//Physics object
#ifdef USEBOX2DPHYSICS
PhysicsObject::PhysicsObject(PhyWorld* m_world,bool solid) : PhyObject(m_world)
{
	m_type |= OBJECT_TYPE_PHYSICSOBJ;
	m_flags |= OBJECT_Flag_Tansform;
	m_object = 0;
	m_objtype = 0;
	m_team = Team0_Category;
	m_teammask = Team0_Mask;

	m_current = 0;
	m_delay = 0;
	m_speed = 0;
	m_animtype = 0;
	m_playtype = 0;
	m_cpos = 0;
	m_rspos = 0.0f;
	m_repos = C_PI * 2.0f;
	m_scale = 1.0f;

	sensor = false;
	objtype = 0;
	shapetype = 0;
	editing = false;
}

PhysicsObject::PhysicsObject(PhyWorld* m_world, float hx, float hy, short team, short mask, int objtype, bool sensor) : PhyObject(m_world)
{
	m_type |= OBJECT_TYPE_PHYSICSOBJ;
	m_flags |= OBJECT_Flag_Tansform;
	m_object = 0;
	m_team = Team0_Category;
	m_teammask = Team0_Mask;
	m_objtype = 0;
	m_current = 0;
	m_delay = 0;
	m_speed = 0.2f;
	m_animtype = 0;
	m_playtype = 0;
	m_cpos = 0;
	m_rspos = 0.0f;
	m_repos = C_PI * 2.0f;
	m_scale = 1.0f;
	m_location.left = 100;
	m_location.top = 100;
	m_location.right = 200;
	m_location.bottom = 200;
	editing = false;

	this->sensor = sensor;
	this->objtype = objtype;

	m_world->AddToUpdateList(this);
	b2PolygonShape shape;
	shape.SetAsBox(hx / PIXM, hy / PIXM, b2Vec2(0,0), 0.0f);
	
	b2BodyDef bdef;
	if(objtype == 0) bdef.type = b2_dynamicBody;
	else if(objtype == 1) bdef.type = b2_staticBody;
	else if(objtype == 2) bdef.type = b2_kinematicBody;

	bdef.position = b2Vec2(100.0f / PIXM, 100.0f / PIXM);

	m_body =  m_world->GetWorld().CreateBody(&bdef);

	b2FixtureDef fix;
	fix.density = 1.0f;
	fix.filter.categoryBits = team;
	fix.filter.maskBits = mask;
	fix.filter.groupIndex = 0;
	fix.isSensor = false;
	fix.shape = &shape;
	
	m_object = m_body->CreateFixture(&fix);
	m_body->SetUserData(this);

	//m_body->SetLinearVelocity(b2Vec2(0, 1.4f));
	shapetype = 0;
}

float PhysicsObject::GetHx() const
{
	if(m_body!=0 && m_object!=0)
	{
		/*b2Vec2 pos = m_body->GetPosition();
		b2PolygonShape* shape = (b2PolygonShape*)m_object->GetShape();*/
	
		//return abs(shape->m_vertices[2].x - shape->m_vertices[0].x)/2.0f;
		return (m_location.right - m_location.left) / 2.0f / PIXM;
	}
	return 0;
}

float PhysicsObject::GetHy() const
{
	if(m_body!=0 && m_object!=0)
	{
		/*b2Vec2 pos = m_body->GetPosition();
		b2PolygonShape* shape = (b2PolygonShape*)m_object->GetShape();
	
		return abs(shape->m_vertices[2].y - shape->m_vertices[0].y)/2.0f;*/
		return (m_location.bottom - m_location.top) / 2.0f / PIXM;
	}
	return 0;
}

short PhysicsObject::GetTeam() const
{
	return m_team;
}

short PhysicsObject::GetTeamMask() const
{
	return m_teammask;
}

void PhysicsObject::SetSensor(bool val)
{
	if(m_object!=0)
	{
		for(b2Fixture* f = m_body->GetFixtureList(); f; f = f->GetNext())
		{
			sensor = val;
			f->SetSensor(val);
		}
	}
}

bool PhysicsObject::GetSensor() const
{
	if(m_object!=0)
	{
		return m_object->IsSensor();
	}
	return false;
}

void PhysicsObject::SetBodyType(int val)
{
	if(m_body!=0)
	{
		objtype = val;
		if(objtype == 0) m_body->SetType(b2_dynamicBody);
		else if(objtype == 1) m_body->SetType(b2_staticBody);
		else if(objtype == 2) m_body->SetType(b2_kinematicBody);
	}
}

int PhysicsObject::GetBodyType() const
{
	if(m_body!=0)
	{
		return objtype;
	}

	return 0;
}

void PhysicsObject::SetVelocity(const PointF& vel)
{
	if(m_body!=0)
	{
		m_body->SetLinearVelocity(b2Vec2(vel.left/PIXM, vel.top/PIXM));
	}
}

PointF PhysicsObject::GetVelocity() const
{
	if(m_body!=0)
	{
		b2Vec2 vel =  m_body->GetLinearVelocity();
		return PointF(vel.x * PIXM, vel.y * PIXM);
	}

	return PointF(0, 0);
}

void PhysicsObject::ApplyForce(const PointF& vel)
{
	if(m_body!=0)
	{
		m_body->ApplyForceToCenter(b2Vec2(vel.left/PIXM, vel.top/PIXM));
	}
}

void PhysicsObject::ApplyImpulse(const PointF& vel)
{
	if(m_body!=0)
	{
		m_body->ApplyLinearImpulse(b2Vec2(vel.left/PIXM, vel.top/PIXM), m_body->GetPosition());
	}
}

void PhysicsObject::ApplyTorque(float val)
{
	if(m_body!=0)
	{
		m_body->ApplyTorque(val);
	}
}

void PhysicsObject::ApplyAngImpulse(float vel)
{
	if(m_body!=0)
	{
		m_body->ApplyAngularImpulse(vel);
	}
}

void PhysicsObject::SetTeam(short val, short mask)
{
	if(m_object!=0)
	{
		b2Filter filter = m_object->GetFilterData();
		filter.categoryBits = val;
		filter.maskBits = mask;
		for(b2Fixture* f = m_body->GetFixtureList(); f; f = f->GetNext())
		{
			f->SetFilterData(filter);
		}
	}
	m_team = val;
	m_teammask = mask;
}

b2Fixture* PhysicsObject::GetObject()
{
	return m_object;
}

RectangleF& PhysicsObject::GetObjectArea()
{
	//if(m_body!=0 && m_object!=0)
	//{
	//	b2Vec2 pos = m_body->GetPosition();
	//	b2PolygonShape* shape = (b2PolygonShape*)m_object->GetShape();
	//
	//	/*float dx = abs(shape->m_vertices[2].x - shape->m_vertices[0].x)/2.0f * PIXM;
	//	float dy = abs(shape->m_vertices[2].y - shape->m_vertices[0].y)/2.0f * PIXM;*/

	//	/*m_location.left = pos.x * PIXM - hx * PIXM;
	//	m_location.right = pos.x  * PIXM + hx * PIXM;
	//	m_location.top = pos.y * PIXM - hy * PIXM;
	//	m_location.bottom = pos.y * PIXM + hy * PIXM;*/
	//}
	if(m_body!=0 && m_object!=0)
	{
		b2Vec2 pos = m_body->GetPosition();
		float hx = (m_location.right - m_location.left) / 2.0f;
		float hy = (m_location.bottom - m_location.top) / 2.0f;
		m_location.left = pos.x * PIXM - hx;
		m_location.right = pos.x  * PIXM + hx;
		m_location.top = pos.y * PIXM - hy;
		m_location.bottom = pos.y * PIXM + hy;
	}
	return m_location;
}

void PhysicsObject::SetObjectArea(const RectangleF& val)
{
	if(m_body!=0 && m_object!=0)
	{
		m_location = val;
		b2FixtureDef def;
		def.density = m_object->GetDensity();
		def.filter = m_object->GetFilterData();
		def.friction = m_object->GetFriction();
		def.isSensor = m_object->IsSensor();
		def.restitution = m_object->GetRestitution();

		b2Fixture* fix = m_body->GetFixtureList();
		while(fix)
		{
			b2Fixture* tmpfix = fix;
			fix = fix->GetNext();
			m_body->DestroyFixture(tmpfix);
		}

		m_object = 0;

		float hx = (val.right - val.left) / 2.0f;
		float hy = (val.bottom - val.top) / 2.0f;
		hx /= PIXM;
		hy /= PIXM;

		if(shapetype == 0)
		{
			b2PolygonShape shape;
			shape.SetAsBox(hx, hy, b2Vec2(0,0), 0);
			def.shape = &shape;
			b2Vec2 pos((val.left / PIXM) + hx, (val.top / PIXM) + hy);
			m_object = m_body->CreateFixture(&def);
			m_body->SetTransform(pos, m_body->GetAngle());

		}
		else if(shapetype == 2 && points.size() > 2)
		{
			b2PolygonShape shape;
			vector<b2Vec2> pts;
			for(unsigned int i = 0; i<points.size(); i++)
			{
				b2Vec2 pt;
				pt.x = points[i].left * hx * 2 * PIXM - hx * PIXM;
				pt.y = points[i].top * hy * 2 * PIXM - hy * PIXM;
				pt.x /= PIXM;
				pt.y /= PIXM;
				pts.push_back(pt);
			}

			b2Polygon poly(&pts[0], pts.size());
			b2Polygon polys[50];

			int count = DecomposeConvex(&poly, polys, 100);

			for(int i=0; i<count; i++)
			{
				b2Vec2* vecs = polys[i].GetVertexVecs();
				shape.Set(vecs, polys[i].nVertices);
				def.shape = &shape;
				delete[] vecs;
				m_object = m_body->CreateFixture(&def);
			}

			//b2FixtureDef* deleteMe = DecomposeConvexAndAddTo(&poly, m_body, &def);
			//delete[] deleteMe;

			//shape.Set(&pts[0], pts.size());
			//def.shape = &shape;
			b2Vec2 pos((val.left / PIXM) + hx, (val.top / PIXM) + hy);
			//m_object = m_body->CreateFixture(&def);
			//m_object = m_body->CreateFixture(&def);
			m_object = m_body->GetFixtureList();
			m_body->SetTransform(pos, m_body->GetAngle());
		}
		else if(shapetype == 3 && points.size() > 1)
		{
			b2ChainShape shape;
			vector<b2Vec2> pts;
			for(unsigned int i = 0; i<points.size(); i++)
			{
				b2Vec2 pt;
				pt.x = points[i].left * hx * 2 * PIXM - hx * PIXM;
				pt.y = points[i].top * hy * 2 * PIXM - hy * PIXM;
				pt.x /= PIXM;
				pt.y /= PIXM;
				pts.push_back(pt);
			}

			shape.CreateChain(&pts[0], pts.size());
			def.shape = &shape;
			b2Vec2 pos((val.left / PIXM) + hx, (val.top / PIXM) + hy);
			m_object = m_body->CreateFixture(&def);
			m_body->SetTransform(pos, m_body->GetAngle());
		}
		else if(shapetype == 4 && points.size() > 1)
		{
			b2ChainShape shape;
			vector<b2Vec2> pts;
			for(unsigned int i = 0; i<points.size(); i++)
			{
				b2Vec2 pt;
				pt.x = points[i].left * hx * 2 * PIXM - hx * PIXM;
				pt.y = points[i].top * hy * 2 * PIXM - hy * PIXM;
				pt.x /= PIXM;
				pt.y /= PIXM;
				pts.push_back(pt);
			}

			shape.CreateLoop(&pts[0], pts.size());
			def.shape = &shape;
			b2Vec2 pos((val.left / PIXM) + hx, (val.top / PIXM) + hy);
			m_object = m_body->CreateFixture(&def);
			m_body->SetTransform(pos, m_body->GetAngle());
		}
		else
		{
			b2CircleShape shape;
			shape.m_radius = hx;
			shape.m_p = b2Vec2(0, 0);
			def.shape = &shape;
			b2Vec2 pos((val.left / PIXM) + hx, (val.top / PIXM) + hy);
			m_object = m_body->CreateFixture(&def);
			m_body->SetTransform(pos, m_body->GetAngle());
		}		
	}
}

void PhysicsObject::MouseDown(unsigned short button, unsigned short buttons, PointI const& pt)
{
	PhyObject::MouseDown(button, buttons, pt);
	if(editing)
	{
		if(button == KEY_RBUTTON)
		{
			editing = false;
			shapetype = 3;
			SetObjectArea(m_location);
		}
		else
		{
			RectangleF pos;
			float hx = (m_location.right - m_location.left);
			float hy = (m_location.bottom - m_location.top);
			pos.left = (float)pt.left;
			pos.top = (float)pt.top;
			
			RectangleF rel = ToRelativePos(pos);
			points.push_back(PointF(rel.left / hx, rel.top / hy));
			if(points.size()>1) shapetype = 3;
			SetObjectArea(m_location);
		}
	}
}

void PhysicsObject::KeyDown(unsigned int key)
{
	PhyObject::KeyDown(key);
	if(!IsEditing()) return;
	if(key == KEY_E)
	{
		points.clear();
		editing = true;
	}
}

void PhysicsObject::Update(float delta)
{
	PhyObject::Update(delta);
}

void PhysicsObject::SetObjectTransform(const RectangleF& val)
{
	if(m_body!=0)
	{
		float hx = (val.right - val.left) / 2.0f;
		float hy = (val.bottom - val.top) / 2.0f;
		hx /= PIXM;
		hy /= PIXM;

		b2Vec2 pos((val.left / PIXM) + hx, (val.top / PIXM) + hy);
		m_body->SetTransform(pos, m_body->GetAngle());
	}
}

void PhysicsObject::SetObjectTransform(const PointF& val)
{
	if(m_body!=0)
	{
		b2Vec2 pos(val.left / PIXM, val.top / PIXM);
		m_body->SetTransform(pos, m_body->GetAngle());
	}
}

PointF PhysicsObject::GetObjectTransform()
{
	if(m_body!=0)
	{
		b2Vec2 ts = m_body->GetPosition();
		return PointF(ts.x  * PIXM, ts.y  * PIXM);
	}

	return PointF(0, 0);
}

void PhysicsObject::SetObjectAngle(float val)
{
	if(m_body!=0)
	{
		m_body->SetTransform(m_body->GetPosition(), val);
	}
}

float PhysicsObject::GetObjectAngle()
{
	if(m_body!=0)
	{
		return m_body->GetAngle();
	}
	return 0;
}

void PhysicsObject::SetGravityScale(float val)
{
	if(m_body!=0)
	{
		m_body->SetGravityScale(val);
	}
}

void PhysicsObject::SetAngularDamping(float val)
{
	if(m_body!=0)
	{
		m_body->SetAngularDamping(val);
	}
}

void PhysicsObject::SetLinearDamping(float val)
{
	if(m_body!=0)
	{
		m_body->SetLinearDamping(val);
	}
}

float PhysicsObject::GetGravityScale() const
{
	if(m_body!=0)
	{
		return m_body->GetGravityScale();
	}
	return 1.0f;
}

float PhysicsObject::GetAngularDamping() const
{
	if(m_body!=0)
	{
		return m_body->GetAngularDamping();
	}
	return 0;
}

float PhysicsObject::GetLinearDamping() const
{
	if(m_body!=0)
	{
		return m_body->GetLinearDamping();
	}
	return 0;
}

void PhysicsObject::LockRotation(bool val)
{
	if(m_body!=0)
	{
		m_body->SetFixedRotation(val);
	}
}

bool PhysicsObject::isFixedRotation() const
{
	if(m_body!=0)
	{
		return m_body->IsFixedRotation();
	}
	return false;
}

void PhysicsObject::SetDensity(float val)
{
	if(m_object!=0)
	{
		for(b2Fixture* f = m_body->GetFixtureList(); f; f = f->GetNext())
		{
			f->SetDensity(val);
		}
		m_body->ResetMassData();
	}
}

float PhysicsObject::GetDensity() const
{
	if(m_object!=0)
	{
		return m_object->GetDensity();
	}

	return 1.0f;
}

void PhysicsObject::SetBodyShape(int val)
{
	if(!(shapetype==val))
	{
		shapetype = val;
		SetObjectArea(m_location);
	}
}

int PhysicsObject::GetBodyShape() const
{
	return shapetype;
}

void PhysicsObject::SetRestitution(float val)
{
	if(m_object!=0)
	{
		for(b2Fixture* f = m_body->GetFixtureList(); f; f = f->GetNext())
		{
			f->SetRestitution(val);
		}
	}
}

void PhysicsObject::SetFriction(float val)
{
	if(m_object!=0)
	{
		for(b2Fixture* f = m_body->GetFixtureList(); f; f = f->GetNext())
		{
			f->SetFriction(val);
		}
	}
}

float PhysicsObject::GetRestitution() const
{
	if(m_object!=0)
	{
		return m_object->GetRestitution();
	}

	return 0;
}

float PhysicsObject::GetFriction() const
{
	if(m_object!=0)
	{
		return m_object->GetFriction();
	}

	return 0.8f;
}

void PhysicsObject::PhyStep(float delta)
{
	PhyObject::PhyStep(delta);
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
		SetObjectTransform(RectangleF(pt.left, pt.top, pt.left + width, pt.top + height));
	}
	if(m_animtype&2)
	{
		SetObjectAngle(m_rspos + (m_repos - m_rspos) * m_cpos);
	}

	if(m_current>=m_delay)
	{
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

	if(m_flags&OBJECT_Flag_ChangeByVelocity)
	{
		b2Vec2 vel = m_body->GetLinearVelocity();
		vel.Normalize();
		float ang = acos(vel.x);
		if(vel.y < 0) ang = C_PI + (C_PI - ang);
		SetObjectAngle(ang);
	}
}

Object* PhysicsObject::Clone()
{
	PhysicsObject* obj = new PhysicsObject(m_world, 50, 50, Team0_Category, Team0_Mask, objtype, sensor);
	*obj = *this;
	return obj;
}

Object* PhysicsObject::BuilderClone()
{
	PhysicsObject* obj = new PhysicsObject(m_world, false);
	*obj = *this;
	return obj;
}

void PhysicsObject::BeginContact (b2Contact *contact, PhyObject* objb)
{
	PhyObject::BeginContact(contact, objb);
	if(objb->GetType()&OBJECT_TYPE_PHYSICSOBJ)
		if(m_parent) m_parent->Callback(this, OBJECT_Message_Collision, ((PhysicsObject*)objb)->m_objtype, 0);
}

void PhysicsObject::EndContact (b2Contact *contact, PhyObject* objb)
{
	PhyObject::EndContact(contact, objb);
	if(objb->GetType()&OBJECT_TYPE_PHYSICSOBJ)
		if(m_parent) m_parent->Callback(this, OBJECT_Message_EndContact, ((PhysicsObject*)objb)->m_objtype, 0);
}

void PhysicsObject::Reset()
{
	if(m_playtype==1) m_cpos = 1.0f;
	else m_cpos = 0.0f;
	m_current = 0;
}

bool PhysicsObject::TestPoint(const PointI& pt)
{
	if(m_body)
	{
		for(b2Fixture* f = m_body->GetFixtureList(); f; f = f->GetNext())
		{
			if(f->GetShape()->GetType() == b2Shape::e_chain)
			{
				for(int i =0; i<f->GetShape()->GetChildCount(); i++)
				{
					b2AABB aabb = f->GetAABB(i);
					RectangleF rect(aabb.lowerBound.x * PIXM, aabb.lowerBound.y * PIXM, aabb.upperBound.x * PIXM, aabb.upperBound.y * PIXM);
					if(PointI::Intersect(pt, GetAbsolutePos(&rect))) return true;
				}
			}
			else
			{
				b2AABB aabb = f->GetAABB(0);
				RectangleF rect(aabb.lowerBound.x * PIXM, aabb.lowerBound.y * PIXM, aabb.upperBound.x * PIXM, aabb.upperBound.y * PIXM);
				if(PointI::Intersect(pt, GetAbsolutePos(&rect))) return true;
			}
		}		
		return false;
	}
	else
	{
		return PhyObject::TestPoint(pt);
	}
}

const PhysicsObject&  PhysicsObject::operator=(const PhysicsObject& val)
{
	DrawableObject::operator=(val);
	m_current = val.m_current;
	m_cpos = val.m_cpos;
	m_scale = val.m_scale;
	points = val.points;
	
	SetTeam(val.m_team, val.m_teammask);
	SetSensor(val.sensor);
	SetBodyType(val.objtype);
	SetBodyShape(val.shapetype);
	
	SetObjectArea(m_location);

	SetRestitution(val.GetRestitution());
	SetFriction(val.GetFriction());
	SetDensity(val.GetDensity());
	SetGravityScale(val.GetGravityScale());
	SetLinearDamping(val.GetLinearDamping());
	SetAngularDamping(val.GetAngularDamping());
	SetVelocity(val.GetVelocity());
	SetAngularVelocity(val.GetAngularVelocity());
	LockRotation(val.isFixedRotation());

	m_delay = val.m_delay;
	m_speed = val.m_speed;
	m_spos = val.m_spos;
	m_epos = val.m_epos;
	m_rspos = val.m_rspos;
	m_repos = val.m_repos;
	m_animtype = val.m_animtype;
	m_playtype = val.m_playtype;
	m_pingpong = val.m_pingpong;

	return *this;
}

void PhysicsObject::SetAngularVelocity(float val)
{
	if(m_body!=0)
	{
		m_body->SetAngularVelocity(val);
	}
}

float PhysicsObject::GetAngularVelocity() const
{
	if(m_body!=0)
	{
		return m_body->GetAngularVelocity();
	}
	return 0.0f;
}

void PhysicsObject::Save( BaseIO& str )
{
	PhyObject::Save(str);
	str.Write(m_delay, 0, "name=Delay;help=start delay.;category=PhysicsAnimation");
	str.Write(m_speed, 0, "name=Speed;help=Speed.");
	str.Write(m_spos.left, 0, "name=Start X;help=start X.");
	str.Write(m_spos.top, 0, "name=Start Y;help=start Y.");
	str.Write(m_epos.left, 0, "name=End X;help=End X.");
	str.Write(m_epos.top, 0, "name=End Y;help=End Y.");
	str.Write(m_animtype, 0, "name=Animation Type;help=Animation type 1 - position, 2 - rotation;type=flags;list=position,rotation");
	str.Write(m_playtype, 0, "name=Play type;help=Play type 0 - forward, 1 - backward, 2 - loop.;type=list;list=forward,backward,loop");
	str.Write(m_pingpong, 0, "name=Pingpong;help=Pingpong.");
	str.Write(m_rspos, 0, "name=Rotation start;help=Rotation start.");
	str.Write(m_repos, 0, "name=Rotation end;help=Rotation end.");
	str.Write(objtype, 0, "name=Body Type.;type=list;list=Dynamic,Static,Kinematic;category=Physics Object");
	str.Write(sensor, 0, "name=Sensor.");
	str.Write(shapetype, 0, "name=Object Shape.;type=list;list=Box,Circle,ShapeData,Chain,ChainLoop");
	str.Write(GetRestitution(), 0, "name=Restitution.");
	str.Write(GetFriction(), 0, "name=Friction.");
	str.Write(GetDensity(), 0, "name=Density.");
	str.Write(GetGravityScale(), 0, "name=GravityScale.");
	str.Write(GetLinearDamping(), 0, "name=LinearDamping.");
	str.Write(GetAngularDamping(), 0, "name=AngularDamping.");
	str.Write(GetVelocity().left, 0, "name=VelocityX.");
	str.Write(GetVelocity().top, 0, "name=VelocityY.");
	str.Write(GetAngularVelocity(), 0, "name=AngularVelocity.");
	str.Write(isFixedRotation(), 0, "name=Fixed Rotation.");

	string st;
	stringstream s;
	for(unsigned int i = 0; i<points.size(); i++)
	{
		s << points[i].left;
		s << ",";
		s << points[i].top;
		if(i!=points.size()-1) s << ";";
	}
	st = s.str();
	str.Write(st, 0, "name=Shape data");
}

void PhysicsObject::Load( BaseIO& str )
{
	PhyObject::Load(str);
	str.Read(m_delay);
	str.Read(m_speed);
	str.Read(m_spos.left);
	str.Read(m_spos.top);
	str.Read(m_epos.left);
	str.Read(m_epos.top);
	str.Read(m_animtype);
	str.Read(m_playtype);
	str.Read(m_pingpong);
	str.Read(m_rspos);
	str.Read(m_repos);
	str.Read(objtype);
	str.Read(sensor);
	str.Read(shapetype);
	float tmpd = 0;
	str.Read(tmpd);
	SetRestitution(tmpd);
	tmpd = 0;
	str.Read(tmpd);
	SetFriction(tmpd);

	tmpd = 0;
	str.Read(tmpd);
	SetDensity(tmpd);

	tmpd = 0;
	str.Read(tmpd);
	SetGravityScale(tmpd);

	tmpd = 0;
	str.Read(tmpd);
	SetLinearDamping(tmpd);

	tmpd = 0;
	str.Read(tmpd);
	SetAngularDamping(tmpd);

	float x = 0, y=0;
	str.Read(x);
	str.Read(y);
	SetVelocity(PointF(x, y));

	tmpd = 0;
	str.Read(tmpd);
	SetAngularVelocity(tmpd);

	bool valb = false;
	str.Read(valb);
	LockRotation(valb);

	string st;
	str.Read(st);
	points.clear();
	vector<string> s;
	GetStringArray(s, st.c_str(), ';');
	for(unsigned int i=0; i<s.size(); i++)
	{
		vector<string> s2;
		GetStringArray(s2, s[i].c_str(), ',');
		if(s2.size()>=2)
		{
			PointF pt;
			pt.left = (float)atof(s2[0].c_str());
			pt.top = (float)atof(s2[1].c_str());
			points.push_back(pt);
		}
	}

	SetObjectArea(m_location);
	SetBodyType(objtype);
	SetSensor(sensor);
	Reset();
}
#endif

#endif