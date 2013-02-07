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
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

CMyFrameBase::~CMyFrameBase()
{
}
