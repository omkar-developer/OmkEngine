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

#include "wx/wxprec.h"
 
#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#include "IDialogs.h"
#include <wx/stc/stc.h>
#include <stdafx.h>

#define wxSCI_INDIC0_MASK 0x20
#define wxSCI_INDIC1_MASK 0x40
#define wxSCI_INDIC2_MASK 0x80
#define wxSCI_INDICS_MASK 0xE0

class MainApp: public wxApp // MainApp is the class for our application 
{
	// MainApp just acts as a container for the window, or frame in MainFrame
public:
	bool loop;
	virtual bool OnInit(); 
	void onIdle(wxIdleEvent& evt);
	void DisconnectEvt();
};
 

DECLARE_APP(MainApp)

 // A macro that tells wxWidgets to create an instance of our application 

class CScriptEditor;

class Dialog: public IDialog
{
    public:
        Dialog(wxDialog *dlg);
		MainApp *app;
		void Callback (Object* obj, unsigned int Msg, unsigned int Param1, void* Param2);
		Object* sel;
		bool loading;

		CScriptEditor* cdiag;

    private:
        virtual void OnClose(wxCloseEvent& event);
		virtual void On_Create( wxCommandEvent& event );
		virtual void On_Update( wxCommandEvent& event );
		void On_Property(wxPropertyGridEvent& event);
		virtual void OnMDown( wxMouseEvent& event );

		void On_Delete( wxCommandEvent& event );
		void On_Save( wxCommandEvent& event );
		void On_Laad( wxCommandEvent& event );

		void On_SetPath( wxCommandEvent& event );
		void On_Reset( wxCommandEvent& event );
		virtual void On_SetColor( wxCommandEvent& event );
		virtual void On_Copy( wxCommandEvent& event );

		virtual void On_ReScript( wxCommandEvent& event );
		virtual void On_DDraw( wxCommandEvent& event );

		virtual void On_ShowAll( wxCommandEvent& event );
		virtual void On_Hide( wxCommandEvent& event );

		virtual void ON_ScriptEdiitor( wxCommandEvent& event );
		void On_console( wxCommandEvent& event );
};


class CColorDialog : public ColorDialog
{
    public:
        CColorDialog(wxDialog *dlg);
		virtual void On_Update( wxScrollEvent& event );
		unsigned int* m_color;        
};

bool icompstr(const string& s1, const string& s2);

class CScriptEditor : public ScriptEditor
{
private:
	wxTimer m_timer;
	wxTimer m_timer2;
	wxString m_keys1;
	wxString m_keys2;
	wxStyledTextCtrl* m_editor;
	void OnMarginClick(wxStyledTextEvent &event);
	void OnAddChar(wxStyledTextEvent &event);
	float mousex, mousey;

public:
	map<string, string> m_help;
	set<string, icompstrs> m_objlist;
	wxString m_list;
	wxString m_list2;
	wxString m_classes;
	void GenerateList(string& str);
	CScriptEditor(wxDialog *dlg);
	void SetCode(string& str);

	virtual void On_Save( wxCommandEvent& event );
	virtual void On_Load( wxCommandEvent& event );
	virtual void On_Compile( wxCommandEvent& event );
	virtual void On_New( wxCommandEvent& event );

	void Changed(wxStyledTextEvent &event);
	void UpdateUI(wxStyledTextEvent &event);

	void OnMove( wxStyledTextEvent& event );
	void Notify( wxTimerEvent& event );
	void Notify2( wxTimerEvent& event );

	void UpdateErrors();
};