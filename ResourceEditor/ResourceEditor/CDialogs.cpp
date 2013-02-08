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
#include <wx/filedlg.h>
#include <fstream>

RectangleI::RectangleI()
{
	left=top=right=bottom=0;
}

RectangleI::RectangleI(int left, int top, int right, int bottom)
{
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
}

Dialog::Dialog(wxDialog *dlg)
    : IDialog(dlg)
{
	m_listCtrl1->InsertColumn(0, wxT("Name"), 0, 100);
	m_listCtrl1->InsertColumn(1, wxT("Group"));
	wxListItem itm;
	m_listCtrl1->GetColumn(1, itm);
	itm.SetWidth(50);
	m_listCtrl1->SetColumn(1, itm);
	m_listCtrl1->InsertColumn(2, wxT("Type"));	
	m_listCtrl1->GetColumn(2, itm);
	itm.SetWidth(50);
	m_listCtrl1->SetColumn(2, itm);
	m_listCtrl1->InsertColumn(3, wxT("Filename"), 0, 200);
}

void Dialog::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void Dialog::UpdateList()
{
	m_listCtrl1->DeleteAllItems();
	for(long i=0; i<data.size(); i++)
	{
		m_listCtrl1->InsertItem(m_listCtrl1->GetItemCount(), data[i].name);
		m_listCtrl1->SetItem(i, 1, wxString::FromDouble(data[i].group));
		m_listCtrl1->SetItem(i, 2, wxString::FromDouble(data[i].type));
		m_listCtrl1->SetItem(i, 3, data[i].filename);
	}
}

void Dialog::OnPickFile( wxCommandEvent& event )
{
	wxFileDialog diag(this, wxT("Select resource"), wxT("."), wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if(diag.ShowModal()==wxID_OK)
	{
		wxFileName name = diag.GetPath();
		name.MakeRelativeTo();
		wxString path = name.GetFullPath();
		path.Replace(wxT("\\"), wxT("/"));
		res_filename->SetValue(path);
		wxString fn = name.GetName();
		if(name.GetExt().CmpNoCase(wxT("png"))==0)
		{
			res_name->SetValue(fn);
			res_type->SetSelection(1);
		}
		else if(name.GetExt().CmpNoCase(wxT("iset"))==0)
		{
			res_name->SetValue(fn);
			res_type->SetSelection(6);
		}
		else if(name.GetExt().CmpNoCase(wxT("ogg"))==0)
		{
			res_name->SetValue(fn);
			res_type->SetSelection(3);
		}
	}
}

void Dialog::On_ClearAll( wxCommandEvent& event )
{
	data.clear();
	UpdateList();
}

void Dialog::AutoFillDir(wxString dname)
{
	wxDir dir(dname);
	wxString n;
	bool count = 0;
	count = dir.GetFirst(&n);
	while(count)
	{
		if(n==wxT("."))
		{
			count = dir.GetNext(&n);
			continue;
		}
		wxFileName name = dname + wxT("\\") + n;
		if(wxDir::Exists(n))
		{
			AutoFillDir(dname + wxT("\\") + n);
		}
		else
		{
			name.MakeRelativeTo();
			wxString path = name.GetFullPath();
			path.Replace(wxT("\\"), wxT("/"));
			wxString fn = name.GetName();

			datatype d;
			d.filename = path;
			d.group = 0;
			d.name = fn;

			if(name.GetExt().CmpNoCase(wxT("png"))==0)
			{
				d.type = 1;
				bool found = false;
				for(unsigned int i = 0; i < data.size(); i++)
				{
					if(data[i].name == d.name || data[i].filename == d.filename)
					{
						found = true;
						break;
					}
				}
				if(!found) data.push_back(d);
			}
			else if(name.GetExt().CmpNoCase(wxT("iset"))==0)
			{
				d.type = 6;
				d.name += wxT("iset");
				bool found = false;
				for(unsigned int i = 0; i < data.size(); i++)
				{
					if(data[i].name == d.name || data[i].filename == d.filename)
					{
						found = true;
						break;
					}
				}
				if(!found) data.push_back(d);
			}
			else if(name.GetExt().CmpNoCase(wxT("ogg"))==0)
			{
				d.type = 3;
				bool found = false;
				for(unsigned int i = 0; i < data.size(); i++)
				{
					if(data[i].name == d.name || data[i].filename == d.filename)
					{
						found = true;
						break;
					}
				}
				if(!found) data.push_back(d);
			}
		}

		count = dir.GetNext(&n);
	}
}

void Dialog::On_AutoCreate( wxCommandEvent& event )
{
	//data.clear();
	AutoFillDir(wxGetCwd());
	UpdateList();
}

void Dialog::OnListItemSelected( wxListEvent& event )
{
	long l= 0;
	long sel = event.GetItem().GetId();
	res_filename->SetValue(m_listCtrl1->GetItemText(sel, 3));

	m_listCtrl1->GetItemText(sel, 1).ToLong(&l);
	res_group->SetValue(l);

	res_name->SetValue(m_listCtrl1->GetItemText(sel, 0));

	l = 0;
	m_listCtrl1->GetItemText(sel, 2).ToLong(&l);
	res_type->SetSelection(l);
}

void Dialog::On_Save( wxCommandEvent& event )
{
	wxFileDialog diag(this, wxT("Save"), wxT("."), wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_SAVE);
	if(diag.ShowModal()==wxID_OK)
	{
		wxString str = diag.GetPath();
		const wchar_t* fpath = str.wc_str();
		ofstream fs(fpath, fstream::binary);
		Variant count = data.size();
		count >> fs;

		Variant name;
		Variant path;
		Variant type;
		Variant group;

		for(unsigned int i=0; i<static_cast<unsigned int>(count); i++)
		{
			name = data[i].name.ToStdString().c_str();
			path = data[i].filename.wc_str();
			type = data[i].type;
			group = data[i].group;

			string nm = name;
			wstring pth = path;

			if(nm=="test")
			{
				int s = 0;
			}

			name >> fs;
			path >> fs;
			type >> fs;
			group >> fs;
		}

		fs.close();
	}
}

void Dialog::On_Load( wxCommandEvent& event )
{
	wxFileDialog diag(this, wxT("Load"), wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_OPEN);
	if(diag.ShowModal()==wxID_OK)
	{
		data.clear();
		wxString str = diag.GetPath();
		const wchar_t* fpath = str.wc_str();
		ifstream fs;
		fs.open(fpath, ios::binary);
		Variant count;
		count << fs;

		Variant name;
		Variant path;
		Variant type;
		Variant group;

		for(unsigned int i=0; i<static_cast<unsigned int>(count); i++)
		{
			name << fs;
			path.Read(&fs);
			type << fs;
			group << fs;

			string nm = name;
			wstring pth = path;

			if(nm=="test")
			{
				int s = 0;
			}

			datatype d;
			d.filename = static_cast<wstring>(path);
			d.group = group;
			d.name = static_cast<string>(name);
			d.type = type;
			data.push_back(d);
		}

		fs.close();
		UpdateList();
	}
}

void Dialog::On_Quit( wxCommandEvent& event )
{
	CSpriteEditor editor(this);
	editor.ShowModal();
}

void Dialog::on_Add( wxCommandEvent& event )
{
	datatype d;
	d.filename = res_filename->GetValue();
	d.group = res_group->GetValue();
	d.name = res_name->GetValue();
	d.type = res_type->GetSelection();
	data.push_back(d);
	UpdateList();
}

void Dialog::On_Modify( wxCommandEvent& event )
{
	if(m_listCtrl1->GetSelectedItemCount() == 0) return;
	long index = -1;
	index = m_listCtrl1->GetNextItem(index, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	if(index != -1)
	{
		datatype& d = data[index];
		d.filename = res_filename->GetValue();
		d.group = res_group->GetValue();
		d.name = res_name->GetValue();
		d.type = res_type->GetSelection();
	}
	UpdateList();
}

void Dialog::On_Delete( wxCommandEvent& event )
{
	if(m_listCtrl1->GetSelectedItemCount() == 0) return;
	long index = -1;
	index = m_listCtrl1->GetNextItem(index, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	long dif = 0;
	while(index != -1)
	{
		data.erase(data.begin() + index - dif);
		++dif;
		index = m_listCtrl1->GetNextItem(index, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	}
	UpdateList();
}


CSpriteEditor::CSpriteEditor(wxDialog *dlg)
    : SpriteEditor(dlg)
{

}

void CSpriteEditor::UpdateISetList()
{
	iset_list->Clear();
	for(long i=0; i<frames.size(); i++)
	{
		wxString str;
		str << wxT("Delay:");
		str << frames[i].delay;
		str << wxT(" Rectangle(");
		str << frames[i].rect.left;
		str << wxT(",");
		str << frames[i].rect.top;
		str << wxT(",");
		str << frames[i].rect.right;
		str << wxT(",");
		str << frames[i].rect.bottom;
		str << wxT(")");
		iset_list->AppendString(str);
	}
}

void CSpriteEditor::UpdateAnimList()
{
	an_list->Clear();
	for(long i=0; i<Indexes.size(); i++)
	{
		an_list->AppendString(static_cast<const char*>(Indexes[i]));
	}
}

void CSpriteEditor::On_isetAdd( wxCommandEvent& event )
{
	SpriteFrame frame;
	long l; 
	double d;
	iset_left->GetValue().ToLong(&l);
	frame.rect.left = l;
	iset_top->GetValue().ToLong(&l);
	frame.rect.top = l;
	iset_right->GetValue().ToLong(&l);
	frame.rect.right = l;
	iset_bottom->GetValue().ToLong(&l);
	frame.rect.bottom = l;
	iset_delay->GetValue().ToDouble(&d);
	frame.delay = d;

	frames.push_back(frame);
	UpdateISetList();
}

void CSpriteEditor::ON_IsetModify( wxCommandEvent& event )
{
	if(iset_list->GetSelection() == -1) return;
	long index = -1;
	index = iset_list->GetSelection();
	if(index != -1)
	{
		SpriteFrame& frame = frames[index];
		long l; 
		double d;
		iset_left->GetValue().ToLong(&l);
		frame.rect.left = l;
		iset_top->GetValue().ToLong(&l);
		frame.rect.top = l;
		iset_right->GetValue().ToLong(&l);
		frame.rect.right = l;
		iset_bottom->GetValue().ToLong(&l);
		frame.rect.bottom = l;
		iset_delay->GetValue().ToDouble(&d);
		frame.delay = d;
	}
	UpdateISetList();
}

void CSpriteEditor::On_IsetDelete( wxCommandEvent& event )
{
	if(iset_list->GetSelection() == -1) return;
	long index = -1;
	index = iset_list->GetSelection();
	if(index != -1)
	{
		frames.erase(frames.begin() + index);
	}
	UpdateISetList();
}

void CSpriteEditor::On_AnAdd( wxCommandEvent& event )
{
	wxString str2 = an_data->GetValue();
	Variant str = str2.ToStdString().c_str();
	Indexes.push_back(str);
	UpdateAnimList();
}

void CSpriteEditor::On_anModify( wxCommandEvent& event )
{
	if(an_list->GetSelection() == -1) return;
	long index = -1;
	index = an_list->GetSelection();
	if(index != -1)
	{
		Variant& var = Indexes[index];
		wxString str2 = an_data->GetValue();
		var = str2.ToStdString().c_str();
	}
	UpdateAnimList();
}

void CSpriteEditor::On_AnDelete( wxCommandEvent& event )
{
	if(an_list->GetSelection() == -1) return;
	long index = -1;
	index = an_list->GetSelection();
	if(index != -1)
	{
		Indexes.erase(Indexes.begin() + index);
	}
	UpdateAnimList();
}

void CSpriteEditor::On_Save( wxCommandEvent& event )
{
	wxFileDialog diag(this, wxT("Save"), wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_SAVE);
	if(diag.ShowModal()==wxID_OK)
	{
		Variant str, str2;
		bool autogen = false;
		bool good = false;

		wxString str3 = diag.GetPath();
		const wchar_t* fpath = str3.wc_str();

		ofstream fs(fpath, fstream::binary);
		str = m_ResName->GetValue().ToStdString().c_str();
		autogen = m_AutoGen->GetValue();
		str >> fs;
		fs.write((char*)&autogen, sizeof(bool));

		for(int i=0; i<frames.size(); i++)
		{
			fs.write((char*)&frames[i].delay, sizeof(float));
			fs.write((char*)&frames[i].rect, sizeof(RectangleI));
			good = true;
		}

		SpriteFrame frame;
		frame.rect = RectangleI(0,0,0,0);
		frame.delay = 0;
		fs.write((char*)&frame.delay, sizeof(float));
		fs.write((char*)&frame.rect, sizeof(RectangleI));

		for(int i=0; i<Indexes.size(); i++)
		{
			Indexes[i] >> fs;
		}

		fs.close();
	}
}

void CSpriteEditor::On_Load( wxCommandEvent& event )
{
	wxFileDialog diag(this, wxT("Load"), wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_OPEN);
	if(diag.ShowModal()==wxID_OK)
	{
		Indexes.clear();
		frames.clear();
		Variant str, str2;
		bool autogen = false;
		bool good = false;
		SpriteFrame frame;

		wxString str3 = diag.GetPath();
		const wchar_t* fpath = str3.wc_str();

		fstream fs(fpath, ios::in | ios::binary);
		str << fs;
		fs.read((char*)&autogen, sizeof(bool));

		m_ResName->SetValue((const char*)str);
		m_AutoGen->SetValue(autogen);

		while(!fs.eof())
		{
			fs.read((char*)&frame.delay, sizeof(float));
			fs.read((char*)&frame.rect, sizeof(RectangleI));
			good = true;
			if(frame.delay == 0 && frame.rect.left == 0 && frame.rect.right == 0 && frame.rect.top == 0 && frame.rect.bottom == 0)
			{
				break;
			}
			else
			{
				frames.push_back(frame);
			}
		}

		while(!fs.eof())
		{
			str2.ClearString();
			str2 << fs;
			Indexes.push_back(str2);
		}

		fs.close();
		UpdateAnimList();
		UpdateISetList();
	}
}