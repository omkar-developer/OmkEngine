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

#define AddConstant(val1, val2) val1 << "const uint " #val2 " = "; val1<<val2; val1<<";";

class ScriptRectangle
{
private:
	int ref;

public:
	float x, y, r, b;

	ScriptRectangle();
	void addref();
	void release();
};

class ScriptPoint
{
private:
	int ref;

public:
	float x, y;

	ScriptPoint();
	void addref();
	void release();
};

void RegisterConstants(stringstream& sstr);
int GetStoreInt(string str);
float GetStoreFloat(string str);
void SetStoreInt(string str, int val);
void SetStoreFloat(string str, float val);
void EraseStoreInt(string str);
void EraseStoreFloat(string str);
bool ExistsStoreInt(string str);
bool ExistsStoreFloat(string str);

void OpenURL(std::string str);

class ScriptObject
{
private:
	int ref;

public:
	Object* obj;

	ScriptObject* operator=(ScriptObject* o);

	ScriptObject();
	void addref();
	void release();

	//functions
	unsigned int GetID();
	void Show();
	void Hide();
	void Enable();
	void Disable();

	unsigned int GetObjectType();
	void SetObjectType(unsigned int val);
	string GetObjectName();
	void SetObjectName(string name);

	void SetProperty(string name, float val);
	float GetProperty(string name);
	void ClearProperties();

	void SetText(string text);
	string GetText();

	void SetValue(int val);
	int GetValue();

	void SetMax(float val);
	float GetMax();
	void SetMin(float val);
	float GetMin();

	void SetColor(unsigned int color);
	unsigned int GetColor();

	void SetRotation(float val);
	float GetRotation();

	int GetType();
	void QueueForDelete();
	void Draw();

	unsigned int GetFlags();
	void SetFlags(unsigned int val);

	ScriptRectangle* GetObjectArea();
	void SetObjectArea(ScriptRectangle* val);

	ScriptRectangle* GetAbsolutePosition();

	void SetLayer(float val);
	float GetLayer();

	void SetSprite(string name, int index, int animtype, bool paused);

	void SetCapture();
	bool GetCapture();
	void ReleaseCapture();

	void StartTimer();
	void PauseTimer();
	void StopTimer();

	void Play(int animtype);
	void Play1();
	void Pause();
	void Stop();

	void PlaySprite();
	void SetSpriteIndex(unsigned int ind);
	void MirrorSpriteX(bool val);
	void MirrorSpriteY(bool val);
	void PauseSprite();
	void StopSprite();

	void SetDelay(float delay);
	void SetSpeed(float speed);
	void SetAnimatiomType(int type);
	void SetPlayType(int type);
	void SetPingPong(bool val);
	void SetPosAnim(ScriptPoint* start, ScriptPoint* end);
	void SetRotationAnim(float start, float end);
	void SetColorAnim(unsigned int start, unsigned int end);
	void SetScaleAnim(float start, float end);

	float GetDelay();
	float GetSpeed();
	int GetAnimatiomType();
	int GetPlayType();
	bool GetPingPong();
	ScriptPoint* GetPosAnimStart();
	ScriptPoint* GetPosAnimEnd();
	float GetRotationAnimStart();
	float GetRotationAnimEnd();
	unsigned int GetColorAnimStart();
	unsigned int GetColorAnimEnd();
	float GetScaleAnimStart();
	float GetScaleAnimEnd();

	void EmitParticles1(ScriptPoint* pt);
	void EmitParticles2(float x, float y, float size = 1.0f);

	void AddEmitRect(ScriptRectangle* pt, float life = 1.0f);
	void AddEmitRect2(float x, float y, float life, float size = 1.0f);
	void ClearEmitRect();

	bool isPlaying();
	void SetSoundResource(string name);
	void SetVolume(int val);
	int GetVolume();

	//physics
#ifdef USEBOX2DPHYSICS
	short GetTeam();
	short GetTeamMask();
	void SetTeam(short val, short mask);

	void SetSensor(bool val);
	bool GetSensor();
	void SetBodyType(int val);
	int GetBodyType();

	void SetVelocity(ScriptPoint* vel);
	ScriptPoint* GetVelocity();
	void ApplyForce(ScriptPoint* vel);
	void ApplyImpulse(ScriptPoint* vel);
	void ApplyTorque(float val);
	void ApplyAngImpulse(float vel);

	void SetTransform(ScriptPoint* vel);
	ScriptPoint* GetTransform();

	void SetPhyAngle(float val);
	float GetPhyAngle();

	void SetDensity(float val);
	float GetDensity();

	void SetRestitution(float val);
	void SetFriction(float val);
	float GetRestitution();
	float GetFriction();

	void SetBodyShape(int val);
	int GetBodyShape();

	void SetGravityScale(float val);
	void SetAngularDamping(float val);
	void SetLinearDamping(float val);

	float GetGravityScale();
	float GetAngularDamping();
	float GetLinearDamping();
	void LockRotation(bool val);

	bool isFixedRotation();
	void SetAngularVelocity(float val);
	float GetAngularVelocity();

	//Joint
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
#endif
};

struct ScriptError
{
	int row, col;
	asEMsgType type;
	string message;
};

class BinaryStream : public asIBinaryStream
{
private:
	BaseIO* m_io;

public:
	BinaryStream(BaseIO* bio = 0);
	void Read(void *ptr, asUINT size);
	void Write(const void *ptr, asUINT size);
};

class Script
{
private:
	asIScriptEngine* engine;
	asIScriptContext* mainfn;
	asIScriptContext* callbackfn;
	ScriptObject sobj;
	asIScriptFunction* funid;
	asIScriptFunction* fid;

public:
	static PointI m_clkpoint;
	string m_curcode;
	map<int, ScriptError> errors;

	Script();

	void Callback (Object* obj, unsigned int Msg, unsigned int Param1, void* Param2);
	asIScriptEngine* GetEngine();

	string GetFunctionNames(set<string, icompstrs>& arr);
	string GetClassNames();
	void PopulateWithHelp(map<string, string>& m_help);

	asIScriptModule* GetModule();

	bool Open(const wchar_t* fname);
	bool Compile(const char* fname, bool execute);
	bool Execute();
	~Script();
};