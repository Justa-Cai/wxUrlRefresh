#include "url_api.h"
#include "tinyxml/tinyxml.h"

/// global var
static bool g_bInit = false;

//////////////////////////
/// CURLOPT 
/////////////////////////
CUrlOpt::CUrlOpt( wxString url, URL_OPT_TYPE type/*=URL_OPT_REFRESH*/, wxString proxy/*=wxEmptyString*/ )
:m_type(type)
,m_url(url)
,m_proxy(proxy)
,m_ret(CURLE_OK)
{
	if (!g_bInit) {
		curl_global_init(CURL_GLOBAL_ALL);
		g_bInit = true;
	}
}

CUrlOpt::~CUrlOpt()
{

}

size_t my_write_data( void *buffer, size_t size, size_t nmemb, void *userp )
{
	CUrlOpt *pUrlOpt = (CUrlOpt *)userp;
	return pUrlOpt->write_data(buffer, size, nmemb, userp);
}

size_t CUrlOpt::write_data( void *buffer, size_t size, size_t nmemb, void *userp )
{
	if (m_type==URL_OPT_FETCH) {
		m_content += wxString((const char*)buffer);
		return nmemb*size;
	}
	else if (m_type == URL_OPT_REFRESH)
		return 0;
	return 0;
}

wxString &CUrlOpt::GetContent()
{
	return m_content;
}

wxString CUrlOpt::GetErrorString()
{
	return wxString(curl_easy_strerror(m_ret));
}

CURLcode CUrlOpt::Run()
{
	CURL *curl;
	CURLcode res;
	char *pUrl, *pPorxy=NULL;

	m_content.Clear();
	if (m_url.IsEmpty())
		return (CURLcode)-1;

	curl = curl_easy_init();
	if(!curl){
		wxLogError("curl_easy_init fail\n");
		return CURLE_FAILED_INIT;
	}

	pUrl = (char*)calloc(m_url.Length()+1, 1);
	wxStrcpy(pUrl, m_url.c_str());

	if (!m_proxy.IsEmpty()) {
		pPorxy= (char*)calloc(m_proxy.Length()+1, 1);
		wxStrcpy(pPorxy, m_proxy.c_str());
	}

	curl_easy_setopt(curl, CURLOPT_URL, pUrl);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

	if (!m_proxy.IsEmpty()){
		curl_easy_setopt(curl, CURLOPT_PROXY, pPorxy);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
	}

	res = curl_easy_perform(curl);

	curl_easy_cleanup(curl);

	m_ret = res;
	if (pUrl)   free(pUrl);
	if (pPorxy) free(pPorxy);
	return res;
}

//////////////////////////
/// CProxyParse
/////////////////////////

int CProxyParse::Run()
{
	wxString path = wxT("d:/work/windows/wxUrlRefresh/data/最新透明HTTP代理服务器.htm");
	if (!wxFileExists(path))
		return -1;

	TiXmlDocument doc;
	if (!doc.LoadFile(path))
		return -1;

}
