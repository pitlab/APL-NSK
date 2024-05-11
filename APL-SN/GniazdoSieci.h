#pragma once

// Element docelowy polecenia CGniazdoSieci


class CGniazdoSieci : public CAsyncSocket
{
public:
	CGniazdoSieci();
	virtual ~CGniazdoSieci();
private:
	CView* m_pWnd;
	LPVOID m_pWnd2;

public:
	void UstawRodzica(CView* pWnd);
	//void UstawRodzica2(void* pWnd);


	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);

	BOOL m_bPolaczono;
	UINT m_iPort = 4000;
	CString m_strNazwa = (L"127.0.0.1");
	//CString m_strAdresSieciowy = (L"192.168.1.102");
};


