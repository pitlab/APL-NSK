#pragma once
#define WYKRES_WSPOLNA_SKALA	1
#define WYKRES_OSOBNA_SKALA		2

#define ZRODLO_TELEMETRIA		0

class DrzewoWykresow :  public CTreeCtrl
{
public:
	DrzewoWykresow();
	~DrzewoWykresow();
	HTREEITEM m_hGlownyWezel;	//g³ówny pieñ drzewa
	struct stZmienna_t
	{
		uint16_t sIdZmiennej = 0;
		uint8_t chZrodloZmiennej = 0;	//0 = telemetria lub jeden z logów 1..n
		CString strNazwa = _T("");
		float fMin = 0.0f;;
		float fMax = 0.0f;	
		D2D1::ColorF cKolorD2D1 = D2D1::ColorF::Blue;
		HTREEITEM hWykres = 0;
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
	stZmienna_t m_stZmiennaNowegoWykresu;
	void DodajWspolny();
	void DodajOsobny();	
	void UstawDaneNowegoWykresu(stZmienna_t stZmienna) { m_stZmiennaNowegoWykresu = stZmienna; }
	void UstawKolorWykresu(COLORREF cKolor);
	D2D1::ColorF KonwertujKolor(COLORREF cKolor);
	COLORREF ZmienKolorFNaCOLORREF(D2D1::ColorF cKolor);
	BOOL ZnajdzWykres(HTREEITEM hWykres, int* nGrupa, int* nWykres);

private:
	D2D1::ColorF m_cKolorD2D1;
};

