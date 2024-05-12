#pragma once

// Element docelowy polecenia CGniazdoSieci
class CGniazdoSieci : public CAsyncSocket
{
public:
	CGniazdoSieci();
	virtual ~CGniazdoSieci();
	BOOL m_bPolaczone;


private:
	CView* m_pWnd;
	

public:
	void UstawRodzica(CView* pWnd);
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
};


