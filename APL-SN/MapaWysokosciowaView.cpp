#include "pch.h"
#include "MapaWysokosciowaView.h"
#include "MapaWysokosciowaDoc.h"

IMPLEMENT_DYNCREATE(CMapaWysokosciowaView, CView)

BEGIN_MESSAGE_MAP(CMapaWysokosciowaView, CView)
END_MESSAGE_MAP()


CMapaWysokosciowaView::CMapaWysokosciowaView()
{
}

CMapaWysokosciowaView::~CMapaWysokosciowaView()
{
}

void CMapaWysokosciowaView::OnDraw(CDC* pDC)
{
    //CMapaWysokosciowaDoc* pDoc = (CMapaWysokosciowaDoc*)GetDocument();
    CMapaWysokosciowaDoc* pDoc = DYNAMIC_DOWNCAST(CMapaWysokosciowaDoc, GetDocument());

    // Tymczasowo tylko test rysowania
    CRect rect;
    GetClientRect(&rect);

    pDC->FillSolidRect(rect, RGB(30, 30, 40));

    pDC->SetTextColor(RGB(255, 255, 255));
    pDC->SetBkMode(TRANSPARENT);

    pDC->TextOut(30, 30, _T("Dokument mapa wysokoœciowa"));
}
