#pragma once


#define MAX_ZMIENNOSC	0.005
#define M_PI 	3.14159265358979323846


#include <afxwin.h>
class CBryla3DView :    public CView
{
    DECLARE_DYNCREATE(CBryla3DView)

protected:
    CBryla3DView();
    virtual ~CBryla3DView();
    DECLARE_MESSAGE_MAP()
    double m_dZoom = 0.5;
    CD2DSolidColorBrush* m_pBrushBlack;
    CD2DSolidColorBrush* m_pBrushGray;
    CD2DTextFormat* m_pTextFormat;
    CD2DTextFormat* m_pTextFormatSkalaY;
    CD2DLinearGradientBrush* m_pLinearGradientBrush;

public:
    //CBryla3DDoc* GetDocument() const;
    virtual void OnDraw(CDC* pDC);

protected:
    afx_msg LRESULT OnDraw2d(WPARAM wParam, LPARAM lParam);
    


public:
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};


