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

#define RES_LOCK if(mutex.Lock())
#define RES_UNLOCK mutex.Unlock()

#define RES_PLOCK if(mutex!=0) if(mutex->Lock())
#define RES_PUNLOCK if(mutex!=0) mutex->Unlock()

#define RES_LOCK_FAST if(mutex.Lock(true))

#define RES_PLOCK_FAST if(mutex!=0) if(mutex->Lock(true))

class C_Mutex
{
private:
#ifdef THREADSAFE
	HANDLE mutex;
#endif

public:
	C_Mutex()
	{
#ifdef THREADSAFE
		mutex = 0;
		mutex = CreateMutex(NULL, FALSE, NULL);
#endif
	}

	bool Lock(bool immediate = false)
	{
#ifdef THREADSAFE
		DWORD ret = WaitForSingleObject(mutex, immediate ? 0 : INFINITE);
		if(ret==WAIT_OBJECT_0 || ret==WAIT_ABANDONED)
		{
			return true;
		}
		return false;
#else
		return true;
#endif
	}

	C_Mutex(const C_Mutex& val)
	{
#ifdef THREADSAFE
		mutex = 0;
		mutex = CreateMutex(NULL, FALSE, NULL);
#endif
	}

	const C_Mutex& operator=(const C_Mutex& val)
	{
#ifdef THREADSAFE
		/*if(mutex != 0) CloseHandle(mutex);
		mutex 
		mutex = CreateMutex(NULL, FALSE, NULL);*/
#endif
		return *this;
	}

	void Unlock()
	{
#ifdef THREADSAFE
		ReleaseMutex(mutex);
#endif
	}

	~C_Mutex()
	{
#ifdef THREADSAFE
		if(mutex!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(mutex);
			mutex = INVALID_HANDLE_VALUE;
		}
#endif
	}
};