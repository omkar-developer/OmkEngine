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

/*
 * Mutex class for locking mechanisms of threading.(currently only usable in windows)
 */
class C_Mutex
{
private:
#ifdef THREADSAFE
	pthread_mutex_t mutex;
#endif

public:
	C_Mutex()
	{
#ifdef THREADSAFE
		/*pthread_mutex_init(&mutex, 0);*/
#endif
	}

	bool Lock(bool immediate = false)
	{
#ifdef THREADSAFE
		/*if(!immediate) return (pthread_mutex_lock(&mutex)==0);
		else return (pthread_mutex_trylock(&mutex)==0);*/
		return true;
#else
		return true;
#endif
	}

	/**
	 *
	 * \param val
	 */
	C_Mutex(const C_Mutex& val)
	{
#ifdef THREADSAFE
		//not implimented
#endif
	}

	const C_Mutex& operator=(const C_Mutex& val)
	{
#ifdef THREADSAFE
		//not implimented
#endif
		return *this;
	}

	void Unlock()
	{
#ifdef THREADSAFE
		/*pthread_mutex_unlock(&mutex);*/
#endif
	}

	~C_Mutex()
	{
#ifdef THREADSAFE
		/*Lock();
		Unlock();
		pthread_mutex_destroy(&mutex);*/
#endif
	}
};
