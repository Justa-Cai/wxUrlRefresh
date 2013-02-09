///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 30 2011)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "UIBASE.h"

///////////////////////////////////////////////////////////////////////////

CMyFrameBase::CMyFrameBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_textCtrlInfo = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CHARWRAP|wxTE_MULTILINE|wxTE_WORDWRAP );
	bSizer1->Add( m_textCtrlInfo, 1, wxALL|wxEXPAND, 5 );
	
	m_buttonDo = new wxButton( this, wxID_ANY, wxT("Do"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_buttonDo, 0, wxALL|wxALIGN_RIGHT, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	m_menubar1 = new wxMenuBar( 0 );
	m_menu1 = new wxMenu();
	wxMenuItem* m_menuItemRefresh;
	m_menuItemRefresh = new wxMenuItem( m_menu1, wxID_MenuItemRefresh, wxString( wxT("Refresh") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItemRefresh );
	
	wxMenuItem* m_menuItemFetch;
	m_menuItemFetch = new wxMenuItem( m_menu1, wxID_MenuItemFetch, wxString( wxT("Proxy Fetch") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItemFetch );
	
	m_menubar1->Append( m_menu1, wxT("&Opt") ); 
	
	this->SetMenuBar( m_menubar1 );
	
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CMyFrameBase::OnClose ) );
	m_buttonDo->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CMyFrameBase::OnButtonClick ), NULL, this );
	this->Connect( m_menuItemRefresh->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CMyFrameBase::OnMenuRefresh ) );
	this->Connect( m_menuItemFetch->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CMyFrameBase::OnMenuFetch ) );
}

CMyFrameBase::~CMyFrameBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CMyFrameBase::OnClose ) );
	m_buttonDo->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CMyFrameBase::OnButtonClick ), NULL, this );
	this->Disconnect( wxID_MenuItemRefresh, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CMyFrameBase::OnMenuRefresh ) );
	this->Disconnect( wxID_MenuItemFetch, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CMyFrameBase::OnMenuFetch ) );
	
}

CDialogProxyFetchBase::CDialogProxyFetchBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	m_btnFetch = new wxButton( this, wxID_BtnFetch, wxT("Fetch"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_btnFetch, 0, wxALL, 5 );
	
	m_staticTextInfo = new wxStaticText( this, wxID_ANY, wxT("Info:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextInfo->Wrap( -1 );
	bSizer3->Add( m_staticTextInfo, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer2->Add( bSizer3, 0, wxEXPAND, 5 );
	
	m_textCtrlInfo = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CHARWRAP|wxTE_MULTILINE|wxTE_WORDWRAP );
	bSizer2->Add( m_textCtrlInfo, 1, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( bSizer2 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( CDialogProxyFetchBase::OnInitDialog ) );
	m_btnFetch->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDialogProxyFetchBase::OnBtnFetchClick ), NULL, this );
}

CDialogProxyFetchBase::~CDialogProxyFetchBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( CDialogProxyFetchBase::OnInitDialog ) );
	m_btnFetch->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDialogProxyFetchBase::OnBtnFetchClick ), NULL, this );
	
}

CDialogProxyRefreshBase::CDialogProxyRefreshBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	m_btnRefresh = new wxButton( this, wxID_ANY, wxT("Refresh"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_btnRefresh, 0, wxALL, 5 );
	
	m_staticTextInfo = new wxStaticText( this, wxID_ANY, wxT("Info:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextInfo->Wrap( -1 );
	bSizer3->Add( m_staticTextInfo, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer2->Add( bSizer3, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer31;
	bSizer31 = new wxBoxSizer( wxHORIZONTAL );
	
	m_checkBoxProxy = new wxCheckBox( this, wxID_ANY, wxT("Use Proxy"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBoxProxy->SetValue(true); 
	bSizer31->Add( m_checkBoxProxy, 0, wxALL, 5 );
	
	bSizer2->Add( bSizer31, 0, 0, 5 );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("Thread Nums"), wxDefaultPosition, wxSize( 80,-1 ), 0 );
	m_staticText6->Wrap( -1 );
	bSizer11->Add( m_staticText6, 0, wxALL, 5 );
	
	m_textCtrlThread = new wxTextCtrl( this, wxID_ANY, wxT("20"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer11->Add( m_textCtrlThread, 0, wxALL, 5 );
	
	bSizer2->Add( bSizer11, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText61 = new wxStaticText( this, wxID_ANY, wxT("Refresh Nums"), wxDefaultPosition, wxSize( 80,-1 ), 0 );
	m_staticText61->Wrap( -1 );
	bSizer12->Add( m_staticText61, 0, wxALL, 5 );
	
	m_textCtrlRefreshNum = new wxTextCtrl( this, wxID_ANY, wxT("100"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer12->Add( m_textCtrlRefreshNum, 0, wxALL, 5 );
	
	bSizer2->Add( bSizer12, 0, wxEXPAND, 5 );
	
	m_textCtrlInfo = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CHARWRAP|wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP );
	bSizer2->Add( m_textCtrlInfo, 1, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( bSizer2 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_btnRefresh->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDialogProxyRefreshBase::OnBtnRefreshClick ), NULL, this );
}

CDialogProxyRefreshBase::~CDialogProxyRefreshBase()
{
	// Disconnect Events
	m_btnRefresh->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDialogProxyRefreshBase::OnBtnRefreshClick ), NULL, this );
	
}
