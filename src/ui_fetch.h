#ifndef _UI_FETCH_H_
#define _UI_FETCH_H_
#include <wx/wx.h>
#include "UIBASE.h"
#include "url_api.h"

enum 
{
	ENUM_CUSTOMEVENT_ID_Id1=7000,
};
BEGIN_DECLARE_EVENT_TYPES()
DECLARE_LOCAL_EVENT_TYPE(ENUM_CUSTOMEVENT_FETCH, ENUM_CUSTOMEVENT_ID_Id1)
END_DECLARE_EVENT_TYPES()


class CDialogProxyFetch : public CDialogProxyFetchBase
{
public:
	CDialogProxyFetch( wxWindow* parent=NULL );
	virtual void OnInitDialog( wxInitDialogEvent& event );

	virtual void OnBtnFetchClick( wxCommandEvent& event );
	void OnFetch(wxCommandEvent& event);
	CProxyDataConfig *m_pConfig;
public:
	DECLARE_EVENT_TABLE();


};

#endif // _UI_FETCH_H_