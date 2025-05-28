#include "pch.h"
#include "PIDtabCtrl.h"
BEGIN_MESSAGE_MAP(PIDtabCtrl, CTabCtrl)
	ON_WM_PAINT()
END_MESSAGE_MAP()



///////////////////////////////////////////////////////////////////////////////////////////////////
// Obs³uga rysowania uk³adu sterowania
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void PIDtabCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Dodaj tutaj swój kod procedury obs³ugi komunikatów
					   // Nie wywo³uj CTabCtrl::OnPaint() do malowania komunikatów
	//CPoint ptLG, ptPD, ptCP;
	CPoint ptLG, ptPD, ptCP, ptEng, ptArc, ptArcEnd;


	//granice obrazka
	ptLG.SetPoint(10, 20);
	ptPD.SetPoint(600, 300);
	//punkt œrodkowy
	ptCP.x = ptLG.x + (ptPD.x - ptLG.x) / 2;
	ptCP.y = ptLG.y + (ptPD.y - ptLG.y) / 2;


	dc.MoveTo(ptLG);
	//dc.LineTo(ptArc.x - 3, ptArc.y + 3);
	dc.LineTo(ptPD);

	//ptArc.x = ptEng.x - (int)(cos(45 * deg2rad) * nArcArrowRadius);
	//ptArc.y = ptEng.y - (int)(sin(45 * deg2rad) * nArcArrowRadius);
	//dc.Arc(ptEng.x - nArcArrowRadius, ptEng.y - nArcArrowRadius, ptEng.x + nArcArrowRadius, ptEng.y + nArcArrowRadius,
		//(ptEng.x + nArcArrowRadius), (ptEng.y - nArcArrowRadius), (ptEng.x - nArcArrowRadius), (ptEng.y - nArcArrowRadius));
	dc.AngleArc(ptCP.x, ptCP.y, 20, 0.0f, 360.0f); 
}
