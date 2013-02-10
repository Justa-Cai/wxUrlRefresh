#include "ui_fetch.h"

DEFINE_LOCAL_EVENT_TYPE(ENUM_CUSTOMEVENT_FETCH)

enum {
	EVENT_TYPE_FETCH_BEGIN,
	EVENT_TYPE_PROGRESS,
	EVENT_TYPE_FETCH_END,
	EVENT_TYPE_ERROR,
	EVENT_TYPE_FETCH_INFO,

	EVENT_TYPE_PARSE_FAIL,
	EVENT_TYPE_PARSE_SUCC,


	EVENT_TYPE_THREAD_EXIT,
};

class CThreadFetchProxyData;
static CThreadFetchProxyData *g_pThread=NULL;

////////////////////////////////////
class CMyUrlOpt:public CUrlOpt
{
public:
	CMyUrlOpt(wxString url, URL_OPT_TYPE type=URL_OPT_REFRESH, wxString proxy=wxEmptyString)
		:CUrlOpt(url, type, proxy)
	{

	}

	virtual int OnProgress( double dltotal, double dlnow, double ultotal, double ulnow ) 
	{
		wxCommandEvent eventCustom(ENUM_CUSTOMEVENT_FETCH);
		eventCustom.SetInt(EVENT_TYPE_PROGRESS);
		eventCustom.SetString(wxString::Format("progress:%.0f/%.0f", dlnow, dltotal));
		wxPostEvent(m_pWindows->GetEventHandler(), eventCustom); 
		return 0;
	}

	void Attach(wxWindow *pWindows)
	{
		m_pWindows = pWindows;
	}

public:
	wxWindow *m_pWindows;

};

class CThreadFetchProxyData:public wxThread
{
public:
	CThreadFetchProxyData(wxString url, wxWindow *pWindows)
		:m_url(url)
		,m_pWindows(pWindows)
		,wxThread(wxTHREAD_JOINABLE)
	{
		Create();
	}

	virtual void* Entry()
	{
		CMyUrlOpt opt(m_url, URL_OPT_FETCH);
		opt.Attach(m_pWindows);
		PostEvent(EVENT_TYPE_FETCH_BEGIN);

		opt.SetTimeout(10);
		opt.SetUseGzip(true);
		if (opt.Run()!=0) {
			PostEvent(EVENT_TYPE_ERROR, wxString::Format("Fetch Url: %s fail %s\n", m_url, opt.GetErrorString()));
			wxLogError("Fetch Url: %s fail %s\n", m_url, opt.GetErrorString());
			return NULL;
		}

		wxString content = opt.GetContent();
		wxLogMessage("content length:%d", content.Len());
	//	PostEvent(EVENT_TYPE_FETCH_INFO, content);
		PostEvent(EVENT_TYPE_FETCH_END);

		if (content.Len()<=0) 
			return NULL;

		PostEvent(EVENT_TYPE_FETCH_INFO, wxT("\n* Parse Data\n"));
		int ret = m_parse.RunFromMem(content);
		if (ret<0)
			PostEvent(EVENT_TYPE_PARSE_FAIL);
		else
			PostEvent(EVENT_TYPE_PARSE_SUCC);
		return NULL;
	}

	void PostEvent(int type, wxString str = wxEmptyString)
	{
		wxCommandEvent eventCustom(ENUM_CUSTOMEVENT_FETCH);
		eventCustom.SetInt(type);
		if (!str.IsEmpty())
			eventCustom.SetString(str);
		wxPostEvent(m_pWindows->GetEventHandler(), eventCustom); 
	}

	virtual void OnExit() 
	{
		PostEvent(EVENT_TYPE_THREAD_EXIT);
	}

public:
	wxString m_url;
	wxWindow *m_pWindows;
	CProxyParse m_parse;
	
};

/////////////////////////////////////
//
////////////////////////////////////
CDialogProxyFetch::CDialogProxyFetch( wxWindow* parent/*=NULL */ ) :CDialogProxyFetchBase(parent)
{
	m_pConfig = CProxyDataConfig::GetHandle();
	m_textCtrlInfo->AppendText(wxString::Format("proxy url nums:%d", m_pConfig->GetCount()));
}

void CDialogProxyFetch::OnInitDialog( wxInitDialogEvent& event )
{
	wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, wxID_BtnFetch);
	wxPostEvent(this->GetEventHandler(), evt);
}

void CDialogProxyFetch::OnBtnFetchClick( wxCommandEvent& event )
{
	if (!g_pThread) {
		m_btnFetch->Enable(false);
		wxString url = "http://51dai.li/http_non_anonymous.html";
		//url = "http://51dai.li/http_fast.html";
		//url = "http://www.baidu.com/";
		//url = "http://51dai.li/http_anonymous.html";
		g_pThread = new CThreadFetchProxyData(url, this);
		g_pThread->Run();
	}
	else 
		wxMessageBox("Thread Running...!!!");

#if 0
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
#endif

}

void CDialogProxyFetch::OnFetch(wxCommandEvent& event)
{
	switch(event.GetInt())
	{
	case EVENT_TYPE_PROGRESS:
		m_staticTextInfo->SetLabel(event.GetString());
		break;

	case EVENT_TYPE_FETCH_BEGIN:
		m_textCtrlInfo->Clear();
		m_textCtrlInfo->AppendText("Begin Fetch Html Data...\n");
		break;

	case EVENT_TYPE_FETCH_END:
		m_textCtrlInfo->AppendText("Finish Fetch Html Data...\n");
		break;

	case EVENT_TYPE_PARSE_SUCC:
		{

			m_textCtrlInfo->AppendText("parse succ\n");
			wxString str;
			for (wxUint32 i=0; i<g_pThread->m_parse.m_array.GetCount(); i++) 
			{
				CProxyData *pData = (CProxyData *)g_pThread->m_parse.m_array[i];
				str += wxString::Format("[%4d]%s:%s\n", i, pData->host, pData->port);
				m_pConfig->Add(pData);
			}
			m_textCtrlInfo->AppendText(str);
			m_pConfig->Save();
			m_textCtrlInfo->AppendText(wxString::Format("proxy url nums:%d", m_pConfig->GetCount()));
		}
		break;

	case EVENT_TYPE_THREAD_EXIT:
		while(g_pThread->IsRunning() == true)
			wxThread::Sleep(1);
		delete g_pThread;
		g_pThread = NULL;
		m_textCtrlInfo->AppendText("Thread Exit\n");
		m_btnFetch->Enable(true);
		break;

	default:
		m_textCtrlInfo->AppendText(event.GetString());
		break;
	}
}

BEGIN_EVENT_TABLE(CDialogProxyFetch, wxDialog)
  EVT_COMMAND(wxID_ANY, ENUM_CUSTOMEVENT_FETCH, CDialogProxyFetch::OnFetch)
  END_EVENT_TABLE()

