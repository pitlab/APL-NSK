#pragma once
class DrzewoWykresow :  public CTreeCtrl
{
public:
	HTREEITEM m_hGlownyWezel;	//g³ówny pieñ drzewa

	struct stGrupaWykresow_t	
	{
	HTREEITEM hGalazWykresow;
	std::vector<uint8_t> vIdZmiennych;
	};


	std::vector<stGrupaWykresow_t> vGrupaWykresow;
	DECLARE_MESSAGE_MAP()
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnDodajWspolny();
	afx_msg void OnDodajOsobny();
	afx_msg void OnUsunWykres();
	afx_msg void OnUpdateUsunWykres(CCmdUI* pCmdUI);

public:
	int DodajWspolny();
	int DodajOsobny();
	afx_msg void OnDropFiles(HDROP hDropInfo);
};

