#include "pch.h"
#include "APL-SN.h"
#include "DrzewoWykresow.h"


std::vector<DrzewoWykresow::stGrupaWykresow_t> DrzewoWykresow::vGrupaWykresow;



BEGIN_MESSAGE_MAP(DrzewoWykresow, CTreeCtrl)
	ON_COMMAND(ID_DODAJ_WSPOLNY, &DrzewoWykresow::OnDodajWspolny)
	ON_COMMAND(ID_DODAJ_OSOBNY, &DrzewoWykresow::OnDodajOsobny)
	ON_COMMAND(ID_USUN_WYKRES, &DrzewoWykresow::OnUsunWykres)
	ON_UPDATE_COMMAND_UI(ID_USUN_WYKRES, &DrzewoWykresow::OnUpdateUsunWykres)
	ON_WM_CONTEXTMENU()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &DrzewoWykresow::OnTvnBegindrag)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


DrzewoWykresow::DrzewoWykresow()
: m_cKolorD2D1(D2D1::ColorF::Blue)
{

}

DrzewoWykresow::~DrzewoWykresow()
{

}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na wywo³anie meny kontekstowego
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void DrzewoWykresow::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: Dodaj tutaj swój kod procedury obs³ugi komunikatów
	CMenu menu;
	menu.LoadMenu(IDR_MENU_WYKRESOW);
	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Obs³uga menu kontekstowego -> Dodaj wspólny
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void DrzewoWykresow::OnDodajWspolny()
{
	DodajWspolny();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Obs³uga menu kontekstowego -> Dodaj osobny
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void DrzewoWykresow::OnDodajOsobny()
{
	DodajOsobny();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Obs³uga menu kontekstowego -> Usuñ wykres
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void DrzewoWykresow::OnUsunWykres()
{
	HTREEITEM hDrzewa = GetSelectedItem();
	int nLiczbaGrupWykresow = (int)vGrupaWykresow.size();
	for (int g = 0; g < nLiczbaGrupWykresow; g++)
	{
		if (vGrupaWykresow[g].hGalazWykresow == hDrzewa)
		{
			DeleteItem(vGrupaWykresow[g].hGalazWykresow);
			vGrupaWykresow.erase(vGrupaWykresow.begin()+g);
			break;	//wyjdŸ i nie indeksuj dalej zmiennej bo zmniejszy³ siê jej rozmiar i ostatni element nie jest dostêpny
		}

		//usuwanie wykresów
		int nLiczbaWykresow = (int)vGrupaWykresow[g].vZmienne.size();
		for (int w = 0; w < nLiczbaWykresow; w++)
		{
			if (vGrupaWykresow[g].vZmienne[w].hWykres == hDrzewa)
			{
				DeleteItem(vGrupaWykresow[g].vZmienne[w].hWykres);
				vGrupaWykresow[g].vZmienne.erase(vGrupaWykresow[g].vZmienne.begin() + w);
				break;	//wyjdŸ i nie indeksuj dalej
			}
		}
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Gdy nie ma ju¿ wykresów w drzewie to wy³¹cz polecenie w menu kontekstowym
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void DrzewoWykresow::OnUpdateUsunWykres(CCmdUI* pCmdUI)
{
	// TODO: Dodaj tutaj swój kod procedury obs³ugi polecenia uaktualnienia UI
	if (vGrupaWykresow.empty())
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Dodaje do drzewa ga³¹Ÿ wykresu o espólnej skali dla ka¿dego wykresu
// Parametry: 
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void DrzewoWykresow::DodajWspolny()
{
	stGrupaWykresow_t stWykresow;

	int n = 1 + (int)vGrupaWykresow.size();
	CString strNazwaWykresu;
	strNazwaWykresu.Format(_T("Wspólna skala %d"), n);
	stWykresow.hGalazWykresow = InsertItem(strNazwaWykresu, 1, 1, m_hGlownyWezel);
	stWykresow.chTypWykresu = WYKRES_WSPOLNA_SKALA;
	vGrupaWykresow.push_back(stWykresow);
	Expand(m_hGlownyWezel, TVE_EXPAND);		//rozwiñ ga³êzie w g³ównym wêŸle drzewa
}	



///////////////////////////////////////////////////////////////////////////////////////////////////
// Dodaje do drzewa ga³¹Ÿ wykresu o osobnej skali dla ka¿dego wykresu
// Parametry: 
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void DrzewoWykresow::DodajOsobny()
{
	stGrupaWykresow_t stWykresow;

	int n = 1 + (int)vGrupaWykresow.size();
	CString strNazwaWykresu;
	strNazwaWykresu.Format(_T("Osobne skale %d"), n);
	stWykresow.hGalazWykresow = InsertItem(strNazwaWykresu, 1, 1, m_hGlownyWezel);
	stWykresow.chTypWykresu = WYKRES_OSOBNA_SKALA;
	vGrupaWykresow.push_back(stWykresow);
	Expand(m_hGlownyWezel, TVE_EXPAND);		//rozwiñ ga³êzie w g³ównym wêŸle drzewa
	Invalidate();
}



void DrzewoWykresow::OnTvnBegindrag(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Dodaj tutaj swój kod procedury obs³ugi powiadamiania kontrolki
	*pResult = 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Upuszczenie przeciaganej zmiennej do ga³êzi drzewa
// Parametry:  point - punkt nad którym znajduje siê mysz
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void DrzewoWykresow::OnLButtonUp(UINT nFlags, CPoint point)
{
	HTREEITEM hGalaz = HitTest(point);
	HTREEITEM hWykres;
	stZmienna_t stZmienna;

	int nRozmiar = (int)vGrupaWykresow.size();
	for (int n = 0; n < nRozmiar; n++)
	{
		if (vGrupaWykresow[n].hGalazWykresow == hGalaz)
		{
			if (vGrupaWykresow[n].chTypWykresu == WYKRES_OSOBNA_SKALA)
				hWykres = InsertItem(m_stZmiennaNowegoWykresu.strNazwa, 3, 4, hGalaz);
			else
				hWykres = InsertItem(m_stZmiennaNowegoWykresu.strNazwa, 5, 6, hGalaz);
			stZmienna.hWykres = hWykres;
			stZmienna.chIdZmiennej = m_stZmiennaNowegoWykresu.chIdZmiennej;
			stZmienna.strNazwa = m_stZmiennaNowegoWykresu.strNazwa;
			stZmienna.cKolorD2D1 = m_cKolorD2D1;
			
			vGrupaWykresow[n].vZmienne.push_back(stZmienna);
			Expand(vGrupaWykresow[n].hGalazWykresow, TVE_EXPAND);
			Invalidate();
			UpdateWindow();
		}
	}
	CTreeCtrl::OnLButtonUp(nFlags, point);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Na podstawie identyfikatora ga³êzi identyfikuje grupê wykresów i numer wykresu
// Parametry:  
// [we] hWykres - identyfikator elementu drzewa
// [wy] *nGrupa - wskaŸnik na znalezion¹ grupê wykresów
// [wy] *nWykres - wskaŸnika na znaleziony wykres
// zwraca: TRUE: znaleziono, FALSE: nie znaleziono
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL DrzewoWykresow::ZnajdzWykres(HTREEITEM hWykres, int* nGrupa, int* nWykres)
{
	//wyglada na to ¿e drzewo zbudowane poprzednim razem ma inne identyfikatory ga³êzi
	//wiêc to co jest zapisane w statycznym wektorze jest nieaktualne dla nowej instancji okna

	int nLiczbaGrupWykresow = (int)vGrupaWykresow.size();
	for (int g = 0; g < nLiczbaGrupWykresow; g++)
	{
		int nLiczbaWykresow = (int)vGrupaWykresow[g].vZmienne.size();
		for (int w = 0; w < nLiczbaWykresow; w++)
		{
			if (hWykres == vGrupaWykresow[g].vZmienne[w].hWykres)
			{
				*nGrupa = g;
				*nWykres = w;
				return TRUE;
			}
		}
	}
	return FALSE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Ustawia wewnêtrzn¹ zmienn¹ koloru wykresu. 
// Parametry:  cKolor - kolor wejœciowy
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void DrzewoWykresow::UstawKolorWykresu(COLORREF cKolor)
{
	m_cKolorD2D1 = KonwertujKolor(cKolor);		
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Robi konwersje z COLORREF na D2D1::ColorF
// Parametry:  cKolor - kolor wejœciowy COLORREF
// zwraca: kolor wejœciowy D2D1::ColorF
///////////////////////////////////////////////////////////////////////////////////////////////////
D2D1::ColorF DrzewoWykresow::KonwertujKolor(COLORREF cKolor)
{
	return D2D1::ColorF((float)GetRValue(cKolor), (float)GetGValue(cKolor), (float)GetBValue(cKolor), 1.0f);
}