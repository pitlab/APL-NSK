#include "pch.h"
#include "MapaWysokosciowaView.h"
#include "MapaWysokosciowaDoc.h"

IMPLEMENT_DYNCREATE(CMapaWysokosciowaView, CView)

BEGIN_MESSAGE_MAP(CMapaWysokosciowaView, CView)
    ON_REGISTERED_MESSAGE(AFX_WM_DRAW2D, &CMapaWysokosciowaView::OnDraw2d)
END_MESSAGE_MAP()


CMapaWysokosciowaView::CMapaWysokosciowaView()
{
    // Enable D2D support for this window:
    EnableD2DSupport();
}

CMapaWysokosciowaView::~CMapaWysokosciowaView()
{
}

void CMapaWysokosciowaView::OnDraw(CDC* pDC)
{
    
}



LRESULT CMapaWysokosciowaView::OnDraw2d(WPARAM wParam, LPARAM lParam)
{
    CHwndRenderTarget* pRenderTarget =
        reinterpret_cast<CHwndRenderTarget*>(lParam);

    ASSERT_VALID(pRenderTarget);

    if (pRenderTarget == nullptr)
        return FALSE;

    ID2D1RenderTarget* pRT =
        pRenderTarget->GetRenderTarget();

    if (pRT == nullptr)
        return FALSE;

    CRect rc;
    GetClientRect(&rc);

    // T³o
    pRT->Clear(D2D1::ColorF(D2D1::ColorF::DarkSlateGray));

    // Przyk³adowy prostok¹t
    CComPtr<ID2D1SolidColorBrush> brush;

    HRESULT hr = pRT->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::Orange),
        &brush);

    if (SUCCEEDED(hr))
    {
        pRT->DrawRectangle(
            D2D1::RectF(
                50.0f, 50.0f,
                250.0f, 200.0f),
            brush,
            3.0f);
    }

    return TRUE;
}