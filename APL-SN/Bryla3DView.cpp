#include "pch.h"
#include "Bryla3DView.h"
#include "Bryla3DDoc.h"

IMPLEMENT_DYNCREATE(CBryla3DView, CView)

BEGIN_MESSAGE_MAP(CBryla3DView, CView)
END_MESSAGE_MAP()

CBryla3DView::CBryla3DView()
{
}

CBryla3DView::~CBryla3DView()
{
}

void CBryla3DView::OnDraw(CDC* pDC)
{
    //CBryla3DDoc* pDoc = (CBryla3DDoc*)GetDocument();
    CBryla3DDoc* pDoc = DYNAMIC_DOWNCAST(CBryla3DDoc, GetDocument());

    // Tymczasowo tylko test rysowania
    CRect rect;
    GetClientRect(&rect);

    pDC->FillSolidRect(rect, RGB(30, 30, 40));

    pDC->SetTextColor(RGB(255, 255, 255));
    pDC->SetBkMode(TRANSPARENT);

    pDC->TextOut(30, 30, _T("Dokument Bryla 3D"));
}
