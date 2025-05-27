// KonfiguracjaWyresow.cpp: plik implementacji
//

#include "pch.h"
#include "APL-SN.h"
#include "KonfiguracjaWyresow.h"
#include "afxdialogex.h"


// Okno dialogowe KonfiguracjaWyresow

IMPLEMENT_DYNAMIC(KonfiguracjaWyresow, CDialogEx)

KonfiguracjaWyresow::KonfiguracjaWyresow(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KONFIG_WYKRESOW, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

KonfiguracjaWyresow::~KonfiguracjaWyresow()
{
}

void KonfiguracjaWyresow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTA_DANYCH, m_cListaDanych);
	DDX_Control(pDX, IDC_TREE_WYKRESOW, m_cDrzewoWykresow);
}


BEGIN_MESSAGE_MAP(KonfiguracjaWyresow, CDialogEx)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_TREE_WYKRESOW, &KonfiguracjaWyresow::OnTvnBegindragTreeWykresow)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LISTA_DANYCH, &KonfiguracjaWyresow::OnLvnBegindragListaDanych)
	ON_BN_CLICKED(IDOK, &KonfiguracjaWyresow::OnBnClickedOk)
	ON_NOTIFY(BCN_DROPDOWN, IDOK, &KonfiguracjaWyresow::OnDropdownIdok)
END_MESSAGE_MAP()


// Procedury obsługi komunikatów KonfiguracjaWyresow

/// <summary>
/// Rozpocząto przeciaganie zmiennej w obrębie drzewa konfiguracji wykresów
/// </summary>
/// <param name="pNMHDR"></param>
/// <param name="pResult"></param>
void KonfiguracjaWyresow::OnTvnBegindragTreeWykresow(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
	HTREEITEM hWykres = m_cDrzewoWykresow.GetSelectedItem();

	CImageList* pImageList = m_cDrzewoWykresow.CreateDragImage(hWykres);

	delete pImageList;
	*pResult = 0;
}


/// <summary>
/// Rozpocząto przeciaganie zmiennej z listy dostepnych danych
/// </summary>
/// <param name="pNMHDR"></param>
/// <param name="pResult"></param>
void KonfiguracjaWyresow::OnLvnBegindragListaDanych(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
	*pResult = 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Inicjalizacja zawartości okna dialogowego
// Zwraca: TRUE
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL KonfiguracjaWyresow::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString strNapis;
	HTREEITEM hGrupa;
	//HTREEITEM hWykres;

	// TODO:  Dodaj tutaj dodatkową inicjację
	m_cListaDanych.InsertColumn(0, _T("Nazwa zmiennej"), LVCFMT_CENTER, 180);
	m_cListaDanych.InsertColumn(1, _T("Liczba pomiarów"), LVCFMT_CENTER, 90);
	for (int n = 0; n < 5; n++)
	{
		strNapis.Format(_T("Dane %d"), n);
		m_cListaDanych.InsertItem(n, strNapis);
		strNapis.Format(_T("%d"), n*100);
		m_cListaDanych.SetItemText(n, 1, strNapis);
	}
	
	m_cDrzewoWykresow.m_hGlownyWezel = m_cDrzewoWykresow.InsertItem(_T("Okno wykresów"), 1, 1, TVI_ROOT, TVI_FIRST);
	m_cDrzewoWykresow.SetItemState(m_cDrzewoWykresow.m_hGlownyWezel, TVIS_BOLD, TVIS_BOLD);	//pogrub 
	
	//wstaw dwie pierwsze gałęzie wykresów
	m_cDrzewoWykresow.DodajWspolny();
	m_cDrzewoWykresow.DodajOsobny();
	m_cDrzewoWykresow.Expand(m_cDrzewoWykresow.m_hGlownyWezel, TVE_EXPAND);		//rozwiń gałęzie w głównym węźle drzewa

	return TRUE;  // return TRUE unless you set the focus to a control
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Zamknięcie okna przyciskiem OK
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfiguracjaWyresow::OnBnClickedOk()
{
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
	CDialogEx::OnOK();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na upuszczenie w obrębie okna
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfiguracjaWyresow::OnDropdownIdok(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMBCDROPDOWN pDropDown = reinterpret_cast<LPNMBCDROPDOWN>(pNMHDR);
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
	*pResult = 0;
}

