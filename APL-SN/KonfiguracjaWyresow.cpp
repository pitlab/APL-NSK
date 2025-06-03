// KonfiguracjaWyresow.cpp: plik implementacji
// Fajny przykład drag&drop jest tutaj: https://www.codeproject.com/Articles/886711/Drag-Drop-Images-and-Drop-Descriptions-for-MFC-App

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
	if (m_cDrzewoWykresow)
		m_cDrzewoWykresow.DeleteAllItems();
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
//obsługa komunikatu
	ON_WM_DROPFILES()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTA_DANYCH, &KonfiguracjaWyresow::OnLvnItemchangedListaDanych)
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

	//CImageList* pImageList = m_cDrzewoWykresow.CreateDragImage(hWykres);
	//m_bKursorPrzeciaganie = TRUE;
	//delete pImageList;
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
	int nIndeksZmiennej = m_cListaDanych.GetSelectionMark();
	if (nIndeksZmiennej > 0)
	{
		CString strNazwa = m_cListaDanych.GetItemText(nIndeksZmiennej, 0);
		m_cDrzewoWykresow.UstawNazweNowegoWykresu(strNazwa);
	}
	m_bKursorPrzeciaganie = TRUE;
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
	int nRozmiarZmiennej;
	int nLiczbaDanychTele;
	int nLicznikZmiennych = 0;
	
	m_cListaDanych.InsertColumn(0, _T("Nazwa zmiennej"), LVCFMT_CENTER, 180);
	m_cListaDanych.InsertColumn(1, _T("Liczba pomiarów"), LVCFMT_CENTER, 90);
	
	//czy są jakieś dane?
	nLiczbaDanychTele = (int)getProtokol().m_vDaneTelemetryczne.size();
	if (nLiczbaDanychTele)
	{
		for (int n = 0; n < LICZBA_ZMIENNYCH_TELEMETRYCZNYCH; n++)
		{
			//policz niezerowe dane dla tej zmiennej
			nRozmiarZmiennej = 0;
			for (int x = 0; x < nLiczbaDanychTele; x++)
			{
				if (getProtokol().m_vDaneTelemetryczne[x].dane[n])
					nRozmiarZmiennej++;
			}
			
			//na listę wstaw tylko istniejace dane
			if (nRozmiarZmiennej)
			{
				m_cListaDanych.InsertItem(nLicznikZmiennych, getKomunikacja().m_strNazwyZmiennychTele[n]);
				strNapis.Format(_T("%d"), nRozmiarZmiennej);
				m_cListaDanych.SetItemText(nLicznikZmiennych, 1, strNapis);
				nLicznikZmiennych++;
			}
		}
	}
	
	m_cDrzewoWykresow.m_hGlownyWezel = m_cDrzewoWykresow.InsertItem(_T("Okno wykresów"), 1, 1, TVI_ROOT, TVI_FIRST);
	m_cDrzewoWykresow.SetItemState(m_cDrzewoWykresow.m_hGlownyWezel, TVIS_BOLD, TVIS_BOLD);	//pogrub 
	
	//wstaw dwie pierwsze gałęzie wykresów
	m_cDrzewoWykresow.DodajWspolny();
	m_cDrzewoWykresow.DodajOsobny();
	m_cDrzewoWykresow.Expand(m_cDrzewoWykresow.m_hGlownyWezel, TVE_EXPAND);		//rozwiń gałęzie w głównym węźle drzewa

	m_DropTarget.Register(this);
	//m_cDrzewoWykresow.DragAcceptFiles();	//drzewo akceptuje przeciagane do niego wykresy

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



void KonfiguracjaWyresow::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Dodaj tutaj swój kod procedury obsługi komunikatów i/lub wywołaj domyślny
	if (m_bPrzeciaganieMysza)
	{
		m_cpPozycjaMyszy = point;
		Invalidate();
		UpdateWindow();
	}
	CDialogEx::OnMouseMove(nFlags, point);
}



void KonfiguracjaWyresow::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Dodaj tutaj swój kod procedury obsługi komunikatów i/lub wywołaj domyślny
	m_bPrzeciaganieMysza = TRUE;
	//SetCapture();

	CDialogEx::OnLButtonDown(nFlags, point);
}



void KonfiguracjaWyresow::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Dodaj tutaj swój kod procedury obsługi komunikatów i/lub wywołaj domyślny
	m_bPrzeciaganieMysza = FALSE;
	//ReleaseCapture();
	m_bKursorPrzeciaganie = FALSE;
	CDialogEx::OnLButtonUp(nFlags, point);
}



//przesłania zmianę kursora
BOOL KonfiguracjaWyresow::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Dodaj tutaj swój kod procedury obsługi komunikatów i/lub wywołaj domyślny
	if (m_bKursorPrzeciaganie)
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
		return TRUE;
	}
	else
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		return TRUE;
	}


	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}



void KonfiguracjaWyresow::OnLvnItemchangedListaDanych(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
	*pResult = 0;
}
