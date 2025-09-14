#pragma once
#define ROZM_DANYCH_WE		1024
#define LICZBA_INSTANCJI	2		//obecnie mamy gniazdo s³uchaj¹ce i gniazdo po³aczenia

// Element docelowy polecenia CGniazdoSieci
class CGniazdoSieci : public CAsyncSocket
{
public:
	CGniazdoSieci();
	virtual ~CGniazdoSieci();
	BOOL m_bPolaczone;
	BOOL CzyPolaczonoEth() { return m_bPolaczone; }
	void UstawRodzica(CView* pWnd);
	int m_wskaznikInstancji;
	static int m_LicznikInstancji;
	static HANDLE m_hZdarzeniePolaczonoEth[LICZBA_INSTANCJI];	//zmienne statyczne maj¹ 2 uintancje poniewa¿ s¹ uruchamiane: gniazdo s³uchajace (serwer) i gniazdo po³¹czenia (klient) 
	static HANDLE m_hZdarzenieOdebranoEth[LICZBA_INSTANCJI];


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


//CGniazdoSieci& getGniazdoSieci();

