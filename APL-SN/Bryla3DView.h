#pragma once
#include <afxwin.h>
class CBryla3DView :    public CView
{
    DECLARE_DYNCREATE(CBryla3DView)

protected:
    CBryla3DView();
    
    virtual ~CBryla3DView();

public:
    virtual void OnDraw(CDC* pDC);

protected:
    DECLARE_MESSAGE_MAP()
};


