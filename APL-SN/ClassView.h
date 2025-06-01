
#pragma once

#include "ViewTree.h"
#include "komunikacja/polecenia_komunikacyjne.h"


class CDrzewoTelemetrii : public CViewTree
{
public:
	CDrzewoTelemetrii() noexcept;
	virtual ~CDrzewoTelemetrii();
	int m_nIndeksWrona;	//indeks w roju zaznaczonego wrona 
	struct DrzewoZmiennychTelemetrii
	{
		uint16_t m_sOkresTelemetrii;
		TCHAR tchNazwa[DLUGOSC_NAZWY];
		uint8_t chIkona;
	} stZmienne[LICZBA_ZMIENNYCH_TELEMETRYCZNYCH];
public:

	
	void WstawZmiennaTelemetrii(uint8_t chId, uint16_t sOkres, LPCTSTR lpszItem, int nObrazek, HTREEITEM hParent);
	uint16_t PobierzOkres(uint8_t chId);
	void UstawOkres(uint8_t chId, uint16_t sOkres);
	uint8_t PobierzId(CString strNazwa);
};



class CClassToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CClassView : public CDockablePane
{
public:
	CClassView() noexcept;
	virtual ~CClassView();

	void AdjustLayout();
	void OnChangeVisualStyle();
	int m_nIndeksWrona;	//indeks w roju zaznaczonego wrona 
	
protected:
	CClassToolBar m_wndToolBar;
	CDrzewoTelemetrii m_wndClassView;
	CImageList m_ClassViewImages;
	UINT m_nCurrSort;

	void FillClassView();

private:
	static UINT WatekCzekajNaZmianePolaczenia(LPVOID pParam);
	uint8_t WlasciwyWatekCzekajNaZmianePolaczenia();
	BOOL m_bKoniecWatkuCzekaniaNaZmianePolaczenia;
	CWinThread* pWskWatkuCzekaniaNaPolaczenie;

// Przesłania
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnClassAddMemberFunction();
	afx_msg void OnClassAddMemberVariable();
	afx_msg void OnClassDefinition();
	afx_msg void OnClassProperties();
	afx_msg void OnNewFolder();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKonfigTelemetrii();
	afx_msg void OnKonfigRejestratora();
	afx_msg void OnKonfigWykresow();
};

