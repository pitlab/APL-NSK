#include "pch.h"
#include "MapaWysokosciowaView.h"
#include "MapaWysokosciowaDoc.h"

IMPLEMENT_DYNCREATE(CMapaWysokosciowaView, CView)

BEGIN_MESSAGE_MAP(CMapaWysokosciowaView, CView)
    ON_REGISTERED_MESSAGE(AFX_WM_DRAW2D, &CMapaWysokosciowaView::OnDraw2d)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


CMapaWysokosciowaView::CMapaWysokosciowaView()
{
    // Enable D2D support for this window:
    EnableD2DSupport();
	m_pTextFormat = new CD2DTextFormat(GetRenderTarget(), _T("Verdana"), 14);
	m_pBrushMapyNMT = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::Gray));

	D2D1_GRADIENT_STOP gradientStopsT³a[2] = {
	{0.0f, D2D1::ColorF(D2D1::ColorF::White)},
	{1.0f, D2D1::ColorF(D2D1::ColorF::Cornsilk)} };
	m_pLinearGradientBrush = new CD2DLinearGradientBrush(GetRenderTarget(), gradientStopsT³a, ARRAYSIZE(gradientStopsT³a), D2D1::LinearGradientBrushProperties(D2D1::Point2F(0, 0), D2D1::Point2F(0, 0)));
}

CMapaWysokosciowaView::~CMapaWysokosciowaView()
{
}

void CMapaWysokosciowaView::OnDraw(CDC* pDC)
{
    
}



LRESULT CMapaWysokosciowaView::OnDraw2d(WPARAM wParam, LPARAM lParam)
{
    CHwndRenderTarget* pRenderTarget = reinterpret_cast<CHwndRenderTarget*>(lParam);
    ASSERT_VALID(pRenderTarget);
    if (pRenderTarget == nullptr)
        return FALSE;

	// Rzutowanie wskaŸnika dokumentu
	CMapaWysokosciowaDoc* pDoc = dynamic_cast<CMapaWysokosciowaDoc*>(GetDocument());
	ASSERT_VALID(pDoc);
	//pDoc->SetTitle(_T("Numeryczny Model Terenu"));
	
    ID2D1RenderTarget* pRT = pRenderTarget->GetRenderTarget();
    if (pRT == nullptr)
        return FALSE;

	CRect rOkno, rObiekt;
	GetClientRect(&rOkno);

	pRenderTarget->FillRectangle(rOkno, m_pLinearGradientBrush);
	
	RysujMapeNumeryczn¹RGB512(&pDoc->m_stNMT, pRenderTarget, m_pBrushMapyNMT);
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Rysuje mapê Numerycznego Modelu Terenu. Kolory rozkladaja siê 8 odcinkach po 64 stopnie jasnoœci nastepuj¹co:
// RRRR = 8..5 x 64
//   GGGG = 6..3 x 64
//     BBBB = 4..1 x 64
// Parametry: 
//  *stNMT - wskaŸnik na mapê
//  pRenderTarget - narzêdzie rysuj¹ce
//  pBrush - parametry pêdzla rysujacego osie
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CMapaWysokosciowaView::RysujMapeNumeryczn¹RGB512(CMapaWysokosciowaDoc::stNumerycznyModelTerenu_t* stNMT, CHwndRenderTarget* pRenderTarget, CD2DSolidColorBrush* pBrush)
{
	size_t Indeks;
	int16_t sPodstawaKoloru, sKolorR, sKolorG, sKolorB;
	CComPtr<ID2D1Bitmap> m_spCameraBitmap;
	//size_t bufferSize = ((size_t)stNMT->nKolumn * stNMT->nWierszy * sizeof(float));
	size_t bufferSize = stNMT->vfWysokoœæ.size() * sizeof(float);
	std::vector<uint8_t> bgraBuffer(bufferSize);
	float fSkalaKoloru = (8 * 64) / (stNMT->fWysMax - stNMT->fWysMin);

	//for (int n = 0; n < stNMT->vfWysokoœæ.size(); n++)
	for (int n = 0; n < stNMT->nKolumn * stNMT->nWierszy; n++)
	{
		Indeks = n * sizeof(float);
		if (stNMT->vfWysokoœæ[n] != stNMT->fNodata)
		{
			//rysuj skalê kolorów przechodz¹c¹ z czerwonej przez zielon¹ w niebiesk¹ z czêœci¹ wspóln¹ 128 miedzy s¹siadujacymu kolorami
			sPodstawaKoloru = (int16_t)((stNMT->vfWysokoœæ[n] - stNMT->fWysMin) * fSkalaKoloru);
			if (sPodstawaKoloru > (6 * 64))	//tylko czerwony
			{
				sKolorB = 0;
				sKolorG = 0;
				sKolorR = sPodstawaKoloru - (4 * 64);
			}
			else
				if (sPodstawaKoloru > (4 * 64))	//czerwono-zielony
				{
					sKolorB = 0;
					sKolorG = (6 * 64) - sPodstawaKoloru;
					sKolorR = sPodstawaKoloru - (4 * 64);
				}
				else
					if (sPodstawaKoloru > (2 * 64))	//zielono-niebieski
					{
						sKolorR = 0;
						sKolorB = (4 * 64) - sPodstawaKoloru;
						sKolorG = sPodstawaKoloru - (6 * 64);
					}
					else   //tylko niebieski
					{
						sKolorB = (4 * 64) - sPodstawaKoloru;
						sKolorG = 0;
						sKolorR = 0;
					}

			bgraBuffer[Indeks + 0] = (uint8_t)sKolorB;	//B
			bgraBuffer[Indeks + 1] = (uint8_t)sKolorG;	//G
			bgraBuffer[Indeks + 2] = (uint8_t)sKolorR;	//R
		}
		else
		{
			bgraBuffer[Indeks + 0] = 250;	//B
			bgraBuffer[Indeks + 1] = 0;		//G
			bgraBuffer[Indeks + 2] = 250;	//R
		}
		bgraBuffer[Indeks + 3] = 0xFF;			//Alfa		
	}

	D2D1_SIZE_U size = D2D1::SizeU(stNMT->nKolumn, stNMT->nWierszy);
	D2D1_BITMAP_PROPERTIES props = D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE));
	m_spCameraBitmap.Release();
	ID2D1RenderTarget* pRT = GetRenderTarget()->GetRenderTarget();
	HRESULT hr = pRT->CreateBitmap(size, bgraBuffer.data(), stNMT->nKolumn * 4, &props, &m_spCameraBitmap);

	if (m_spCameraBitmap)
		pRT->DrawBitmap(m_spCameraBitmap, D2D1::RectF(0.f, 0.f, (FLOAT)stNMT->nKolumn * m_fZoomPoziomo, (FLOAT)stNMT->nWierszy * m_fZoomPoziomo));
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na krêcenie kólkiem myszy, zmienia powiêkszenie mapy
// Parametry: 
//  nFlags - ?
//  zDelta - wzglêdny obrót kólka myszy
//  pt - wspó³rzêdne myszy
// zwraca: komunkat systemowy
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMapaWysokosciowaView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (zDelta > 0)
		m_fZoomPoziomo *= 1.1;
	else
		m_fZoomPoziomo /= 1.1;
	Invalidate(FALSE);	
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
