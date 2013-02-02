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

template<class type>
class AbstractSmartPtr
{
protected:
	type* m_ptr;
	unsigned int* m_ref;

public:
	C_Mutex* mutex;

	bool IsValid()
	{
		if(m_ptr==0) return false;
		return true;
	}

	bool Lock()
	{
		if(mutex!=0) return mutex->Lock();
		return true;
	}

	void Unlock()
	{
		if(mutex!=0) mutex->Unlock();
	}

	unsigned int GetRefCount()
	{
		if(m_ref!=0) return (*m_ref);
		return 0;
	}

	AbstractSmartPtr():m_ptr(0)
	{
		m_ref = new unsigned int(0);
		mutex = 0;
		AddRef();
	}

	AbstractSmartPtr(type* ptr)
	{
		m_ref = new unsigned int(0);
		m_ptr = ptr;
		mutex = 0;
		AddRef();
	}

	AbstractSmartPtr(const AbstractSmartPtr<type>& ptr)
	{
		this->m_ptr = ptr.m_ptr;
		this->m_ref = ptr.m_ref;
		this->mutex = ptr.mutex;
		++(*m_ref);
	}

	AbstractSmartPtr<type>& operator=(const AbstractSmartPtr<type>& ptr)
	{
		if(&ptr != this)
		{
			Release();
			this->m_ptr = ptr.m_ptr;
			this->m_ref = ptr.m_ref;
			this->mutex = ptr.mutex;
			++(*m_ref);
		}
		return *this;
	}

	void AddRef()
	{
		++(*m_ref);
	}

	void Release()
	{
		--(*m_ref);
		if((*m_ref) == 0)
		{
			if(m_ptr != 0)
			{
				m_ptr->Release();
				delete m_ptr;
				m_ptr = 0;
			}

			delete m_ref;
			m_ref = 0;
		}
	}

	template<class type2>
	type2* GetPtr ()
	{
		return reinterpret_cast<type2*>(m_ptr);
	}

	type* operator-> ()
	{
		return m_ptr;
	}

	operator type*()
	{
		return m_ptr;
	}

	~AbstractSmartPtr()
	{
		Release();
	}
};

template<class type>
class SmartPtr
{
protected:
	type* m_ptr;
	unsigned int* m_ref;

public:
	C_Mutex* mutex;

	bool IsValid()
	{
		if(m_ptr==0) return false;
		return true;
	}

	bool Lock()
	{
		if(mutex!=0) return mutex->Lock();
		return true;
	}

	void Unlock()
	{
		if(mutex!=0) mutex->Unlock();
	}

	unsigned int GetRefCount()
	{
		if(m_ref!=0) return (*m_ref);
		return 0;
	}

	SmartPtr():m_ptr(0)
	{
		m_ref = new unsigned int(0);
		mutex = 0;
		AddRef();
	}

	SmartPtr(type* ptr)
	{
		m_ref = new unsigned int(0);
		m_ptr = ptr;
		mutex = 0;
		AddRef();
	}

	SmartPtr(const SmartPtr<type>& ptr)
	{
		this->m_ptr = ptr.m_ptr;
		this->m_ref = ptr.m_ref;
		this->mutex = ptr.mutex;
		++(*m_ref);
	}

	SmartPtr<type>& operator=(const SmartPtr<type>& ptr)
	{
		if(&ptr != this)
		{
			Release();
			this->m_ptr = ptr.m_ptr;
			this->m_ref = ptr.m_ref;
			this->mutex = ptr.mutex;
			++(*m_ref);
		}
		return *this;
	}

	void AddRef()
	{
		++(*m_ref);
	}

	void Release()
	{
		--(*m_ref);
		if((*m_ref) == 0)
		{
			if(m_ptr != 0)
			{
				delete m_ptr;
				m_ptr = 0;
			}

			delete m_ref;
			m_ref = 0;
		}
	}	

	operator type()
	{
		return *m_ptr;
	}

	type operator* ()
	{
		return *m_ptr;
	}

	type* operator-> ()
	{
		return m_ptr;
	}

	~SmartPtr()
	{
		Release();
	}
};

template<class type>
class TmpPtr
{
private:
	type* m_ptr;
	C_Mutex* mutex;

public:
	TmpPtr(type* ptr, C_Mutex* mt)
	{
		m_ptr = ptr;
		mutex = mt;
		mutex->Lock();
	}

	type* operator-> ()
	{
		return m_ptr;
	}

	~TmpPtr()
	{
		mutex->Unlock();
	}

};

template<class type>
class TSafeObject
{
private:
	type obj;
	C_Mutex mutex;

public:

	TSafeObject(){};
	TSafeObject(type obj2)
	{
		obj = obj2;
	}

	bool GetObject(type& obj2)
	{
		bool tmp = false;
		RES_LOCK
		{
			obj2 = obj;
			tmp = true;
		}
		RES_UNLOCK;
		return tmp;
	}

	TSafeObject(const TSafeObject<type>& obj2)
	{
		RES_LOCK
		{
			obj = static_cast<type>(obj2.obj);
		}
		RES_UNLOCK;
	}

	const TSafeObject<type>& operator= (const TSafeObject<type>& obj2)
	{
		RES_LOCK
		{
			obj = static_cast<type>(obj2.obj);
		}
		RES_UNLOCK;
		return *this;
	}

	const TSafeObject<type>& operator= (type& obj2)
	{
		RES_LOCK
		{
			obj = obj2;
		}
		RES_UNLOCK;
		return *this;
	}
};