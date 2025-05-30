// KonfigPID.cpp: plik implementacji
//

#include "pch.h"
#include "APL-SN.h"
#include "KonfigPID.h"
#include "afxdialogex.h"
#include <math.h>
#include "Errors.h"
#include "pid_kanaly.h"

// Okno dialogowe KonfigPID

IMPLEMENT_DYNAMIC(KonfigPID, CDialogEx)

KonfigPID::KonfigPID(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KONFIG_PID, pParent)
	, m_strTI2(_T(""))
	, m_strTI1(_T(""))
	, m_strKP1(_T(""))
	, m_strKP2(_T(""))
	, m_strFiltrD1(_T(""))
	, m_strFiltrD2(_T(""))
	, m_strTD1(_T(""))
	, m_strTD2(_T(""))
	, m_strOgrCalki1(_T(""))
	, m_strOgrCalki2(_T(""))
	, m_strTest(_T(""))
	, m_strMinWyj1(_T(""))
	, m_strMaxWyj1(_T(""))
	, m_strMinWyj2(_T(""))
	, m_strMaxWyj2(_T(""))
	, m_bKatowy1(FALSE)
	, m_bKatowy2(FALSE)
{

}

KonfigPID::~KonfigPID()
{
}

void KonfigPID::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_KANAL_PID, m_ctrlKanalPID);
	DDX_Text(pDX, IDC_EDIT_TI2, m_strTI2);
	DDX_Text(pDX, IDC_EDIT_TI1, m_strTI1);
	DDX_Text(pDX, IDC_EDIT_KP1, m_strKP1);
	DDX_Text(pDX, IDC_EDIT_KP2, m_strKP2);
	DDX_Text(pDX, IDC_EDIT_FILTRD1, m_strFiltrD1);
	DDX_Text(pDX, IDC_EDIT_FILTRD2, m_strFiltrD2);
	DDX_Text(pDX, IDC_EDIT_TD1, m_strTD1);
	DDX_Text(pDX, IDC_EDIT_TD2, m_strTD2);
	DDX_Text(pDX, IDC_EDIT_LIMIT_CALKI1, m_strOgrCalki1);
	DDX_Text(pDX, IDC_EDIT_LIMIT_CALKI2, m_strOgrCalki2);
	DDX_Text(pDX, IDC_EDIT_MIN_WY1, m_strMinWyj1);
	DDX_Text(pDX, IDC_EDIT_MAX_WY1, m_strMaxWyj1);
	DDX_Text(pDX, IDC_EDIT_MIN_WY2, m_strMinWyj2);
	DDX_Text(pDX, IDC_EDIT_MAX_WY2, m_strMaxWyj2);
	DDX_Check(pDX, IDC_CHECK_KATOWY1, m_bKatowy1);
	DDX_Check(pDX, IDC_CHECK_KATOWY2, m_bKatowy2);
}


BEGIN_MESSAGE_MAP(KonfigPID, CDialogEx)
	ON_BN_CLICKED(IDOK, &KonfigPID::OnBnClickedOk)
	//ON_WM_PAINT()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_KANAL_PID, &KonfigPID::OnTcnSelchangeTabKanalPid)
	ON_EN_CHANGE(IDC_EDIT_KP1, &KonfigPID::OnEnChangeEditKp1)
	ON_EN_CHANGE(IDC_EDIT_KP2, &KonfigPID::OnEnChangeEditKp2)
	ON_EN_CHANGE(IDC_EDIT_TI1, &KonfigPID::OnEnChangeEditTi1)
	ON_EN_CHANGE(IDC_EDIT_TI2, &KonfigPID::OnEnChangeEditTi2)
	ON_EN_CHANGE(IDC_EDIT_TD1, &KonfigPID::OnEnChangeEditTd1)
	ON_EN_CHANGE(IDC_EDIT_TD2, &KonfigPID::OnEnChangeEditTd2)
	ON_EN_CHANGE(IDC_EDIT_MIN_WY1, &KonfigPID::OnEnChangeEditMinWy1)
	ON_EN_CHANGE(IDC_EDIT_MAX_WY1, &KonfigPID::OnEnChangeEditMaxWy1)
	ON_EN_CHANGE(IDC_EDIT_MIN_WY2, &KonfigPID::OnEnChangeEditMinWy2)
	ON_EN_CHANGE(IDC_EDIT_MAX_WY2, &KonfigPID::OnEnChangeEditMaxWy2)
END_MESSAGE_MAP()


// Procedury obsługi komunikatów KonfigPID


///////////////////////////////////////////////////////////////////////////////////////////////////
// Inicjalizacja zawartości okna dialogowego
// Zwraca: TRUE
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL KonfigPID::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	float fDane[LICZBA_ZMIENNYCH_FLOAT_REG_PID];
	uint8_t chErr, chLicznikProb;

	// TODO:  Dodaj tutaj dodatkową inicjację
	m_ctrlKanalPID.InsertItem(0, _T("Przechylenie"));
	m_ctrlKanalPID.InsertItem(1, _T("Pochylenie"));
	m_ctrlKanalPID.InsertItem(2, _T("Odchylenie"));
	m_ctrlKanalPID.InsertItem(3, _T("Wysokość"));
	m_nBiezacyRegulator = m_ctrlKanalPID.GetCurSel();

	for (int n = 0; n < LICZBA_REGULATOROW_PID; n++)
	{
		chErr = getKomunikacja().InicjujOdczytFloatFRAM(LICZBA_ZMIENNYCH_FLOAT_REG_PID, FAU_PID_P0 + n * LICZBA_ZMIENNYCH_FLOAT_REG_PID);
		chLicznikProb = 5;
		do {
			chErr = getKomunikacja().CzytajDaneFloatFRAM(fDane, LICZBA_ZMIENNYCH_FLOAT_REG_PID);
			chLicznikProb--;
		} while ((chErr == 2) && chLicznikProb);	//kod błędu ERR_HAL_BUSY mówiący że dane jeszcze nie są gotowe
		m_stPID[n].fKp = fDane[0];			// FA_USER_PID+0   //4U wzmocnienienie członu P regulatora 0
		m_stPID[n].fTi = fDane[1];			//FA_USER_PID+4   //4U wzmocnienienie członu I regulatora 0
		m_stPID[n].fTd = fDane[2];			//FA_USER_PID+8   //4U wzmocnienienie członu D regulatora 0
		m_stPID[n].fOgrCalki = fDane[3];	//FA_USER_PID+12  //4U górna granica wartości całki członu I regulatora 0
		m_stPID[n].fMinWyj = fDane[4];		//FA_USER_PID+16  //4U 
		m_stPID[n].fMaxWyj = fDane[5];		//FA_USER_PID+20  //4U 
		m_stPID[n].bZmieniony = FALSE;
	}
	
	UstawKontrolki();

	//m_strFiltrD1;
	//m_strFiltrD2;

	UpdateData(FALSE);



	return TRUE;  // return TRUE unless you set the focus to a control
				  // WYJĄTEK: Strona właściwości OCX powinna zwrócić FALSE
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Wstawia zawartość do pól edycyjnych
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::UstawKontrolki()
{
	m_strKP1.Format(_T("%.4f"), m_stPID[2 * m_nBiezacyRegulator + 0].fKp);
	m_strKP1.Replace(_T('.'), _T(','));
	m_strKP2.Format(_T("%.4f"), m_stPID[2 * m_nBiezacyRegulator + 1].fKp);
	m_strKP2.Replace(_T('.'), _T(','));

	m_strTI1.Format(_T("%.4f"), m_stPID[2 * m_nBiezacyRegulator + 0].fTi);
	m_strTD1.Replace(_T('.'), _T(','));
	m_strTI2.Format(_T("%.4f"), m_stPID[2 * m_nBiezacyRegulator + 1].fTi);
	m_strTD1.Replace(_T('.'), _T(','));

	m_strTD1.Format(_T("%.4f"), m_stPID[2 * m_nBiezacyRegulator + 0].fTd);
	m_strTD1.Replace(_T('.'), _T(','));
	m_strTD2.Format(_T("%.4f"), m_stPID[2 * m_nBiezacyRegulator + 1].fTd);
	m_strTD1.Replace(_T('.'), _T(','));

	m_strOgrCalki1.Format(_T("%.4f"), m_stPID[2 * m_nBiezacyRegulator + 0].fOgrCalki);
	m_strTD1.Replace(_T('.'), _T(','));
	m_strOgrCalki2.Format(_T("%.4f"), m_stPID[2 * m_nBiezacyRegulator + 1].fOgrCalki);
	m_strTD1.Replace(_T('.'), _T(','));

	m_strMinWyj1.Format(_T("%.4f"), m_stPID[2 * m_nBiezacyRegulator + 0].fMinWyj);
	m_strMinWyj1.Replace(_T('.'), _T(','));
	m_strMinWyj2.Format(_T("%.4f"), m_stPID[2 * m_nBiezacyRegulator + 1].fMinWyj);
	m_strMinWyj2.Replace(_T('.'), _T(','));

	m_strMaxWyj1.Format(_T("%.4f"), m_stPID[2 * m_nBiezacyRegulator + 0].fMaxWyj);
	m_strMaxWyj1.Replace(_T('.'), _T(','));
	m_strMaxWyj2.Format(_T("%.4f"), m_stPID[2 * m_nBiezacyRegulator + 1].fMaxWyj);
	m_strMaxWyj2.Replace(_T('.'), _T(','));

	m_bKatowy1 = m_stPID[2 * m_nBiezacyRegulator + 0].chPodstFiltraD & 0x80;
	m_bKatowy2 = m_stPID[2 * m_nBiezacyRegulator + 1].chPodstFiltraD & 0x80;

	m_strFiltrD1.Format(_T("%d"), m_stPID[2 * m_nBiezacyRegulator + 0].chPodstFiltraD & 0x7F);
	m_strFiltrD2.Format(_T("%d"), m_stPID[2 * m_nBiezacyRegulator + 1].chPodstFiltraD & 0x7F);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Zamknięcie okna przyciskiem OK
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnBnClickedOk()
{
	float fDane[LICZBA_ZMIENNYCH_FLOAT_REG_PID];
	uint8_t chErr, chLicznikProb;
	CString strLiczba;

	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
	for (int n = 0; n < LICZBA_REGULATOROW_PID; n++)
	{
		fDane[0] = m_stPID[n].fKp;			// FA_USER_PID+0   //4U wzmocnienienie członu P regulatora 0
		fDane[1] = m_stPID[n].fTi;			//FA_USER_PID+4   //4U wzmocnienienie członu I regulatora 0
		fDane[2] = m_stPID[n].fTd;			//FA_USER_PID+8   //4U wzmocnienienie członu D regulatora 0
		fDane[3] = m_stPID[n].fOgrCalki;	//FA_USER_PID+12  //4U górna granica wartości całki członu I regulatora 0
		fDane[4] = m_stPID[n].fMinWyj;
		fDane[5] = m_stPID[n].fMaxWyj;
		chErr = getKomunikacja().ZapiszDaneFloatFRAM(fDane, LICZBA_ZMIENNYCH_FLOAT_REG_PID, FAU_PID_P0 + n * LICZBA_ZMIENNYCH_FLOAT_REG_PID);
	}

	strLiczba = m_strKP1.GetString();
	_ttof(strLiczba);
		

	
	CDialogEx::OnOK();
}


/*

void KonfigPID::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Dodaj tutaj swój kod procedury obsługi komunikatów
					   // Nie wywołuj CDialogEx::OnPaint() do malowania komunikatów

	dc.MoveTo(10, 10);
	dc.LineTo(40, 40);

	
} */


///////////////////////////////////////////////////////////////////////////////////////////////////
// Kliknięto na zakłądke zmany kanału regulatorów
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnTcnSelchangeTabKanalPid(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
	UpdateData(TRUE);
	m_nBiezacyRegulator = m_ctrlKanalPID.GetCurSel();
	m_strTest.Format(_T("Indeks kanału: %d"), m_nBiezacyRegulator);
	UstawKontrolki();
	UpdateData(FALSE);
	*pResult = 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Edycja kontrolki wzmocnienia regulatora pochodnej
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnEnChangeEditKp1()
{
	UpdateData(TRUE);
	CString strLiczba = m_strKP1.GetString();
	strLiczba.Replace(_T(','), _T('.'));
	m_stPID[2 * m_nBiezacyRegulator + 0].fKp = (float)_wtof(strLiczba);
	m_stPID[2 * m_nBiezacyRegulator + 0].bZmieniony = TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Edycja kontrolki wzmocnienia regulatora kąta
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnEnChangeEditKp2()
{
	UpdateData(TRUE);
	CString strLiczba = m_strKP2.GetString();
	strLiczba.Replace(_T(','), _T('.'));
	m_stPID[2 * m_nBiezacyRegulator + 1].fKp = (float)_wtof(strLiczba);
	m_stPID[2 * m_nBiezacyRegulator + 1].bZmieniony = TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Edycja kontrolki Ti regulatora pochodnej
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnEnChangeEditTi1()
{
	UpdateData(TRUE);
	CString strLiczba = m_strTI1.GetString();
	strLiczba.Replace(_T(','), _T('.'));
	m_stPID[2 * m_nBiezacyRegulator + 0].fTi = (float)_wtof(strLiczba);
	m_stPID[2 * m_nBiezacyRegulator + 0].bZmieniony = TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Edycja kontrolki Ti regulatora kąta
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnEnChangeEditTi2()
{
	UpdateData(TRUE);
	CString strLiczba = m_strTI2.GetString();
	strLiczba.Replace(_T(','), _T('.'));
	m_stPID[2 * m_nBiezacyRegulator + 1].fTi = (float)_wtof(strLiczba);
	m_stPID[2 * m_nBiezacyRegulator + 1].bZmieniony = TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Edycja kontrolki Td regulatora pochodnej
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnEnChangeEditTd1()
{
	UpdateData(TRUE);
	CString strLiczba = m_strTD1.GetString();
	strLiczba.Replace(_T(','), _T('.'));
	m_stPID[2 * m_nBiezacyRegulator + 0].fTd = (float)_wtof(strLiczba);
	m_stPID[2 * m_nBiezacyRegulator + 0].bZmieniony = TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Edycja kontrolki Td regulatora kąta
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnEnChangeEditTd2()
{
	UpdateData(TRUE);
	CString strLiczba = m_strTD2.GetString();
	strLiczba.Replace(_T(','), _T('.'));
	m_stPID[2 * m_nBiezacyRegulator + 1].fTd = (float)_wtof(strLiczba);
	m_stPID[2 * m_nBiezacyRegulator + 1].bZmieniony = TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Edycja kontrolki minimum wyjścia regulatora pochodnej
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnEnChangeEditMinWy1()
{
	UpdateData(TRUE);
	CString strLiczba = m_strMinWyj1.GetString();
	strLiczba.Replace(_T(','), _T('.'));
	m_stPID[2 * m_nBiezacyRegulator + 0].fMinWyj = (float)_wtof(strLiczba);
	m_stPID[2 * m_nBiezacyRegulator + 0].bZmieniony = TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Edycja kontrolki maksimum wyjścia regulatora pochodnej
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnEnChangeEditMaxWy1()
{
	UpdateData(TRUE);
	CString strLiczba = m_strMaxWyj1.GetString();
	strLiczba.Replace(_T(','), _T('.'));
	m_stPID[2 * m_nBiezacyRegulator + 0].fMaxWyj = (float)_wtof(strLiczba);
	m_stPID[2 * m_nBiezacyRegulator + 0].bZmieniony = TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Edycja kontrolki minimum wyjścia regulatora kąta
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnEnChangeEditMinWy2()
{
	UpdateData(TRUE);
	CString strLiczba = m_strMinWyj2.GetString();
	strLiczba.Replace(_T(','), _T('.'));
	m_stPID[2 * m_nBiezacyRegulator + 1].fMinWyj = (float)_wtof(strLiczba);
	m_stPID[2 * m_nBiezacyRegulator + 1].bZmieniony = TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Edycja kontrolki maksimum wyjścia regulatora kąta
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnEnChangeEditMaxWy2()
{
	UpdateData(TRUE);
	CString strLiczba = m_strMaxWyj2.GetString();
	strLiczba.Replace(_T(','), _T('.'));
	m_stPID[2 * m_nBiezacyRegulator + 1].fMaxWyj = (float)_wtof(strLiczba);
	m_stPID[2 * m_nBiezacyRegulator + 1].bZmieniony = TRUE;
}
