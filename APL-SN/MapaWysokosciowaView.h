#pragma once
#include <afxwin.h>
class CMapaWysokosciowaView :    public CView
{
    DECLARE_DYNCREATE(CMapaWysokosciowaView)

protected:
    CMapaWysokosciowaView();
    virtual ~CMapaWysokosciowaView();
    DECLARE_MESSAGE_MAP()

public:
    virtual void OnDraw(CDC* pDC);
  

protected:
    afx_msg LRESULT OnDraw2d(WPARAM wParam, LPARAM lParam);

};

