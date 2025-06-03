#pragma once
class DrzewoWykresow :  public CTreeCtrl
{
public:
	HTREEITEM m_hGlownyWezel;	//g³ówny pieñ drzewa
	struct stZmienna_t
	{
		HTREEITEM hZmiennej;
		uint8_t chIdZmiennej;
	};

	struct stGrupaWykresow_t	
	{
		HTREEITEM hGalazWykresow = 0;
		std::vector<stZmienna_t> vZmienne;
	};

private:
	std::vector<stGrupaWykresow_t> vGrupaWykresow;
	DECLARE_MESSAGE_MAP()
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnDodajWspolny();
	afx_msg void OnDodajOsobny();
	afx_msg void OnUsunWykres();
	afx_msg void OnUpdateUsunWykres(CCmdUI* pCmdUI);
	afx_msg void OnTvnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

public:
	int DodajWspolny();
	int DodajOsobny();
	CString m_sNazwaNowegoWykresu;
	void UstawNazweNowegoWykresu(CString strNazwa) { m_sNazwaNowegoWykresu = strNazwa; }
};

