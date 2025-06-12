// Mixer.cpp : implementation file
//
#include "pch.h"
#include "APL-SN.h"
#include "MixerDlg.h"
#include "konfig_fram.h"
//#include "modemdata.h"
#define _USE_MATH_DEFINES 
#include < math.h >

// CMixerDlg dialog

IMPLEMENT_DYNAMIC(CMixerDlg, CDialog)

CMixerDlg::CMixerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMixerDlg::IDD, pParent)
	, m_strKat1(_T(""))
	, m_strKat2(_T(""))
	, m_strKat3(_T(""))
	, m_strKat4(_T(""))
	, m_strKat5(_T(""))
	, m_strKat6(_T(""))
	, m_strKat7(_T(""))
	, m_strKat8(_T(""))
	, m_strDlugosc1(_T(""))
	, m_strDlugosc2(_T(""))
	, m_strDlugosc3(_T(""))
	, m_strDlugosc4(_T(""))
	, m_strDlugosc5(_T(""))
	, m_strDlugosc6(_T(""))
	, m_strDlugosc8(_T(""))
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
}


BEGIN_MESSAGE_MAP(CMixerDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CMixerDlg::OnBnClickedOk)
	ON_WM_PAINT()
	ON_NOTIFY(LVN_HOTTRACK, IDC_LIST_PREDEFINIOWANE, &CMixerDlg::OnLvnHotTrackLstPredefined)


	//ON_BN_CLICKED(IDC_BUT_SAVE, &CMixerDlg::OnBnClickedButSave)
	//ON_BN_CLICKED(IDC_BUT_READ, &CMixerDlg::OnBnClickedButRead)
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
END_MESSAGE_MAP()


// CMixerDlg message handlers



BOOL CMixerDlg::OnInitDialog()
{
	int x, y;

	CDialog::OnInitDialog();
	//wyzeruj ca³¹ tabelê predefiniowanej konfiguracji
	for (y=0; y<4; y++)
	{
		for (x=0; x< KANALY_MIKSERA; x++)
		{
			m_stPredefKonfig[y].fDlug[x] = 0;
			m_stPredefKonfig[y].fKat[x] = 0;
		}
	}

	//predefinionana konfiguracja dla 4X
	m_stPredefKonfig[0].fKat[0] = 45;
	m_stPredefKonfig[0].fKat[1] = 135;
	m_stPredefKonfig[0].fKat[2] = -135;
	m_stPredefKonfig[0].fKat[3] = -45;
	m_stPredefKonfig[0].strName = _T("Quadrokopter X");
	
	//predefinionana konfiguracja dla 4+
	m_stPredefKonfig[1].fKat[0] = 0;
	m_stPredefKonfig[1].fKat[1] = 90;
	m_stPredefKonfig[1].fKat[2] = 180;
	m_stPredefKonfig[1].fKat[3] = -90;
	m_stPredefKonfig[1].strName = _T("Quadrokopter +");
	
	for (x=0; x<4; x++)
	{
		m_stPredefKonfig[0].fDlug[x] = 250;
		m_stPredefKonfig[1].fDlug[x] = 250;
	}

	//predefinionana konfiguracja dla 6X
	m_stPredefKonfig[2].fKat[0] = 30;
	m_stPredefKonfig[2].fKat[1] = 90;
	m_stPredefKonfig[2].fKat[2] = 150;
	m_stPredefKonfig[2].fKat[3] = -150;
	m_stPredefKonfig[2].fKat[4] = -90;
	m_stPredefKonfig[2].fKat[5] = -30;
	m_stPredefKonfig[2].strName = _T("Hexakopter X");
	for (x=0; x<6; x++)
		m_stPredefKonfig[2].fDlug[x] = 300;

	//predefinionana konfiguracja dla 8X
	m_stPredefKonfig[3].fKat[0] = 22.5;
	m_stPredefKonfig[3].fKat[1] = 67.5;
	m_stPredefKonfig[3].fKat[2] = 112.5;
	m_stPredefKonfig[3].fKat[3] = 157.5;
	m_stPredefKonfig[3].fKat[4] = 202.5;
	m_stPredefKonfig[3].fKat[5] = 247.5;
	m_stPredefKonfig[3].fKat[6] = 292.5;
	m_stPredefKonfig[3].fKat[7] = 337.5;
	m_stPredefKonfig[3].strName = _T("Oktokopter X");
	for (x=0; x<8; x++)
		m_stPredefKonfig[3].fDlug[x] = 380;

	//dczytaj nastawy z APL
	OnBnClickedButRead();
	//UpdateAngleLen();	//wstaw nastawy do okna

	m_ctlPredefList.InsertColumn(0, _T("Konfiguracja"));
	m_ctlPredefList.SetColumnWidth(0, 200);
	for (x=0; x<4; x++)
		m_ctlPredefList.InsertItem(x, m_stPredefKonfig[x].strName);

	m_ctlPredefList.SetHoverTime(50);
	//m_ctlPredefList.SetItemState(0, 1, 0xFF);
	/*for (x=0; x<LICZBA_SERV_OUT; x++)
	{
		m_fAngle[x] = m_stPredefConfig[0].fAngle[x];
		m_fLenght[x] = m_stPredefConfig[0].fLenght[x];
	}*/
	
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


//uaktualnij pola edycyjne danymi
void CMixerDlg::UpdateAngleLen()
{
	m_strKat1.Format(_T("%.1f"), m_fKatRamienia[0]);
	m_strKat2.Format(_T("%.1f"), m_fKatRamienia[1]);
	m_strKat3.Format(_T("%.1f"), m_fKatRamienia[2]);
	m_strKat4.Format(_T("%.1f"), m_fKatRamienia[3]);
	m_strKat5.Format(_T("%.1f"), m_fKatRamienia[4]);
	m_strKat6.Format(_T("%.1f"), m_fKatRamienia[5]);
	m_strKat7.Format(_T("%.1f"), m_fKatRamienia[6]);
	m_strKat8.Format(_T("%.1f"), m_fKatRamienia[7]);


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



void CMixerDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CMixerDlg::OnPaint()
{
	CPoint ptLG, ptPD, ptCP, ptEng, ptArc, ptArcEnd;
	int x, nCenterRadius, nEngineRadius, nArcArrowRadius;
	float fScaleXY;
	CString str;
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages

	//granice obrazka
	ptLG.SetPoint(240, 10);
	ptPD.SetPoint(530, 300);
	//punkt œrodkowy
	ptCP.x = ptLG.x + (ptPD.x - ptLG.x)/2;
	ptCP.y = ptLG.y + (ptPD.y - ptLG.y)/2;

	//dc.Ellipse(ptLG.x, ptLG.y, ptPD.x, ptPD.y);

	
	nCenterRadius = 36;
	nEngineRadius = 120;	//œmig³o 10" w mm
	

	//oblicz skalê proporcji rysowania ramion
	float fMaxLen = m_fDlugoscRamienia[0];
	for (x=1; x< KANALY_MIKSERA; x++)
	{
		if (m_fDlugoscRamienia[x] > fMaxLen)
			fMaxLen = m_fDlugoscRamienia[x];		//najd³u¿sze ramiê
	}
	//skala w pix/mm
	fScaleXY = (ptPD.x - ptLG.x) / (2*(fMaxLen + nEngineRadius));

	//rysuj centraln¹ czêœæ ramy
	dc.Ellipse(ptCP.x-nCenterRadius, ptCP.y-nCenterRadius, ptCP.x+nCenterRadius, ptCP.y+nCenterRadius);

	//rysuj silniki
	nEngineRadius = (int)(nEngineRadius * fScaleXY + 0.5);
	//nArcArrowRadius = (int)(0.7*nEngineRadius);
	for (x=0; x< KANALY_MIKSERA; x++)
	{
		if (m_fDlugoscRamienia[x])
		{
			ptEng.x = ptCP.x + int(sin(m_fKatRamienia[x]* DEG2RAD)* m_fDlugoscRamienia[x]*fScaleXY);
			ptEng.y = ptCP.y - int(cos(m_fKatRamienia[x]* DEG2RAD)* m_fDlugoscRamienia[x]*fScaleXY);
			dc.Ellipse(ptEng.x-nEngineRadius, ptEng.y-nEngineRadius, ptEng.x+nEngineRadius, ptEng.y+nEngineRadius);
			str.Format(_T("%d"), x+1);
			dc.TextOutW(ptEng.x-4, ptEng.y-6, str, 1);

			//rysuj strza³kê kierunku obrotów: nieparzyste - CW, parzyste - CCW
			if ((x/2)*2 == x)		//okreœl czy silnik jest parzysty czy nie
			{
				//silniki nieparzyste - strza³ka CW
				//oblicz wspórzêdne wierzcho³ka strza³ki
				nArcArrowRadius = (int)(0.75*nEngineRadius);
				ptArcEnd.x = ptEng.x + (int)(cos(20* DEG2RAD)*nArcArrowRadius);
				ptArcEnd.y = ptEng.y - (int)(sin(20* DEG2RAD)*nArcArrowRadius);
				
				//rysuj doln¹ liniê strza³ki
				nArcArrowRadius = (int)(0.7*nEngineRadius);
				dc.Arc(ptEng.x-nArcArrowRadius, ptEng.y-nArcArrowRadius, ptEng.x+nArcArrowRadius, ptEng.y+nArcArrowRadius, 
					(ptEng.x+nArcArrowRadius), (ptEng.y-nArcArrowRadius), (ptEng.x-nArcArrowRadius), (ptEng.y-nArcArrowRadius));
				ptArc.x = ptEng.x + (int)(cos(45* DEG2RAD)*nArcArrowRadius);
				ptArc.y = ptEng.y - (int)(sin(45* DEG2RAD)*nArcArrowRadius);
				dc.MoveTo(ptArc);
				dc.LineTo(ptArc.x-3, ptArc.y+3);
				dc.LineTo(ptArcEnd);

				//rysuj górn¹ liniê strza³ki
				nArcArrowRadius = (int)(0.8*nEngineRadius);
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
				nArcArrowRadius = (int)(0.75*nEngineRadius);
				ptArcEnd.x = ptEng.x - (int)(cos(20* DEG2RAD)*nArcArrowRadius);
				ptArcEnd.y = ptEng.y - (int)(sin(20* DEG2RAD)*nArcArrowRadius);
				
				//rysuj doln¹ liniê strza³ki
				nArcArrowRadius = (int)(0.7*nEngineRadius);
				dc.Arc(ptEng.x-nArcArrowRadius, ptEng.y-nArcArrowRadius, ptEng.x+nArcArrowRadius, ptEng.y+nArcArrowRadius, 
					(ptEng.x+nArcArrowRadius), (ptEng.y-nArcArrowRadius), (ptEng.x-nArcArrowRadius), (ptEng.y-nArcArrowRadius));
				ptArc.x = ptEng.x - (int)(cos(45* DEG2RAD)*nArcArrowRadius);
				ptArc.y = ptEng.y - (int)(sin(45* DEG2RAD)*nArcArrowRadius);
				dc.MoveTo(ptArc);
				dc.LineTo(ptArc.x+3, ptArc.y+3);
				dc.LineTo(ptArcEnd);

				//rysuj górn¹ liniê strza³ki
				nArcArrowRadius = (int)(0.8*nEngineRadius);
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
	int nSel, x;

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	UpdateData();
	nSel = m_ctlPredefList.GetSelectionMark();
	if ((nSel > -1) && (nSel != m_nActualSel))
	{
		for (x=0; x< KANALY_MIKSERA; x++)
		{
			m_fKatRamienia[x] = m_stPredefKonfig[nSel].fKat[x];
			m_fDlugoscRamienia[x] = m_stPredefKonfig[nSel].fDlug[x];
		}
		UpdateAngleLen();
		Invalidate();
		m_nActualSel = nSel;
	}
	UpdateData(FALSE);
}



//przeliczenie danych i zapis do APL
void CMixerDlg::OnBnClickedButSave()
{
	int x, nErr;
	float fMixPith[KANALY_MIKSERA];
	float fMixRoll[KANALY_MIKSERA];
	float fMixYaw[KANALY_MIKSERA];

	for (x=0; x< KANALY_MIKSERA; x++)
	{
		fMixPith[x] = (float)(m_fDlugoscRamienia[x] * cos(m_fKatRamienia[x]* DEG2RAD));
		fMixRoll[x] = (float)(m_fDlugoscRamienia[x] * sin(m_fKatRamienia[x]* DEG2RAD));
		//mamy uk³ad prawoskrêtny, wiêc silniki parzyste CCW daj¹ moment dodatni a 
		// silniki nieparzyste CW daj¹ moment ujemny
		if ((x/2)*2 == x)		//okreœl czy silnik jest parzysty (x=0 => silnik nr 1 - nieparzysty)
			fMixYaw[x] = -1;
		else
			fMixYaw[x] = 1;
	}

	//zapisz dane do FRAM
	for (x=0; x< KANALY_MIKSERA; x++)
	{
		//nErr = getModemData().ZapiszFloat32Val2Fram(fMixPith[x], FAU_MIX_PITCH+4*x);
		//nErr = getModemData().ZapiszFloat32Val2Fram(fMixRoll[x], FAU_MIX_ROLL+4*x);
	}
	//prze³aduj konfiguracjê
	//getModemData().PrzeladujKonfigPID();
}


//odczytaj dane z AutoPitLota i oblicz na ich podstawie d³ugoœci ramion i k¹ty
void CMixerDlg::OnBnClickedButRead()
{
	int x, nErr;
	float fMixPith[KANALY_MIKSERA];
	float fMixRoll[KANALY_MIKSERA];

	//czytaj dane z FRAM
	for (x=0; x< KANALY_MIKSERA; x++)
	{
		/*nErr = getModemData().CzytajFram2Float32(&fMixPith[x], FAU_MIX_PITCH + 4 * x);
		if (nErr == IDABORT) return;
		nErr = getModemData().CzytajFram2Float32(&fMixRoll[x], FAU_MIX_ROLL+4*x);
		if (nErr == IDABORT) return;*/

		if (fMixPith[x])	//zabezpieczenie przed dzieleniem przez zero
		{
			m_fKatRamienia[x] = (float)(DEG2RAD * atan2(fMixRoll[x],fMixPith[x]));
			m_fDlugoscRamienia[x] = (float)sqrt(fMixRoll[x]*fMixRoll[x] + fMixPith[x]*fMixPith[x]);
		}
		else
		{
			m_fKatRamienia[x] = 0;
			m_fDlugoscRamienia[x] = 0;
		}
	}
	m_nActualSel = -1;	//wskaŸnik ostatnio wybranej przedefiniowanej pozycji
	UpdateAngleLen();
	Invalidate();
}


void CMixerDlg::OnEnChangeEditKat1()
{
	UpdateData();
	m_fKatRamienia[0] = (float)_wtof(m_strKat1);
	Invalidate();
}


void CMixerDlg::OnEnChangeEditKat2()
{
	UpdateData();
	m_fKatRamienia[1] = (float)_wtof(m_strKat2);
	Invalidate();
}


void CMixerDlg::OnEnChangeEditKat3()
{
	UpdateData();
	m_fKatRamienia[2] = (float)_wtof(m_strKat3);
	Invalidate();
}


void CMixerDlg::OnEnChangeEditKat4()
{
	UpdateData();
	m_fKatRamienia[3] = (float)_wtof(m_strKat4);
	Invalidate();
}


void CMixerDlg::OnEnChangeEditKat5()
{
	UpdateData();
	m_fKatRamienia[4] = (float)_wtof(m_strKat5);
	Invalidate();
}


void CMixerDlg::OnEnChangeEditKat6()
{
	UpdateData();
	m_fKatRamienia[5] = (float)_wtof(m_strKat6);
	Invalidate();
}


void CMixerDlg::OnEnChangeEditKat7()
{
	UpdateData();
	m_fKatRamienia[6] = (float)_wtof(m_strKat7);
	Invalidate();
}


void CMixerDlg::OnEnChangeEditKat8()
{
	UpdateData();
	m_fKatRamienia[7] = (float)_wtof(m_strKat8);
	Invalidate();
}


void CMixerDlg::OnEnChangeEditDlugosc1()
{
	UpdateData();
	m_fDlugoscRamienia[0] = (float)_wtof(m_strDlugosc1);
	Invalidate();
}


void CMixerDlg::OnEnChangeEditDlugosc2()
{
	UpdateData();
	m_fDlugoscRamienia[1] = (float)_wtof(m_strDlugosc2);
	Invalidate();
}


void CMixerDlg::OnEnChangeEditDlugosc3()
{
	UpdateData();
	m_fDlugoscRamienia[2] = (float)_wtof(m_strDlugosc3);
	Invalidate();
}


void CMixerDlg::OnEnChangeEditDlugosc4()
{
	UpdateData();
	m_fDlugoscRamienia[3] = (float)_wtof(m_strDlugosc4);
	Invalidate();
}


void CMixerDlg::OnEnChangeEditDlugosc5()
{
	UpdateData();
	m_fDlugoscRamienia[4] = (float)_wtof(m_strDlugosc5);
	Invalidate();
}


void CMixerDlg::OnEnChangeEditDlugosc6()
{
	UpdateData();
	m_fDlugoscRamienia[5] = (float)_wtof(m_strDlugosc6);
	Invalidate();
}


void CMixerDlg::OnEnChangeEditDlugosc7()
{
	UpdateData();
	m_fDlugoscRamienia[6] = (float)_wtof(m_strDlugosc7);
	Invalidate();
}


void CMixerDlg::OnEnChangeEditDlugosc8()
{
	UpdateData();
	m_fDlugoscRamienia[7] = (float)_wtof(m_strDlugosc8);
	Invalidate();
}
