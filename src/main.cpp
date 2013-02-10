// wxUrlRefresh.cpp : 定义控制台应用程序的入口点。
//

#include <wx/wx.h>
#include <wx/event.h>
#include "UIBASE.h"
#include "url_api.h"
#include "ui_refresh.h"
#include "ui_fetch.h"


class MyApp: public wxApp
{
public:
	virtual bool OnInit();
};
wxIMPLEMENT_APP(MyApp);


class CMyFrame: public CMyFrameBase
{
public:
	CMyFrame( wxWindow* parent=NULL )
		:CMyFrameBase(parent)
	{
		wxLogWindow *pLogWin = new wxLogWindow(this, "LOG");
		wxLog::SetActiveTarget(pLogWin);

		wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, wxID_MenuItemRefresh);
		AddPendingEvent(evt);
	}

	virtual void OnMenuRefresh( wxCommandEvent& event ) 
	{
		CDialogProxyRefresh *pDlg = new CDialogProxyRefresh(this);
		pDlg->ShowModal();
		pDlg->Destroy();
		delete pDlg;
	}

	virtual void OnMenuFetch( wxCommandEvent& event ) 
	{
		CDialogProxyFetch *pDlg = new CDialogProxyFetch(this);
		pDlg->ShowModal();
		pDlg->Destroy();
		delete pDlg;
	}

};

bool MyApp::OnInit()
{
	CMyFrame *frame = new CMyFrame(NULL);
	frame->Show( true );
	return true;
}

