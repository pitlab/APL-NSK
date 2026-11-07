#include "pch.h"
#include "Bryla3DView.h"
#include "Bryla3DDoc.h"
#include "sys_def_wspolny.h"

IMPLEMENT_DYNCREATE(CBryla3DView, CView)

BEGIN_MESSAGE_MAP(CBryla3DView, CView)
    ON_REGISTERED_MESSAGE(AFX_WM_DRAW2D, &CBryla3DView::OnDraw2d)
ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

CBryla3DView::CBryla3DView()
{
    // Enable D2D support for this window:
    EnableD2DSupport();

	m_pBrushBlack = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::Black));
	m_pBrushGray = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::Gray));

	m_pTextFormat = new CD2DTextFormat(GetRenderTarget(), _T("Verdana"), 14);
	m_pTextFormatSkalaY = new CD2DTextFormat(GetRenderTarget(), _T("Verdana"), 11);
	m_pTextFormat->Get()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);

	D2D1_GRADIENT_STOP gradientStops[2];
	gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::White);
	gradientStops[0].position = 0.f;
	//gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::AliceBlue);	//ok bardzo delikatne
	//gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::Azure);
	gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::Cornsilk);
	gradientStops[1].position = 1.f;
	m_pLinearGradientBrush = new CD2DLinearGradientBrush(GetRenderTarget(), gradientStops, ARRAYSIZE(gradientStops), D2D1::LinearGradientBrushProperties(D2D1::Point2F(0, 0), D2D1::Point2F(0, 0)));

}

CBryla3DView::~CBryla3DView()
{
}

void CBryla3DView::OnDraw(CDC* pDC)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
// G³ówna funkcja rysujaca bry³ê wykorzystujaca Direct 2D
// Parametry: 
//  wParam - ?
//  lParam - wskaŸnik na CHwndRenderTarget
// zwraca: komunkat systemowy
///////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CBryla3DView::OnDraw2d(WPARAM wParam, LPARAM lParam)
{    
	CRect rectWnd;
	CRect rect;
	GetClientRect(rectWnd);
	CD2DSizeF promien;
	CD2DPointF ptStart, ptStop;
	int nOffsetY = 9 * rectWnd.bottom / 10;
	int n, m;
	double wekt_we[3], wekt_wy[3], kat_obr[3] = { 0,0,0 };
	double dDlugosc;
	double dBladDopasowania2D = 0;	//b³¹d dopasowania danych do równania okrêgu
	CString strText;

    CBryla3DDoc* pDoc = (CBryla3DDoc*)GetDocument();
    ASSERT_VALID(pDoc);

    CHwndRenderTarget* pRenderTarget = reinterpret_cast<CHwndRenderTarget*>(lParam);
    ASSERT_VALID(pRenderTarget);

    if (pRenderTarget == nullptr)
        return FALSE;

    ID2D1RenderTarget* pRT = pRenderTarget->GetRenderTarget();

    if (pRT == nullptr)
        return FALSE;

    CRect rc;
    GetClientRect(&rc);

    // T³o
    pRT->Clear(D2D1::ColorF(D2D1::ColorF::DarkSlateGray));




	//rysuj uk³ad wspó³rzednych
	int rozmiar = rectWnd.bottom;
	if (rectWnd.right < rozmiar) //wykres XY jest symetryczny, wiêc rozmiarem bêdzie mniejszy z rozmiarów okna
		rozmiar = rectWnd.right;

	double skala = rozmiar / (NOMINALNE_MAGN)*m_dZoom;

	//oœ pozioma Y
	ptStart.x = (float)(rectWnd.right - rozmiar) / 2;
	ptStart.y = (float)rectWnd.bottom / 2;
	ptStop.x = (float)(rectWnd.right - rozmiar) / 2 + rozmiar;
	ptStop.y = (float)rectWnd.bottom / 2;
	pRenderTarget->DrawLine(ptStart, ptStop, m_pBrushBlack);
	rect = { (int)ptStop.x - 20, (int)ptStop.y, (int)ptStop.x, (int)ptStop.y + 20 };
	pRenderTarget->DrawText(_T("Y-"), rect, m_pBrushBlack, m_pTextFormat);
	rect = { (int)ptStart.x + 5, (int)ptStart.y, (int)ptStart.x + 30, (int)ptStart.y + 20 };
	pRenderTarget->DrawText(_T("Y+"), rect, m_pBrushBlack, m_pTextFormat);

	//oœ pionowa X
	ptStart.x = (float)(rectWnd.right) / 2;
	ptStart.y = (float)(rectWnd.bottom - rozmiar) / 2;
	ptStop.x = (float)(rectWnd.right / 2);
	ptStop.y = (float)(rectWnd.bottom - rozmiar) / 2 + rozmiar;
	pRenderTarget->DrawLine(ptStart, ptStop, m_pBrushBlack);
	rect = { (int)ptStart.x + 2, (int)ptStart.y + 5, (int)ptStart.x + 30, (int)ptStart.y + 20 };
	pRenderTarget->DrawText(_T("X+"), rect, m_pBrushBlack, m_pTextFormat);
	rect = { (int)ptStop.x + 2, (int)ptStop.y - 20, (int)ptStop.x + 30, (int)ptStop.y };
	pRenderTarget->DrawText(_T("X-"), rect, m_pBrushBlack, m_pTextFormat);


	//okr¹g
	ptStart.x = (float)rectWnd.right / 2;
	ptStart.y = (float)rectWnd.bottom / 2;
	promien.height = promien.width = (float)(skala * NOMINALNE_MAGN * cosf(INKLINACJA_MAG));
	CD2DEllipse okrag = CD2DEllipse(ptStart, promien);
	pRenderTarget->DrawEllipse(okrag, m_pBrushBlack);

	//okr¹g o sta³ym promieniu
	promien.height = promien.width = (float)(rozmiar * 0.4);
	okrag = CD2DEllipse(ptStart, promien);
	pRenderTarget->DrawEllipse(okrag, m_pBrushGray);

	//Tytu³ wykresu
	rect = { rectWnd.right - strText.GetLength() * 8, 20, rectWnd.right, 40 };
	pRenderTarget->DrawText(_T("Wykres biegunowy 3D"), rect, m_pBrushBlack, m_pTextFormat);
    return TRUE;
}




///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na krêcenie kólkiem myszy, zmienia poiêkszenie rysunku
// Parametry: 
//  nFlags - ?
//  zDelta - wzglêdny obrót kólka myszy
//  pt - wspó³rzêdne myszy
// zwraca: komunkat systemowy
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CBryla3DView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (zDelta > 0)
		m_dZoom *= 1.1;
	else
		m_dZoom /= 1.1;
	Invalidate(FALSE);

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
