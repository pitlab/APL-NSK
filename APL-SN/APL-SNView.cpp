
// APL-SNView.cpp: implementacja klasy CAPLSNView
//

#include "pch.h"
#include "framework.h"
// Element SHARED_HANDLERS można zdefiniować w projekcie ATL z implementacją podglądu, miniaturze
// procedury obsługi serializacji i filtrów wyszukiwania oraz umożliwia udostępnianie kodu dokumentu w tym projekcie.
#ifndef SHARED_HANDLERS
#include "APL-SN.h"
#endif

#include "APL-SNDoc.h"
#include "APL-SNView.h"
#include "komunikaty_sieci.h"
#include "Errors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAPLSNView

IMPLEMENT_DYNCREATE(CAPLSNView, CView)

BEGIN_MESSAGE_MAP(CAPLSNView, CView)
	// Standardowe polecenia drukowania
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CAPLSNView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
//	ON_MESSAGE(111, &CAPLSNView::OnAccept)
//ON_WM_XBUTTONUP()
ON_WM_INPUT()
END_MESSAGE_MAP()

// Tworzenie/niszczenie obiektu CAPLSNView

CAPLSNView::CAPLSNView() noexcept
{
	uint8_t chErr = m_cProtokol.PolaczPort(ETH, 4000, 0, L"127.0.0.1", this);
	if (chErr == ERR_OK)
		m_bPolaczonoETH = TRUE;
	else
		m_bPolaczonoETH = FALSE;

	chErr = m_cProtokol.PolaczPort(UART, 0, 115200, L"COM5", this);
	if (chErr == ERR_OK)
		m_bPolaczonoUART = TRUE;
	else
		m_bPolaczonoUART = FALSE;
}


CAPLSNView::~CAPLSNView()
{
	if (m_bPolaczonoETH)
		m_cProtokol.ZamknijPort(ETH);

	if (m_bPolaczonoUART)
		m_cProtokol.ZamknijPort(UART);
}

BOOL CAPLSNView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: zmodyfikuj klasę Window lub style w tym miejscu, modyfikując
	//  styl kaskadowy CREATESTRUCT

	return CView::PreCreateWindow(cs);
	return TRUE;
}

// Rysowanie obiektu CAPLSNView

void CAPLSNView::OnDraw(CDC* /*pDC*/)
{
	CAPLSNDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: tutaj dodaj kod rysowania danych natywnych
}


// Drukowanie obiektu CAPLSNView


void CAPLSNView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CAPLSNView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// domyślne przygotowanie
	return DoPreparePrinting(pInfo);
}

void CAPLSNView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: dodaj kolejne inicjowanie przed rozpoczęciem drukowania
}

void CAPLSNView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: dodaj czyszczenie po drukowaniu
}

void CAPLSNView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CAPLSNView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Diagnostyka klasy CAPLSNView

#ifdef _DEBUG
void CAPLSNView::AssertValid() const
{
	CView::AssertValid();
}

void CAPLSNView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAPLSNDoc* CAPLSNView::GetDocument() const // wbudowana jest wersja bez debugowania
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAPLSNDoc)));
	return (CAPLSNDoc*)m_pDocument;
}
#endif //_DEBUG


//połączenie sieciowe APL do okna widoku

void CAPLSNView::OnPolacz()
{

}

void CAPLSNView::OnAkceptuj()
{
	
}



// Procedury obsługi komunikatów CAPLSNView



///////////////////////////////////////////////////////////////////////////////////////////////////
// Odbiera komunikat WM_INPUT generowany przez zdarzenia klasy CGniazdoSieci i przekazuje sterowanie do klasy protokołu
// [i] nInputcode - identyfikator zdarzenia
// [i] hRawInput - wolne do wykorzystania
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::OnRawInput(UINT nInputcode, HRAWINPUT hRawInput)
{
	char chBufor[1025];
	int iRozmiar, iOdebrano;

	CAPLSNDoc* pDoc = GetDocument();
	assert(pDoc);

	iRozmiar = sizeof(chBufor);
	switch (nInputcode)
	{
	case ON_ACCEPT:		m_cProtokol.AkceptujPolaczenieETH();	
		pDoc->SetTitle(L"Ustanowiono połączenie ETH");
		break;

	case ON_SEND:	m_cProtokol.WyslanoDaneETH();
		//pDoc->SetTitle(L"Wysłano dane");
		break;

	case ON_RECEIVE:	m_cProtokol.OdbierzDaneETH();
		pDoc->SetTitle(L"Odebrano dane");
		break;

	case ON_CLOSE:		m_cProtokol.ZamknijPort(ETH);			
		pDoc->SetTitle(L"Zamknięto port ETH");
		break;
	}

}
