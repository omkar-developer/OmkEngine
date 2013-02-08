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
	currentloader = m_list.end();
	m_progress = 0;
	m_grpid = 0;
	m_sychloading = false;
}

bool ResourceManager::GetSynchLoading()
{
	return m_sychloading;
}

void ResourceManager::ResetSynchLoader(unsigned int gid)
{
	currentloader = m_list.begin();
	m_progress = 0;
	m_grpid = gid;
	m_sychloading = true;
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

bool ResourceManager::ReleaseResource(const char* name)
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
	set<string> files;
	ZipFile::GetFilesList(files);

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
		char p[256];
		WideCharToMultiByte(p, path, 256);
		files.erase(p);
	}

	fs.Close();

	vector<string> arr;
	vector<string> arr2;
	vector<string> arr3;

#ifdef ANDROID
	vector<string> arr4;
#endif

	for(set<string>::iterator i = files.begin(); i != files.end(); i++)
	{
#ifdef ANDROID
		arr4.clear();
		GetStringArray(arr4, (*i).c_str(), '/');
		string asset = arr4[0];
		std::transform(asset.begin(), asset.end(), asset.begin(), tolower);
		if(asset != "assets") continue;
		arr4.erase(arr4.begin());

		string tpaths;

		for(unsigned int i = 0; i<arr4.size(); i++)
		{
			tpaths += arr4[i];
		}
#else
		string tpaths = (*i);
#endif

		arr.clear();
		GetStringArray(arr, tpaths.c_str(), '.');
		arr2.clear();

		string pth = arr[0];
		for(unsigned int f = 1; f<arr.size() - 1; f++)
		{
			pth += arr[f];
		}

		GetStringArray(arr2, pth.c_str(), '/');

		string fname = arr2[arr2.size()-1];
		string ext;
		if(arr.size()>1) ext = arr[arr.size()-1];
		//std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
		StrToLower(ext);

		arr3.clear();
		GetStringArray(arr3, pth.c_str(), '#');
		fname = arr3[0];
		int grp = 0;
		if(arr3.size()>1) 
		{
			grp = atoi(arr3[arr3.size()-1].c_str());

			pth = arr3[0];
			for(unsigned int f = 1; f<arr3.size() - 1; f++)
			{
				if(f!=1) pth += "/";
				pth += arr3[f];
			}
		}

		wchar_t wc[256];
		MultiByteToWideChar(wc, tpaths.c_str(), 256);

		if(ext == "png")
		{			
			CreateResource(pth.c_str(), grp, RES_TYPE_IMAGE, wc);
		}
		else if(ext == "iset")
		{
			pth += "iset";
			CreateResource(pth.c_str(), grp, RES_TYPE_IMAGESET, wc);
		}
		else if(ext == "ogg")
		{
			pth += "sound";
			CreateResource(pth.c_str(), grp, RES_TYPE_SOUND, wc);
		}
		else if(ext == "ttf")
		{
			
			
		}
	}

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
			SetLoadingStatus(static_cast<int>((f/(float)sz)*100.0f));
		}
	}
	RES_UNLOCK;
}

bool ResourceManager::LoadResourcesByGroupSynch()
{
	unsigned int sz = m_list.size();
	
	if(currentloader==m_list.end())
	{
		return true;
	}

	if(currentloader->second->GetResourceStatus() == IResource::Waiting && currentloader->second->m_group == m_grpid)
	{
#if defined( ANDROID )
		char tmp[250];
		WideCharToMultiByte(tmp, currentloader->second->GetFileName(), 250);
		__android_log_print(ANDROID_LOG_VERBOSE, "native", "Loading File >> %s", tmp);
#endif
#ifdef __AVM2__
		char tmp[250];
		WideCharToMultiByte(tmp, currentloader->second->GetFileName(), 250);
		string str = "Loading File >> ";
		str += tmp;		
		PrintAVMLog(str);
#endif
		if(currentloader->second->LoadFromFile(this))
		{
			currentloader->second->SetResourceStatus(IResource::Loaded);
#ifdef ANDROID 
			__android_log_print(ANDROID_LOG_VERBOSE, "native", "Loading Sucessful >> %s", tmp);
#endif
#ifdef __AVM2__
			str = "Loading Successful >> ";
			str += tmp;
			PrintAVMLog(str);
#endif
		}
	}
	++m_progress;
	currentloader++;
	SetLoadingStatus(static_cast<int>((m_progress/(float)sz)*100.0f));

	return false;
}

void ResourceManager::LoadResourcesByGroup(unsigned int id)
{
	RES_LOCK
	{
		unsigned int sz = m_list.size();
		int f = 0;

		for(map<string, Resource>::iterator i = m_list.begin(); i!=m_list.end(); ++i)
		{
			if(i->second->GetResourceStatus() == IResource::Waiting && i->second->m_group == id)
			{
#if defined( ANDROID )
				char tmp[250];
				WideCharToMultiByte(tmp, i->second->GetFileName(), 250);
				__android_log_print(ANDROID_LOG_VERBOSE, "native", "Loading File >> %s", tmp);
#endif
#ifdef __AVM2__
				char tmp[250];
				WideCharToMultiByte(tmp, i->second->GetFileName(), 250);
				string str = "Loading File >> ";
				str += tmp;		
				PrintAVMLog(str);
#endif
				if(i->second->LoadFromFile(this))
				{
					i->second->SetResourceStatus(IResource::Loaded);
#ifdef ANDROID 
				__android_log_print(ANDROID_LOG_VERBOSE, "native", "Loading Sucessful >> %s", tmp);
#endif
#ifdef __AVM2__
				str = "Loading Successful >> ";
				str += tmp;
				PrintAVMLog(str);
#endif
				}
			}
			++f;
			SetLoadingStatus(static_cast<int>((f/(float)sz)*100.0f));
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
#ifdef ANDROID 
				char tmp[250];
				WideCharToMultiByte(tmp, i->second->GetFileName(), 250);
				__android_log_print(ANDROID_LOG_VERBOSE, "native", "Loading File >> %s", tmp);
#endif
				if(i->second->LoadFromFile(this)) i->second->SetResourceStatus(IResource::Loaded);
#ifdef ANDROID 
				__android_log_print(ANDROID_LOG_VERBOSE, "native", "Loading Sucessful >> %s", tmp);
#endif
			}
			++f;
			SetLoadingStatus(static_cast<int>((f/(float)sz)*100.0f));
		}		
	}
	RES_UNLOCK;
}

void ResourceManager::GetResourceNames(vector<string>& val)
{
	val.clear();
	RES_LOCK
	{
		unsigned int sz = m_list.size();
		int f = 0;
		for(map<string, Resource>::iterator i = m_list.begin(); i!=m_list.end(); ++i)
		{
			if(i->second->GetType()==RES_TYPE_SPRITE) continue;
			stringstream str;
			str << i->second->GetName();
			/*str << "\n";
			str << i->second->GetType();*/ 
			val.push_back(str.str());
			++f;
		}		
	}
	RES_UNLOCK;
}

void ResourceManager::OnReset()
{
#ifdef ANDROID
	__android_log_print(ANDROID_LOG_VERBOSE, "native", "<<< Resetting resources >>>");
#endif
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