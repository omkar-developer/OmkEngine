#include "stdafx.h"

class GameAPI : public GameWorld
{
private:

public:
	GameAPI(const wchar_t* title, int winwidth, int winheight, bool fullscreen = false);

	void Initialize();
	void RegisterObjects(ObjectBuilder* v_builder);
	void Release();
	void MainCallback(Object* obj, unsigned int Msg, unsigned int Param1, void* Param2);

#ifdef USESCRIPTING
	void RegisterScriptObjects(Script* engine);
#endif
};
