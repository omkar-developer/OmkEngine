#include "GameCode.h"

GameAPI::GameAPI(const wchar_t* title, int winwidth, int winheight, bool fullscreen) : GameWorld(title, winwidth, winheight, fullscreen)
{
	
}

void GameAPI::Initialize()
{
	//Game->GetPhysicsWorld()->GetWorld().SetGravity(b2Vec2(0, 0.5f));
	GetMainContainer()->Callback.RegisterSingleHandler<GameAPI, &GameAPI::MainCallback>(this);
	srand(time(NULL));
}

void GameAPI::RegisterObjects(ObjectBuilder* v_builder)
{
	/*v_builder->RegisterBuilder("Sloid Object", new SolidObject(Game->GetPhysicsWorld(), true));
	v_builder->RegisterBuilder("Sloid StsticObject", new SolidObject(Game->GetPhysicsWorld(), false));
	v_builder->RegisterBuilder("Parachute", new Parachute(Game->GetPhysicsWorld(), true));*/
}

void GameAPI::Release()
{
	
}

void GameAPI::MainCallback(Object* obj, unsigned int Msg, unsigned int Param1, void* Param2)
{
	
}

#ifdef USESCRIPTING

void GameAPI::RegisterScriptObjects(Script* engine)
{
	//engine->GetEngine()->RegisterGlobalFunction("void CreateAsteroid(uint objid, uint type, float yoffset = 32, int xoffset = 0, int xmax = 750, float width = 64, float height = 32)", asFUNCTION(CreateAsteroid), asCALL_CDECL);
	//engine->GetEngine()->RegisterObjectMethod("Object", "uint GetObjectType()", asFUNCTION(GetObjectType2), asCALL_CDECL_OBJLAST);
}

#endif

GameWorld* GetGameAPI(const wchar_t* title, int winwidth, int winheight, bool fullscreen)
{
	return new GameAPI(title, winwidth, winheight, fullscreen);
}

void DisplayMoreTexts(UIWString& txt, unsigned int id)
{

}

void DisplayMoreProgress(float& progress, unsigned int id)
{

}

void RegisterConstants(stringstream& sstr)
{
	
}