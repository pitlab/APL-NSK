#include "pch.h"
#include "APL-SN.h"
#include "DrzewoWykresow.h"
BEGIN_MESSAGE_MAP(DrzewoWykresow, CTreeCtrl)
	ON_COMMAND(ID_DODAJ_WSPOLNY, &DrzewoWykresow::OnDodajWspolny)
	ON_COMMAND(ID_DODAJ_OSOBNY, &DrzewoWykresow::OnDodajOsobny)
	ON_COMMAND(ID_USUN_WYKRES, &DrzewoWykresow::OnUsunWykres)
	ON_UPDATE_COMMAND_UI(ID_USUN_WYKRES, &DrzewoWykresow::OnUpdateUsunWykres)
	ON_WM_CONTEXTMENU()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &DrzewoWykresow::OnTvnBegindrag)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



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


void DrzewoWykresow::OnDodajWspolny()
{
	// TODO: Dodaj tutaj sw�j kod procedury obs�ugi polecenia
	DodajWspolny();
}


void DrzewoWykresow::OnDodajOsobny()
{
	// TODO: Dodaj tutaj sw�j kod procedury obs�ugi polecenia
	DodajOsobny();
}


void DrzewoWykresow::OnUsunWykres()
{
	// TODO: Dodaj tutaj sw�j kod procedury obs�ugi polecenia
	//trzeba znale�� spos�b aby okresli� kt�ry element jest wskazany i ma by� usuni�ty. Na razie zak�adam �e pierwszy
	DeleteItem(vGrupaWykresow[0].hGalazWykresow);
	vGrupaWykresow.erase(vGrupaWykresow.begin());
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
	vGrupaWykresow.push_back(stWykresow);
	return 0;
}


int DrzewoWykresow::DodajOsobny()
{
	stGrupaWykresow_t stWykresow;

	int n = 1 + (int)vGrupaWykresow.size();
	CString strNazwaWykresu;
	strNazwaWykresu.Format(_T("Osobne skale %d"), n);
	stWykresow.hGalazWykresow = InsertItem(strNazwaWykresu, 2, 2, m_hGlownyWezel);
	vGrupaWykresow.push_back(stWykresow);
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
	CString strNazwaZmiennej = _T("abc");

	int nRozmiar = (int)vGrupaWykresow.size();
	for (int n = 0; n < nRozmiar; n++)
	{
		if (vGrupaWykresow[n].hGalazWykresow == item)
		{
			stZmienna.hZmiennej = InsertItem(strNazwaZmiennej, 2, 2, item);
			vGrupaWykresow[n].vZmienne.push_back(stZmienna);			
			Expand(vGrupaWykresow[n].hGalazWykresow, TVE_EXPAND);
			Invalidate();
			UpdateWindow();
		}
	}
	CTreeCtrl::OnLButtonUp(nFlags, point);
}
