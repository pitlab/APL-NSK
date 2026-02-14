// OdbiornikiRC.cpp: plik implementacji
//

#include "pch.h"
#include "APL-SN.h"
#include "OdbiornikiRC.h"
#include "afxdialogex.h"
#include "sys_def_wspolny.h"
#include "konfig_fram.h"
#include "Errors.h"
#include "pid_kanaly.h"


// Okno dialogowe OdbiornikiRC

IMPLEMENT_DYNAMIC(OdbiornikiRC, CDialogEx)

OdbiornikiRC::OdbiornikiRC(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ODBIORNIKI_RC, pParent)
	, m_bZmienionoUstawienie(FALSE)
	, m_bZmienionoKanalDrazkow(FALSE)
	, m_bZmienionoMinMax(FALSE)
	, m_bZmienionoFunkcjeKanalow(FALSE)
	, m_bZmienionoFunkcjeWyjscRC(FALSE)
{

}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Destruktor
///////////////////////////////////////////////////////////////////////////////////////////////////
OdbiornikiRC::~OdbiornikiRC()
{
	if (m_bZmodyfikowanoTelemetrie)
	{
		getKomunikacja().ZapiszOkresTelemetrii(m_sBackupOkresuTelemetrii, LICZBA_ZMIENNYCH_TELEMETRYCZNYCH);		//przywróć telemetrię z backupu jeżeli była zmieniana
	}
}



void OdbiornikiRC::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_PROGRESS1, m_ctlRC1Kan1);
	DDX_Control(pDX, IDC_PROGRESS2, m_ctlRC1Kan2);
	DDX_Control(pDX, IDC_PROGRESS3, m_ctlRC1Kan3);
	DDX_Control(pDX, IDC_PROGRESS4, m_ctlRC1Kan4);
	DDX_Control(pDX, IDC_PROGRESS5, m_ctlRC1Kan5);
	DDX_Control(pDX, IDC_PROGRESS6, m_ctlRC1Kan6);
	DDX_Control(pDX, IDC_PROGRESS7, m_ctlRC1Kan7);
	DDX_Control(pDX, IDC_PROGRESS8, m_ctlRC1Kan8);
	DDX_Control(pDX, IDC_PROGRESS9, m_ctlRC1Kan9);
	DDX_Control(pDX, IDC_PROGRESS10, m_ctlRC1Kan10);
	DDX_Control(pDX, IDC_PROGRESS11, m_ctlRC1Kan11);
	DDX_Control(pDX, IDC_PROGRESS12, m_ctlRC1Kan12);
	DDX_Control(pDX, IDC_PROGRESS13, m_ctlRC1Kan13);
	DDX_Control(pDX, IDC_PROGRESS14, m_ctlRC1Kan14);
	DDX_Control(pDX, IDC_PROGRESS15, m_ctlRC1Kan15);
	DDX_Control(pDX, IDC_PROGRESS16, m_ctlRC1Kan16);
	DDX_Control(pDX, IDC_PROGRESS33, m_ctlSerwo1);
	DDX_Control(pDX, IDC_PROGRESS34, m_ctlSerwo2);
	DDX_Control(pDX, IDC_PROGRESS35, m_ctlSerwo3);
	DDX_Control(pDX, IDC_PROGRESS36, m_ctlSerwo4);
	DDX_Control(pDX, IDC_PROGRESS37, m_ctlSerwo5);
	DDX_Control(pDX, IDC_PROGRESS38, m_ctlSerwo6);
	DDX_Control(pDX, IDC_PROGRESS39, m_ctlSerwo7);
	DDX_Control(pDX, IDC_PROGRESS40, m_ctlSerwo8);
	DDX_Control(pDX, IDC_PROGRESS41, m_ctlSerwo9);
	DDX_Control(pDX, IDC_PROGRESS42, m_ctlSerwo10);
	DDX_Control(pDX, IDC_PROGRESS43, m_ctlSerwo11);
	DDX_Control(pDX, IDC_PROGRESS44, m_ctlSerwo12);
	DDX_Control(pDX, IDC_PROGRESS45, m_ctlSerwo13);
	DDX_Control(pDX, IDC_PROGRESS46, m_ctlSerwo14);
	DDX_Control(pDX, IDC_PROGRESS47, m_ctlSerwo15);
	DDX_Control(pDX, IDC_PROGRESS48, m_ctlSerwo16);
	DDX_Control(pDX, IDC_COMBO_SERWO1, m_ctlTypWyjscia1);
	DDX_Control(pDX, IDC_COMBO_SERWO2, m_ctlTypWyjscia2);
	DDX_Control(pDX, IDC_COMBO_SERWO3, m_ctlTypWyjscia3);
	DDX_Control(pDX, IDC_COMBO_SERWO4, m_ctlTypWyjscia4);
	DDX_Control(pDX, IDC_COMBO_SERWO5, m_ctlTypWyjscia5);
	DDX_Control(pDX, IDC_COMBO_SERWO6, m_ctlTypWyjscia6);
	DDX_Control(pDX, IDC_COMBO_SERWO7, m_ctlTypWyjscia7);
	DDX_Control(pDX, IDC_COMBO_SERWO8, m_ctlTypWyjscia8);
	DDX_Control(pDX, IDC_COMBO_SERWO9, m_ctlTypWyjscia9_16);
	DDX_Control(pDX, IDC_COMBO_RC1, m_ctlOdbiornikRC1);
	DDX_Control(pDX, IDC_COMBO_RC2, m_ctlOdbiornikRC2);

	DDX_Text(pDX, IDC_STAT_MIN_MAX01, m_stEkstrema[0].strMinMax);
	DDX_Text(pDX, IDC_STAT_MIN_MAX02, m_stEkstrema[1].strMinMax);
	DDX_Text(pDX, IDC_STAT_MIN_MAX03, m_stEkstrema[2].strMinMax);
	DDX_Text(pDX, IDC_STAT_MIN_MAX04, m_stEkstrema[3].strMinMax);
	DDX_Text(pDX, IDC_STAT_MIN_MAX05, m_stEkstrema[4].strMinMax);
	DDX_Text(pDX, IDC_STAT_MIN_MAX06, m_stEkstrema[5].strMinMax);
	DDX_Text(pDX, IDC_STAT_MIN_MAX07, m_stEkstrema[6].strMinMax);
	DDX_Text(pDX, IDC_STAT_MIN_MAX08, m_stEkstrema[7].strMinMax);
	DDX_Text(pDX, IDC_STAT_MIN_MAX09, m_stEkstrema[8].strMinMax);
	DDX_Text(pDX, IDC_STAT_MIN_MAX10, m_stEkstrema[9].strMinMax);
	DDX_Text(pDX, IDC_STAT_MIN_MAX11, m_stEkstrema[10].strMinMax);
	DDX_Text(pDX, IDC_STAT_MIN_MAX12, m_stEkstrema[11].strMinMax);
	DDX_Text(pDX, IDC_STAT_MIN_MAX13, m_stEkstrema[12].strMinMax);
	DDX_Text(pDX, IDC_STAT_MIN_MAX14, m_stEkstrema[13].strMinMax);
	DDX_Text(pDX, IDC_STAT_MIN_MAX15, m_stEkstrema[14].strMinMax);
	DDX_Text(pDX, IDC_STAT_MIN_MAX16, m_stEkstrema[15].strMinMax);
	DDX_Control(pDX, IDC_COMBO_KAN_PRZE, m_ctlKanalPrzechylenia);
	DDX_Control(pDX, IDC_COMBO_KAN_POCH1, m_ctlKanalPochylenia);
	DDX_Control(pDX, IDC_COMBO_KAN_ODCH, m_ctlKanalOdchylenia);
	DDX_Control(pDX, IDC_COMBO_KAN_WYSO, m_ctlKanalWysokosci);
	DDX_Control(pDX, IDC_COMBO_KAN5, m_ctlFunkcjaKanalu5);
	DDX_Control(pDX, IDC_COMBO_KAN6, m_ctlFunkcjaKanalu6);
	DDX_Control(pDX, IDC_COMBO_KAN7, m_ctlFunkcjaKanalu7);
	DDX_Control(pDX, IDC_COMBO_KAN8, m_ctlFunkcjaKanalu8);
	DDX_Control(pDX, IDC_COMBO_KAN9, m_ctlFunkcjaKanalu9);
	DDX_Control(pDX, IDC_COMBO_KAN10, m_ctlFunkcjaKanalu10);
	DDX_Control(pDX, IDC_COMBO_KAN11, m_ctlFunkcjaKanalu11);
	DDX_Control(pDX, IDC_COMBO_KAN12, m_ctlFunkcjaKanalu12);
	DDX_Control(pDX, IDC_COMBO_KAN13, m_ctlFunkcjaKanalu13);
	DDX_Control(pDX, IDC_COMBO_KAN14, m_ctlFunkcjaKanalu14);
	DDX_Control(pDX, IDC_COMBO_KAN15, m_ctlFunkcjaKanalu15);
	DDX_Control(pDX, IDC_COMBO_KAN16, m_ctlFunkcjaKanalu16);
	DDX_Control(pDX, IDC_COMBO_FUNKCJA_SERWA1, m_ctlFunkcjaWyjscia1);
	DDX_Control(pDX, IDC_COMBO_FUNKCJA_SERWA2, m_ctlFunkcjaWyjscia2);
	DDX_Control(pDX, IDC_COMBO_FUNKCJA_SERWA3, m_ctlFunkcjaWyjscia3);
	DDX_Control(pDX, IDC_COMBO_FUNKCJA_SERWA4, m_ctlFunkcjaWyjscia4);
	DDX_Control(pDX, IDC_COMBO_FUNKCJA_SERWA5, m_ctlFunkcjaWyjscia5);
	DDX_Control(pDX, IDC_COMBO_FUNKCJA_SERWA6, m_ctlFunkcjaWyjscia6);
	DDX_Control(pDX, IDC_COMBO_FUNKCJA_SERWA7, m_ctlFunkcjaWyjscia7);
	DDX_Control(pDX, IDC_COMBO_FUNKCJA_SERWA8, m_ctlFunkcjaWyjscia8);
	DDX_Control(pDX, IDC_COMBO_FUNKCJA_SERWA9, m_ctlFunkcjaWyjscia9);
	DDX_Control(pDX, IDC_COMBO_FUNKCJA_SERWA10, m_ctlFunkcjaWyjscia10);
	DDX_Control(pDX, IDC_COMBO_FUNKCJA_SERWA11, m_ctlFunkcjaWyjscia11);
	DDX_Control(pDX, IDC_COMBO_FUNKCJA_SERWA12, m_ctlFunkcjaWyjscia12);
	DDX_Control(pDX, IDC_COMBO_FUNKCJA_SERWA13, m_ctlFunkcjaWyjscia13);
	DDX_Control(pDX, IDC_COMBO_FUNKCJA_SERWA14, m_ctlFunkcjaWyjscia14);
	DDX_Control(pDX, IDC_COMBO_FUNKCJA_SERWA15, m_ctlFunkcjaWyjscia15);
	DDX_Control(pDX, IDC_COMBO_FUNKCJA_SERWA16, m_ctlFunkcjaWyjscia16);
}


BEGIN_MESSAGE_MAP(OdbiornikiRC, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_RC1, &OdbiornikiRC::OnCbnSelchangeComboRc1)
	ON_CBN_SELCHANGE(IDC_COMBO_RC2, &OdbiornikiRC::OnCbnSelchangeComboRc2)
	ON_CBN_SELCHANGE(IDC_COMBO_SERWO1, &OdbiornikiRC::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO_SERWO2, &OdbiornikiRC::OnCbnSelchangeCombo2)
	ON_CBN_SELCHANGE(IDC_COMBO_SERWO3, &OdbiornikiRC::OnCbnSelchangeCombo3)
	ON_CBN_SELCHANGE(IDC_COMBO_SERWO4, &OdbiornikiRC::OnCbnSelchangeCombo4)
	ON_CBN_SELCHANGE(IDC_COMBO_SERWO5, &OdbiornikiRC::OnCbnSelchangeCombo5)
	ON_CBN_SELCHANGE(IDC_COMBO_SERWO6, &OdbiornikiRC::OnCbnSelchangeCombo6)
	ON_CBN_SELCHANGE(IDC_COMBO_SERWO7, &OdbiornikiRC::OnCbnSelchangeCombo7)
	ON_CBN_SELCHANGE(IDC_COMBO_SERWO8, &OdbiornikiRC::OnCbnSelchangeCombo8)
	ON_CBN_SELCHANGE(IDC_COMBO_SERWO9, &OdbiornikiRC::OnCbnSelchangeCombo9)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &OdbiornikiRC::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &OdbiornikiRC::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_KAN_PRZE, &OdbiornikiRC::OnCbnSelchangeComboKanPrze)
	ON_CBN_SELCHANGE(IDC_COMBO_KAN_POCH1, &OdbiornikiRC::OnCbnSelchangeComboKanPoch1)
	ON_CBN_SELCHANGE(IDC_COMBO_KAN_ODCH, &OdbiornikiRC::OnCbnSelchangeComboKanOdch)
	ON_CBN_SELCHANGE(IDC_COMBO_KAN_WYSO, &OdbiornikiRC::OnCbnSelchangeComboKanWyso)
	ON_CBN_SELCHANGE(IDC_COMBO_KAN5, &OdbiornikiRC::OnCbnSelchangeComboKan5)
	ON_CBN_SELCHANGE(IDC_COMBO_KAN6, &OdbiornikiRC::OnCbnSelchangeComboKan6)
	ON_CBN_SELCHANGE(IDC_COMBO_KAN7, &OdbiornikiRC::OnCbnSelchangeComboKan7)
	ON_CBN_SELCHANGE(IDC_COMBO_KAN8, &OdbiornikiRC::OnCbnSelchangeComboKan8)
	ON_CBN_SELCHANGE(IDC_COMBO_KAN9, &OdbiornikiRC::OnCbnSelchangeComboKan9)
	ON_CBN_SELCHANGE(IDC_COMBO_KAN10, &OdbiornikiRC::OnCbnSelchangeComboKan10)
	ON_CBN_SELCHANGE(IDC_COMBO_KAN11, &OdbiornikiRC::OnCbnSelchangeComboKan11)
	ON_CBN_SELCHANGE(IDC_COMBO_KAN12, &OdbiornikiRC::OnCbnSelchangeComboKan12)
	ON_CBN_SELCHANGE(IDC_COMBO_KAN13, &OdbiornikiRC::OnCbnSelchangeComboKan13)
	ON_CBN_SELCHANGE(IDC_COMBO_KAN14, &OdbiornikiRC::OnCbnSelchangeComboKan14)
	ON_CBN_SELCHANGE(IDC_COMBO_KAN15, &OdbiornikiRC::OnCbnSelchangeComboKan15)
	ON_CBN_SELCHANGE(IDC_COMBO_KAN16, &OdbiornikiRC::OnCbnSelchangeComboKan16)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNKCJA_SERWA1, &OdbiornikiRC::OnCbnSelchangeComboFunkcjaSerwa1)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNKCJA_SERWA2, &OdbiornikiRC::OnCbnSelchangeComboFunkcjaSerwa2)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNKCJA_SERWA3, &OdbiornikiRC::OnCbnSelchangeComboFunkcjaSerwa3)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNKCJA_SERWA4, &OdbiornikiRC::OnCbnSelchangeComboFunkcjaSerwa4)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNKCJA_SERWA5, &OdbiornikiRC::OnCbnSelchangeComboFunkcjaSerwa5)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNKCJA_SERWA6, &OdbiornikiRC::OnCbnSelchangeComboFunkcjaSerwa6)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNKCJA_SERWA7, &OdbiornikiRC::OnCbnSelchangeComboFunkcjaSerwa7)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNKCJA_SERWA8, &OdbiornikiRC::OnCbnSelchangeComboFunkcjaSerwa8)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNKCJA_SERWA9, &OdbiornikiRC::OnCbnSelchangeComboFunkcjaSerwa9)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNKCJA_SERWA10, &OdbiornikiRC::OnCbnSelchangeComboFunkcjaSerwa10)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNKCJA_SERWA11, &OdbiornikiRC::OnCbnSelchangeComboFunkcjaSerwa11)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNKCJA_SERWA12, &OdbiornikiRC::OnCbnSelchangeComboFunkcjaSerwa12)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNKCJA_SERWA13, &OdbiornikiRC::OnCbnSelchangeComboFunkcjaSerwa13)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNKCJA_SERWA14, &OdbiornikiRC::OnCbnSelchangeComboFunkcjaSerwa14)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNKCJA_SERWA15, &OdbiornikiRC::OnCbnSelchangeComboFunkcjaSerwa15)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNKCJA_SERWA16, &OdbiornikiRC::OnCbnSelchangeComboFunkcjaSerwa16)
END_MESSAGE_MAP()


// Procedury obsługi komunikatów OdbiornikiRC

///////////////////////////////////////////////////////////////////////////////////////////////////
// Inicjalizacja kontrolek okna
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL OdbiornikiRC::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	uint8_t chErr, chDane[KANALY_WYJSC_RC];	//dane są odczytwane czwórkami, więc trzeba zarezerwowac na odczyt minimum 4 bajty
	int nIndeksDronaWRoju;
	uint16_t sOkresTelemetrii[LICZBA_ZMIENNYCH_TELEMETRYCZNYCH];
	CString strNapis;

	//Odczytaj z roju liste telemetrii i zachowaj ją jako backup do odtworzenia przy wyjściu
	if (getKomunikacja().m_cRoj.vWron.size())
	{
		nIndeksDronaWRoju = getKomunikacja().m_cRoj.m_nIndeksWrona;
		for (int n = 0; n < LICZBA_ZMIENNYCH_TELEMETRYCZNYCH; n++)
		{
			m_sBackupOkresuTelemetrii[n] = getKomunikacja().m_cRoj.vWron[nIndeksDronaWRoju].m_sOkresTelemetrii[n];
		}

		//ustaw telemetrię wejść i wyjść RC na CZESTOTLIWOSC_ODSWIEZANIA [Hz], resztę wyłącz
		for (int n = 0; n < LICZBA_ZMIENNYCH_TELEMETRYCZNYCH; n++)
		{
			if ((n >= TELEID_RC_KAN1) && (n <= TELEID_SERWO16))
				sOkresTelemetrii[n] = (uint16_t)(MAX_CZESTOTLIWOSC_TELEMETRII / CZESTOTLIWOSC_ODSWIEZANIA);
			else
				sOkresTelemetrii[n] = TEMETETRIA_WYLACZONA;
		}	

		//zapisz do APL telemetrię wysyłająca kanały odbiorników i serwa
		chErr = getKomunikacja().ZapiszOkresTelemetrii(sOkresTelemetrii, LICZBA_ZMIENNYCH_TELEMETRYCZNYCH);
		if (chErr)
		{
			CString strKomunikat;
			strKomunikat.Format(_T("Nie można rekonfigurować telemetrii! \nKod błędu: %d"), chErr);
			MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojojoj!"), MB_ICONEXCLAMATION, 0);
			CDialogEx::OnCancel();
			return FALSE;
		}
		else
			m_bZmodyfikowanoTelemetrie = TRUE;

		//włącz zbieranie ekstremów RC w CM4. W aplikacji dane są tylko w celach poglądowych
		chErr = getKomunikacja().ZbierajEkstremaWejscRC();
	}

	//definiuj zakres kanalów
	m_ctlRC1Kan1.SetRange(PPM_MIN, PPM_MAX);
	m_ctlRC1Kan2.SetRange(PPM_MIN, PPM_MAX);
	m_ctlRC1Kan3.SetRange(PPM_MIN, PPM_MAX);
	m_ctlRC1Kan4.SetRange(PPM_MIN, PPM_MAX);
	m_ctlRC1Kan5.SetRange(PPM_MIN, PPM_MAX);
	m_ctlRC1Kan6.SetRange(PPM_MIN, PPM_MAX);
	m_ctlRC1Kan7.SetRange(PPM_MIN, PPM_MAX);
	m_ctlRC1Kan8.SetRange(PPM_MIN, PPM_MAX);
	m_ctlRC1Kan9.SetRange(PPM_MIN, PPM_MAX);
	m_ctlRC1Kan10.SetRange(PPM_MIN, PPM_MAX);
	m_ctlRC1Kan11.SetRange(PPM_MIN, PPM_MAX);
	m_ctlRC1Kan12.SetRange(PPM_MIN, PPM_MAX);
	m_ctlRC1Kan13.SetRange(PPM_MIN, PPM_MAX);
	m_ctlRC1Kan14.SetRange(PPM_MIN, PPM_MAX);
	m_ctlRC1Kan15.SetRange(PPM_MIN, PPM_MAX);
	m_ctlRC1Kan16.SetRange(PPM_MIN, PPM_MAX);

	m_ctlSerwo1.SetRange(PPM_MIN, PPM_MAX);
	m_ctlSerwo2.SetRange(PPM_MIN, PPM_MAX);
	m_ctlSerwo3.SetRange(PPM_MIN, PPM_MAX);
	m_ctlSerwo4.SetRange(PPM_MIN, PPM_MAX);
	m_ctlSerwo5.SetRange(PPM_MIN, PPM_MAX);
	m_ctlSerwo6.SetRange(PPM_MIN, PPM_MAX);
	m_ctlSerwo7.SetRange(PPM_MIN, PPM_MAX);
	m_ctlSerwo8.SetRange(PPM_MIN, PPM_MAX);
	m_ctlSerwo9.SetRange(PPM_MIN, PPM_MAX);
	m_ctlSerwo10.SetRange(PPM_MIN, PPM_MAX);
	m_ctlSerwo11.SetRange(PPM_MIN, PPM_MAX);
	m_ctlSerwo12.SetRange(PPM_MIN, PPM_MAX);
	m_ctlSerwo13.SetRange(PPM_MIN, PPM_MAX);
	m_ctlSerwo14.SetRange(PPM_MIN, PPM_MAX);
	m_ctlSerwo15.SetRange(PPM_MIN, PPM_MAX);
	m_ctlSerwo16.SetRange(PPM_MIN, PPM_MAX);
	
	m_ctlOdbiornikRC1.InsertString(0, _T("CPPM1"));
	m_ctlOdbiornikRC1.InsertString(1, _T("S-Bus1"));
	m_ctlOdbiornikRC2.InsertString(0, _T("CPPM2"));
	m_ctlOdbiornikRC2.InsertString(1, _T("S-Bus2"));

	m_ctlTypWyjscia1.InsertString(0, _T("Port I/O PB9"));	//wyjście skonfigurowane jako wjściowy port IO do debugowania algorytmów
	m_ctlTypWyjscia1.InsertString(1, _T("Wyjście S-Bus"));	//wyjście S-Bus
	m_ctlTypWyjscia1.InsertString(2, _T("-"));				//wejście ADC
	m_ctlTypWyjscia1.InsertString(3, _T("-"));				//funkcja alternatywna
	m_ctlTypWyjscia1.InsertString(4, _T("Serwo1 50Hz"));
	m_ctlTypWyjscia1.InsertString(5, _T("-"));				//wyjście PWM 100Hz
	m_ctlTypWyjscia1.InsertString(6, _T("-"));				//wyjście PWM 200Hz
	m_ctlTypWyjscia1.InsertString(7, _T("ESC1 400Hz"));		//wyjście PWM 400Hz	
	m_ctlTypWyjscia1.InsertString(8, _T("DShot150"));		//wyjście DShot150
	m_ctlTypWyjscia1.InsertString(9, _T("DShot300"));		//wyjście DShot300
	m_ctlTypWyjscia1.InsertString(10, _T("DShot600"));		//wyjście DShot600
	m_ctlTypWyjscia1.InsertString(11, _T("DShot1200"));		//wyjście DShot1200		

	m_ctlTypWyjscia2.InsertString(0, _T("Port I/O PB10"));	//wyjście skonfigurowane jako wjściowy port IO do debugowania algorytmów
	m_ctlTypWyjscia2.InsertString(1, _T("Wyjście S-Bus"));	//wyjście S-Bus
	m_ctlTypWyjscia2.InsertString(2, _T("-"));				//wejście ADC
	m_ctlTypWyjscia2.InsertString(3, _T("CS QSPI"));		//funkcja alternatywna
	m_ctlTypWyjscia2.InsertString(4, _T("Serwo1 50Hz"));
	m_ctlTypWyjscia2.InsertString(5, _T("-"));				//wyjście PWM 100Hz
	m_ctlTypWyjscia2.InsertString(6, _T("-"));				//wyjście PWM 200Hz
	m_ctlTypWyjscia2.InsertString(7, _T("ESC2 400Hz"));		//wyjście PWM 400Hz	
	m_ctlTypWyjscia2.InsertString(8, _T("DShot150"));		//wyjście DShot150
	m_ctlTypWyjscia2.InsertString(9, _T("DShot300"));		//wyjście DShot300
	m_ctlTypWyjscia2.InsertString(10, _T("DShot600"));		//wyjście DShot600
	m_ctlTypWyjscia2.InsertString(11, _T("DShot1200"));		//wyjście DShot1200

	m_ctlTypWyjscia3.InsertString(0, _T("Port I/O PA15"));	//wyjście skonfigurowane jako wjściowy port IO do debugowania algorytmów
	m_ctlTypWyjscia3.InsertString(1, _T("Wyjście S-Bus"));	//wyjście S-Bus
	m_ctlTypWyjscia3.InsertString(2, _T("-"));				//wejście ADC
	m_ctlTypWyjscia3.InsertString(3, _T("CS QSPI"));		//funkcja alternatywna
	m_ctlTypWyjscia3.InsertString(4, _T("Serwo1 50Hz"));
	m_ctlTypWyjscia3.InsertString(5, _T("-"));				
	m_ctlTypWyjscia3.InsertString(6, _T("-"));				
	m_ctlTypWyjscia3.InsertString(7, _T("ESC3 400Hz"));		
	m_ctlTypWyjscia3.InsertString(8, _T("DShot150"));		
	m_ctlTypWyjscia3.InsertString(9, _T("DShot300"));		
	m_ctlTypWyjscia3.InsertString(10, _T("DShot600"));		
	m_ctlTypWyjscia3.InsertString(11, _T("DShot1200"));		

	m_ctlTypWyjscia4.InsertString(0, _T("Port I/O PB0"));	//wyjście skonfigurowane jako wjściowy port IO do debugowania algorytmów
	m_ctlTypWyjscia4.InsertString(1, _T("Wyjście S-Bus"));	//wyjście S-Bus
	m_ctlTypWyjscia4.InsertString(2, _T("ADC1_INP9"));		//wejście ADC
	m_ctlTypWyjscia4.InsertString(3, _T("-"));				//funkcja alternatywna
	m_ctlTypWyjscia4.InsertString(4, _T("Serwo1 50Hz"));
	m_ctlTypWyjscia4.InsertString(5, _T("-"));
	m_ctlTypWyjscia4.InsertString(6, _T("-"));
	m_ctlTypWyjscia4.InsertString(7, _T("ESC4 400Hz"));
	m_ctlTypWyjscia4.InsertString(8, _T("DShot150"));
	m_ctlTypWyjscia4.InsertString(9, _T("DShot300"));
	m_ctlTypWyjscia4.InsertString(10, _T("DShot600"));
	m_ctlTypWyjscia4.InsertString(11, _T("DShot1200"));

	m_ctlTypWyjscia5.InsertString(0, _T("Port I/O PB1"));	//wyjście skonfigurowane jako wjściowy port IO do debugowania algorytmów
	m_ctlTypWyjscia5.InsertString(1, _T("Wyjście S-Bus"));	//wyjście S-Bus
	m_ctlTypWyjscia5.InsertString(2, _T("ADC1_INP5"));		//wejście ADC
	m_ctlTypWyjscia5.InsertString(3, _T("-"));				//funkcja alternatywna
	m_ctlTypWyjscia5.InsertString(4, _T("Serwo1 50Hz"));
	m_ctlTypWyjscia5.InsertString(5, _T("-"));
	m_ctlTypWyjscia5.InsertString(6, _T("-"));
	m_ctlTypWyjscia5.InsertString(7, _T("ESC5 400Hz"));
	m_ctlTypWyjscia5.InsertString(8, _T("DShot150"));
	m_ctlTypWyjscia5.InsertString(9, _T("DShot300"));
	m_ctlTypWyjscia5.InsertString(10, _T("DShot600"));
	m_ctlTypWyjscia5.InsertString(11, _T("DShot1200"));

	m_ctlTypWyjscia6.InsertString(0, _T("Port I/O PI5"));	//wyjście skonfigurowane jako wjściowy port IO do debugowania algorytmów
	m_ctlTypWyjscia6.InsertString(1, _T("Wyjście S-Bus"));	//wyjście S-Bus
	m_ctlTypWyjscia6.InsertString(2, _T("ADC1_INP2"));		//wejście ADC
	m_ctlTypWyjscia6.InsertString(3, _T("-"));				//funkcja alternatywna
	m_ctlTypWyjscia6.InsertString(4, _T("Serwo1 50Hz"));
	m_ctlTypWyjscia6.InsertString(5, _T("-"));
	m_ctlTypWyjscia6.InsertString(6, _T("-"));
	m_ctlTypWyjscia6.InsertString(7, _T("ESC6 400Hz"));
	m_ctlTypWyjscia6.InsertString(8, _T("DShot150"));
	m_ctlTypWyjscia6.InsertString(9, _T("DShot300"));
	m_ctlTypWyjscia6.InsertString(10, _T("DShot600"));
	m_ctlTypWyjscia6.InsertString(11, _T("DShot1200"));

	m_ctlTypWyjscia7.InsertString(0, _T("Port I/O PI10"));	//wyjście skonfigurowane jako wjściowy port IO do debugowania algorytmów
	m_ctlTypWyjscia7.InsertString(1, _T("Wyjście S-Bus"));	//wyjście S-Bus
	m_ctlTypWyjscia7.InsertString(2, _T("-"));				//wejście ADC
	m_ctlTypWyjscia7.InsertString(3, _T("-"));				//funkcja alternatywna
	m_ctlTypWyjscia7.InsertString(4, _T("Serwo1 50Hz"));
	m_ctlTypWyjscia7.InsertString(5, _T("-"));
	m_ctlTypWyjscia7.InsertString(6, _T("-"));
	m_ctlTypWyjscia7.InsertString(7, _T("ESC7 400Hz"));
	m_ctlTypWyjscia7.InsertString(8, _T("DShot150"));
	m_ctlTypWyjscia7.InsertString(9, _T("DShot300"));
	m_ctlTypWyjscia7.InsertString(10, _T("DShot600"));
	m_ctlTypWyjscia7.InsertString(11, _T("DShot1200"));

	m_ctlTypWyjscia8.InsertString(0, _T("Port I/O PH15"));	//wyjście skonfigurowane jako wjściowy port IO do debugowania algorytmów
	m_ctlTypWyjscia8.InsertString(1, _T("Wyjście S-Bus"));	//wyjście S-Bus
	m_ctlTypWyjscia8.InsertString(2, _T("-"));				//wejście ADC
	m_ctlTypWyjscia8.InsertString(3, _T("-"));				//funkcja alternatywna
	m_ctlTypWyjscia8.InsertString(4, _T("Serwo1 50Hz"));
	m_ctlTypWyjscia8.InsertString(5, _T("-"));
	m_ctlTypWyjscia8.InsertString(6, _T("-"));
	m_ctlTypWyjscia8.InsertString(7, _T("ESC8 400Hz"));
	m_ctlTypWyjscia8.InsertString(8, _T("DShot150"));
	m_ctlTypWyjscia8.InsertString(9, _T("DShot300"));
	m_ctlTypWyjscia8.InsertString(10, _T("DShot600"));
	m_ctlTypWyjscia8.InsertString(11, _T("DShot1200"));

	m_ctlTypWyjscia9_16.InsertString(0, _T("-"));			//wyjście skonfigurowane jako wjściowy port IO do debugowania algorytmów
	m_ctlTypWyjscia9_16.InsertString(1, _T("-"));			//wyjście S-Bus
	m_ctlTypWyjscia9_16.InsertString(2, _T("-"));			//wejście ADC
	m_ctlTypWyjscia9_16.InsertString(3, _T("-"));			//funkcja alternatywna
	m_ctlTypWyjscia9_16.InsertString(4, _T("Serwa 9-16 50Hz"));
	m_ctlTypWyjscia9_16.InsertString(5, _T("ESC 9-12 100Hz"));	//wyjście PWM 100Hz
	m_ctlTypWyjscia9_16.InsertString(6, _T("ESC 9-10 200Hz"));	//wyjście PWM 200Hz
	m_ctlTypWyjscia9_16.InsertString(7, _T("ESC 9 400Hz"));		//wyjście PWM 400Hz	
	m_ctlTypWyjscia9_16.InsertString(8, _T("-"));
	m_ctlTypWyjscia9_16.InsertString(7, _T("-"));
	m_ctlTypWyjscia9_16.InsertString(10, _T("-"));
	m_ctlTypWyjscia9_16.InsertString(11, _T("-"));

	for (int n = 0; n < 4; n++)
	{
		strNapis.Format(_T("Kanał %d"), n+1);
		m_ctlKanalPrzechylenia.InsertString(n, strNapis);
		m_ctlKanalPochylenia.InsertString(n, strNapis);
		m_ctlKanalOdchylenia.InsertString(n, strNapis);
		m_ctlKanalWysokosci.InsertString(n, strNapis);
	}

	//wypełnij comboboxy funkcji wejść RC
	for (int n = 0; n < 4; n++)
	{
		switch (n)
		{
		case 0: strNapis.Format(_T("Wyjście OD1")); break;
		case 1: strNapis.Format(_T("Wyjście OD2")); break;
		case 2: strNapis.Format(_T("Mów komunikat 1")); break;
		case 3: strNapis.Format(_T("Mów komunikat 2")); break;
		}
		m_ctlFunkcjaKanalu5.InsertString(n, strNapis);
		m_ctlFunkcjaKanalu6.InsertString(n, strNapis);
		m_ctlFunkcjaKanalu7.InsertString(n, strNapis);
		m_ctlFunkcjaKanalu8.InsertString(n, strNapis);
		m_ctlFunkcjaKanalu9.InsertString(n, strNapis);
		m_ctlFunkcjaKanalu10.InsertString(n, strNapis);
		m_ctlFunkcjaKanalu11.InsertString(n, strNapis);
		m_ctlFunkcjaKanalu12.InsertString(n, strNapis);
		m_ctlFunkcjaKanalu13.InsertString(n, strNapis);
		m_ctlFunkcjaKanalu14.InsertString(n, strNapis);
		m_ctlFunkcjaKanalu15.InsertString(n, strNapis);
		m_ctlFunkcjaKanalu16.InsertString(n, strNapis);
	}



	//odczytaj konfigurację wejść odbiorników i wyjść serw.
	chErr = getKomunikacja().CzytajU8FRAM(chDane, 6, FAU_KONF_ODB_RC);
	if (chErr == ERR_OK)
	{
		//konfiguracja odbiorników RC: Bity 0..3 = RC1, bity 4..7 = RC2: 0=PPM, 1=S-Bus, 2=inne (CS QSPI dla RC2)
		m_ctlOdbiornikRC1.SetCurSel(chDane[0] & 0x0F);
		m_ctlOdbiornikRC2.SetCurSel((chDane[0] & 0xF0) >> 4);

		//1U konfiguracja wyjść: Bity 0..3 = Wyjście 1, bity 4..7 = Wyjście 2: 0=PWM 400Hz, 1=SBus
		m_ctlTypWyjscia1.SetCurSel(chDane[1] & 0x0F);
		m_ctlTypWyjscia2.SetCurSel((chDane[1] & 0xF0) >> 4);

		//1U konfiguracja wyjść: Bity 0..3 = Wyjście 3, bity 4..7 = Wyjście 4: 0=PWM 400Hz, 1=ADC
		m_ctlTypWyjscia3.SetCurSel(chDane[2] & 0x0F);
		m_ctlTypWyjscia4.SetCurSel((chDane[2] & 0xF0) >> 4);

		//1U konfiguracja wyjść: Bity 0..3 = Wyjście 5, bity 4..7 = Wyjście 6: 0=PWM 400Hz, , 1=ADC
		m_ctlTypWyjscia5.SetCurSel(chDane[3] & 0x0F);
		m_ctlTypWyjscia6.SetCurSel((chDane[3] & 0xF0) >> 4);

		//1U konfiguracja wyjść: Bity 0..3 = Wyjście 7, bity 4..7 = Wyjście 8: 0=PWM 400Hz
		m_ctlTypWyjscia7.SetCurSel(chDane[4] & 0x0F);
		m_ctlTypWyjscia8.SetCurSel((chDane[4] & 0xF0) >> 4);

		//1U konfiguracja wyjść:  0=wyjścia 9..16 PWM 50Hz, 1=wyjścia 9..12 PWM 100Hz, 2=wyjścia 9..10 PWM 200Hz, 3=wyjście 9 PWM 400Hz
		m_ctlTypWyjscia9_16.SetCurSel(chDane[5]);
	}

	//odczytaj konfigurację wejść odbiorników i wyjść serw.
	chErr = getKomunikacja().CzytajU8FRAM(chDane, LICZBA_DRAZKOW, FAU_KAN_DRAZKA_RC);
	if (chErr == ERR_OK)
	{
		m_ctlKanalPrzechylenia.SetCurSel(chDane[0]);
		m_ctlKanalPochylenia.SetCurSel(chDane[1]);
		m_ctlKanalOdchylenia.SetCurSel(chDane[2]);
		m_ctlKanalWysokosci.SetCurSel(chDane[3]);
	}

	//odczytaj konfigurację wejść odbiorników i wyjść serw.
	chErr = getKomunikacja().CzytajU8FRAM(chDane, KANALY_FUNKCYJNE, FAU_FUNKCJA_KAN_RC);
	if (chErr == ERR_OK)
	{
		m_ctlFunkcjaKanalu5.SetCurSel(chDane[0]);
		m_ctlFunkcjaKanalu6.SetCurSel(chDane[1]);
		m_ctlFunkcjaKanalu7.SetCurSel(chDane[2]);
		m_ctlFunkcjaKanalu8.SetCurSel(chDane[3]);
		m_ctlFunkcjaKanalu9.SetCurSel(chDane[4]);
		m_ctlFunkcjaKanalu10.SetCurSel(chDane[5]);
		m_ctlFunkcjaKanalu11.SetCurSel(chDane[6]);
		m_ctlFunkcjaKanalu12.SetCurSel(chDane[7]);
		m_ctlFunkcjaKanalu13.SetCurSel(chDane[8]);
		m_ctlFunkcjaKanalu14.SetCurSel(chDane[9]);
		m_ctlFunkcjaKanalu15.SetCurSel(chDane[10]);
		m_ctlFunkcjaKanalu16.SetCurSel(chDane[11]);
	}
		
	//inicjuj
	m_bZmienionoMinMax = FALSE;
	for (uint8_t n = 0; n < 16; n++)
	{
		m_stEkstrema[n].bZmieniono = FALSE;
		m_stEkstrema[n].sMin = PPM_MAX;
		m_stEkstrema[n].sMax = PPM_MIN;
	}

	//ustaw napisy okreslające funkcje wyjść RC
	for (int n = FSER_SILNIK1; n <= FSER_SILNIK8; n++)
	{
		strNapis.Format(_T("Silnik %d"), n + 1);
		m_ctlFunkcjaWyjscia1.InsertString(n, strNapis);
		m_ctlFunkcjaWyjscia2.InsertString(n, strNapis);
		m_ctlFunkcjaWyjscia3.InsertString(n, strNapis);
		m_ctlFunkcjaWyjscia4.InsertString(n, strNapis);
		m_ctlFunkcjaWyjscia5.InsertString(n, strNapis);
		m_ctlFunkcjaWyjscia6.InsertString(n, strNapis);
		m_ctlFunkcjaWyjscia7.InsertString(n, strNapis);
		m_ctlFunkcjaWyjscia8.InsertString(n, strNapis);
		m_ctlFunkcjaWyjscia9.InsertString(n, strNapis);
		m_ctlFunkcjaWyjscia10.InsertString(n, strNapis);
		m_ctlFunkcjaWyjscia11.InsertString(n, strNapis);
		m_ctlFunkcjaWyjscia12.InsertString(n, strNapis);
		m_ctlFunkcjaWyjscia13.InsertString(n, strNapis);
		m_ctlFunkcjaWyjscia14.InsertString(n, strNapis);
		m_ctlFunkcjaWyjscia15.InsertString(n, strNapis);
		m_ctlFunkcjaWyjscia16.InsertString(n, strNapis);
	}

	for (int n = FSER_WE_RC1; n <= FSER_WE_RC16; n++)
	{
		strNapis.Format(_T("Wejście RC %d"), n - FSER_WE_RC1 + 1);
		m_ctlFunkcjaWyjscia1.InsertString(n, strNapis);
		m_ctlFunkcjaWyjscia2.InsertString(n, strNapis);
		m_ctlFunkcjaWyjscia3.InsertString(n, strNapis);
		m_ctlFunkcjaWyjscia4.InsertString(n, strNapis);
		m_ctlFunkcjaWyjscia5.InsertString(n, strNapis);
		m_ctlFunkcjaWyjscia6.InsertString(n, strNapis);
		m_ctlFunkcjaWyjscia7.InsertString(n, strNapis);
		m_ctlFunkcjaWyjscia8.InsertString(n, strNapis);
		m_ctlFunkcjaWyjscia9.InsertString(n, strNapis);
		m_ctlFunkcjaWyjscia10.InsertString(n, strNapis);
		m_ctlFunkcjaWyjscia11.InsertString(n, strNapis);
		m_ctlFunkcjaWyjscia12.InsertString(n, strNapis);
		m_ctlFunkcjaWyjscia13.InsertString(n, strNapis);
		m_ctlFunkcjaWyjscia14.InsertString(n, strNapis);
		m_ctlFunkcjaWyjscia15.InsertString(n, strNapis);
		m_ctlFunkcjaWyjscia16.InsertString(n, strNapis);
	}

	//odczytaj funkcję wyjścia
	chErr = getKomunikacja().CzytajU8FRAM(chDane, KANALY_WYJSC_RC, FAU_FUNKCJA_WY_RC);
	if (chErr == ERR_OK)
	{
		m_ctlFunkcjaWyjscia1.SetCurSel(chDane[0]);
		m_ctlFunkcjaWyjscia2.SetCurSel(chDane[1]);
		m_ctlFunkcjaWyjscia3.SetCurSel(chDane[2]);
		m_ctlFunkcjaWyjscia4.SetCurSel(chDane[3]);
		m_ctlFunkcjaWyjscia5.SetCurSel(chDane[4]);
		m_ctlFunkcjaWyjscia6.SetCurSel(chDane[5]);
		m_ctlFunkcjaWyjscia7.SetCurSel(chDane[6]);
		m_ctlFunkcjaWyjscia8.SetCurSel(chDane[7]);
		m_ctlFunkcjaWyjscia9.SetCurSel(chDane[8]);
		m_ctlFunkcjaWyjscia10.SetCurSel(chDane[9]);
		m_ctlFunkcjaWyjscia11.SetCurSel(chDane[10]);
		m_ctlFunkcjaWyjscia12.SetCurSel(chDane[11]);
		m_ctlFunkcjaWyjscia13.SetCurSel(chDane[12]);
		m_ctlFunkcjaWyjscia14.SetCurSel(chDane[13]);
		m_ctlFunkcjaWyjscia15.SetCurSel(chDane[14]);
		m_ctlFunkcjaWyjscia16.SetCurSel(chDane[15]);
	}

	SetTimer(IDT_TIMER_RC, 100, (TIMERPROC)NULL);
	WstawDaneKanalow();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // WYJĄTEK: Strona właściwości OCX powinna zwrócić FALSE
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wstawia zmienne telemetryczne do zmiennych pasków poszczególnych kanałów
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t OdbiornikiRC::WstawDaneKanalow()
{
	int nWartoscRC;
	int nIndeksTele = (int)getProtokol().m_vDaneTelemetryczne.size();
	if (nIndeksTele == 0)
		return ERR_NO_DATA_RECEIVED;
	else
		nIndeksTele -= 1;	//indeks jest liczbą danych -1
	m_ctlRC1Kan1.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC_KAN1]);
	m_ctlRC1Kan2.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC_KAN2]);
	m_ctlRC1Kan3.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC_KAN3]);
	m_ctlRC1Kan4.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC_KAN4]);
	m_ctlRC1Kan5.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC_KAN5]);
	m_ctlRC1Kan6.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC_KAN6]);
	m_ctlRC1Kan7.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC_KAN7]);
	m_ctlRC1Kan8.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC_KAN8]);
	m_ctlRC1Kan9.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC_KAN9]);
	m_ctlRC1Kan10.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC_KAN10]);
	m_ctlRC1Kan11.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC_KAN11]);
	m_ctlRC1Kan12.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC_KAN12]);
	m_ctlRC1Kan13.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC_KAN13]);
	m_ctlRC1Kan14.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC_KAN14]);
	m_ctlRC1Kan15.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC_KAN15]);
	m_ctlRC1Kan16.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC_KAN16]);

	//aktualizuj wartości min i max
	for (int n = 0; n < 16; n++)
	{
		nWartoscRC = getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC_KAN1 + n];
		if (nWartoscRC < m_stEkstrema[n].sMin)
		{
			m_stEkstrema[n].sMin = nWartoscRC;
			m_stEkstrema[n].bZmieniono = TRUE;
		}
		if (nWartoscRC > m_stEkstrema[n].sMax)
		{
			m_stEkstrema[n].sMax = nWartoscRC;
			m_stEkstrema[n].bZmieniono = TRUE;			
		}
		//przynajmniej w jednym kanale musi być ustawiony pełen zakres aby zapisało takie nastawy
		if ((m_stEkstrema[n].sMin < PPM_M90) && (m_stEkstrema[n].sMax > PPM_P90))
			m_bZmienionoMinMax = TRUE;
	}

	m_ctlSerwo1.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_SERWO1]);
	m_ctlSerwo2.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_SERWO2]);
	m_ctlSerwo3.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_SERWO3]);
	m_ctlSerwo4.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_SERWO4]);
	m_ctlSerwo5.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_SERWO5]);
	m_ctlSerwo6.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_SERWO6]);
	m_ctlSerwo7.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_SERWO7]);
	m_ctlSerwo8.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_SERWO8]);
	m_ctlSerwo9.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_SERWO9]);
	m_ctlSerwo10.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_SERWO10]);
	m_ctlSerwo11.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_SERWO11]);
	m_ctlSerwo12.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_SERWO12]);
	m_ctlSerwo13.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_SERWO13]);
	m_ctlSerwo14.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_SERWO14]);
	m_ctlSerwo15.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_SERWO15]);
	m_ctlSerwo16.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_SERWO16]);
	UpdateData(FALSE);
	return ERR_OK;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na upływ czasu timera powodująca odświeżenia pasków kanałówRC w oknie
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void OdbiornikiRC::UstawMinMax()
{
	for (uint8_t n = 0; n < 16; n++)
	{
		if (m_stEkstrema[n].bZmieniono)
		{
			m_stEkstrema[n].strMinMax.Format(_T("%d-%d"), m_stEkstrema[n].sMin, m_stEkstrema[n].sMax);
			m_stEkstrema[n].bZmieniono = FALSE;
		}
		UpdateData(FALSE);
	}	
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na upływ czasu timera powodująca odświeżenia pasków kanałówRC w oknie
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void OdbiornikiRC::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Dodaj tutaj swój kod procedury obsługi komunikatów i/lub wywołaj domyślny
	WstawDaneKanalow();
	UstawMinMax();
	CDialogEx::OnTimer(nIDEvent);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na naciśnięcie przycisku OK - zapisuje konfigurcję jeżeli została zmieniona
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void OdbiornikiRC::OnBnClickedOk()
{
	uint8_t chErr, chDane[KANALY_WYJSC_RC];
	CString strKomunikat;

	KillTimer(IDT_TIMER_RC);
	if (m_bZmienionoUstawienie)
	{	
		chDane[0] = m_ctlOdbiornikRC1.GetCurSel() | m_ctlOdbiornikRC2.GetCurSel() << 4;
		chDane[1] = m_ctlTypWyjscia1.GetCurSel() | m_ctlTypWyjscia2.GetCurSel() << 4;
		chDane[2] = m_ctlTypWyjscia3.GetCurSel() | m_ctlTypWyjscia4.GetCurSel() << 4;
		chDane[3] = m_ctlTypWyjscia5.GetCurSel() | m_ctlTypWyjscia6.GetCurSel() << 4;
		chDane[4] = m_ctlTypWyjscia7.GetCurSel() | m_ctlTypWyjscia8.GetCurSel() << 4;
		chDane[5] = m_ctlTypWyjscia9_16.GetCurSel();
		chErr = getKomunikacja().ZapiszDaneU8FRAM(chDane, 6, FAU_KONF_ODB_RC);

		chErr = getKomunikacja().RekonfigurujWeWyRC();	//przeładuj konfigurację
		if (chErr != ERR_OK)
		{
			strKomunikat.Format(_T("Błąd nr %d zapisu konfiguracji"), chErr);
			MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojoj!"), MB_ICONWARNING, 0);
			CDialogEx::OnOK();
		}
	}

	if (m_bZmienionoMinMax)
	{
		chErr = getKomunikacja().ZapiszEkstremaWejscRC();
		if (chErr != ERR_OK)
		{
			strKomunikat.Format(_T("Błąd nr %d zapisu konfiguracji"), chErr);
			MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojoj!"), MB_ICONWARNING, 0);
			CDialogEx::OnOK();
		}
	}

	if (m_bZmienionoKanalDrazkow)
	{
		chDane[0] = m_ctlKanalPrzechylenia.GetCurSel();
		chDane[1] = m_ctlKanalPochylenia.GetCurSel();
		chDane[2] = m_ctlKanalOdchylenia.GetCurSel();
		chDane[3] = m_ctlKanalWysokosci.GetCurSel();
		chErr = getKomunikacja().ZapiszDaneU8FRAM(chDane, LICZBA_DRAZKOW, FAU_KAN_DRAZKA_RC);
		if (chErr != ERR_OK)
		{
			strKomunikat.Format(_T("Błąd nr %d zapisu konfiguracji"), chErr);
			MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojoj!"), MB_ICONWARNING, 0);
			CDialogEx::OnOK();
		}
	}

	if (m_bZmienionoFunkcjeKanalow)
	{
		chDane[0] = m_ctlFunkcjaKanalu5.GetCurSel();
		chDane[1] = m_ctlFunkcjaKanalu6.GetCurSel();
		chDane[2] = m_ctlFunkcjaKanalu7.GetCurSel();
		chDane[3] = m_ctlFunkcjaKanalu8.GetCurSel();
		chDane[4] = m_ctlFunkcjaKanalu9.GetCurSel();
		chDane[5] = m_ctlFunkcjaKanalu10.GetCurSel();
		chDane[6] = m_ctlFunkcjaKanalu11.GetCurSel();
		chDane[7] = m_ctlFunkcjaKanalu12.GetCurSel();
		chDane[8] = m_ctlFunkcjaKanalu13.GetCurSel();
		chDane[9] = m_ctlFunkcjaKanalu14.GetCurSel();
		chDane[10] = m_ctlFunkcjaKanalu15.GetCurSel();
		chDane[11] = m_ctlFunkcjaKanalu16.GetCurSel();

		chErr = getKomunikacja().ZapiszDaneU8FRAM(chDane, KANALY_FUNKCYJNE, FAU_FUNKCJA_KAN_RC);
		if (chErr != ERR_OK)
		{
			strKomunikat.Format(_T("Błąd nr %d zapisu konfiguracji"), chErr);
			MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojoj!"), MB_ICONWARNING, 0);
			CDialogEx::OnOK();
		}	
	}

	if (m_bZmienionoFunkcjeWyjscRC)
	{
		chDane[0] = m_ctlFunkcjaWyjscia1.GetCurSel();
		chDane[1] = m_ctlFunkcjaWyjscia2.GetCurSel();
		chDane[2] = m_ctlFunkcjaWyjscia3.GetCurSel();
		chDane[3] = m_ctlFunkcjaWyjscia4.GetCurSel();
		chDane[4] = m_ctlFunkcjaWyjscia5.GetCurSel();
		chDane[5] = m_ctlFunkcjaWyjscia6.GetCurSel();
		chDane[6] = m_ctlFunkcjaWyjscia7.GetCurSel();
		chDane[7] = m_ctlFunkcjaWyjscia8.GetCurSel();
		chDane[8] = m_ctlFunkcjaWyjscia9.GetCurSel();
		chDane[9] = m_ctlFunkcjaWyjscia10.GetCurSel();
		chDane[10] = m_ctlFunkcjaWyjscia11.GetCurSel();
		chDane[11] = m_ctlFunkcjaWyjscia12.GetCurSel();
		chDane[12] = m_ctlFunkcjaWyjscia13.GetCurSel();
		chDane[13] = m_ctlFunkcjaWyjscia14.GetCurSel();
		chDane[14] = m_ctlFunkcjaWyjscia15.GetCurSel();
		chDane[15] = m_ctlFunkcjaWyjscia16.GetCurSel();

		chErr = getKomunikacja().ZapiszDaneU8FRAM(chDane, KANALY_WYJSC_RC, FAU_FUNKCJA_WY_RC);
		if (chErr != ERR_OK)
		{
			strKomunikat.Format(_T("Błąd nr %d zapisu konfiguracji"), chErr);
			MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojoj!"), MB_ICONWARNING, 0);
			CDialogEx::OnOK();
		}
	}

	//sprawdź czy była zmiana i czy należy zresetować CM4
	if (m_bZmienionoUstawienie | m_bZmienionoMinMax | m_bZmienionoKanalDrazkow | m_bZmienionoKanalDrazkow | m_bZmienionoFunkcjeKanalow)
	{
		//wyświetl okno z pytaniem czy resetować CM4
		int nRet = MessageBoxExW(this->m_hWnd, _T("Nastąpiła zmiana konfiguracji. \nCzy wykonać reset rdzenia CM4?"), _T("Potrzebny reset aby zmiana przyniosła efekt"), MB_ICONWARNING + MB_YESNO, 0);
		if (nRet == IDYES)
		{
			chErr = getKomunikacja().ResetujCM4();
			if (chErr != ERR_OK)
			{
				if (chErr == 8)	//BLAD_ODMOWA_WYKONANIA
					strKomunikat.Format(_T("Odmowa wykonania resetu ze względów bezpieczeństwa"));
				else
					strKomunikat.Format(_T("Błąd nr %d wykonania polecenia"), chErr);
				MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojoj!"), MB_ICONWARNING, 0);
				CDialogEx::OnOK();
			}
			chErr = getKomunikacja().WyslijOK();	//wyłącz polecenie resetujące
		}
	}


	CDialogEx::OnOK();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na naciśnięcie przycisku Anuluj
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void OdbiornikiRC::OnBnClickedCancel()
{
	KillTimer(IDT_TIMER_RC);
	CDialogEx::OnCancel();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmianę typu wyjścia 1
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void OdbiornikiRC::OnCbnSelchangeCombo1()
{
	m_bZmienionoUstawienie = TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmianę typu wyjścia 2
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void OdbiornikiRC::OnCbnSelchangeCombo2()
{
	m_bZmienionoUstawienie = TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmianę typu wyjścia 3
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void OdbiornikiRC::OnCbnSelchangeCombo3()
{
	m_bZmienionoUstawienie = TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmianę typu wyjścia 4
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void OdbiornikiRC::OnCbnSelchangeCombo4()
{
	m_bZmienionoUstawienie = TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmianę typu wyjścia 5
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void OdbiornikiRC::OnCbnSelchangeCombo5()
{
	m_bZmienionoUstawienie = TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmianę typu wyjścia 6
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void OdbiornikiRC::OnCbnSelchangeCombo6()
{
	m_bZmienionoUstawienie = TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmianę typu wyjścia 7
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void OdbiornikiRC::OnCbnSelchangeCombo7()
{
	m_bZmienionoUstawienie = TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmianę typu wyjścia 8
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void OdbiornikiRC::OnCbnSelchangeCombo8()
{
	m_bZmienionoUstawienie = TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmianę typu wyjść 9-16
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void OdbiornikiRC::OnCbnSelchangeCombo9()
{
	m_bZmienionoUstawienie = TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmianę typu odbiornika 1
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void OdbiornikiRC::OnCbnSelchangeComboRc1()
{
	m_bZmienionoUstawienie = TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmianę typu odbiornika 2
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void OdbiornikiRC::OnCbnSelchangeComboRc2()
{
	m_bZmienionoUstawienie = TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmianę numeru kanału dla funkcji: Przechylenie
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void OdbiornikiRC::OnCbnSelchangeComboKanPrze()
{
	m_bZmienionoKanalDrazkow = TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmianę numeru kanału dla funkcji: Pochylenie
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void OdbiornikiRC::OnCbnSelchangeComboKanPoch1()
{
	m_bZmienionoKanalDrazkow = TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmianę numeru kanału dla funkcji: Odchylenie
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void OdbiornikiRC::OnCbnSelchangeComboKanOdch()
{
	m_bZmienionoKanalDrazkow = TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmianę numeru kanału dla funkcji: Wysokość
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void OdbiornikiRC::OnCbnSelchangeComboKanWyso()
{
	m_bZmienionoKanalDrazkow = TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmianę numeru funkcji dla kanałów 5..16
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void OdbiornikiRC::OnCbnSelchangeComboKan5()
{
	m_bZmienionoFunkcjeKanalow = TRUE;
}


void OdbiornikiRC::OnCbnSelchangeComboKan6()
{
	m_bZmienionoFunkcjeKanalow = TRUE;
}


void OdbiornikiRC::OnCbnSelchangeComboKan7()
{
	m_bZmienionoFunkcjeKanalow = TRUE;
}


void OdbiornikiRC::OnCbnSelchangeComboKan8()
{
	m_bZmienionoFunkcjeKanalow = TRUE;
}


void OdbiornikiRC::OnCbnSelchangeComboKan9()
{
	m_bZmienionoFunkcjeKanalow = TRUE;
}


void OdbiornikiRC::OnCbnSelchangeComboKan10()
{
	m_bZmienionoFunkcjeKanalow = TRUE;
}


void OdbiornikiRC::OnCbnSelchangeComboKan11()
{
	m_bZmienionoFunkcjeKanalow = TRUE;
}


void OdbiornikiRC::OnCbnSelchangeComboKan12()
{
	m_bZmienionoFunkcjeKanalow = TRUE;
}


void OdbiornikiRC::OnCbnSelchangeComboKan13()
{
	m_bZmienionoFunkcjeKanalow = TRUE;
}


void OdbiornikiRC::OnCbnSelchangeComboKan14()
{
	m_bZmienionoFunkcjeKanalow = TRUE;
}


void OdbiornikiRC::OnCbnSelchangeComboKan15()
{
	m_bZmienionoFunkcjeKanalow = TRUE;
}


void OdbiornikiRC::OnCbnSelchangeComboKan16()
{
	m_bZmienionoFunkcjeKanalow = TRUE;
}




void OdbiornikiRC::OnCbnSelchangeComboFunkcjaSerwa1()
{
	m_bZmienionoFunkcjeWyjscRC = TRUE;
}


void OdbiornikiRC::OnCbnSelchangeComboFunkcjaSerwa2()
{
	m_bZmienionoFunkcjeWyjscRC = TRUE;
}


void OdbiornikiRC::OnCbnSelchangeComboFunkcjaSerwa3()
{
	m_bZmienionoFunkcjeWyjscRC = TRUE;
}


void OdbiornikiRC::OnCbnSelchangeComboFunkcjaSerwa4()
{
	m_bZmienionoFunkcjeWyjscRC = TRUE;
}


void OdbiornikiRC::OnCbnSelchangeComboFunkcjaSerwa5()
{
	m_bZmienionoFunkcjeWyjscRC = TRUE;
}


void OdbiornikiRC::OnCbnSelchangeComboFunkcjaSerwa6()
{
	m_bZmienionoFunkcjeWyjscRC = TRUE;
}


void OdbiornikiRC::OnCbnSelchangeComboFunkcjaSerwa7()
{
	m_bZmienionoFunkcjeWyjscRC = TRUE;
}


void OdbiornikiRC::OnCbnSelchangeComboFunkcjaSerwa8()
{
	m_bZmienionoFunkcjeWyjscRC = TRUE;
}


void OdbiornikiRC::OnCbnSelchangeComboFunkcjaSerwa9()
{
	m_bZmienionoFunkcjeWyjscRC = TRUE;
}


void OdbiornikiRC::OnCbnSelchangeComboFunkcjaSerwa10()
{
	m_bZmienionoFunkcjeWyjscRC = TRUE;
}


void OdbiornikiRC::OnCbnSelchangeComboFunkcjaSerwa11()
{
	m_bZmienionoFunkcjeWyjscRC = TRUE;
}


void OdbiornikiRC::OnCbnSelchangeComboFunkcjaSerwa12()
{
	m_bZmienionoFunkcjeWyjscRC = TRUE;
}


void OdbiornikiRC::OnCbnSelchangeComboFunkcjaSerwa13()
{
	m_bZmienionoFunkcjeWyjscRC = TRUE;
}


void OdbiornikiRC::OnCbnSelchangeComboFunkcjaSerwa14()
{
	m_bZmienionoFunkcjeWyjscRC = TRUE;
}


void OdbiornikiRC::OnCbnSelchangeComboFunkcjaSerwa15()
{
	m_bZmienionoFunkcjeWyjscRC = TRUE;
}


void OdbiornikiRC::OnCbnSelchangeComboFunkcjaSerwa16()
{
	m_bZmienionoFunkcjeWyjscRC = TRUE;
}
