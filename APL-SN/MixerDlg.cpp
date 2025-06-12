// Mixer.cpp : implementation file
//
#include "pch.h"
#include "APL-SN.h"
#include "MixerDlg.h"
#include "konfig_fram.h"
//#include "modemdata.h"

// CMixerDlg dialog

IMPLEMENT_DYNAMIC(CMixerDlg, CDialog)

CMixerDlg::CMixerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMixerDlg::IDD, pParent)
	, m_strAngle01(_T(""))
	, m_strAngle02(_T(""))
	, m_strAngle03(_T(""))
	, m_strAngle04(_T(""))
	, m_strAngle05(_T(""))
	, m_strAngle06(_T(""))
	, m_strAngle07(_T(""))
	, m_strAngle08(_T(""))
	, m_strAngle09(_T(""))
	, m_strAngle10(_T(""))
	, m_strAngle11(_T(""))
	, m_strAngle12(_T(""))
	, m_strLength01(_T(""))
	, m_strLength02(_T(""))
	, m_strLength03(_T(""))
	, m_strLength04(_T(""))
	, m_strLength05(_T(""))
	, m_strLength06(_T(""))
	, m_strLength07(_T(""))
	, m_strLength08(_T(""))
{

}

CMixerDlg::~CMixerDlg()
{
}

void CMixerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_ANGLE01, m_strAngle01);
	DDX_Text(pDX, IDC_EDT_ANGLE02, m_strAngle02);
	DDX_Text(pDX, IDC_EDT_ANGLE03, m_strAngle03);
	DDX_Text(pDX, IDC_EDT_ANGLE04, m_strAngle04);
	DDX_Text(pDX, IDC_EDT_ANGLE05, m_strAngle05);
	DDX_Text(pDX, IDC_EDT_ANGLE06, m_strAngle06);
	DDX_Text(pDX, IDC_EDT_ANGLE07, m_strAngle07);
	DDX_Text(pDX, IDC_EDT_ANGLE08, m_strAngle08);

	DDX_Text(pDX, IDC_EDT_LENGTH01, m_strLength01);
	DDX_Text(pDX, IDC_EDT_LENGTH02, m_strLength02);
	DDX_Text(pDX, IDC_EDT_LENGTH03, m_strLength03);
	DDX_Text(pDX, IDC_EDT_LENGTH04, m_strLength04);
	DDX_Text(pDX, IDC_EDT_LENGTH05, m_strLength05);
	DDX_Text(pDX, IDC_EDT_LENGTH06, m_strLength06);
	DDX_Text(pDX, IDC_EDT_LENGTH07, m_strLength07);
	DDX_Text(pDX, IDC_EDT_LENGTH08, m_strLength08);

	DDX_Control(pDX, IDC_LST_PREDEFINED, m_ctlPredefList);
}


BEGIN_MESSAGE_MAP(CMixerDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CMixerDlg::OnBnClickedOk)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_EDT_ANGLE01, &CMixerDlg::OnEnChangeEdtAngle01)
	ON_NOTIFY(LVN_HOTTRACK, IDC_LST_PREDEFINED, &CMixerDlg::OnLvnHotTrackLstPredefined)
	ON_EN_CHANGE(IDC_EDT_ANGLE02, &CMixerDlg::OnEnChangeEdtAngle02)
	ON_EN_CHANGE(IDC_EDT_ANGLE03, &CMixerDlg::OnEnChangeEdtAngle03)
	ON_EN_CHANGE(IDC_EDT_ANGLE04, &CMixerDlg::OnEnChangeEdtAngle04)
	ON_EN_CHANGE(IDC_EDT_ANGLE05, &CMixerDlg::OnEnChangeEdtAngle05)
	ON_EN_CHANGE(IDC_EDT_ANGLE06, &CMixerDlg::OnEnChangeEdtAngle06)
	ON_EN_CHANGE(IDC_EDT_ANGLE07, &CMixerDlg::OnEnChangeEdtAngle07)
	ON_EN_CHANGE(IDC_EDT_ANGLE08, &CMixerDlg::OnEnChangeEdtAngle08)

	ON_EN_CHANGE(IDC_EDT_LENGTH01, &CMixerDlg::OnEnChangeEdtLength01)
	ON_EN_CHANGE(IDC_EDT_LENGTH02, &CMixerDlg::OnEnChangeEdtLength02)
	ON_EN_CHANGE(IDC_EDT_LENGTH03, &CMixerDlg::OnEnChangeEdtLength03)
	ON_EN_CHANGE(IDC_EDT_LENGTH04, &CMixerDlg::OnEnChangeEdtLength04)
	ON_EN_CHANGE(IDC_EDT_LENGTH05, &CMixerDlg::OnEnChangeEdtLength05)
	ON_EN_CHANGE(IDC_EDT_LENGTH06, &CMixerDlg::OnEnChangeEdtLength06)
	ON_EN_CHANGE(IDC_EDT_LENGTH07, &CMixerDlg::OnEnChangeEdtLength07)
	ON_EN_CHANGE(IDC_EDT_LENGTH08, &CMixerDlg::OnEnChangeEdtLength08)

	ON_BN_CLICKED(IDC_BUT_SAVE, &CMixerDlg::OnBnClickedButSave)
	ON_BN_CLICKED(IDC_BUT_READ, &CMixerDlg::OnBnClickedButRead)
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
			m_stPredefConfig[y].fLenght[x] = 0;
			m_stPredefConfig[y].fAngle[x] = 0;
		}
	}

	//predefinionana konfiguracja dla 4X
	m_stPredefConfig[0].fAngle[0] = 45;
	m_stPredefConfig[0].fAngle[1] = 135;
	m_stPredefConfig[0].fAngle[2] = -135;
	m_stPredefConfig[0].fAngle[3] = -45;
	m_stPredefConfig[0].strName = _T("Quadrokopter X");
	
	//predefinionana konfiguracja dla 4+
	m_stPredefConfig[1].fAngle[0] = 0;
	m_stPredefConfig[1].fAngle[1] = 90;
	m_stPredefConfig[1].fAngle[2] = 180;
	m_stPredefConfig[1].fAngle[3] = -90;
	m_stPredefConfig[1].strName = _T("Quadrokopter +");
	
	for (x=0; x<4; x++)
	{
		m_stPredefConfig[0].fLenght[x] = 250;
		m_stPredefConfig[1].fLenght[x] = 250;
	}

	//predefinionana konfiguracja dla 6X
	m_stPredefConfig[2].fAngle[0] = 30;
	m_stPredefConfig[2].fAngle[1] = 90;
	m_stPredefConfig[2].fAngle[2] = 150;
	m_stPredefConfig[2].fAngle[3] = -150;
	m_stPredefConfig[2].fAngle[4] = -90;
	m_stPredefConfig[2].fAngle[5] = -30;
	m_stPredefConfig[2].strName = _T("Hexakopter X");
	for (x=0; x<6; x++)
		m_stPredefConfig[2].fLenght[x] = 300;

	//predefinionana konfiguracja dla 8X
	m_stPredefConfig[3].fAngle[0] = 22.5;
	m_stPredefConfig[3].fAngle[1] = 67.5;
	m_stPredefConfig[3].fAngle[2] = 112.5;
	m_stPredefConfig[3].fAngle[3] = 157.5;
	m_stPredefConfig[3].fAngle[4] = 202.5;
	m_stPredefConfig[3].fAngle[5] = 247.5;
	m_stPredefConfig[3].fAngle[6] = 292.5;
	m_stPredefConfig[3].fAngle[7] = 337.5;
	m_stPredefConfig[3].strName = _T("Oktokopter X");
	for (x=0; x<8; x++)
		m_stPredefConfig[3].fLenght[x] = 380;

	//dczytaj nastawy z APL
	OnBnClickedButRead();
	//UpdateAngleLen();	//wstaw nastawy do okna

	m_ctlPredefList.InsertColumn(0, _T("Configuration"));
	m_ctlPredefList.SetColumnWidth(0, 200);
	for (x=0; x<4; x++)
		m_ctlPredefList.InsertItem(x, m_stPredefConfig[x].strName);	

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
	m_strAngle01.Format(_T("%.1f"), m_fAngle[0]);
	m_strAngle02.Format(_T("%.1f"), m_fAngle[1]);
	m_strAngle03.Format(_T("%.1f"), m_fAngle[2]);
	m_strAngle04.Format(_T("%.1f"), m_fAngle[3]);
	m_strAngle05.Format(_T("%.1f"), m_fAngle[4]);
	m_strAngle06.Format(_T("%.1f"), m_fAngle[5]);
	m_strAngle07.Format(_T("%.1f"), m_fAngle[6]);
	m_strAngle08.Format(_T("%.1f"), m_fAngle[7]);


	m_strLength01.Format(_T("%.1f"), m_fLenght[0]);
	m_strLength02.Format(_T("%.1f"), m_fLenght[1]);
	m_strLength03.Format(_T("%.1f"), m_fLenght[2]);
	m_strLength04.Format(_T("%.1f"), m_fLenght[3]);
	m_strLength05.Format(_T("%.1f"), m_fLenght[4]);
	m_strLength06.Format(_T("%.1f"), m_fLenght[5]);
	m_strLength07.Format(_T("%.1f"), m_fLenght[6]);
	m_strLength08.Format(_T("%.1f"), m_fLenght[7]);

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
	float fMaxLen = m_fLenght[0];
	for (x=1; x< KANALY_MIKSERA; x++)
	{
		if (m_fLenght[x] > fMaxLen)
			fMaxLen = m_fLenght[x];		//najd³u¿sze ramiê
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
		if (m_fLenght[x])
		{
			ptEng.x = ptCP.x + int(sin(m_fAngle[x]* DEG2RAD)*m_fLenght[x]*fScaleXY);
			ptEng.y = ptCP.y - int(cos(m_fAngle[x]* DEG2RAD)*m_fLenght[x]*fScaleXY);
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
			m_fAngle[x] = m_stPredefConfig[nSel].fAngle[x];
			m_fLenght[x] = m_stPredefConfig[nSel].fLenght[x];
		}
		UpdateAngleLen();
		Invalidate();
		m_nActualSel = nSel;
	}
	UpdateData(FALSE);
}


void CMixerDlg::OnEnChangeEdtAngle01()
{
	UpdateData();
	m_fAngle[0] = (float)_wtof(m_strAngle01);
	Invalidate();
}


void CMixerDlg::OnEnChangeEdtAngle02()
{
	UpdateData();
	m_fAngle[1] = (float)_wtof(m_strAngle02);
	Invalidate();
}

void CMixerDlg::OnEnChangeEdtAngle03()
{
	UpdateData();
	m_fAngle[2] = (float)_wtof(m_strAngle03);
	Invalidate();
}

void CMixerDlg::OnEnChangeEdtAngle04()
{
	UpdateData();
	m_fAngle[3] = (float)_wtof(m_strAngle04);
	Invalidate();
}

void CMixerDlg::OnEnChangeEdtAngle05()
{
	UpdateData();
	m_fAngle[4] = (float)_wtof(m_strAngle05);
	Invalidate();
}

void CMixerDlg::OnEnChangeEdtAngle06()
{
	UpdateData();
	m_fAngle[5] = (float)_wtof(m_strAngle06);
	Invalidate();
}

void CMixerDlg::OnEnChangeEdtAngle07()
{
	UpdateData();
	m_fAngle[6] = (float)_wtof(m_strAngle07);
	Invalidate();
}

void CMixerDlg::OnEnChangeEdtAngle08()
{
	UpdateData();
	m_fAngle[7] = (float)_wtof(m_strAngle08);
	Invalidate();
}

void CMixerDlg::OnEnChangeEdtLength01()
{
	UpdateData();
	m_fLenght[0] = (float)_wtof(m_strLength01);
	Invalidate();
}

void CMixerDlg::OnEnChangeEdtLength02()
{
	UpdateData();
	m_fLenght[1] = (float)_wtof(m_strLength02);
	Invalidate();
}

void CMixerDlg::OnEnChangeEdtLength03()
{
	UpdateData();
	m_fLenght[2] = (float)_wtof(m_strLength03);
	Invalidate();
}

void CMixerDlg::OnEnChangeEdtLength04()
{
	UpdateData();
	m_fLenght[3] = (float)_wtof(m_strLength04);
	Invalidate();
}

void CMixerDlg::OnEnChangeEdtLength05()
{
	UpdateData();
	m_fLenght[4] = (float)_wtof(m_strLength05);
	Invalidate();
}

void CMixerDlg::OnEnChangeEdtLength06()
{
	UpdateData();
	m_fLenght[5] = (float)_wtof(m_strLength06);
	Invalidate();
}

void CMixerDlg::OnEnChangeEdtLength07()
{
	UpdateData();
	m_fLenght[6] = (float)_wtof(m_strLength07);
	Invalidate();
}

void CMixerDlg::OnEnChangeEdtLength08()
{
	UpdateData();
	m_fLenght[7] = (float)_wtof(m_strLength08);
	Invalidate();
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
		fMixPith[x] = (float)(m_fLenght[x] * cos(m_fAngle[x]* DEG2RAD));
		fMixRoll[x] = (float)(m_fLenght[x] * sin(m_fAngle[x]* DEG2RAD));
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
		nErr = getModemData().ZapiszFloat32Val2Fram(fMixPith[x], FAU_MIX_PITCH+4*x);
		nErr = getModemData().ZapiszFloat32Val2Fram(fMixRoll[x], FAU_MIX_ROLL+4*x);
	}
	//prze³aduj konfiguracjê
	getModemData().PrzeladujKonfigPID();
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
		nErr = getModemData().CzytajFram2Float32(&fMixPith[x], FAU_MIX_PITCH+4*x);
		if (nErr == IDABORT) return;
		nErr = getModemData().CzytajFram2Float32(&fMixRoll[x], FAU_MIX_ROLL+4*x);
		if (nErr == IDABORT) return;

		if (fMixPith[x])	//zabezpieczenie przed dzieleniem przez zero
		{
			m_fAngle[x] = (float)(DEG2RAD * atan2(fMixRoll[x],fMixPith[x]));
			m_fLenght[x] = (float)sqrt(fMixRoll[x]*fMixRoll[x] + fMixPith[x]*fMixPith[x]);
		}
		else
		{
			m_fAngle[x] = 0;
			m_fLenght[x] = 0;
		}
	}
	m_nActualSel = -1;	//wskaŸnik ostatnio wybranej przedefiniowanej pozycji
	UpdateAngleLen();
	Invalidate();
}
