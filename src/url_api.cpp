#include "url_api.h"
#include "tinyxml/tinyxml.h"
#include <tidy.h>
#include <buffio.h>
#include <wx/file.h>
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
		if (!wxStrcmp(element.Value(), "tr") && !wxStrcmp(element.Attribute("bgcolor"), "#FFFFFF"))
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
		return true; 
	}
public:
	wxArrayPtrVoid *m_pArray;
	CProxyData *m_pProxyData;
};

int CProxyParse::Run()
{
	char *pBuffer;
	//http://www.51proxied.com/http_non_anonymous.html
	wxString path = wxT("f:/work/windows/wxUrlRefresh/data/最新透明HTTP代理服务器.htm");
	wxString path1 = wxT("f:/work/windows/wxUrlRefresh/data/result.xml");
	if (!wxFileExists(path))
		return -1;

	if (wxFileExists(path1)) 
	{
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
		}


		return 0;
	}

	wxFile f(path);
	if (!f.IsOpened()) 
		return -2;

	pBuffer = (char*)calloc(f.Length()+1, 1);
	f.Read(pBuffer, f.Length());
	f.Close();

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
		if ( rc > 0 )
			WriteAllToFile("f:/work/windows/wxUrlRefresh/data/error.xml", (char*)errbuf.bp, errbuf.size);
		WriteAllToFile("f:/work/windows/wxUrlRefresh/data/result.xml", (char*)output.bp, output.size);
#endif

	}
	else
		wxLogError("tidyFail");


	tidyBufFree( &output );
	tidyBufFree( &errbuf );
	tidyRelease( tdoc );

	if (pBuffer) free(pBuffer);

	return 0;
}
