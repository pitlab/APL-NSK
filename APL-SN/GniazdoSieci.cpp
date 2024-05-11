// CGniazdoSieci.cpp: plik implementacji
//

#include "pch.h"
#include "GniazdoSieci.h"
#include "komunikaty_sieci.h"


// CGniazdoSieci

CGniazdoSieci::CGniazdoSieci()
{
}

CGniazdoSieci::~CGniazdoSieci()
{
}


// Funkcje składowe CGniazdoSieci 


void CGniazdoSieci::UstawRodzica(CView* pWnd)
{
	m_pWnd = pWnd;
}


/*void CGniazdoSieci::UstawRodzica2(CProtokol* pWnd)
{
	m_pWnd2 = pWnd;
}*/

void CGniazdoSieci::OnAccept(int nErrorCode)
{
	m_pWnd->SendMessage(WM_INPUT, ON_ACCEPT, nErrorCode);
	m_bPolaczono = TRUE;
	CAsyncSocket::OnAccept(nErrorCode);
}


void CGniazdoSieci::OnClose(int nErrorCode)
{
	// TODO: Dodaj tutaj swój wyspecjalizowany kod i/lub wywołaj klasę bazową
	m_pWnd->SendMessage(WM_INPUT, ON_CLOSE, nErrorCode);
	CAsyncSocket::OnClose(nErrorCode);
}


void CGniazdoSieci::OnConnect(int nErrorCode)
{
	// TODO: Dodaj tutaj swój wyspecjalizowany kod i/lub wywołaj klasę bazową
	//m_pWnd->SendMessage(WM_INPUT, ON_CONNECT, nErrorCode);
	
	CAsyncSocket::OnConnect(nErrorCode);
}


void CGniazdoSieci::OnReceive(int nErrorCode)
{
	// TODO: Dodaj tutaj swój wyspecjalizowany kod i/lub wywołaj klasę bazową
	m_pWnd->SendMessage(WM_INPUT, ON_RECEIVE, nErrorCode);
	CAsyncSocket::OnReceive(nErrorCode);
}


void CGniazdoSieci::OnSend(int nErrorCode)
{
	// TODO: Dodaj tutaj swój wyspecjalizowany kod i/lub wywołaj klasę bazową
	m_pWnd->SendMessage(WM_INPUT, ON_SEND, nErrorCode);
	CAsyncSocket::OnSend(nErrorCode);
}
