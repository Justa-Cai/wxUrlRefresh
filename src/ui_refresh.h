#ifndef _UI_REFRESH_H_
#define _UI_REFRESH_H_
#include <wx/wx.h>
#include "UIBASE.h"
#include "url_api.h"

enum 
{
	ENUM_CUSTOMEVENT_ID_Id2=7000,
};
BEGIN_DECLARE_EVENT_TYPES()
DECLARE_LOCAL_EVENT_TYPE(ENUM_CUSTOMEVENT_REFRESH, ENUM_CUSTOMEVENT_ID_Id2)
END_DECLARE_EVENT_TYPES()

class CDialogProxyRefresh: public CDialogProxyRefreshBase
{
public:
	CDialogProxyRefresh( wxWindow* parent=NULL );
	~CDialogProxyRefresh();

	virtual void OnBtnRefreshClick( wxCommandEvent& event );

	virtual void OnIdle( wxIdleEvent& event );

	void OnRefreshCMD( wxCommandEvent& event);

	void CreatThreads();
	void RefreshInfo();
public:
	CProxyDataConfig *m_pConfig;
	bool m_bRefresh, m_bUseProxy;
	int m_iThreadNums, m_iRefreshNums, m_iThreadMax, m_iRefreshMax;
	int m_iProxyPos, m_iProxyCount;
	wxString m_url;
public:
	DECLARE_EVENT_TABLE()
};

#endif // _UI_REFRESH_H_