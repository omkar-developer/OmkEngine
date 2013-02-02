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

void* tproc(void* tptr)
{
	C_Thread* th = static_cast<C_Thread*>(tptr);
	th->Proc(th);
	return 0;
}

pthread_t C_Thread::GetID()
{
	return thread;
}

void C_Thread::Start()
{
	//not implimented
}

void C_Thread::Pause()
{
	//not implimented
}

bool C_Thread::Terminate()
{
	pthread_kill(thread, SIGKILL);
	return true;
}

bool C_Thread::Wait()
{
	pthread_join(thread, 0);
	return true;
}

C_Thread::C_Thread()
{
	pthread_create(&thread, 0, tproc, this);
}

C_Thread::~C_Thread()
{
	Terminate();
	//not implimented
}