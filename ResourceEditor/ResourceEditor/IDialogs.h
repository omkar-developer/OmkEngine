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
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/listctrl.h>
#include <wx/statbox.h>
#include <wx/dialog.h>
#include <wx/listbox.h>
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
			ID_DIALOG = 1000,
			ID_RES_NAME,
			ID_RES_GROUP,
			ID_RES_TYPE,
			ID_BSAVE,
			ID_BLOAD,
			ID_BQUIT
		};
		
		wxStaticText* m_staticText1;
		wxTextCtrl* res_name;
		wxStaticText* m_staticText2;
		wxSpinCtrl* res_group;
		wxStaticText* m_staticText3;
		wxChoice* res_type;
		wxStaticText* m_staticText5;
		wxTextCtrl* res_filename;
		wxButton* m_button4;
		wxButton* m_button5;
		wxButton* m_button6;
		wxButton* m_button7;
		wxButton* m_button17;
		wxListCtrl* m_listCtrl1;
		wxButton* m_button1;
		wxButton* m_button2;
		wxButton* m_button3;
		wxButton* m_button16;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnPickFile( wxCommandEvent& event ) { event.Skip(); }
		virtual void on_Add( wxCommandEvent& event ) { event.Skip(); }
		virtual void On_Modify( wxCommandEvent& event ) { event.Skip(); }
		virtual void On_Delete( wxCommandEvent& event ) { event.Skip(); }
		virtual void On_AutoCreate( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnListItemSelected( wxListEvent& event ) { event.Skip(); }
		virtual void On_Save( wxCommandEvent& event ) { event.Skip(); }
		virtual void On_Load( wxCommandEvent& event ) { event.Skip(); }
		virtual void On_Quit( wxCommandEvent& event ) { event.Skip(); }
		virtual void On_ClearAll( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		IDialog( wxWindow* parent, wxWindowID id = ID_DIALOG, const wxString& title = wxT("Resource Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 405,412 ), long style = wxCAPTION|wxCLOSE_BOX|wxDEFAULT_DIALOG_STYLE|wxSYSTEM_MENU ); 
		~IDialog();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class SpriteEditor
///////////////////////////////////////////////////////////////////////////////
class SpriteEditor : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText5;
		wxTextCtrl* iset_left;
		wxStaticText* m_staticText6;
		wxTextCtrl* iset_right;
		wxStaticText* m_staticText7;
		wxTextCtrl* iset_top;
		wxStaticText* m_staticText8;
		wxTextCtrl* iset_bottom;
		wxStaticText* m_staticText11;
		wxTextCtrl* iset_delay;
		wxButton* m_button14;
		wxButton* m_button15;
		wxButton* m_button16;
		wxListBox* iset_list;
		wxStaticText* m_staticText9;
		wxTextCtrl* an_data;
		wxButton* m_button8;
		wxButton* m_button9;
		wxButton* m_button10;
		wxListBox* an_list;
		wxStaticText* m_staticText10;
		wxTextCtrl* m_ResName;
		wxCheckBox* m_AutoGen;
		wxButton* m_button17;
		wxButton* m_button18;
		
		// Virtual event handlers, overide them in your derived class
		virtual void On_isetAdd( wxCommandEvent& event ) { event.Skip(); }
		virtual void ON_IsetModify( wxCommandEvent& event ) { event.Skip(); }
		virtual void On_IsetDelete( wxCommandEvent& event ) { event.Skip(); }
		virtual void On_isetSelect( wxCommandEvent& event ) { event.Skip(); }
		virtual void On_AnAdd( wxCommandEvent& event ) { event.Skip(); }
		virtual void On_anModify( wxCommandEvent& event ) { event.Skip(); }
		virtual void On_AnDelete( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAnimSelected( wxCommandEvent& event ) { event.Skip(); }
		virtual void On_Save( wxCommandEvent& event ) { event.Skip(); }
		virtual void On_Load( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		SpriteEditor( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Sprite Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 343,600 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~SpriteEditor();
	
};

#endif //__IDIALOGS_H__
