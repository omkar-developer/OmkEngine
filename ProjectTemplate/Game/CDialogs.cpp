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

#include "CDialogs.h"
#include "PropertiesIO.h"

////
wxPGChoices resources;
vector<string> names;

PropertiesIO::PropertiesIO(ResourceManager* mgr) : BaseIO(mgr)
{
	pmgr = 0;
	pos = 0;
}

PropertiesIO::~PropertiesIO()
{
	//Close();
}

wxPropertyGridManager* PropertiesIO::GetPropertyGridManager()
{
	return pmgr;
}

void PropertiesIO::SetPropertyGridManager(wxPropertyGridManager* val)
{
	pmgr = val;
	pos = 0;
	it=pmgr->GetVIterator(wxPG_ITERATE_PROPERTIES|wxPG_ITERATE_HIDDEN);
}

bool PropertiesIO::IsGood()
{
	if(pmgr==0) return false;
	return true;
}

bool PropertiesIO::Open(const wchar_t* filename, Access access, CreationFlags flags, ShareMode share)
{
	Close();
	return true;
}

void PropertiesIO::Close()
{
	pos = 0;
	if(!pmgr) return;
	//pmgr->Clear();
	pmgr->CollapseAll();
	it=pmgr->GetVIterator(wxPG_ITERATE_PROPERTIES|wxPG_ITERATE_HIDDEN);
}

bool PropertiesIO::Seek(long long val, SeekMode mode, long long* newdest)
{
	if(mode==Seek_Set && val < GetSize()) pos = val;
	else if(mode==Seek_Current && pos + val < GetSize()) pos += val;
	else if(mode==Seek_End) pos = GetSize() - 1;
	else return false;
	if(newdest) *newdest = pos;
	return true;
}

long long PropertiesIO::GetPosition()
{
	return pos;
}

long long PropertiesIO::GetSize()
{
	if(pmgr)
	{
		long long i = 0;
		wxPGVIterator it2;
		for(it2=pmgr->GetVIterator(wxPG_ITERATE_PROPERTIES);!it2.AtEnd();it2.Next(), i++)
		{
		}
		return i;
	}
	return 0;
}

bool PropertiesIO::Write(const bool& val, unsigned int* sz, const char* name, bool hidden)
{
	if(hidden) return true;
	if(!pmgr) return false;
	map<string, string> lst;
	if(name!=0) GetStringMap(lst, name);
	if(lst["category"] != wxT(""))
	{
		pmgr->Append( new wxPropertyCategory(lst["category"]) );
	}
	pmgr->Append(new wxBoolProperty(lst["name"], *wxPGProperty::sm_wxPG_LABEL, val))->SetHelpString(lst["help"]);
	if(sz!=0) *sz = 1;
	return true;
}
bool PropertiesIO::Read(bool& val, unsigned int* sz, bool hidden)
{
	if(hidden) return true;
	if(!pmgr) return false;
	wxPGProperty* p = it.GetProperty();
	val = p->GetValue().GetBool();
	it.Next();
	if(it.AtEnd())
	{
		if(sz!=0) *sz = 0;
		return false;
	}
	++pos;
	if(sz!=0) *sz = 1;
	return true;
}

bool PropertiesIO::Write(const char& val, unsigned int* sz, const char* name, bool hidden)
{
	if(hidden) return true;
	if(!pmgr) return false;
	map<string, string> lst;
	if(name!=0) GetStringMap(lst, name);
	if(lst["category"] != wxT(""))
	{
		pmgr->Append( new wxPropertyCategory(lst["category"]) );
	}
	pmgr->Append(new wxStringProperty(lst["name"], *wxPGProperty::sm_wxPG_LABEL, val))->SetHelpString(lst["help"]);
	if(sz!=0) *sz = 1;
	return true;
}

bool PropertiesIO::Read(char& val, unsigned int* sz, bool hidden)
{
	if(hidden) return true;
	if(!pmgr) return false;
	wxPGProperty* p = it.GetProperty();
	val = p->GetValue().GetChar();
	it.Next();
	if(it.AtEnd())
	{
		if(sz!=0) *sz = 0;
		return false;
	}
	++pos;
	if(sz!=0) *sz = 1;
	return true;
}

bool PropertiesIO::Write(const wchar_t& val, unsigned int* sz, const char* name, bool hidden)
{
	if(hidden) return true;
	if(!pmgr) return false;
	map<string, string> lst;
	if(name!=0) GetStringMap(lst, name);
	if(lst["category"] != wxT(""))
	{
		pmgr->Append( new wxPropertyCategory(lst["category"]) );
	}
	pmgr->Append(new wxStringProperty(lst["name"], *wxPGProperty::sm_wxPG_LABEL, val))->SetHelpString(lst["help"]);
	if(sz!=0) *sz = 1;
	return true;
}

bool PropertiesIO::Read(wchar_t& val, unsigned int* sz, bool hidden)
{
	if(hidden) return true;
	if(!pmgr) return false;
	wxPGProperty* p = it.GetProperty();
	val = p->GetValue().GetChar();
	it.Next();
	if(it.AtEnd())
	{
		if(sz!=0) *sz = 0;
		return false;
	}
	++pos;
	if(sz!=0) *sz = 1;
	return true;
}

bool PropertiesIO::Write(const short& val, unsigned int* sz, const char* name, bool hidden)
{
	if(hidden) return true;
	if(!pmgr) return false;
	map<string, string> lst;
	if(name!=0) GetStringMap(lst, name);
	if(lst["category"] != wxT(""))
	{
		pmgr->Append( new wxPropertyCategory(lst["category"]) );
	}
	pmgr->Append(new wxIntProperty(lst["name"], *wxPGProperty::sm_wxPG_LABEL, val))->SetHelpString(lst["help"]);
	if(sz!=0) *sz = 1;
	return true;
}

bool PropertiesIO::Read(short& val, unsigned int* sz, bool hidden)
{
	if(hidden) return true;
	if(!pmgr) return false;
	wxPGProperty* p = it.GetProperty();
	val = p->GetValue().GetInteger();
	it.Next();
	if(it.AtEnd())
	{
		if(sz!=0) *sz = 0;
		return false;
	}
	++pos;
	if(sz!=0) *sz = 1;
	return true;
}

bool PropertiesIO::Write(const unsigned short& val, unsigned int* sz, const char* name, bool hidden)
{
	if(hidden) return true;
	if(!pmgr) return false;
	map<string, string> lst;
	if(name!=0) GetStringMap(lst, name);
	if(lst["category"] != wxT(""))
	{
		pmgr->Append( new wxPropertyCategory(lst["category"]) );
	}
	pmgr->Append(new wxUIntProperty(lst["name"], *wxPGProperty::sm_wxPG_LABEL, val))->SetHelpString(lst["help"]);
	if(sz!=0) *sz = 1;
	return true;
}

bool PropertiesIO::Read(unsigned short& val, unsigned int* sz, bool hidden)
{
	if(hidden) return true;
	if(!pmgr) return false;
	wxPGProperty* p = it.GetProperty();
	val = p->GetValue().GetInteger();
	it.Next();
	if(it.AtEnd())
	{
		if(sz!=0) *sz = 0;
		return false;
	}
	++pos;
	if(sz!=0) *sz = 1;
	return true;
}

bool PropertiesIO::Write(const int& val, unsigned int* sz, const char* name, bool hidden)
{
	if(hidden) return true;
	if(!pmgr) return false;
	map<string, string> lst;
	if(name!=0) GetStringMap(lst, name);
	if(lst["category"] != wxT(""))
	{
		pmgr->Append( new wxPropertyCategory(lst["category"]) );
	}
	if(lst["type"]==wxT("list"))
	{
		vector<string> vals;
		GetStringArray(vals, lst["list"].c_str(), ',');
		wxPGChoices choice;
		
		for(unsigned int i = 0; i<vals.size(); i++)
		{
			choice.Add(vals[i].c_str());
		}

		wxEnumProperty* prop = new wxEnumProperty(lst["name"], *wxPGProperty::sm_wxPG_LABEL, choice, val);
		//wxColourProperty* prop = new wxColourProperty(lst["name"], *wxPGProperty::sm_wxPG_LABEL, val);
		prop->SetHelpString(lst["help"]);
		//prop->SetAttribute("HasAlpha", true);
		pmgr->Append(prop);
		//prop->set
	}
	else if(lst["type"]==wxT("flags"))
	{
		vector<string> vals;
		GetStringArray(vals, lst["list"].c_str(), ',');
		wxPGChoices choice;
		
		for(unsigned int i = 0; i<vals.size(); i++)
		{
			choice.Add(vals[i].c_str(), pow(2.0f, (float)i));
		}

		wxFlagsProperty* prop = new wxFlagsProperty(lst["name"], *wxPGProperty::sm_wxPG_LABEL, choice, val);
		
		//wxColourProperty* prop = new wxColourProperty(lst["name"], *wxPGProperty::sm_wxPG_LABEL, val);

		prop->SetHelpString(lst["help"]);
		prop->SetAttribute(wxPG_BOOL_USE_CHECKBOX, true);
		pmgr->Append(prop);
		//prop->set
	}
	else pmgr->Append(new wxIntProperty(lst["name"], *wxPGProperty::sm_wxPG_LABEL, val))->SetHelpString(lst["help"]);
	if(sz!=0) *sz = 1;
	return true;
}

bool PropertiesIO::Read(int& val, unsigned int* sz, bool hidden)
{
	if(hidden) return true;
	if(!pmgr) return false;
	wxPGProperty* p = it.GetProperty();
	val = p->GetValue().GetInteger();
	it.Next();
	if(it.AtEnd())
	{
		if(sz!=0) *sz = 0;
		return false;
	}
	++pos;
	if(sz!=0) *sz = 1;
	return true;
}

bool PropertiesIO::Write(const unsigned int& val, unsigned int* sz, const char* name, bool hidden)
{
	if(hidden) return true;
	if(!pmgr) return false;
	map<string, string> lst;
	if(name!=0) GetStringMap(lst, name);
	if(lst["category"] != wxT(""))
	{
		pmgr->Append( new wxPropertyCategory(lst["category"]) );
	}
	if(lst["type"]==wxT("color"))
	{
		wxColourProperty* prop = new wxColourProperty(lst["name"], *wxPGProperty::sm_wxPG_LABEL, val);
		prop->SetHelpString(lst["help"]);
		prop->SetAttribute("HasAlpha", true);

		ColorARGB32 col2(val);
		wxColour col;
		col.SetRGBA(val);
		col.Set(col2.GetR(), col2.GetG(), col2.GetB(), col2.GetA());
		wxVariant var;
		var << col;
		prop->SetValue(var);
		pmgr->Append(prop);
		//prop->set
	}
	else if(lst["type"]==wxT("list"))
	{
		vector<string> vals;
		GetStringArray(vals, lst["list"].c_str(), ',');
		wxPGChoices choice;
		
		for(unsigned int i = 0; i<vals.size(); i++)
		{
			choice.Add(vals[i].c_str());
		}

		wxEnumProperty* prop = new wxEnumProperty(lst["name"], *wxPGProperty::sm_wxPG_LABEL, choice, val);
		//wxColourProperty* prop = new wxColourProperty(lst["name"], *wxPGProperty::sm_wxPG_LABEL, val);
		prop->SetHelpString(lst["help"]);
		//prop->SetAttribute("HasAlpha", true);
		pmgr->Append(prop);
		//prop->set
	}
	else if(lst["type"]==wxT("flags"))
	{
		vector<string> vals;
		GetStringArray(vals, lst["list"].c_str(), ',');
		wxPGChoices choice;
		
		for(unsigned int i = 0; i<vals.size(); i++)
		{
			choice.Add(vals[i].c_str(), pow(2.0f, (float)i));
		}

		wxFlagsProperty* prop = new wxFlagsProperty(lst["name"], *wxPGProperty::sm_wxPG_LABEL, choice, val);
		
		//wxColourProperty* prop = new wxColourProperty(lst["name"], *wxPGProperty::sm_wxPG_LABEL, val);

		prop->SetHelpString(lst["help"]);
		prop->SetAttribute(wxPG_BOOL_USE_CHECKBOX, true);
		pmgr->Append(prop);
		//prop->set
	}
	else pmgr->Append(new wxUIntProperty(lst["name"], *wxPGProperty::sm_wxPG_LABEL, val))->SetHelpString(lst["help"]);
	if(sz!=0) *sz = 1;
	return true;
}

bool PropertiesIO::Read(unsigned int& val, unsigned int* sz, bool hidden)
{
	if(hidden) return true;
	if(!pmgr) return false;
	wxPGProperty* p = it.GetProperty();
	wxString vt = p->GetValueType();
	if(vt != wxT("long"))
	{
		wxString str = p->GetValueAsString();
		wxColour col;
		col << p->GetValue();
		val = ColorARGB32(col.Alpha(), col.Red(), col.Green(), col.Blue());
	}
	else
	{
		val = p->GetValue().GetInteger();
	}

	it.Next();
	if(it.AtEnd())
	{
		if(sz!=0) *sz = 0;
		return false;
	}
	++pos;
	if(sz!=0) *sz = 1;
	return true;
}

bool PropertiesIO::Write(const long& val, unsigned int* sz, const char* name, bool hidden)
{
	if(hidden) return true;
	if(!pmgr) return false;
	map<string, string> lst;
	if(name!=0) GetStringMap(lst, name);
	if(lst["category"] != wxT(""))
	{
		pmgr->Append( new wxPropertyCategory(lst["category"]) );
	}
	pmgr->Append(new wxIntProperty(lst["name"], *wxPGProperty::sm_wxPG_LABEL, val))->SetHelpString(lst["help"]);
	if(sz!=0) *sz = 1;
	return true;
}

bool PropertiesIO::Read(long& val, unsigned int* sz, bool hidden)
{
	if(hidden) return true;
	if(!pmgr) return false;
	wxPGProperty* p = it.GetProperty();
	val = p->GetValue().GetLong();
	it.Next();
	if(it.AtEnd())
	{
		if(sz!=0) *sz = 0;
		return false;
	}
	++pos;
	if(sz!=0) *sz = 1;
	return true;
}

bool PropertiesIO::Write(const unsigned long& val, unsigned int* sz, const char* name, bool hidden)
{
	if(hidden) return true;
	if(!pmgr) return false;
	map<string, string> lst;
	if(name!=0) GetStringMap(lst, name);
	if(lst["category"] != wxT(""))
	{
		pmgr->Append( new wxPropertyCategory(lst["category"]) );
	}
	pmgr->Append(new wxUIntProperty(lst["name"], *wxPGProperty::sm_wxPG_LABEL, val))->SetHelpString(lst["help"]);
	if(sz!=0) *sz = 1;
	return true;
}

bool PropertiesIO::Read(unsigned long& val, unsigned int* sz, bool hidden)
{
	if(hidden) return true;
	if(!pmgr) return false;
	wxPGProperty* p = it.GetProperty();
	val = p->GetValue().GetLong();
	it.Next();
	if(it.AtEnd())
	{
		if(sz!=0) *sz = 0;
		return false;
	}
	++pos;
	if(sz!=0) *sz = 1;
	return true;
}

bool PropertiesIO::Write(const long long& val, unsigned int* sz, const char* name, bool hidden)
{
	if(hidden) return true;
	if(!pmgr) return false;
	map<string, string> lst;
	if(name!=0) GetStringMap(lst, name);
	if(lst["category"] != wxT(""))
	{
		pmgr->Append( new wxPropertyCategory(lst["category"]) );
	}
	pmgr->Append(new wxIntProperty(lst["name"], *wxPGProperty::sm_wxPG_LABEL, val))->SetHelpString(lst["help"]);
	if(sz!=0) *sz = 1;
	return true;
}

bool PropertiesIO::Read(long long& val, unsigned int* sz, bool hidden)
{
	if(hidden) return true;
	if(!pmgr) return false;
	wxPGProperty* p = it.GetProperty();
	val = p->GetValue().GetLongLong().GetValue();
	it.Next();
	if(it.AtEnd())
	{
		if(sz!=0) *sz = 0;
		return false;
	}
	++pos;
	if(sz!=0) *sz = 1;
	return true;
}

bool PropertiesIO::Write(const unsigned long long& val, unsigned int* sz, const char* name, bool hidden)
{
	if(hidden) return true;
	if(!pmgr) return false;
	map<string, string> lst;
	if(name!=0) GetStringMap(lst, name);
	if(lst["category"] != wxT(""))
	{
		pmgr->Append( new wxPropertyCategory(lst["category"]) );
	}
	pmgr->Append(new wxUIntProperty(lst["name"], *wxPGProperty::sm_wxPG_LABEL, val))->SetHelpString(lst["help"]);
	if(sz!=0) *sz = 1;
	return true;
}

bool PropertiesIO::Read(unsigned long long& val, unsigned int* sz, bool hidden)
{
	if(hidden) return true;
	if(!pmgr) return false;
	wxPGProperty* p = it.GetProperty();
	val = p->GetValue().GetULongLong().GetValue();
	it.Next();
	if(it.AtEnd())
	{
		if(sz!=0) *sz = 0;
		return false;
	}
	++pos;
	if(sz!=0) *sz = 1;
	return true;
}

bool PropertiesIO::Write(const float& val, unsigned int* sz, const char* name, bool hidden)
{
	if(hidden) return true;
	if(!pmgr) return false;
	map<string, string> lst;
	if(name!=0) GetStringMap(lst, name);
	if(lst["category"] != wxT(""))
	{
		pmgr->Append( new wxPropertyCategory(lst["category"]) );
	}
	pmgr->Append(new wxFloatProperty(lst["name"], *wxPGProperty::sm_wxPG_LABEL, val))->SetHelpString(lst["help"]);
	if(sz!=0) *sz = 1;
	return true;
}

bool PropertiesIO::Read(float& val, unsigned int* sz, bool hidden)
{
	if(hidden) return true;
	if(!pmgr) return false;
	wxPGProperty* p = it.GetProperty();
	val = p->GetValue().GetDouble();
	it.Next();
	if(it.AtEnd())
	{
		if(sz!=0) *sz = 0;
		return false;
	}
	++pos;
	if(sz!=0) *sz = 1;
	return true;
}

bool PropertiesIO::Write(const double& val, unsigned int* sz, const char* name, bool hidden)
{
	if(hidden) return true;
	if(!pmgr) return false;
	map<string, string> lst;
	if(name!=0) GetStringMap(lst, name);
	if(lst["category"] != wxT(""))
	{
		pmgr->Append( new wxPropertyCategory(lst["category"]) );
	}
	pmgr->Append(new wxFloatProperty(lst["name"], *wxPGProperty::sm_wxPG_LABEL, val))->SetHelpString(lst["help"]);
	if(sz!=0) *sz = 1;
	return true;
}

bool PropertiesIO::Read(double& val, unsigned int* sz, bool hidden)
{
	if(hidden) return true;
	if(!pmgr) return false;
	wxPGProperty* p = it.GetProperty();
	val = p->GetValue().GetDouble();
	it.Next();
	if(it.AtEnd())
	{
		if(sz!=0) *sz = 0;
		return false;
	}
	++pos;
	if(sz!=0) *sz = 1;
	return true;
}

bool PropertiesIO::Write(const char* val, unsigned int* sz, const char* name, bool hidden)
{
	if(hidden) return true;
	if(!pmgr) return false;
	map<string, string> lst;
	if(name!=0) GetStringMap(lst, name);
	if(lst["category"] != wxT(""))
	{
		pmgr->Append( new wxPropertyCategory(lst["category"]) );
	}
	if(lst["name"]=="ObjectType") return true;
	pmgr->Append(new wxLongStringProperty(lst["name"], *wxPGProperty::sm_wxPG_LABEL, val))->SetHelpString(lst["help"]);
	if(sz!=0) *sz = 1;
	return true;
}

bool PropertiesIO::Write(const wchar_t* val, unsigned int* sz, const char* name, bool hidden)
{
	if(hidden) return true;
	if(!pmgr) return false;
	map<string, string> lst;
	if(name!=0) GetStringMap(lst, name);
	if(lst["category"] != wxT(""))
	{
		pmgr->Append( new wxPropertyCategory(lst["category"]) );
	}
	pmgr->Append(new wxLongStringProperty(lst["name"], *wxPGProperty::sm_wxPG_LABEL, val))->SetHelpString(lst["help"]);
	if(sz!=0) *sz = 1;
	return true;
}

bool PropertiesIO::Write(const string& val, unsigned int* sz, const char* name, bool hidden)
{
	if(hidden) return true;
	return Write(val.c_str(), sz, name);
}

bool PropertiesIO::Read(string& val, unsigned int* sz, bool hidden)
{
	if(hidden) return true;
	if(!pmgr) return false;
	wxPGProperty* p = it.GetProperty();
	wxString s = p->GetValue().GetString();
	s.Replace(wxT("\\n"), wxT("\n"));
	val = s.ToStdString();
	it.Next();
	if(it.AtEnd())
	{
		if(sz!=0) *sz = 0;
		return false;
	}
	++pos;
	if(sz!=0) *sz = 1;
	return true;
}

bool PropertiesIO::Write(const wstring& val, unsigned int* sz, const char* name, bool hidden)
{
	if(hidden) return true;
	return Write(val.c_str(), sz, name);
}

bool PropertiesIO::Read(wstring& val, unsigned int* sz, bool hidden)
{
	if(hidden) return true;
	if(!pmgr) return false;
	wxPGProperty* p = it.GetProperty();
	wxString s = p->GetValue().GetString();
	s.Replace(wxT("\\n"), wxT("\n"));
	val = s.ToStdWstring();
	it.Next();
	if(it.AtEnd())
	{
		if(sz!=0) *sz = 0;
		return false;
	}
	++pos;
	if(sz!=0) *sz = 1;
	return true;
}

bool PropertiesIO::Write(Resource* val, unsigned int* sz, const char* name, bool hidden)
{
	if(hidden) return true;
	if(!pmgr) return false;
	map<string, string> lst;
	if(name!=0) GetStringMap(lst, name);
	if(lst["category"] != wxT(""))
	{
		pmgr->Append( new wxPropertyCategory(lst["category"]) );
	}

	stringstream str(lst["default"]);
	unsigned int def = 0;
	str >> def;

	string restype = "name=";
	restype += lst["name"];
	restype += " Resource Type;help=Resource type;type=list;list=none,image,font,sound,sprite,object,imageset";
	string resname;
	resname += lst["name"];
	resname += " Resource Name";

	wxString v;
	if(val->IsValid())
	{
		Write((*val)->GetType(), sz, restype.c_str());
		v = (*val)->GetName();
	}
	else
	{
		Write(def, sz, restype.c_str());
	}

	wxEditEnumProperty* prop = new wxEditEnumProperty(resname, *wxPGProperty::sm_wxPG_LABEL, resources, v);
	pmgr->Append(prop)->SetHelpString(lst["help"]);
	
	if(sz!=0) *sz = 1;
	return true;
}

bool PropertiesIO::Read(Resource* val, unsigned int* sz, bool hidden)
{
	if(hidden) return true;
	if(!pmgr) return false;

	unsigned int rtype = 0;
	Read(rtype);

	wxPGProperty* p = it.GetProperty();

	wxString valtype = p->GetValueType();

	string str;

	if(valtype=="string")
	{
		str = p->GetValue().GetString();
	}
	else
	{
		int val = p->GetValue().GetInteger();
		if(val>=0 && val < names.size()) str = names[val];
	}
	
	if(str.size()<1)
	{
		*val = 0;
		it.Next();
		++pos;
		return false;
	}
	if(m_mgr==0)
	{
		*val = 0;
		it.Next();
		++pos;
		return false;
	}
	m_mgr->GetResource(str.c_str(), rtype, val);

	it.Next();
	if(it.AtEnd())
	{
		if(sz!=0) *sz = 0;
		return false;
	}
	++pos;
	if(sz!=0) *sz = 1;
	return true;
}

bool PropertiesIO::Write(const void* val, unsigned int* sz, const char* name)
{
	return false;
}

bool PropertiesIO::Read(void* val, unsigned int* sz)
{
	return false;
}
///////////////////////////////////////



Dialog::Dialog(wxDialog *dlg)
    : IDialog(dlg)
{
	//D3DXVECTOR3 vec1(1.0f, 0.0f, 0.0f);
	//D3DXVECTOR3 vec2(0, 0, 1.0f), out;
	//D3DXVec3Cross(&out, &vec1, &vec2);
	Game = GetGameAPI(L"Editor", 800, 480);
	Game->Init();
	loading = false;

	Game->GetResourceManager()->GetResourceNames(names);

	for(unsigned int i = 0; i<names.size(); i++)
	{
		resources.Add(names[i].c_str(), i);
	}
	
	Game->GetMainContainer()->Callback.RegisterHandler<Dialog, &Dialog::Callback>(this);
	sel = 0;
	Game->GetMainContainer()->SetEditable(true);

	m_propertyGridManager1->Connect(wxEVT_PG_CHANGED, wxPropertyGridEventHandler(Dialog::On_Property), 0, this);

	vector<string> lst;
	Game->GetBuilder()->GetBuildersList(lst);
	m_listBox1->Clear();
	for(unsigned int i=0; i<lst.size(); i++)
	{
		m_listBox1->Append(lst[i]);
	}

	Game->ShowFPS = true;
#ifdef USEBOX2DPHYSICS
	Game->m_DebugDraw = false;
#endif
	Game->DrawBoxes = false;

	cdiag = new CScriptEditor(this);
}

void Dialog::On_DDraw( wxCommandEvent& event )
{
	bool flag = !Game->DrawBoxes;
#ifdef USEBOX2DPHYSICS
	Game->m_DebugDraw = flag;
#endif
	Game->DrawBoxes = flag;
}

void Dialog::OnMDown( wxMouseEvent& event )
{

}

void Dialog::On_Delete( wxCommandEvent& event )
{
	if(sel)
	{
		//Game->GetMainContainer()->SetEditorObject(0);
		sel->QueueForDelete();
		sel = 0;
	}
}

void Dialog::On_ShowAll( wxCommandEvent& event )
{
	if(sel && sel->GetType()&OBJECT_TYPE_CONTAINER)
	{
		((ObjectsContainer*)(sel))->ShowAll();
	}
	else if(sel)
	{
		sel->m_flags &= ~OBJECT_Flag_Invisible;
	}
}

void Dialog::On_Hide( wxCommandEvent& event )
{
	if(sel && sel->GetType()&OBJECT_TYPE_CONTAINER)
	{
		((ObjectsContainer*)(sel))->SetEditorObject(0);
		((ObjectsContainer*)(sel))->HideAll();	
	}
	else if(sel && sel->GetType()&OBJECT_TYPE_DRAWABLE)
	{
		/*Game->GetMainContainer()->SetEditorObject(0);
		((DrawableObject*)sel)->ReleaseCapture();
		((DrawableObject*)sel)->ReleaseDrawTop();
		((DrawableObject*)sel)->ReleaseEditor();
		((DrawableObject*)sel)->ReleaseSelected();*/
		sel->m_flags |= OBJECT_Flag_Invisible;
	}
}

void Dialog::On_SetPath( wxCommandEvent& event )
{
	Game->GetDevice()->SetFullscreen(!Game->GetDevice()->GetFullscreen());
	Game->GetMainContainer()->AdjustSize(*Game->GetDevice());
}

void Dialog::On_Reset( wxCommandEvent& event )
{
	if(sel)
	{
		wxString str;
		str = wxGetTextFromUser("Enter Property Name : ", "Input");
		sel->SetProperty(str.ToStdString().c_str(), 0);
	}
}

void Dialog::On_SetColor( wxCommandEvent& event )
{
	if(sel && sel->GetType()&OBJECT_TYPE_DRAWABLE)
	{
		CColorDialog diag(this);
		diag.m_color = &(((DrawableObject*)(sel))->m_backcolor);
		diag.ShowModal();
	}
}

void Dialog::On_Save( wxCommandEvent& event )
{
	wxFileDialog diag(this, wxT("Save"), wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if(diag.ShowModal()==wxID_OK)
	{
		C_File file(Game->GetResourceManager());
		file.Open(diag.GetPath().ToStdWstring().c_str(), BaseIO::Access_ALL);
		Game->GetMainContainer()->SaveAll(file);
		file.Close();
	}
}

void Dialog::On_Laad( wxCommandEvent& event )
{
	wxFileDialog diag(this, wxT("Load"), wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_OPEN);
	if(diag.ShowModal()==wxID_OK)
	{
		wstring s = diag.GetPath().ToStdWstring();
		sel = 0;
		Game->GetMainContainer()->SetEditable(false);
#ifdef USEBOX2DPHYSICS
		Game->GetPhysicsWorld()->SetUpdate(true);
		Game->GetPhysicsWorld()->ClearAllObjects();
#endif
		Game->GetMainContainer()->Clear();
		C_File file(Game->GetResourceManager());
		file.Open(s.c_str(), BaseIO::Access_ALL);
		loading = true;
		//obc->Load(file);
		Game->GetMainContainer()->LoadAll(file, *Game->GetBuilder(), true);
		file.Close();
		Game->ResetTimer();
		loading = false;
	}
}

void Dialog::Callback (Object* obj, unsigned int Msg, unsigned int Param1, void* Param2)
{
	if(loading) return;
	if(Msg==OBJECT_Message_SelEdit || Msg==OBJECT_Message_MouseUp)
	{
		PropertiesIO io;
		io.SetPropertyGridManager(m_propertyGridManager1);
		io.Open();
		m_propertyGridManager1->GetGrid()->Clear();
		Object* obj2 = Game->GetMainContainer()->GetEditorObjectRecursive();
		if(obj2==0) obj2 = Game->GetMainContainer();
		obj2->Save(io);
		sel = obj2;
		io.Close();
	}
	else if(Msg==OBJECT_Message_SelClear)
	{
		PropertiesIO io;
		io.SetPropertyGridManager(m_propertyGridManager1);
		io.Open();
		m_propertyGridManager1->GetGrid()->Clear();

		if(obj!=0) obj->Save(io);
		sel = obj;
		io.Close();
	}
}

void Dialog::On_Create( wxCommandEvent& event )
{
	if(m_listBox1->GetSelection()<0) return;
	string str = m_listBox1->GetString(m_listBox1->GetSelection());
	if(sel!=0 && sel->GetType()&OBJECT_TYPE_CONTAINER)
	{
		((ObjectsContainer*)sel)->SetEditable(true);
		((ObjectsContainer*)sel)->Callback.RegisterHandler<Dialog, &Dialog::Callback>(this);
		((ObjectsContainer*)sel)->AddObject(Game->GetBuilder()->BuildObject(str.c_str()));
	}
	else
	{
		Game->GetMainContainer()->AddObject(Game->GetBuilder()->BuildObject(str.c_str()));
	}
	Game->GetMainContainer()->SetEditable(true);
#ifdef USEBOX2DPHYSICS
	Game->GetPhysicsWorld()->SetUpdate(false);
#endif
}

void Dialog::On_Copy( wxCommandEvent& event )
{
	if(sel==0) return;
	if(sel->GetType()&OBJECT_TYPE_CONTAINER) return;
	Object* obj = Game->GetBuilder()->BuildObject(sel->GetName().c_str());
	if(obj==0) return;
	PropertiesIO io(Game->GetResourceManager());
	io.SetPropertyGridManager(m_propertyGridManager1);
	io.Open();
	obj->Load(io);
	obj->m_parent = 0;
	Game->GetMainContainer()->AddObject(obj);
	Game->GetMainContainer()->SetEditable(true);
#ifdef USEBOX2DPHYSICS
	Game->GetPhysicsWorld()->SetUpdate(false);
#endif
}

void Dialog::On_Property(wxPropertyGridEvent& event)
{
	PropertiesIO io(Game->GetResourceManager());
	io.SetPropertyGridManager(m_propertyGridManager1);
	if(sel!=0)
	{
		sel->Load(io);
		ObjectsContainer* p = sel->GetParent();
		if(p!=0)
		{
			p->UpdateEditor();
		}
	}	
}

void Dialog::On_ReScript( wxCommandEvent& event )
{
#ifdef USESCRIPTING
	Game->m_script.Open(L"Script.as");
#endif
}

void Dialog::On_Update( wxCommandEvent& event )
{
	/*PropertiesIO io(&mgr);
	io.SetPropertyGridManager(m_propertyGridManager1);
	if(sel!=0)
	{
		sel->Load(io);
		ObjectsContainer* p = sel->GetParent();
		if(p!=0)
		{
			p->UpdateEditor();
		}
	}*/
	bool edt = !Game->GetMainContainer()->GetEditable();
	Game->GetMainContainer()->SetEditable(edt);
#ifdef USEBOX2DPHYSICS
	Game->GetPhysicsWorld()->SetUpdate(!edt);
#endif
	if(edt) Game->Pause();
	else Game->Unpause();
	/*C_File file(&mgr);
	file.Open(L"e:\\test.s", BaseIO::Access_Write);
	obc->SaveAll(file);
	file.Close();*/
}

void Dialog::OnClose(wxCloseEvent &event)
{
	app->loop = false;
	app->DisconnectEvt();
	if(cdiag!=0) delete cdiag;
	delete Game;
	Game = 0;
    Destroy();
}

void Dialog::On_console( wxCommandEvent& event )
{
	AllocConsole();

    HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
    int hCrt = _open_osfhandle((long) handle_out, _O_TEXT);
    FILE* hf_out = _fdopen(hCrt, "w");
    setvbuf(hf_out, NULL, _IONBF, 1);
    *stdout = *hf_out;

    HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
    hCrt = _open_osfhandle((long) handle_in, _O_TEXT);
    FILE* hf_in = _fdopen(hCrt, "r");
    setvbuf(hf_in, NULL, _IONBF, 128);
    *stdin = *hf_in;
}

void Dialog::ON_ScriptEdiitor( wxCommandEvent& event )
{
	if(cdiag->IsVisible())
	{
		cdiag->SetFocus();
	}
	else
	{
		cdiag->m_classes = Game->m_script.GetClassNames();
		cdiag->GenerateList(Game->m_script.GetFunctionNames(cdiag->m_objlist));
		Game->m_script.PopulateWithHelp(cdiag->m_help);
		cdiag->SetCode(Game->m_script.m_curcode);
		cdiag->Show();
	}
}

CColorDialog::CColorDialog(wxDialog *dlg) : ColorDialog(dlg)
{
	m_color = 0;
}

void CColorDialog::On_Update( wxScrollEvent& event )
{
	if(m_color!=0)
	{
		(*m_color) = ColorARGB32((unsigned char)m_Alpha->GetValue(), (unsigned char)m_Red->GetValue(), (unsigned char)m_Green->GetValue(), (unsigned char)m_Blue->GetValue());
	}
	wxString str;
	str << ColorARGB32((unsigned char)m_Alpha->GetValue(), (unsigned char)m_Red->GetValue(), (unsigned char)m_Green->GetValue(), (unsigned char)m_Blue->GetValue());
	m_textCtrl1->SetValue(str);
}

enum
{
    MARGIN_LINE_NUMBERS,
    MARGIN_FOLD
};
// Editor

void CScriptEditor::Notify2( wxTimerEvent& event )
{
	if(Game->m_script.Compile(m_editor->GetText(), false))
	{
		m_classes = Game->m_script.GetClassNames();
		GenerateList(Game->m_script.GetFunctionNames(m_objlist));
		Game->m_script.PopulateWithHelp(m_help);
	}
	//SetCode(Game->m_script.m_curcode);
	UpdateErrors();
	printf("notify");
}

CScriptEditor::CScriptEditor(wxDialog *dlg) : ScriptEditor(dlg)
{
	mousex = 0;
	mousey = 0;
	m_timer.SetOwner(this->GetEventHandler(), 247);
	m_timer2.SetOwner(this->GetEventHandler(), 249);
	this->Connect(m_timer.GetId(), wxEVT_TIMER, wxTimerEventHandler(CScriptEditor::Notify), NULL, this);
	this->Connect(m_timer2.GetId(), wxEVT_TIMER, wxTimerEventHandler(CScriptEditor::Notify2), NULL, this);
	//EVT_TIMER (247, wxTimerEventHandler(CScriptEditor::Notify));

	m_editor = new wxStyledTextCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, wxEmptyString );

	//m_editor->SetMarginType (3, wxSTC_MARGIN_SYMBOL);
 //   m_editor->SetMarginWidth(3, 14);
	//m_editor->StyleSetBackground(3, wxColor(255, 255, 255) );

	m_editor->SetMarginWidth (MARGIN_LINE_NUMBERS, 30);
    m_editor->StyleSetForeground (wxSTC_STYLE_LINENUMBER, wxColour (75, 75, 75) );
	m_editor->StyleSetBackground (wxSTC_STYLE_LINENUMBER, wxColour (220, 220, 220));
    m_editor->SetMarginType(MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER);

	m_editor->SetMarginType (MARGIN_FOLD, wxSTC_MARGIN_SYMBOL);
    m_editor->SetMarginWidth(MARGIN_FOLD, 14);
    m_editor->SetMarginMask (MARGIN_FOLD, wxSTC_MASK_FOLDERS);
	m_editor->StyleSetBackground(MARGIN_FOLD, wxColor(255, 255, 255) );
	m_editor->SetMarginSensitive(MARGIN_FOLD, true);

	// Properties found from http://www.scintilla.org/SciTEDoc.html
	m_editor->SetProperty (wxT("fold"),         wxT("1") );
	m_editor->SetProperty (wxT("fold.comment"), wxT("1") );
	m_editor->SetProperty (wxT("fold.compact"), wxT("1") );
	m_editor->SetProperty (wxT("fold.flags "),  wxT("30") );

	wxColor grey( 100, 100, 100 );
	m_editor->MarkerDefine (wxSTC_MARKNUM_FOLDER, wxSTC_MARK_CIRCLEPLUSCONNECTED );
	m_editor->MarkerSetForeground (wxSTC_MARKNUM_FOLDER, wxColor("white"));
	m_editor->MarkerSetBackground (wxSTC_MARKNUM_FOLDER, grey);

	m_editor->MarkerDefine (wxSTC_MARKNUM_FOLDEROPEN,    wxSTC_MARK_CIRCLEMINUSCONNECTED);
	m_editor->MarkerSetForeground (wxSTC_MARKNUM_FOLDEROPEN, wxColor("white"));
	m_editor->MarkerSetBackground (wxSTC_MARKNUM_FOLDEROPEN, grey);

	m_editor->MarkerDefine (wxSTC_MARKNUM_FOLDERSUB,     wxSTC_MARK_VLINE);
	m_editor->MarkerSetForeground (wxSTC_MARKNUM_FOLDERSUB, grey);
	m_editor->MarkerSetBackground (wxSTC_MARKNUM_FOLDERSUB, grey);

	m_editor->MarkerDefine (wxSTC_MARKNUM_FOLDEREND,     wxSTC_MARK_CIRCLEPLUSCONNECTED);
	m_editor->MarkerSetForeground (wxSTC_MARKNUM_FOLDEREND, wxColor("white"));
	m_editor->MarkerSetBackground (wxSTC_MARKNUM_FOLDEREND, grey);

	m_editor->MarkerDefine (wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_CIRCLEMINUSCONNECTED);
	m_editor->MarkerSetForeground (wxSTC_MARKNUM_FOLDEROPENMID, wxColor("white"));
	m_editor->MarkerSetBackground (wxSTC_MARKNUM_FOLDEROPENMID, grey);

	m_editor->MarkerDefine (wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_TCORNERCURVE);
	m_editor->MarkerSetForeground (wxSTC_MARKNUM_FOLDERMIDTAIL, grey);
	m_editor->MarkerSetBackground (wxSTC_MARKNUM_FOLDERMIDTAIL, grey);

	m_editor->MarkerDefine (wxSTC_MARKNUM_FOLDERTAIL,    wxSTC_MARK_LCORNERCURVE);
	m_editor->MarkerSetForeground (wxSTC_MARKNUM_FOLDERTAIL, grey);
	m_editor->MarkerSetBackground (wxSTC_MARKNUM_FOLDERTAIL, grey);
	// ---- End of code folding part

	m_editor->MarkerDefine (1, 22);
	m_editor->MarkerSetForeground (1, wxColour(255,105,100));
	m_editor->MarkerSetBackground (1, wxColour(255,105,100));
	//m_editor->MarkerSetAlpha(1, 255);
	m_editor->MarkerDefine (2, 22);
	m_editor->MarkerSetForeground (2, wxColour(255,200,100));
	m_editor->MarkerSetBackground (2, wxColour(255,200,100));

	m_editor->MarkerDefine (3, 22);
	m_editor->MarkerSetForeground (3, wxColour(100,155,255));
	m_editor->MarkerSetBackground (3, wxColour(100,155,255));
	

	m_editor->StyleSetSize(32, 12);
	wxFont fnt(12, wxFONTFAMILY_SCRIPT, 0, wxT("Calibri"));
	m_editor->StyleSetFont(32, fnt);

	m_editor->StyleClearAll();
	m_editor->SetLexer(wxSTC_LEX_CPP);
	m_editor->StyleSetForeground (wxSTC_C_STRING,            wxColour(150,0,0));
	m_editor->StyleSetForeground (wxSTC_C_PREPROCESSOR,      wxColour(165,105,0));
	m_editor->StyleSetForeground (wxSTC_C_IDENTIFIER,        wxColour(40,0,60));
	m_editor->StyleSetForeground (wxSTC_C_NUMBER,            wxColour(0,150,0));
	m_editor->StyleSetForeground (wxSTC_C_CHARACTER,         wxColour(150,0,0));
	m_editor->StyleSetForeground (wxSTC_C_WORD,              wxColour(0,150,200));
	m_editor->StyleSetForeground (wxSTC_C_WORD2,             wxColour(0,150,0));
	m_editor->StyleSetForeground (wxSTC_C_COMMENT,           wxColour(150,150,150));
	m_editor->StyleSetForeground (wxSTC_C_COMMENTLINE,       wxColour(150,150,150));
	m_editor->StyleSetForeground (wxSTC_C_COMMENTDOC,        wxColour(150,150,150));
	m_editor->StyleSetForeground (wxSTC_C_COMMENTDOCKEYWORD, wxColour(0,0,200));
	m_editor->StyleSetForeground (wxSTC_C_COMMENTDOCKEYWORDERROR, wxColour(0,0,200));
	m_editor->StyleSetBold(wxSTC_C_WORD, true);
	m_editor->StyleSetBold(wxSTC_C_WORD2, true);
	m_editor->StyleSetBold(wxSTC_C_COMMENTDOCKEYWORD, true);

	m_editor->StyleSetBold(wxSTC_STYLE_BRACELIGHT, true);
	m_editor->StyleSetBold(wxSTC_STYLE_BRACEBAD, true);

	//m_editor->StyleSetItalic(wxSTC_STYLE_BRACELIGHT, true);
	//m_editor->StyleSetItalic(wxSTC_STYLE_BRACEBAD, true);

	m_editor->StyleSetBackground (wxSTC_STYLE_BRACELIGHT, wxColour(230,230,230));
	m_editor->StyleSetBackground (wxSTC_STYLE_BRACEBAD, wxColour(230,230,230));

	m_editor->SetIndent(8);
	m_editor->SetTabIndents(true);
	m_editor->SetBackSpaceUnIndents(true);
	m_editor->SetIndentationGuides(true);
	m_editor->SetViewWhiteSpace(2);
	m_editor->AutoCompSetIgnoreCase(true);

	m_editor->AutoCompSetFillUps("(");

	// a sample list of keywords, I haven't included them all to keep it short...
	m_keys1 = wxT("return for while break continue class get set final super private interface mixin enum funcdef typedef namespace import if else switch case do is null and in true false inout or out override cast from shared default xor this");
	m_keys2 = wxT("const int int8 int16 int64 uint8 uint16 uint64 bool float void double uint");
	m_editor->SetKeyWords(0, m_keys1);
	m_editor->SetKeyWords(1, m_keys2);

	//Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(CScriptEditor::onClose), NULL, this);
	m_editor->Connect(wxEVT_STC_MARGINCLICK, wxStyledTextEventHandler(CScriptEditor::OnMarginClick), NULL, this);

	m_editor->Connect(wxEVT_STC_CHARADDED, wxStyledTextEventHandler(CScriptEditor::OnAddChar), NULL, this);
	m_editor->Connect(wxEVT_STC_CHANGE, wxStyledTextEventHandler(CScriptEditor::Changed), NULL, this);
	m_editor->Connect(wxEVT_STC_UPDATEUI, wxStyledTextEventHandler(CScriptEditor::UpdateUI), NULL, this);
	m_editor->Connect(wxEVT_STC_DWELLSTART, wxStyledTextEventHandler(CScriptEditor::OnMove), NULL, this);

	m_editor->SetMouseDwellTime(100);
	m_editor->SetModEventMask(3);
	//m_editor->Bind(wxEVT_MOTION, &CScriptEditor::OnMove, this);

	/*wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Editor : ") ), wxVERTICAL );*/
	wxSizer* sz = GetSizer();
	//sbSizer1->Add( (wxWindow*)m_editor, 1, wxALL | wxEXPAND, 5 );
	sz->Add((wxWindow*)m_editor, 1, wxALL | wxEXPAND, 5 );
	this->Layout();
}

void CScriptEditor::UpdateUI(wxStyledTextEvent &event)
{
	int pos = m_editor->GetCurrentPos();
	int c = m_editor->GetCharAt(pos);

	if(c=='}' || c==']' || c==')')
	{
		int match = m_editor->BraceMatch(pos);
		if(match!=-1)
		{
			m_editor->BraceHighlight(pos, match);
		}
		else
		{
			m_editor->BraceBadLight(pos);
		}
	}
	else
	{
		m_editor->BraceHighlight(-1, -1);
		m_editor->BraceBadLight(-1);
	}
}

int GetLeftOfWord(wxStyledTextCtrl* text, bool& dot)
{
	int pos = text->GetCurrentPos();
	int line = text->LineFromPosition(pos);
	int spos = text->PositionFromLine(line);

	int finddot = text->FindText(pos, spos, ".");
	int find = finddot;

	int find2 = text->FindText(pos, spos, "=");

	int find3 = text->FindText(pos, spos, "(");

	int find4 = text->FindText(pos, spos, " ");

	int find5 = text->FindText(pos, spos, "\t");

	if(find2!=1 && find < find2)
	{
		find = find2;
	}

	if(find3!=1 && find < find3)
	{
		find = find3;
	}

	if(find4!=1 && find < find4)
	{
		find = find4;
	}

	if(find5!=1 && find < find5)
	{
		find = find5;
	}

	if(find != -1)
	{
		if(find==finddot)
		{
			dot = true;
		}
		return pos - find - 1;
	}

	return pos - spos;
}

void AutoIndent(wxStyledTextCtrl* text)
{
	int pos = text->GetCurrentPos();
	int line = text->LineFromPosition(pos);
	int spos = text->PositionFromLine(line);
	int pline = line - 1;
	
	if(pline < 0)
	{
		return;
	}

	int indent = text->GetLineIndentation(pline);
	int ind = 0;
	
	int find = text->FindText(pos, spos, "}");
	if(find == -1)
	{
		int pend = text->GetLineEndPosition(line);
		int pstart = text->PositionFromLine(pline);
		find = text->FindText(pstart, pend, "{");
		if(find != -1)
		{
			text->SetLineIndentation(line, indent + text->GetIndent());
			ind = text->GetLineEndPosition(line);
			text->SetSelection(ind, ind);
		}
		else
		{
			text->SetLineIndentation(line, indent);
			ind = text->GetLineEndPosition(line);
			text->SetSelection(ind, ind);
		}
	}
	else
	{
		text->SetLineIndentation(line, indent);
		ind = text->GetLineEndPosition(line);
		text->SetSelection(ind, ind);
	}
}

wxString GetPrevWord(wxStyledTextCtrl* text)
{
	int pos = text->GetCurrentPos();
	int line = text->LineFromPosition(pos);
	int spos = text->PositionFromLine(line);	

	int finddot = text->FindText(pos, spos, ".");
	int find = finddot;

	int find2 = text->FindText(pos-1, spos, "=");

	int find3 = text->FindText(pos-1, spos, "(");

	int find4 = text->FindText(pos-1, spos, " ");

	int find5 = text->FindText(pos-1, spos, "\t");

	if(find2!=1 && find < find2)
	{
		find = find2;
	}

	if(find3!=1 && find < find3)
	{
		find = find3;
	}

	if(find4!=1 && find < find4)
	{
		find = find4;
	}

	if(find5!=1 && find < find5)
	{
		find = find5;
	}

	if(find != -1)
	{
		return text->GetTextRange(find+1, pos - 1);
	}

	return text->GetTextRange(spos, pos - 1);
}

void CScriptEditor::OnMarginClick(wxStyledTextEvent &event)
{
	if (event.GetMargin() == MARGIN_FOLD)
	{
		int lineClick = m_editor->LineFromPosition(event.GetPosition());
		int levelClick = m_editor->GetFoldLevel(lineClick);

		if ((levelClick & wxSTC_FOLDLEVELHEADERFLAG) > 0)
		{
			m_editor->ToggleFold(lineClick);
		}
	}
}

void CScriptEditor::Changed(wxStyledTextEvent &event)
{
	if(m_autocheck->IsChecked())
	{
		m_timer2.Stop();
		m_timer2.Start(2000, true);
	}
}

void CScriptEditor::OnAddChar(wxStyledTextEvent &event)
{
	int key = event.GetKey();
	int pos = m_editor->GetCurrentPos();
	int line = m_editor->LineFromPosition(pos);
	m_editor->MarkerDelete(line, 1);
	m_editor->MarkerDelete(line, 2);
	m_editor->MarkerDelete(line, 3);

	if(key == 13)
	{
		AutoIndent(m_editor);
	}
	else if (key == '}')
	{
		int indent = m_editor->GetLineIndentation(line);
		/*
		int spos = m_editor->PositionFromLine(line);
		int c = m_editor->GetCharAt(spos + indent - 1);
		
		if(c=='}' || c==0)
		{*/
			m_editor->SetLineIndentation(line, indent - m_editor->GetIndent());
			int ind = m_editor->GetLineEndPosition(line);
			m_editor->SetSelection(ind, ind);
		/*}*/
	}
	else if (key == '(')
	{
		wxString st = GetPrevWord(m_editor);
		map<string, string>::iterator it =  m_help.find(st.ToStdString());
		if(it!=m_help.end())
		{
			int pos = m_editor->GetCurrentPos();
			m_editor->CallTipShow(pos, it->second);
		}
	}
	else if (key == '.')
	{
		m_editor->AutoCompShow(0, m_list2);
	}
	else
	{
		bool dot = false;
		int fnd = GetLeftOfWord(m_editor, dot);
		if(fnd>0) 
		{
			if(!dot) m_editor->AutoCompShow(fnd, m_list);
			else m_editor->AutoCompShow(fnd, m_list2);
		}
	}
}

bool icompstr(const string& s1, const string& s2)
{
	int i = strcmpi(s1.c_str(), s2.c_str());
	if(i<0) return true;
	return false;
}

void CScriptEditor::GenerateList(string& str)
{
	string tmplist;
	tmplist = str;
	tmplist += " ";
	tmplist += m_keys1;
	tmplist += " ";
	tmplist += m_keys2;

	vector<string> arr;
	GetStringArray(arr, tmplist.c_str(), ' ');
	sort(arr.begin(), arr.end(), icompstr);

	m_list = "";
	for (unsigned int i = 0; i < arr.size(); i++)
	{
		if(i!=0) m_list += wxT(" ");
		m_list += arr[i];
	}

	//sort(m_objlist.begin(), m_objlist.end(), icompstr);
	m_list2 = "";
	for (set<string, icompstrs>::iterator i = m_objlist.begin(); i != m_objlist.end(); i++)
	{
		if(i!=m_objlist.begin()) m_list2 += wxT(" ");
		m_list2 += (*i);
	}

	string kys = m_keys2;
	kys += " ";
	kys += m_classes;
	m_editor->SetKeyWords(1, kys);
}

void CScriptEditor::SetCode(string& str)
{
	m_editor->SetText(str);
}

void CScriptEditor::On_Save( wxCommandEvent& event )
{
	wxFileDialog diag(this, wxT("Save"), wxEmptyString, wxEmptyString, "Script files (*.as)|*.as|Compiled Script files (*.asc)|*.asc|All Files (*.*)|*.*" , wxFD_SAVE);
	if(diag.ShowModal()==wxID_OK)
	{
		if(diag.GetFilterIndex() == 1)
		{
			asIScriptModule* mod = Game->m_script.GetModule();
			if(mod!=0)
			{
				wstring s = diag.GetPath().ToStdWstring();
				string str = m_editor->GetText();
				C_File file(Game->GetResourceManager());
				file.Open(s.c_str(), BaseIO::Access_ALL);
				BinaryStream bs(&file);
				mod->SaveByteCode(&bs);
				file.Close();
			}
		}
		else
		{
			wstring s = diag.GetPath().ToStdWstring();
			string str = m_editor->GetText();
			C_File file(Game->GetResourceManager());
			file.Open(s.c_str(), BaseIO::Access_ALL);
			unsigned int sz = str.size();
			file.Write((void*)str.c_str(), &sz);
			file.Close();
		}
	}
}

void CScriptEditor::On_Load( wxCommandEvent& event )
{
	wxFileDialog diag(this, wxT("Load"), wxEmptyString, wxEmptyString, "Script files (*.as)|*.as|All Files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if(diag.ShowModal()==wxID_OK)
	{
		wstring s = diag.GetPath().ToStdWstring();

		char* tmp = new char[4096];
		C_File file;

		if(!file.Open(s.c_str(), BaseIO::Access_Read))
		{
			delete [] tmp;
			return;
		}

		string m_curcode = "";
		unsigned int sz = 0;
		while(1)
		{
			sz = 4095;
			if(!file.Read((void*)tmp, &sz))
			{
				break;
			}
			tmp[sz] = '\0';
			m_curcode += tmp;

			if(sz==0)
			{
				break;
			}
		}

		file.Close();
		delete [] tmp;

		if(Game->m_script.Compile(m_curcode.c_str(), true))
		{
			m_classes = Game->m_script.GetClassNames();
			GenerateList(Game->m_script.GetFunctionNames(m_objlist));
			Game->m_script.PopulateWithHelp(m_help);
		}
		SetCode(Game->m_script.m_curcode);
		UpdateErrors();
	}
}

void CScriptEditor::On_Compile( wxCommandEvent& event )
{
	if(Game->m_script.Compile(m_editor->GetText(), true))
	{
		m_classes = Game->m_script.GetClassNames();
		GenerateList(Game->m_script.GetFunctionNames(m_objlist));
		Game->m_script.PopulateWithHelp(m_help);
	}
	//SetCode(Game->m_script.m_curcode);
	UpdateErrors();
}

void CScriptEditor::On_New( wxCommandEvent& event )
{
	m_editor->SetText("void main()\n{\n\t\n}\n\nvoid callback(Object@ obj, uint Msg, uint Param1)\n{\n\t\n}");
	if(Game->m_script.Compile(m_editor->GetText(), false))
	{
		m_classes = Game->m_script.GetClassNames();
		GenerateList(Game->m_script.GetFunctionNames(m_objlist));
		Game->m_script.PopulateWithHelp(m_help);
	}
	SetCode(Game->m_script.m_curcode);
	UpdateErrors();
}

void CScriptEditor::OnMove( wxStyledTextEvent& event )
{
	m_timer.Stop();
	m_timer.Start(100, true);
	if(m_editor->CallTipActive()) m_editor->CallTipCancel();
	mousex = event.GetX();
	mousey = event.GetY();
	//m_editor->HandleMouseMove(mousex, mousey, 0);
}

void CScriptEditor::UpdateErrors()
{
	m_editor->MarkerDeleteAll(1);
	m_editor->MarkerDeleteAll(2);
	m_editor->MarkerDeleteAll(3);
	for(map<int, ScriptError>::iterator i = Game->m_script.errors.begin(); i != Game->m_script.errors.end(); i++)
	{
		if(i->second.type == asMSGTYPE_ERROR) m_editor->MarkerAdd(i->second.row - 1, 1);
		else if(i->second.type == asMSGTYPE_WARNING) m_editor->MarkerAdd(i->second.row - 1, 2);
		else m_editor->MarkerAdd(i->second.row - 1, 3);
	}
}

void CScriptEditor::Notify( wxTimerEvent& event )
{
	int pos = m_editor->PositionFromPointClose(mousex, mousey);
	if(pos!=-1)
	{
		int line = m_editor->LineFromPosition(pos);

		map<int, ScriptError>::iterator i = Game->m_script.errors.find(line + 1);
		if(i != Game->m_script.errors.end())
		{
			
			int spos = m_editor->PositionFromLine(line);
			if(m_editor->CallTipActive()) m_editor->CallTipCancel();
			wxString st;
			st = i->second.message;
			st += wxT(" (col:");
			st << i->second.col;
			st += wxT(")");
			m_editor->CallTipShow(spos, st);
		}
		else
		{
			int wstart = m_editor->WordStartPosition(pos, true);
			int wend = m_editor->WordEndPosition(pos, true);
			wxString txt = m_editor->GetTextRange(wstart, wend);
			map<string, string>::iterator it =  m_help.find(txt.ToStdString());
			if(it!=m_help.end())
			{
				if(m_editor->CallTipActive()) m_editor->CallTipCancel();
				m_editor->CallTipShow(wstart, it->second);
			}
		}
	}
}