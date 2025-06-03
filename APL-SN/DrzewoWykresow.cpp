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
// Reakcja na wywo�anie meny kontekstowego
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void DrzewoWykresow::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: Dodaj tutaj sw�j kod procedury obs�ugi komunikat�w
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
// Obs�uga menu kontekstowego -> Dodaj wsp�lny
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void DrzewoWykresow::OnDodajWspolny()
{
	// TODO: Dodaj tutaj sw�j kod procedury obs�ugi polecenia
	DodajWspolny();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Obs�uga menu kontekstowego -> Dodaj osobny
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void DrzewoWykresow::OnDodajOsobny()
{
	// TODO: Dodaj tutaj sw�j kod procedury obs�ugi polecenia
	DodajOsobny();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Obs�uga menu kontekstowego -> Usu� wykres
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
			break;	//wyjd� i nie indeksuj dalej zmiennej bo zmniejszy� si� jej rozmiar i ostatni element nie jest dost�pny
		}
	}
}


//Gdy nie ma ju� wykres�w w drzewie to wy��cz polecenie
void DrzewoWykresow::OnUpdateUsunWykres(CCmdUI* pCmdUI)
{
	// TODO: Dodaj tutaj sw�j kod procedury obs�ugi polecenia uaktualnienia UI
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
	strNazwaWykresu.Format(_T("Wsp�lna skala %d"), n);
	stWykresow.hGalazWykresow = InsertItem(strNazwaWykresu, 2, 2, m_hGlownyWezel);
	stWykresow.chTypWykresu = WYKRES_WSPOLNA_SKALA;
	vGrupaWykresow.push_back(stWykresow);
	Expand(m_hGlownyWezel, TVE_EXPAND);		//rozwi� ga��zie w g��wnym w�le drzewa
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
	Expand(m_hGlownyWezel, TVE_EXPAND);		//rozwi� ga��zie w g��wnym w�le drzewa
	Invalidate();
	return 0;
}



void DrzewoWykresow::OnTvnBegindrag(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Dodaj tutaj sw�j kod procedury obs�ugi powiadamiania kontrolki
	*pResult = 0;
}



void DrzewoWykresow::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Dodaj tutaj sw�j kod procedury obs�ugi komunikat�w i/lub wywo�aj domy�lny
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
