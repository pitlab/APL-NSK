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
	// TODO: Dodaj tutaj swój kod procedury obs³ugi polecenia
	DodajWspolny();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Obs³uga menu kontekstowego -> Dodaj osobny
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void DrzewoWykresow::OnDodajOsobny()
{
	// TODO: Dodaj tutaj swój kod procedury obs³ugi polecenia
	DodajOsobny();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Obs³uga menu kontekstowego -> Usuñ wykres
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void DrzewoWykresow::OnUsunWykres()
{
	HTREEITEM hGalaz = GetSelectedItem();
	int nLiczbaWykresow = (int)vGrupaWykresow.size();
	for (int n = 0; n < nLiczbaWykresow; n++)
	{
		if (vGrupaWykresow[n].hGalazWykresow == hGalaz)
		{
			DeleteItem(vGrupaWykresow[n].hGalazWykresow);
			vGrupaWykresow.erase(vGrupaWykresow.begin()+n);
			break;	//wyjdŸ i nie indeksuj dalej zmiennej bo zmniejszy³ siê jej rozmiar i ostatni element nie jest dostêpny
		}
	}
}


//Gdy nie ma ju¿ wykresów w drzewie to wy³¹cz polecenie
void DrzewoWykresow::OnUpdateUsunWykres(CCmdUI* pCmdUI)
{
	// TODO: Dodaj tutaj swój kod procedury obs³ugi polecenia uaktualnienia UI
	if (vGrupaWykresow.empty())
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


int DrzewoWykresow::DodajWspolny()
{
	stGrupaWykresow_t stWykresow;

	int n = 1 + (int)vGrupaWykresow.size();
	CString strNazwaWykresu;
	strNazwaWykresu.Format(_T("Wspólna skala %d"), n);
	stWykresow.hGalazWykresow = InsertItem(strNazwaWykresu, 2, 2, m_hGlownyWezel);
	stWykresow.chTypWykresu = WYKRES_WSPOLNA_SKALA;
	vGrupaWykresow.push_back(stWykresow);
	Expand(m_hGlownyWezel, TVE_EXPAND);		//rozwiñ ga³êzie w g³ównym wêŸle drzewa
	//Invalidate();
	return 0;
}


int DrzewoWykresow::DodajOsobny()
{
	stGrupaWykresow_t stWykresow;

	int n = 1 + (int)vGrupaWykresow.size();
	CString strNazwaWykresu;
	strNazwaWykresu.Format(_T("Osobne skale %d"), n);
	stWykresow.hGalazWykresow = InsertItem(strNazwaWykresu, 2, 2, m_hGlownyWezel);
	stWykresow.chTypWykresu = WYKRES_OSOBNA_SKALA;
	vGrupaWykresow.push_back(stWykresow);
	Expand(m_hGlownyWezel, TVE_EXPAND);		//rozwiñ ga³êzie w g³ównym wêŸle drzewa
	Invalidate();
	return 0;
}



void DrzewoWykresow::OnTvnBegindrag(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Dodaj tutaj swój kod procedury obs³ugi powiadamiania kontrolki
	*pResult = 0;
}



void DrzewoWykresow::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Dodaj tutaj swój kod procedury obs³ugi komunikatów i/lub wywo³aj domyœlny
	HTREEITEM item = HitTest(point);
	stZmienna_t stZmienna;

	int nRozmiar = (int)vGrupaWykresow.size();
	for (int n = 0; n < nRozmiar; n++)
	{
		if (vGrupaWykresow[n].hGalazWykresow == item)
		{
			stZmienna.hZmiennej = InsertItem(m_stZmiennaNowegoWykresu.strNazwa, 2, 2, item);
			stZmienna.chIdZmiennej = m_stZmiennaNowegoWykresu.chIdZmiennej;
			vGrupaWykresow[n].vZmienne.push_back(stZmienna);			
			Expand(vGrupaWykresow[n].hGalazWykresow, TVE_EXPAND);
			Invalidate();
			UpdateWindow();
		}
	}
	CTreeCtrl::OnLButtonUp(nFlags, point);
}
