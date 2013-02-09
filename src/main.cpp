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

		//wxEVT_COMMAND_MENU_SELECTED evt;
		//wxPostEvent(this->GetEventHandler(), evt);
		wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, wxID_MenuItemFetch);
		AddPendingEvent(evt);

#if 0
		CUrlOpt opt("http://www.baidu.com/", URL_OPT_FETCH);
		CURLcode ret = opt.Run();
		if (ret == 0)
			m_textCtrlInfo->AppendText(opt.GetContent());
		else
			m_textCtrlInfo->AppendText(wxString::Format("opt error:%s\n", opt.GetErrorString()));
#endif
	}

	virtual void OnButtonClick( wxCommandEvent& event ) 
	{
		CProxyParse parse;
		int ret = parse.Run("http://51dai.li/http_non_anonymous.html", 0);
		if ( ret<0 )
			m_textCtrlInfo->AppendText(wxString::Format("parse fail ret:%d\n", ret));
		else 
		{
			m_textCtrlInfo->AppendText("parse succ\n");
			wxString str;
			for (wxUint32 i=0; i<parse.m_array.GetCount(); i++) 
			{
				CProxyData *pData = (CProxyData *)parse.m_array[i];
				str += wxString::Format("[%4d]%s:%s\n", i, pData->host, pData->port);
			}
			m_textCtrlInfo->AppendText(str);
		}
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

