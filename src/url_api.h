#ifndef _URL_API_H_H
#define _URL_API_H_H
#include "curl/curl.h"
#include "wx/wx.h"

enum URL_OPT_TYPE {
	URL_OPT_FETCH,    /*!< ��ȡ������ҳ���� */
	URL_OPT_REFRESH,  /*!< ˢ��ģʽ���ڵ�һ�����ݹ����󣬷���0x0��������������ݣ��ӿ�ˢ�� */
};

/*! \class CUrlOpt
 *  \brief ��ַ������
 * 
 *  ��ַ�����࣬ˢ�»��߻�ȡ��ַ���� 
 */
class CUrlOpt
{
public:
	CUrlOpt(wxString url, URL_OPT_TYPE type=URL_OPT_REFRESH, wxString proxy=wxEmptyString);
	~CUrlOpt();

public:
	/*! \brief ��ʼ���в���
	 *  
	 *  \return �������
	 *  \retval 0     �ɹ�
	 *  \retval ����  ����
	 *  \retval -1    ��ҳ��ַΪ��
	 *
	 *  \note �ò�������
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

/*! \brief ��������
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
 *  �������ݴ���
 */
class CProxyParse
{
public:
	~CProxyParse()
	{
		m_array.Clear();
	}
	/*! \return �������
	 *          - -1 ��ȡ��ҳ���ݴ���
	 */
	int Run(wxString url, int timeout=10);
	int RunFromMem(wxString content);

public:
	wxArrayPtrVoid m_array;
};

WX_DECLARE_HASH_MAP( wxString, CProxyData*, wxStringHash, wxStringEqual, CProxyDataHash);
/*! \brief CProxyDataConfig
 *  �������� �洢
 */
class CProxyDataConfig
{
public:
	~CProxyDataConfig();

	static CProxyDataConfig *GetHandle();

public:
	/*! \brief ���ļ��м��ش�������
	 */
	bool Load();

	/*! \brief ���浱ǰ������
	 */
	bool Save();

	/*! \brief ���Ӵ������� 
	 *
	 *  \note �������Ψһ��
	 */
	bool Add(CProxyData *pData);

	/*! \brief �Ƴ�ĳ����������
	 */
	bool Remove(CProxyData *pData);

	bool Remove(wxString key);


	/*! \brief ��ȡ�������� ����
	 *
	 */
	int GetCount();

	/*! \brief ��ȡ���ݿ��� ĳ��������ַ
	 */
	wxString GetProxyHttp(int pos);
public:
	CProxyDataHash m_hash;
	wxMutex m_mutext;
	wxString m_path;
};
#endif //_URL_API_H_H


