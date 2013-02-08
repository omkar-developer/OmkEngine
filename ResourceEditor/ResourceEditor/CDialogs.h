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

#include "IDialogs.h"
#include <string>
#include <stdio.h>
#include <wx\dir.h>

using namespace std;
#include "Variant.h"

struct datatype
{
	wxString name;
	int group;
	int type;
	wxString filename;
};

struct RectangleI
{
	int left;
	int top;
	int right;
	int bottom;

	RectangleI();
	RectangleI(int left, int top, int right, int bottom);
};

struct SpriteFrame
{
	float delay;
	RectangleI rect;

	SpriteFrame()
	{
		delay = 0.0f;
		rect.left = 0;
		rect.top = 0;
		rect.right = 0;
		rect.bottom = 0;
	}

	SpriteFrame(float delay, RectangleI& rect)
	{
		this->delay = delay;
		this->rect = rect;
	}

	SpriteFrame(float delay, int left, int top, int right, int bottom)
	{
		this->delay = delay;
		this->rect.left = left;
		this->rect.top = top;
		this->rect.right = right;
		this->rect.bottom = bottom;
	}
};

class Dialog: public IDialog
{
    public:
        Dialog(wxDialog *dlg);
		wxVector<datatype> data;

    protected:
        virtual void OnClose(wxCloseEvent& event);

		virtual void OnPickFile( wxCommandEvent& event );
		virtual void OnListItemSelected( wxListEvent& event );
		virtual void On_Save( wxCommandEvent& event );
		virtual void On_Load( wxCommandEvent& event );
		virtual void On_Quit( wxCommandEvent& event );

		virtual void on_Add( wxCommandEvent& event );
		virtual void On_Modify( wxCommandEvent& event );
		virtual void On_Delete( wxCommandEvent& event );

		virtual void On_ClearAll( wxCommandEvent& event );
		virtual void On_AutoCreate( wxCommandEvent& event );
		void AutoFillDir(wxString name);

		void UpdateList();
};

class CSpriteEditor : public SpriteEditor
{
    public:
        CSpriteEditor(wxDialog *dlg);

    protected:
		wxVector<SpriteFrame> frames;
		wxVector<Variant> Indexes;

		virtual void On_isetAdd( wxCommandEvent& event );
		virtual void ON_IsetModify( wxCommandEvent& event );
		virtual void On_IsetDelete( wxCommandEvent& event );
		virtual void On_AnAdd( wxCommandEvent& event );
		virtual void On_anModify( wxCommandEvent& event );
		virtual void On_AnDelete( wxCommandEvent& event );
		virtual void On_Save( wxCommandEvent& event );
		virtual void On_Load( wxCommandEvent& event );

		void UpdateISetList();
		void UpdateAnimList();
};