// CGniazdoSieci.cpp: plik implementacji
//

#include "pch.h"
#include "GniazdoSieci.h"
#include "KomunikatySieci.h"



//inicjuj zmienne statyczne używane wątku
HANDLE		CGniazdoSieci::m_hZdarzeniePolaczonoEth = NULL;
HANDLE		CGniazdoSieci::m_hZdarzenieOdebranoEth = NULL;


CGniazdoSieci::CGniazdoSieci()
: m_pWnd(NULL)
, m_bPolaczone(FALSE)
{
	m_hZdarzeniePolaczonoEth = CreateEvent(NULL, false, false, _T("")); // auto-reset event, non-signalled state
	m_hZdarzenieOdebranoEth = CreateEvent(NULL, false, false, _T("")); // auto-reset event, non-signalled state
}

CGniazdoSieci::~CGniazdoSieci()
{
	CloseHandle(m_hZdarzeniePolaczonoEth);
	CloseHandle(m_hZdarzenieOdebranoEth);

}


// Funkcje składowe CGniazdoSieci 


//////////////////////////////////////////////////////////////////////////////////////////////////
// Ustawia wskaźnik na okno gdzie będą lądowały komunikaty generowane przez SendMessage()
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CGniazdoSieci::UstawRodzica(CView* pWnd)
{
	m_pWnd = pWnd;
}



//////////////////////////////////////////////////////////////////////////////////////////////////
// Sygnalizuje zdarzenie nawiązania połączenia przez serwer
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CGniazdoSieci::OnAccept(int nErrorCode)
{
	if (nErrorCode == 0)
	{
		m_pWnd->SendMessage(WM_INPUT, ON_ACCEPT, nErrorCode);
		SetEvent(m_hZdarzeniePolaczonoEth);		//generuj zdarzenie, ktre będzie odebrane w CProtokol::WlasciwyWatekSluchajPortuEth()
	}
	CAsyncSocket::OnAccept(nErrorCode);
}



//////////////////////////////////////////////////////////////////////////////////////////////////
// Sygnalizuje zdarzenie zakończenia połączenie
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CGniazdoSieci::OnClose(int nErrorCode)
{
	m_pWnd->SendMessage(WM_INPUT, ON_CLOSE, nErrorCode);
	m_bPolaczone = FALSE;
	CAsyncSocket::OnClose(nErrorCode);
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// Sygnalizuje zdarzenie nawiązania połączenia przez klienta
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CGniazdoSieci::OnConnect(int nErrorCode)
{
	if (nErrorCode == 0)
	{
		m_pWnd->SendMessage(WM_INPUT, ON_CONNECT, nErrorCode);
		m_bPolaczone = TRUE;
		SetEvent(m_hZdarzeniePolaczonoEth);		//generuj zdarzenie, ktre będzie odebrane w CProtokol::WlasciwyWatekSluchajPortuEth()
	}
	else
		m_bPolaczone = FALSE;	
	CAsyncSocket::OnConnect(nErrorCode);
}



//////////////////////////////////////////////////////////////////////////////////////////////////
// Sygnalizuje zdarzenie odbioru danych
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CGniazdoSieci::OnReceive(int nErrorCode)
{
	if (nErrorCode == 0)
	{
		SetEvent(m_hZdarzenieOdebranoEth);		//generuj zdarzenie, ktre będzie odebrane w CProtokol::WlasciwyWatekSluchajPortuEth()
		m_pWnd->SendMessage(WM_INPUT, ON_RECEIVE, nErrorCode);
	}
	CAsyncSocket::OnReceive(nErrorCode);
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// Sygnalizuje zdarzenie wysłania danych
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CGniazdoSieci::OnSend(int nErrorCode)
{
	m_pWnd->SendMessage(WM_INPUT, ON_SEND, nErrorCode);
	CAsyncSocket::OnSend(nErrorCode);
}
