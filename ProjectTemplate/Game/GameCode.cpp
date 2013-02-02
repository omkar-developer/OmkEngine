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

#include "GameCode.h"

GameAPI::GameAPI(wchar_t const* title, int winwidth, int winheight, bool fullscreen) : GameWorld(title, winwidth, winheight, fullscreen)
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

//GameWorld* GetGameAPI(wchar_t* title, int winwidth, int winheight, bool fullscreen)
//{
//	return new GameAPI(title, winwidth, winheight, fullscreen);
//}

void DisplayMoreTexts(UIWString& txt, unsigned int id)
{

}

void DisplayMoreProgress(float& progress, unsigned int id)
{

}

void RegisterConstants(stringstream& sstr)
{
	
}