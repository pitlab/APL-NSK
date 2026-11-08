#pragma once
#include <afxwin.h>
#include "MapaWysokosciowaDoc.h"
#include "KodyBledow.h"

class CMapaWysokosciowaView :    public CView
{
    DECLARE_DYNCREATE(CMapaWysokosciowaView)

protected:
    CMapaWysokosciowaView();
    virtual ~CMapaWysokosciowaView();
    DECLARE_MESSAGE_MAP()

public:
    virtual void OnDraw(CDC* pDC);
    void RysujMapeNumeryczn¹RGB512(CMapaWysokosciowaDoc::stNumerycznyModelTerenu_t* stNMT, CHwndRenderTarget* pRenderTarget, CD2DSolidColorBrush* pBrush);

protected:
    afx_msg LRESULT OnDraw2d(WPARAM wParam, LPARAM lParam);
    float m_fZoomPoziomo;
    CD2DSolidColorBrush* m_pBrushMapyNMT;
    CD2DTextFormat* m_pTextFormat;
    CD2DLinearGradientBrush* m_pLinearGradientBrush;

public:
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

