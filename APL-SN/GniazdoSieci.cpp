// CGniazdoSieci.cpp: plik implementacji
//

#include "pch.h"
#include "GniazdoSieci.h"
#include "KomunikatySieci.h"



//inicjuj zmienne statyczne używane wątku	
int			CGniazdoSieci::m_LicznikInstancji = 0;
HANDLE		CGniazdoSieci::m_hZdarzeniePolaczonoEth[LICZBA_INSTANCJI] = { NULL, NULL };
HANDLE		CGniazdoSieci::m_hZdarzenieOdebranoEth[LICZBA_INSTANCJI] = { NULL, NULL };


CGniazdoSieci::CGniazdoSieci()
: m_pWnd(NULL)
, m_bPolaczone(FALSE)
{
	m_wskaznikInstancji = m_LicznikInstancji++;
	m_hZdarzeniePolaczonoEth[m_wskaznikInstancji] = CreateEvent(NULL, false, false, _T("")); // auto-reset event, non-signalled state
	m_hZdarzenieOdebranoEth[m_wskaznikInstancji] = CreateEvent(NULL, false, false, _T("")); // auto-reset event, non-signalled state
	
}

CGniazdoSieci::~CGniazdoSieci()
{
	m_wskaznikInstancji = --m_wskaznikInstancji;
	CloseHandle(m_hZdarzeniePolaczonoEth[m_wskaznikInstancji]);
	CloseHandle(m_hZdarzenieOdebranoEth[m_wskaznikInstancji]);
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
		SetEvent(m_hZdarzeniePolaczonoEth[m_wskaznikInstancji]);		//generuj zdarzenie, ktre będzie odebrane w CProtokol::WlasciwyWatekSluchajPortuEth()
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
		SetEvent(m_hZdarzeniePolaczonoEth[m_wskaznikInstancji]);		//generuj zdarzenie, ktre będzie odebrane w CProtokol::WlasciwyWatekSluchajPortuEth()
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
		SetEvent(m_hZdarzenieOdebranoEth[m_wskaznikInstancji]);		//generuj zdarzenie, ktre będzie odebrane w CProtokol::WlasciwyWatekSluchajPortuEth()
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
