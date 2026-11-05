#pragma once
#include <afxwin.h>
class CMapaWysokosciowaView :    public CView
{
    DECLARE_DYNCREATE(CMapaWysokosciowaView)

protected:
    CMapaWysokosciowaView();
    virtual ~CMapaWysokosciowaView();

public:
    virtual void OnDraw(CDC* pDC);

protected:
    DECLARE_MESSAGE_MAP()

};

