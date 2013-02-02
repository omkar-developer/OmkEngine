///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 10 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "IDialogs.h"

///////////////////////////////////////////////////////////////////////////

IDialog::IDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	m_listBox1 = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxSize( 250,150 ), 0, NULL, 0 ); 
	bSizer2->Add( m_listBox1, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button61 = new wxButton( this, wxID_ANY, wxT("Set Path"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_button61, 0, wxALL, 5 );
	
	m_button71 = new wxButton( this, wxID_ANY, wxT("Reset"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_button71, 0, wxALL, 5 );
	
	m_button8 = new wxButton( this, wxID_ANY, wxT("Set Color"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_button8, 0, wxALL, 5 );
	
	
	bSizer2->Add( bSizer5, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button1 = new wxButton( this, wxID_ANY, wxT("Create"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button1, 0, wxALL, 5 );
	
	m_button3 = new wxButton( this, wxID_ANY, wxT("Toggle Editable"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button3, 0, wxALL, 5 );
	
	m_button7 = new wxButton( this, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button7, 0, wxALL, 5 );
	
	
	bSizer2->Add( bSizer3, 0, 0, 5 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button5 = new wxButton( this, wxID_ANY, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_button5, 0, wxALL, 5 );
	
	m_button6 = new wxButton( this, wxID_ANY, wxT("Load"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_button6, 0, wxALL, 5 );
	
	m_button9 = new wxButton( this, wxID_ANY, wxT("Copy"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_button9, 0, wxALL, 5 );
	
	
	bSizer2->Add( bSizer4, 0, 0, 5 );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button10 = new wxButton( this, wxID_ANY, wxT("Reload script"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer11->Add( m_button10, 0, wxALL, 5 );
	
	m_button11 = new wxButton( this, wxID_ANY, wxT("Debug Draw"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer11->Add( m_button11, 0, wxALL, 5 );
	
	m_button19 = new wxButton( this, wxID_ANY, wxT("Console"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer11->Add( m_button19, 0, wxALL, 5 );
	
	
	bSizer2->Add( bSizer11, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button12 = new wxButton( this, wxID_ANY, wxT("Show All"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer12->Add( m_button12, 0, wxALL, 5 );
	
	m_button13 = new wxButton( this, wxID_ANY, wxT("Hide"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer12->Add( m_button13, 0, wxALL, 5 );
	
	m_button14 = new wxButton( this, wxID_ANY, wxT("Script Editor"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer12->Add( m_button14, 0, wxALL, 5 );
	
	
	bSizer2->Add( bSizer12, 0, wxEXPAND, 5 );
	
	m_propertyGridManager1 = new wxPropertyGridManager(this, wxID_ANY, wxDefaultPosition, wxSize( 250,-1 ), wxPGMAN_DEFAULT_STYLE|wxPG_BOLD_MODIFIED|wxPG_DESCRIPTION|wxPG_TOOLTIPS|wxTAB_TRAVERSAL);
	bSizer2->Add( m_propertyGridManager1, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizer1->Add( bSizer2, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( IDialog::OnClose ) );
	m_button61->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IDialog::On_SetPath ), NULL, this );
	m_button71->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IDialog::On_Reset ), NULL, this );
	m_button8->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IDialog::On_SetColor ), NULL, this );
	m_button1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IDialog::On_Create ), NULL, this );
	m_button3->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IDialog::On_Update ), NULL, this );
	m_button7->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IDialog::On_Delete ), NULL, this );
	m_button5->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IDialog::On_Save ), NULL, this );
	m_button6->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IDialog::On_Laad ), NULL, this );
	m_button9->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IDialog::On_Copy ), NULL, this );
	m_button10->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IDialog::On_ReScript ), NULL, this );
	m_button11->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IDialog::On_DDraw ), NULL, this );
	m_button19->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IDialog::On_console ), NULL, this );
	m_button12->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IDialog::On_ShowAll ), NULL, this );
	m_button13->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IDialog::On_Hide ), NULL, this );
	m_button14->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IDialog::ON_ScriptEdiitor ), NULL, this );
}

IDialog::~IDialog()
{
}

m_drawarea::m_drawarea( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	
	this->Centre( wxBOTH );
}

m_drawarea::~m_drawarea()
{
}

ColorDialog::ColorDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Alpha : "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer7->Add( m_staticText1, 0, wxALL, 5 );
	
	m_Alpha = new wxSlider( this, wxID_ANY, 0, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	bSizer7->Add( m_Alpha, 1, wxALL, 5 );
	
	
	bSizer6->Add( bSizer7, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer71;
	bSizer71 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText11 = new wxStaticText( this, wxID_ANY, wxT("Red :    "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	bSizer71->Add( m_staticText11, 0, wxALL, 5 );
	
	m_Red = new wxSlider( this, wxID_ANY, 0, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	bSizer71->Add( m_Red, 1, wxALL, 5 );
	
	
	bSizer6->Add( bSizer71, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer72;
	bSizer72 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText12 = new wxStaticText( this, wxID_ANY, wxT("Green : "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	bSizer72->Add( m_staticText12, 0, wxALL, 5 );
	
	m_Green = new wxSlider( this, wxID_ANY, 0, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	bSizer72->Add( m_Green, 1, wxALL, 5 );
	
	
	bSizer6->Add( bSizer72, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer73;
	bSizer73 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText13 = new wxStaticText( this, wxID_ANY, wxT("Blue :    "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	bSizer73->Add( m_staticText13, 0, wxALL, 5 );
	
	m_Blue = new wxSlider( this, wxID_ANY, 0, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	bSizer73->Add( m_Blue, 1, wxALL, 5 );
	
	
	bSizer6->Add( bSizer73, 1, wxEXPAND, 5 );
	
	m_textCtrl1 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_textCtrl1, 0, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer6 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_Alpha->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Alpha->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Alpha->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Alpha->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Alpha->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Alpha->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Alpha->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Alpha->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Alpha->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Red->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Red->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Red->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Red->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Red->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Red->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Red->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Red->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Red->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Green->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Green->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Green->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Green->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Green->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Green->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Green->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Green->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Green->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Blue->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Blue->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Blue->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Blue->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Blue->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Blue->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Blue->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Blue->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
	m_Blue->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( ColorDialog::On_Update ), NULL, this );
}

ColorDialog::~ColorDialog()
{
}

ScriptEditor::ScriptEditor( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button15 = new wxButton( this, wxID_ANY, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15->Add( m_button15, 0, wxALL, 5 );
	
	m_button16 = new wxButton( this, wxID_ANY, wxT("Load"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15->Add( m_button16, 0, wxALL, 5 );
	
	m_button17 = new wxButton( this, wxID_ANY, wxT("Compile"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15->Add( m_button17, 0, wxALL, 5 );
	
	m_button18 = new wxButton( this, wxID_ANY, wxT("Clear All"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15->Add( m_button18, 0, wxALL, 5 );
	
	m_autocheck = new wxCheckBox( this, wxID_ANY, wxT("Auto check errors"), wxDefaultPosition, wxDefaultSize, 0 );
	m_autocheck->SetValue(true); 
	bSizer15->Add( m_autocheck, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer13->Add( bSizer15, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer13 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_button15->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptEditor::On_Save ), NULL, this );
	m_button16->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptEditor::On_Load ), NULL, this );
	m_button17->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptEditor::On_Compile ), NULL, this );
	m_button18->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptEditor::On_New ), NULL, this );
}

ScriptEditor::~ScriptEditor()
{
}
