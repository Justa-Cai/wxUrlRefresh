// wxUrlRefresh.cpp : 定义控制台应用程序的入口点。
//

#include "wx/wx.h"
#include "UIBASE.h"
#include "url_api.h"


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

		CProxyParse parse;
		if (parse.Run()<0)
			m_textCtrlInfo->AppendText("parse fail\n");
		else 
		{
			m_textCtrlInfo->AppendText("parse succ\n");
			wxString str;
			for (wxUint32 i=0; i<parse.m_array.GetCount(); i++) 
			{
				CProxyData *pData = (CProxyData *)parse.m_array[i];
				str += wxString::Format("%s:%s\n", pData->host, pData->port);
			}
			m_textCtrlInfo->AppendText(str);
		}


#if 0
		CUrlOpt opt("http://www.baidu.com/", URL_OPT_FETCH);
		CURLcode ret = opt.Run();
		if (ret == 0)
			m_textCtrlInfo->AppendText(opt.GetContent());
		else
			m_textCtrlInfo->AppendText(wxString::Format("opt error:%s\n", opt.GetErrorString()));
#endif
	}
};

bool MyApp::OnInit()
{
	CMyFrame *frame = new CMyFrame(NULL);
	frame->Show( true );
	return true;
}

