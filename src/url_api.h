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
	int Run();

	wxArrayPtrVoid &GetArray() { return m_array;}	

public:
	wxArrayPtrVoid m_array;
};
#endif //_URL_API_H_H


