// KonfigPID.cpp: plik implementacji
//

#include "pch.h"
#include "APL-SN.h"
#include "KonfigPID.h"
#include "afxdialogex.h"
#include <math.h>
#include "Errors.h"

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
	, m_strLimitCalki1(_T(""))
	, m_strLimitCalki2(_T(""))
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
	DDX_Text(pDX, IDC_EDIT_LIMIT_CALKI1, m_strLimitCalki1);
	DDX_Text(pDX, IDC_EDIT_LIMIT_CALKI2, m_strLimitCalki2);
}


BEGIN_MESSAGE_MAP(KonfigPID, CDialogEx)
	ON_BN_CLICKED(IDOK, &KonfigPID::OnBnClickedOk)
	ON_WM_PAINT()
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

	for (int n = 0; n < LICZBA_REGULATOROW_PID; n++)
	{
		getKomunikacja().InicjujOdczytFloatFRAM(6, FAU_PID_P0 + n * LICZBA_ZMIENNYCH_FLOAT_REG_PID);
		chLicznikProb = 5;
		do {
			chErr = getKomunikacja().CzytajDaneFloatFRAM(fDane, LICZBA_ZMIENNYCH_FLOAT_REG_PID);
			chLicznikProb--;
		} while ((chErr == 2) && chLicznikProb);	//kod błędu ERR_HAL_BUSY mówiący że dane jeszcze nie są gotowe
		m_stPID[n].fKp = fDane[0];			// FA_USER_PID+0   //4U wzmocnienienie członu P regulatora 0
		m_stPID[n].fTi = fDane[1];			//FA_USER_PID+4   //4U wzmocnienienie członu I regulatora 0
		m_stPID[n].fTd = fDane[2];			//FA_USER_PID+8   //4U wzmocnienienie członu D regulatora 0
		m_stPID[n].fLimitCalki = fDane[3];	//FA_USER_PID+12  //4U górna granica wartości całki członu I regulatora 0
	}

	m_nBiezacyRegulator = m_ctrlKanalPID.GetCurSel();

	//ustaw kontrolki
	m_strKP1.Format(_T("%.4f"), m_stPID[m_nBiezacyRegulator].fKp);
	m_strKP2.Format(_T("%.4f"), m_stPID[m_nBiezacyRegulator + LICZBA_REGULATOROW_PID/2].fKp);
	
	m_strTI1.Format(_T("%.4f"), m_stPID[m_nBiezacyRegulator].fTi);
	m_strTI2.Format(_T("%.4f"), m_stPID[m_nBiezacyRegulator + LICZBA_REGULATOROW_PID/2].fTi);

	m_strTD1.Format(_T("%.4f"), m_stPID[m_nBiezacyRegulator].fTd);
	m_strTD2.Format(_T("%.4f"), m_stPID[m_nBiezacyRegulator + LICZBA_REGULATOROW_PID / 2].fTd);

	m_strLimitCalki1.Format(_T("%.4f"), m_stPID[m_nBiezacyRegulator].fLimitCalki);
	m_strLimitCalki2.Format(_T("%.4f"), m_stPID[m_nBiezacyRegulator + LICZBA_REGULATOROW_PID / 2].fLimitCalki);

	//m_strFiltrD1;
	//m_strFiltrD2;

	UpdateData(FALSE);



	return TRUE;  // return TRUE unless you set the focus to a control
				  // WYJĄTEK: Strona właściwości OCX powinna zwrócić FALSE
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Zamknięcie okna przyciskiem OK
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnBnClickedOk()
{
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
	CDialogEx::OnOK();
}




void KonfigPID::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Dodaj tutaj swój kod procedury obsługi komunikatów
					   // Nie wywołuj CDialogEx::OnPaint() do malowania komunikatów

	dc.MoveTo(10, 10);
	dc.LineTo(40, 40);

	
}

