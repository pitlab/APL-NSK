#include "pch.h"
#include "APL-SN.h"
#include "Sfera3DView.h"
#include "Sfera3DDoc.h"
#include "sys_def_wspolny.h"
#include "KodyBledow.h"

IMPLEMENT_DYNCREATE(CSfera3DView, CView)

BEGIN_MESSAGE_MAP(CSfera3DView, CView)
    ON_REGISTERED_MESSAGE(AFX_WM_DRAW2D, &CSfera3DView::OnDraw2d)
ON_WM_MOUSEWHEEL()
ON_WM_RBUTTONDOWN()
ON_WM_RBUTTONUP()
ON_WM_MOUSEMOVE()
ON_WM_CREATE()
//ON_BN_CLICKED(ID_CHECKBOX_MAG1, CSfera3DView::OnCheckboxMag1Clicked)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////////////////////////
//Konstruktor
///////////////////////////////////////////////////////////////////////////////////////////////////
CSfera3DView::CSfera3DView()
: pWskWatkuOdswiezaniaSfery3D(FALSE)
, m_bOknoGotowe(FALSE)
, m_bObracanieMyszk¹(FALSE)
{
	//m_bKoniecWatkuOdswiezaniaTelemtrii = FALSE;
	//m_bOknoGotowe = FALSE;	//nie pobieraj jeszcze danych bo okno rysowania nie jest gotowe
    EnableD2DSupport();

	m_pBrushBlack = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::Black));
	m_pBrushGray = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::Gray));
	m_pBrushR = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::Red));
	m_pBrushG = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::Green));
	m_pBrushB = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::Blue));

	m_pTextFormatTytu³ = new CD2DTextFormat(GetRenderTarget(), _T("Verdana"), 14);
	m_pTextFormatOpis = new CD2DTextFormat(GetRenderTarget(), _T("Verdana"), 11);
	m_pTextFormatTytu³->Get()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);

	D2D1_GRADIENT_STOP gradientStopsT³a[2] = {
	{0.0f, D2D1::ColorF(D2D1::ColorF::White)},
	{1.0f, D2D1::ColorF(D2D1::ColorF::Cornsilk)}};
	m_pLinearGradientBrush = new CD2DLinearGradientBrush(GetRenderTarget(), gradientStopsT³a, ARRAYSIZE(gradientStopsT³a), D2D1::LinearGradientBrushProperties(D2D1::Point2F(0, 0), D2D1::Point2F(0, 0)));

	//je¿eli jest po³¹czenie to uruchom w¹tek inwaliduj¹cy widok sfery gdy pojawi¹ siê nowe dane telemetryczne
	if (getKomunikacja().CzyPolaczonoUart())
	{
		pWskWatkuOdswiezaniaSfery3D = AfxBeginThread((AFX_THREADPROC)WatekInvalidujStanSfery, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////
//Destruktor
///////////////////////////////////////////////////////////////////////////////////////////////////
CSfera3DView::~CSfera3DView()
{
	m_bKoniecWatkuOdswiezaniaTelemtrii = TRUE;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//OnCreate tworzy checkoxy wyboru magnetometru w oknie
///////////////////////////////////////////////////////////////////////////////////////////////////
int CSfera3DView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Tworzenie checkboxa wewn¹trz okna widoku (wspó³rzêdne x, y, szerokoœæ, wysokoœæ)
	//m_cbMag1.Create(_T("Magnetometr 1"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, CRect(10, 40, 200, 56), this, ID_CHECKBOX_MAG1);
	//m_cbMag2.Create(_T("Magnetometr 2"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, CRect(10, 60, 200, 77), this, ID_CHECKBOX_MAG2);
	//m_cbMag3.Create(_T("Magnetometr 3"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, CRect(10, 80, 200, 98), this, ID_CHECKBOX_MAG3);

	return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Standardowa funkcja rysujaca nie jest u¿ywana ale nie mo¿na jej usun¹æ
///////////////////////////////////////////////////////////////////////////////////////////////////
void CSfera3DView::OnDraw(CDC* pDC)
{

}



///////////////////////////////////////////////////////////////////////////////////////////////////
// G³ówna funkcja rysujaca bry³ê wykorzystujaca Direct 2D
// Parametry: 
//  wParam - ?
//  lParam - wskaŸnik na CHwndRenderTarget
// zwraca: komunkat systemowy
///////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CSfera3DView::OnDraw2d(WPARAM wParam, LPARAM lParam)
{    
	CRect rOkno, rObiekt;
	GetClientRect(&rOkno);
	CD2DSizeF promien;
	CD2DPointF ptStart, ptStop;
	int nOffsetY = 9 * rOkno.bottom / 10;
	float wekt_we[3], wekt_wy[3], kat_obr[3] = { 0,0,0 };
	float fDlugoscWektoraMagn;
	double dBladDopasowania2D = 0;	//b³¹d dopasowania danych do równania okrêgu
	CString strText;
	size_t zRozmiar;

	CSfera3DDoc* pDoc = (CSfera3DDoc*)GetDocument();
    ASSERT_VALID(pDoc);

	//pDoc->SetTitle(_T("Magnetometr 3D"));	//powoduje wyj¹tek

    CHwndRenderTarget* pRenderTarget = reinterpret_cast<CHwndRenderTarget*>(lParam);
    ASSERT_VALID(pRenderTarget);
    if (pRenderTarget == nullptr)
        return FALSE;

    ID2D1RenderTarget* pRT = pRenderTarget->GetRenderTarget();
    if (pRT == nullptr)
      return FALSE;

	pRenderTarget->FillRectangle(rOkno, m_pLinearGradientBrush);
	m_bOknoGotowe = TRUE;	//mo¿na zacz¹æ pobieraæ dane do rysowania sfery

		//Tytu³ wykresu
	rObiekt = { rOkno.left + 15, 10, rOkno.right, 25 };
	pRenderTarget->DrawText(_T("Wykres biegunowy 3D magnetometru"), rObiekt, m_pBrushBlack, m_pTextFormatTytu³);
	rObiekt = { rOkno.left + 15, 25, rOkno.right, 40 };
	pRenderTarget->DrawText(_T("Obrót: PPM + mysz"), rObiekt, m_pBrushBlack, m_pTextFormatOpis);

	//rysuj uk³ad wspó³rzednych
	int rozmiar = rOkno.bottom;
	if (rOkno.right < rozmiar) //wykres XY jest symetryczny, wiêc rozmiarem bêdzie mniejszy z rozmiarów okna
		rozmiar = rOkno.right;

	float fSkala = (float)rozmiar / (NOMINALNE_MAGN)*m_fZoom;

	//oœ pozioma Y
	ptStart.x = (float)(rOkno.right - rozmiar) / 2;
	ptStart.y = (float)rOkno.bottom / 2;
	ptStop.x = (float)(rOkno.right - rozmiar) / 2 + rozmiar;
	ptStop.y = (float)rOkno.bottom / 2;
	pRenderTarget->DrawLine(ptStart, ptStop, m_pBrushBlack);
	rObiekt = { (int)ptStop.x - 20, (int)ptStop.y, (int)ptStop.x, (int)ptStop.y + 20 };
	pRenderTarget->DrawText(_T("Y-"), rObiekt, m_pBrushBlack, m_pTextFormatTytu³);
	rObiekt = { (int)ptStart.x + 5, (int)ptStart.y, (int)ptStart.x + 30, (int)ptStart.y + 20 };
	pRenderTarget->DrawText(_T("Y+"), rObiekt, m_pBrushBlack, m_pTextFormatTytu³);

	//oœ pionowa X
	ptStart.x = (float)(rOkno.right) / 2;
	ptStart.y = (float)(rOkno.bottom - rozmiar) / 2;
	ptStop.x = (float)(rOkno.right / 2);
	ptStop.y = (float)(rOkno.bottom - rozmiar) / 2 + rozmiar;
	pRenderTarget->DrawLine(ptStart, ptStop, m_pBrushBlack);
	rObiekt = { (int)ptStart.x + 2, (int)ptStart.y + 5, (int)ptStart.x + 30, (int)ptStart.y + 20 };
	pRenderTarget->DrawText(_T("X+"), rObiekt, m_pBrushBlack, m_pTextFormatTytu³);
	rObiekt = { (int)ptStop.x + 2, (int)ptStop.y - 20, (int)ptStop.x + 30, (int)ptStop.y };
	pRenderTarget->DrawText(_T("X-"), rObiekt, m_pBrushBlack, m_pTextFormatTytu³);

	//okr¹g o sta³ym promieniu
	ptStart.x = (float)rOkno.right / 2;
	ptStart.y = (float)rOkno.bottom / 2;
	promien.height = promien.width = (float)(rozmiar * 0.4f);
	CD2DEllipse okrag = CD2DEllipse(ptStart, promien);
	pRenderTarget->DrawEllipse(okrag, m_pBrushGray);

	//okr¹g dla plaskiego obrotu
	okrag.point.x = (float)rOkno.right / 2;
	okrag.point.y = (float)rOkno.bottom / 2;
	okrag.radiusX = okrag.radiusY = fSkala * NOMINALNE_MAGN * cosf(INKLINACJA_MAG);
	pRenderTarget->DrawEllipse(okrag, m_pBrushBlack);

	//okr¹g dla zwyk³ego obrotu
	okrag.point.x = (float)rOkno.right / 2;
	okrag.point.y = (float)rOkno.bottom / 2;
	okrag.radiusX = okrag.radiusY = fSkala * NOMINALNE_MAGN;
	pRenderTarget->DrawEllipse(okrag, m_pBrushBlack);

	//rysuj sferê
	zRozmiar = getProtokol().m_vDaneTelemetryczne.size();
	ptStart.x = (float)(rOkno.right) / 2;
	ptStart.y = (float)(rOkno.bottom - rozmiar) / 2;

	//dodaj k¹t obrotu zadany myszk¹
	kat_obr[0] = m_ptZmianaPozycjiMyszy.y * 0.005f;
	kat_obr[1] = m_ptZmianaPozycjiMyszy.x * 0.005f;
	kat_obr[2] = 0.0f;

	//w³¹cz ochronê dostêpu do danych 
	//std::lock_guard<std::mutex> lock(getProtokol().m_mutexDanychTelemetrii);		
	std::unique_lock<std::mutex> lock(getProtokol().m_mutexDanychTelemetrii, std::try_to_lock);
	if (lock.owns_lock())
	{
		if (getProtokol().m_vDaneTelemetryczne.size() == 0)
			return FALSE;

		//pocz¹tek wykresu jest indeksem zerowym wektora
		for (uint8_t i = 0; i < 3; i++)
			wekt_we[i] = getProtokol().m_vDaneTelemetryczne[0].dane[TID_MAGNE1X + i];

		Obrot_wektora(wekt_we, wekt_wy, kat_obr);
		ptStart.x = (float)(rOkno.right / 2) + (fSkala * wekt_wy[0]);
		ptStart.y = (float)(rOkno.bottom / 2) - (fSkala * wekt_wy[1]);

		for (size_t n = 1; n < zRozmiar; n++)
		{
			for (uint8_t i = 0; i < 3; i++)
				wekt_we[i] = getProtokol().m_vDaneTelemetryczne[n].dane[TID_MAGNE1X + i];	//zg³asza wyj¹tek: naruszenie zasad dostêpu - po dodaniu mutexa chyba ju¿ nie
			Obrot_wektora(wekt_we, wekt_wy, kat_obr);

			ptStop.x = (float)(rOkno.right / 2) + (fSkala * wekt_wy[0]);
			ptStop.y = (float)(rOkno.bottom / 2) - (fSkala * wekt_wy[1]);

			CComPtr<ID2D1SolidColorBrush> pBrush;
			fDlugoscWektoraMagn = getProtokol().m_vDaneTelemetryczne[n].dane[TID_DLUG_VMAG1];
			D2D1_COLOR_F color = UstawKolor(fDlugoscWektoraMagn);

			pRT->CreateSolidColorBrush(color, &pBrush);
			pRT->DrawLine(ptStart, ptStop, pBrush, 2.0);
			ptStart.x = ptStop.x;
			ptStart.y = ptStop.y;
		}
	}
    return TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Funkcja zwraca kolor do rysowania odcinka linii na sferze w zale¿nosci od d³ugoœci wektora magnetycznego
// Nominalna wartoœæ jest rysowana kolorem zielonym, mniejsza niebieskim a wiêksza czerwonym
// Parametry: 
//  fD³ugoœæWektora - d³ugoœæ wektora magnetycznego w mikroteslach
// zwraca: kolor
///////////////////////////////////////////////////////////////////////////////////////////////////
D2D1_COLOR_F CSfera3DView:: UstawKolor(float fD³ugoœæWektora)
{
	float r, g, b;
	float fDlugoœæWzgledna = (fD³ugoœæWektora  - 0.9f * NOMINALNE_MAGN)/ (1.1f * NOMINALNE_MAGN - 0.9f * NOMINALNE_MAGN);
	if (fDlugoœæWzgledna > 1.0f)
		fDlugoœæWzgledna = 1.0f;
	if (fDlugoœæWzgledna < 0.0f)
		fDlugoœæWzgledna = 0.0f;

	if (fDlugoœæWzgledna < 0.5f)
	{
		float k = fDlugoœæWzgledna * 2.0f;

		r = 0.0f;
		g = k;
		b = 1.0f - k;
	}
	else
	{
		float k = (fDlugoœæWzgledna - 0.5f) * 2.0f;

		r = k;
		g = 1.0f - k;
		b = 0.0f;
	}
	return D2D1::ColorF(r, g, b);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na krêcenie kólkiem myszy, zmienia poiêkszenie rysunku
// Parametry: 
//  nFlags - ?
//  zDelta - wzglêdny obrót kólka myszy
//  pt - wspó³rzêdne myszy
// zwraca: komunkat systemowy
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSfera3DView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (zDelta > 0)
		m_fZoom *= 1.1f;
	else
		m_fZoom /= 1.1f;
	Invalidate(FALSE);

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wywo³uje w¹tek rysuj¹cy odœwie¿ajacy pozycje sfery
// Zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CSfera3DView::WatekInvalidujStanSfery(LPVOID pParam)
{
	return reinterpret_cast<CSfera3DView*>(pParam)->WlasciwyWatekInvalidujStanSfery();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// W¹tek uruchamiaj¹cy odœwiezanie wykresu po przyjœciu telemetrii
// Zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CSfera3DView::WlasciwyWatekInvalidujStanSfery()
{
	while (!m_bKoniecWatkuOdswiezaniaTelemtrii)
	{
		if (m_bOknoGotowe)
		{
			uint32_t nErr = WaitForSingleObject(getProtokol().m_hZdarzenieRamkaTelemetriiGotowa, INFINITE);
			if (nErr != WAIT_TIMEOUT)
			{
				Invalidate();
			}
		}
	}
	return ERR_OK;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// obraca wektor 3D o podane k¹ty wokó³ 3 osi na podstawie : https://pl.wikipedia.org/wiki/Macierz_obrotu
//wekt_we[3] - wektor wejœciowy
//wekt_wy[3] - wektor wyjœciowy
//kat_obr[3] - k¹ty obrotu [rad]
///////////////////////////////////////////////////////////////////////////////////////////////////
void CSfera3DView::Obrot_wektora(float *wekt_we, float *wekt_wy, float *kat_obr)
{
	uint8_t n;
	float temp1[3], temp2[3];

	//obrót o k¹t odchylenia
	if (kat_obr[2] != 0.0f)
	{
		temp1[0] = cosf(kat_obr[2]) * wekt_we[0] - sinf(kat_obr[2]) * wekt_we[1];
		temp1[1] = sinf(kat_obr[2]) * wekt_we[0] + cosf(kat_obr[2]) * wekt_we[1];
		temp1[2] = wekt_we[2];
	}
	else
	{
		for (n = 0; n < 3; n++)
			temp1[n] = wekt_we[n];
	}

	//obrót o k¹t pochylenia
	if (kat_obr[1] != 0.0f)
	{
		temp2[0] = cosf(kat_obr[1]) * temp1[0] + sinf(kat_obr[1]) * temp1[2];
		temp2[1] = temp1[1];
		temp2[2] = -sinf(kat_obr[1]) * temp1[0] + cosf(kat_obr[1]) * temp1[2];
	}
	else
	{
		for (n = 0; n < 3; n++)
			temp2[n] = temp1[n];
	}

	//obrót o k¹t przechylenia
	if (kat_obr[0] != 0.0f)
	{
		wekt_wy[0] = temp2[0];
		wekt_wy[1] = cosf(kat_obr[0]) * temp2[1] - sinf(kat_obr[0]) * temp2[2];
		wekt_wy[2] = sinf(kat_obr[0]) * temp2[1] + cosf(kat_obr[0]) * temp2[2];
	}
	else
	{
		for (n = 0; n < 3; n++)
			wekt_wy[n] = temp2[n];
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na wciœniêcie prawego przycisku myszy
// W³¹cza obracanie sfery i zapamietuje pozycjê pocz¹tkow¹
///////////////////////////////////////////////////////////////////////////////////////////////////
void CSfera3DView::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_bObracanieMyszk¹ = TRUE;
	m_ptPocz¹tkowaPozycjaMyszy = point;
	CView::OnRButtonDown(nFlags, point);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zwolnienie prawego przycisku myszy
// Wy³¹cza obracanie sfery
///////////////////////////////////////////////////////////////////////////////////////////////////
void CSfera3DView::OnRButtonUp(UINT nFlags, CPoint point)
{
	m_bObracanieMyszk¹ = FALSE;
	CView::OnRButtonUp(nFlags, point);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na ruch myszy
// Oblicza przesuniêcie myszy jako podstawê obrotu sfery
///////////////////////////////////////////////////////////////////////////////////////////////////
void CSfera3DView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bObracanieMyszk¹)
	{
		m_ptZmianaPozycjiMyszy = point - m_ptPocz¹tkowaPozycjaMyszy;
		Invalidate();
	}
	CView::OnMouseMove(nFlags, point);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Obs³uga klikniêcia chekboxów wyboru magnetometru
/*//////////////////////////////////////////////////////////////////////////////////////////////////
void CSfera3DView::OnCheckboxMag1Clicked()
{
	m_bRysujMag1 = (m_cbMag1.GetCheck() == BST_CHECKED);
	Invalidate();
}

void CSfera3DView::OnCheckboxMag2Clicked()
{
	m_bRysujMag2 = (m_cbMag2.GetCheck() == BST_CHECKED);
	Invalidate();
}

void CSfera3DView::OnCheckboxMag3Clicked()
{
	m_bRysujMag3 = (m_cbMag3.GetCheck() == BST_CHECKED);
	Invalidate();
}*/