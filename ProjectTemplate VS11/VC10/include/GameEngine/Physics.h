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

#ifndef PIXM
#define PIXM 40.0f
#endif

#define EPS 0.001f

/** \addtogroup PhysicsObjectTypes
* \{
*/
#define OBJECT_TYPE_PHYOBJECT	0x200000		///<Abstract physics object type.
/// \}

#define Team0_Category 0x1
#define Team1_Category 0x2
#define Team2_Category 0x4
#define Team3_Category 0x8
#define Team4_Category 0x10
//
#define Team0_Mask 0xFFFF
#define Team1_Mask Team1_Category | Team0_Category | Team2_Category | Team4_Category
#define Team2_Mask Team2_Category | Team0_Category | Team1_Category | Team3_Category
#define Team3_Mask Team0_Category | Team2_Category | Team4_Category
#define Team4_Mask Team0_Category | Team1_Category | Team3_Category

class PhyWorld;

class PhyObject : public DrawableObject
{
private:
	bool m_validit;
	list<PhyObject*>::iterator it;

protected:
	PhyWorld* m_world;
	b2Body* m_body;

public:
	PhyObject(PhyWorld* world);
	virtual ~PhyObject();

	b2Body* GetBody();

	virtual void QueueForDelete();

	virtual RectangleF& GetObjectArea();
	virtual void SetObjectArea(const RectangleF& val);

	virtual float GetAngle();
	virtual void SetAngle(float val);

	virtual void Process( unsigned int Msg, unsigned int Param1, void* Param2 );

	virtual void Update( float delta );
	virtual void PhyStep( float delta ){};

	virtual void PhyPreStep(){};

	virtual Object* Clone() = 0;

	virtual void BeginContact (b2Contact *contact, PhyObject* objb){};
	virtual void EndContact (b2Contact *contact, PhyObject* objb){}; 
	virtual void PreSolve (b2Contact *contact, const b2Manifold *oldManifold, PhyObject* objb){};
	virtual void PostSolve (b2Contact *contact, const b2ContactImpulse *impulse, PhyObject* objb){};

	friend class PhyWorld;
};

class PhyJoint : public DrawableObject
{
private:

protected:
	PhyWorld* m_world;
	b2Joint* m_joint;
	ObjectConnection obj1;
	ObjectConnection obj2;

	int m_jointtype;
	float m_refangle;
	bool m_collidecon;
	bool m_enablelimit;
	float m_limit1;
	float m_limit2;

	bool m_enablemotor;
	float m_motortorque;
	float m_motorspeed;

	bool m_editing;

	b2Vec2 m_b1anchor;
	b2Vec2 m_b2anchor;
	b2Vec2 m_jointaxis;
	b2Vec2 m_jointaxis2;

	float m_frequency;
	float m_dampingratio;

public:
	PhyJoint(PhyWorld* world);
	PhyJoint(PhyWorld* m_world, int type);
	virtual ~PhyJoint();

	virtual const PhyJoint& operator=(const PhyJoint& val);

	b2Joint* GetJoint();

	virtual RectangleF& GetObjectArea();
	virtual void SetObjectArea(const RectangleF& val);
	virtual void PostLoadAll();

	virtual void MouseDown(unsigned short button, unsigned short buttons, PointI const& pt);
	virtual void KeyDown(unsigned int key);

	virtual void Update( float delta );

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual void UpdateJoint();

	void Disconnected(DrawableObject* obj);

	virtual Object* Clone();

	void SetLimits(float limit1, float limit2);
	void EnableLimits(bool val);
	void EnableMotor(bool val);
	void SetMotorSpeed(bool val);
	void SetMotorPower(bool val);
	void SetDampingRatio(float val);
	void SetFrequency(float val);

	bool IsLimitEnabled();
	bool IsMotorEnabled();
	float GetLimit1();
	float GetLimit2();
	float GetMotorPower();
	float GetMootorSpeed();
	float GetDampingRatio();
	float GetFrequency();

	friend class PhyWorld;
};

class PhysicsObject : public PhyObject
{
private:
	float m_current;
	float m_cpos;
	float m_scale;
	bool editing;
	vector<PointF> points;

protected:
	b2Fixture* m_object;
	short m_team;
	short m_teammask;
	bool sensor;
	int objtype;
	int shapetype;

public:
	float m_delay;
	float m_speed;
	PointF m_spos;
	PointF m_epos;
	float m_rspos;
	float m_repos;
	int m_animtype;
	int m_playtype;
	bool m_pingpong;

	PhysicsObject(PhyWorld* m_world, bool solid);
	PhysicsObject(PhyWorld* m_world, float hx, float hy, short team, short mask, int objtype, bool sensor = false);

	virtual const PhysicsObject& operator=(const PhysicsObject& val);
	
	void Reset();

	float GetHx() const;
	float GetHy() const;

	short GetTeam() const;
	short GetTeamMask() const;
	virtual void SetTeam(short val, short mask);

	void SetSensor(bool val);
	bool GetSensor() const;
	void SetBodyType(int val);
	int GetBodyType() const;

	void SetVelocity(const PointF& vel);
	PointF GetVelocity() const;
	void ApplyForce(const PointF& vel);
	void ApplyImpulse(const PointF& vel);
	void ApplyTorque(float val);
	void ApplyAngImpulse(float vel);

	void SetGravityScale(float val);
	void SetAngularDamping(float val);
	void SetLinearDamping(float val);

	void SetRestitution(float val);
	void SetFriction(float val);
	float GetRestitution() const;
	float GetFriction() const;

	float GetGravityScale() const;
	float GetAngularDamping() const;
	float GetLinearDamping() const;

	void SetDensity(float val);
	float GetDensity() const;

	void SetBodyShape(int val);
	int GetBodyShape() const;

	void LockRotation(bool val);
	bool isFixedRotation() const;

	void SetAngularVelocity(float val);
	float GetAngularVelocity() const;

	virtual b2Fixture* GetObject();
	virtual RectangleF& GetObjectArea();
	virtual void SetObjectArea(const RectangleF& val);
	virtual void SetObjectTransform(const RectangleF& val);
	virtual void SetObjectTransform(const PointF& val);
	virtual void SetObjectAngle(float val);
	virtual float GetObjectAngle();
	virtual PointF GetObjectTransform();

	virtual bool TestPoint(const PointI& pt);

	virtual void Update(float delta);
	virtual void PhyStep(float delta);

	virtual void Save( BaseIO& str );
	virtual void Load( BaseIO& str );

	virtual void BeginContact (b2Contact *contact, PhyObject* objb);
	virtual void EndContact (b2Contact *contact, PhyObject* objb);
	virtual Object* Clone();
	virtual Object* BuilderClone();

	virtual void MouseDown(unsigned short button, unsigned short buttons, PointI const& pt);
	virtual void KeyDown(unsigned int key);
};

//Collision callback
class CollisionHandler : public b2ContactListener
{
protected:
	void BeginContact (b2Contact *contact);
	void EndContact (b2Contact *contact);
	void PreSolve (b2Contact *contact, const b2Manifold *oldManifold);
	void PostSolve (b2Contact *contact, const b2ContactImpulse *impulse);
};

//debug draw
class DebugDraw : public b2Draw
{
private:
	C_Renderer* m_rnd;
	
public:
	DebugDraw(C_Renderer* renderer = 0);

	void DrawPolygon (const b2Vec2 *vertices, int32 vertexCount, const b2Color &color);
	void DrawSolidPolygon (const b2Vec2 *vertices, int32 vertexCount, const b2Color &color);
	void DrawCircle (const b2Vec2 &center, float32 radius, const b2Color &color);
	void DrawSolidCircle (const b2Vec2 &center, float32 radius, const b2Vec2 &axis, const b2Color &color);
	void DrawSegment (const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color);
	void DrawTransform (const b2Transform &xf);

	void SetRenderer(C_Renderer* renderer);
	C_Renderer* GetRenderer();
};

//World
class PhyWorld
{
private:
	float m_prev;
	bool m_update;
	CollisionHandler m_colhandler;

protected:
	b2World m_world;	
	float m_frequency;
	int m_viterations;
	int m_piterations;
	list<PhyObject*> m_updatelist;
	set<PhyObject*> m_DeleteList;

public:
	PhyWorld(const b2Vec2& gravity = b2Vec2(0.0f, 0.0f));

	b2World& GetWorld();

	bool GetUpdate();
	void SetUpdate(bool val = true);

	float GetFrequency();
	void SetFrequency(float val);

	int GetVelocityIterations();
	void SetVelocityIterations(int val);

	int GetPositionIterations();
	void SetPositionIterations(int val);
	
	void Update(float delta);

	void AddToDeleteQueue(PhyObject* val);
	void ProcessDeleteQueue();

	void AddToUpdateList(PhyObject* val);
	void DeleteFromUpdateList(PhyObject* val);
	void ClearUpdateList();
	void ClearAllObjects();

	void ChangeContactListener(b2ContactListener* callback);
	void RestoreContactListener();

	void SetDebugDraw(b2Draw* val);

	void DrawDebug();
};