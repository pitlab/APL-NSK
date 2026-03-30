
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
#include "komunikacja/KomunikatySieci.h"
#include "Errors.h"
#include <stdio.h>
#include <string.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


int	CAPLSNView::m_LicznikInstancji = 0;
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

	//obsługa poleceń toolbara
	ON_COMMAND(ID_KONFIG_PORT, &CAPLSNView::OnKonfigPort)
	ON_COMMAND(ID_POLACZ_COM, &CAPLSNView::OnPolaczCom)
	ON_COMMAND(ID_POLACZ_ETH, &CAPLSNView::OnPolaczEth)
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
	ON_COMMAND(ID_KONFIG_WYKRESOW, &CAPLSNView::OnKonfigWykresow)
	ON_UPDATE_COMMAND_UI(ID_KONFIG_WYKRESOW, &CAPLSNView::OnUpdateKonfigWykresow)
	ON_COMMAND(ID_KONFIG_TELEMETRII, &CAPLSNView::OnKonfigTelemetrii)
	ON_UPDATE_COMMAND_UI(ID_KONFIG_TELEMETRII, &CAPLSNView::OnUpdateKonfigTelemetrii)
	ON_COMMAND(ID_KONFIG_REJESTRATORA, &CAPLSNView::OnKonfigRejestratora)
	ON_UPDATE_COMMAND_UI(ID_KONFIG_REJESTRATORA, &CAPLSNView::OnUpdateKonfigRejestratora)

	ON_COMMAND(ID_BUT_POBIERZ_FFT, &CAPLSNView::OnButPobierzFft)
	ON_UPDATE_COMMAND_UI(ID_BUT_POBIERZ_FFT, &CAPLSNView::OnUpdateButPobierzFft)
END_MESSAGE_MAP()

// Tworzenie/niszczenie obiektu CAPLSNView

CAPLSNView::CAPLSNView() noexcept
: m_bPolaczono(FALSE)
, m_chAdresAutopilota(2)
, m_nTypPolaczenia(UART)
, m_nPredkoscPortuCom(0)
, m_nNumerPortuCom(0)
, m_nNumerPortuEth(0)
, m_bRysujPasekPostepu(FALSE)
, m_bRysujTelemetrie(FALSE)
, m_bKoniecWatkuOdswiezaniaTelemtrii(TRUE)
, m_bKoniecWatkuPaskaPostepu(FALSE)
, m_fZoomPionowo(1.0f)
, m_fZoomPoziomo(1.0f)
, m_nVscroll(0)
, m_bOknoGotowe(FALSE)
, m_bWykresZawieraZero(TRUE)
, m_nIloscDanychWykresu(0)
, m_nMaxScrollPoziomo(0)
, m_nBiezacyScrollPoziomo(0)
, m_sLiczbaFragmentowPaskaPostepu(0)
, m_sBiezacyStanPaskaPostepu(0)
, m_nSzerokoscOkna(0)
, m_nSzerokoscWykresu(0)
, pWskWatkuPaskaPostepu(0)
, pWskWatkuOdswiezaniaTelemetrii(0)
, m_nSzerokoscKamery(0)
, m_nWysokoscKamery(0)
{
	getKomunikacja().m_chAdresAutopilota = m_chAdresAutopilota;	//przekaż domyślny adres do klasy komunikacyjnej
	// Enable D2D support for this window:
	EnableD2DSupport();
	m_LicznikInstancji++;
	// Initialize D2D resources:
	m_pBrushWykresu = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::Blue));
	m_pBrushOsiWykresu = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::Gray));

	//m_pBrushLegendy = new CD2DBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::Gray));
	
	m_pTextFormat = new CD2DTextFormat(GetRenderTarget(), _T("Verdana"), 11);

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
		getKomunikacja().Rozlacz();
	}
}



BOOL CAPLSNView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: zmodyfikuj klasę Window lub style w tym miejscu, modyfikując
	//  styl kaskadowy CREATESTRUCT
	m_cObslugaRejestru.CzytajRejestrIP(L"AdresIP", m_chAdresIP);
	m_cObslugaRejestru.CzytajRejestrInt(L"PredkoscCOM", &m_nPredkoscPortuCom);
	m_cObslugaRejestru.CzytajRejestrInt(L"NrPortuCOM", &m_nNumerPortuCom);
	m_cObslugaRejestru.CzytajRejestrInt(L"PortETH", &m_nNumerPortuEth);
	m_cObslugaRejestru.CzytajRejestrInt(L"TypPortu", &m_nTypPolaczenia);

	m_strAdresIP.Format(L"%d.%d.%d.%d", m_chAdresIP[0], m_chAdresIP[1], m_chAdresIP[2], m_chAdresIP[3]);
	getKomunikacja().UstawRodzica(this);
	getKomunikacja().UstawAdresIP(m_strAdresIP);
	getKomunikacja().UstawNumerPortuETH(m_nNumerPortuEth);
	getKomunikacja().UstawTypPolaczenia((uint8_t)m_nTypPolaczenia);
	getKomunikacja().UstawPredkoscPortuUART(m_nPredkoscPortuCom);
	getKomunikacja().UstawNumerPortuUART(m_nNumerPortuCom);

	/*uint8_t chErr = getKomunikacja().Polacz(this);
	if (chErr == ERR_OK)
	{
		m_bPolaczono = TRUE;		
		if (m_bKoniecWatkuOdswiezaniaTelemtrii)
		{
			m_bKoniecWatkuOdswiezaniaTelemtrii = FALSE;
			pWskWatkuOdswiezaniaTelemetrii = AfxBeginThread((AFX_THREADPROC)WatekInvalidujWytkresTelemetrii, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
		}
	}
	else*/
		m_bPolaczono = FALSE;
	
	return CView::PreCreateWindow(cs);
	
	return TRUE;
}

// Rysowanie obiektu CAPLSNView - docelowo do usunięcia
void CAPLSNView::OnDraw(CDC* pDC)
{
	CAPLSNDoc* pDoc = GetDocument();
	RECT prost;
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: tutaj dodaj kod rysowania danych natywnych


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



///////////////////////////////////////////////////////////////////////////////////////////////////
// Główna funkcja rysujaca dokument wykorzystujaca Direct 2D
// Parametry: 
//  wParam - ?
//  lParam - wskaźnik na CHwndRenderTarget
// zwraca: komunkat systemowy
///////////////////////////////////////////////////////////////////////////////////////////////////
afx_msg LRESULT CAPLSNView::OnDraw2d(WPARAM wParam, LPARAM lParam)
{
	CHwndRenderTarget* pRenderTarget = (CHwndRenderTarget*)lParam;
	ASSERT_VALID(pRenderTarget);
	CAPLSNDoc* pDoc = GetDocument();
	stKonfigWykresu_t stKonfigLewy, stKonfigPrawy;
	int nLiczbaWykresow;
	int nIdZmiennej;
	float fRozpietoscWykresu;
	CRect oknoGrupy;
	GetClientRect(oknoGrupy);
	stKonfigLewy.rOknoWykresu = stKonfigPrawy.rOknoWykresu = oknoGrupy;
	m_bOknoGotowe = TRUE;
	
	pRenderTarget->FillRectangle(oknoGrupy, m_pLinearGradientBrush);
	stKonfigLewy.fSkalaX = stKonfigPrawy.fSkalaX = m_fZoomPoziomo;
	stKonfigLewy.fSkalaY = stKonfigPrawy.fSkalaY = (float)stKonfigLewy.rOknoWykresu.bottom / 40.0f * m_fZoomPionowo;
	stKonfigLewy.fHscroll = stKonfigPrawy.fHscroll = (float)m_nBiezacyScrollPoziomo;

	//wyświetl obraz z kamery
	uint8_t chKolor[3];
	uint16_t sPix;

	const CD2DRectU rectUObraz(0, 0, 480, 320);
	CD2DRectF rectObraz;
	ID2D1Bitmap* m_pCameraBitmap = nullptr;

	if (pDoc->m_bZdjecieGotowe)
	{
		m_nSzerokoscKamery = 480;
		m_nWysokoscKamery = 320;
		size_t Indeks;
		CComPtr<ID2D1Bitmap> m_spCameraBitmap;
		size_t bufferSize = static_cast<size_t>(m_nSzerokoscKamery) * static_cast<size_t>(m_nWysokoscKamery) * 4;
		std::vector<uint8_t> bgraBuffer(bufferSize);

		for (int y = 0; y < m_nWysokoscKamery; y++)
		{
			for (int x = 0; x < m_nSzerokoscKamery; x++)
			{
				sPix = pDoc->m_sZdjecieRGB565[y * 480 + x];
				chKolor[0] = ((sPix & 0xF800) >> 11) * 8;		//R
				chKolor[1] = ((sPix & 0x070E) >> 5) * 4;		//G
				chKolor[2] = (sPix & 0x001F) * 8;				//B


				Indeks = (y * 480 + x) *4;
				bgraBuffer[Indeks + 0] = chKolor[2];	//B
				bgraBuffer[Indeks + 1] = chKolor[1];	//G
				bgraBuffer[Indeks + 2] = chKolor[0];	//R
				bgraBuffer[Indeks + 3] = 0xFF;			//Alfa
			}
		}

		D2D1_SIZE_U size = D2D1::SizeU(m_nSzerokoscKamery, m_nWysokoscKamery);
		D2D1_BITMAP_PROPERTIES props = D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE));

		m_spCameraBitmap.Release();

		ID2D1RenderTarget* pRT = GetRenderTarget()->GetRenderTarget();
		HRESULT hr = pRT->CreateBitmap(size, bgraBuffer.data(), m_nSzerokoscKamery * 4, &props, &m_spCameraBitmap);

		if (m_spCameraBitmap)
		{
			pRT->DrawBitmap(m_spCameraBitmap, D2D1::RectF(0.f, 0.f, (FLOAT)m_nSzerokoscKamery, (FLOAT)m_nWysokoscKamery));
		}
	}

	//wodospady 6 FFT
	if (pDoc->m_bFFTGotowe)
	{
		m_nSzerokoscKamery = 512;
		m_nWysokoscKamery = LICZBA_TESTOW_FFT;
		size_t Indeks;
		CComPtr<ID2D1Bitmap> m_spCameraBitmap;
		size_t bufferSize = static_cast<size_t>(m_nSzerokoscKamery) * static_cast<size_t>(m_nWysokoscKamery * LICZBA_WYKRESOW_FFT) * 4;
		std::vector<uint8_t> bgraBuffer(bufferSize);
		
		//for (int t = 0; t < LICZBA_ZMIENNYCH_FFT; t++)
		for (int t = 0; t < LICZBA_WYKRESOW_FFT; t++)
		{			
			for (int y = 0; y < m_nWysokoscKamery; y++)
			{
				for (int x = 0; x < m_nSzerokoscKamery; x++)
				{
					Indeks = (t * m_nWysokoscKamery * m_nSzerokoscKamery + y * m_nSzerokoscKamery + x) * 4;

					for (int b=0; b<3; b++)
						bgraBuffer[Indeks + b] = 0;		//wyczyść składowe RGB
					
					switch (t)
					{
					case 0: bgraBuffer[Indeks + 0] = (uint8_t)(pDoc->m_fWynikFFT[y][t][x] * WODOSPAD_SKALA_KOLORU);		break;	//B
					case 1: bgraBuffer[Indeks + 1] = (uint8_t)(pDoc->m_fWynikFFT[y][t][x] * WODOSPAD_SKALA_KOLORU);		break;	//G
					case 2: bgraBuffer[Indeks + 2] = (uint8_t)(pDoc->m_fWynikFFT[y][t][x] * WODOSPAD_SKALA_KOLORU);		break;	//R						
					}
					bgraBuffer[Indeks + 3] = 0xFF;			//Alfa
				}
			}
		}

		D2D1_SIZE_U size = D2D1::SizeU(m_nSzerokoscKamery, m_nWysokoscKamery * LICZBA_WYKRESOW_FFT);
		D2D1_BITMAP_PROPERTIES props = D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE));

		m_spCameraBitmap.Release();

		ID2D1RenderTarget* pRT = GetRenderTarget()->GetRenderTarget();
		HRESULT hr = pRT->CreateBitmap(size, bgraBuffer.data(), m_nSzerokoscKamery * 4, &props, &m_spCameraBitmap);

		if (m_spCameraBitmap)
		{
			pRT->DrawBitmap(m_spCameraBitmap, D2D1::RectF(0.f, 0.f, (FLOAT)m_nSzerokoscKamery, (FLOAT)m_nWysokoscKamery * LICZBA_WYKRESOW_FFT));
		}
	}


	//rysowanie wykresów telemetrii
	if (m_bRysujTelemetrie || m_cKonfiguracjaWykresow.m_bZawieraLog)	//czy są dane telemetryczne lub wczytany z pliku log
	{
		m_bRysujTelemetrie = FALSE;
		stKonfigLewy.bStronaPrawa = stKonfigPrawy.bStronaPrawa = FALSE;
		stKonfigLewy.rOknoWykresu.left = stKonfigPrawy.rOknoWykresu.left = MIEJSCE_NA_LEGENDE;
		stKonfigLewy.rOknoWykresu.right = stKonfigPrawy.rOknoWykresu.right -= MIEJSCE_NA_LEGENDE;	//miejsce na legendę
		//stKonfigLewy.rOknoWykresu.right -= 1;	//brak legendy z prawej strony

		//rysuj okna grup wykresów
		int nLiczbaGrupWykresow = (int)m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow.size();
		stKonfigLewy.rOknoWykresu.top = stKonfigPrawy.rOknoWykresu.top = MIEJSCE_MIEDZY_WYKRESAMI;
		for (int g = 0; g < nLiczbaGrupWykresow; g++)
		{
			stKonfigLewy.fMinWykresu = stKonfigPrawy.fMinWykresu = WARTOSC_MAX;
			stKonfigLewy.fMaxWykresu = stKonfigPrawy.fMaxWykresu = WARTOSC_MIN;
			stKonfigLewy.fPoziomZera = stKonfigPrawy.fPoziomZera = 0.0f;

			//wspólrzędne x początku legendy, będą zwiększane w każdej iteracji aby opisy nie nachodziły na siebie
			stKonfigLewy.fStartLegendy = (float)(stKonfigLewy.rOknoWykresu.left + 5);		//zaczyna się od lewej i rođnie
			stKonfigPrawy.fStartLegendy = (float)(stKonfigLewy.rOknoWykresu.right - 5);		//zaczyna się od prawej i maleje
			stKonfigLewy.rOknoWykresu.bottom = stKonfigPrawy.rOknoWykresu.bottom = (g + 1) * oknoGrupy.bottom / nLiczbaGrupWykresow - MIEJSCE_MIEDZY_WYKRESAMI / 2;

			//wykresy wspólnej skali używają konfiguracji dla wykresów z lewej strony
			if (m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow[g].chTypWykresu == WYKRES_WSPOLNA_SKALA)
			{
				//znajdź globalne ekstrema w grupie wykresów o wspólnej skali
				nLiczbaWykresow = (int)m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne.size();
				for (int w = 0; w < nLiczbaWykresow; w++)
				{
					nIdZmiennej = m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne[w].sIdZmiennej;
					if (m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne[w].chZrodloZmiennej == ZRODLO_TELEMETRIA)	//0 == telemetria
					{
						if (stKonfigLewy.fMinWykresu > getProtokol().m_stEkstremaTelemetrii[nIdZmiennej].fMin)
							stKonfigLewy.fMinWykresu = getProtokol().m_stEkstremaTelemetrii[nIdZmiennej].fMin;
						if (stKonfigLewy.fMaxWykresu < getProtokol().m_stEkstremaTelemetrii[nIdZmiennej].fMax)
							stKonfigLewy.fMaxWykresu = getProtokol().m_stEkstremaTelemetrii[nIdZmiennej].fMax;
					}
					else               //log
					{
						if (stKonfigLewy.fMinWykresu > pDoc->m_vLog[nIdZmiennej].fMin)
							stKonfigLewy.fMinWykresu = pDoc->m_vLog[nIdZmiennej].fMin;
						if (stKonfigLewy.fMaxWykresu < pDoc->m_vLog[nIdZmiennej].fMax)
							stKonfigLewy.fMaxWykresu = pDoc->m_vLog[nIdZmiennej].fMax;
					}
				}

				stKonfigPrawy.fMinWykresu = stKonfigLewy.fMinWykresu;
				stKonfigPrawy.fMaxWykresu = stKonfigLewy.fMaxWykresu;

				if ((stKonfigLewy.fMinWykresu == WARTOSC_MAX) && (stKonfigLewy.fMinWykresu == WARTOSC_MIN))	//jeżeli wartość nie jest zmieniona od czasu inicjalizacji
					return FALSE;
				
				fRozpietoscWykresu = fabsf(stKonfigLewy.fMaxWykresu - stKonfigLewy.fMinWykresu);
				if (fRozpietoscWykresu < MIN_POZPIETOSC_WYKRESU)
					fRozpietoscWykresu = MIN_POZPIETOSC_WYKRESU;	//zapobiega dzieleniu przez 0

				stKonfigLewy.fSkalaY = (stKonfigLewy.rOknoWykresu.bottom - stKonfigLewy.rOknoWykresu.top) / fRozpietoscWykresu;
				stKonfigPrawy.fSkalaY = stKonfigLewy.fSkalaY;
				
				//czy zero jest na wykresie?
				if ((stKonfigLewy.fMinWykresu < 0.0f) && (stKonfigLewy.fMaxWykresu > 0.0f))
				{
					stKonfigLewy.fPoziomZera = (float)stKonfigLewy.rOknoWykresu.bottom - stKonfigLewy.fSkalaY * (float)fabsf(stKonfigLewy.fMinWykresu);
					stKonfigLewy.bWykresPrzechodziPrzezZero = TRUE;
				}
				else
				{
					stKonfigLewy.fPoziomZera = (float)stKonfigLewy.rOknoWykresu.bottom;
					stKonfigLewy.bWykresPrzechodziPrzezZero = FALSE;
				}
				stKonfigPrawy.fPoziomZera = stKonfigLewy.fPoziomZera;
				stKonfigPrawy.bWykresPrzechodziPrzezZero = stKonfigLewy.bWykresPrzechodziPrzezZero;

				for (int w = 0; w < nLiczbaWykresow; w++)
				{
					nIdZmiennej = m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne[w].sIdZmiennej;					
					m_pBrushWykresu->SetColor(m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne[w].cKolorD2D1);
					if (m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne[w].chZrodloZmiennej == ZRODLO_TELEMETRIA)	//0 == telemetria
					{						
						RysujWykresTelemetrii(&stKonfigLewy, &getProtokol().m_vDaneTelemetryczne, nIdZmiennej, pRenderTarget, m_pBrushWykresu);
					}
					else
					{
						int nID = m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne[w].sIdZmiennej;
						RysujWykresLogu(stKonfigLewy.rOknoWykresu, (float)m_nBiezacyScrollPoziomo, stKonfigLewy.fPoziomZera, stKonfigLewy.fSkalaX, stKonfigLewy.fSkalaY, nID, pRenderTarget, m_pBrushWykresu);
					}
				}
			}	

			//Na wykresie są dwie osobne grupy wykresów i dwie konfiguracje dla wykresów ze skalą po lewej i prawej stronie
			if (m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow[g].chTypWykresu == WYKRES_OSOBNA_SKALA)
			{
				nLiczbaWykresow = (int)m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne.size();
				for (int w = 0; w < nLiczbaWykresow; w++)
				{
					nIdZmiennej = m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne[w].sIdZmiennej;					
					if (w == 0)	//skala lewa dla wykresu 1
					{
						if (m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne[w].chZrodloZmiennej == ZRODLO_TELEMETRIA)	//0 == telemetria
						{
							if (stKonfigLewy.fMinWykresu > getProtokol().m_stEkstremaTelemetrii[nIdZmiennej].fMin)
								stKonfigLewy.fMinWykresu = getProtokol().m_stEkstremaTelemetrii[nIdZmiennej].fMin;
							if (stKonfigLewy.fMaxWykresu < getProtokol().m_stEkstremaTelemetrii[nIdZmiennej].fMax)
								stKonfigLewy.fMaxWykresu = getProtokol().m_stEkstremaTelemetrii[nIdZmiennej].fMax;
						}
						else            //log
						{
							if (stKonfigLewy.fMinWykresu > pDoc->m_vLog[nIdZmiennej].fMin)
								stKonfigLewy.fMinWykresu = pDoc->m_vLog[nIdZmiennej].fMin;
							if (stKonfigLewy.fMaxWykresu < pDoc->m_vLog[nIdZmiennej].fMax)
								stKonfigLewy.fMaxWykresu = pDoc->m_vLog[nIdZmiennej].fMax;
						}

						fRozpietoscWykresu = fabsf(stKonfigLewy.fMaxWykresu - stKonfigLewy.fMinWykresu);
						if (fRozpietoscWykresu < MIN_POZPIETOSC_WYKRESU)
							fRozpietoscWykresu = MIN_POZPIETOSC_WYKRESU;	//zapobiega dzieleniu przez 0
						stKonfigLewy.fSkalaY = (stKonfigLewy.rOknoWykresu.bottom - stKonfigLewy.rOknoWykresu.top) / fRozpietoscWykresu;

						//czy zero jest na wykresie?
						if ((stKonfigLewy.fMinWykresu < 0.0f) && (stKonfigLewy.fMaxWykresu > 0.0f))
						{
							stKonfigLewy.fPoziomZera = (float)stKonfigLewy.rOknoWykresu.bottom - stKonfigLewy.fSkalaY * (float)fabsf(stKonfigLewy.fMinWykresu);
							stKonfigLewy.bWykresPrzechodziPrzezZero = TRUE;
						}
						else
						{
							stKonfigLewy.fPoziomZera = (float)stKonfigLewy.rOknoWykresu.bottom;
							stKonfigLewy.bWykresPrzechodziPrzezZero = FALSE;
						}		

						m_pBrushWykresu->SetColor(m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne[w].cKolorD2D1);
						if (m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne[w].chZrodloZmiennej == ZRODLO_TELEMETRIA)	//0 == telemetria
						{
							RysujWykresTelemetrii(&stKonfigLewy, &getProtokol().m_vDaneTelemetryczne, nIdZmiennej, pRenderTarget, m_pBrushWykresu);
						}
						else
						{
							int nID = m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne[w].sIdZmiennej;
							RysujWykresLogu(stKonfigLewy.rOknoWykresu, (float)m_nBiezacyScrollPoziomo, stKonfigLewy.fPoziomZera, stKonfigLewy.fSkalaX, stKonfigLewy.fSkalaY, nID, pRenderTarget, m_pBrushWykresu);
						}
					}
					if (w == 1)	//skala prawa dla wykresu 2 po prawej stronie
					{
						stKonfigPrawy.bStronaPrawa = TRUE;
						if (m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne[w].chZrodloZmiennej == ZRODLO_TELEMETRIA)	//0 == telemetria
						{
							if (stKonfigPrawy.fMinWykresu > getProtokol().m_stEkstremaTelemetrii[nIdZmiennej].fMin)
								stKonfigPrawy.fMinWykresu = getProtokol().m_stEkstremaTelemetrii[nIdZmiennej].fMin;
							if (stKonfigPrawy.fMaxWykresu < getProtokol().m_stEkstremaTelemetrii[nIdZmiennej].fMax)
								stKonfigPrawy.fMaxWykresu = getProtokol().m_stEkstremaTelemetrii[nIdZmiennej].fMax;
						}
						else            //log
						{
							if (stKonfigPrawy.fMinWykresu > pDoc->m_vLog[nIdZmiennej].fMin)
								stKonfigPrawy.fMinWykresu = pDoc->m_vLog[nIdZmiennej].fMin;
							if (stKonfigPrawy.fMaxWykresu < pDoc->m_vLog[nIdZmiennej].fMax)
								stKonfigPrawy.fMaxWykresu = pDoc->m_vLog[nIdZmiennej].fMax;
						}

						fRozpietoscWykresu = (fabsf(stKonfigPrawy.fMinWykresu) + fabsf(stKonfigPrawy.fMaxWykresu));
						if (fRozpietoscWykresu < MIN_POZPIETOSC_WYKRESU)
							fRozpietoscWykresu = MIN_POZPIETOSC_WYKRESU;	//zapobiega dzieleniu przez 0
						stKonfigPrawy.fSkalaY = (stKonfigPrawy.rOknoWykresu.bottom - stKonfigPrawy.rOknoWykresu.top) / fRozpietoscWykresu;

						//czy zero jest na wykresie?
						if ((stKonfigPrawy.fMinWykresu < 0.0f) && (stKonfigPrawy.fMaxWykresu > 0.0f))
						{
							stKonfigPrawy.fPoziomZera = (float)stKonfigPrawy.rOknoWykresu.bottom - stKonfigPrawy.fSkalaY * (float)fabsf(stKonfigPrawy.fMinWykresu);
							stKonfigPrawy.bWykresPrzechodziPrzezZero = TRUE;
						}
						else
						{
							stKonfigPrawy.fPoziomZera = (float)stKonfigPrawy.rOknoWykresu.bottom;
							stKonfigPrawy.bWykresPrzechodziPrzezZero = FALSE;
						}

						m_pBrushWykresu->SetColor(m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne[w].cKolorD2D1);
						if (m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne[w].chZrodloZmiennej == ZRODLO_TELEMETRIA)	//0 == telemetria
						{
							RysujWykresTelemetrii(&stKonfigPrawy, &getProtokol().m_vDaneTelemetryczne, nIdZmiennej, pRenderTarget, m_pBrushWykresu);
						}
						else
						{
							int nID = m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne[w].sIdZmiennej;
							RysujWykresLogu(stKonfigPrawy.rOknoWykresu, (float)m_nBiezacyScrollPoziomo, stKonfigPrawy.fPoziomZera, stKonfigPrawy.fSkalaX, stKonfigPrawy.fSkalaY, nID, pRenderTarget, m_pBrushWykresu);
						}
					}
				}
			}
			RysujOknoGrupyWykresow(&stKonfigLewy, &stKonfigPrawy, pRenderTarget, m_pBrushOsiWykresu);
			stKonfigLewy.rOknoWykresu.top = stKonfigLewy.rOknoWykresu.bottom + MIEJSCE_MIEDZY_WYKRESAMI;
			stKonfigPrawy.rOknoWykresu.top = stKonfigPrawy.rOknoWykresu.bottom + MIEJSCE_MIEDZY_WYKRESAMI;
		}		
	}
	return TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Rysuje wykres logu pobrany z dokumentu
// Parametry:
//  okno - obszar rysowania
//  fHscroll - przesunięcie danych w poziomie podpięte do poziomego paska przewijania
//  fVpos - przesuniecie środka wykresu w pionie po to aby upchnąc wiele wykresów w oknie
//  fSkalaX, fSkalaY - wspólczynniki skalowania danych w poziomie i pionie sterowane kólkiem myszy (X) i kólkiem z Shift (Y)
//  nIndeksZmiennej - indeks zmiennej w logu
//  pRenderTarget - wskaźnik na narzędzie renderujące cały wykres
//  pBrush - wskaźnik na narzędzie rysujace określonym kolorem
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::RysujWykresLogu(CRect okno, float fHscroll, float fVzera, float fSkalaX, float fSkalaY, int nIndeksZmiennej, CHwndRenderTarget* pRenderTarget, CD2DSolidColorBrush* pBrush)
{
	CD2DPointF pktfPoczatek, pktfKoniec;
	CAPLSNDoc* pDoc = GetDocument();
	int nIndeks;
	if (pDoc->m_vLog.size() && pDoc->m_bOdczytanoLog)
	{
		m_nIloscDanychWykresu = (int)pDoc->m_vLog[nIndeksZmiennej].vfWartosci.size();
		//float fSkalaX = (float)okno.right / m_nIloscDanychWykresu * m_fZoomPoziomo;
		//float fSkalaX = m_fZoomPoziomo;
		//float fSkalaY = (float)okno.bottom / 40.0f * m_fZoomPionowo;
		//pktfPoczatek.x = (float)okno.left + fHscroll;
		pktfPoczatek.x = (float)okno.left;
		pktfPoczatek.y = fVzera - (pDoc->m_vLog[nIndeksZmiennej].vfWartosci[0] * fSkalaY);
		for (int n = 1; n < m_nIloscDanychWykresu; n++)
		{
			//pktfKoniec.x = (float)(okno.left + (n - m_nBiezacyScrollPoziomo) * fSkalaX);
			pktfKoniec.x = (float)(okno.left + n * fSkalaX);
			//pktfKoniec.y = fVzera - (pDoc->m_vLog[nIndeksZmiennej].vfWartosci[n] * fSkalaY);
			nIndeks = n + (int)(m_nBiezacyScrollPoziomo * fSkalaX);
			//nIndeks = n + m_nBiezacyScrollPoziomo;
			if (nIndeks < 0)
				nIndeks = 0;
			if (nIndeks >= m_nIloscDanychWykresu)
				nIndeks = m_nIloscDanychWykresu-1;
			pktfKoniec.y = fVzera - (pDoc->m_vLog[nIndeksZmiennej].vfWartosci[nIndeks] * fSkalaY);
			pRenderTarget->DrawLine(pktfPoczatek, pktfKoniec, m_pBrushWykresu);
			pktfPoczatek = pktfKoniec;
		}
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Rysuje wykres pobrany z telemetrii
// Parametry:
// *stKonfig - wskaźnik na strukturę zmiennych konfiguracji wykresu
//  vRamkaTele - wektor zmiennych telemetrycznych z którego trzeba wyłuskać potrzebna dane
//  nIndeksZmiennej - indeks zmiennej do wyświetlenia
//  pRenderTarget - wskaźnik na narzędzie renderujące cały wykres
//  pBrush - wskaźnik na narzędzie rysujace określonym kolorem
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::RysujWykresTelemetrii(stKonfigWykresu_t* stKonfig, std::vector<stTelemetria_t>* vDaneTele, int nIndeksZmiennej, CHwndRenderTarget* pRenderTarget, CD2DSolidColorBrush* pBrush)
{
	float fZmienna;
	long lLiczbaRamek = (long)vDaneTele->size();
	if (!lLiczbaRamek)
		return;
	stTelemetria_t stDaneTele;
	int32_t nIndexRamki = lLiczbaRamek - 1;
	CD2DPointF pktfPoczatek, pktfKoniec;
	CD2DRectF rectLegenda;

	//rysuj legendę
	CString strNazwa = getKomunikacja().m_strNazwyZmiennychTele[nIndeksZmiennej];
	rectLegenda.top = (float)(stKonfig->rOknoWykresu.top + 5);
	rectLegenda.bottom = (float)(rectLegenda.top + 20);
	if (stKonfig->bStronaPrawa)
	{
		rectLegenda.right = stKonfig->fStartLegendy;
		rectLegenda.left = (float)(rectLegenda.right - 120);
		stKonfig->fStartLegendy = rectLegenda.left;
	}
	else
	{
		rectLegenda.left = stKonfig->fStartLegendy;
		rectLegenda.right = (float)(rectLegenda.left + 120);
		stKonfig->fStartLegendy = rectLegenda.right;
	}
	pRenderTarget->DrawText(strNazwa, rectLegenda, pBrush, m_pTextFormat);

	//rysuj wykres
	pktfPoczatek.x = (float)stKonfig->rOknoWykresu.left + stKonfig->fHscroll;
	pktfKoniec.x = 1.0f * stKonfig->fSkalaX + stKonfig->rOknoWykresu.left + stKonfig->fHscroll;

	//znajdź ostatnią istniejacą zmienną. Może jej nie być w ostatniej ramce, ze względu na różne okresy telemerii
	do
	{
		fZmienna = (*vDaneTele)[nIndexRamki--].dane[nIndeksZmiennej];
	} while (!fZmienna && nIndexRamki);
	
	//jeżeli cofając się do początku nie znajdzie żadnej zmiennej to zakończ rysowanie
	if (!fZmienna && nIndexRamki)
		return;
	
	//Znajdź najwiekszy rozmiar wykresu potrzebny do ustawienia poziomego scrollbara
	if (vDaneTele->size() > m_nIloscDanychWykresu)
		m_nIloscDanychWykresu = (int)vDaneTele->size();

	if (stKonfig->bWykresPrzechodziPrzezZero)
		pktfPoczatek.y = stKonfig->fPoziomZera - (fZmienna * stKonfig->fSkalaY);
	else
		pktfPoczatek.y = stKonfig->fPoziomZera - ((fZmienna - stKonfig->fMinWykresu) * stKonfig->fSkalaY);

	do    //sprawdzaj wektor ramki od końca aż napełni się wykres
	{
		fZmienna = (*vDaneTele)[nIndexRamki--].dane[nIndeksZmiennej];
		if (fZmienna)
		{
			pktfKoniec.x += stKonfig->fSkalaX;
			if (stKonfig->bWykresPrzechodziPrzezZero)
				pktfKoniec.y = stKonfig->fPoziomZera - (fZmienna * stKonfig->fSkalaY);
			else
				pktfKoniec.y = stKonfig->fPoziomZera - ((fZmienna - stKonfig->fMinWykresu) * stKonfig->fSkalaY);

			pRenderTarget->DrawLine(pktfPoczatek, pktfKoniec, pBrush);
			pktfPoczatek = pktfKoniec;
		}
	} while ((pktfKoniec.x < stKonfig->rOknoWykresu.right) && (nIndexRamki >= 0));	//pobierz danych na szerokość okna lub tyle ile się da
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Rysuje oś układu wspórzędnych dla grupy wykresów
// Parametry: 
//  *stKonfig - wskaźnik na strukturę ze zmiennymi konfiguracji wykresu
//  pRenderTarget - narzędzie rysujące
//  pBrush - parametry pędzla rysujacego osie
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::RysujOknoGrupyWykresow(stKonfigWykresu_t* stKonfigLewy, stKonfigWykresu_t* stKonfigPrawy, CHwndRenderTarget* pRenderTarget, CD2DSolidColorBrush* pBrush)
{
	CD2DPointF pktfPoczatek, pktfKoniec;
	CD2DRectF rectWartosciOsi;
	CString strNazwa;
	float fSkokPodzialki;
	float fWartosc;
	int nPodzPonizejZera;
	int nPodzPowyzejZera;

	//ramka okna wykresu
	pktfPoczatek.x = pktfKoniec.x = (float)(stKonfigLewy->rOknoWykresu.left);
	pktfPoczatek.y = (float)stKonfigLewy->rOknoWykresu.top;
	pktfKoniec.y = (float)stKonfigLewy->rOknoWykresu.bottom;
	pRenderTarget->DrawLine(pktfPoczatek, pktfKoniec, pBrush, 0.4f);
	pktfPoczatek = pktfKoniec;
	pktfKoniec.x = (float)(stKonfigLewy->rOknoWykresu.right);
	pRenderTarget->DrawLine(pktfPoczatek, pktfKoniec, pBrush, 0.4f);
	pktfPoczatek = pktfKoniec;
	pktfKoniec.y = (float)stKonfigLewy->rOknoWykresu.top;
	pRenderTarget->DrawLine(pktfPoczatek, pktfKoniec, pBrush, 0.4f);
	pktfPoczatek = pktfKoniec;
	pktfKoniec.x = (float)(stKonfigLewy->rOknoWykresu.left);
	pRenderTarget->DrawLine(pktfPoczatek, pktfKoniec, pBrush, 0.4f);

	//lewa strona osi Y: Min1 i Max1, rysuj podziałki dla zera i powyżej
	fSkokPodzialki = ZnajdzPodzialke(stKonfigLewy->rOknoWykresu, stKonfigLewy->fMinWykresu, stKonfigLewy->fMaxWykresu);
	if (stKonfigLewy->fMaxWykresu > 0)
	{
		if (stKonfigLewy->bWykresPrzechodziPrzezZero)
			nPodzPowyzejZera = (int)round(fabs(stKonfigLewy->fMaxWykresu) / fSkokPodzialki);
		else
			nPodzPowyzejZera = (int)round(fabs(stKonfigLewy->fMaxWykresu - stKonfigLewy->fMinWykresu) / fSkokPodzialki);
	}
	else
		nPodzPowyzejZera = 0;
	for (int n = 0; n <= nPodzPowyzejZera; n++)
	{
		pktfPoczatek.x = (float)(stKonfigLewy->rOknoWykresu.left);;
		pktfKoniec.x = (float)(stKonfigLewy->rOknoWykresu.right);
		pktfPoczatek.y = pktfKoniec.y = stKonfigLewy->fPoziomZera - n * fSkokPodzialki * stKonfigLewy->fSkalaY;
		if (n==0)
			pRenderTarget->DrawLine(pktfPoczatek, pktfKoniec, pBrush, 0.5f);	//linie zera rysuj grubszą
		else
			pRenderTarget->DrawLine(pktfPoczatek, pktfKoniec, pBrush, 0.1f);

		rectWartosciOsi.top = pktfPoczatek.y - 10;
		rectWartosciOsi.bottom = rectWartosciOsi.top + 20;
		rectWartosciOsi.left = 0;
		rectWartosciOsi.right = MIEJSCE_NA_LEGENDE;
		if (stKonfigLewy->bWykresPrzechodziPrzezZero)
			fWartosc = n * fSkokPodzialki;
		else
			fWartosc = stKonfigLewy->fMinWykresu + n * fSkokPodzialki;
		
		if (n * fSkokPodzialki < 1)
			strNazwa.Format(_T("%.3f"), fWartosc);
		else if (n * fSkokPodzialki < 10)
			strNazwa.Format(_T("%.2f"), fWartosc);
		else if (n * fSkokPodzialki < 100.f)
			strNazwa.Format(_T("%.1f"), fWartosc);
		else
			strNazwa.Format(_T("%.0f"), fWartosc);

		pRenderTarget->DrawText(strNazwa, rectWartosciOsi, pBrush, m_pTextFormat);
	}
	//lewa strona, podzialki ponizej zera
	if (stKonfigLewy->fMinWykresu < 0)
		nPodzPonizejZera = (int)round(fabs(stKonfigLewy->fMinWykresu) / fSkokPodzialki);
	else
		nPodzPonizejZera = 0;
	for (int n = 1; n <= nPodzPonizejZera; n++)
	{
		pktfPoczatek.x = (float)(stKonfigLewy->rOknoWykresu.left);;
		pktfKoniec.x = (float)(stKonfigLewy->rOknoWykresu.right);
		pktfPoczatek.y = pktfKoniec.y = stKonfigLewy->fPoziomZera + n * fSkokPodzialki * stKonfigLewy->fSkalaY;
		if (pktfPoczatek.y < stKonfigLewy->rOknoWykresu.bottom)	//nie rysuj poniżej okna
		{
			pRenderTarget->DrawLine(pktfPoczatek, pktfKoniec, pBrush, 0.1f);
			rectWartosciOsi.top = pktfPoczatek.y - 10;
			rectWartosciOsi.bottom = rectWartosciOsi.top + 20;
			rectWartosciOsi.left = 0;
			rectWartosciOsi.right = MIEJSCE_NA_LEGENDE;
			if (stKonfigLewy->bWykresPrzechodziPrzezZero)
				fWartosc = n * -fSkokPodzialki;
			else
				fWartosc = stKonfigLewy->fMinWykresu + n * -fSkokPodzialki;

			if (n * fSkokPodzialki < 1)
				strNazwa.Format(_T("%.3f"), fWartosc);
			else if (n * fSkokPodzialki < 10)
				strNazwa.Format(_T("%.2f"), fWartosc);
			else if (n * fSkokPodzialki < 100.f)
				strNazwa.Format(_T("%.1f"), fWartosc);
			else
				strNazwa.Format(_T("%.0f"), fWartosc);
			pRenderTarget->DrawText(strNazwa, rectWartosciOsi, pBrush, m_pTextFormat);
		}
	}

	//prawa strona osi Y: Min2 i Max2, rysuj podziałki dla zera i powyżej
	fSkokPodzialki = ZnajdzPodzialke(stKonfigPrawy->rOknoWykresu, stKonfigPrawy->fMinWykresu, stKonfigPrawy->fMaxWykresu);
	if (stKonfigPrawy->fMaxWykresu > 0)
	{
		if (stKonfigPrawy->bWykresPrzechodziPrzezZero)
			nPodzPowyzejZera = (int)round(fabs(stKonfigPrawy->fMaxWykresu) / fSkokPodzialki);
		else
			nPodzPowyzejZera = (int)round(fabs(stKonfigPrawy->fMaxWykresu - stKonfigPrawy->fMinWykresu) / fSkokPodzialki);
	}
	else
		nPodzPowyzejZera = 0;

	for (int n = 0; n <= nPodzPowyzejZera; n++)
	{
		pktfPoczatek.x = (float)(stKonfigPrawy->rOknoWykresu.left);;
		pktfKoniec.x = (float)(stKonfigPrawy->rOknoWykresu.right);
		pktfPoczatek.y = pktfKoniec.y = stKonfigPrawy->fPoziomZera - n * fSkokPodzialki * stKonfigPrawy->fSkalaY;
		if (n == 0)
			pRenderTarget->DrawLine(pktfPoczatek, pktfKoniec, pBrush, 0.5f);	//linie zera rysuj grubszą
		else
			pRenderTarget->DrawLine(pktfPoczatek, pktfKoniec, pBrush, 0.1f);

		rectWartosciOsi.top = pktfPoczatek.y - 10;
		rectWartosciOsi.bottom = rectWartosciOsi.top + 20;
		rectWartosciOsi.left = (float)stKonfigPrawy->rOknoWykresu.right;
		rectWartosciOsi.right = (float)(stKonfigPrawy->rOknoWykresu.right + MIEJSCE_NA_LEGENDE);
		//fWartosc = stKonfigPrawy->fMinWykresu + n * fSkokPodzialki;
		if (stKonfigPrawy->bWykresPrzechodziPrzezZero)
			fWartosc = n * fSkokPodzialki;
		else
			fWartosc = stKonfigPrawy->fMinWykresu + n * fSkokPodzialki;

		if (n * fSkokPodzialki < 1)
			strNazwa.Format(_T("%.3f"), fWartosc);
		else if (n * fSkokPodzialki < 10)
			strNazwa.Format(_T("%.2f"), fWartosc);
		else if (n * fSkokPodzialki < 100)
			strNazwa.Format(_T("%.1f"), fWartosc);
		else
			strNazwa.Format(_T("%.0f"), fWartosc);

		pRenderTarget->DrawText(strNazwa, rectWartosciOsi, pBrush, m_pTextFormat);
	}
	//prawa strona, podzialki poniżej zera
	nPodzPonizejZera = (int)round(fabs(stKonfigPrawy->fMinWykresu) / fSkokPodzialki);
	for (int n = 1; n <= nPodzPonizejZera; n++)
	{
		pktfPoczatek.x = (float)(stKonfigPrawy->rOknoWykresu.left);;
		pktfKoniec.x = (float)(stKonfigPrawy->rOknoWykresu.right);
		pktfPoczatek.y = pktfKoniec.y = stKonfigPrawy->fPoziomZera + n * fSkokPodzialki * stKonfigPrawy->fSkalaY;
		pRenderTarget->DrawLine(pktfPoczatek, pktfKoniec, pBrush, 0.1f);
		rectWartosciOsi.top = pktfPoczatek.y - 10;
		rectWartosciOsi.bottom = rectWartosciOsi.top + 20;
		rectWartosciOsi.left = (float)stKonfigPrawy->rOknoWykresu.right;;
		rectWartosciOsi.right = (float)(stKonfigPrawy->rOknoWykresu.right + MIEJSCE_NA_LEGENDE);
		//fWartosc = stKonfigPrawy->fMinWykresu + n * -fSkokPodzialki;
		if (stKonfigPrawy->bWykresPrzechodziPrzezZero)
			fWartosc = n * -fSkokPodzialki;
		else
			fWartosc = stKonfigPrawy->fMinWykresu + n * -fSkokPodzialki;
		if (n * fSkokPodzialki < 1)
			strNazwa.Format(_T("%.3f"), fWartosc);
		else if (n * fSkokPodzialki < 10)
			strNazwa.Format(_T("%.2f"), fWartosc);
		else if (n * fSkokPodzialki < 100)
			strNazwa.Format(_T("%.1f"), fWartosc);
		else
			strNazwa.Format(_T("%.0f"), fWartosc);
		pRenderTarget->DrawText(strNazwa, rectWartosciOsi, pBrush, m_pTextFormat);
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Oblicza wielkość podziałki skali Y
// Parametry:
//  [we] okno - prostokąt zokreślający rozmiar okna
//  [we] fMin, fMax - ekstrema sygnału do narysowania wykresu
// zwraca: wartość podziałki
///////////////////////////////////////////////////////////////////////////////////////////////////
float CAPLSNView::ZnajdzPodzialke(CRect okno, float fMin, float fMax)
{
	//okno dzielę na  odcinki po n pixeli, tam może zmieścić się opis skali
	int nProponowanaLiczbaPodzialek = (okno.bottom - okno.top) / ODLEGLOSC_MIEDZY_PODZIALKAMI_OSI;
	float fZakresWskazan;
	float fPierwotnySkokPodzialki;
	float fFinalnySkokPodzialki;
	int nMnoznik = 1;

	if ((fMin < 0.0f) && (fMax > 0.0f))
		fZakresWskazan = (fabsf(fMin) + fabsf(fMax));
	else
		fZakresWskazan = fMax - fMin;

	fPierwotnySkokPodzialki = fFinalnySkokPodzialki = fZakresWskazan / nProponowanaLiczbaPodzialek;
	if (fPierwotnySkokPodzialki)
	{
		while (round(fFinalnySkokPodzialki) < 100)	//potrzeba 3 miejsc znaczących
		//while (round(fFinalnySkokPodzialki) < 10)	//potrzeba 2 miejsc znaczących
		{
			nMnoznik *= 10;
			fFinalnySkokPodzialki = fPierwotnySkokPodzialki * nMnoznik;
		}
	}
	fFinalnySkokPodzialki = (float)round(fFinalnySkokPodzialki) / nMnoznik;
	return fFinalnySkokPodzialki;
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


//zdarzenie aktywacji menu kontekstowego w oknie dokumentu
void CAPLSNView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_POPUP_WIDOKU);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}
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
	CAPLSNDoc* pDoc = GetDocument();

	switch (nInputcode)
	{
	case ON_ACCEPT:		getKomunikacja().AkceptujPolaczenieETH();
		pDoc->SetTitle(L"Ustanowiono połączenie ETH");
		break;

	case ON_SEND:	
		pDoc->SetTitle(L"Wysłano dane");
		break;

	case ON_RECEIVE:
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
	m_cKonfigPolacz.UstawAdresIP(m_chAdresIP);
	m_cKonfigPolacz.UstawTypPolaczenia(m_nTypPolaczenia);
	m_cKonfigPolacz.DoModal();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Łączy się z portem com / UART
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::OnPolaczCom()
{
	getKomunikacja().UstawTypPolaczenia(UART);
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
// Łączy się z portem Ethernet
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::OnPolaczEth()
{
	getKomunikacja().UstawTypPolaczenia(ETHK);
	getKomunikacja().UstawNumerPortuETH(m_nNumerPortuEth);
	getKomunikacja().UstawAdresIP(m_strAdresIP);
	if (getKomunikacja().CzyPolaczonoEth())
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
// wywyła polecenie zrobienia zdjecia
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::OnZrobZdjecie()
{
	uint8_t chErr;

	CAPLSNDoc* pDoc = GetDocument();

	uint32_t rozmiar = sizeof(pDoc->m_sZdjecieRGB565);
	//uruchom wątek aktualizujący pasek postępu pobierania zdjęcia
	m_bKoniecWatkuPaskaPostepu = FALSE;
	pWskWatkuPaskaPostepu = AfxBeginThread((AFX_THREADPROC)WatekRysujPasekPostepu, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);

	m_sLiczbaFragmentowPaskaPostepu = rozmiar / ROZM_DANYCH_UART;
	m_sBiezacyStanPaskaPostepu = 0;
	pDoc->m_bZdjecieGotowe = FALSE;
	chErr = getKomunikacja().ZrobZdjecie(pDoc->m_sZdjecieRGB565);
	if (chErr == ERR_OK)
	{
		pDoc->m_bZdjecieGotowe = TRUE;
		m_bKoniecWatkuPaskaPostepu = TRUE;
		Invalidate();
	}
	else
	{
		CString strKomunikat;
		strKomunikat.Format(L"Wystąpił błąd: %d", chErr);
		MessageBoxW(strKomunikat, L"Ojojoj!");	//dodać flagi
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Aktualizuje stan przycisku zrób zdjęcie w pasku narzędzi
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::OnUpdateZrobZdjecie(CCmdUI* pCmdUI)
{
	m_bPolaczono = getKomunikacja().CzyPolaczonoEth() | getKomunikacja().CzyPolaczonoUart();
	pCmdUI->Enable(m_bPolaczono);
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
			uint32_t nErr = WaitForSingleObject(getProtokol().m_hZdarzenieRamkaTelemetriiGotowa, INFINITE);
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
	m_bPolaczono = getKomunikacja().CzyPolaczonoEth() | getKomunikacja().CzyPolaczonoUart();
	pCmdUI->Enable(m_bPolaczono);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmiane rozmiaru okna. Ustawia zmianną m_nSzerokoscOkna porzebną do aktualizacji stanu pasków przewijania
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	m_nSzerokoscOkna = cx;
	m_nSzerokoscWykresu = m_nSzerokoscOkna - 2 * MIEJSCE_NA_LEGENDE;
	
	/*CAPLSNDoc* pDoc = GetDocument();
	if (pDoc)
	{
		if (pDoc->m_vLog.size())
			m_nIloscDanychWykresu = (int)pDoc->m_vLog[9].vfWartosci.size();
	}*/

	m_pLinearGradientBrush->SetEndPoint(CPoint(cx, cy));
	UstawScrollOdWidoku();
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



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na przewijanie w pionie. Modyfikuje zmienną m_nVscroll dla różnych sposobów zmiany położenia paska przewijania
// Parametry:
//  nSBCode - komunikat okreslający sposobów zmiany położenia paska przewijania
//  nPos - liczbowa wartość paska przewijania gdy był przeciagany w konkretne miejsce
//  pScrollBar* - wskaxnik na nasek przewijania
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
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



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na przewijanie w poziomie. Modyfikuje zmienną m_nVscroll dla różnych sposobów zmiany położenia paska przewijania
// Parametry:
//  nSBCode - komunikat okreslający sposobów zmiany położenia paska przewijania
//  nPos - liczbowa wartość paska przewijania gdy był przeciagany w konkretne miejsce
//  pScrollBar* - wskaxnik na nasek przewijania
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Dodaj tutaj swój kod procedury obsługi komunikatów i/lub wywołaj domyślny
	//UINT nKrok = (UINT)(5 * m_fZoomPoziomo);
	UINT nKrok = 5;
	//UINT nStrona = (UINT)round(m_nSzerokoscWykresu * m_fZoomPoziomo);
	UINT nStrona = m_nSzerokoscWykresu;
	switch (nSBCode)
	{
	case SB_LEFT:			m_nBiezacyScrollPoziomo = 0;	break;	// Scroll to far left.
	case SB_ENDSCROLL:		break;	//End scroll.
	case SB_LINELEFT:		if (m_nBiezacyScrollPoziomo > (int)nKrok)	m_nBiezacyScrollPoziomo -= nKrok;	break;// Scroll left.
	case SB_LINERIGHT:		m_nBiezacyScrollPoziomo += nKrok;	break;	//Scroll right.
	case SB_PAGELEFT:		if (m_nBiezacyScrollPoziomo > (int)nStrona)	m_nBiezacyScrollPoziomo -= nStrona;	break;	//Scroll one page left.
	case SB_PAGERIGHT:		m_nBiezacyScrollPoziomo += nStrona;	break;	//Scroll one page right.
	case SB_RIGHT:			m_nBiezacyScrollPoziomo = nPos;	break;	//Scroll to far right.
	case SB_THUMBPOSITION:	m_nBiezacyScrollPoziomo = nPos;	break; //Scroll to absolute position.The current position is specified by the nPos parameter.
	case SB_THUMBTRACK:		m_nBiezacyScrollPoziomo = nPos;	break;	//Drag scroll
	}
	
	CView::OnHScroll(nSBCode, m_nBiezacyScrollPoziomo, pScrollBar);
	UstawScrollOdWidoku();
	Invalidate();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Funkcja uaktualnia położenie obu pasków przewijania. Uruchamiane po zmianie stanu okna gdy np przyjdą nowe dane telemetryczne albo zostanie wczytany nowy log
// Parametry: brak
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::UstawScrollOdWidoku()
{
	SCROLLINFO scrollInfo;

	if (!GetScrollInfo(SB_HORZ, &scrollInfo))
	{
		TRACE("Can't get scroll info");
		return;
	}
	m_nMaxScrollPoziomo = max(((int)round(m_nIloscDanychWykresu * m_fZoomPoziomo) - m_nSzerokoscWykresu) , 0);
	m_nBiezacyScrollPoziomo = min(m_nBiezacyScrollPoziomo, m_nMaxScrollPoziomo);

	scrollInfo.nMin = 0;
	//scrollInfo.nMax = (int)(m_nIloscDanychWykresu * m_fZoomPoziomo) - m_nSzerokoscWykresu;
	scrollInfo.nMax = m_nMaxScrollPoziomo;
	scrollInfo.nPos = (int)round(m_nBiezacyScrollPoziomo * m_fZoomPoziomo);
	scrollInfo.nPage = m_nSzerokoscWykresu / 4;
	VERIFY(SetScrollInfo(SB_HORZ, &scrollInfo));
	TRACE("BieżScroll=%d, MaxScroll=%d, Wykres=%d, zoom=%.3f\n", m_nBiezacyScrollPoziomo, m_nMaxScrollPoziomo, m_nSzerokoscWykresu, m_fZoomPoziomo);
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


///////////////////////////////////////////////////////////////////////////////////////////////////
// Uruchamia konfigurację wykresów z menu kontekstowego
// Parametry: brak
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::OnKonfigWykresow()
{
	m_cKonfiguracjaWykresow.DoModal();
}


// aktualizacja dostepnosci polecenia w zależności od stanu połączenia
void CAPLSNView::OnUpdateKonfigWykresow(CCmdUI* pCmdUI)
{
	if (m_bPolaczono || GetDocument()->m_bOdczytanoLog)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Uruchamia konfigurację telemetrii z menu kontekstowego
// Parametry: brak
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::OnKonfigTelemetrii()
{
	m_cKonfigTelemetrii.DoModal();
}


// aktualizacja dostepnosci polecenia w zależności od stanu połączenia
void CAPLSNView::OnUpdateKonfigTelemetrii(CCmdUI* pCmdUI)
{
	if (m_bPolaczono)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Uruchamia konfigurację rejestratora z menu kontekstowego
// Parametry: brak
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::OnKonfigRejestratora()
{
	m_cKonfigRejestratora.DoModal();
}


void CAPLSNView::OnUpdateKonfigRejestratora(CCmdUI* pCmdUI)
{
	
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Pobiera wyniki FFT po naciśnięciu przycisku w pasku narzedziowym
// Parametry: brak
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::OnButPobierzFft()
{
	uint8_t chBłąd;
	int nIndeksPomiaru, nRozmiarFFT;
	uint8_t chWykładnikPotęgi, chRodzajOkna, chAktywneSilniki;
	uint16_t sMaxWysterowanie;
	CAPLSNDoc* pDoc = GetDocument();

	chBłąd = getKomunikacja().CzytajParametryFFT(&chWykładnikPotęgi, &chRodzajOkna, &chAktywneSilniki, &sMaxWysterowanie);
	if (chBłąd)
		return;
	nRozmiarFFT = (int)powf(2.0f, (float)chWykładnikPotęgi);

	for (int nTestu = 0; nTestu < LICZBA_TESTOW_FFT; nTestu++)
	{
		//for (int nZmienna = 0; nZmienna < LICZBA_ZMIENNYCH_FFT; nZmienna++)
		for (int nZmienna = 0; nZmienna < LICZBA_WYKRESOW_FFT; nZmienna++)		
		{
			for (int nRamkaPomiaru = 0; nRamkaPomiaru < ((nRozmiarFFT / 2) / LICZB_FLOAT_WRAMCE); nRamkaPomiaru++)
			{
				chBłąd = getKomunikacja().CzytajWynikiFFT(nTestu, nZmienna, nRamkaPomiaru, &pDoc->m_fWynikFFT[nTestu][nZmienna][nRamkaPomiaru * LICZB_FLOAT_WRAMCE], LICZB_FLOAT_WRAMCE);
				if (chBłąd)
					return;
				pDoc->m_bFFTGotowe = TRUE;
			}
		}
	}


	pDoc->m_bFFTGotowe = TRUE;
}


// aktualizacja dostepnosci polecenia w zależności od stanu połączenia
void CAPLSNView::OnUpdateButPobierzFft(CCmdUI* pCmdUI)
{
	if (m_bPolaczono)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}
