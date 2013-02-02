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

// for 0 args
class BaseSignal
{
public:
	virtual void DisconnectSlot(void* ptr){};
};

class BaseSlot
{
public:
	vector<BaseSignal*> Signals;

public:
	virtual ~BaseSlot();
	virtual void DisconnectSignal(BaseSignal* ptr);
};

template<class cls, class ret, ret (cls::*fn)()>
class Invoker0
{
public:
	static ret Invoke(void* ptr)
	{
		return (static_cast<cls*>(ptr)->*fn)();
	}
};

template<class ret, ret (*fn)()>
class Invoker0g
{
public:
	static ret Invoke(void* ptr)
	{
		return (*fn)();
	}
};

template<class ret = void>
class BaseSignal0
{
public:
	typedef ret (*inv) (void*);
	inv f0;
	void* p;
	BaseSlot* s;
	BaseSignal* parent;

public:
	BaseSignal0()
	{
		p = 0;
		f0 = 0;
		s = 0;
		parent = 0;
	}

	template<class cls, ret (cls::*fn)()>
	void Connect(cls* ptr)
	{
		f0 = &Invoker0<cls, ret, fn>::Invoke;
		p = ptr;
	}

	template<ret (*fn)()>
	void Connect()
	{
		f0 = &Invoker0g<ret, fn>::Invoke;
		p = 0;
	}

	bool Check()
	{
		if(f0=0) return false;
		return true;
	}

	ret Invoke()
	{
		return f0(p);
	}

	ret operator() ()
	{
		return f0(p);
	}

	bool operator==(BaseSignal0<ret>& val)
	{
		if(val.f0 == f0 && val.p == p) return true;
		return false;
	}

	void SetSlot(BaseSlot* val)
	{
		this->s = val;
		if(s != 0) s->Signals.push_back(parent);
	}

	BaseSignal0& operator=(BaseSignal0 const& val)
	{
		this->f0 = val.f0;
		this->p = val.p;
		this->parent = val.parent;
		SetSlot(val.s);
		return *this;
	}

	BaseSignal0(BaseSignal0 const& val)
	{
		this->f0 = val.f0;
		this->p = val.p;
		this->parent = val.parent;
		SetSlot(val.s);
	}

	virtual ~BaseSignal0()
	{
		if(s!=0) s->DisconnectSignal(parent);
	}
};

class Signal0 : public BaseSignal
{
protected:
	vector<BaseSignal0< > > events;
	unsigned int i;
	unsigned int sz;

public:
	template<class cls, void (cls::*fn)()>
	unsigned int RegisterSingleHandler(cls* obj)
	{
		BaseSignal0<> bs;
		bs.Connect<cls, fn>(obj);
		events.push_back(bs);
		return events.size() - 1;
	}

	template<class cls, void (cls::*fn)()>
	unsigned int RegisterHandler(cls* obj)
	{
		BaseSignal0<> bs;
		bs.Connect<cls, fn>(obj);
		bs.parent = this;
		bs.SetSlot(dynamic_cast<BaseSlot*>(obj));
		events.push_back(bs);
		return events.size() - 1;
	}

	template<void (*fn)()>
	unsigned int RegisterHandler()
	{
		BaseSignal0<> bs;
		bs.Connect<fn>();
		events.push_back(bs);
		return events.size() - 1;
	}

	template<class cls, void (cls::*fn)()>
	void DisconnectHandler(cls* obj)
	{
		BaseSignal0<> bs;
		bs.Connect<cls, fn>(obj);
		sz = events.size();
		for(i = 0; i<sz; i++)
		{
			if(events[i]==bs)
			{
				DisconnectHandler(i);
				return;
			}
		}
	}

	template<void (*fn)()>
	void DisconnectHandler()
	{
		BaseSignal0<> bs;
		bs.Connect<fn>();
		sz = events.size();
		for(i = 0; i<sz; i++)
		{
			if(events[i]==bs)
			{
				DisconnectHandler(i);
				return;
			}
		}
	}

	void DisconnectHandler(unsigned int del)
	{
		if(del<events.size())
		{
			events.erase(events.begin()+del);
		}
	}

	void Invoke()
	{
		sz = events.size();
		for(i = 0; i<sz; i++)
		{
			events[i].Invoke();
		}		
	}

	void operator() ()
	{
		sz = events.size();
		for(i = 0; i<sz; i++)
		{
			events[i].Invoke();
		}
	}

	void DisconnectAll()
	{
		events.clear();
	}

	void DisconnectSlot(void* obj)
	{
		for(vector<BaseSignal0< > >::iterator i = events.begin(); i!=events.end(); i++)
		{
			if(i->s == obj)
			{
				i = events.erase(i);
			}
			if(i==events.end()) break;
		}
	}
};

// for 1 args
template<class cls, class ret, class pt0, ret (cls::*fn)(pt0)>
class Invoker1
{
public:
	static ret Invoke(void* ptr, pt0 p0)
	{
		return (static_cast<cls*>(ptr)->*fn)(p0);
	}
};

template<class ret, class pt0, ret (*fn)(pt0)>
class Invoker1g
{
public:
	static ret Invoke(void* ptr, pt0 p0)
	{
		return (*fn)(p0);
	}
};

template<class pt0, class ret = void>
class BaseSignal1
{
public:
	typedef ret (*inv) (void*, pt0);
	inv f0;
	void* p;

	BaseSlot* s;
	BaseSignal* parent;

	BaseSignal1()
	{
		p = 0;
		f0 = 0;
		s = 0;
		parent = 0;
	}

	template<class cls, ret (cls::*fn)(pt0)>
	void Connect(cls* ptr)
	{
		f0 = &Invoker1<cls, ret, pt0, fn>::Invoke;
		p = ptr;
	}

	template<ret (*fn)(pt0)>
	void Connect()
	{
		f0 = &Invoker1g<ret, pt0, fn>::Invoke;
		p = 0;
	}

	bool Check()
	{
		if(f0=0) return false;
		return true;
	}

	ret Invoke(pt0 p0)
	{
		return f0(p, p0);
	}

	ret operator() (pt0 p0)
	{
		return f0(p, p0);
	}

	bool operator==(BaseSignal1<pt0,ret>& val)
	{
		if(val.f0 == f0 && val.p == p) return true;
		return false;
	}

	void SetSlot(BaseSlot* val)
	{
		this->s = val;
		if(s != 0) s->Signals.push_back(parent);
	}

	BaseSignal1& operator=(BaseSignal1 const& val)
	{
		this->f0 = val.f0;
		this->p = val.p;
		this->parent = val.parent;
		SetSlot(val.s);
		return *this;
	}

	BaseSignal1(BaseSignal1 const& val)
	{
		this->f0 = val.f0;
		this->p = val.p;
		this->parent = val.parent;
		SetSlot(val.s);
	}

	virtual ~BaseSignal1()
	{
		if(s!=0) s->DisconnectSignal(parent);
	}
};

template<class pt0>
class Signal1 : public BaseSignal
{
protected:
	vector<BaseSignal1<pt0> > events;
	unsigned int i;
	unsigned int sz;

public:
	template<class cls, void (cls::*fn)(pt0)>
	unsigned int RegisterSingleHandler(cls* obj)
	{
		BaseSignal1<pt0> bs;
		bs.template Connect<cls, fn>(obj);
		events.push_back(bs);
		return events.size() - 1;
	}

	template<class cls, void (cls::*fn)(pt0)>
	unsigned int RegisterHandler(cls* obj)
	{
		BaseSignal1<pt0> bs;
		bs.template Connect<cls, fn>(obj);
		bs.parent = this;
		bs.SetSlot(dynamic_cast<BaseSlot*>(obj));
		events.push_back(bs);
		return events.size() - 1;
	}

	template<void (*fn)(pt0)>
	unsigned int RegisterHandler()
	{
		BaseSignal1<pt0> bs;
		bs.template Connect<fn>();
		events.push_back(bs);
		return events.size() - 1;
	}

	void DisconnectHandler(unsigned int del)
	{
		if(del<events.size())
		events.erase(events.begin()+del);
	}

	template<class cls, void (cls::*fn)(pt0)>
	void DisconnectHandler(cls* obj)
	{
		BaseSignal1<pt0> bs;
		bs.template Connect<cls, fn>(obj);
		sz = events.size();
		for(i = 0; i<sz; i++)
		{
			if(events[i]==bs)
			{
				DisconnectHandler(i);
				return;
			}
		}
	}

	template<void (*fn)(pt0)>
	void DisconnectHandler()
	{
		BaseSignal1<pt0> bs;
		bs.template Connect<fn>();
		sz = events.size();
		for(i = 0; i<sz; i++)
		{
			if(events[i]==bs)
			{
				DisconnectHandler(i);
				return;
			}
		}
	}

	void Invoke(pt0 p0)
	{
		sz = events.size();
		for(i = 0; i<sz; i++)
		{
			events[i].Invoke(p0);
		}		
	}

	void operator() (pt0 p0)
	{
		sz = events.size();
		for(i = 0; i<sz; i++)
		{
			events[i].Invoke(p0);
		}
	}

	void DisconnectAll()
	{
		events.clear();
	}

	void DisconnectSlot(void* obj)
	{
		for(typename vector<BaseSignal1<pt0> >::iterator i = events.begin(); i!=events.end(); i++)
		{
			if(i->s == obj)
			{
				i = events.erase(i);
			}
			if(i==events.end()) break;
		}
	}
};

// for 2 params
template<class cls, class ret, class pt0, class pt1, ret (cls::*fn)(pt0, pt1)>
class Invoker2
{
public:
	static ret Invoke(void* ptr, pt0 p0, pt1 p1)
	{
		return (static_cast<cls*>(ptr)->*fn)(p0, p1);
	}
};

template<class ret, class pt0, class pt1, ret (*fn)(pt0, pt1)>
class Invoker2g
{
public:
	static ret Invoke(void* ptr, pt0 p0, pt1 p1)
	{
		return (*fn)(p0, p1);
	}
};

template<class pt0, class pt1, class ret = void>
class BaseSignal2
{
public:
	typedef ret (*inv) (void*, pt0, pt1);
	inv f0;
	void* p;

	BaseSlot* s;
	BaseSignal* parent;

	BaseSignal2()
	{
		p = 0;
		f0 = 0;
		s = 0;
		parent = 0;
	}

	template<class cls, ret (cls::*fn)(pt0, pt1)>
	void Connect(cls* ptr)
	{
		f0 = &Invoker2<cls, ret, pt0, pt1, fn>::Invoke;
		p = ptr;
	}

	template<ret (*fn)(pt0, pt1)>
	void Connect()
	{
		f0 = &Invoker2g<ret, pt0, pt1, fn>::Invoke;
		p = 0;
	}

	bool Check()
	{
		if(f0=0) return false;
		return true;
	}

	ret Invoke(pt0 p0, pt1 p1)
	{
		return f0(p, p0, p1);
	}

	ret operator() (pt0 p0, pt1 p1)
	{
		return f0(p, p0, p1);
	}

	bool operator==(BaseSignal2<pt0, pt1, ret>& val)
	{
		if(val.f0 == f0 && val.p == p) return true;
		return false;
	}

	void SetSlot(BaseSlot* val)
	{
		this->s = val;
		if(s != 0) s->Signals.push_back(parent);
	}

	BaseSignal2& operator=(BaseSignal2 const& val)
	{
		this->f0 = val.f0;
		this->p = val.p;
		this->parent = val.parent;
		SetSlot(val.s);
		return *this;
	}

	BaseSignal2(BaseSignal2 const& val)
	{
		this->f0 = val.f0;
		this->p = val.p;
		this->parent = val.parent;
		SetSlot(val.s);
	}

	virtual ~BaseSignal2()
	{
		if(s!=0) s->DisconnectSignal(parent);
	}
};

template<class pt0, class pt1>
class Signal2 : public BaseSignal
{
protected:
	vector<BaseSignal2<pt0, pt1> > events;
	unsigned int i;
	unsigned int sz;

public:
	template<class cls, void (cls::*fn)(pt0, pt1)>
	unsigned int RegisterSingleHandler(cls* obj)
	{
		BaseSignal2<pt0, pt1> bs;
		bs.template Connect<cls, fn>(obj);
		events.push_back(bs);
		return events.size() - 1;
	}

	template<class cls, void (cls::*fn)(pt0, pt1)>
	unsigned int RegisterHandler(cls* obj)
	{
		BaseSignal2<pt0, pt1> bs;
		bs.template Connect<cls, fn>(obj);
		bs.parent = this;
		bs.SetSlot(dynamic_cast<BaseSlot*>(obj));
		events.push_back(bs);
		return events.size() - 1;
	}

	template<void (*fn)(pt0, pt1)>
	unsigned int RegisterHandler()
	{
		BaseSignal2<pt0, pt1> bs;
		bs.template Connect<fn>();
		events.push_back(bs);
		return events.size() - 1;
	}

	void DisconnectHandler(unsigned int del)
	{
		if(del<events.size())
		events.erase(events.begin()+del);
	}

	template<class cls, void (cls::*fn)(pt0, pt1)>
	void DisconnectHandler(cls* obj)
	{
		BaseSignal2<pt0, pt1> bs;
		bs.template Connect<cls, fn>(obj);
		sz = events.size();
		for(i = 0; i<sz; i++)
		{
			if(events[i]==bs)
			{
				DisconnectHandler(i);
				return;
			}
		}
	}

	template<void (*fn)(pt0, pt1)>
	void DisconnectHandler()
	{
		BaseSignal2<pt0, pt1> bs;
		bs.template Connect<fn>();
		sz = events.size();
		for(i = 0; i<sz; i++)
		{
			if(events[i]==bs)
			{
				DisconnectHandler(i);
				return;
			}
		}
	}

	void Invoke(pt0 p0, pt1 p1)
	{
		sz = events.size();
		for(i = 0; i<sz; i++)
		{
			events[i].Invoke(p0, p1);
		}		
	}

	void operator() (pt0 p0, pt1 p1)
	{
		sz = events.size();
		for(i = 0; i<sz; i++)
		{
			events[i].Invoke(p0,p1);
		}
	}

	void DisconnectAll()
	{
		events.clear();
	}

	void DisconnectSlot(void* obj)
	{
		for(typename vector<BaseSignal2<pt0, pt1> >::iterator i = events.begin(); i!=events.end(); i++)
		{
			if(i->s == obj)
			{
				i = events.erase(i);
			}
			if(i==events.end()) break;
		}
	}
};

// for 3 params
template<class cls, class ret, class pt0, class pt1, class pt2, ret (cls::*fn)(pt0, pt1, pt2)>
class Invoker3
{
public:
	static ret Invoke(void* ptr, pt0 p0, pt1 p1, pt2 p2)
	{
		return (static_cast<cls*>(ptr)->*fn)(p0, p1, p2);
	}
};

template<class ret, class pt0, class pt1, class pt2, ret (*fn)(pt0, pt1, pt2)>
class Invoker3g
{
public:
	static ret Invoke(void* ptr, pt0 p0, pt1 p1, pt2 p2)
	{
		return (*fn)(p0, p1, p2);
	}
};

template<class pt0, class pt1, class pt2, class ret = void>
class BaseSignal3
{
public:
	typedef ret (*inv) (void*, pt0, pt1, pt2);
	inv f0;
	void* p;

	BaseSlot* s;
	BaseSignal* parent;

	BaseSignal3()
	{
		p = 0;
		f0 = 0;
		s = 0;
		parent = 0;
	}

	template<class cls, ret (cls::*fn)(pt0, pt1, pt2)>
	void Connect(cls* ptr)
	{
		f0 = &Invoker3<cls, ret, pt0, pt1, pt2, fn>::Invoke;
		p = ptr;
	}

	template<ret (*fn)(pt0, pt1, pt2)>
	void Connect()
	{
		f0 = &Invoker3g<ret, pt0, pt1, pt2, fn>::Invoke;
		p = 0;
	}

	bool Check()
	{
		if(f0=0) return false;
		return true;
	}

	ret Invoke(pt0 p0, pt1 p1, pt2 p2)
	{
		return f0(p, p0, p1, p2);
	}

	ret operator() (pt0 p0, pt1 p1, pt2 p2)
	{
		return f0(p, p0, p1, p2);
	}

	bool operator==(BaseSignal3<pt0, pt1, pt2, ret>& val)
	{
		if(val.f0 == f0 && val.p == p) return true;
		return false;
	}

	void SetSlot(BaseSlot* val)
	{
		this->s = val;
		if(s != 0) s->Signals.push_back(parent);
	}

	BaseSignal3& operator=(BaseSignal3 const& val)
	{
		this->f0 = val.f0;
		this->p = val.p;
		this->parent = val.parent;
		SetSlot(val.s);
		return *this;
	}

	BaseSignal3(BaseSignal3 const& val)
	{
		this->f0 = val.f0;
		this->p = val.p;
		this->parent = val.parent;
		SetSlot(val.s);
	}

	virtual ~BaseSignal3()
	{
		if(s!=0) s->DisconnectSignal(parent);
	}
};

template<class pt0, class pt1, class pt2>
class Signal3 : public BaseSignal
{
protected:
	vector<BaseSignal3<pt0, pt1, pt2> > events;
	unsigned int i;
	unsigned int sz;

public:
	template<class cls, void (cls::*fn)(pt0, pt1, pt2)>
	unsigned int RegisterSingleHandler(cls* obj)
	{
		BaseSignal3<pt0, pt1, pt2> bs;
		bs.template Connect<cls, fn>(obj);
		events.push_back(bs);
		return events.size() - 1;
	}

	template<class cls, void (cls::*fn)(pt0, pt1, pt2)>
	unsigned int RegisterHandler(cls* obj)
	{
		BaseSignal3<pt0, pt1, pt2> bs;
		bs.template Connect<cls, fn>(obj);
		bs.parent = this;
		bs.SetSlot(dynamic_cast<BaseSlot*>(obj));
		events.push_back(bs);
		return events.size() - 1;
	}

	template<void (*fn)(pt0, pt1, pt2)>
	unsigned int RegisterHandler()
	{
		BaseSignal3<pt0, pt1, pt2> bs;
		bs.template Connect<fn>();
		events.push_back(bs);
		return events.size() - 1;
	}

	void DisconnectHandler(unsigned int del)
	{
		if(del<events.size())
		events.erase(events.begin()+del);
	}

	template<class cls, void (cls::*fn)(pt0, pt1, pt2)>
	void DisconnectHandler(cls* obj)
	{
		BaseSignal3<pt0, pt1, pt2> bs;
		bs.template Connect<cls, fn>(obj);
		sz = events.size();
		for(i = 0; i<sz; i++)
		{
			if(events[i]==bs)
			{
				DisconnectHandler(i);
				return;
			}
		}
	}

	template<void (*fn)(pt0, pt1, pt2)>
	void DisconnectHandler()
	{
		BaseSignal3<pt0, pt1, pt2> bs;
		bs.template Connect<fn>();
		sz = events.size();
		for(i = 0; i<sz; i++)
		{
			if(events[i]==bs)
			{
				DisconnectHandler(i);
				return;
			}
		}
	}

	void Invoke(pt0 p0, pt1 p1, pt2 p2)
	{
		sz = events.size();
		for(i = 0; i<sz; i++)
		{
			events[i].Invoke(p0, p1, p2);
		}		
	}

	void operator() (pt0 p0, pt1 p1, pt2 p2)
	{
		sz = events.size();
		for(i = 0; i<sz; i++)
		{
			events[i].Invoke(p0,p1,p2);
		}
	}

	void DisconnectAll()
	{
		events.clear();
	}

	void DisconnectSlot(void* obj)
	{
		for(typename vector<BaseSignal3<pt0,pt1,pt2> >::iterator i = events.begin(); i!=events.end(); i++)
		{
			if(i->s == obj)
			{
				i = events.erase(i);
			}
			if(i==events.end()) break;
		}
	}
};

// for 4 params
template<class cls, class ret, class pt0, class pt1, class pt2, class pt3, ret (cls::*fn)(pt0, pt1, pt2, pt3)>
class Invoker4
{
public:
	static ret Invoke(void* ptr, pt0 p0, pt1 p1, pt2 p2, pt3 p3)
	{
		return (static_cast<cls*>(ptr)->*fn)(p0, p1, p2, p3);
	}
};

template<class ret, class pt0, class pt1, class pt2, class pt3, ret (*fn)(pt0, pt1, pt2, pt3)>
class Invoker4g
{
public:
	static ret Invoke(void* ptr, pt0 p0, pt1 p1, pt2 p2, pt3 p3)
	{
		return (*fn)(p0, p1, p2, p3);
	}
};

template<class pt0, class pt1, class pt2, class pt3, class ret = void>
class BaseSignal4
{
public:
	typedef ret (*inv) (void*, pt0, pt1, pt2, pt3);
	inv f0;
	void* p;

	BaseSlot* s;
	BaseSignal* parent;

	BaseSignal4()
	{
		p = 0;
		f0 = 0;
		s = 0;
		parent = 0;
	}

	template<class cls, ret (cls::*fn)(pt0, pt1, pt2, pt3)>
	void Connect(cls* ptr)
	{
		f0 = &Invoker4<cls, ret, pt0, pt1, pt2, pt3, fn>::Invoke;
		p = ptr;
	}

	template<ret (*fn)(pt0, pt1, pt2, pt3)>
	void Connect()
	{
		f0 = &Invoker4g<ret, pt0, pt1, pt2, pt3, fn>::Invoke;
		p = 0;
	}

	bool Check()
	{
		if(f0=0) return false;
		return true;
	}

	ret Invoke(pt0 p0, pt1 p1, pt2 p2, pt3 p3)
	{
		return f0(p, p0, p1, p2, p3);
	}

	ret operator() (pt0 p0, pt1 p1, pt2 p2, pt3 p3)
	{
		return f0(p, p0, p1, p2, p3);
	}

	bool operator==(BaseSignal4<pt0, pt1, pt2, pt3, ret>& val)
	{
		if(val.f0 == f0 && val.p == p) return true;
		return false;
	}

	void SetSlot(BaseSlot* val)
	{
		this->s = val;
		if(s != 0) s->Signals.push_back(parent);
	}

	BaseSignal4& operator=(BaseSignal4 const& val)
	{
		this->f0 = val.f0;
		this->p = val.p;
		this->parent = val.parent;
		SetSlot(val.s);
		return *this;
	}

	BaseSignal4(BaseSignal4 const& val)
	{
		this->f0 = val.f0;
		this->p = val.p;
		this->parent = val.parent;
		SetSlot(val.s);
	}

	virtual ~BaseSignal4()
	{
		if(s!=0) s->DisconnectSignal(parent);
	}
};

template<class pt0, class pt1, class pt2, class pt3>
class Signal4 : public BaseSignal
{
protected:
	vector<BaseSignal4<pt0, pt1, pt2, pt3> > events;
	unsigned int i;
	unsigned int sz;

public:
	template<class cls, void (cls::*fn)(pt0, pt1, pt2, pt3)>
	unsigned int RegisterSingleHandler(cls* obj)
	{
		BaseSignal4<pt0, pt1, pt2, pt3> bs;
		bs.template Connect<cls, fn>(obj);
		events.push_back(bs);
		return events.size() - 1;
	}

	template<class cls, void (cls::*fn)(pt0, pt1, pt2, pt3)>
	unsigned int RegisterHandler(cls* obj)
	{
		BaseSignal4<pt0, pt1, pt2, pt3> bs;
		bs.template Connect<cls, fn>(obj);
		bs.parent = this;
		bs.SetSlot(dynamic_cast<BaseSlot*>(obj));
		events.push_back(bs);
		return events.size() - 1;
	}

	template<void (*fn)(pt0, pt1, pt2, pt3)>
	unsigned int RegisterHandler()
	{
		BaseSignal4<pt0, pt1, pt2, pt3> bs;
		bs.template Connect<fn>();
		events.push_back(bs);
		return events.size() - 1;
	}

	void DisconnectHandler(unsigned int del)
	{
		if(del<events.size())
		events.erase(events.begin()+del);
	}

	template<class cls, void (cls::*fn)(pt0, pt1, pt2, pt3)>
	void DisconnectHandler(cls* obj)
	{
		BaseSignal4<pt0, pt1, pt2, pt3> bs;
		bs.template Connect<cls, fn>(obj);
		sz = events.size();
		for(i = 0; i<sz; i++)
		{
			if(events[i]==bs)
			{
				DisconnectHandler(i);
				return;
			}
		}
	}

	template<void (*fn)(pt0, pt1, pt2, pt3)>
	void DisconnectHandler()
	{
		BaseSignal4<pt0, pt1, pt2, pt3> bs;
		bs.template Connect<fn>();
		sz = events.size();
		for(i = 0; i<sz; i++)
		{
			if(events[i]==bs)
			{
				DisconnectHandler(i);
				return;
			}
		}
	}

	void Invoke(pt0 p0, pt1 p1, pt2 p2, pt3 p3)
	{
		sz = events.size();
		for(i = 0; i<sz; i++)
		{
			events[i].Invoke(p0, p1, p2, p3);
		}		
	}

	void operator() (pt0 p0, pt1 p1, pt2 p2, pt3 p3)
	{
		sz = events.size();
		for(i = 0; i<sz; i++)
		{
			events[i].Invoke(p0, p1, p2, p3);
		}
	}

	void DisconnectAll()
	{
		events.clear();
	}

	void DisconnectSlot(void* obj)
	{
		for(typename vector<BaseSignal4<pt0,pt1,pt2,pt3> >::iterator i = events.begin(); i!=events.end(); i++)
		{
			if(i->s == obj)
			{
				i = events.erase(i);
			}
			if(i==events.end()) break;
		}
	}
};