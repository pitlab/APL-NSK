// WyjsciaRC.cpp: plik implementacji
//

#include "pch.h"
#include "APL-SN.h"
#include "WyjsciaRC.h"
#include "afxdialogex.h"
#include "konfig_fram.h"
#include "Errors.h"
#include "pid_kanaly.h"

// Okno dialogowe WyjsciaRC

IMPLEMENT_DYNAMIC(WyjsciaRC, CDialogEx)

WyjsciaRC::WyjsciaRC(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WYJSCIA_RC, pParent)
	, m_sBackupOkresuTelemetrii{ 0 }
	, m_bZmienionoUstawienie(FALSE)
	, m_bZmienionoFunkcjeWyjscRC(FALSE)
{

}

WyjsciaRC::~WyjsciaRC()
{
	//przywróc pierwotne nastawy telemetrii
	PrzywrocTelemetrie(m_sBackupOkresuTelemetrii);	
}

void WyjsciaRC::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_ctlSerwo1);
	DDX_Control(pDX, IDC_PROGRESS2, m_ctlSerwo2);
	DDX_Control(pDX, IDC_PROGRESS3, m_ctlSerwo3);
	DDX_Control(pDX, IDC_PROGRESS4, m_ctlSerwo4);
	DDX_Control(pDX, IDC_PROGRESS5, m_ctlSerwo5);
	DDX_Control(pDX, IDC_PROGRESS6, m_ctlSerwo6);
	DDX_Control(pDX, IDC_PROGRESS7, m_ctlSerwo7);
	DDX_Control(pDX, IDC_PROGRESS8, m_ctlSerwo8);
	DDX_Control(pDX, IDC_PROGRESS9, m_ctlSerwo9);
	DDX_Control(pDX, IDC_PROGRESS10, m_ctlSerwo10);
	DDX_Control(pDX, IDC_PROGRESS11, m_ctlSerwo11);
	DDX_Control(pDX, IDC_PROGRESS12, m_ctlSerwo12);
	DDX_Control(pDX, IDC_PROGRESS13, m_ctlSerwo13);
	DDX_Control(pDX, IDC_PROGRESS14, m_ctlSerwo14);
	DDX_Control(pDX, IDC_PROGRESS15, m_ctlSerwo15);
	DDX_Control(pDX, IDC_PROGRESS16, m_ctlSerwo16);
	DDX_Control(pDX, IDC_COMBO_SERWO1, m_ctlTypWyjscia1);
	DDX_Control(pDX, IDC_COMBO_SERWO2, m_ctlTypWyjscia2);
	DDX_Control(pDX, IDC_COMBO_SERWO3, m_ctlTypWyjscia3);
	DDX_Control(pDX, IDC_COMBO_SERWO4, m_ctlTypWyjscia4);
	DDX_Control(pDX, IDC_COMBO_SERWO5, m_ctlTypWyjscia5);
	DDX_Control(pDX, IDC_COMBO_SERWO6, m_ctlTypWyjscia6);
	DDX_Control(pDX, IDC_COMBO_SERWO7, m_ctlTypWyjscia7);
	DDX_Control(pDX, IDC_COMBO_SERWO8, m_ctlTypWyjscia8);
	DDX_Control(pDX, IDC_COMBO_SERWO9, m_ctlTypWyjscia9_16);
	DDX_Control(pDX, IDC_COMBO_FUNKCJA_WYJSCIA1, m_ctlFunkcjaWyjscia1);
	DDX_Control(pDX, IDC_COMBO_FUNKCJA_WYJSCIA2, m_ctlFunkcjaWyjscia2);
	DDX_Control(pDX, IDC_COMBO_FUNKCJA_WYJSCIA3, m_ctlFunkcjaWyjscia3);
	DDX_Control(pDX, IDC_COMBO_FUNKCJA_WYJSCIA4, m_ctlFunkcjaWyjscia4);
	DDX_Control(pDX, IDC_COMBO_FUNKCJA_WYJSCIA5, m_ctlFunkcjaWyjscia5);
	DDX_Control(pDX, IDC_COMBO_FUNKCJA_WYJSCIA6, m_ctlFunkcjaWyjscia6);
	DDX_Control(pDX, IDC_COMBO_FUNKCJA_WYJSCIA7, m_ctlFunkcjaWyjscia7);
	DDX_Control(pDX, IDC_COMBO_FUNKCJA_WYJSCIA8, m_ctlFunkcjaWyjscia8);
	DDX_Control(pDX, IDC_COMBO_FUNKCJA_WYJSCIA9, m_ctlFunkcjaWyjscia9);
	DDX_Control(pDX, IDC_COMBO_FUNKCJA_WYJSCIA10, m_ctlFunkcjaWyjscia10);
	DDX_Control(pDX, IDC_COMBO_FUNKCJA_WYJSCIA11, m_ctlFunkcjaWyjscia11);
	DDX_Control(pDX, IDC_COMBO_FUNKCJA_WYJSCIA12, m_ctlFunkcjaWyjscia12);
	DDX_Control(pDX, IDC_COMBO_FUNKCJA_WYJSCIA13, m_ctlFunkcjaWyjscia13);
	DDX_Control(pDX, IDC_COMBO_FUNKCJA_WYJSCIA14, m_ctlFunkcjaWyjscia14);
	DDX_Control(pDX, IDC_COMBO_FUNKCJA_WYJSCIA15, m_ctlFunkcjaWyjscia15);
	DDX_Control(pDX, IDC_COMBO_FUNKCJA_WYJSCIA16, m_ctlFunkcjaWyjscia16);
	DDX_Text(pDX, IDC_STAT_WART_KAN01, strWyjscie[0]);
	DDX_Text(pDX, IDC_STAT_WART_KAN02, strWyjscie[1]);
	DDX_Text(pDX, IDC_STAT_WART_KAN03, strWyjscie[2]);
	DDX_Text(pDX, IDC_STAT_WART_KAN04, strWyjscie[3]);
	DDX_Text(pDX, IDC_STAT_WART_KAN05, strWyjscie[4]);
	DDX_Text(pDX, IDC_STAT_WART_KAN06, strWyjscie[5]);
	DDX_Text(pDX, IDC_STAT_WART_KAN07, strWyjscie[6]);
	DDX_Text(pDX, IDC_STAT_WART_KAN08, strWyjscie[7]);
	DDX_Text(pDX, IDC_STAT_WART_KAN09, strWyjscie[8]);
	DDX_Text(pDX, IDC_STAT_WART_KAN10, strWyjscie[9]);
	DDX_Text(pDX, IDC_STAT_WART_KAN11, strWyjscie[10]);
	DDX_Text(pDX, IDC_STAT_WART_KAN12, strWyjscie[11]);
	DDX_Text(pDX, IDC_STAT_WART_KAN13, strWyjscie[12]);
	DDX_Text(pDX, IDC_STAT_WART_KAN14, strWyjscie[13]);
	DDX_Text(pDX, IDC_STAT_WART_KAN15, strWyjscie[14]);
	DDX_Text(pDX, IDC_STAT_WART_KAN16, strWyjscie[15]);
}


BEGIN_MESSAGE_MAP(WyjsciaRC, CDialogEx)
	ON_BN_CLICKED(IDOK, &WyjsciaRC::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &WyjsciaRC::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_SERWO1, &WyjsciaRC::OnCbnSelchangeComboSerwo1)
	ON_CBN_SELCHANGE(IDC_COMBO_SERWO2, &WyjsciaRC::OnCbnSelchangeComboSerwo2)
	ON_CBN_SELCHANGE(IDC_COMBO_SERWO3, &WyjsciaRC::OnCbnSelchangeComboSerwo3)
	ON_CBN_SELCHANGE(IDC_COMBO_SERWO4, &WyjsciaRC::OnCbnSelchangeComboSerwo4)
	ON_CBN_SELCHANGE(IDC_COMBO_SERWO6, &WyjsciaRC::OnCbnSelchangeComboSerwo6)
	ON_CBN_SELCHANGE(IDC_COMBO_SERWO8, &WyjsciaRC::OnCbnSelchangeComboSerwo8)
	ON_CBN_SELCHANGE(IDC_COMBO_SERWO7, &WyjsciaRC::OnCbnSelchangeComboSerwo7)
	ON_CBN_SELCHANGE(IDC_COMBO_SERWO9, &WyjsciaRC::OnCbnSelchangeComboSerwo9)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNKCJA_WYJSCIA1, &WyjsciaRC::OnCbnSelchangeComboFunkcjaWyjscia1)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNKCJA_WYJSCIA2, &WyjsciaRC::OnCbnSelchangeComboFunkcjaWyjscia2)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNKCJA_WYJSCIA3, &WyjsciaRC::OnCbnSelchangeComboFunkcjaWyjscia3)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNKCJA_WYJSCIA4, &WyjsciaRC::OnCbnSelchangeComboFunkcjaWyjscia4)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNKCJA_WYJSCIA5, &WyjsciaRC::OnCbnSelchangeComboFunkcjaWyjscia5)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNKCJA_WYJSCIA6, &WyjsciaRC::OnCbnSelchangeComboFunkcjaWyjscia6)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNKCJA_WYJSCIA7, &WyjsciaRC::OnCbnSelchangeComboFunkcjaWyjscia7)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNKCJA_WYJSCIA8, &WyjsciaRC::OnCbnSelchangeComboFunkcjaWyjscia8)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNKCJA_WYJSCIA9, &WyjsciaRC::OnCbnSelchangeComboFunkcjaWyjscia9)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNKCJA_WYJSCIA10, &WyjsciaRC::OnCbnSelchangeComboFunkcjaWyjscia10)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNKCJA_WYJSCIA11, &WyjsciaRC::OnCbnSelchangeComboFunkcjaWyjscia11)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNKCJA_WYJSCIA12, &WyjsciaRC::OnCbnSelchangeComboFunkcjaWyjscia12)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNKCJA_WYJSCIA13, &WyjsciaRC::OnCbnSelchangeComboFunkcjaWyjscia13)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNKCJA_WYJSCIA14, &WyjsciaRC::OnCbnSelchangeComboFunkcjaWyjscia14)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNKCJA_WYJSCIA15, &WyjsciaRC::OnCbnSelchangeComboFunkcjaWyjscia15)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNKCJA_WYJSCIA16, &WyjsciaRC::OnCbnSelchangeComboFunkcjaWyjscia16)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_SERWO5, &WyjsciaRC::OnCbnSelchangeComboSerwo5)
END_MESSAGE_MAP()


// Procedury obsługi komunikatów WyjsciaRC





BOOL WyjsciaRC::OnInitDialog()
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
			m_sBackupOkresuTelemetrii[n] = getKomunikacja().m_cRoj.vWron[nIndeksDronaWRoju].m_sOkresTelemetrii[n];
		
		//ustaw telemetrię wejść i wyjść RC na CZESTOTLIWOSC_ODSWIEZANIA [Hz], resztę wyłącz
		for (int n = 0; n < LICZBA_ZMIENNYCH_TELEMETRYCZNYCH; n++)
		{
			if ((n >= TID_SERWO1) && (n <= TID_SERWO16))
				sOkresTelemetrii[n] = (uint16_t)(MAX_CZESTOTLIWOSC_TELEMETRII / CZESTOTLIWOSC_ODSWIEZANIA_KANALOW_RC);
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
	}

	//definiuj zakres kanalów
	m_ctlSerwo1.SetRange(0, WE_RC_MAX);
	m_ctlSerwo2.SetRange(0, WE_RC_MAX);
	m_ctlSerwo3.SetRange(0, WE_RC_MAX);
	m_ctlSerwo4.SetRange(0, WE_RC_MAX);
	m_ctlSerwo5.SetRange(0, WE_RC_MAX);
	m_ctlSerwo6.SetRange(0, WE_RC_MAX);
	m_ctlSerwo7.SetRange(0, WE_RC_MAX);
	m_ctlSerwo8.SetRange(0, WE_RC_MAX);
	m_ctlSerwo9.SetRange(0, WE_RC_MAX);
	m_ctlSerwo10.SetRange(0, WE_RC_MAX);
	m_ctlSerwo11.SetRange(0, WE_RC_MAX);
	m_ctlSerwo12.SetRange(0, WE_RC_MAX);
	m_ctlSerwo13.SetRange(0, WE_RC_MAX);
	m_ctlSerwo14.SetRange(0, WE_RC_MAX);
	m_ctlSerwo15.SetRange(0, WE_RC_MAX);
	m_ctlSerwo16.SetRange(0, WE_RC_MAX);

	m_ctlTypWyjscia1.InsertString(0, _T("Port I/O PB9"));	//wyjście skonfigurowane jako wjściowy port IO do debugowania algorytmów
	m_ctlTypWyjscia1.InsertString(1, _T("Wyjście S-Bus"));	//wyjście S-Bus

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
	m_ctlTypWyjscia2.InsertString(12, _T("LED WS281x"));	//obsługa programowalnego LED RGB

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
	m_ctlTypWyjscia3.InsertString(12, _T("LED WS281x"));	//obsługa programowalnego LED RGB

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
	m_ctlTypWyjscia4.InsertString(12, _T("LED WS281x"));	//obsługa programowalnego LED RGB

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
	m_ctlTypWyjscia5.InsertString(12, _T("LED WS281x"));	//obsługa programowalnego LED RGB

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
	m_ctlTypWyjscia6.InsertString(12, _T("LED WS281x"));	//obsługa programowalnego LED RGB

	m_ctlTypWyjscia7.InsertString(0, _T("Port I/O PI10"));	//wyjście skonfigurowane jako wjściowy port IO do debugowania algorytmów

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
	m_ctlTypWyjscia8.InsertString(12, _T("LED WS281x"));	//obsługa programowalnego LED RGB

	m_ctlTypWyjscia9_16.InsertString(0, _T("-"));			//wyjście skonfigurowane jako wjściowy port IO do debugowania algorytmów
	m_ctlTypWyjscia9_16.InsertString(1, _T("-"));			//wyjście S-Bus
	m_ctlTypWyjscia9_16.InsertString(2, _T("-"));			//wejście ADC
	m_ctlTypWyjscia9_16.InsertString(3, _T("-"));			//funkcja alternatywna
	m_ctlTypWyjscia9_16.InsertString(4, _T("Serwa 9-16 50Hz"));
	m_ctlTypWyjscia9_16.InsertString(5, _T("ESC 9-12 100Hz"));	//wyjście PWM 100Hz
	m_ctlTypWyjscia9_16.InsertString(6, _T("ESC 9-10 200Hz"));	//wyjście PWM 200Hz
	m_ctlTypWyjscia9_16.InsertString(7, _T("ESC 9 400Hz"));		//wyjście PWM 400Hz	



	//odczytaj konfigurację  wyjść serw.
	chErr = getKomunikacja().CzytajU8FRAM(chDane, 5, FAU_KONF_SERWA12);
	if (chErr == ERR_OK)
	{
		//1U konfiguracja wyjść: Bity 0..3 = Wyjście 1, bity 4..7 = Wyjście 2: 0=PWM 400Hz, 1=SBus
		m_ctlTypWyjscia1.SetCurSel(chDane[0] & 0x0F);
		m_ctlTypWyjscia2.SetCurSel((chDane[0] & 0xF0) >> 4);

		//1U konfiguracja wyjść: Bity 0..3 = Wyjście 3, bity 4..7 = Wyjście 4: 0=PWM 400Hz, 1=ADC
		m_ctlTypWyjscia3.SetCurSel(chDane[1] & 0x0F);
		m_ctlTypWyjscia4.SetCurSel((chDane[1] & 0xF0) >> 4);

		//1U konfiguracja wyjść: Bity 0..3 = Wyjście 5, bity 4..7 = Wyjście 6: 0=PWM 400Hz, , 1=ADC
		m_ctlTypWyjscia5.SetCurSel(chDane[2] & 0x0F);
		m_ctlTypWyjscia6.SetCurSel((chDane[2] & 0xF0) >> 4);

		//1U konfiguracja wyjść: Bity 0..3 = Wyjście 7, bity 4..7 = Wyjście 8: 0=PWM 400Hz
		m_ctlTypWyjscia7.SetCurSel(chDane[3] & 0x0F);
		m_ctlTypWyjscia8.SetCurSel((chDane[3] & 0xF0) >> 4);

		//1U konfiguracja wyjść:  0=wyjścia 9..16 PWM 50Hz, 1=wyjścia 9..12 PWM 100Hz, 2=wyjścia 9..10 PWM 200Hz, 3=wyjście 9 PWM 400Hz
		m_ctlTypWyjscia9_16.SetCurSel(chDane[4]);
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

	//włącz lub wyłącz funkcje wyjść
	for (int n = 0; n < KANALY_WYJSC_RC; n++)
		AktywujComboFunkcjiWyjscia(n);

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
// Reakcja na przycisk OK
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void WyjsciaRC::OnBnClickedOk()
{
	uint8_t cBlad, cDane[KANALY_WYJSC_RC];
	CString strKomunikat;

	KillTimer(IDT_TIMER_RC);
	if (m_bZmienionoUstawienie)
	{
		cDane[0] = m_ctlTypWyjscia1.GetCurSel() | m_ctlTypWyjscia2.GetCurSel() << 4;
		cDane[1] = m_ctlTypWyjscia3.GetCurSel() | m_ctlTypWyjscia4.GetCurSel() << 4;
		cDane[2] = m_ctlTypWyjscia5.GetCurSel() | m_ctlTypWyjscia6.GetCurSel() << 4;
		cDane[3] = m_ctlTypWyjscia7.GetCurSel() | m_ctlTypWyjscia8.GetCurSel() << 4;
		cDane[4] = m_ctlTypWyjscia9_16.GetCurSel();
		cBlad = getKomunikacja().ZapiszDaneU8FRAM(cDane, 5, FAU_KONF_SERWA12);
	}

	if (m_bZmienionoFunkcjeWyjscRC)
	{
		cDane[0] = m_ctlFunkcjaWyjscia1.GetCurSel();
		cDane[1] = m_ctlFunkcjaWyjscia2.GetCurSel();
		cDane[2] = m_ctlFunkcjaWyjscia3.GetCurSel();
		cDane[3] = m_ctlFunkcjaWyjscia4.GetCurSel();
		cDane[4] = m_ctlFunkcjaWyjscia5.GetCurSel();
		cDane[5] = m_ctlFunkcjaWyjscia6.GetCurSel();
		cDane[6] = m_ctlFunkcjaWyjscia7.GetCurSel();
		cDane[7] = m_ctlFunkcjaWyjscia8.GetCurSel();
		cDane[8] = m_ctlFunkcjaWyjscia9.GetCurSel();
		cDane[9] = m_ctlFunkcjaWyjscia10.GetCurSel();
		cDane[10] = m_ctlFunkcjaWyjscia11.GetCurSel();
		cDane[11] = m_ctlFunkcjaWyjscia12.GetCurSel();
		cDane[12] = m_ctlFunkcjaWyjscia13.GetCurSel();
		cDane[13] = m_ctlFunkcjaWyjscia14.GetCurSel();
		cDane[14] = m_ctlFunkcjaWyjscia15.GetCurSel();
		cDane[15] = m_ctlFunkcjaWyjscia16.GetCurSel();

		cBlad = getKomunikacja().ZapiszDaneU8FRAM(cDane, KANALY_WYJSC_RC, FAU_FUNKCJA_WY_RC);
		if (cBlad != ERR_OK)
		{
			strKomunikat.Format(_T("Błąd nr %d zapisu konfiguracji"), cBlad);
			MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojoj!"), MB_ICONWARNING, 0);
			CDialogEx::OnOK();
		}
	}

	cBlad = getKomunikacja().RekonfigurujWyRC();	//przeładuj konfigurację
	if (cBlad != ERR_OK)
	{
		strKomunikat.Format(_T("Błąd nr %d zapisu konfiguracji"), cBlad);
		MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojoj!"), MB_ICONWARNING, 0);
		CDialogEx::OnOK();
	}

	/*/sprawdź czy była zmiana i czy należy zresetować CM4
	if (m_bZmienionoUstawienie | m_bZmienionoFunkcjeWyjscRC)
	{
		//wyświetl okno z pytaniem czy resetować CM4
		int nRet = MessageBoxExW(this->m_hWnd, _T("Nastąpiła zmiana konfiguracji. \nCzy wykonać reset rdzenia CM4?"), _T("Potrzebny reset aby zmiana przyniosła efekt"), MB_ICONWARNING + MB_YESNO, 0);
		if (nRet == IDYES)
		{
			cBlad = getKomunikacja().ResetujCM4();
			if (cBlad != ERR_OK)
			{
				if (cBlad == 8)	//BLAD_ODMOWA_WYKONANIA
					strKomunikat.Format(_T("Odmowa wykonania resetu ze względów bezpieczeństwa"));
				else
					strKomunikat.Format(_T("Błąd nr %d wykonania polecenia"), cBlad);
				MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojoj!"), MB_ICONWARNING, 0);
				CDialogEx::OnOK();
			}
			cBlad = getKomunikacja().WyslijOK();	//wyłącz polecenie resetujące
		}
	}*/

	CDialogEx::OnOK();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na przycisk Anuluj
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void WyjsciaRC::OnBnClickedCancel()
{
	KillTimer(IDT_TIMER_RC);
	CDialogEx::OnCancel();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na upływ czasu timera powodująca odświeżenia pasków kanałówRC w oknie
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void WyjsciaRC::OnTimer(UINT_PTR nIDEvent)
{
	WstawDaneKanalow();

	CDialogEx::OnTimer(nIDEvent);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Zapisuje nastawy telemetrii pobierajac je z wczesniej zrobionego backupu
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void WyjsciaRC::PrzywrocTelemetrie(uint16_t* Backup)
{

	uint8_t chErr = getKomunikacja().ZapiszOkresTelemetrii(m_sBackupOkresuTelemetrii, LICZBA_ZMIENNYCH_TELEMETRYCZNYCH);
	if (chErr)
	{
		CString strKomunikat;
		strKomunikat.Format(_T("Nie można rekonfigurować telemetrii! \nKod błędu: %d"), chErr);
		MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojojoj!"), MB_ICONEXCLAMATION, 0);
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wlącza lub wyłącza na szaro combo box definiujący funkcję wyjścia
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void WyjsciaRC::AktywujComboFunkcjiWyjscia(int NrWyjscia)
{
	switch (NrWyjscia)
	{
	case 0: GetDlgItem(IDC_COMBO_FUNKCJA_WYJSCIA1)->EnableWindow((m_ctlTypWyjscia1.GetCurSel() >= TWY_SERWO_50HZ) && (m_ctlTypWyjscia1.GetCurSel() <= TWY_DSHOT_1200));	break;
	case 1: GetDlgItem(IDC_COMBO_FUNKCJA_WYJSCIA2)->EnableWindow((m_ctlTypWyjscia2.GetCurSel() >= TWY_SERWO_50HZ) && (m_ctlTypWyjscia2.GetCurSel() <= TWY_DSHOT_1200));	break;
	case 2: GetDlgItem(IDC_COMBO_FUNKCJA_WYJSCIA3)->EnableWindow((m_ctlTypWyjscia3.GetCurSel() >= TWY_SERWO_50HZ) && (m_ctlTypWyjscia3.GetCurSel() <= TWY_DSHOT_1200));	break;
	case 3: GetDlgItem(IDC_COMBO_FUNKCJA_WYJSCIA4)->EnableWindow((m_ctlTypWyjscia4.GetCurSel() >= TWY_SERWO_50HZ) && (m_ctlTypWyjscia4.GetCurSel() <= TWY_DSHOT_1200));	break;

	case 4: GetDlgItem(IDC_COMBO_FUNKCJA_WYJSCIA5)->EnableWindow((m_ctlTypWyjscia5.GetCurSel() >= TWY_SERWO_50HZ) && (m_ctlTypWyjscia5.GetCurSel() <= TWY_DSHOT_1200));	break;
	case 5: GetDlgItem(IDC_COMBO_FUNKCJA_WYJSCIA6)->EnableWindow((m_ctlTypWyjscia6.GetCurSel() >= TWY_SERWO_50HZ) && (m_ctlTypWyjscia6.GetCurSel() <= TWY_DSHOT_1200));	break;
	case 6: GetDlgItem(IDC_COMBO_FUNKCJA_WYJSCIA7)->EnableWindow((m_ctlTypWyjscia7.GetCurSel() >= TWY_SERWO_50HZ) && (m_ctlTypWyjscia7.GetCurSel() <= TWY_DSHOT_1200));	break;
	case 7: GetDlgItem(IDC_COMBO_FUNKCJA_WYJSCIA8)->EnableWindow((m_ctlTypWyjscia8.GetCurSel() >= TWY_SERWO_50HZ) && (m_ctlTypWyjscia8.GetCurSel() <= TWY_DSHOT_1200));	break;

	case 8:  GetDlgItem(IDC_COMBO_FUNKCJA_WYJSCIA9)->EnableWindow((m_ctlTypWyjscia9_16.GetCurSel()  >= TWY_SERWO_50HZ) && (m_ctlTypWyjscia9_16.GetCurSel() <= TWY_PWM_400HZ));	break;
	case 9:  GetDlgItem(IDC_COMBO_FUNKCJA_WYJSCIA10)->EnableWindow((m_ctlTypWyjscia9_16.GetCurSel() >= TWY_SERWO_50HZ) && (m_ctlTypWyjscia9_16.GetCurSel() <= TWY_PWM_200HZ));	break;
	case 10: GetDlgItem(IDC_COMBO_FUNKCJA_WYJSCIA11)->EnableWindow((m_ctlTypWyjscia9_16.GetCurSel() >= TWY_SERWO_50HZ) && (m_ctlTypWyjscia9_16.GetCurSel() <= TWY_PWM_100HZ));	break;
	case 11: GetDlgItem(IDC_COMBO_FUNKCJA_WYJSCIA12)->EnableWindow((m_ctlTypWyjscia9_16.GetCurSel() >= TWY_SERWO_50HZ) && (m_ctlTypWyjscia9_16.GetCurSel() <= TWY_PWM_100HZ));	break;

	case 12: GetDlgItem(IDC_COMBO_FUNKCJA_WYJSCIA13)->EnableWindow((m_ctlTypWyjscia9_16.GetCurSel() >= TWY_SERWO_50HZ) && (m_ctlTypWyjscia9_16.GetCurSel() <= TWY_SERWO_50HZ));	break;
	case 13: GetDlgItem(IDC_COMBO_FUNKCJA_WYJSCIA14)->EnableWindow((m_ctlTypWyjscia9_16.GetCurSel() >= TWY_SERWO_50HZ) && (m_ctlTypWyjscia9_16.GetCurSel() <= TWY_SERWO_50HZ));	break;
	case 14: GetDlgItem(IDC_COMBO_FUNKCJA_WYJSCIA15)->EnableWindow((m_ctlTypWyjscia9_16.GetCurSel() >= TWY_SERWO_50HZ) && (m_ctlTypWyjscia9_16.GetCurSel() <= TWY_SERWO_50HZ));	break;
	case 15: GetDlgItem(IDC_COMBO_FUNKCJA_WYJSCIA16)->EnableWindow((m_ctlTypWyjscia9_16.GetCurSel() >= TWY_SERWO_50HZ) && (m_ctlTypWyjscia9_16.GetCurSel() <= TWY_SERWO_50HZ));	break;
	default: break;
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wstawia zmienne telemetryczne do zmiennych pasków poszczególnych kanałów
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t WyjsciaRC::WstawDaneKanalow()
{
	int nWartoscRC;
	int nIndeksTele = (int)getProtokol().m_vDaneTelemetryczne.size();
	if (nIndeksTele == 0)
		return ERR_NO_DATA_RECEIVED;
	else
		nIndeksTele -= 1;	//indeks jest liczbą danych -1

	//aktualizuj wartości liczbowe kanałów
	for (int n = 0; n < KANALY_WYJSC_RC; n++)
	{
		nWartoscRC = (int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TID_SERWO1 + n];
		strWyjscie[n].Format(_T("%d"), nWartoscRC);
	}

	m_ctlSerwo1.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TID_SERWO1]);
	m_ctlSerwo2.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TID_SERWO2]);
	m_ctlSerwo3.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TID_SERWO3]);
	m_ctlSerwo4.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TID_SERWO4]);
	m_ctlSerwo5.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TID_SERWO5]);
	m_ctlSerwo6.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TID_SERWO6]);
	m_ctlSerwo7.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TID_SERWO7]);
	m_ctlSerwo8.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TID_SERWO8]);
	m_ctlSerwo9.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TID_SERWO9]);
	m_ctlSerwo10.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TID_SERWO10]);
	m_ctlSerwo11.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TID_SERWO11]);
	m_ctlSerwo12.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TID_SERWO12]);
	m_ctlSerwo13.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TID_SERWO13]);
	m_ctlSerwo14.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TID_SERWO14]);
	m_ctlSerwo15.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TID_SERWO15]);
	m_ctlSerwo16.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TID_SERWO16]);
	UpdateData(FALSE);
	return ERR_OK;
}




void WyjsciaRC::OnCbnSelchangeComboSerwo1()
{
	AktywujComboFunkcjiWyjscia(0);
	m_bZmienionoUstawienie = TRUE;
}


void WyjsciaRC::OnCbnSelchangeComboSerwo2()
{
	int nFunkcjaKan2 = m_ctlTypWyjscia2.GetCurSel();
	int nFunkcjaKan3 = m_ctlTypWyjscia3.GetCurSel();
	GetDlgItem(IDC_COMBO_FUNKCJA_WYJSCIA2)->EnableWindow((nFunkcjaKan2 == nFunkcjaKan3) && ((nFunkcjaKan2 == TWY_SERWO_50HZ) || ((nFunkcjaKan2 >= TWY_PWM_400HZ) && (nFunkcjaKan2 <= TWY_DSHOT_1200))));
	m_bZmienionoUstawienie = TRUE;
}


void WyjsciaRC::OnCbnSelchangeComboSerwo3()
{
	int nFunkcjaKan2 = m_ctlTypWyjscia2.GetCurSel();
	int nFunkcjaKan3 = m_ctlTypWyjscia3.GetCurSel();
	GetDlgItem(IDC_COMBO_FUNKCJA_WYJSCIA3)->EnableWindow((nFunkcjaKan3 == nFunkcjaKan2) && ((nFunkcjaKan3 == TWY_SERWO_50HZ) || ((nFunkcjaKan3 >= TWY_PWM_400HZ) && (nFunkcjaKan3 <= TWY_DSHOT_1200))));
	m_bZmienionoUstawienie = TRUE;
}


void WyjsciaRC::OnCbnSelchangeComboSerwo4()
{
	int nFunkcjaKan4 = m_ctlTypWyjscia4.GetCurSel();
	int nFunkcjaKan5 = m_ctlTypWyjscia5.GetCurSel();
	GetDlgItem(IDC_COMBO_FUNKCJA_WYJSCIA4)->EnableWindow((nFunkcjaKan4 == nFunkcjaKan5) && ((nFunkcjaKan4 == TWY_SERWO_50HZ) || ((nFunkcjaKan4 >= TWY_PWM_400HZ) && (nFunkcjaKan4 <= TWY_DSHOT_1200))));
	m_bZmienionoUstawienie = TRUE;
}


void WyjsciaRC::OnCbnSelchangeComboSerwo5()
{
	int nFunkcjaKan4 = m_ctlTypWyjscia4.GetCurSel();
	int nFunkcjaKan5 = m_ctlTypWyjscia5.GetCurSel();
	GetDlgItem(IDC_COMBO_FUNKCJA_WYJSCIA5)->EnableWindow((nFunkcjaKan5 == nFunkcjaKan4) && ((nFunkcjaKan5 == TWY_SERWO_50HZ) || ((nFunkcjaKan5 >= TWY_PWM_400HZ) && (nFunkcjaKan5 <= TWY_DSHOT_1200))));
	m_bZmienionoUstawienie = TRUE;
}


void WyjsciaRC::OnCbnSelchangeComboSerwo6()
{
	int nFunkcjaKan6 = m_ctlTypWyjscia6.GetCurSel();
	int nFunkcjaKan8 = m_ctlTypWyjscia8.GetCurSel();
	GetDlgItem(IDC_COMBO_FUNKCJA_WYJSCIA6)->EnableWindow((nFunkcjaKan6 == nFunkcjaKan8) && ((nFunkcjaKan6 == TWY_SERWO_50HZ) || ((nFunkcjaKan6 >= TWY_PWM_400HZ) && (nFunkcjaKan6 <= TWY_DSHOT_1200))));
	m_bZmienionoUstawienie = TRUE;
}


void WyjsciaRC::OnCbnSelchangeComboSerwo8()
{
	int nFunkcjaKan6 = m_ctlTypWyjscia6.GetCurSel();
	int nFunkcjaKan8 = m_ctlTypWyjscia8.GetCurSel();
	GetDlgItem(IDC_COMBO_FUNKCJA_WYJSCIA8)->EnableWindow((nFunkcjaKan6 == nFunkcjaKan8) && ((nFunkcjaKan8 == TWY_SERWO_50HZ) || ((nFunkcjaKan8 >= TWY_PWM_400HZ) && (nFunkcjaKan8 <= TWY_DSHOT_1200))));
	m_bZmienionoUstawienie = TRUE;
}


void WyjsciaRC::OnCbnSelchangeComboSerwo7()
{
	GetDlgItem(IDC_COMBO_FUNKCJA_WYJSCIA7)->EnableWindow(FALSE);	//zawsze nieaktywne
	m_bZmienionoUstawienie = TRUE;
}


void WyjsciaRC::OnCbnSelchangeComboSerwo9()
{
	for (int n=8; n<16; n++)
		AktywujComboFunkcjiWyjscia(n);
	m_bZmienionoUstawienie = TRUE;
}


void WyjsciaRC::OnCbnSelchangeComboFunkcjaWyjscia1()
{
	m_bZmienionoFunkcjeWyjscRC = TRUE;
}


void WyjsciaRC::OnCbnSelchangeComboFunkcjaWyjscia2()
{
	m_bZmienionoFunkcjeWyjscRC = TRUE;
}


void WyjsciaRC::OnCbnSelchangeComboFunkcjaWyjscia3()
{
	m_bZmienionoFunkcjeWyjscRC = TRUE;
}


void WyjsciaRC::OnCbnSelchangeComboFunkcjaWyjscia4()
{
	m_bZmienionoFunkcjeWyjscRC = TRUE;
}


void WyjsciaRC::OnCbnSelchangeComboFunkcjaWyjscia5()
{
	m_bZmienionoFunkcjeWyjscRC = TRUE;
}


void WyjsciaRC::OnCbnSelchangeComboFunkcjaWyjscia6()
{
	m_bZmienionoFunkcjeWyjscRC = TRUE;
}


void WyjsciaRC::OnCbnSelchangeComboFunkcjaWyjscia7()
{
	m_bZmienionoFunkcjeWyjscRC = TRUE;
}


void WyjsciaRC::OnCbnSelchangeComboFunkcjaWyjscia8()
{
	m_bZmienionoFunkcjeWyjscRC = TRUE;
}


void WyjsciaRC::OnCbnSelchangeComboFunkcjaWyjscia9()
{
	m_bZmienionoFunkcjeWyjscRC = TRUE;
}


void WyjsciaRC::OnCbnSelchangeComboFunkcjaWyjscia10()
{
	m_bZmienionoFunkcjeWyjscRC = TRUE;
}


void WyjsciaRC::OnCbnSelchangeComboFunkcjaWyjscia11()
{
	m_bZmienionoFunkcjeWyjscRC = TRUE;
}


void WyjsciaRC::OnCbnSelchangeComboFunkcjaWyjscia12()
{
	m_bZmienionoFunkcjeWyjscRC = TRUE;
}


void WyjsciaRC::OnCbnSelchangeComboFunkcjaWyjscia13()
{
	m_bZmienionoFunkcjeWyjscRC = TRUE;
}


void WyjsciaRC::OnCbnSelchangeComboFunkcjaWyjscia14()
{
	m_bZmienionoFunkcjeWyjscRC = TRUE;
}


void WyjsciaRC::OnCbnSelchangeComboFunkcjaWyjscia15()
{
	m_bZmienionoFunkcjeWyjscRC = TRUE;
}


void WyjsciaRC::OnCbnSelchangeComboFunkcjaWyjscia16()
{
	m_bZmienionoFunkcjeWyjscRC = TRUE;
}





