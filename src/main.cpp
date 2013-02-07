// wxUrlRefresh.cpp : 定义控制台应用程序的入口点。
//

#include "wx/wx.h"
#include "UIBASE.h"
#include "curl/curl.h"
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

