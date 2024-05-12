// CGniazdoSieci.cpp: plik implementacji
//

#include "pch.h"
#include "GniazdoSieci.h"
#include "KomunikatySieci.h"


// CGniazdoSieci

CGniazdoSieci::CGniazdoSieci()
: m_pWnd(NULL)
, m_bPolaczone(FALSE)
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


void CGniazdoSieci::OnAccept(int nErrorCode)
{
	m_pWnd->SendMessage(WM_INPUT, ON_ACCEPT, nErrorCode);
	CAsyncSocket::OnAccept(nErrorCode);
}


void CGniazdoSieci::OnClose(int nErrorCode)
{
	m_pWnd->SendMessage(WM_INPUT, ON_CLOSE, nErrorCode);
	m_bPolaczone = FALSE;
	CAsyncSocket::OnClose(nErrorCode);
}


void CGniazdoSieci::OnConnect(int nErrorCode)
{
	m_pWnd->SendMessage(WM_INPUT, ON_CONNECT, nErrorCode);
	m_bPolaczone = TRUE;
	CAsyncSocket::OnConnect(nErrorCode);
}


void CGniazdoSieci::OnReceive(int nErrorCode)
{
	m_pWnd->SendMessage(WM_INPUT, ON_RECEIVE, nErrorCode);
	CAsyncSocket::OnReceive(nErrorCode);
}


void CGniazdoSieci::OnSend(int nErrorCode)
{
	m_pWnd->SendMessage(WM_INPUT, ON_SEND, nErrorCode);
	CAsyncSocket::OnSend(nErrorCode);
}
