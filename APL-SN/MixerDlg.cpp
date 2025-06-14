// Mixer.cpp : implementation file
//
#include "pch.h"
#include "APL-SN.h"
#include "MixerDlg.h"
#include "konfig_fram.h"
#include "Errors.h"
#define _USE_MATH_DEFINES 
#include < math.h >

// CMixerDlg dialog

IMPLEMENT_DYNAMIC(CMixerDlg, CDialog)

CMixerDlg::CMixerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMixerDlg::IDD, pParent)
	, m_strKat1(_T("0"))
	, m_strKat2(_T("0"))
	, m_strKat3(_T("0"))
	, m_strKat4(_T("0"))
	, m_strKat5(_T("0"))
	, m_strKat6(_T("0"))
	, m_strKat7(_T("0"))
	, m_strKat8(_T("0"))
	, m_strDlugosc1(_T("0"))
	, m_strDlugosc2(_T("0"))
	, m_strDlugosc3(_T("0"))
	, m_strDlugosc4(_T("0"))
	, m_strDlugosc5(_T("0"))
	, m_strDlugosc6(_T("0"))
	, m_strDlugosc8(_T("0"))
	, m_strSrednicaSmiglaMm(_T("254"))
	, m_strSrednicaSmiglaCale(_T("10"))
	, m_fPromienSmigla(127)
	, m_bBylaZmianaDlugosci(FALSE)
	, m_bBylaZmianaKata(FALSE)
{

}

CMixerDlg::~CMixerDlg()
{
}

void CMixerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);


	//DDX_Control(pDX, IDC_LST_PREDEFINED, m_ctlPredefList);
	DDX_Text(pDX, IDC_EDIT_KAT1, m_strKat1);
	DDX_Text(pDX, IDC_EDIT_KAT2, m_strKat2);
	DDX_Text(pDX, IDC_EDIT_KAT3, m_strKat3);
	DDX_Text(pDX, IDC_EDIT_KAT4, m_strKat4);
	DDX_Text(pDX, IDC_EDIT_KAT5, m_strKat5);
	DDX_Text(pDX, IDC_EDIT_KAT6, m_strKat6);
	DDX_Text(pDX, IDC_EDIT_KAT7, m_strKat7);
	DDX_Text(pDX, IDC_EDIT_KAT8, m_strKat8);
	DDX_Text(pDX, IDC_EDIT_DLUGOSC1, m_strDlugosc1);
	DDX_Text(pDX, IDC_EDIT_DLUGOSC2, m_strDlugosc2);
	DDX_Text(pDX, IDC_EDIT_DLUGOSC3, m_strDlugosc3);
	DDX_Text(pDX, IDC_EDIT_DLUGOSC4, m_strDlugosc4);
	DDX_Text(pDX, IDC_EDIT_DLUGOSC5, m_strDlugosc5);
	DDX_Text(pDX, IDC_EDIT_DLUGOSC6, m_strDlugosc6);
	DDX_Text(pDX, IDC_EDIT_DLUGOSC7, m_strDlugosc7);
	DDX_Text(pDX, IDC_EDIT_DLUGOSC8, m_strDlugosc8);
	DDX_Control(pDX, IDC_LIST_PREDEFINIOWANE, m_ctlPredefiniowane);
	DDX_Control(pDX, IDC_LIST_PREDEFINIOWANE, m_ctlPredefiniowane);
	DDX_Text(pDX, IDC_EDIT_SREDNICA_SMIGLA_MM, m_strSrednicaSmiglaMm);
	DDX_Text(pDX, IDC_EDIT_SREDNICA_SMIGLA_CALE, m_strSrednicaSmiglaCale);
}


BEGIN_MESSAGE_MAP(CMixerDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CMixerDlg::OnBnClickedOk)
	ON_WM_PAINT()
	ON_NOTIFY(LVN_HOTTRACK, IDC_LIST_PREDEFINIOWANE, &CMixerDlg::OnLvnHotTrackLstPredefined)
	ON_EN_CHANGE(IDC_EDIT_KAT1, &CMixerDlg::OnEnChangeEditKat1)
	ON_EN_CHANGE(IDC_EDIT_KAT2, &CMixerDlg::OnEnChangeEditKat2)
	ON_EN_CHANGE(IDC_EDIT_KAT3, &CMixerDlg::OnEnChangeEditKat3)
	ON_EN_CHANGE(IDC_EDIT_KAT4, &CMixerDlg::OnEnChangeEditKat4)
	ON_EN_CHANGE(IDC_EDIT_KAT5, &CMixerDlg::OnEnChangeEditKat5)
	ON_EN_CHANGE(IDC_EDIT_KAT6, &CMixerDlg::OnEnChangeEditKat6)
	ON_EN_CHANGE(IDC_EDIT_KAT7, &CMixerDlg::OnEnChangeEditKat7)
	ON_EN_CHANGE(IDC_EDIT_KAT8, &CMixerDlg::OnEnChangeEditKat8)
	ON_EN_CHANGE(IDC_EDIT_DLUGOSC1, &CMixerDlg::OnEnChangeEditDlugosc1)
	ON_EN_CHANGE(IDC_EDIT_DLUGOSC2, &CMixerDlg::OnEnChangeEditDlugosc2)
	ON_EN_CHANGE(IDC_EDIT_DLUGOSC3, &CMixerDlg::OnEnChangeEditDlugosc3)
	ON_EN_CHANGE(IDC_EDIT_DLUGOSC4, &CMixerDlg::OnEnChangeEditDlugosc4)
	ON_EN_CHANGE(IDC_EDIT_DLUGOSC5, &CMixerDlg::OnEnChangeEditDlugosc5)
	ON_EN_CHANGE(IDC_EDIT_DLUGOSC6, &CMixerDlg::OnEnChangeEditDlugosc6)
	ON_EN_CHANGE(IDC_EDIT_DLUGOSC7, &CMixerDlg::OnEnChangeEditDlugosc7)
	ON_EN_CHANGE(IDC_EDIT_DLUGOSC8, &CMixerDlg::OnEnChangeEditDlugosc8)
	ON_EN_CHANGE(IDC_EDIT_SREDNICA_SMIGLA_MM, &CMixerDlg::OnEnChangeEditSrednicaSmiglaMm)
	ON_EN_CHANGE(IDC_EDIT_SREDNICA_SMIGLA_CALE, &CMixerDlg::OnEnChangeEditSrednicaSmiglaCale)
END_MESSAGE_MAP()


// CMixerDlg message handlers



BOOL CMixerDlg::OnInitDialog()
{
	uint8_t chErr;
	float fMixPrze[KANALY_MIKSERA];
	float fMixPoch[KANALY_MIKSERA];
	//float fMixOdch[KANALY_MIKSERA];

	CDialog::OnInitDialog();

	//odczytaj dane z APL i oblicz na ich podstawie d³ugoœci ramion i k¹ty
	chErr = getKomunikacja().CzytajFloatFRAM(fMixPrze, KANALY_MIKSERA, FAU_MIX_PRZECH);	//wspó³czynnik wp³ywu przechylenia na dany silnik
	chErr |= getKomunikacja().CzytajFloatFRAM(fMixPoch, KANALY_MIKSERA, FAU_MIX_POCHYL);	//wspó³czynnik wp³ywu pochylenia na dany silnik
	//chErr |= getKomunikacja().CzytajFloatFRAM(fMixOdch, KANALY_MIKSERA, FAU_MIX_ODCHYL);	//wspó³czynnik wp³ywu odchylenia na dany silnik
	if (chErr)
		MessageBoxExW(this->m_hWnd, _T("B³¹d odczytu danych"), _T("Ojojoj!"), MB_ICONWARNING, 0);

	for (int n = 0; n < KANALY_MIKSERA; n++)
	{
		if (fMixPoch[n])	//zabezpieczenie przed dzieleniem przez zero
		{
			m_fKatRamienia[n] = (float)(atan2(fMixPrze[n], fMixPoch[n]));
			m_fDlugoscRamienia[n] = (float)sqrt(fMixPrze[n] * fMixPrze[n] + fMixPoch[n] * fMixPoch[n]);
		}
		else
		{
			m_fKatRamienia[n] = 0;
			m_fDlugoscRamienia[n] = 0;
		}
	}
	m_nActualSel = -1;	//wskaŸnik ostatnio wybranej przedefiniowanej pozycji
	AktualizujDlugoscKat();
	Invalidate();

	//wyzeruj ca³¹ tabelê predefiniowanej konfiguracji
	for (int k=0; k< ILOSC_PREDEF_KONF; k++)
	{
		for (int n=0; n< KANALY_MIKSERA; n++)
		{
			m_stPredefKonfig[k].fDlug[n] = 0;
			m_stPredefKonfig[k].fKat[n] = 0;
		}
	}

	//predefinionana konfiguracja dla 4X
	m_stPredefKonfig[0].fKat[0] = (float)(45 * DEG2RAD);
	m_stPredefKonfig[0].fKat[1] = (float)(135 * DEG2RAD);
	m_stPredefKonfig[0].fKat[2] = (float)(-135 * DEG2RAD);
	m_stPredefKonfig[0].fKat[3] = (float)(-45 * DEG2RAD);
	m_stPredefKonfig[0].strName = _T("Kwadrokopter X");
	
	//predefinionana konfiguracja dla 4+
	m_stPredefKonfig[1].fKat[0] = (float)(0 * DEG2RAD);
	m_stPredefKonfig[1].fKat[1] = (float)(90 * DEG2RAD);
	m_stPredefKonfig[1].fKat[2] = (float)(180 * DEG2RAD);
	m_stPredefKonfig[1].fKat[3] = (float)(-90 * DEG2RAD);
	m_stPredefKonfig[1].strName = _T("Kwadrokopter +");
	
	for (int n=0; n<4; n++)
	{
		m_stPredefKonfig[0].fDlug[n] = 250;
		m_stPredefKonfig[1].fDlug[n] = 250;
	}

	//predefinionana konfiguracja dla 6X
	m_stPredefKonfig[2].fKat[0] = (float)(30 * DEG2RAD);
	m_stPredefKonfig[2].fKat[1] = (float)(90 * DEG2RAD);
	m_stPredefKonfig[2].fKat[2] = (float)(150 * DEG2RAD);
	m_stPredefKonfig[2].fKat[3] = (float)(-150 * DEG2RAD);
	m_stPredefKonfig[2].fKat[4] = (float)(-90 * DEG2RAD);
	m_stPredefKonfig[2].fKat[5] = (float)(-30 * DEG2RAD);
	m_stPredefKonfig[2].strName = _T("Hexakopter X");
	for (int n = 0; n < 6; n++)
		m_stPredefKonfig[2].fDlug[n] = 300;

	//predefinionana konfiguracja dla 6H
	m_stPredefKonfig[3].fKat[0] = (float)(35 * DEG2RAD);
	m_stPredefKonfig[3].fKat[1] = (float)(90 * DEG2RAD);
	m_stPredefKonfig[3].fKat[2] = (float)(145 * DEG2RAD);
	m_stPredefKonfig[3].fKat[3] = (float)(-145 * DEG2RAD);
	m_stPredefKonfig[3].fKat[4] = (float)(-90 * DEG2RAD);
	m_stPredefKonfig[3].fKat[5] = (float)(-35 * DEG2RAD);
	m_stPredefKonfig[3].strName = _T("Hexakopter H");
	for (int n = 0; n < 6; n++)
		m_stPredefKonfig[3].fDlug[n] = 350;
	m_stPredefKonfig[3].fDlug[1] = 200;
	m_stPredefKonfig[3].fDlug[4] = 200;

	//predefinionana konfiguracja dla 8X
	m_stPredefKonfig[4].fKat[0] = (float)(22.5 * DEG2RAD);
	m_stPredefKonfig[4].fKat[1] = (float)(67.5 * DEG2RAD);
	m_stPredefKonfig[4].fKat[2] = (float)(112.5 * DEG2RAD);
	m_stPredefKonfig[4].fKat[3] = (float)(157.5 * DEG2RAD);
	m_stPredefKonfig[4].fKat[4] = (float)(-157.5 * DEG2RAD);
	m_stPredefKonfig[4].fKat[5] = (float)(-112.5 * DEG2RAD);
	m_stPredefKonfig[4].fKat[6] = (float)(-67.5 * DEG2RAD);
	m_stPredefKonfig[4].fKat[7] = (float)(-22.5 * DEG2RAD);
	m_stPredefKonfig[4].strName = _T("Oktokopter X");
	for (int n = 0; n < 8; n++)
		m_stPredefKonfig[4].fDlug[n] = 380;

	m_ctlPredefiniowane.InsertColumn(0, _T("Przyk³adowa konfiguracja"));
	m_ctlPredefiniowane.SetColumnWidth(0, 200);
	for (int n=0; n< ILOSC_PREDEF_KONF; n++)
		m_ctlPredefiniowane.InsertItem(n, m_stPredefKonfig[n].strName);
	m_ctlPredefiniowane.SetHoverTime(10);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


//uaktualnij pola edycyjne danymi
void CMixerDlg::AktualizujDlugoscKat()
{
	m_strKat1.Format(_T("%.1f"), m_fKatRamienia[0] * RAD2DEG);
	m_strKat2.Format(_T("%.1f"), m_fKatRamienia[1] * RAD2DEG);
	m_strKat3.Format(_T("%.1f"), m_fKatRamienia[2] * RAD2DEG);
	m_strKat4.Format(_T("%.1f"), m_fKatRamienia[3] * RAD2DEG);
	m_strKat5.Format(_T("%.1f"), m_fKatRamienia[4] * RAD2DEG);
	m_strKat6.Format(_T("%.1f"), m_fKatRamienia[5] * RAD2DEG);
	m_strKat7.Format(_T("%.1f"), m_fKatRamienia[6] * RAD2DEG);
	m_strKat8.Format(_T("%.1f"), m_fKatRamienia[7] * RAD2DEG);

	m_strDlugosc1.Format(_T("%.1f"), m_fDlugoscRamienia[0]);
	m_strDlugosc2.Format(_T("%.1f"), m_fDlugoscRamienia[1]);
	m_strDlugosc3.Format(_T("%.1f"), m_fDlugoscRamienia[2]);
	m_strDlugosc4.Format(_T("%.1f"), m_fDlugoscRamienia[3]);
	m_strDlugosc5.Format(_T("%.1f"), m_fDlugoscRamienia[4]);
	m_strDlugosc6.Format(_T("%.1f"), m_fDlugoscRamienia[5]);
	m_strDlugosc7.Format(_T("%.1f"), m_fDlugoscRamienia[6]);
	m_strDlugosc8.Format(_T("%.1f"), m_fDlugoscRamienia[7]);
	UpdateData(FALSE);
}




void CMixerDlg::OnPaint()
{
	CPoint ptLG, ptPD, ptCP, ptEng, ptArc, ptArcEnd;
	int x, nCenterRadius, nArcArrowRadius;
	int nPromienSmigla;
	float fScaleXY;
	CString str;
	HPEN hPenOsi = CreatePen(PS_DASHDOT, 1, RGB(150, 150, 150));
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(50, 50, 50));
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages

	//granice obrazka
	ptLG.SetPoint(240, 10);
	ptPD.SetPoint(530, 300);
	//punkt œrodkowy
	ptCP.x = ptLG.x + (ptPD.x - ptLG.x)/2;
	ptCP.y = ptLG.y + (ptPD.y - ptLG.y)/2;
	nCenterRadius = 36;

	//oblicz skalê proporcji rysowania ramion
	float fMaxLen = m_fDlugoscRamienia[0];
	for (x=1; x< KANALY_MIKSERA; x++)
	{
		if (m_fDlugoscRamienia[x] > fMaxLen)
			fMaxLen = m_fDlugoscRamienia[x];		//najd³u¿sze ramiê
	}
	
	fScaleXY = (ptPD.x - ptLG.x) / (2*(fMaxLen + m_fPromienSmigla));		//skala w pix/mm

	//rysuj œrodek ciê¿koœci
	//dc.Ellipse(ptCP.x-nCenterRadius, ptCP.y-nCenterRadius, ptCP.x+nCenterRadius, ptCP.y+nCenterRadius);
	str.Format(_T("SC"));
	dc.TextOutW(ptCP.x - 8, ptCP.y - 8, str, 2);

	//rysuj silniki
	
	nPromienSmigla = (int)(m_fPromienSmigla * fScaleXY + 0.5);	//przejscie z mm na pixele
	for (x=0; x< KANALY_MIKSERA; x++)
	{
		if (m_fDlugoscRamienia[x])
		{
			ptEng.x = ptCP.x + int(sin(m_fKatRamienia[x])* m_fDlugoscRamienia[x]*fScaleXY);
			ptEng.y = ptCP.y - int(cos(m_fKatRamienia[x])* m_fDlugoscRamienia[x]*fScaleXY);
			dc.Ellipse(ptEng.x- nPromienSmigla, ptEng.y- nPromienSmigla, ptEng.x+ nPromienSmigla, ptEng.y+ nPromienSmigla);
			str.Format(_T("%d"), x+1);
			dc.TextOutW(ptEng.x-4, ptEng.y-6, str, 1);

			//rysuj ramiê
			dc.SelectObject(hPenOsi);
			dc.MoveTo(ptCP);
			dc.LineTo(ptEng);
			dc.SelectObject(hPen);

			//rysuj strza³kê kierunku obrotów: nieparzyste - CW, parzyste - CCW
			if ((x/2)*2 == x)		//okreœl czy silnik jest parzysty czy nie
			{
				//silniki nieparzyste - strza³ka CW
				//oblicz wspórzêdne wierzcho³ka strza³ki
				nArcArrowRadius = (int)(0.75* nPromienSmigla);
				ptArcEnd.x = ptEng.x + (int)(cos(20* DEG2RAD)*nArcArrowRadius);
				ptArcEnd.y = ptEng.y - (int)(sin(20* DEG2RAD)*nArcArrowRadius);
				
				//rysuj doln¹ liniê strza³ki
				nArcArrowRadius = (int)(0.7* nPromienSmigla);
				dc.Arc(ptEng.x-nArcArrowRadius, ptEng.y-nArcArrowRadius, ptEng.x+nArcArrowRadius, ptEng.y+nArcArrowRadius, 
					(ptEng.x+nArcArrowRadius), (ptEng.y-nArcArrowRadius), (ptEng.x-nArcArrowRadius), (ptEng.y-nArcArrowRadius));
				ptArc.x = ptEng.x + (int)(cos(45* DEG2RAD)*nArcArrowRadius);
				ptArc.y = ptEng.y - (int)(sin(45* DEG2RAD)*nArcArrowRadius);
				dc.MoveTo(ptArc);
				dc.LineTo(ptArc.x-3, ptArc.y+3);
				dc.LineTo(ptArcEnd);

				//rysuj górn¹ liniê strza³ki
				nArcArrowRadius = (int)(0.8* nPromienSmigla);
				dc.Arc(ptEng.x-nArcArrowRadius, ptEng.y-nArcArrowRadius, ptEng.x+nArcArrowRadius, ptEng.y+nArcArrowRadius, 
					(ptEng.x+nArcArrowRadius), (ptEng.y-nArcArrowRadius), (ptEng.x-nArcArrowRadius), (ptEng.y-nArcArrowRadius));
				ptArc.x = ptEng.x + (int)(cos(45* DEG2RAD)*nArcArrowRadius);
				ptArc.y = ptEng.y - (int)(sin(45* DEG2RAD)*nArcArrowRadius);
				dc.MoveTo(ptArc);
				dc.LineTo(ptArc.x+3, ptArc.y-3);
				dc.LineTo(ptArcEnd);
			}
			else
			{
				//silniki parzyste - strza³ka CCW
				//oblicz wspórzêdne wierzcho³ka strza³ki
				nArcArrowRadius = (int)(0.75* nPromienSmigla);
				ptArcEnd.x = ptEng.x - (int)(cos(20* DEG2RAD)*nArcArrowRadius);
				ptArcEnd.y = ptEng.y - (int)(sin(20* DEG2RAD)*nArcArrowRadius);
				
				//rysuj doln¹ liniê strza³ki
				nArcArrowRadius = (int)(0.7* nPromienSmigla);
				dc.Arc(ptEng.x-nArcArrowRadius, ptEng.y-nArcArrowRadius, ptEng.x+nArcArrowRadius, ptEng.y+nArcArrowRadius, 
					(ptEng.x+nArcArrowRadius), (ptEng.y-nArcArrowRadius), (ptEng.x-nArcArrowRadius), (ptEng.y-nArcArrowRadius));
				ptArc.x = ptEng.x - (int)(cos(45* DEG2RAD)*nArcArrowRadius);
				ptArc.y = ptEng.y - (int)(sin(45* DEG2RAD)*nArcArrowRadius);
				dc.MoveTo(ptArc);
				dc.LineTo(ptArc.x+3, ptArc.y+3);
				dc.LineTo(ptArcEnd);

				//rysuj górn¹ liniê strza³ki
				nArcArrowRadius = (int)(0.8* nPromienSmigla);
				dc.Arc(ptEng.x-nArcArrowRadius, ptEng.y-nArcArrowRadius, ptEng.x+nArcArrowRadius, ptEng.y+nArcArrowRadius, 
					(ptEng.x+nArcArrowRadius), (ptEng.y-nArcArrowRadius), (ptEng.x-nArcArrowRadius), (ptEng.y-nArcArrowRadius));
				ptArc.x = ptEng.x - (int)(cos(45* DEG2RAD)*nArcArrowRadius);
				ptArc.y = ptEng.y - (int)(sin(45* DEG2RAD)*nArcArrowRadius);
				dc.MoveTo(ptArc);
				dc.LineTo(ptArc.x-3, ptArc.y-3);
				dc.LineTo(ptArcEnd);
			}
		}
	}	
}



void CMixerDlg::OnLvnHotTrackLstPredefined(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
	UpdateData();
	int nSel = m_ctlPredefiniowane.GetSelectionMark();
	if ((nSel > -1) && (nSel != m_nActualSel))
	{
		for (int x=0; x< KANALY_MIKSERA; x++)
		{
			m_fKatRamienia[x] = m_stPredefKonfig[nSel].fKat[x];
			m_fDlugoscRamienia[x] = m_stPredefKonfig[nSel].fDlug[x];
		}
		AktualizujDlugoscKat();
		Invalidate();
		m_nActualSel = nSel;
	}
	UpdateData(FALSE);
}



//przeliczenie danych i zapis do APL
void CMixerDlg::OnBnClickedOk()
{
	float fMixPrze[KANALY_MIKSERA];	//wspó³czynnik wp³ywu przechylenia na dany silnik
	float fMixPoch[KANALY_MIKSERA];	//wspó³czynnik wp³ywu pochylenia na dany silnik
	float fMixOdch[KANALY_MIKSERA];	//wspó³czynnik wp³ywu odchylenia na dany silnik
	uint8_t chErr;

	for (int n = 0; n < KANALY_MIKSERA; n++)
	{
		fMixPrze[n] = (float)(m_fDlugoscRamienia[n] * sinf(m_fKatRamienia[n]));
		fMixPoch[n] = (float)(m_fDlugoscRamienia[n] * cosf(m_fKatRamienia[n]));
		
		//mamy uk³ad prawoskrêtny, wiêc silniki parzyste CCW daj¹ moment dodatni a 
		// silniki nieparzyste CW daj¹ moment ujemny
		if ((n / 2) * 2 == n)		//okreœl czy silnik jest parzysty (x=0 => silnik nr 1 - nieparzysty)
			fMixOdch[n] = -1;
		else
			fMixOdch[n] = 1;
	}

	//zapisz dane do FRAM
	chErr = getKomunikacja().ZapiszFloatFRAM(fMixPrze, KANALY_MIKSERA, FAU_MIX_PRZECH);
	chErr |= getKomunikacja().ZapiszFloatFRAM(fMixPoch, KANALY_MIKSERA, FAU_MIX_POCHYL);
	chErr |= getKomunikacja().ZapiszFloatFRAM(fMixOdch, KANALY_MIKSERA, FAU_MIX_ODCHYL);
	if (chErr != ERR_OK)
		MessageBoxExW(this->m_hWnd, _T("B³¹d zapisu"), _T("Ojojoj!"), MB_ICONWARNING, 0);
 
	OnOK();
}



void CMixerDlg::OnEnChangeEditKat1()
{
	UpdateData();
	m_fKatRamienia[0] = (float)(_wtof(m_strKat1) * DEG2RAD);
	m_bBylaZmianaKata = TRUE;
	Invalidate();
}


void CMixerDlg::OnEnChangeEditKat2()
{
	UpdateData();
	m_fKatRamienia[1] = (float)(_wtof(m_strKat2) * DEG2RAD);
	m_bBylaZmianaKata = TRUE;
	Invalidate();
}


void CMixerDlg::OnEnChangeEditKat3()
{
	UpdateData();
	m_fKatRamienia[2] = (float)(_wtof(m_strKat3) * DEG2RAD);
	m_bBylaZmianaKata = TRUE;
	Invalidate();
}


void CMixerDlg::OnEnChangeEditKat4()
{
	UpdateData();
	m_fKatRamienia[3] = (float)(_wtof(m_strKat4) * DEG2RAD);
	m_bBylaZmianaKata = TRUE;
	Invalidate();
}


void CMixerDlg::OnEnChangeEditKat5()
{
	UpdateData();
	m_fKatRamienia[4] = (float)(_wtof(m_strKat5) * DEG2RAD);
	m_bBylaZmianaKata = TRUE;
	Invalidate();
}


void CMixerDlg::OnEnChangeEditKat6()
{
	UpdateData();
	m_fKatRamienia[5] = (float)(_wtof(m_strKat6) * DEG2RAD);
	m_bBylaZmianaKata = TRUE;
	Invalidate();
}


void CMixerDlg::OnEnChangeEditKat7()
{
	UpdateData();
	m_fKatRamienia[6] = (float)(_wtof(m_strKat7) * DEG2RAD);
	m_bBylaZmianaKata = TRUE;
	Invalidate();
}


void CMixerDlg::OnEnChangeEditKat8()
{
	UpdateData();
	m_fKatRamienia[7] = (float)(_wtof(m_strKat8) * DEG2RAD);
	m_bBylaZmianaKata = TRUE;
	Invalidate();
}


void CMixerDlg::OnEnChangeEditDlugosc1()
{
	UpdateData();
	m_fDlugoscRamienia[0] = (float)_wtof(m_strDlugosc1);
	m_bBylaZmianaDlugosci = TRUE;
	Invalidate();
}


void CMixerDlg::OnEnChangeEditDlugosc2()
{
	UpdateData();
	m_fDlugoscRamienia[1] = (float)_wtof(m_strDlugosc2);
	m_bBylaZmianaDlugosci = TRUE;
	Invalidate();
}


void CMixerDlg::OnEnChangeEditDlugosc3()
{
	UpdateData();
	m_fDlugoscRamienia[2] = (float)_wtof(m_strDlugosc3);
	m_bBylaZmianaDlugosci = TRUE;
	Invalidate();
}


void CMixerDlg::OnEnChangeEditDlugosc4()
{
	UpdateData();
	m_fDlugoscRamienia[3] = (float)_wtof(m_strDlugosc4);
	m_bBylaZmianaDlugosci = TRUE;
	Invalidate();
}


void CMixerDlg::OnEnChangeEditDlugosc5()
{
	UpdateData();
	m_fDlugoscRamienia[4] = (float)_wtof(m_strDlugosc5);
	m_bBylaZmianaDlugosci = TRUE;
	Invalidate();
}


void CMixerDlg::OnEnChangeEditDlugosc6()
{
	UpdateData();
	m_fDlugoscRamienia[5] = (float)_wtof(m_strDlugosc6);
	m_bBylaZmianaDlugosci = TRUE;
	Invalidate();
}


void CMixerDlg::OnEnChangeEditDlugosc7()
{
	UpdateData();
	m_fDlugoscRamienia[6] = (float)_wtof(m_strDlugosc7);
	m_bBylaZmianaDlugosci = TRUE;
	Invalidate();
}


void CMixerDlg::OnEnChangeEditDlugosc8()
{
	UpdateData();
	m_fDlugoscRamienia[7] = (float)_wtof(m_strDlugosc8);
	m_bBylaZmianaDlugosci = TRUE;
	Invalidate();
}


void CMixerDlg::OnEnChangeEditSrednicaSmiglaMm()
{
	UpdateData();
	m_fPromienSmigla = (float)_wtof(m_strSrednicaSmiglaMm) / 2;
	m_strSrednicaSmiglaCale.Format(_T("%1.f"), 2 * m_fPromienSmigla / 25.4f);
	UpdateData(FALSE);
	Invalidate();
}


void CMixerDlg::OnEnChangeEditSrednicaSmiglaCale()
{
	UpdateData();
	m_fPromienSmigla = (float)_wtof(m_strSrednicaSmiglaCale) * 25.4f / 2;
	m_strSrednicaSmiglaMm.Format(_T("%1.f"), 2 * m_fPromienSmigla);
	UpdateData(FALSE);
	Invalidate();
}
