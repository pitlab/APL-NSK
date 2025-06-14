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
	if (m_ObrazkiDrzewa)
		m_ObrazkiDrzewa.DeleteImageList();
	
	if (m_cDrzewoWykresow)
		m_cDrzewoWykresow.DeleteAllItems();
}

void KonfiguracjaWyresow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTA_DANYCH, m_cListaDanych);
	DDX_Control(pDX, IDC_TREE_WYKRESOW, m_cDrzewoWykresow);
	DDX_Control(pDX, IDC_MFCCOLOR, m_ctrlKolor);
}


BEGIN_MESSAGE_MAP(KonfiguracjaWyresow, CDialogEx)
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
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_MFCCOLOR, &KonfiguracjaWyresow::OnBnClickedMfccolor)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_WYKRESOW, &KonfiguracjaWyresow::OnTvnSelchangedTreeWykresow)
	ON_NOTIFY(TVN_SELCHANGING, IDC_TREE_WYKRESOW, &KonfiguracjaWyresow::OnTvnSelchangingTreeWykresow)
	ON_NOTIFY(NM_CLICK, IDC_TREE_WYKRESOW, &KonfiguracjaWyresow::OnNMClickTreeWykresow)
END_MESSAGE_MAP()


// Procedury obsługi komunikatów KonfiguracjaWyresow

/// <summary>
/// Rozpocząto przeciaganie zmiennej z listy dostepnych danych
/// </summary>
/// <param name="pNMHDR"></param>
/// <param name="pResult"></param>
void KonfiguracjaWyresow::OnLvnBegindragListaDanych(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	DrzewoWykresow::stZmienna_t stZmienna;
	UpdateData(TRUE);
	int nIndeksZmiennej = m_cListaDanych.GetSelectionMark();
	stZmienna.strNazwa = m_cListaDanych.GetItemText(nIndeksZmiennej, 0);
	stZmienna.chIdZmiennej = (uint8_t)_ttoi(m_cListaDanych.GetItemText(nIndeksZmiennej, 1));
	stZmienna.fMin = (float)_ttof(m_cListaDanych.GetItemText(nIndeksZmiennej, 3));
	stZmienna.fMax = (float)_ttof(m_cListaDanych.GetItemText(nIndeksZmiennej, 4));
	COLORREF cKolor = m_ctrlKolor.GetColor();
	stZmienna.cKolorD2D1 = (D2D1::ColorF((float)GetRValue(cKolor), (float)GetGValue(cKolor), (float)GetBValue(cKolor), 1.0f));
	
	if ((stZmienna.strNazwa) && (stZmienna.chIdZmiennej < LICZBA_ZMIENNYCH_TELEMETRYCZNYCH))
	{
		m_cDrzewoWykresow.UstawDaneNowegoWykresu(stZmienna);
		m_bKursorPrzeciaganie = TRUE;
	}
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
	int nLiczbaDanych;
	int nLicznikZmiennych = 0;
	float fZmienna, fMin, fMax;
	
	m_cListaDanych.InsertColumn(0, _T("Nazwa zmiennej"), LVCFMT_CENTER, 120);
	m_cListaDanych.InsertColumn(1, _T("ID"), LVCFMT_CENTER, 30);
	m_cListaDanych.InsertColumn(2, _T("Pomiarów"), LVCFMT_CENTER, 60);
	m_cListaDanych.InsertColumn(3, _T("Min"), LVCFMT_CENTER, 50);
	m_cListaDanych.InsertColumn(4, _T("Max"), LVCFMT_CENTER, 50);
	//czy są jakieś dane?
	nLiczbaDanych = (int)getProtokol().m_vDaneTelemetryczne.size();
	if (nLiczbaDanych)
	{
		for (int n = 0; n < LICZBA_ZMIENNYCH_TELEMETRYCZNYCH; n++)
		{
			//policz niezerowe dane dla tej zmiennej
			nRozmiarZmiennej = 0;
			fMin = fMax = 0.0f;
			for (int x = 0; x < nLiczbaDanych; x++)
			{
				fZmienna = getProtokol().m_vDaneTelemetryczne[x].dane[n];
				if (fZmienna)
				{
					nRozmiarZmiennej++;
					if (fZmienna < fMin)
						fMin = fZmienna;
					if (fZmienna > fMax)
						fMax = fZmienna;
				}
			}
			
			//na listę wstaw tylko istniejace dane
			if (nRozmiarZmiennej)
			{
				m_cListaDanych.InsertItem(nLicznikZmiennych, getKomunikacja().m_strNazwyZmiennychTele[n]);	
				strNapis.Format(_T("%d"), n);
				m_cListaDanych.SetItemText(nLicznikZmiennych, 1, strNapis);	//ID
				strNapis.Format(_T("%d"), nRozmiarZmiennej);
				m_cListaDanych.SetItemText(nLicznikZmiennych, 2, strNapis);	//liczba pomiarów
				strNapis.Format(_T("%.3f"), fMin);
				m_cListaDanych.SetItemText(nLicznikZmiennych, 3, strNapis);	//min
				strNapis.Format(_T("%.3f"), fMax);
				m_cListaDanych.SetItemText(nLicznikZmiennych, 4, strNapis);	//max
				nLicznikZmiennych++;
			}
		}
	}
	/*
	//do listy dodaj zmiene z logu
	//AfxGetApp()->GetFirstDocTemplatePosition();
	//(CAPLSNDoc*) pDoc = CAPLSNDoc::CDocument::GetFirstDocPosition();
	POSITION Pos = AfxGetApp()->GetFirstDocTemplatePosition();
	CDocTemplate* pDocTmpl = AfxGetApp()->GetNextDocTemplate(Pos);
	POSITION PosDoc = pDocTmpl->GetFirstDocPosition();
	//https://www.codeproject.com/Tips/5349107/Activate-Document-Window-in-MDI-Application
	if (PosDoc != NULL)
	{
		CAPLSNDoc* pDoc = pDocTmpl->GetNextDoc(PosDoc);
		// CDocument* pDoc = pDocTmpl->GetNextDoc(PosDoc);
		//nLiczbaDanych = (int)GetDocument()->m_vLog.size();
		nLiczbaDanych = (int)pDoc->m_vLog.size();

		for (int n = 0; n < nLiczbaDanych; n++)
		{
			m_cListaDanych.InsertItem(n, pDoc->m_vLog[n].strNazwaZmiennej);
			nRozmiarZmiennej = pDoc->m_vLog[n].vfWartosci.size();
			strNapis.Format(_T("%d"), nRozmiarZmiennej);
			m_cListaDanych.SetItemText(nLicznikZmiennych, 1, strNapis);
		}
	}*/
	
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | TVS_SHOWSELALWAYS;
	m_cDrzewoWykresow.SetExtendedStyle(dwViewStyle, dwViewStyle);

	m_cDrzewoWykresow.m_hGlownyWezel = m_cDrzewoWykresow.InsertItem(_T("Wykresy"), 0, 0, TVI_ROOT, TVI_FIRST);
	m_cDrzewoWykresow.SetItemState(m_cDrzewoWykresow.m_hGlownyWezel, TVIS_BOLD, TVIS_BOLD);	//pogrub 
	CString strNazwaGalezi;
	CString strNazwaWykresu;

	//wstaw do kontrolki obiekty wcześniej zdefinioane w statycznym wektorze
	int nLiczbaGrupWykresow = (int)m_cDrzewoWykresow.vGrupaWykresow.size();
	if (!nLiczbaGrupWykresow)
	{
		//Jezeli nie ma żadnej grupy wykresów to dodaj pierwszą
		DrzewoWykresow::stGrupaWykresow_t stGrupa;
		stGrupa.chTypWykresu = WYKRES_WSPOLNA_SKALA;
		m_cDrzewoWykresow.vGrupaWykresow.push_back(stGrupa);
		nLiczbaGrupWykresow = 1;
	}

	HTREEITEM hDrzewa;
	for (int g = 0; g < nLiczbaGrupWykresow; g++)
	{
		if (m_cDrzewoWykresow.vGrupaWykresow[g].chTypWykresu == WYKRES_WSPOLNA_SKALA)
			strNazwaGalezi.Format(_T("Wspólna skala %d"), g+1);
		else
			strNazwaGalezi.Format(_T("Osobne skale %d"), g+1);

		hDrzewa = m_cDrzewoWykresow.InsertItem(strNazwaGalezi, 1, 1, m_cDrzewoWykresow.m_hGlownyWezel);
		m_cDrzewoWykresow.vGrupaWykresow[g].hGalazWykresow = hDrzewa;

		//jeżeli istnieją wykresy utworzone wcześniej to wstaw nowe uchwyty do nich
		int nLiczbaWykresow = (int)m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne.size();
		for (int w = 0; w < nLiczbaWykresow; w++)
		{
			strNazwaWykresu = m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne[w].strNazwa;
			if (m_cDrzewoWykresow.vGrupaWykresow[g].chTypWykresu == WYKRES_OSOBNA_SKALA)
				hDrzewa = m_cDrzewoWykresow.InsertItem(strNazwaWykresu, 3, 4, m_cDrzewoWykresow.vGrupaWykresow[g].hGalazWykresow);
			else
				hDrzewa = m_cDrzewoWykresow.InsertItem(strNazwaWykresu, 5, 6, m_cDrzewoWykresow.vGrupaWykresow[g].hGalazWykresow);
			m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne[w].hWykres = hDrzewa;
		}
		m_cDrzewoWykresow.Expand(m_cDrzewoWykresow.vGrupaWykresow[g].hGalazWykresow, TVE_EXPAND);
	}
	m_cDrzewoWykresow.Expand(m_cDrzewoWykresow.m_hGlownyWezel, TVE_EXPAND);		//rozwiń gałęzie w głównym węźle drzewa

	m_DropTarget.Register(this);
	//m_cDrzewoWykresow.DragAcceptFiles();	//drzewo akceptuje przeciagane do niego wykresy
	//CMFCColorPickerCtrl();

	m_ctrlKolor.SetColor(RGB(0, 0, 255));	//ustaw niebieski, bo taki kolor domyślnie mają wykresy
	//m_ctrlKolor.SetType(CMFCColorPickerCtrl::COLORTYPE::HEX_GREYSCALE);	// LUMINANCE, PICKER, HEX and HEX_GREYSCALE.
	//CPalette paleta;
	//paleta.CreateHalftonePalette(this->GetDC());
	/*LOGPALETTE logpalete;
	paleta.CreatePalette(&logpalete);
	m_ctrlKolor.SetPalette(&paleta);
	m_ctrlKolor.SetColor(0x000000FF);*/


	//obsługa obrazków w drzewie
	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;
	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Nie można załadować mapy bitowej: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return FALSE;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;
	m_ObrazkiDrzewa.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_ObrazkiDrzewa.Add(&bmp, RGB(255, 0, 0));
	m_cDrzewoWykresow.SetImageList(&m_ObrazkiDrzewa, TVSIL_NORMAL);
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
	m_bPrzeciaganieMysza = TRUE;
	CDialogEx::OnLButtonDown(nFlags, point);
}



void KonfiguracjaWyresow::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bPrzeciaganieMysza = FALSE;
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



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmianę koloru: ustawia kolor w klasie drzewa skąd umieszczany jest w wektorze konfiguracji wykresów
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfiguracjaWyresow::OnBnClickedMfccolor()
{
	int nGrupa = 0;
	int nWykres = 0;
	COLORREF cKolor = m_ctrlKolor.GetColor();


	/*CMFCColorDialog dlg(RGB(0, 0, 255), 0, this);

	if (dlg.DoModal() == IDOK)
	{
		cKolor = m_ctrlKolor.GetColor();
		//m_colorButton.SetColor(cKolor, cKolor);
	}*/



	m_cDrzewoWykresow.UstawKolorWykresu(cKolor);
	

	HTREEITEM hWykres = m_cDrzewoWykresow.GetSelectedItem();
	if (m_cDrzewoWykresow.ZnajdzWykres(hWykres, &nGrupa, &nWykres))
	{
		m_cDrzewoWykresow.vGrupaWykresow[nGrupa].vZmienne[nWykres].cKolorD2D1 = m_cDrzewoWykresow.KonwertujKolor(cKolor);
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmianę wyboru elementu w drzewie
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfiguracjaWyresow::OnTvnSelchangedTreeWykresow(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	//D2D1::ColorF cKolorD2D1;

	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
	//pNMTreeView->itemNew;

	
	*pResult = 0;
}


void KonfiguracjaWyresow::OnTvnSelchangingTreeWykresow(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki

	*pResult = 0;
}


void KonfiguracjaWyresow::OnNMClickTreeWykresow(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
	COLORREF cKolor;

	HTREEITEM hDrzewa = m_cDrzewoWykresow.GetSelectedItem();
	int nLiczbaGrupWykresow = (int)m_cDrzewoWykresow.vGrupaWykresow.size();
	for (int g = 0; g < nLiczbaGrupWykresow; g++)
	{
		int nLiczbaWykresow = (int)m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne.size();
		for (int w = 0; w < nLiczbaWykresow; w++)
		{
			if (m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne[w].hWykres == hDrzewa)
			{
				D2D1::ColorF cKolorD2D1 = m_cDrzewoWykresow.vGrupaWykresow[g].vZmienne[w].cKolorD2D1;
				cKolor = m_cDrzewoWykresow.ZmienKolorFNaCOLORREF(cKolorD2D1);
				m_ctrlKolor.SetColor(cKolor);
			}
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}
