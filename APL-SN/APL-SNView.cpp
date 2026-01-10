
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
, m_nIloscDanychWykresu(0)
, m_nMaxScrollPoziomo(0)
, m_nBiezacyScrollPoziomo(0)
, m_sLiczbaFragmentowPaskaPostepu(0)
, m_sBiezacyStanPaskaPostepu(0)
, m_nSzerokoscOkna(0)
, m_nSzerokoscWykresu(0)
, pWskWatkuPaskaPostepu(0)
, pWskWatkuOdswiezaniaTelemetrii(0)
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
	//int nGora, nDol;
	int nLiczbaWykresow;
	int nIdZmiennej;
	float fStartLegendy;
	float fMinWykresu = 0.0f, fMaxWykresu = 0.0f;
	float fMinLewy, fMinPrawy, fMaxLewy, fMaxPrawy;	//ekstrema dwóch pierwszych wykresów
	CRect oknoGrupy, OknoWykresu;
	GetClientRect(oknoGrupy);
	OknoWykresu = oknoGrupy;
	m_bOknoGotowe = TRUE;

	pRenderTarget->FillRectangle(oknoGrupy, m_pLinearGradientBrush);
	float fSkalaX = m_fZoomPoziomo;
	float fSkalaY = (float)OknoWykresu.bottom / 40.0f * m_fZoomPionowo;
	float fPoziomZera, fPoziomZeraLewy, fPoziomZeraPrawy;

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


	//rysowanie wykresów telemetrii
	if (m_bRysujTelemetrie || m_cKonfiguracjaWykresow.m_bZawieraLog)	//czy są dane telemetryczne lub wczytany z pliku log
	{
		m_bRysujTelemetrie = FALSE;
		OknoWykresu.left = MIEJSCE_PRZED_WYKRESEM;
		OknoWykresu.right -= MIEJSCE_PRZED_WYKRESEM;

		//rysuj okna grup wykresów
		int nLiczbaGrupWykresow = (int)m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow.size();
		OknoWykresu.top = MIEJSCE_MIEDZY_WYKRESAMI;
		for (int g = 0; g < nLiczbaGrupWykresow; g++)
		{
			fMinLewy = fMaxLewy = fPoziomZeraLewy = fMinPrawy = fMaxPrawy = fPoziomZeraPrawy = 0.0f;;
			fStartLegendy = (float)(OknoWykresu.left + 5);	//wspólrzędne x początku legendy, będą zwiększane w każdej iteracji aby opisy nie nachodziły na siebie
			OknoWykresu.bottom = (g + 1) * oknoGrupy.bottom / nLiczbaGrupWykresow - MIEJSCE_MIEDZY_WYKRESAMI / 2;			
			if (m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow[g].chTypWykresu == WYKRES_WSPOLNA_SKALA)
			{
				//znajdź globalne ekstrema w grupie wykresów o wspólnej skali
				nLiczbaWykresow = (int)m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne.size();
				fMinWykresu = fMaxWykresu = 0.0f;
				for (int w = 0; w < nLiczbaWykresow; w++)
				{
					nIdZmiennej = m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne[w].sIdZmiennej;
					if (m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne[w].chZrodloZmiennej == ZRODLO_TELEMETRIA)	//0 == telemetria
					{
						if (fMinWykresu > getProtokol().m_stEkstremaTelemetrii[nIdZmiennej].fMin)
							fMinWykresu = getProtokol().m_stEkstremaTelemetrii[nIdZmiennej].fMin;
						if (fMaxWykresu < getProtokol().m_stEkstremaTelemetrii[nIdZmiennej].fMax)
							fMaxWykresu = getProtokol().m_stEkstremaTelemetrii[nIdZmiennej].fMax;
					}
					else               //log
					{
						if (fMinWykresu > pDoc->m_vLog[nIdZmiennej].fMin)
							fMinWykresu = pDoc->m_vLog[nIdZmiennej].fMin;
						if (fMaxWykresu < pDoc->m_vLog[nIdZmiennej].fMax)
							fMaxWykresu = pDoc->m_vLog[nIdZmiennej].fMax;
					}
				}
				fMinLewy = fMinPrawy = fMinWykresu;
				fMaxLewy = fMaxPrawy = fMaxWykresu;
				if ((fMinWykresu == 0.0) && (fMaxWykresu == 0.0f))
					return FALSE;
				fSkalaY = (OknoWykresu.bottom - OknoWykresu.top) / (fabsf(fMinWykresu) + fabsf(fMaxWykresu));
				fPoziomZeraLewy = fPoziomZeraPrawy = fPoziomZera = (float)OknoWykresu.bottom - fSkalaY * (float)fabsf(fMinWykresu);
				for (int w = 0; w < nLiczbaWykresow; w++)
				{
					nIdZmiennej = m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne[w].sIdZmiennej;					
					m_pBrushWykresu->SetColor(m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne[w].cKolorD2D1);
					if (m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne[w].chZrodloZmiennej == ZRODLO_TELEMETRIA)	//0 == telemetria
						RysujWykresTelemetrii(OknoWykresu, (float)m_nBiezacyScrollPoziomo, fPoziomZera, fSkalaX, fSkalaY, &getProtokol().m_vDaneTelemetryczne, nIdZmiennej, pRenderTarget, m_pBrushWykresu, &fStartLegendy);
					else
					{
						int nID = m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne[w].sIdZmiennej;
						RysujWykresLogu(OknoWykresu, (float)m_nBiezacyScrollPoziomo, fPoziomZera, fSkalaX, fSkalaY, nID, pRenderTarget, m_pBrushWykresu);
					}
				}
			}	
			if (m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow[g].chTypWykresu == WYKRES_OSOBNA_SKALA)
			{
				nLiczbaWykresow = (int)m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne.size();
				for (int w = 0; w < nLiczbaWykresow; w++)
				{
					nIdZmiennej = m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne[w].sIdZmiennej;
					fMinWykresu = fMaxWykresu = 0.0f;
					if (m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne[w].chZrodloZmiennej == ZRODLO_TELEMETRIA)	//0 == telemetria
					{
						if (fMinWykresu > getProtokol().m_stEkstremaTelemetrii[nIdZmiennej].fMin)
							fMinWykresu = getProtokol().m_stEkstremaTelemetrii[nIdZmiennej].fMin;
						if (fMaxWykresu < getProtokol().m_stEkstremaTelemetrii[nIdZmiennej].fMax)
							fMaxWykresu = getProtokol().m_stEkstremaTelemetrii[nIdZmiennej].fMax;
					}
					else            //log
					{
						if (fMinWykresu > pDoc->m_vLog[nIdZmiennej].fMin)
							fMinWykresu = pDoc->m_vLog[nIdZmiennej].fMin;
						if (fMaxWykresu < pDoc->m_vLog[nIdZmiennej].fMax)
							fMaxWykresu = pDoc->m_vLog[nIdZmiennej].fMax;
					}
						
					fSkalaY = (OknoWykresu.bottom - OknoWykresu.top) / (fabsf(fMinWykresu) + fabsf(fMaxWykresu));
					fPoziomZera = (float)OknoWykresu.bottom - (fSkalaY * (float)fabsf(fMinWykresu));					
					if (w == 0)	//skala lewa dla wykresu 1
					{
						fMinLewy = fMinWykresu;
						fMaxLewy = fMaxWykresu;
						fPoziomZeraLewy = fPoziomZera;
					}
					if (w == 1)	//skala prawa dla wykresu 2
					{
						fMinPrawy = fMinWykresu;
						fMaxPrawy = fMaxWykresu;
						fPoziomZeraPrawy = fPoziomZera;
					}					
					m_pBrushWykresu->SetColor(m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne[w].cKolorD2D1);
					if (m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne[w].chZrodloZmiennej == ZRODLO_TELEMETRIA)	//0 == telemetria
						RysujWykresTelemetrii(OknoWykresu, (float)m_nBiezacyScrollPoziomo, fPoziomZera, fSkalaX, fSkalaY, &getProtokol().m_vDaneTelemetryczne, nIdZmiennej, pRenderTarget, m_pBrushWykresu, &fStartLegendy);
					else
					{
						int nID = m_cKonfiguracjaWykresow.m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne[w].sIdZmiennej;
						RysujWykresLogu(OknoWykresu, (float)m_nBiezacyScrollPoziomo, fPoziomZera, fSkalaX, fSkalaY, nID, pRenderTarget, m_pBrushWykresu);
					}
				}
			}
			fPoziomZera = (float)OknoWykresu.bottom - fSkalaY * (float)fabsf(fMinWykresu);
			RysujOknoGrupyWykresow(OknoWykresu, fPoziomZeraLewy, fPoziomZeraPrawy, fMinLewy, fMaxLewy, fMinPrawy, fMaxPrawy, pRenderTarget, m_pBrushOsiWykresu);
			OknoWykresu.top = OknoWykresu.bottom + MIEJSCE_MIEDZY_WYKRESAMI;
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
// Rysuje wykres logu pobrany z telemetrii
// Parametry:
//  okno - obszar rysowania
//  fHscroll - przesunięcie danych w poziomie podpięte do poziomego paska przewijania
//  fVpos - przesuniecie środka wykresu w pionie po to aby upchnąc wiele wykresów w oknie
//  fSkalaX, fSkalaY - wspólczynniki skalowania danych w poziomie i pionie sterowane kólkiem myszy (X) i kólkiem z Shift (Y)
//  vRamkaTele - wektor zmiennych telemetrycznych z którego trzeba wyłuskać potrzebna dane
//  nIndeksZmiennej - indeks zmiennej do wyświetlenia
//  pRenderTarget - wskaźnik na narzędzie renderujące cały wykres
//  pBrush - wskaźnik na narzędzie rysujace określonym kolorem
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::RysujWykresTelemetrii(CRect okno, float fHscroll, float fVzera, float fSkalaX, float fSkalaY, std::vector<stTelemetria_t> *vDaneTele, int nIndeksZmiennej, CHwndRenderTarget *pRenderTarget, CD2DSolidColorBrush *pBrush, float *fStartLegendy)
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
	rectLegenda.top = (float)(okno.top + 5);
	rectLegenda.bottom = (float)(rectLegenda.top + 20);
	rectLegenda.left = *fStartLegendy;
	rectLegenda.right = (float)(rectLegenda.left + 120);
	*fStartLegendy = rectLegenda.right;
	pRenderTarget->DrawText(strNazwa, rectLegenda, pBrush, m_pTextFormat);

	//rysuj wykres
	pktfPoczatek.x = (float)okno.left + fHscroll;
	pktfKoniec.x = 1.0f * fSkalaX + okno.left + fHscroll;

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

	pktfPoczatek.y = fVzera - (fZmienna * fSkalaY);

	do    //sprawdzaj wektor ramki od końca aż napełni się wykres
	{
		fZmienna = (*vDaneTele)[nIndexRamki--].dane[nIndeksZmiennej];
		if (fZmienna)
		{
			pktfKoniec.x += fSkalaX;
			pktfKoniec.y = fVzera - (fZmienna * fSkalaY);
			pRenderTarget->DrawLine(pktfPoczatek, pktfKoniec, pBrush);
			pktfPoczatek = pktfKoniec;
		}
	} while ((pktfKoniec.x < okno.right) && (nIndexRamki >= 0));	//pobierz danych na szerokość okna lub tyle ile się da
}




///////////////////////////////////////////////////////////////////////////////////////////////////
// Rysuje oś układu wspórzędnych dla grupy wykresów
// Parametry: 
//  okno - współrzedne okna wszystkich grup wykresów
//  nGora, nDol - współrzedne Y góry i dołu grupy wykresów
//  pRenderTarget - narzędzie rysujące
//  pBrush - parametry pędzla rysujacego osie
// zwraca: komunkat systemowy
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::RysujOknoGrupyWykresow(CRect okno, float fZero1, float fZero2, float fMin1, float fMax1, float fMin2, float fMax2, CHwndRenderTarget* pRenderTarget, CD2DSolidColorBrush* pBrush)
{
	CD2DPointF pktfPoczatek, pktfKoniec;
	CD2DRectF rectWartosciOsi;
	CString strNazwa;
	float fSkokPodzialki;
	float fSkalaY;
	int nPodzPonizejZera;
	int nPodzPowyzejZera;

	//ramka okna wykresu
	pktfPoczatek.x = pktfKoniec.x = (float)(okno.left);
	pktfPoczatek.y = (float)okno.top;
	pktfKoniec.y = (float)okno.bottom;
	pRenderTarget->DrawLine(pktfPoczatek, pktfKoniec, pBrush, 0.4f);
	pktfPoczatek = pktfKoniec;
	pktfKoniec.x = (float)(okno.right);
	pRenderTarget->DrawLine(pktfPoczatek, pktfKoniec, pBrush, 0.4f);
	pktfPoczatek = pktfKoniec;
	pktfKoniec.y = (float)okno.top;
	pRenderTarget->DrawLine(pktfPoczatek, pktfKoniec, pBrush, 0.4f);
	pktfPoczatek = pktfKoniec;
	pktfKoniec.x = (float)(okno.left);
	pRenderTarget->DrawLine(pktfPoczatek, pktfKoniec, pBrush, 0.4f);

	//lewa strona osi Y: Min1 i Max1, rysuj podziałki dla zera i powyżej
	fSkalaY = (okno.bottom - okno.top) / (fabsf(fMin1) + fabsf(fMax1));
	fSkokPodzialki = ZnajdzPodzialke(okno, fMin1, fMax1);
	nPodzPowyzejZera = (int)round(fabs(fMax1) / fSkokPodzialki);
	for (int n = 0; n <= nPodzPowyzejZera; n++)
	{
		pktfPoczatek.x = (float)(okno.left);;
		pktfKoniec.x = (float)(okno.right);
		pktfPoczatek.y = pktfKoniec.y = fZero1 - n* fSkokPodzialki * fSkalaY;
		if (n==0)
			pRenderTarget->DrawLine(pktfPoczatek, pktfKoniec, pBrush, 0.5f);	//linie zera rysuj grubszą
		else
			pRenderTarget->DrawLine(pktfPoczatek, pktfKoniec, pBrush, 0.1f);

		rectWartosciOsi.top = pktfPoczatek.y - 10;
		rectWartosciOsi.bottom = rectWartosciOsi.top + 20;
		rectWartosciOsi.left = 0;
		rectWartosciOsi.right = MIEJSCE_PRZED_WYKRESEM;
		if (n * fSkokPodzialki < 1)
			strNazwa.Format(_T("%.3f"), n * fSkokPodzialki);
		else if (n * fSkokPodzialki < 10)
			strNazwa.Format(_T("%.2f"), n * fSkokPodzialki);
		else if (n * fSkokPodzialki < 100.f)
			strNazwa.Format(_T("%.1f"), n * fSkokPodzialki);
		else
			strNazwa.Format(_T("%.0f"), n * fSkokPodzialki);

		pRenderTarget->DrawText(strNazwa, rectWartosciOsi, pBrush, m_pTextFormat);
	}
	//lewa strona, podzialki ponizej zera
	nPodzPonizejZera = (int)round(fabs(fMin1) / fSkokPodzialki);
	for (int n = 1; n <= nPodzPonizejZera; n++)
	{
		pktfPoczatek.x = (float)(okno.left);;
		pktfKoniec.x = (float)(okno.right);
		pktfPoczatek.y = pktfKoniec.y = fZero1 + n * fSkokPodzialki * fSkalaY;
		if (pktfPoczatek.y < okno.bottom)	//nie rysuj poniżej okna
		{
			pRenderTarget->DrawLine(pktfPoczatek, pktfKoniec, pBrush, 0.1f);
			rectWartosciOsi.top = pktfPoczatek.y - 10;
			rectWartosciOsi.bottom = rectWartosciOsi.top + 20;
			rectWartosciOsi.left = 0;
			rectWartosciOsi.right = MIEJSCE_PRZED_WYKRESEM;
			if (n * fSkokPodzialki < 1)
				strNazwa.Format(_T("%.3f"), n * -fSkokPodzialki);
			else if (n * fSkokPodzialki < 10)
				strNazwa.Format(_T("%.2f"), n * -fSkokPodzialki);
			else if (n * fSkokPodzialki < 100.f)
				strNazwa.Format(_T("%.1f"), n * -fSkokPodzialki);
			else
				strNazwa.Format(_T("%.0f"), n * -fSkokPodzialki);
			pRenderTarget->DrawText(strNazwa, rectWartosciOsi, pBrush, m_pTextFormat);
		}
	}

	//prawa strona osi Y: Min2 i Max2, rysuj podziałki dla zera i powyżej
	fSkalaY = (okno.bottom - okno.top) / (fabsf(fMin2) + fabsf(fMax2));
	fSkokPodzialki = ZnajdzPodzialke(okno, fMin2, fMax2);
	nPodzPowyzejZera = (int)round(fabs(fMax2) / fSkokPodzialki);
	for (int n = 0; n <= nPodzPowyzejZera; n++)
	{
		pktfPoczatek.x = (float)(okno.left);;
		pktfKoniec.x = (float)(okno.right);
		pktfPoczatek.y = pktfKoniec.y = fZero2 - n * fSkokPodzialki * fSkalaY;
		if (n == 0)
			pRenderTarget->DrawLine(pktfPoczatek, pktfKoniec, pBrush, 0.5f);	//linie zera rysuj grubszą
		else
			pRenderTarget->DrawLine(pktfPoczatek, pktfKoniec, pBrush, 0.1f);

		rectWartosciOsi.top = pktfPoczatek.y - 10;
		rectWartosciOsi.bottom = rectWartosciOsi.top + 20;
		rectWartosciOsi.left = (float)okno.right;
		rectWartosciOsi.right = (float)(okno.right + MIEJSCE_PRZED_WYKRESEM);
		if (n * fSkokPodzialki < 1)
			strNazwa.Format(_T("%.3f"), n * fSkokPodzialki);
		else if (n * fSkokPodzialki < 10)
			strNazwa.Format(_T("%.2f"), n * fSkokPodzialki);
		else if (n * fSkokPodzialki < 100)
			strNazwa.Format(_T("%.1f"), n * fSkokPodzialki);
		else
			strNazwa.Format(_T("%.0f"), n * fSkokPodzialki);

		pRenderTarget->DrawText(strNazwa, rectWartosciOsi, pBrush, m_pTextFormat);
	}
	//prawa strona, podzialki ponizej zera
	nPodzPonizejZera = (int)round(fabs(fMin2) / fSkokPodzialki);
	for (int n = 1; n <= nPodzPonizejZera; n++)
	{
		pktfPoczatek.x = (float)(okno.left);;
		pktfKoniec.x = (float)(okno.right);
		pktfPoczatek.y = pktfKoniec.y = fZero2 + n * fSkokPodzialki * fSkalaY;
		pRenderTarget->DrawLine(pktfPoczatek, pktfKoniec, pBrush, 0.1f);
		rectWartosciOsi.top = pktfPoczatek.y - 10;
		rectWartosciOsi.bottom = rectWartosciOsi.top + 20;
		rectWartosciOsi.left = (float)okno.right;;
		rectWartosciOsi.right = (float)(okno.right + MIEJSCE_PRZED_WYKRESEM);
		if (n * fSkokPodzialki < 1)
			strNazwa.Format(_T("%.3f"), n * -fSkokPodzialki);
		else if (n * fSkokPodzialki < 10)
			strNazwa.Format(_T("%.2f"), n * -fSkokPodzialki);
		else if (n * fSkokPodzialki < 100)
			strNazwa.Format(_T("%.1f"), n * -fSkokPodzialki);
		else
			strNazwa.Format(_T("%.0f"), n * -fSkokPodzialki);
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
	float fZakresWskazan = fabsf(fMax) + fabsf(fMin);
	float fPierwotnySkokPodzialki = fZakresWskazan / nProponowanaLiczbaPodzialek;
	float fFinalnySkokPodzialki = fPierwotnySkokPodzialki;
	int nMnoznik = 1;
	if (fPierwotnySkokPodzialki)
	{
		while (round(fFinalnySkokPodzialki) < 100)	//potrzeba 3 miejsc znaczących
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
	m_nSzerokoscWykresu = m_nSzerokoscOkna - 2 * MIEJSCE_PRZED_WYKRESEM;
	
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

