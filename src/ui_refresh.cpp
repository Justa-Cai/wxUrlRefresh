#include "ui_refresh.h"

DEFINE_LOCAL_EVENT_TYPE(ENUM_CUSTOMEVENT_REFRESH)

enum {
	EVENT_TYPE_REFRESH_INFO,
	EVENT_TYPE_REFRESH_SUCC,
	EVENT_TYPE_REFRESH_FAIL,

	EVENT_TYPE_THREAD_EXIT,
};

class CThreadRefresh:public wxThread
{
public:
	CThreadRefresh(wxString url, wxString proxy, wxWindow *pWindows)
		:m_url(url)
		,m_proxy(proxy)
		,m_pWindows(pWindows)
	{
		Create();
	}

	virtual void * Entry() 
	{
		CUrlOpt opt(m_url, URL_OPT_REFRESH, m_proxy);
		m_bResult = (opt.Run() == CURLE_WRITE_ERROR);
		if (m_bResult)
			PostEvent(EVENT_TYPE_REFRESH_SUCC, m_proxy);
		else
			PostEvent(EVENT_TYPE_REFRESH_FAIL, m_proxy);
		return NULL;
	}

	virtual void OnExit() 
	{
		PostEvent(EVENT_TYPE_THREAD_EXIT);
	}

	void PostEvent(int type, wxString str = wxEmptyString)
	{
		wxCommandEvent eventCustom(ENUM_CUSTOMEVENT_REFRESH);
		eventCustom.SetInt(type);
		if (!str.IsEmpty())
			eventCustom.SetString(str);
		wxPostEvent(m_pWindows->GetEventHandler(), eventCustom); 
	}

public:
	wxString m_url, m_proxy;
	wxWindow *m_pWindows;
	bool m_bResult;
};

CDialogProxyRefresh::CDialogProxyRefresh( wxWindow* parent/*=NULL */ ) :CDialogProxyRefreshBase(parent)
{
	m_pConfig = CProxyDataConfig::GetHandle();
	m_textCtrlInfo->AppendText(wxString::Format("Proxy Num:%d\n", m_pConfig->GetCount()));
	m_textCtrlHttp->SetLabel("http://pp.163.com/yuchia/pp/8044003.html");
	m_bRefresh = false;
	m_iProxyCount = m_pConfig->GetCount();
}

CDialogProxyRefresh::~CDialogProxyRefresh()
{
	if (m_iProxyCount!= m_pConfig->GetCount())
		m_pConfig->Save();
}

void CDialogProxyRefresh::OnBtnRefreshClick( wxCommandEvent& event )
{
	if (!m_bRefresh) {
		m_btnRefresh->Enable(false);
		// create max threads...
		m_iRefreshMax  = wxAtoi(m_textCtrlRefreshNum->GetValue());
		m_iThreadMax   = wxAtoi(m_textCtrlThread->GetValue());
		m_iRefreshNums = 0;
		m_iThreadNums  = 0;
		m_bUseProxy = m_checkBoxProxy->GetValue();
		m_url = m_textCtrlHttp->GetValue();
		m_iProxyPos = 0;
		m_bRefresh = true;
		if (m_iRefreshMax<m_iThreadMax)
			m_iThreadMax = m_iRefreshMax;

		for (int i=0; i<m_iThreadMax; i++)
			CreatThreads();
	}

}

void CDialogProxyRefresh::OnIdle( wxIdleEvent& event )
{
	event.Skip();
}

void CDialogProxyRefresh::CreatThreads()
{
	CThreadRefresh *pThread;

	// 已刷完
	if (m_iRefreshNums>=m_iRefreshMax)
	{
		if(m_iThreadNums <= 0)
		{
			m_bRefresh = false;
			m_btnRefresh->Enable(true);
			m_textCtrlInfo->AppendText("*****Finish...............\n");
		}
		else 
			m_textCtrlInfo->AppendText(wxString::Format("! wait thread exit...%d\n", m_iThreadNums));
	}

	// 如果未达到最大线程,创建线程
	// 理论会多刷m_iThreadMax，但加快速度总比等待好
	if (m_iRefreshNums<m_iRefreshMax && m_iThreadNums<m_iThreadMax) 
	{
		if (m_iProxyPos>=m_pConfig->GetCount())
			m_iProxyPos = 0;
		wxString proxy = m_pConfig->GetProxyHttp(m_iProxyPos);

		if (m_bUseProxy)
			pThread = new CThreadRefresh(m_url, proxy, this);
		else
			pThread = new CThreadRefresh(m_url, wxEmptyString, this);
		m_iThreadNums++;
		m_iProxyPos++;
		pThread->Run();
	}
	RefreshInfo();
}

void CDialogProxyRefresh::RefreshInfo()
{
	m_staticTextInfo->SetLabel(wxString::Format("Refresh %d/%d Thread:%d ProxyNums:%d", m_iRefreshNums, m_iRefreshMax, m_iThreadNums, m_pConfig->GetCount()));
}


void CDialogProxyRefresh::OnRefreshCMD( wxCommandEvent& event )
{
	switch(event.GetInt())
	{
	case EVENT_TYPE_REFRESH_INFO:
		m_textCtrlInfo->AppendText(event.GetString());
		break;
	case EVENT_TYPE_REFRESH_SUCC:
		{
			m_textCtrlInfo->AppendText(wxString::Format("* %s succ\n", event.GetString()));
			m_iRefreshNums++;
			RefreshInfo();
		}
		break;
	case EVENT_TYPE_REFRESH_FAIL:
		{
			if (m_bUseProxy) {
				wxString key = event.GetString();
				if (key.Len()>0)
					m_pConfig->Remove(key);
			}

			m_textCtrlInfo->AppendText(wxString::Format("* %s fail\n", event.GetString()));
		}
		break;
	case EVENT_TYPE_THREAD_EXIT:
		m_iThreadNums--;
		CreatThreads();
		break;
	default:
		m_textCtrlInfo->AppendText(event.GetString());
		break;
	}
}


BEGIN_EVENT_TABLE(CDialogProxyRefresh, wxDialog)
EVT_COMMAND(wxID_ANY, ENUM_CUSTOMEVENT_REFRESH, CDialogProxyRefresh::OnRefreshCMD)
END_EVENT_TABLE()
