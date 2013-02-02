///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 10 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __IDIALOGS_H__
#define __IDIALOGS_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/listbox.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/manager.h>
#include <wx/propgrid/advprops.h>
#ifdef __VISUALC__
#include <wx/link_additions.h>
#endif //__VISUALC__
#include <wx/dialog.h>
#include <wx/stattext.h>
#include <wx/slider.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class IDialog
///////////////////////////////////////////////////////////////////////////////
class IDialog : public wxDialog 
{
	private:
	
	protected:
		enum
		{
			ID_DIALOG = 1000
		};
		
		wxListBox* m_listBox1;
		wxButton* m_button61;
		wxButton* m_button71;
		wxButton* m_button8;
		wxButton* m_button1;
		wxButton* m_button3;
		wxButton* m_button7;
		wxButton* m_button5;
		wxButton* m_button6;
		wxButton* m_button9;
		wxButton* m_button10;
		wxButton* m_button11;
		wxButton* m_button19;
		wxButton* m_button12;
		wxButton* m_button13;
		wxButton* m_button14;
		wxPropertyGridManager* m_propertyGridManager1;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void On_SetPath( wxCommandEvent& event ) { event.Skip(); }
		virtual void On_Reset( wxCommandEvent& event ) { event.Skip(); }
		virtual void On_SetColor( wxCommandEvent& event ) { event.Skip(); }
		virtual void On_Create( wxCommandEvent& event ) { event.Skip(); }
		virtual void On_Update( wxCommandEvent& event ) { event.Skip(); }
		virtual void On_Delete( wxCommandEvent& event ) { event.Skip(); }
		virtual void On_Save( wxCommandEvent& event ) { event.Skip(); }
		virtual void On_Laad( wxCommandEvent& event ) { event.Skip(); }
		virtual void On_Copy( wxCommandEvent& event ) { event.Skip(); }
		virtual void On_ReScript( wxCommandEvent& event ) { event.Skip(); }
		virtual void On_DDraw( wxCommandEvent& event ) { event.Skip(); }
		virtual void On_console( wxCommandEvent& event ) { event.Skip(); }
		virtual void On_ShowAll( wxCommandEvent& event ) { event.Skip(); }
		virtual void On_Hide( wxCommandEvent& event ) { event.Skip(); }
		virtual void ON_ScriptEdiitor( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		IDialog( wxWindow* parent, wxWindowID id = ID_DIALOG, const wxString& title = wxT("Tools"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 313,637 ), long style = wxCAPTION|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU ); 
		~IDialog();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class m_drawarea
///////////////////////////////////////////////////////////////////////////////
class m_drawarea : public wxDialog 
{
	private:
	
	protected:
	
	public:
		
		m_drawarea( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 640,480 ), long style = wxDEFAULT_DIALOG_STYLE|wxWANTS_CHARS ); 
		~m_drawarea();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ColorDialog
///////////////////////////////////////////////////////////////////////////////
class ColorDialog : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText1;
		wxSlider* m_Alpha;
		wxStaticText* m_staticText11;
		wxSlider* m_Red;
		wxStaticText* m_staticText12;
		wxSlider* m_Green;
		wxStaticText* m_staticText13;
		wxSlider* m_Blue;
		wxTextCtrl* m_textCtrl1;
		
		// Virtual event handlers, overide them in your derived class
		virtual void On_Update( wxScrollEvent& event ) { event.Skip(); }
		
	
	public:
		
		ColorDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Color"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 554,229 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~ColorDialog();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ScriptEditor
///////////////////////////////////////////////////////////////////////////////
class ScriptEditor : public wxDialog 
{
	private:
	
	protected:
		wxButton* m_button15;
		wxButton* m_button16;
		wxButton* m_button17;
		wxButton* m_button18;
		wxCheckBox* m_autocheck;
		
		// Virtual event handlers, overide them in your derived class
		virtual void On_Save( wxCommandEvent& event ) { event.Skip(); }
		virtual void On_Load( wxCommandEvent& event ) { event.Skip(); }
		virtual void On_Compile( wxCommandEvent& event ) { event.Skip(); }
		virtual void On_New( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		ScriptEditor( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("ScriptEditor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 630,518 ), long style = wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxRESIZE_BORDER ); 
		~ScriptEditor();
	
};

#endif //__IDIALOGS_H__
