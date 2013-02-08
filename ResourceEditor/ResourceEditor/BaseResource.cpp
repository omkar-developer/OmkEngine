#include "CDialogs.h"

// Resource Impl
const char* IResource::GetName() const
{
	return m_name.c_str();
}

IResource::IResource()
{
	m_status = IResource::Waiting;
	m_group = 0;
}

const wchar_t* IResource::GetFileName() const
{
	return m_filename.c_str();
}

void IResource::SetFileName(const wchar_t* filename)
{
	m_filename = filename;
}

const unsigned int IResource::GetType() const
{
	return m_type;
}

void IResource::SetType(const unsigned int type)
{
	m_type = type;
}

const unsigned int IResource::GetResourceGroup() const
{
	return m_group;
}

void IResource::SetResourceGroup(const unsigned int group)
{
	m_group = group;
}

IResource::Status IResource::GetResourceStatus()
{
	IResource::Status tmp = IResource::Waiting;
	RES_LOCK
	{
		tmp = m_status;
	}
	RES_UNLOCK;
	return tmp;
}

void IResource::SetResourceStatus(const Status val)
{
	RES_LOCK
	{
		m_status = val;
	}
	RES_UNLOCK;
}

// Resource Container
ResourceManager::ResourceManager()
{
	LoadingStatus = 0;
	m_dev = 0;
	UserData = 0;
	m_player = 0;
}

void ResourceManager::SetLoadingStatus(int status)
{
	LoadingStatus = status;
	int i = 0;
	if(LoadingStatus.GetObject(i))
	{
		OnLoadingStatusChanged(i);
	}
}

int ResourceManager::GetLoadingStatus()
{
	int i = 0;
	LoadingStatus.GetObject(i);
	return i;
}

bool ResourceManager::GetResource(const char* name, unsigned int type, Resource* res)
{
	bool tmp = false;
	RES_LOCK
	{
		string s;
		if(type==RES_TYPE_FONT)
		{
			s = "5";
			s += name;
		}
		else if(type==RES_TYPE_IMAGE)
		{
			s = "1";
			s += name;
		}
		else if(type==RES_TYPE_IMAGESET)
		{
			s = "2";
			s += name;
		}
		else if(type==RES_TYPE_SOUND)
		{
			s = "3";
			s += name;
		}
		else if(type==RES_TYPE_SPRITE)
		{
			s = "4";
			s += name;
		}
		else
		{
			s = name;
		}

		map<string, Resource >::iterator it = m_list.find(s);
		if(it != m_list.end())
		{
			if(it->second->m_type == type) *res = it->second;
		}
		else
		{
			*res = 0;
		}
	}
	RES_UNLOCK;
	return tmp;
}

void ResourceManager::AddResource(const char* name, IResource* res)
{
	RES_LOCK
	{
		string s;
		if(res->GetType()==RES_TYPE_FONT)
		{
			s += "5";
			s += name;
		}
		else if(res->GetType()==RES_TYPE_IMAGE)
		{
			s = "1";
			s += name;
		}
		else if(res->GetType()==RES_TYPE_IMAGESET)
		{
			s = "2";
			s += name;
		}
		else if(res->GetType()==RES_TYPE_SOUND)
		{
			s = "3";
			s += name;
		}
		else if(res->GetType()==RES_TYPE_SPRITE)
		{
			s = "4";
			s += name;
		}
		else
		{
			s = name;
		}

		m_list[s] = res;
	}
	RES_UNLOCK;
}

bool ResourceManager::RemoveResource(const char* name)
{
	bool tmp = false;
	RES_LOCK
	{
		string s;
		for(int f=0; f<5; f++)
		{
			s = f;
			s += name;
			map<string, Resource >::iterator i = m_list.find(s);
			if(i!=m_list.end()) 
			{
				m_list.erase(i);
				tmp = true;
				break;
			}
		}
	}
	RES_UNLOCK;
	return tmp;
}

bool ResourceManager:: ReleaseResource(const char* name)
{
	bool tmp = false;
	RES_LOCK
	{
		map<string, Resource >::iterator i;

		for(int f=0; f<5; f++)
		{
			string s;
			s = f;
			s += name;
			i = m_list.find(s);
			if(i!=m_list.end()) break;
		}

		if(i!=m_list.end())
		{
			IResource* impl = i->second->Clone();
			impl->m_group =  i->second->m_group;
			impl->m_filename =  i->second->m_filename;
			impl->m_type =  i->second->m_type;
			i->second = impl;
			tmp = true;
		}
	}
	RES_UNLOCK;
	return tmp;
}

void ResourceManager::ReleaseAllResources()
{
	RES_LOCK
	{
		unsigned int sz = m_list.size();
		float f = 0;
		for(map<string, Resource >::iterator i = m_list.begin(); i!=m_list.end(); ++i)
		{
			//i = m_list.erase(i);
			IResource* impl = i->second->Clone();
			impl->m_group =  i->second->m_group;
			impl->m_filename =  i->second->m_filename;
			impl->m_type =  i->second->m_type;
			i->second = impl;
			++f;
			SetLoadingStatus(static_cast<int>((f/sz)*100.0f));
		}
	}
	RES_UNLOCK;
}

void ResourceManager::ReleaseResourceGroup(unsigned int id)
{
	RES_LOCK
	{
		unsigned int sz = m_list.size();
		float f = 0;
		for(map<string, Resource >::iterator i = m_list.begin(); i!=m_list.end(); ++i)
		{
			if(i->second->m_group == id)
			{
				//i = m_list.erase(i);
				IResource* impl = i->second->Clone();
				impl->m_group =  i->second->m_group;
				impl->m_filename =  i->second->m_filename;
				impl->m_type =  i->second->m_type;
				i->second = impl;
				++f;
				SetLoadingStatus(static_cast<int>((f/sz)*100.0f));
			}
		}
	}
	RES_UNLOCK;
}

void ResourceManager::ReleaseResourceType(unsigned int type)
{
	RES_LOCK
	{
		unsigned int sz = m_list.size();
		float f = 0;
		for(map<string, Resource>::iterator i = m_list.begin(); i!=m_list.end(); ++i)
		{
			if(i->second->m_type == type)
			{
				//i = m_list.erase(i);
				IResource* impl = i->second->Clone();
				impl->m_group =  i->second->m_group;
				impl->m_filename =  i->second->m_filename;
				impl->m_type =  i->second->m_type;
				i->second = impl;
				++f;
				SetLoadingStatus(static_cast<int>((f/sz)*100.0f));
			}
		}
	}
	RES_UNLOCK;
}

bool ResourceManager::PrepareResources(const wchar_t* filename)
{
	bool flag = false;
	ZipFile fs;
	if(!fs.Open(filename)) return false;
	Variant count;
	count << fs;

	Variant name;
	Variant path;
	Variant type;
	Variant group;

	for(unsigned int i=0; i<static_cast<unsigned int>(count); i++)
	{
		name << fs;
		path << fs;
		type << fs;
		group << fs;

		if(CreateResource(name, group, type, path)) flag = true;
	}

	fs.Close();
	return flag;
}

void ResourceManager::LoadResources()
{
	RES_LOCK
	{
		/*LoadResourcesByType(RES_TYPE_FONT);
		LoadResourcesByType(RES_TYPE_IMAGE);
		LoadResourcesByType(RES_TYPE_SOUND);
		LoadResourcesByType(RES_TYPE_IMAGESET);
		LoadResourcesByType(RES_TYPE_SPRITE);*/

		unsigned int sz = m_list.size();
		float f = 0;
		for(map<string, Resource>::iterator i = m_list.begin(); i!=m_list.end(); ++i)
		{
			if(i->second->GetResourceStatus() == IResource::Waiting)
			{
				if(i->second->LoadFromFile(this)) i->second->SetResourceStatus(IResource::Loaded);
			}
			++f;
			SetLoadingStatus(static_cast<int>((f/sz)*100.0f));
		}
	}
	RES_UNLOCK;
}

void ResourceManager::LoadResourcesByGroup(unsigned int id)
{
	RES_LOCK
	{
		unsigned int sz = m_list.size();
		int f = 0;
		/*for(map<string, Resource>::iterator i = m_list.begin(); i!=m_list.end(); ++i)
		{
			if(i->second->GetResourceStatus() == IResource::Waiting && i->second->m_group == id && i->second->m_type == RES_TYPE_FONT)
			{
				if(i->second->LoadFromFile(this)) i->second->SetResourceStatus(IResource::Loaded);
			}
			++f;
			SetLoadingStatus(static_cast<int>((f/sz)*100.0f));
		}

		f = 0;
		for(map<string, Resource>::iterator i = m_list.begin(); i!=m_list.end(); ++i)
		{
			if(i->second->GetResourceStatus() == IResource::Waiting && i->second->m_group == id && i->second->m_type == RES_TYPE_IMAGE)
			{
				if(i->second->LoadFromFile(this)) i->second->SetResourceStatus(IResource::Loaded);
			}
			++f;
			SetLoadingStatus(static_cast<int>((f/sz)*100.0f));
		}

		f = 0;
		for(map<string, Resource>::iterator i = m_list.begin(); i!=m_list.end(); ++i)
		{
			if(i->second->GetResourceStatus() == IResource::Waiting && i->second->m_group == id && i->second->m_type == RES_TYPE_SOUND)
			{
				if(i->second->LoadFromFile(this)) i->second->SetResourceStatus(IResource::Loaded);
			}
			++f;
			SetLoadingStatus(static_cast<int>((f/sz)*100.0f));
		}

		f = 0;
		for(map<string, Resource>::iterator i = m_list.begin(); i!=m_list.end(); ++i)
		{
			if(i->second->GetResourceStatus() == IResource::Waiting && i->second->m_group == id && i->second->m_type == RES_TYPE_IMAGESET)
			{
				if(i->second->LoadFromFile(this)) i->second->SetResourceStatus(IResource::Loaded);
			}
			++f;
			SetLoadingStatus(static_cast<int>((f/sz)*100.0f));
		}

		f = 0;
		for(map<string, Resource>::iterator i = m_list.begin(); i!=m_list.end(); ++i)
		{
			if(i->second->GetResourceStatus() == IResource::Waiting && i->second->m_group == id && i->second->m_type == RES_TYPE_SPRITE)
			{
				if(i->second->LoadFromFile(this)) i->second->SetResourceStatus(IResource::Loaded);
			}
			++f;
			SetLoadingStatus(static_cast<int>((f/sz)*100.0f));
		}*/

		f = 0;
		for(map<string, Resource>::iterator i = m_list.begin(); i!=m_list.end(); ++i)
		{
			if(i->second->GetResourceStatus() == IResource::Waiting && i->second->m_group == id)
			{
				if(i->second->LoadFromFile(this)) i->second->SetResourceStatus(IResource::Loaded);
			}
			++f;
			SetLoadingStatus(static_cast<int>((f/sz)*100.0f));
		}
	}
	RES_UNLOCK;
}

void ResourceManager::LoadResourcesByType(unsigned int type)
{
	RES_LOCK
	{
		unsigned int sz = m_list.size();
		int f = 0;
		for(map<string, Resource>::iterator i = m_list.begin(); i!=m_list.end(); ++i)
		{
			if(i->second->GetResourceStatus() == IResource::Waiting && i->second->m_type == type)
			{
				if(i->second->LoadFromFile(this)) i->second->SetResourceStatus(IResource::Loaded);
			}
			++f;
			SetLoadingStatus(static_cast<int>((f/sz)*100.0f));
		}		
	}
	RES_UNLOCK;
}

void ResourceManager::OnReset()
{
	RES_LOCK
	{
		unsigned int sz = m_list.size();
		int f = 0;
		for(map<string, Resource>::iterator i = m_list.begin(); i!=m_list.end(); ++i)
		{
			i->second->OnReset();
			++f;
			SetLoadingStatus(static_cast<int>((f/sz)*100.0f));
		}		
	}
	RES_UNLOCK;
}

void ResourceManager::OnLost()
{
	RES_LOCK
	{
		unsigned int sz = m_list.size();
		int f = 0;
		for(map<string, Resource>::iterator i = m_list.begin(); i!=m_list.end(); ++i)
		{
			i->second->OnLost();
			++f;
			SetLoadingStatus(static_cast<int>((f/sz)*100.0f));
		}
	}
	RES_UNLOCK;
}

void ResourceManager::RegisterLoader(unsigned int type, IResource* res)
{
	m_type_list[type] = res;
}

bool ResourceManager::RemoveLoader(unsigned int type)
{
	if(m_type_list.erase(type)>0)
	{
		return true;
	}
	return false;
}

void ResourceManager::ClearLoaders()
{
	m_type_list.clear();
}

bool ResourceManager::CreateResource(const char* name, unsigned int group, unsigned int type, const wchar_t* path)
{
	bool flag = false;
	map<unsigned int, Resource>::iterator it = m_type_list.find(type);
	if(it != m_type_list.end())
	{
		IResource* impl = it->second->Clone();
		impl->m_group = group;
		impl->m_filename = path;
		impl->m_type = type;
		impl->m_name = name;
		AddResource(name, impl);
		return true;
	}
	return false;
}

void ResourceManager::SetDevice(C_Device* dev)
{
	m_dev = dev;
}

C_Device* ResourceManager::GetDevice()
{
	return m_dev;
}

void ResourceManager::SetSoundDevice(C_SoundDevice* dev)
{
	m_player = dev;
}

C_SoundDevice* ResourceManager::GetSoundDevice()
{
	return m_player;
}