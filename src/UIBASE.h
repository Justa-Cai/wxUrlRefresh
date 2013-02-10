///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 30 2011)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __UIBASE_H__
#define __UIBASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/hyperlink.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/frame.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/dialog.h>
#include <wx/checkbox.h>

///////////////////////////////////////////////////////////////////////////

#define wxID_MenuItemRefresh 1000
#define wxID_MenuItemFetch 1001
#define wxID_BtnFetch 1002

///////////////////////////////////////////////////////////////////////////////
/// Class CMyFrameBase
///////////////////////////////////////////////////////////////////////////////
class CMyFrameBase : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* m_menubar1;
		wxMenu* m_menu1;
		wxHyperlinkCtrl* m_hyperlink1;
		wxTextCtrl* m_textCtrl8;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnMenuRefresh( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuFetch( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		CMyFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("wxUrlRefresh"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 640,480 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~CMyFrameBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class CDialogProxyFetchBase
///////////////////////////////////////////////////////////////////////////////
class CDialogProxyFetchBase : public wxDialog 
{
	private:
	
	protected:
		wxButton* m_btnFetch;
		wxStaticText* m_staticTextInfo;
		wxTextCtrl* m_textCtrlInfo;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnInitDialog( wxInitDialogEvent& event ) { event.Skip(); }
		virtual void OnBtnFetchClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		CDialogProxyFetchBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Proxy Fetch"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 600,480 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~CDialogProxyFetchBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class CDialogProxyRefreshBase
///////////////////////////////////////////////////////////////////////////////
class CDialogProxyRefreshBase : public wxDialog 
{
	private:
	
	protected:
		wxButton* m_btnRefresh;
		wxTextCtrl* m_textCtrlHttp;
		wxCheckBox* m_checkBoxProxy;
		wxStaticText* m_staticText6;
		wxTextCtrl* m_textCtrlThread;
		wxStaticText* m_staticText61;
		wxTextCtrl* m_textCtrlRefreshNum;
		wxStaticText* m_staticTextInfo;
		wxTextCtrl* m_textCtrlInfo;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnIdle( wxIdleEvent& event ) { event.Skip(); }
		virtual void OnBtnRefreshClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		CDialogProxyRefreshBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Refresh"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 600,480 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~CDialogProxyRefreshBase();
	
};

#endif //__UIBASE_H__
