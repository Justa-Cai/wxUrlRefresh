#include "url_api.h"
#include "tinyxml/tinyxml.h"
#include <tidy.h>
#include <buffio.h>
#include <wx/file.h>
#include <wx/mstream.h>
#include <wx/zstream.h>
#include <wx/txtstrm.h>
#include <wx/sstream.h>

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
,m_timeout(0)
,m_bGzip(false)
,m_bGBK(false)
{
	if (!g_bInit) {
		curl_global_init(CURL_GLOBAL_ALL);
		g_bInit = true;
	}
}

CUrlOpt::~CUrlOpt()
{
	if (m_pBuf)
		free(m_pBuf);
}

static size_t my_write_func( void *buffer, size_t size, size_t nmemb, void *userp )
{
	CUrlOpt *pUrlOpt = (CUrlOpt *)userp;
	return pUrlOpt->OnWriteData(buffer, size, nmemb, userp);
}

static int my_progress_func(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
	CUrlOpt *pUrlOpt = (CUrlOpt *)clientp;
	return pUrlOpt->OnProgress(dltotal, dlnow, ultotal, ulnow);
}

int my_curl_debug_callback (CURL *curl, curl_infotype type, char *buf, size_t size, void *ptr)
{
	if (type == CURLINFO_TEXT)
		wxLogMessage("%s", buf);

	return 0;
}

size_t my_header_callback( void *ptr, size_t size, size_t nmemb, void *userdata)
{
	CUrlOpt *pUrlOpt = (CUrlOpt *)userdata;
	return pUrlOpt->OnHeaderData(ptr, size, nmemb);
}

size_t CUrlOpt::OnWriteData( void *buffer, size_t size, size_t nmemb, void *userp )
{
	if (m_type==URL_OPT_FETCH) {
#if 0
		wxMemoryInputStream istream((char*)buffer, size*nmemb);
		wxZlibInputStream zstream(istream, wxZLIB_GZIP);
		wxString str;
		wxStringOutputStream ostream(&str);
		zstream.Read(ostream);
		m_content += str;
#endif
		m_pBuf = (char*)realloc(m_pBuf, m_length + size*nmemb);
		memcpy(m_pBuf+m_length, buffer, size*nmemb);
		m_length += size*nmemb;

		wxLogMessage("OnWriteData nmemb*size:%d m_length:%d", nmemb*size, m_length);
		return nmemb*size;
	}
	else if (m_type == URL_OPT_REFRESH)
		return 0;
	return 0;
}

wxString CUrlOpt::GetContent()
{
	return m_content;
}

wxString CUrlOpt::GetErrorString()
{
	return wxString(curl_easy_strerror(m_ret));
}

int CUrlOpt::OnProgress( double dltotal, double dlnow, double ultotal, double ulnow )
{
	return 0;
}

int CUrlOpt::OnHeaderData( void *ptr, size_t size, size_t nmemb)
{
	if (wxStrstr((char*)ptr, "charset=gbk"))
		m_bGBK = true;
	return size*nmemb;
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
#ifdef _DEBUG
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
	curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, my_curl_debug_callback);
#endif
	curl_easy_setopt(curl, CURLOPT_URL, pUrl);
#if 1
	curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_write_func);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
	curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, my_progress_func);
	curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, this);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, m_timeout);
	curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, my_header_callback);
	curl_easy_setopt(curl, CURLOPT_HEADERDATA, this);

	if (m_bGzip) 
		curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "gzip");


	curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:18.0) Gecko/20100101 Firefox/18.0");
#endif
	if (!m_proxy.IsEmpty()){
		curl_easy_setopt(curl, CURLOPT_PROXY, pPorxy);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
	}

	m_length = 0x0;
	m_pBuf = NULL;
	res = curl_easy_perform(curl);

	curl_easy_cleanup(curl);

	m_ret = res;
	if (pUrl)   free(pUrl);
	if (pPorxy) free(pPorxy);

	if (res==0) 
	{
		if (m_bGzip)
		{
			wxMemoryInputStream istream((char*)m_pBuf, m_length);
			wxZlibInputStream zstream(istream, wxZLIB_GZIP);
			wxStringOutputStream ostream(&m_content);
			zstream.Read(ostream);
		}
		else {
			wxConvAuto *pConv;
			if (m_bGBK)
				pConv = new wxConvAuto(wxFONTENCODING_CP936);
			else
				pConv = new wxConvAuto(wxFONTENCODING_UTF8);
			m_content = wxString(m_pBuf, *pConv, m_length);
			delete pConv;
		}
	}
	return res;
}

//////////////////////////
/// CProxyParse
/////////////////////////
int WriteAllToFile(const char *path, char *content, size_t len)
{
	if (wxFileExists(path))
		wxRemoveFile(path);
	
	wxFile f(path, wxFile::write_append);
	if (!f.IsOpened())
		return -1;

	f.Write(content, len);
	f.Close();
	return 0;
}

/*! \brief xml遍历类 处理代理数据 
 *
 */
class CTiXmlProxyVistor: public TiXmlVisitor
{
	//<tr bgcolor="#FFFFFF" onmouseover=
	//	"this.style.backgroundColor='#ffcc00';" onmouseout=
	//	"this.style.backgroundColor='#ffffff';" style=
	//	"background-color: rgb(255, 255, 255);">
	//	<td width="30">1</td>
	//	<td>122.72.0.145</td>
	//	<td width="60">80</td>
	//	<td width="60">CN</td>
	//	</tr>
public:
	CTiXmlProxyVistor(wxArrayPtrVoid *array)
		:m_pArray(array)
	{
		m_pArray->Clear();
	}

	~CTiXmlProxyVistor()
	{
	}

	/// Visit an element.
	virtual bool VisitEnter( const TiXmlElement& element, const TiXmlAttribute* firstAttribute )	
	{ 
		if (element.Value()!=0x0 && !wxStrcmp(element.Value(), "tr")) 
		{
			if (!wxStrcmp(element.Attribute("bgcolor"), "#FFFFFF"))
			{
				// first  num
				const TiXmlElement *pChild = element.FirstChildElement();

				if (pChild) 
				{
					// child
					pChild = pChild->NextSiblingElement();
					wxString host = wxString(pChild->GetText(), wxConvUTF8);

					// port
					pChild = pChild->NextSiblingElement();
					wxString port = wxString(pChild->GetText(), wxConvUTF8);

					if (port.Len()>0 && wxAtoi(port)!=0) {
						m_pProxyData = new CProxyData(host, port);
						m_pArray->Add(m_pProxyData);
					}
				}
			}
		}
		return true; 
	}
public:
	wxArrayPtrVoid *m_pArray;
	CProxyData *m_pProxyData;
};

int CProxyParse::Run(wxString url, int timeout)

{
	char *pBuffer;
	//http://www.51proxied.com/http_non_anonymous.html
	//wxString path = wxT("f:/work/windows/wxUrlRefresh/data/最新透明HTTP代理服务器.htm");
	//wxString path1 = wxT("f:/work/windows/wxUrlRefresh/data/result.xml");

	CUrlOpt opt(url, URL_OPT_FETCH);
	wxString data_path = wxGetCwd() + "/data/";
	wxString path1 = data_path + "_tmp.xml";

	if (!wxDirExists(data_path))
		wxMkdir(data_path);

	wxLogMessage("Fetch :%s", url);
	//opt.SetTimeout(10);
	if (opt.Run()!=0) {
		wxLogError("Fetch Url:%s fail :%s\n", url, opt.GetErrorString());
		return -1;
	}
	wxString content = opt.GetContent();

	pBuffer = (char*)calloc(content.Length()+1, 1);
	wxStrncpy(pBuffer, content, content.Len()+1);


	wxLogMessage("Run Tidy!");
	TidyBuffer output;
	TidyBuffer errbuf;
	int rc = -1;
	Bool ok;
	TidyDoc tdoc = tidyCreate();                     // Initialize "document"
	tidyBufInit( &output );
	tidyBufInit( &errbuf );
	//printf( "Tidying:\t\%s\\n", input );
	tidySetCharEncoding(tdoc, "utf8");
	ok = tidyOptSetBool( tdoc, TidyXhtmlOut, yes );  // Convert to XHTML
	if ( ok )
		rc = tidySetErrorBuffer( tdoc, &errbuf );      // Capture diagnostics
	if ( rc >= 0 )
		rc = tidyParseString( tdoc, pBuffer );           // Parse the input
	if ( rc >= 0 )
		rc = tidyCleanAndRepair( tdoc );               // Tidy it up!
	if ( rc >= 0 )
		rc = tidyRunDiagnostics( tdoc );               // Kvetch
	if ( rc > 1 )                                    // If error, force output.
		rc = ( tidyOptSetBool(tdoc, TidyForceOutput, yes) ? rc : -1 );
	if ( rc >= 0 )
		rc = tidySaveBuffer( tdoc, &output );          // Pretty Print
	if ( rc >= 0 )
	{
#ifdef _DEBUG
		//if ( rc > 0 )
		//	WriteAllToFile("f:/work/windows/wxUrlRefresh/data/error.xml", (char*)errbuf.bp, errbuf.size);
		WriteAllToFile(path1, (char*)output.bp, output.size);
#endif

	}
	else
		wxLogError("tidyFail");

	tidyBufFree( &output );
	tidyBufFree( &errbuf );
	tidyRelease( tdoc );
	if (pBuffer) free(pBuffer);


	wxLogMessage("Fetch data!");
	// 解析数据
	TiXmlDocument doc(path1);
	if (doc.LoadFile()) 
	{
		// root
		CTiXmlProxyVistor vistor(&m_array);
		TiXmlElement *pRoot = doc.RootElement();
		pRoot->Accept(&vistor);
	}
	else
	{
		printf("shit");
		return -2;
	}
	return 0;
}

int CProxyParse::RunFromMem( wxString content )
{
	char *pBuffer;
	//http://www.51proxied.com/http_non_anonymous.html
	//wxString path = wxT("f:/work/windows/wxUrlRefresh/data/最新透明HTTP代理服务器.htm");
	//wxString path1 = wxT("f:/work/windows/wxUrlRefresh/data/result.xml");

	wxString data_path = wxGetCwd() + "/data/";
	wxString path1 = data_path + "_tmp.xml";

	if (!wxDirExists(data_path))
		wxMkdir(data_path);

	pBuffer = (char*)calloc(content.Length()+1, 1);
	wxStrncpy(pBuffer, content, content.Len()+1);


	wxLogMessage("Run Tidy!");
	TidyBuffer output;
	TidyBuffer errbuf;
	int rc = -1;
	Bool ok;
	TidyDoc tdoc = tidyCreate();                     // Initialize "document"
	tidyBufInit( &output );
	tidyBufInit( &errbuf );
	//printf( "Tidying:\t\%s\\n", input );
	tidySetCharEncoding(tdoc, "utf8");
	ok = tidyOptSetBool( tdoc, TidyXhtmlOut, yes );  // Convert to XHTML
	if ( ok )
		rc = tidySetErrorBuffer( tdoc, &errbuf );      // Capture diagnostics
	if ( rc >= 0 )
		rc = tidyParseString( tdoc, pBuffer );           // Parse the input
	if ( rc >= 0 )
		rc = tidyCleanAndRepair( tdoc );               // Tidy it up!
	if ( rc >= 0 )
		rc = tidyRunDiagnostics( tdoc );               // Kvetch
	if ( rc > 1 )                                    // If error, force output.
		rc = ( tidyOptSetBool(tdoc, TidyForceOutput, yes) ? rc : -1 );
	if ( rc >= 0 )
		rc = tidySaveBuffer( tdoc, &output );          // Pretty Print
	if ( rc >= 0 )
	{
#ifdef _DEBUG
		//if ( rc > 0 )
		//	WriteAllToFile("f:/work/windows/wxUrlRefresh/data/error.xml", (char*)errbuf.bp, errbuf.size);
		WriteAllToFile(path1, (char*)output.bp, output.size);
#endif

	}
	else
		wxLogError("tidyFail");

	tidyBufFree( &output );
	tidyBufFree( &errbuf );
	tidyRelease( tdoc );
	if (pBuffer) free(pBuffer);


	wxLogMessage("Fetch data!");
	// 解析数据
	TiXmlDocument doc(path1);
	if (doc.LoadFile()) 
	{
		// root
		CTiXmlProxyVistor vistor(&m_array);
		TiXmlElement *pRoot = doc.RootElement();
		pRoot->Accept(&vistor);
	}
	else
	{
		wxLogMessage("shit");
		return -2;
	}
	return 0;
}

/////////////////////////////////
// CProxyDataConfig
/////////////////////////////////
CProxyDataConfig * CProxyDataConfig::GetHandle()
{
	static CProxyDataConfig *config = NULL;

	if (!config)
	{
		config = new CProxyDataConfig;
		config->m_path = wxGetCwd() + "/data/";
		if (!wxDirExists(config->m_path))
			wxMkdir(config->m_path);
		config->m_path += "proxy_data.xml";
		config->Load();
	}

	return config;
}


bool CProxyDataConfig::Load()
{
	if (!wxFileExists(m_path))
		return false;

	TiXmlDocument doc(m_path);

	if (doc.LoadFile()) 
	{
		TiXmlElement *pRoot = doc.RootElement();
		TiXmlElement *pChild = pRoot->FirstChildElement();
		while(pChild) 
		{
			CProxyData data;
			data.host = pChild->Attribute("host");
			data.port = pChild->Attribute("port");
			Add(&data);
			pChild = pChild->NextSiblingElement();
		}
		return true;
	}

	return false;
}

bool CProxyDataConfig::Save()
{
	wxMutexLocker locker(m_mutext);
	CProxyDataHash::iterator it;

	TiXmlDocument doc;
	TiXmlElement *pRoot = new TiXmlElement("CONFIG");
	doc.LinkEndChild(pRoot);

	for( it = m_hash.begin(); it != m_hash.end(); ++it )
	{
		wxString key = it->first;
		CProxyData *pData = it->second;
		TiXmlElement *pElement = new TiXmlElement("PROXY");
		pElement->SetAttribute("proxy_url", key);
		pElement->SetAttribute("host", pData->host);
		pElement->SetAttribute("port", pData->port);
		pRoot->LinkEndChild(pElement);
	}

	doc.SaveFile(m_path);
	return true;
}

bool CProxyDataConfig::Add( CProxyData *pData )
{
	wxMutexLocker locker(m_mutext);
	CProxyData *pTmpData;
	wxString key;
	key.Printf("http://%s:%s", pData->host, pData->port);

	pTmpData = m_hash[key];
	if (pTmpData)
		return false;

	pTmpData = new CProxyData;
	*pTmpData = *pData;
	m_hash[key] = pTmpData;
	return true;
}

bool CProxyDataConfig::Remove( CProxyData *pData )
{
	wxMutexLocker locker(m_mutext);
	CProxyData *pTmpData;
	wxString key;
	key.Printf("http://%s:%s", pData->host, pData->port);

	pTmpData = m_hash[key];
	if (!pTmpData)
		return false;

	m_hash.erase(key);
	delete pTmpData;
	return true;
}

CProxyDataConfig::~CProxyDataConfig()
{
	wxMutexLocker locker(m_mutext);
	CProxyDataHash::iterator it;
	for( it = m_hash.begin(); it != m_hash.end(); ++it )
	{
		wxString key = it->first;
		CProxyData *pData = it->second;

		delete pData;
	}

	m_hash.clear();
}

int CProxyDataConfig::GetCount()
{
	wxMutexLocker locker(m_mutext);
	return m_hash.size();
}

wxString CProxyDataConfig::GetProxyHttp( int pos )
{
	wxMutexLocker locker(m_mutext);
	CProxyDataHash::iterator it;
	int i=0;

	if (pos>GetCount())
		return wxEmptyString;

	for( it = m_hash.begin(); it != m_hash.end(); ++it )
	{
		wxString key = it->first;
		if (i++==pos)
			return key;
	}
	return wxEmptyString;
}

bool CProxyDataConfig::Remove( wxString key )
{
	wxMutexLocker locker(m_mutext);
	CProxyData *pTmpData;

	pTmpData = m_hash[key];
	if (!pTmpData)
		return false;

	m_hash.erase(key);
	delete pTmpData;
	return true;
}