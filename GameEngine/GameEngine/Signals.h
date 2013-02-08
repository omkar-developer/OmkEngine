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
/**
 * Base class for all signals.
 */
class BaseSignal
{
public:
	/**
	 * Derived class implements this to disconnect connected slots to this signal.
	 * \param ptr Pointer to the slot to disconnect.
	 */
	virtual void DisconnectSlot(void* ptr){};
};

/**
 *	All derived classes from this one are disconnected automatically on destruction of that class.
 */
class BaseSlot
{
public:
	vector<BaseSignal*> Signals; ///< Pointers to signals connected to this object.

public:
	virtual ~BaseSlot(); ///< Virtual destructor deletes all signals connected to this object.

	/**
	 * Disconnects specified signal and deletes it from this list if it exists.
	 * \param ptr Pointer to the signal to be disconnected.
	 */
	virtual void DisconnectSignal(BaseSignal* ptr);
};

/**
 * Used to invoke provided function from class.
 * \tparam ret Return data type.
 * \tparam cls Class name.
 * \tparam fn Function pointer.
 */
template<class cls, class ret, ret (cls::*fn)()>
class Invoker0
{
public:
	/**
	 * Invokes specified function.
	 * \param ptr Pointer to the class object used to invoke this function.
	 * \return Returns provided data type value.
	 */
	static ret Invoke(void* ptr)
	{
		return (static_cast<cls*>(ptr)->*fn)();
	}
};

/**
 * Used to invoke global function.
 * \tparam ret Return data type.
 * \tparam fn Function pointer.
 */
template<class ret, ret (*fn)()>
class Invoker0g
{
public:
	/**
	 * Invokes specified function with specified arguments.
	 * \param ptr Not used its just for signature matching purpose.
	 * \return Returns provided data type value.
	 */
	static ret Invoke(void* ptr)
	{
		return (*fn)();
	}
};

/**
 * Signal to dynamically manage class/global function pointers.
 * \tparam ret Return type of the functions which can be linked by this signal.
 */
template<class ret = void>
class BaseSignal0
{
public:
	typedef ret (*inv) (void*); ///< Function type.
	inv f0;  ///< Function pointer to invoke linked function..
	void* p; ///< Pointer to the class object used to invoke specified class function.
	BaseSlot* s; ///<< Pointer to the base slot connected to this signal.
	BaseSignal* parent; ///< Parent signal class object used for auto disconnection of slots.

public:
	/**
	 * Default constructor.
	 */
	BaseSignal0()
	{
		p = 0;
		f0 = 0;
		s = 0;
		parent = 0;
	}

	/**
	 * Connects specified class and class function to this signal.
	 * \param ptr Pointer to the class object which is used to invoke specified function.
	 * \tparam cls Class name.
	 * \tparam fn Pointer to the class function.
	 */
	template<class cls, ret (cls::*fn)()>
	void Connect(cls* ptr)
	{
		f0 = &Invoker0<cls, ret, fn>::Invoke;
		p = ptr;
	}

	/**
	 * Connects specified function with his signal.
	 * \tparam fn Pointer to the class function.
	 */
	template<ret (*fn)()>
	void Connect()
	{
		f0 = &Invoker0g<ret, fn>::Invoke;
		p = 0;
	}

	/**
	 * Checks whether function pointer is null.
	 * \return true if function pointer is not null otherwise false.
	 */
	bool Check()
	{
		if(f0==0) return false;
		return true;
	}

	/**
	 * Invokes function linked to this signal.
	 * \return Specified data type value.
	 */
	ret Invoke()
	{
		return f0(p);
	}

	/**
	 * Invokes function linked to this signal.
	 * \return Specified data type value.
	 */
	ret operator() ()
	{
		return f0(p);
	}

	/**
	 * Compares specified signal with this signal.
	 * \param val Pointer to the signal to compare with.
	 * \return true both are equal otherwise false.
	 */
	bool operator==(BaseSignal0<ret>& val)
	{
		if(val.f0 == f0 && val.p == p) return true;
		return false;
	}

	/**
	 * Sets slot and adds parent signal to slot connections list.
	 * \param val Pointer to the slot to connect.
	 */
	void SetSlot(BaseSlot* val)
	{
		this->s = val;
		if(s != 0) s->Signals.push_back(parent);
	}

	/**
	 * Copies specified signal to this one.
	 * \param val Pointer to the signal to copy.
	 * \return Reference to this signal.
	 */
	BaseSignal0& operator=(BaseSignal0 const& val)
	{
		this->f0 = val.f0;
		this->p = val.p;
		this->parent = val.parent;
		SetSlot(val.s);
		return *this;
	}

	/**
	 * Copies specified signal to this one.
	 * \param val Pointer to the signal to copy.
	 */
	BaseSignal0(BaseSignal0 const& val)
	{
		this->f0 = val.f0;
		this->p = val.p;
		this->parent = val.parent;
		SetSlot(val.s);
	}

	/**
	 * Virtual destructor disconnects all signals from linked slot.
	 */
	virtual ~BaseSignal0()
	{
		if(s!=0) s->DisconnectSignal(parent);
	}
};

/**
 * Signal used for signaling of events to the specified functions.
 */
class Signal0 : public BaseSignal
{
protected:
	vector<BaseSignal0< > > events; ///< Signals list.
	unsigned int i; ///< Used for iteration.
	unsigned int sz; ///< Used for iteration.

public:
	/**
	 * Connects specified not derived(inherited) class and class function to this signal.
	 * \param obj Pointer to the class object which is used to invoke specified function.
	 * \return Number of connections to this signal - 1.
	 * \tparam cls Class name.
	 * \tparam fn Pointer to the class function.
	 */
	template<class cls, void (cls::*fn)()>
	unsigned int RegisterSingleHandler(cls* obj)
	{
		BaseSignal0<> bs;
		bs.Connect<cls, fn>(obj);
		events.push_back(bs);
		return events.size() - 1;
	}

	/**
	 * Connects specified derived(inherited) class and class function to this signal.
	 * \param obj Pointer to the class object which is used to invoke specified function.
	 * \return Number of connections to this signal - 1.
	 * \tparam cls Class name.
	 * \tparam fn Pointer to the class function.
	 */
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

	/**
	 * Connects specified global function to this signal.
	 * \return Number of connections to this signal - 1.
	 * \tparam fn Pointer to the global function.
	 */
	template<void (*fn)()>
	unsigned int RegisterHandler()
	{
		BaseSignal0<> bs;
		bs.Connect<fn>();
		events.push_back(bs);
		return events.size() - 1;
	}

	/**
	 * Disconnects specified class and class function from this signal.
	 * \param obj Pointer to the class object which is to be disconnected.
	 * \tparam cls Class name.
	 * \tparam fn Pointer to the class function.
	 */
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

	/**
	 * Disconnects specified global function from this signal.
	 * \tparam fn Pointer to the global function.
	 */
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

	/**
	 * Erases specified event by its index.
	 * \param del Index of the base signal to delete.
	 */
	void DisconnectHandler(unsigned int del)
	{
		if(del<events.size())
		{
			events.erase(events.begin()+del);
		}
	}

	/**
	 * Invokes this signal and calls all function connected to it.
	 */
	void Invoke()
	{
		sz = events.size();
		for(i = 0; i<sz; i++)
		{
			events[i].Invoke();
		}		
	}

	/**
	 * Invokes this signal and calls all function connected to it.
	 */
	void operator() ()
	{
		sz = events.size();
		for(i = 0; i<sz; i++)
		{
			events[i].Invoke();
		}
	}

	/**
	 * Disconnects all base signals.
	 */
	void DisconnectAll()
	{
		events.clear();
	}

	/**
	 * Disconnects specified slot from this signal.
	 * \param obj Pointer to the slot to disconnect.
	 */
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
/**
 * Used to invoke provided function from class.
 * \tparam ret Return data type.
 * \tparam cls Class name.
 * \tparam pt0 Data type of the parameter 1.
 * \tparam fn Function pointer.
 */
template<class cls, class ret, class pt0, ret (cls::*fn)(pt0)>
class Invoker1
{
public:
	/**
	 * Invokes specified function.
	 * \param ptr Pointer to the class object used to invoke this function.
	 * \param p0 Value of the parameter 1.
	 * \return Returns provided data type value.
	 */
	static ret Invoke(void* ptr, pt0 p0)
	{
		return (static_cast<cls*>(ptr)->*fn)(p0);
	}
};

/**
 * Used to invoke global function.
 * \tparam ret Return data type.
 * \tparam fn Function pointer.
 * \tparam pt0 Data type of the parameter 1.
 */
template<class ret, class pt0, ret (*fn)(pt0)>
class Invoker1g
{
public:
	/**
	 * Invokes specified function with specified arguments.
	 * \param ptr Not used its just for signature matching purpose.
	 * \param p0 Value of the parameter 1.
	 * \return Returns provided data type value.
	 */
	static ret Invoke(void* ptr, pt0 p0)
	{
		return (*fn)(p0);
	}
};

/**
 * Signal to dynamically manage class/global function pointers.
 * \tparam pt0 Data type of the first parameter.
 * \tparam ret Return type of the functions which can be linked by this signal.
 */
template<class pt0, class ret = void>
class BaseSignal1
{
public:
	typedef ret (*inv) (void*, pt0); ///< Function type.
	inv f0; ///< Function pointer to invoke linked function.
	void* p; ///< Pointer to the class object used to invoke specified class function.

	BaseSlot* s; ///<< Pointer to the base slot connected to this signal.
	BaseSignal* parent; ///< Parent signal class object used for auto disconnection of slots.

	/**
	 * Default constructor.
	 */
	BaseSignal1()
	{
		p = 0;
		f0 = 0;
		s = 0;
		parent = 0;
	}

	/**
	 * Connects specified class and class function to this signal.
	 * \param ptr Pointer to the class object which is used to invoke specified function.
	 * \tparam cls Class name.
	 * \tparam fn Pointer to the class function.
	 */
	template<class cls, ret (cls::*fn)(pt0)>
	void Connect(cls* ptr)
	{
		f0 = &Invoker1<cls, ret, pt0, fn>::Invoke;
		p = ptr;
	}

	/**
	 * Connects specified function with his signal.
	 * \tparam fn Pointer to the class function.
	 */
	template<ret (*fn)(pt0)>
	void Connect()
	{
		f0 = &Invoker1g<ret, pt0, fn>::Invoke;
		p = 0;
	}

	/**
	 * Checks whether function pointer is null.
	 * \return true if function pointer is not null otherwise false.
	 */
	bool Check()
	{
		if(f0==0) return false;
		return true;
	}

	/**
	 * Invokes function linked to this signal.
	 * \param p0 Value of the parameter 1.
	 * \return Specified data type value.
	 */
	ret Invoke(pt0 p0)
	{
		return f0(p, p0);
	}

	/**
	 * Invokes function linked to this signal.
	 * \param p0 Value of the parameter 1.
	 * \return Specified data type value.
	 */
	ret operator() (pt0 p0)
	{
		return f0(p, p0);
	}

	/**
	 * Compares specified signal with this signal.
	 * \param val Pointer to the signal to compare with.
	 * \return true both are equal otherwise false.
	 */
	bool operator==(BaseSignal1<pt0,ret>& val)
	{
		if(val.f0 == f0 && val.p == p) return true;
		return false;
	}

	/**
	 * Sets slot and adds parent signal to slot connections list.
	 * \param val Pointer to the slot to connect.
	 */
	void SetSlot(BaseSlot* val)
	{
		this->s = val;
		if(s != 0) s->Signals.push_back(parent);
	}

	/**
	 * Copies specified signal to this one.
	 * \param val Pointer to the signal to copy.
	 * \return Reference to this signal.
	 */
	BaseSignal1& operator=(BaseSignal1 const& val)
	{
		this->f0 = val.f0;
		this->p = val.p;
		this->parent = val.parent;
		SetSlot(val.s);
		return *this;
	}

	/**
	 * Copies specified signal to this one.
	 * \param val Pointer to the signal to copy.
	 */
	BaseSignal1(BaseSignal1 const& val)
	{
		this->f0 = val.f0;
		this->p = val.p;
		this->parent = val.parent;
		SetSlot(val.s);
	}

	/**
	 * Virtual destructor disconnects all signals from linked slot.
	 */
	virtual ~BaseSignal1()
	{
		if(s!=0) s->DisconnectSignal(parent);
	}
};

/**
 * Signal used for signaling of events to the specified functions.
 * \tparam pt0 Data type of the parameter 1.
 */
template<class pt0>
class Signal1 : public BaseSignal
{
protected:
	vector<BaseSignal1<pt0> > events; ///< Signals list.
	unsigned int i; ///< Used for iteration.
	unsigned int sz; ///< Used for iteration.

public:
	/**
	 * Connects specified not derived(inherited) class and class function to this signal.
	 * \param obj Pointer to the class object which is used to invoke specified function.
	 * \return Number of connections to this signal - 1.
	 * \tparam cls Class name.
	 * \tparam fn Pointer to the class function.
	 */
	template<class cls, void (cls::*fn)(pt0)>
	unsigned int RegisterSingleHandler(cls* obj)
	{
		BaseSignal1<pt0> bs;
		bs.template Connect<cls, fn>(obj);
		events.push_back(bs);
		return events.size() - 1;
	}

	/**
	 * Connects specified derived(inherited) class and class function to this signal.
	 * \param obj Pointer to the class object which is used to invoke specified function.
	 * \return Number of connections to this signal - 1.
	 * \tparam cls Class name.
	 * \tparam fn Pointer to the class function.
	 */
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

	/**
	 * Connects specified global function to this signal.
	 * \return Number of connections to this signal - 1.
	 * \tparam fn Pointer to the global function.
	 */
	template<void (*fn)(pt0)>
	unsigned int RegisterHandler()
	{
		BaseSignal1<pt0> bs;
		bs.template Connect<fn>();
		events.push_back(bs);
		return events.size() - 1;
	}

	/**
	 * Erases specified event by its index.
	 * \param del Index of the base signal to delete.
	 */
	void DisconnectHandler(unsigned int del)
	{
		if(del<events.size())
		events.erase(events.begin()+del);
	}

	/**
	 * Disconnects specified class and class function from this signal.
	 * \param obj Pointer to the class object which is to be disconnected.
	 * \tparam cls Class name.
	 * \tparam fn Pointer to the class function.
	 */
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

	/**
	 * Disconnects specified global function from this signal.
	 * \tparam fn Pointer to the global function.
	 */
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

	/**
	 * Invokes this signal and calls all function connected to it.
	 * \param p0 Value of the parameter 1.
	 */
	void Invoke(pt0 p0)
	{
		sz = events.size();
		for(i = 0; i<sz; i++)
		{
			events[i].Invoke(p0);
		}		
	}

	/**
	 * Invokes this signal and calls all function connected to it.
	 * \param p0 Value of the parameter 1.
	 */
	void operator() (pt0 p0)
	{
		sz = events.size();
		for(i = 0; i<sz; i++)
		{
			events[i].Invoke(p0);
		}
	}

	/**
	 * Disconnects all base signals.
	 */
	void DisconnectAll()
	{
		events.clear();
	}


	/**
	 * Disconnects specified slot from this signal.
	 * \param obj Pointer to the slot to disconnect.
	 */
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

// for 2 args
/**
 * Used to invoke provided function from class.
 * \tparam ret Return data type.
 * \tparam cls Class name.
 * \tparam pt0 Data type of the parameter 1.
 * \tparam pt1 Data type of the parameter 2.
 * \tparam fn Function pointer.
 */
template<class cls, class ret, class pt0, class pt1, ret (cls::*fn)(pt0, pt1)>
class Invoker2
{
public:
	/**
	 * Invokes specified function.
	 * \param ptr Pointer to the class object used to invoke this function.
	 * \param p0 Value of the parameter 1.
	 * \param p1 Value of the parameter 2.
	 * \return Returns provided data type value.
	 */
	static ret Invoke(void* ptr, pt0 p0, pt1 p1)
	{
		return (static_cast<cls*>(ptr)->*fn)(p0, p1);
	}
};

/**
 * Used to invoke global function.
 * \tparam ret Return data type.
 * \tparam fn Function pointer.
 * \tparam pt0 Data type of the parameter 1.
 * \tparam pt1 Data type of the parameter 2.
 */
template<class ret, class pt0, class pt1, ret (*fn)(pt0, pt1)>
class Invoker2g
{
public:
	/**
	 * Invokes specified function with specified arguments.
	 * \param ptr Not used its just for signature matching purpose.
	 * \param p0 Value of the parameter 1.
	 * \param p1 Value of the parameter 2.
	 * \return Returns provided data type value.
	 */
	static ret Invoke(void* ptr, pt0 p0, pt1 p1)
	{
		return (*fn)(p0, p1);
	}
};

/**
 * Signal to dynamically manage class/global function pointers.
 * \tparam pt0 Data type of the first parameter.
 * \tparam ret Return type of the functions which can be linked by this signal.
 */
template<class pt0, class pt1, class ret = void>
class BaseSignal2
{
public:
	typedef ret (*inv) (void*, pt0, pt1); ///< Function type.
	inv f0; ///< Function pointer to invoke linked function.
	void* p; ///< Pointer to the class object used to invoke specified class function.

	BaseSlot* s; ///<< Pointer to the base slot connected to this signal.
	BaseSignal* parent; ///< Parent signal class object used for auto disconnection of slots.

	/**
	 * Default constructor.
	 */
	BaseSignal2()
	{
		p = 0;
		f0 = 0;
		s = 0;
		parent = 0;
	}

	/**
	 * Connects specified class and class function to this signal.
	 * \param ptr Pointer to the class object which is used to invoke specified function.
	 * \tparam cls Class name.
	 * \tparam fn Pointer to the class function.
	 */
	template<class cls, ret (cls::*fn)(pt0, pt1)>
	void Connect(cls* ptr)
	{
		f0 = &Invoker2<cls, ret, pt0, pt1, fn>::Invoke;
		p = ptr;
	}

	/**
	 * Connects specified function with his signal.
	 * \tparam fn Pointer to the class function.
	 */
	template<ret (*fn)(pt0, pt1)>
	void Connect()
	{
		f0 = &Invoker2g<ret, pt0, pt1, fn>::Invoke;
		p = 0;
	}

	/**
	 * Checks whether function pointer is null.
	 * \return true if function pointer is not null otherwise false.
	 */
	bool Check()
	{
		if(f0==0) return false;
		return true;
	}

	/**
	 * Invokes function linked to this signal.
	 * \param p0 Value of the parameter 1.
	 * \param p1 Value of the parameter 2.
	 * \return Specified data type value.
	 */
	ret Invoke(pt0 p0, pt1 p1)
	{
		return f0(p, p0, p1);
	}

	/**
	 * Invokes function linked to this signal.
	 * \param p0 Value of the parameter 1.
	 * \param p1 Value of the parameter 2.
	 * \return Specified data type value.
	 */
	ret operator() (pt0 p0, pt1 p1)
	{
		return f0(p, p0, p1);
	}

	/**
	 * Compares specified signal with this signal.
	 * \param val Pointer to the signal to compare with.
	 * \return true both are equal otherwise false.
	 */
	bool operator==(BaseSignal2<pt0, pt1,ret>& val)
	{
		if(val.f0 == f0 && val.p == p) return true;
		return false;
	}

	/**
	 * Sets slot and adds parent signal to slot connections list.
	 * \param val Pointer to the slot to connect.
	 */
	void SetSlot(BaseSlot* val)
	{
		this->s = val;
		if(s != 0) s->Signals.push_back(parent);
	}

	/**
	 * Copies specified signal to this one.
	 * \param val Pointer to the signal to copy.
	 * \return Reference to this signal.
	 */
	BaseSignal2& operator=(BaseSignal2 const& val)
	{
		this->f0 = val.f0;
		this->p = val.p;
		this->parent = val.parent;
		SetSlot(val.s);
		return *this;
	}

	/**
	 * Copies specified signal to this one.
	 * \param val Pointer to the signal to copy.
	 */
	BaseSignal2(BaseSignal2 const& val)
	{
		this->f0 = val.f0;
		this->p = val.p;
		this->parent = val.parent;
		SetSlot(val.s);
	}

	/**
	 * Virtual destructor disconnects all signals from linked slot.
	 */
	virtual ~BaseSignal2()
	{
		if(s!=0) s->DisconnectSignal(parent);
	}
};

/**
 * Signal used for signaling of events to the specified functions.
 * \tparam pt0 Data type of the parameter 1.
 * \tparam pt1 Data type of the parameter 2.
 */
template<class pt0, class pt1>
class Signal2 : public BaseSignal
{
protected:
	vector<BaseSignal2<pt0, pt1> > events; ///< Signals list.
	unsigned int i; ///< Used for iteration.
	unsigned int sz; ///< Used for iteration.

public:
	/**
	 * Connects specified not derived(inherited) class and class function to this signal.
	 * \param obj Pointer to the class object which is used to invoke specified function.
	 * \return Number of connections to this signal - 1.
	 * \tparam cls Class name.
	 * \tparam fn Pointer to the class function.
	 */
	template<class cls, void (cls::*fn)(pt0, pt1)>
	unsigned int RegisterSingleHandler(cls* obj)
	{
		BaseSignal2<pt0, pt1> bs;
		bs.template Connect<cls, fn>(obj);
		events.push_back(bs);
		return events.size() - 1;
	}

	/**
	 * Connects specified derived(inherited) class and class function to this signal.
	 * \param obj Pointer to the class object which is used to invoke specified function.
	 * \return Number of connections to this signal - 1.
	 * \tparam cls Class name.
	 * \tparam fn Pointer to the class function.
	 */
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

	/**
	 * Connects specified global function to this signal.
	 * \return Number of connections to this signal - 1.
	 * \tparam fn Pointer to the global function.
	 */
	template<void (*fn)(pt0, pt1)>
	unsigned int RegisterHandler()
	{
		BaseSignal2<pt0, pt1> bs;
		bs.template Connect<fn>();
		events.push_back(bs);
		return events.size() - 1;
	}

	/**
	 * Erases specified event by its index.
	 * \param del Index of the base signal to delete.
	 */
	void DisconnectHandler(unsigned int del)
	{
		if(del<events.size())
		events.erase(events.begin()+del);
	}

	/**
	 * Disconnects specified class and class function from this signal.
	 * \param obj Pointer to the class object which is to be disconnected.
	 * \tparam cls Class name.
	 * \tparam fn Pointer to the class function.
	 */
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

	/**
	 * Disconnects specified global function from this signal.
	 * \tparam fn Pointer to the global function.
	 */
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

	/**
	 * Invokes this signal and calls all function connected to it.
	 * \param p0 Value of the parameter 1.
	 * \param p1 Value of the parameter 2.
	 */
	void Invoke(pt0 p0, pt1 p1)
	{
		sz = events.size();
		for(i = 0; i<sz; i++)
		{
			events[i].Invoke(p0, p1);
		}		
	}

	/**
	 * Invokes this signal and calls all function connected to it.
	 * \param p0 Value of the parameter 1.
	 * \param p1 Value of the parameter 2.
	 */
	void operator() (pt0 p0, pt1 p1)
	{
		sz = events.size();
		for(i = 0; i<sz; i++)
		{
			events[i].Invoke(p0, p1);
		}
	}

	/**
	 * Disconnects all base signals.
	 */
	void DisconnectAll()
	{
		events.clear();
	}


	/**
	 * Disconnects specified slot from this signal.
	 * \param obj Pointer to the slot to disconnect.
	 */
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

// for 3 args
/**
 * Used to invoke provided function from class.
 * \tparam ret Return data type.
 * \tparam cls Class name.
 * \tparam pt0 Data type of the parameter 1.
 * \tparam pt1 Data type of the parameter 2.
 * \tparam pt2 Data type of the parameter 3.
 * \tparam fn Function pointer.
 */
template<class cls, class ret, class pt0, class pt1, class pt2, ret (cls::*fn)(pt0, pt1, pt2)>
class Invoker3
{
public:
	/**
	 * Invokes specified function.
	 * \param ptr Pointer to the class object used to invoke this function.
	 * \param p0 Value of the parameter 1.
	 * \param p1 Value of the parameter 2.
	 * \param p2 Value of the parameter 3.
	 * \return Returns provided data type value.
	 */
	static ret Invoke(void* ptr, pt0 p0, pt1 p1, pt2 p2)
	{
		return (static_cast<cls*>(ptr)->*fn)(p0, p1, p2);
	}
};

/**
 * Used to invoke global function.
 * \tparam ret Return data type.
 * \tparam fn Function pointer.
 * \tparam pt0 Data type of the parameter 1.
 * \tparam pt1 Data type of the parameter 2.
 * \tparam pt2 Data type of the parameter 3.
 */
template<class ret, class pt0, class pt1, class pt2, ret (*fn)(pt0, pt1, pt2)>
class Invoker3g
{
public:
	/**
	 * Invokes specified function with specified arguments.
	 * \param ptr Not used its just for signature matching purpose.
	 * \param p0 Value of the parameter 1.
	 * \param p1 Value of the parameter 2.
	 * \param p2 Value of the parameter 3.
	 * \return Returns provided data type value.
	 */
	static ret Invoke(void* ptr, pt0 p0, pt1 p1, pt2 p2)
	{
		return (*fn)(p0, p1, p2);
	}
};

/**
 * Signal to dynamically manage class/global function pointers.
 * \tparam pt0 Data type of the first parameter.
 * \tparam ret Return type of the functions which can be linked by this signal.
 */
template<class pt0, class pt1, class pt2, class ret = void>
class BaseSignal3
{
public:
	typedef ret (*inv) (void*, pt0, pt1, pt2); ///< Function type.
	inv f0; ///< Function pointer to invoke linked function.
	void* p; ///< Pointer to the class object used to invoke specified class function.

	BaseSlot* s; ///<< Pointer to the base slot connected to this signal.
	BaseSignal* parent; ///< Parent signal class object used for auto disconnection of slots.

	/**
	 * Default constructor.
	 */
	BaseSignal3()
	{
		p = 0;
		f0 = 0;
		s = 0;
		parent = 0;
	}

	/**
	 * Connects specified class and class function to this signal.
	 * \param ptr Pointer to the class object which is used to invoke specified function.
	 * \tparam cls Class name.
	 * \tparam fn Pointer to the class function.
	 */
	template<class cls, ret (cls::*fn)(pt0, pt1, pt2)>
	void Connect(cls* ptr)
	{
		f0 = &Invoker3<cls, ret, pt0, pt1, pt2, fn>::Invoke;
		p = ptr;
	}

	/**
	 * Connects specified function with his signal.
	 * \tparam fn Pointer to the class function.
	 */
	template<ret (*fn)(pt0, pt1, pt2)>
	void Connect()
	{
		f0 = &Invoker3g<ret, pt0, pt1, pt2, fn>::Invoke;
		p = 0;
	}

	/**
	 * Checks whether function pointer is null.
	 * \return true if function pointer is not null otherwise false.
	 */
	bool Check()
	{
		if(f0==0) return false;
		return true;
	}

	/**
	 * Invokes function linked to this signal.
	 * \param p0 Value of the parameter 1.
	 * \param p1 Value of the parameter 2.
	 * \param p2 Value of the parameter 3.
	 * \return Specified data type value.
	 */
	ret Invoke(pt0 p0, pt1 p1, pt2 p2)
	{
		return f0(p, p0, p1, p2);
	}

	/**
	 * Invokes function linked to this signal.
	 * \param p0 Value of the parameter 1.
	 * \param p1 Value of the parameter 2.
	 * \param p2 Value of the parameter 3.
	 * \return Specified data type value.
	 */
	ret operator() (pt0 p0, pt1 p1, pt2 p2)
	{
		return f0(p, p0, p1, p2);
	}

	/**
	 * Compares specified signal with this signal.
	 * \param val Pointer to the signal to compare with.
	 * \return true both are equal otherwise false.
	 */
	bool operator==(BaseSignal3<pt0, pt1, pt2,ret>& val)
	{
		if(val.f0 == f0 && val.p == p) return true;
		return false;
	}

	/**
	 * Sets slot and adds parent signal to slot connections list.
	 * \param val Pointer to the slot to connect.
	 */
	void SetSlot(BaseSlot* val)
	{
		this->s = val;
		if(s != 0) s->Signals.push_back(parent);
	}

	/**
	 * Copies specified signal to this one.
	 * \param val Pointer to the signal to copy.
	 * \return Reference to this signal.
	 */
	BaseSignal3& operator=(BaseSignal3 const& val)
	{
		this->f0 = val.f0;
		this->p = val.p;
		this->parent = val.parent;
		SetSlot(val.s);
		return *this;
	}

	/**
	 * Copies specified signal to this one.
	 * \param val Pointer to the signal to copy.
	 */
	BaseSignal3(BaseSignal3 const& val)
	{
		this->f0 = val.f0;
		this->p = val.p;
		this->parent = val.parent;
		SetSlot(val.s);
	}

	/**
	 * Virtual destructor disconnects all signals from linked slot.
	 */
	virtual ~BaseSignal3()
	{
		if(s!=0) s->DisconnectSignal(parent);
	}
};

/**
 * Signal used for signaling of events to the specified functions.
 * \tparam pt0 Data type of the parameter 1.
 * \tparam pt1 Data type of the parameter 2.
 * \tparam pt2 Data type of the parameter 3.
 */
template<class pt0, class pt1, class pt2>
class Signal3 : public BaseSignal
{
protected:
	vector<BaseSignal3<pt0, pt1, pt2> > events; ///< Signals list.
	unsigned int i; ///< Used for iteration.
	unsigned int sz; ///< Used for iteration.

public:
	/**
	 * Connects specified not derived(inherited) class and class function to this signal.
	 * \param obj Pointer to the class object which is used to invoke specified function.
	 * \return Number of connections to this signal - 1.
	 * \tparam cls Class name.
	 * \tparam fn Pointer to the class function.
	 */
	template<class cls, void (cls::*fn)(pt0, pt1, pt2)>
	unsigned int RegisterSingleHandler(cls* obj)
	{
		BaseSignal3<pt0, pt1, pt2> bs;
		bs.template Connect<cls, fn>(obj);
		events.push_back(bs);
		return events.size() - 1;
	}

	/**
	 * Connects specified derived(inherited) class and class function to this signal.
	 * \param obj Pointer to the class object which is used to invoke specified function.
	 * \return Number of connections to this signal - 1.
	 * \tparam cls Class name.
	 * \tparam fn Pointer to the class function.
	 */
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

	/**
	 * Connects specified global function to this signal.
	 * \return Number of connections to this signal - 1.
	 * \tparam fn Pointer to the global function.
	 */
	template<void (*fn)(pt0, pt1, pt2)>
	unsigned int RegisterHandler()
	{
		BaseSignal3<pt0, pt1, pt2> bs;
		bs.template Connect<fn>();
		events.push_back(bs);
		return events.size() - 1;
	}

	/**
	 * Erases specified event by its index.
	 * \param del Index of the base signal to delete.
	 */
	void DisconnectHandler(unsigned int del)
	{
		if(del<events.size())
		events.erase(events.begin()+del);
	}

	/**
	 * Disconnects specified class and class function from this signal.
	 * \param obj Pointer to the class object which is to be disconnected.
	 * \tparam cls Class name.
	 * \tparam fn Pointer to the class function.
	 */
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

	/**
	 * Disconnects specified global function from this signal.
	 * \tparam fn Pointer to the global function.
	 */
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

	/**
	 * Invokes this signal and calls all function connected to it.
	 * \param p0 Value of the parameter 1.
	 * \param p1 Value of the parameter 2.
	 * \param p2 Value of the parameter 3.
	 */
	void Invoke(pt0 p0, pt1 p1, pt2 p2)
	{
		sz = events.size();
		for(i = 0; i<sz; i++)
		{
			events[i].Invoke(p0, p1, p2);
		}		
	}

	/**
	 * Invokes this signal and calls all function connected to it.
	 * \param p0 Value of the parameter 1.
	 * \param p1 Value of the parameter 2.
	 * \param p2 Value of the parameter 3.
	 */
	void operator() (pt0 p0, pt1 p1, pt2 p2)
	{
		sz = events.size();
		for(i = 0; i<sz; i++)
		{
			events[i].Invoke(p0, p1, p2);
		}
	}

	/**
	 * Disconnects all base signals.
	 */
	void DisconnectAll()
	{
		events.clear();
	}


	/**
	 * Disconnects specified slot from this signal.
	 * \param obj Pointer to the slot to disconnect.
	 */
	void DisconnectSlot(void* obj)
	{
		for(typename vector<BaseSignal3<pt0, pt1, pt2> >::iterator i = events.begin(); i!=events.end(); i++)
		{
			if(i->s == obj)
			{
				i = events.erase(i);
			}
			if(i==events.end()) break;
		}
	}
};

// for 4 args
/**
 * Used to invoke provided function from class.
 * \tparam ret Return data type.
 * \tparam cls Class name.
 * \tparam pt0 Data type of the parameter 1.
 * \tparam pt1 Data type of the parameter 2.
 * \tparam pt2 Data type of the parameter 3.
 * \tparam pt3 Data type of the parameter 4.
 * \tparam fn Function pointer.
 */
template<class cls, class ret, class pt0, class pt1, class pt2, class pt3, ret (cls::*fn)(pt0, pt1, pt2, pt3)>
class Invoker4
{
public:
	/**
	 * Invokes specified function.
	 * \param ptr Pointer to the class object used to invoke this function.
	 * \param p0 Value of the parameter 1.
	 * \param p1 Value of the parameter 2.
	 * \param p2 Value of the parameter 3.
	 * \param p3 Value of the parameter 4.
	 * \return Returns provided data type value.
	 */
	static ret Invoke(void* ptr, pt0 p0, pt1 p1, pt2 p2, pt3 p3)
	{
		return (static_cast<cls*>(ptr)->*fn)(p0, p1, p2, p3);
	}
};

/**
 * Used to invoke global function.
 * \tparam ret Return data type.
 * \tparam fn Function pointer.
 * \tparam pt0 Data type of the parameter 1.
 * \tparam pt1 Data type of the parameter 2.
 * \tparam pt2 Data type of the parameter 3.
 * \tparam pt3 Data type of the parameter 4.
 */
template<class ret, class pt0, class pt1, class pt2, class pt3, ret (*fn)(pt0, pt1, pt2, pt3)>
class Invoker4g
{
public:
	/**
	 * Invokes specified function with specified arguments.
	 * \param ptr Not used its just for signature matching purpose.
	 * \param p0 Value of the parameter 1.
	 * \param p1 Value of the parameter 2.
	 * \param p2 Value of the parameter 3.
	 * \param p3 Value of the parameter 4.
	 * \return Returns provided data type value.
	 */
	static ret Invoke(void* ptr, pt0 p0, pt1 p1, pt2 p2, pt3 p3)
	{
		return (*fn)(p0, p1, p2, p3);
	}
};

/**
 * Signal to dynamically manage class/global function pointers.
 * \tparam pt0 Data type of the first parameter.
 * \tparam ret Return type of the functions which can be linked by this signal.
 */
template<class pt0, class pt1, class pt2, class pt3, class ret = void>
class BaseSignal4
{
public:
	typedef ret (*inv) (void*, pt0, pt1, pt2, pt3); ///< Function type.
	inv f0; ///< Function pointer to invoke linked function.
	void* p; ///< Pointer to the class object used to invoke specified class function.

	BaseSlot* s; ///<< Pointer to the base slot connected to this signal.
	BaseSignal* parent; ///< Parent signal class object used for auto disconnection of slots.

	/**
	 * Default constructor.
	 */
	BaseSignal4()
	{
		p = 0;
		f0 = 0;
		s = 0;
		parent = 0;
	}

	/**
	 * Connects specified class and class function to this signal.
	 * \param ptr Pointer to the class object which is used to invoke specified function.
	 * \tparam cls Class name.
	 * \tparam fn Pointer to the class function.
	 */
	template<class cls, ret (cls::*fn)(pt0, pt1, pt2, pt3)>
	void Connect(cls* ptr)
	{
		f0 = &Invoker4<cls, ret, pt0, pt1, pt2, pt3, fn>::Invoke;
		p = ptr;
	}

	/**
	 * Connects specified function with his signal.
	 * \tparam fn Pointer to the class function.
	 */
	template<ret (*fn)(pt0, pt1, pt2, pt3)>
	void Connect()
	{
		f0 = &Invoker4g<ret, pt0, pt1, pt2, pt3, fn>::Invoke;
		p = 0;
	}

	/**
	 * Checks whether function pointer is null.
	 * \return true if function pointer is not null otherwise false.
	 */
	bool Check()
	{
		if(f0==0) return false;
		return true;
	}

	/**
	 * Invokes function linked to this signal.
	 * \param p0 Value of the parameter 1.
	 * \param p1 Value of the parameter 2.
	 * \param p2 Value of the parameter 3.
	 * \param p3 Value of the parameter 4.
	 * \return Specified data type value.
	 */
	ret Invoke(pt0 p0, pt1 p1, pt2 p2, pt3 p3)
	{
		return f0(p, p0, p1, p2, p3);
	}

	/**
	 * Invokes function linked to this signal.
	 * \param p0 Value of the parameter 1.
	 * \param p1 Value of the parameter 2.
	 * \param p2 Value of the parameter 3.
	 * \param p3 Value of the parameter 4.
	 * \return Specified data type value.
	 */
	ret operator() (pt0 p0, pt1 p1, pt2 p2, pt3 p3)
	{
		return f0(p, p0, p1, p2, p3);
	}

	/**
	 * Compares specified signal with this signal.
	 * \param val Pointer to the signal to compare with.
	 * \return true both are equal otherwise false.
	 */
	bool operator==(BaseSignal4<pt0, pt1, pt2, pt3,ret>& val)
	{
		if(val.f0 == f0 && val.p == p) return true;
		return false;
	}

	/**
	 * Sets slot and adds parent signal to slot connections list.
	 * \param val Pointer to the slot to connect.
	 */
	void SetSlot(BaseSlot* val)
	{
		this->s = val;
		if(s != 0) s->Signals.push_back(parent);
	}

	/**
	 * Copies specified signal to this one.
	 * \param val Pointer to the signal to copy.
	 * \return Reference to this signal.
	 */
	BaseSignal4& operator=(BaseSignal4 const& val)
	{
		this->f0 = val.f0;
		this->p = val.p;
		this->parent = val.parent;
		SetSlot(val.s);
		return *this;
	}

	/**
	 * Copies specified signal to this one.
	 * \param val Pointer to the signal to copy.
	 */
	BaseSignal4(BaseSignal4 const& val)
	{
		this->f0 = val.f0;
		this->p = val.p;
		this->parent = val.parent;
		SetSlot(val.s);
	}

	/**
	 * Virtual destructor disconnects all signals from linked slot.
	 */
	virtual ~BaseSignal4()
	{
		if(s!=0) s->DisconnectSignal(parent);
	}
};

/**
 * Signal used for signaling of events to the specified functions.
 * \tparam pt0 Data type of the parameter 1.
 * \tparam pt1 Data type of the parameter 2.
 * \tparam pt2 Data type of the parameter 3.
 * \tparam pt3 Data type of the parameter 4.
 */
template<class pt0, class pt1, class pt2, class pt3>
class Signal4 : public BaseSignal
{
protected:
	vector<BaseSignal4<pt0, pt1, pt2, pt3> > events; ///< Signals list.
	unsigned int i; ///< Used for iteration.
	unsigned int sz; ///< Used for iteration.

public:
	/**
	 * Connects specified not derived(inherited) class and class function to this signal.
	 * \param obj Pointer to the class object which is used to invoke specified function.
	 * \return Number of connections to this signal - 1.
	 * \tparam cls Class name.
	 * \tparam fn Pointer to the class function.
	 */
	template<class cls, void (cls::*fn)(pt0, pt1, pt2, pt3)>
	unsigned int RegisterSingleHandler(cls* obj)
	{
		BaseSignal4<pt0, pt1, pt2, pt3> bs;
		bs.template Connect<cls, fn>(obj);
		events.push_back(bs);
		return events.size() - 1;
	}

	/**
	 * Connects specified derived(inherited) class and class function to this signal.
	 * \param obj Pointer to the class object which is used to invoke specified function.
	 * \return Number of connections to this signal - 1.
	 * \tparam cls Class name.
	 * \tparam fn Pointer to the class function.
	 */
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

	/**
	 * Connects specified global function to this signal.
	 * \return Number of connections to this signal - 1.
	 * \tparam fn Pointer to the global function.
	 */
	template<void (*fn)(pt0, pt1, pt2, pt3)>
	unsigned int RegisterHandler()
	{
		BaseSignal4<pt0, pt1, pt2, pt3> bs;
		bs.template Connect<fn>();
		events.push_back(bs);
		return events.size() - 1;
	}

	/**
	 * Erases specified event by its index.
	 * \param del Index of the base signal to delete.
	 */
	void DisconnectHandler(unsigned int del)
	{
		if(del<events.size())
		events.erase(events.begin()+del);
	}

	/**
	 * Disconnects specified class and class function from this signal.
	 * \param obj Pointer to the class object which is to be disconnected.
	 * \tparam cls Class name.
	 * \tparam fn Pointer to the class function.
	 */
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

	/**
	 * Disconnects specified global function from this signal.
	 * \tparam fn Pointer to the global function.
	 */
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

	/**
	 * Invokes this signal and calls all function connected to it.
	 * \param p0 Value of the parameter 1.
	 * \param p1 Value of the parameter 2.
	 * \param p2 Value of the parameter 3.
	 * \param p3 Value of the parameter 4.
	 */
	void Invoke(pt0 p0, pt1 p1, pt2 p2, pt3 p3)
	{
		sz = events.size();
		for(i = 0; i<sz; i++)
		{
			events[i].Invoke(p0, p1, p2, p3);
		}		
	}

	/**
	 * Invokes this signal and calls all function connected to it.
	 * \param p0 Value of the parameter 1.
	 * \param p1 Value of the parameter 2.
	 * \param p2 Value of the parameter 3.
	 * \param p3 Value of the parameter 4.
	 */
	void operator() (pt0 p0, pt1 p1, pt2 p2, pt3 p3)
	{
		sz = events.size();
		for(i = 0; i<sz; i++)
		{
			events[i].Invoke(p0, p1, p2, p3);
		}
	}

	/**
	 * Disconnects all base signals.
	 */
	void DisconnectAll()
	{
		events.clear();
	}


	/**
	 * Disconnects specified slot from this signal.
	 * \param obj Pointer to the slot to disconnect.
	 */
	void DisconnectSlot(void* obj)
	{
		for(typename vector<BaseSignal4<pt0, pt1, pt2, pt3> >::iterator i = events.begin(); i!=events.end(); i++)
		{
			if(i->s == obj)
			{
				i = events.erase(i);
			}
			if(i==events.end()) break;
		}
	}
};