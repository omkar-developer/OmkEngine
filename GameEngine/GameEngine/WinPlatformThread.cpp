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

DWORD WINAPI ThreadProc(LPVOID lp)
{
	C_Thread* th = static_cast<C_Thread*>(lp);
	th->Proc(th);
	return 0;
}

HANDLE C_Thread::GetHandle()
{
	return handle;
}

unsigned long C_Thread::GetID()
{
	return id;
}

void C_Thread::Start()
{
	ResumeThread(handle);
}

void C_Thread::Pause()
{
	SuspendThread(handle);
}

bool C_Thread::Terminate()
{
	if(TerminateThread(handle, 0)) return true;
	return false;
}

bool C_Thread::Wait()
{
	DWORD ret = WaitForSingleObject(handle, INFINITE);
	if(ret == WAIT_OBJECT_0)
	{
		return true;
	}
	return false;
}

C_Thread::C_Thread()
{
	id = 0;
	handle = INVALID_HANDLE_VALUE;
	handle = CreateThread(NULL, NULL, ThreadProc, this, CREATE_SUSPENDED, &id);
}

C_Thread::~C_Thread()
{
	CloseHandle(handle);
}