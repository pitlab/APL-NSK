
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
#include <stdio.h>
#include <string.h>

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
	ON_COMMAND(ID_ZAPISZ_PAMIEC, &CAPLSNView::OnZapiszPamiec)
	ON_UPDATE_COMMAND_UI(ID_ZAPISZ_PAMIEC, &CAPLSNView::OnUpdateZapiszPamiec)
	ON_WM_SIZE()
	ON_REGISTERED_MESSAGE(AFX_WM_DRAW2D, &CAPLSNView::OnDraw2d)
	ON_WM_MOUSEWHEEL()
	ON_WM_HOTKEY()
	ON_WM_KEYDOWN()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_COMMAND(ID_OLE_INSERT_NEW, &CAPLSNView::OnOleInsertNew)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

// Tworzenie/niszczenie obiektu CAPLSNView

CAPLSNView::CAPLSNView() noexcept
: m_bPolaczono(FALSE)
, m_bRysujPasekPostepu(FALSE)
, m_bRysujTelemetrie(FALSE)
, m_bRysujLog(FALSE)
, m_bKoniecWatkuOdswiezaniaTelemtrii(TRUE)
, m_chAdresAutopilota(2)
, m_fZoomPionowo(1.0f)
, m_fZoomPoziomo(1.0f)
, m_nVscroll(0)
, m_bOknoGotowe(FALSE)
, m_nIloscDanychWykresu(0)
, m_nMaxScrollPoziomo(0)
, m_nBiezacyScrollPoziomo(0)
{
	getKomunikacja().m_chAdresAutopilota = m_chAdresAutopilota;	//przekaż domyślny adres do klasy komunikacyjnej
	// Enable D2D support for this window:
	EnableD2DSupport();

	// Initialize D2D resources:
	m_pBrushBlack = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::Black));
	m_pBrushWykresuR = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::Red));
	m_pBrushWykresuG = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::Green));
	m_pBrushWykresuB = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::Blue));
	//m_pBrushWykresuR
	m_pTextFormat = new CD2DTextFormat(GetRenderTarget(), _T("Verdana"), 12);

	m_pTextFormat->Get()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	m_pTextFormat->Get()->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	D2D1_GRADIENT_STOP gradientStops[2];
	gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::White);
	gradientStops[0].position = 0.f;
	gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::LightSkyBlue);
	gradientStops[1].position = 1.f;

	m_pLinearGradientBrush = new CD2DLinearGradientBrush(GetRenderTarget(), gradientStops, ARRAYSIZE(gradientStops), D2D1::LinearGradientBrushProperties(D2D1::Point2F(0, 0), D2D1::Point2F(0, 0)));
}


CAPLSNView::~CAPLSNView()
{
	if (m_bPolaczono)
	{
		//CKomunikacja m_cKomunikacja = getKomunikacja();
		getKomunikacja().Rozlacz();
	}
}


BOOL CAPLSNView::PreCreateWindow(CREATESTRUCT& cs)
{
	//CKomunikacja m_cKomunikacja = getKomunikacja();

	// TODO: zmodyfikuj klasę Window lub style w tym miejscu, modyfikując
	//  styl kaskadowy CREATESTRUCT
	m_cObslugaRejestru.CzytajRejestrIP(L"AdresIP", m_chNumerIP);
	m_cObslugaRejestru.CzytajRejestrInt(L"PredkoscCOM", &m_nPredkoscPortuCom);
	m_cObslugaRejestru.CzytajRejestrInt(L"NrPortuCOM", &m_nNumerPortuCom);
	m_cObslugaRejestru.CzytajRejestrInt(L"PortETH", &m_nNumerPortuEth);
	m_cObslugaRejestru.CzytajRejestrInt(L"TypPortu", &m_nTypPolaczenia);

	getKomunikacja().UstawRodzica(this);
	getKomunikacja().UstawAdresPortuETH(L"127.0.0.1");
	getKomunikacja().UstawNumerPortuETH(m_nNumerPortuEth);
	getKomunikacja().UstawTypPolaczenia((uint8_t)m_nTypPolaczenia);
	getKomunikacja().UstawPredkoscPortuUART(m_nPredkoscPortuCom);
	getKomunikacja().UstawNumerPortuUART(m_nNumerPortuCom);

	uint8_t chErr = getKomunikacja().Polacz(this);
	if (chErr == ERR_OK)
	{
		m_bPolaczono = TRUE;		
		if (m_bKoniecWatkuOdswiezaniaTelemtrii)
		{
			m_bKoniecWatkuOdswiezaniaTelemtrii = FALSE;
			pWskWatkuOdswiezaniaTelemetrii = AfxBeginThread((AFX_THREADPROC)WatekInvalidujWytkresTelemetrii, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
		}
	}
	else
		m_bPolaczono = FALSE;
	
	return CView::PreCreateWindow(cs);
	
	return TRUE;
}

// Rysowanie obiektu CAPLSNView

void CAPLSNView::OnDraw(CDC* pDC)
{
	int32_t x, y;
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
		for (y = 0; y < 320; y++)
		{
			for (x = 0; x < 480; x++)
			{
				sPix = pDoc->m_sZdjecie[y * 480 + x];
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
	if (m_bRysujPasekPostepu)
	{
		pDC->GetBoundsRect(&prost, DCB_RESET);
		float fPrzyrost = (float)prost.right / m_sLiczbaFragmentowPaskaPostepu;

		prost.top = prost.bottom - 10;
		prost.right = (uint32_t)(m_sBiezacyStanPaskaPostepu * fPrzyrost);
		pDC->Rectangle(&prost);
	}

	
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


//darzenie aktywacji menu kontekstowego w oknie dokumentu
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
	//CKomunikacja m_cKomunikacja = getKomunikacja();
	//assert(pDoc);

	//iRozmiar = sizeof(chBufor);
	switch (nInputcode)
	{
	case ON_ACCEPT:		getKomunikacja().AkceptujPolaczenieETH();
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
	m_cKonfigPolacz.UstawNumerPortuCom(m_nNumerPortuCom);
	m_cKonfigPolacz.UstawPredkoscPortuCom(m_nPredkoscPortuCom);
	m_cKonfigPolacz.UstawNumerPortuEth(m_nNumerPortuEth);
	m_cKonfigPolacz.UstawAdresIP(m_chNumerIP);
	m_cKonfigPolacz.UstawTypPolaczenia(m_nTypPolaczenia);
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
	//CKomunikacja m_cKomunikacja = getKomunikacja();

	getKomunikacja().UstawTypPolaczenia(UART + m_nTypPolaczenia);
	getKomunikacja().UstawNumerPortuUART(m_nNumerPortuCom);
	getKomunikacja().UstawPredkoscPortuUART(m_nPredkoscPortuCom);
	if (getKomunikacja().CzyPolaczonoUart())
	{
		m_bKoniecWatkuOdswiezaniaTelemtrii = TRUE;
		getKomunikacja().Rozlacz();
	}
	else
	{
		getKomunikacja().Polacz(this);
		if (m_bKoniecWatkuOdswiezaniaTelemtrii)
		{
			m_bKoniecWatkuOdswiezaniaTelemtrii = FALSE;
			pWskWatkuOdswiezaniaTelemetrii = AfxBeginThread((AFX_THREADPROC)WatekInvalidujWytkresTelemetrii, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Aktualizuje stan przycisku połącz com w pasku narzędzi
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::OnUpdatePolaczCom(CCmdUI* pCmdUI)
{
	//CKomunikacja m_cKomunikacja = getKomunikacja();

	pCmdUI->Enable(!getKomunikacja().CzyPolaczonoUart());
}




///////////////////////////////////////////////////////////////////////////////////////////////////
// wywyła polecenie zrobienia zdjecia
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::OnZrobZdjecie()
{
	uint8_t chErr;

	CAPLSNDoc* pDoc = GetDocument();
	//CKomunikacja m_cKomunikacja = getKomunikacja();

	uint32_t rozmiar = sizeof(pDoc->m_sZdjecie);
	//uruchom wątek aktualizujący pasek postępu pobierania zdjęcia
	m_bKoniecWatkuPaskaPostepu = FALSE;
	pWskWatkuPaskaPostepu = AfxBeginThread((AFX_THREADPROC)WatekRysujPasekPostepu, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);

	m_sLiczbaFragmentowPaskaPostepu = rozmiar / ROZM_DANYCH_UART;
	m_sBiezacyStanPaskaPostepu = 0;
	pDoc->m_bZdjecieGotowe = FALSE;
	chErr = getKomunikacja().ZrobZdjecie(pDoc->m_sZdjecie);
	if (chErr == ERR_OK)
	{
		pDoc->m_bZdjecieGotowe = TRUE;
		m_bKoniecWatkuPaskaPostepu = TRUE;
		Invalidate();
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Aktualizuje stan przycisku zrób zdjęcie w pasku narzędzi
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::OnUpdateZrobZdjecie(CCmdUI* pCmdUI)
{
	//CKomunikacja m_cKomunikacja = getKomunikacja();

	pCmdUI->Enable(getKomunikacja().CzyPolaczonoUart());
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
	//CKomunikacja m_cKomunikacja = getKomunikacja();


	while (!m_bKoniecWatkuPaskaPostepu)
	{
		nErr = WaitForSingleObject(getKomunikacja().m_hZdarzeniePaczkaDanych, 200);
		if (nErr != WAIT_TIMEOUT)
		{
			m_sBiezacyStanPaskaPostepu++;
			Invalidate();
		}
	}
	return ERR_OK;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wywołuje wątek rysujący odświeżajacy wukresy telemetrii
// Zwraca: kod błędu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CAPLSNView::WatekInvalidujWytkresTelemetrii(LPVOID pParam)
{
	return reinterpret_cast<CAPLSNView*>(pParam)->WlasciwyWatekInvalidujWytkresTelemetrii();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wątek uruchamiający odświezanie wykresu po przyjściu telemetrii
// Zwraca: kod błędu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CAPLSNView::WlasciwyWatekInvalidujWytkresTelemetrii()
{
	while (!m_bKoniecWatkuOdswiezaniaTelemtrii)
	{
		if (m_bOknoGotowe)
		{
			uint32_t nErr = WaitForSingleObject(m_cProtokol.m_hZdarzenieRamkaTelemetriiGotowa, INFINITE);
			if (nErr != WAIT_TIMEOUT)
			{
				m_bRysujTelemetrie = TRUE;
				Invalidate();
			}
		}
	}
	return ERR_OK;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Otwiera okno dialogowe Zapisz Pamięć Flash
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::OnZapiszPamiec()
{
	m_cDaneFlash.DoModal();
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Aktualizuje stan przycisku Zapisz Pamięć Flash w pasku narzędzi
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::OnUpdateZapiszPamiec(CCmdUI* pCmdUI)
{
	//CKomunikacja m_cKomunikacja = getKomunikacja();

	pCmdUI->Enable(getKomunikacja().CzyPolaczonoUart());
}




void CAPLSNView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Dodaj tutaj swój kod procedury obsługi komunikatów
	m_nSzerokoscOkna = cx;
	
	CAPLSNDoc* pDoc = GetDocument();
	if (pDoc)
	{
		if (pDoc->m_vLog.size())
			m_nIloscDanychWykresu = pDoc->m_vLog[9].vfWartosci.size();
	}

	m_pLinearGradientBrush->SetEndPoint(CPoint(cx, cy));
	UstawScrollOdWidoku();
}



afx_msg LRESULT CAPLSNView::OnDraw2d(WPARAM wParam, LPARAM lParam)
{
	CHwndRenderTarget* pRenderTarget = (CHwndRenderTarget*)lParam;
	ASSERT_VALID(pRenderTarget);
	CD2DPointF pktfPoczatek, pktfKoniec;
	CRect okno;
	GetClientRect(okno);
	m_bOknoGotowe = TRUE;

	pRenderTarget->FillRectangle(okno, m_pLinearGradientBrush);

	//rysowanie wykresów telemetrii
	if (m_bRysujTelemetrie)
	{	
		m_bRysujTelemetrie = FALSE;
		float fZmienna;
		long lLiczbaRamek = (long)getProtokol().m_vRamkaTelemetryczna.size();
		_Telemetria stDaneTele;
		int32_t nIndexRamki;
		long lErr;
		

		//fSkalaX = (float)okno.right / lLiczbaRamek;
		float fSkalaX = m_fZoomPoziomo;
		float fSkalaY = (float)okno.bottom / 40.0f * m_fZoomPionowo;

		//wykres X
		std::vector< CPoint > vPunktyWykresuX;
		//vPunktyWykresuX.reserve(okno.right);
		nIndexRamki = lLiczbaRamek - 1;
		pktfPoczatek.x = (float)m_nBiezacyScrollPoziomo;
		pktfKoniec.x = (1.0f + (float)m_nBiezacyScrollPoziomo) * fSkalaX;
		pktfPoczatek.y = (float)(okno.bottom / 2 + m_nVscroll);
		do    //sprawdzaj wektor ramki od końca aż napełni się wektor punktów wykresu
		{
			stDaneTele = getProtokol().m_vRamkaTelemetryczna[nIndexRamki--];
			if (!stDaneTele.dane.empty())
			{
				lErr = m_cDekoderTelemetrii.PobierzZmienna(&stDaneTele, 0, &fZmienna);
				if (lErr == ERR_OK)
				{
					pktfKoniec.x += fSkalaX;
					pktfKoniec.y = (float)(okno.bottom / 2  + m_nVscroll) - (fZmienna * fSkalaY);
					pRenderTarget->DrawLine(pktfPoczatek, pktfKoniec, m_pBrushWykresuR);
					pktfPoczatek = pktfKoniec;
				}
					//vPunktyWykresuX.insert(vPunktyWykresuX.begin(), CPoint(x++, okno.bottom / 2 - (uint32_t)(fZmienna * fSkalaY)));
			}
		} while ((pktfKoniec.x < okno.right) && (nIndexRamki > 0));	//pobierz danych na szerokość okna lub tyle ile się da

		//wykres Y
		std::vector< CPoint > vPunktyWykresuY;
		//vPunktyWykresuY.reserve(okno.right);
		nIndexRamki = lLiczbaRamek - 1;
		pktfPoczatek.x = (float)m_nBiezacyScrollPoziomo * fSkalaX;
		pktfKoniec.x = (1.0f + (float)m_nBiezacyScrollPoziomo) * fSkalaX;
		pktfPoczatek.y = (float)(okno.bottom / 2 + m_nVscroll);
		do
		{
			stDaneTele = getProtokol().m_vRamkaTelemetryczna[nIndexRamki--];
			if (stDaneTele.dane.size())
			{
				lErr = m_cDekoderTelemetrii.PobierzZmienna(&stDaneTele, 1, &fZmienna);
				if (lErr == ERR_OK)
				{
					pktfKoniec.x += fSkalaX;
					pktfKoniec.y = (float)(okno.bottom / 2 + m_nVscroll) - (fZmienna * fSkalaY);
					pRenderTarget->DrawLine(pktfPoczatek, pktfKoniec, m_pBrushWykresuG);
					pktfPoczatek = pktfKoniec;
				}
					//vPunktyWykresuY.insert(vPunktyWykresuY.begin(), CPoint(x++, okno.bottom / 2 - (uint32_t)(fZmienna * fSkalaY)));
			}
		} while ((pktfKoniec.x < okno.right) && (nIndexRamki > 0));

		//wykres Z
		std::vector< CPoint > vPunktyWykresuZ;
		//vPunktyWykresuZ.reserve(okno.right);
		nIndexRamki = lLiczbaRamek - 1;
		pktfPoczatek.x = (float)m_nBiezacyScrollPoziomo * fSkalaX;
		pktfKoniec.x = (1.0f + (float)m_nBiezacyScrollPoziomo) * fSkalaX;
		pktfPoczatek.y = (float)(okno.bottom / 2 + m_nVscroll);
		do
		{
			stDaneTele = getProtokol().m_vRamkaTelemetryczna[nIndexRamki--];
			if (stDaneTele.dane.size())
			{
				lErr = m_cDekoderTelemetrii.PobierzZmienna(&stDaneTele, 2, &fZmienna);
				if (lErr == ERR_OK)
				{
					pktfKoniec.x += fSkalaX;
					pktfKoniec.y = (float)(okno.bottom / 2 + m_nVscroll) - (fZmienna * fSkalaY);
					pRenderTarget->DrawLine(pktfPoczatek, pktfKoniec, m_pBrushWykresuB);
					pktfPoczatek = pktfKoniec;
				}
					//vPunktyWykresuZ.insert(vPunktyWykresuZ.begin(), CPoint(x++, okno.bottom / 2 - (uint32_t)(fZmienna * fSkalaY)));
			}
		} while ((pktfKoniec.x < okno.right) && (nIndexRamki > 0));
	}

	CAPLSNDoc* pDoc = GetDocument();
	if (pDoc->m_vLog.size() && pDoc->m_bOdczytanoLog)
	{
		m_nIloscDanychWykresu = pDoc->m_vLog[9].vfWartosci.size();
		//float fSkalaX = (float)okno.right / m_nIloscDanychWykresu * m_fZoomPoziomo;
		float fSkalaX = m_fZoomPoziomo;
		float fSkalaY = (float)okno.bottom / 40.0f * m_fZoomPionowo;
		pktfPoczatek.x = 0.0f;
		pktfPoczatek.y = (float)(okno.bottom / 2 + m_nVscroll)  - (pDoc->m_vLog[9].vfWartosci[0] * fSkalaY );
		for (int n = 1; n < m_nIloscDanychWykresu; n++)
		{
			pktfKoniec.x = (float)(n - m_nBiezacyScrollPoziomo) * fSkalaX;	
			pktfKoniec.y = (float)(okno.bottom / 2 + m_nVscroll) - (pDoc->m_vLog[9].vfWartosci[n] * fSkalaY);
			pRenderTarget->DrawLine(pktfPoczatek, pktfKoniec, m_pBrushWykresuB);
			pktfPoczatek = pktfKoniec;
		}
	}
	return TRUE;
}

/// <summary>
/// Reakcja na przekręcenie kółka myszy. Steruje powiększeniem wykresu w poziomie lub pionie
/// </summary>
/// <param name="nFlags"></param>
/// <param name="zDelta"></param>
/// <param name="pt"></param>
/// <returns></returns>
BOOL CAPLSNView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Dodaj tutaj swój kod procedury obsługi komunikatów i/lub wywołaj domyślny
	if (zDelta > 0)
	{
		if (nFlags & MK_CONTROL)
			m_fZoomPionowo *= 1.1f;
		else
			m_fZoomPoziomo *= 1.1f;
	}
	else
	{
		if (nFlags & MK_CONTROL)
			m_fZoomPionowo /= 1.1f;
		else
			m_fZoomPoziomo /= 1.1f; 
	}
	UstawScrollOdWidoku();
	Invalidate();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


/// <summary>
/// Raakcja na naciśnięcie klawisza klawiatury
/// </summary>
/// <param name="nChar"> - zwracany kod przyciśnietego klawisza</param>
/// <param name="nRepCnt"></param>
/// <param name="nFlags"></param>
void CAPLSNView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Dodaj tutaj swój kod procedury obsługi komunikatów i/lub wywołaj domyślny
	switch (nChar)
	{
	case 16: break;	//shift	-	przełacz zoomowanie między poziomem a pionem
	case 17: break;	//crtl
	case 32: break;	//spacja
	case 91: break;	//windows
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CAPLSNView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Dodaj tutaj swój kod procedury obsługi komunikatów i/lub wywołaj domyślny
	//m_nVscroll = nPos;
	switch (nSBCode)
	{
	case SB_BOTTOM:	// Scroll to bottom.

	case SB_ENDSCROLL:	break;		//End scroll.
	case SB_LINEDOWN:	 m_nVscroll += 5;	break;	//Scroll one line down.
	case SB_LINEUP:	 //Scroll one line up.
		if (m_nVscroll > 5)
			m_nVscroll -= 5;
		break;
	case SB_PAGEDOWN: m_nVscroll += 100;	break;	//Scroll one page down.
	case SB_PAGEUP:	 //Scroll one page up.
		if (m_nVscroll > 100)
			m_nVscroll -= 100;
		break;
	case SB_THUMBPOSITION:	m_nVscroll = nPos;	break;	 //Scroll to the absolute position.The current position is provided in nPos.
	case SB_THUMBTRACK:		m_nVscroll = nPos;	break;	//Drag scroll box to specified position.The current position is provided in nPos.
	case SB_TOP: m_nVscroll = 0;	break;		//Scroll to top.


	};
	Invalidate();
	CView::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CAPLSNView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Dodaj tutaj swój kod procedury obsługi komunikatów i/lub wywołaj domyślny
	//UINT nKrok = (UINT)(5 * m_fZoomPoziomo);
	UINT nKrok = 5;
	//UINT nStrona = (UINT)(m_nSzerokoscOkna * m_fZoomPoziomo);
	UINT nStrona = m_nSzerokoscOkna;
	switch (nSBCode)
	{
	case SB_LEFT:	m_nBiezacyScrollPoziomo = 0;	break;	// Scroll to far left.
	case SB_ENDSCROLL:	break;	//End scroll.
	case SB_LINELEFT:	if (m_nBiezacyScrollPoziomo > nKrok)	m_nBiezacyScrollPoziomo -= nKrok;	break;// Scroll left.
	case SB_LINERIGHT:	m_nBiezacyScrollPoziomo += nKrok;	break;	//Scroll right.
	case SB_PAGELEFT:	if (m_nBiezacyScrollPoziomo > nStrona)	m_nBiezacyScrollPoziomo -= nStrona;	break;	//Scroll one page left.
	case SB_PAGERIGHT:	m_nBiezacyScrollPoziomo += nStrona;	break;	//Scroll one page right.
	case SB_RIGHT:	//Scroll to far right.
	case SB_THUMBPOSITION:	m_nBiezacyScrollPoziomo = nPos;	break; //Scroll to absolute position.The current position is specified by the nPos parameter.
	case SB_THUMBTRACK:		m_nBiezacyScrollPoziomo = nPos;	break;	//Drag scroll
	}
	
	CView::OnHScroll(nSBCode, m_nBiezacyScrollPoziomo, pScrollBar);
	UstawScrollOdWidoku();
	Invalidate();
}




void CAPLSNView::UstawScrollOdWidoku()
{
	SCROLLINFO scrollInfo;

	if (!GetScrollInfo(SB_HORZ, &scrollInfo))
	{
		TRACE("Can't get scroll info");
		return;
	}
	m_nMaxScrollPoziomo = max((int)((m_nIloscDanychWykresu - m_nSzerokoscOkna) * m_fZoomPoziomo), 0);
	//m_nMaxScrollPoziomo = max((int)(m_nIloscDanychWykresu * m_fZoomPoziomo) - m_nSzerokoscOkna, 0);
	//m_nMaxScrollPoziomo = max((int)(((m_nIloscDanychWykresu * m_fZoomPoziomo) - m_nSzerokoscOkna) * m_fZoomPoziomo), 0);	//źle
	//m_nMaxScrollPoziomo = max((int)((m_nIloscDanychWykresu - m_nSzerokoscOkna)), 0);
	m_nBiezacyScrollPoziomo = min(m_nBiezacyScrollPoziomo, m_nMaxScrollPoziomo);

	scrollInfo.nMin = 0;
	scrollInfo.nMax = (int)(m_nIloscDanychWykresu * m_fZoomPoziomo);
	scrollInfo.nPos = m_nBiezacyScrollPoziomo;
	scrollInfo.nPage = m_nSzerokoscOkna;
	VERIFY(SetScrollInfo(SB_HORZ, &scrollInfo));
	TRACE("BieżScroll=%d, MaxScroll=%d, Okno=%d, zoom%.1f\n", m_nBiezacyScrollPoziomo, m_nMaxScrollPoziomo, m_nSzerokoscOkna, m_fZoomPoziomo);

	
}


void CAPLSNView::OnOleInsertNew()
{
	// TODO: Dodaj tutaj swój kod procedury obsługi polecenia
}


void CAPLSNView::OnDropFiles(HDROP hDropInfo)
{
	// TODO: Dodaj tutaj swój kod procedury obsługi komunikatów i/lub wywołaj domyślny

	CView::OnDropFiles(hDropInfo);
}
