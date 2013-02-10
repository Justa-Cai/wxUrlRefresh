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

	wxString GetContent();

	wxString GetErrorString();

public:
	/// write data call back
	size_t OnWriteData(void *buffer, size_t size, size_t nmemb, void *userp);
	virtual int OnProgress(double dltotal, double dlnow, double ultotal, double ulnow);
	int OnHeaderData(void *ptr, size_t size, size_t nmemb);
	void SetTimeout(int time) {m_timeout = time;}
	void SetUseGzip(bool v = true) {m_bGzip = v;}

private:
	int m_type, m_timeout;
	wxString m_url, m_proxy, m_content;
	CURLcode m_ret;
	char *m_pBuf;
	size_t m_length;
	bool m_bGzip, m_bGBK;
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
	~CProxyParse()
	{
		m_array.Clear();
	}
	/*! \return 操作结果
	 *          - -1 获取网页内容错误
	 */
	int Run(wxString url, int timeout=10);
	int RunFromMem(wxString content);

public:
	wxArrayPtrVoid m_array;
};

WX_DECLARE_HASH_MAP( wxString, CProxyData*, wxStringHash, wxStringEqual, CProxyDataHash);
/*! \brief CProxyDataConfig
 *  代理数据 存储
 */
class CProxyDataConfig
{
public:
	~CProxyDataConfig();

	static CProxyDataConfig *GetHandle();

public:
	/*! \brief 从文件中加载代理数据
	 */
	bool Load();

	/*! \brief 保存当前的配置
	 */
	bool Save();

	/*! \brief 增加代理数据 
	 *
	 *  \note 处理代理唯一性
	 */
	bool Add(CProxyData *pData);

	/*! \brief 移除某个代理数据
	 */
	bool Remove(CProxyData *pData);

	bool Remove(wxString key);


	/*! \brief 获取代理数据 总量
	 *
	 */
	int GetCount();

	/*! \brief 获取数据库中 某个代理网址
	 */
	wxString GetProxyHttp(int pos);
public:
	CProxyDataHash m_hash;
	wxMutex m_mutext;
	wxString m_path;
};
#endif //_URL_API_H_H


