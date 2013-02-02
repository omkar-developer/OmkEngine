
template <class base, class type>
class property
{
public:
	BaseSignal1<type> Set;
	BaseSignal0<type> Get;

	template<type (base::*g)(), void (base::*s) (type)>
	void Connect(base* ptr)
	{
		Set.template Connect<base, s>(ptr);
		Get.template Connect<base, g>(ptr);
	}

	type operator=(type val)
	{
		Set(val);
		return val;
	}

	operator type()
	{
		return Get();
	}

	type operator->()
	{
		return Get();
	}
};

#define PROPERTY(cls, type, name) property<cls, type> name
#define CONNECT_PROPERTY(cls, name) name.Connect<&cls::Get##name, &cls::Set##name>(this)

template <class base, class type>
class readproperty
{
public:
	BaseSignal0<type> Get;

	template<type (base::*g)()>
	void Connect(base* ptr)
	{
		Get.template Connect<base, g>(ptr);
	}

	operator type()
	{
		return Get();
	}

	type operator->()
	{
		return Get();
	}
};

#define RO_PROPERTY(cls, type, name) readproperty<cls, type> name
#define CONNECT_RO_PROPERTY(cls, name) name.Connect<&cls::Get##name>(this)

class baseproperty
{
public:
	virtual Variant GetProperty()=0;
	virtual void SetPrroperty(Variant val)=0;
};

template <class base, class type>
class SpProperty : public property<base, type>, public baseproperty
{
public:
	Variant GetProperty()
	{
		return Get();
	}

	void SetPrroperty(Variant val)
	{
		Set(val);
	}
};

template <class base, class type>
class SpReadProperty : public readproperty<base, type>, public baseproperty
{
public:
	Variant GetProperty()
	{
		return Get();
	}

	void SetPrroperty(Variant val)
	{
	}
};

class PropertyContainer
{
public:
	map<string, baseproperty*> list;

	Variant GetProperty(string name)
	{
		Variant var;
		map<string, baseproperty*>::iterator it;
		it = list.find(name);
		if(it!=list.end())
		{
			var = it->second->GetProperty();
		}
		return var;
	}

	bool SetPrroperty(string name, Variant var)
	{
		map<string, baseproperty*>::iterator it;
		it = list.find(name);
		if(it!=list.end())
		{
			it->second->SetPrroperty(var);
			return true;
		}
		return false;
	}

	void RegisterProperty(string name, baseproperty* pr)
	{
		list[name] = pr;
	}

	void RemoveProperty(string name)
	{
		map<string, baseproperty*>::iterator i = list.find(name);
		if(i!=list.end()) list.erase(i);
	}
};

#define IPROPERTY(cls, type, name) SpProperty<cls, type> name
#define IRO_PROPERTY(cls, type, name) readproperty<cls, type> name

#define ICONNECT_PROPERTY(cls, name) name.Connect<&cls::Get##name, &cls::Set##name>(this); this->pcontainer.RegisterProperty(##name, name)

#define ICONNECT_RO_PROPERTY(cls, name) name.Connect<&cls::Get##name>(this); this->pcontainer.RegisterProperty(##name, name)