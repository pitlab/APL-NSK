
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
END_MESSAGE_MAP()

// Tworzenie/niszczenie obiektu CAPLSNView

CAPLSNView::CAPLSNView() noexcept
: m_bPolaczono(FALSE)
, m_bRysujPasekPostepu(FALSE)
, m_bRysujTelemetrie(FALSE)
, m_bKoniecWatkuOdswiezaniaTelemtrii(TRUE)
, m_chAdresAutopilota(2)
{
	//CKomunikacja m_cKomunikacja = getKomunikacja();
	m_cKomunikacja.m_chAdresAutopilota = m_chAdresAutopilota;	//przekaż domyślny adres do klasy komunikacyjnej
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
		m_cKomunikacja.Rozlacz();
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

	m_cKomunikacja.UstawRodzica(this);
	m_cKomunikacja.UstawAdresPortuETH(L"127.0.0.1");
	m_cKomunikacja.UstawNumerPortuETH(m_nNumerPortuEth);
	m_cKomunikacja.UstawTypPolaczenia((uint8_t)m_nTypPolaczenia);
	m_cKomunikacja.UstawPredkoscPortuUART(m_nPredkoscPortuCom);
	m_cKomunikacja.UstawNumerPortuUART(m_nNumerPortuCom);

	uint8_t chErr = m_cKomunikacja.Polacz(this);
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

	m_cKomunikacja.UstawTypPolaczenia(UART + m_nTypPolaczenia);
	m_cKomunikacja.UstawNumerPortuUART(m_nNumerPortuCom);
	m_cKomunikacja.UstawPredkoscPortuUART(m_nPredkoscPortuCom);
	if (m_cKomunikacja.CzyPolaczonoUart())
	{
		m_bKoniecWatkuOdswiezaniaTelemtrii = TRUE;
		m_cKomunikacja.Rozlacz();
	}
	else
	{
		m_cKomunikacja.Polacz(this);
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
	//CKomunikacja m_cKomunikacja = getKomunikacja();

	uint32_t rozmiar = sizeof(pDoc->m_sZdjecie);
	//uruchom wątek aktualizujący pasek postępu pobierania zdjęcia
	m_bKoniecWatkuPaskaPostepu = FALSE;
	pWskWatkuPaskaPostepu = AfxBeginThread((AFX_THREADPROC)WatekRysujPasekPostepu, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);

	m_sLiczbaFragmentowPaskaPostepu = rozmiar / ROZM_DANYCH_UART;
	m_sBiezacyStanPaskaPostepu = 0;
	pDoc->m_bZdjecieGotowe = FALSE;
	chErr = m_cKomunikacja.ZrobZdjecie(pDoc->m_sZdjecie);
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
	//CKomunikacja m_cKomunikacja = getKomunikacja();

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
	//CKomunikacja m_cKomunikacja = getKomunikacja();


	while (!m_bKoniecWatkuPaskaPostepu)
	{
		nErr = WaitForSingleObject(m_cKomunikacja.m_hZdarzeniePaczkaDanych, 200);
		if (nErr != WAIT_TIMEOUT)
		{
			m_sBiezacyStanPaskaPostepu++;
			this->Invalidate(TRUE);
			//Invalidate(TRUE);
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
	uint32_t nErr;

	while (!m_bKoniecWatkuOdswiezaniaTelemtrii)
	{
		nErr = WaitForSingleObject(m_cProtokol.m_hZdarzenieRamkaTelemetriiGotowa, 200);
		if (nErr != WAIT_TIMEOUT)
		{
			m_bRysujTelemetrie = TRUE;
			if (this)
				this->Invalidate(TRUE);
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

	pCmdUI->Enable(m_cKomunikacja.CzyPolaczonoUart());
}




void CAPLSNView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Dodaj tutaj swój kod procedury obsługi komunikatów
	m_pLinearGradientBrush->SetEndPoint(CPoint(cx, cy));
}



afx_msg LRESULT CAPLSNView::OnDraw2d(WPARAM wParam, LPARAM lParam)
{
	CHwndRenderTarget* pRenderTarget = (CHwndRenderTarget*)lParam;
	ASSERT_VALID(pRenderTarget);
	CD2DPointF pktfPoczatek, pktfKoniec;
	CRect okno;
	GetClientRect(okno);

	pRenderTarget->FillRectangle(okno, m_pLinearGradientBrush);
	//pRenderTarget->DrawText(_T("Hello, World!"), rect, m_pBrushBlack, m_pTextFormat);

	//rysowanie wykresów telemetrii
	if (m_bRysujTelemetrie)
	{	
		m_bRysujTelemetrie = FALSE;
		float fSkalaY;
		float fZmienna;
		long lLiczbaRamek = (long)getProtokol().m_vRamkaTelemetryczna.size();
		_Telemetria stDaneTele;
		int32_t nIndexRamki;
		long lErr;
		

		//fSkalaX = (float)okno.right / lLiczbaRamek;
		fSkalaY = (float)okno.bottom / 40.0f;

		//wykres X
		std::vector< CPoint > vPunktyWykresuX;
		//vPunktyWykresuX.reserve(okno.right);
		nIndexRamki = lLiczbaRamek - 1;
		pktfPoczatek.x = 0.0f;
		pktfKoniec.x = 1.0f;
		pktfPoczatek.y = (float)okno.bottom / 2;
		do    //sprawdzaj wektor ramki od końca aż napełni się wektor punktów wykresu
		{
			stDaneTele = getProtokol().m_vRamkaTelemetryczna[nIndexRamki--];
			if (!stDaneTele.dane.empty())
			{
				lErr = m_cDekoderTelemetrii.PobierzZmienna(&stDaneTele, 0, &fZmienna);
				if (lErr == ERR_OK)
				{
					pktfKoniec.x += 1.0f;
					pktfKoniec.y = (float)okno.bottom / 2 - (fZmienna * fSkalaY);
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
		pktfPoczatek.x = 0.0f;
		pktfKoniec.x = 1.0f;
		pktfPoczatek.y = (float)okno.bottom / 2;
		do
		{
			stDaneTele = getProtokol().m_vRamkaTelemetryczna[nIndexRamki--];
			if (stDaneTele.dane.size())
			{
				lErr = m_cDekoderTelemetrii.PobierzZmienna(&stDaneTele, 1, &fZmienna);
				if (lErr == ERR_OK)
				{
					pktfKoniec.x += 1.0f;
					pktfKoniec.y = (float)okno.bottom / 2 - (fZmienna * fSkalaY);
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
		pktfPoczatek.x = 0.0f;
		pktfKoniec.x = 1.0f;
		pktfPoczatek.y = (float)okno.bottom / 2;
		do
		{
			stDaneTele = getProtokol().m_vRamkaTelemetryczna[nIndexRamki--];
			if (stDaneTele.dane.size())
			{
				lErr = m_cDekoderTelemetrii.PobierzZmienna(&stDaneTele, 2, &fZmienna);
				if (lErr == ERR_OK)
				{
					pktfKoniec.x += 1.0f;
					pktfKoniec.y = (float)okno.bottom / 2 - (fZmienna * fSkalaY);
					pRenderTarget->DrawLine(pktfPoczatek, pktfKoniec, m_pBrushWykresuB);
					pktfPoczatek = pktfKoniec;
				}
					//vPunktyWykresuZ.insert(vPunktyWykresuZ.begin(), CPoint(x++, okno.bottom / 2 - (uint32_t)(fZmienna * fSkalaY)));
			}
		} while ((pktfKoniec.x < okno.right) && (nIndexRamki > 0));

		

		/*if (vPunktyWykresuX.size())
		{
			pRenderTarget->DrawLine(pktfPoczatek, pktfKoniec, m_pBrushWykresuR);
			pDC->SelectObject(&penWykresuR);
			pDC->Polyline(&vPunktyWykresuX[0], (int)vPunktyWykresuX.size());
		}
		if (vPunktyWykresuY.size())
		{
			pDC->SelectObject(&penWykresuG);
			pDC->Polyline(&vPunktyWykresuY[0], (int)vPunktyWykresuY.size());
		}

		if (vPunktyWykresuZ.size())
		{
			pDC->SelectObject(&penWykresuB);
			pDC->Polyline(&vPunktyWykresuZ[0], (int)vPunktyWykresuZ.size());
		}*/
	}
	return TRUE;
}
