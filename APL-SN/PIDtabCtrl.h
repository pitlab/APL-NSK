#pragma once
#include <afxcmn.h>
class PIDtabCtrl :  public CTabCtrl
{
public:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
//    virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

