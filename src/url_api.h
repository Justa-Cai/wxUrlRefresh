#ifndef _URL_API_H_H
#define _URL_API_H_H
#include "curl/curl.h"
#include "wx/wx.h"

enum URL_OPT_TYPE {
	URL_OPT_FETCH,    /*!< 获取整个网页内容 */
	URL_OPT_REFRESH,  /*!< 刷新模式，在第一次数据过来后，返回0x0，丢弃后面的数据，加快刷新 */
};

/*! \class CUrlOpt
 *  \brief 网址操作类
 * 
 *  网址操作类，刷新或者获取网址内容 
 */
class CUrlOpt
{
public:
	CUrlOpt(wxString url, URL_OPT_TYPE type=URL_OPT_REFRESH, wxString proxy=wxEmptyString);
	~CUrlOpt();

public:
	/*! \brief 开始运行操作
	 *  
	 *  \return 操作结果
	 *  \retval 0     成功
	 *  \retval 其他  错误
	 *  \retval -1    网页地址为空
	 *
	 *  \note 该操作阻塞
	 */
	CURLcode Run();

	wxString &GetContent();

	wxString GetErrorString();

public:
	/// write data call back
	size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);

private:
	int m_type;
	wxString m_url, m_proxy, m_content;
	CURLcode m_ret;
};

/*! \brief 代理数据
 *
 */
class CProxyData
{
public:
	CProxyData()
	{

	}

	CProxyData(const char *strHost, const char *strPort) 
		:host(strHost)
		,port(strPort)

	{

	}

	CProxyData(wxString &strHost, wxString &strPort) 
		:host(strHost)
		,port(strPort)

	{

	}

public:
	wxString host;
	wxString port;
};

/*! \brief CProxyParse
 *  代理数据处理
 */
class CProxyParse
{
public:
	int Run();

	wxArrayPtrVoid &GetArray() { return m_array;}	

public:
	wxArrayPtrVoid m_array;
};
#endif //_URL_API_H_H


