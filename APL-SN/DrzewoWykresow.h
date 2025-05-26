#pragma once
class DrzewoWykresow :  public CTreeCtrl
{
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
};

