#pragma once
#define WYKRES_WSPOLNA_SKALA	1
#define WYKRES_OSOBNA_SKALA		2

class DrzewoWykresow :  public CTreeCtrl
{
public:
	HTREEITEM m_hGlownyWezel;	//g³ówny pieñ drzewa
	struct stZmienna_t
	{
		uint8_t chIdZmiennej;
		CString strNazwa;
		float fMin;
		float fMax;		
	};

	struct stGrupaWykresow_t	
	{
		HTREEITEM hGalazWykresow = 0;
		std::vector<stZmienna_t> vZmienne;
		uint8_t chTypWykresu;
	};

	static std::vector<stGrupaWykresow_t> vGrupaWykresow;

private:	
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
	//CString m_sNazwaNowegoWykresu;
	stZmienna_t m_stZmiennaNowegoWykresu;
	//void UstawNazweNowegoWykresu(CString strNazwa) { m_sNazwaNowegoWykresu = strNazwa; }
	void UstawDaneNowegoWykresu(stZmienna_t stZmienna) { m_stZmiennaNowegoWykresu = stZmienna; }
};

