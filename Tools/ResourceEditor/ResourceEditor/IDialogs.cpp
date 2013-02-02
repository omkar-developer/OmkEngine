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
	
	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Resource Information : ") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Name : "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer1->Add( m_staticText1, 0, wxALL, 5 );
	
	res_name = new wxTextCtrl( this, ID_RES_NAME, wxEmptyString, wxDefaultPosition, wxSize( 300,-1 ), 0 );
	fgSizer1->Add( res_name, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Group : "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	fgSizer1->Add( m_staticText2, 0, wxALL, 5 );
	
	res_group = new wxSpinCtrl( this, ID_RES_GROUP, wxEmptyString, wxDefaultPosition, wxSize( 200,-1 ), wxSP_ARROW_KEYS, 0, 10, 0 );
	fgSizer1->Add( res_group, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("Type : "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer1->Add( m_staticText3, 0, wxALL, 5 );
	
	wxString res_typeChoices[] = { wxT("Please Select"), wxT("Image"), wxT("Font"), wxT("Sound"), wxT("Sprite"), wxT("Object"), wxT("Imageset") };
	int res_typeNChoices = sizeof( res_typeChoices ) / sizeof( wxString );
	res_type = new wxChoice( this, ID_RES_TYPE, wxDefaultPosition, wxSize( 200,-1 ), res_typeNChoices, res_typeChoices, 0 );
	res_type->SetSelection( 0 );
	fgSizer1->Add( res_type, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("File Name : "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	fgSizer1->Add( m_staticText5, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	res_filename = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 250,-1 ), 0 );
	bSizer4->Add( res_filename, 0, wxALL, 5 );
	
	m_button4 = new wxButton( this, wxID_ANY, wxT(". .  ."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_button4, 1, wxALL|wxEXPAND, 5 );
	
	
	fgSizer1->Add( bSizer4, 1, wxEXPAND, 5 );
	
	
	sbSizer1->Add( fgSizer1, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer41;
	bSizer41 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button5 = new wxButton( this, wxID_ANY, wxT("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer41->Add( m_button5, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_button6 = new wxButton( this, wxID_ANY, wxT("Modify"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer41->Add( m_button6, 0, wxALL, 5 );
	
	m_button7 = new wxButton( this, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer41->Add( m_button7, 0, wxALL, 5 );
	
	m_button17 = new wxButton( this, wxID_ANY, wxT("Auto Fill"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer41->Add( m_button17, 0, wxALL, 5 );
	
	
	sbSizer1->Add( bSizer41, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	m_listCtrl1 = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );
	bSizer2->Add( m_listCtrl1, 1, wxALL|wxEXPAND, 5 );
	
	
	sbSizer1->Add( bSizer2, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button1 = new wxButton( this, ID_BSAVE, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button1, 0, wxALL, 5 );
	
	m_button2 = new wxButton( this, ID_BLOAD, wxT("Load"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button2, 0, wxALL, 5 );
	
	m_button3 = new wxButton( this, ID_BQUIT, wxT("Sprite Editor"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button3, 0, wxALL, 5 );
	
	m_button16 = new wxButton( this, wxID_ANY, wxT("ClearAll"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button16, 0, wxALL, 5 );
	
	
	sbSizer1->Add( bSizer3, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	
	this->SetSizer( sbSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( IDialog::OnClose ) );
	m_button4->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IDialog::OnPickFile ), NULL, this );
	m_button5->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IDialog::on_Add ), NULL, this );
	m_button6->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IDialog::On_Modify ), NULL, this );
	m_button7->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IDialog::On_Delete ), NULL, this );
	m_button17->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IDialog::On_AutoCreate ), NULL, this );
	m_listCtrl1->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( IDialog::OnListItemSelected ), NULL, this );
	m_button1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IDialog::On_Save ), NULL, this );
	m_button2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IDialog::On_Load ), NULL, this );
	m_button3->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IDialog::On_Quit ), NULL, this );
	m_button16->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IDialog::On_ClearAll ), NULL, this );
}

IDialog::~IDialog()
{
}

SpriteEditor::SpriteEditor( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Sprite imageset information : ") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 0, 4, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("Left : "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	fgSizer2->Add( m_staticText5, 0, wxALL, 5 );
	
	iset_left = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( iset_left, 0, wxALL, 5 );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("Right : "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizer2->Add( m_staticText6, 0, wxALL, 5 );
	
	iset_right = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( iset_right, 0, wxALL, 5 );
	
	m_staticText7 = new wxStaticText( this, wxID_ANY, wxT("Top : "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	fgSizer2->Add( m_staticText7, 0, wxALL, 5 );
	
	iset_top = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( iset_top, 0, wxALL, 5 );
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("Bottom : "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	fgSizer2->Add( m_staticText8, 0, wxALL, 5 );
	
	iset_bottom = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( iset_bottom, 0, wxALL, 5 );
	
	m_staticText11 = new wxStaticText( this, wxID_ANY, wxT("Delay : "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	fgSizer2->Add( m_staticText11, 0, wxALL, 5 );
	
	iset_delay = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( iset_delay, 0, wxALL, 5 );
	
	
	sbSizer2->Add( fgSizer2, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button14 = new wxButton( this, wxID_ANY, wxT("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_button14, 0, wxALL, 5 );
	
	m_button15 = new wxButton( this, wxID_ANY, wxT("Modify"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_button15, 0, wxALL, 5 );
	
	m_button16 = new wxButton( this, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_button16, 0, wxALL, 5 );
	
	
	sbSizer2->Add( bSizer9, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	iset_list = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	bSizer6->Add( iset_list, 1, wxALL|wxEXPAND, 5 );
	
	
	sbSizer2->Add( bSizer6, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Animations : ") ), wxHORIZONTAL );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText9 = new wxStaticText( this, wxID_ANY, wxT("Index data  : "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	bSizer7->Add( m_staticText9, 0, wxALL, 5 );
	
	an_data = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( an_data, 0, wxALL|wxEXPAND, 5 );
	
	m_button8 = new wxButton( this, wxID_ANY, wxT("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( m_button8, 0, wxALL, 5 );
	
	m_button9 = new wxButton( this, wxID_ANY, wxT("Modify"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( m_button9, 0, wxALL, 5 );
	
	m_button10 = new wxButton( this, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( m_button10, 0, wxALL, 5 );
	
	
	sbSizer4->Add( bSizer7, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );
	
	an_list = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	bSizer8->Add( an_list, 1, wxALL|wxEXPAND, 5 );
	
	
	sbSizer4->Add( bSizer8, 1, wxEXPAND, 5 );
	
	
	sbSizer2->Add( sbSizer4, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText10 = new wxStaticText( this, wxID_ANY, wxT("Name : "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	bSizer11->Add( m_staticText10, 0, wxALL, 5 );
	
	m_ResName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer11->Add( m_ResName, 1, wxALL|wxEXPAND, 5 );
	
	
	sbSizer2->Add( bSizer11, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );
	
	m_AutoGen = new wxCheckBox( this, wxID_ANY, wxT("Auto generate grid"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer12->Add( m_AutoGen, 0, wxALL, 5 );
	
	
	sbSizer2->Add( bSizer12, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button17 = new wxButton( this, wxID_ANY, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer10->Add( m_button17, 0, wxALL, 5 );
	
	m_button18 = new wxButton( this, wxID_ANY, wxT("Load"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer10->Add( m_button18, 0, wxALL, 5 );
	
	
	sbSizer2->Add( bSizer10, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	
	this->SetSizer( sbSizer2 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_button14->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteEditor::On_isetAdd ), NULL, this );
	m_button15->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteEditor::ON_IsetModify ), NULL, this );
	m_button16->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteEditor::On_IsetDelete ), NULL, this );
	iset_list->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( SpriteEditor::On_isetSelect ), NULL, this );
	m_button8->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteEditor::On_AnAdd ), NULL, this );
	m_button9->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteEditor::On_anModify ), NULL, this );
	m_button10->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteEditor::On_AnDelete ), NULL, this );
	an_list->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( SpriteEditor::OnAnimSelected ), NULL, this );
	m_button17->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteEditor::On_Save ), NULL, this );
	m_button18->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteEditor::On_Load ), NULL, this );
}

SpriteEditor::~SpriteEditor()
{
}
