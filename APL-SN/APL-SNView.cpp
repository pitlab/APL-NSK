
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
#include "KomunikatySieci.h"
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
	ON_WM_INPUT()
	ON_COMMAND(ID_KONFIG_PORT, &CAPLSNView::OnKonfigPort)
	ON_UPDATE_COMMAND_UI(ID_KONFIG_PORT, &CAPLSNView::OnUpdateKonfigPort)
	ON_COMMAND(ID_POLACZ_COM, &CAPLSNView::OnPolaczCom)
	ON_UPDATE_COMMAND_UI(ID_POLACZ_COM, &CAPLSNView::OnUpdatePolaczCom)
	ON_COMMAND(ID_ZROB_ZDJECIE, &CAPLSNView::OnZrobZdjecie)
	ON_UPDATE_COMMAND_UI(ID_ZROB_ZDJECIE, &CAPLSNView::OnUpdateZrobZdjecie)
	END_MESSAGE_MAP()

// Tworzenie/niszczenie obiektu CAPLSNView

CAPLSNView::CAPLSNView() noexcept
: m_bPolaczono(FALSE)
{
	
}


CAPLSNView::~CAPLSNView()
{
	if (m_bPolaczono)
		m_cKomunikacja.Rozlacz();
}


BOOL CAPLSNView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: zmodyfikuj klasę Window lub style w tym miejscu, modyfikując
	//  styl kaskadowy CREATESTRUCT
	uint8_t chErr;

	m_cKomunikacja.UstawRodzica(this);
	m_cKomunikacja.UstawAdresPortuETH(L"127.0.0.1");
	m_cKomunikacja.UstawNumerPortuETH(4000);
	m_cKomunikacja.UstawTypPolaczenia(ETHS);
	chErr = m_cKomunikacja.Polacz(this);
	if (chErr == ERR_OK)
		m_bPolaczono = TRUE;
	else
		m_bPolaczono = FALSE;

	return CView::PreCreateWindow(cs);
	return TRUE;
}

// Rysowanie obiektu CAPLSNView

void CAPLSNView::OnDraw(CDC* pDC)
{
	uint32_t x, y;
	uint8_t chKolor[3];
	uint16_t sPix;
	COLORREF crKolor;
	CAPLSNDoc* pDoc = GetDocument();
	RECT prost;
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: tutaj dodaj kod rysowania danych natywnych
	if (pDoc->m_bZdjecieGotowe)
	{
		for (y = 0; y < 240; y++)
		{
			for (x = 0; x < 320; x++)
			{
				sPix = pDoc->m_sZdjecie[y * 320 + x];
				chKolor[0] = ((sPix & 0xF800) >> 11)*8;		//R
				chKolor[1] = ((sPix & 0x070E) >> 5)*4;		//G
				chKolor[2] = (sPix & 0x001F)*8;			//B
				crKolor = RGB(chKolor[0], chKolor[1], chKolor[2]);

				pDC->SetPixel(x, y, crKolor);
			}
		}
		pDoc->m_bZdjecieGotowe = FALSE;
	}

	//rysuj pasek postepu
	pDC->GetBoundsRect(&prost, DCB_RESET);
	float fPrzyrost = prost.right  / m_sLiczbaFragmentowPaskaPostepu;

	prost.top = prost.bottom - 10;
	prost.right = (uint32_t)(m_sBiezacyStanPaskaPostepu * fPrzyrost);
	pDC->Rectangle(&prost);}


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





// Procedury obsługi komunikatów CAPLSNView



///////////////////////////////////////////////////////////////////////////////////////////////////
// Odbiera komunikat WM_INPUT generowany przez zdarzenia klasy CGniazdoSieci i przekazuje sterowanie do klasy protokołu
// [i] nInputcode - identyfikator zdarzenia
// [i] hRawInput - wolne do wykorzystania
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::OnRawInput(UINT nInputcode, HRAWINPUT hRawInput)
{
	//char chBufor[1025];
	//int iRozmiar;

	CAPLSNDoc* pDoc = GetDocument();
	assert(pDoc);

	//iRozmiar = sizeof(chBufor);
	switch (nInputcode)
	{
	case ON_ACCEPT:		m_cKomunikacja.AkceptujPolaczenieETH();
		pDoc->SetTitle(L"Ustanowiono połączenie ETH");
		break;

	case ON_SEND:	//m_cKomunikacja.WyslanoDaneETH();
		pDoc->SetTitle(L"Wysłano dane");
		break;

	case ON_RECEIVE:	//m_cKomunikacja.OdebranoDaneETH();
		//pDoc->SetTitle(m_cKomunikacja.m_vRoj[0].strNazwa);		
		break;

	case ON_CLOSE:		
		pDoc->SetTitle(L"Zamknięto port ETH");
		break;
	}

}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Otwiera okno aktualiacji parametrów portów komunikacyjnych
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::OnKonfigPort()
{
	// TODO: Dodaj tutaj swój kod procedury obsługi polecenia
	m_cKonfigPolacz.DoModal();
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Aktualizuje stan przycisku połącz eth w pasku narzędzi
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::OnUpdateKonfigPort(CCmdUI* pCmdUI)
{
	// TODO: Dodaj tutaj swój kod procedury obsługi polecenia uaktualnienia UI
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Łączy się z portem com / UART
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::OnPolaczCom()
{
	m_cKomunikacja.UstawTypPolaczenia(UART);
	m_cKomunikacja.UstawNumerPortuUART(6);
	m_cKomunikacja.UstawPredkoscPortuUART(115200);
	if (m_cKomunikacja.CzyPolaczonoUart())
		m_cKomunikacja.Rozlacz();
	else
		m_cKomunikacja.Polacz(this);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Aktualizuje stan przycisku połącz com w pasku narzędzi
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::OnUpdatePolaczCom(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_cKomunikacja.CzyPolaczonoUart());
}




///////////////////////////////////////////////////////////////////////////////////////////////////
// wywyła polecenie zrobienia zdjecia
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::OnZrobZdjecie()
{
	uint8_t chErr;

	CAPLSNDoc* pDoc = GetDocument();
	assert(pDoc);

	uint32_t rozmiar = sizeof(pDoc->m_sZdjecie);
	//uruchom wątek aktualizujący pasek postępu pobierania zdjęcia
	m_bKoniecWatkuPaskaPostepu = FALSE;
	pWskWatkuPaskaPostepu = AfxBeginThread((AFX_THREADPROC)WatekRysujPasekPostepu, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);

	m_sLiczbaFragmentowPaskaPostepu = rozmiar / ROZM_DANYCH_UART;
	m_sBiezacyStanPaskaPostepu = 0;
	pDoc->m_bZdjecieGotowe = FALSE;
	chErr = m_cKomunikacja.ZrobZdjecie(2, 320, 240, pDoc->m_sZdjecie);
	if (chErr == ERR_OK)
	{
		pDoc->m_bZdjecieGotowe = TRUE;
		m_bKoniecWatkuPaskaPostepu = TRUE;
		Invalidate(TRUE);
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Aktualizuje stan przycisku zrób zdjęcie w pasku narzędzi
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::OnUpdateZrobZdjecie(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_cKomunikacja.CzyPolaczonoUart());
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Wywołuje wątek rysujący pasek postępu
// Zwraca: kod błędu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CAPLSNView::WatekRysujPasekPostepu(LPVOID pParam)
{
	return reinterpret_cast<CAPLSNView*>(pParam)->WlasciwyWatekRysujPasekPostepu();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wątek  rysujący pasek postępu
// Zwraca: kod błędu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CAPLSNView::WlasciwyWatekRysujPasekPostepu()
{
	uint32_t nErr;

	while (!m_bKoniecWatkuPaskaPostepu)
	{
		nErr = WaitForSingleObject(m_cKomunikacja.m_hZdarzeniePaczkaDanych, 200);
		if (nErr != WAIT_TIMEOUT)
		{
			m_sBiezacyStanPaskaPostepu++;
			Invalidate(TRUE);
		}
	}
	return ERR_OK;
}
