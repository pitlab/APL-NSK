#include "pch.h"
#include "APL-SN.h"
#include "DrzewoWykresow.h"
BEGIN_MESSAGE_MAP(DrzewoWykresow, CTreeCtrl)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_DODAJ_WSPOLNY, &DrzewoWykresow::OnDodajWspolny)
	ON_COMMAND(ID_DODAJ_OSOBNY, &DrzewoWykresow::OnDodajOsobny)
	ON_COMMAND(ID_USUN_WYKRES, &DrzewoWykresow::OnUsunWykres)
	ON_UPDATE_COMMAND_UI(ID_USUN_WYKRES, &DrzewoWykresow::OnUpdateUsunWykres)
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
	// TODO: Dodaj kod implementacji w tym miejscu.
	stGrupaWykresow_t stWykresow;

	int n = 1 + (int)vGrupaWykresow.size();
	CString strNazwaWykresu;
	strNazwaWykresu.Format(_T("Wykres wspolny %d"), n);
	stWykresow.hGalazWykresow = InsertItem(strNazwaWykresu, 2, 2, m_hGlownyWezel);
	vGrupaWykresow.push_back(stWykresow);
	return 0;
}


int DrzewoWykresow::DodajOsobny()
{
	// TODO: Dodaj kod implementacji w tym miejscu.
	stGrupaWykresow_t stWykresow;

	int n = 1 + (int)vGrupaWykresow.size();
	CString strNazwaWykresu;
	strNazwaWykresu.Format(_T("Wykres ososbny %d"), n);
	stWykresow.hGalazWykresow = InsertItem(strNazwaWykresu, 2, 2, m_hGlownyWezel);
	vGrupaWykresow.push_back(stWykresow);
	return 0;
}
