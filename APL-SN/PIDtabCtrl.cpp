#include "pch.h"
#include "PIDtabCtrl.h"
BEGIN_MESSAGE_MAP(PIDtabCtrl, CTabCtrl)
	ON_WM_PAINT()
	ON_WM_CREATE()
END_MESSAGE_MAP()



///////////////////////////////////////////////////////////////////////////////////////////////////
// Obs�uga rysowania uk�adu sterowania
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void PIDtabCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Dodaj tutaj sw�j kod procedury obs�ugi komunikat�w
					   // Nie wywo�uj CTabCtrl::OnPaint() do malowania komunikat�w
	CPoint ptLG, ptPD, ptCP;
	CRect prost;
	dc.GetBoundsRect(&prost, DCB_RESET);

	//granice obrazka
	ptLG.SetPoint(10, 30);
	ptPD.SetPoint(prost.Width()-10, prost.Height()-30);

	//�rodek
	ptCP.x = ptLG.x + (ptPD.x - ptLG.x) / 2;
	ptCP.y = ptLG.y + (ptPD.y - ptLG.y) / 2;

	//w�ze� sumuj�cy regulatora pr�dko�ci
	dc.AngleArc(ptLG.x + 50, ptCP.y, 20, 0.0f, 360.0f);

	dc.AngleArc(ptCP.x + 50, ptCP.y, 20, 0.0f, 360.0f);



	dc.MoveTo(ptLG);
	//dc.LineTo(ptArc.x - 3, ptArc.y + 3);
	dc.LineTo(ptPD);

}


//BOOL PIDtabCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
//{
//	// TODO: Dodaj tutaj sw�j wyspecjalizowany kod i/lub wywo�aj klas� bazow�
//
//	return CTabCtrl::Create(dwStyle, rect, pParentWnd, nID);
//}


int PIDtabCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTabCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Dodaj tutaj sw�j wyspecjalizowany kod tworzenia
	TC_ITEM ti;

	ti.mask = TCIF_TEXT;
	ti.pszText = _T("Tab0");
	InsertItem(0, &ti);
	ti.pszText = _T("Tab1");
	InsertItem(1, &ti);
	ti.pszText = _T("Tab2");
	InsertItem(2, &ti);

	InsertItem(3, _T("abc"));
	ShowWindow(SW_SHOWDEFAULT);
	return 0;
}
