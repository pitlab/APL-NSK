#pragma once
#define ROZM_DANYCH_WE	1024

// Element docelowy polecenia CGniazdoSieci
class CGniazdoSieci : public CAsyncSocket
{
public:
	CGniazdoSieci();
	virtual ~CGniazdoSieci();
	BOOL m_bPolaczone;
	void UstawRodzica(CView* pWnd);
	static HANDLE hZdarzeniePolaczonoEth;
	static HANDLE hZdarzenieOdebranoEth;


private:
	CView* m_pWnd;
	//uint8_t m_chBufOdb[ROZM_DANYCH_WE];

protected:
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);

public:
	

};


