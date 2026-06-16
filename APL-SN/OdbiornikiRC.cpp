// OdbiornikiRC.cpp: plik implementacji
//

#include "pch.h"
#include "APL-SN.h"
#include "OdbiornikiRC.h"
#include "afxdialogex.h"
#include "konfig_fram.h"
#include "Errors.h"
#include "pid_kanaly.h"


// Okno dialogowe OdbiornikiRC

IMPLEMENT_DYNAMIC(OdbiornikiRC, CDialogEx)

OdbiornikiRC::OdbiornikiRC(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ODBIORNIKI_RC, pParent)
	, m_bZmienionoUstawienie(FALSE)
	, m_bZmienionoKanalDrazkow(FALSE)
	, m_bZmienionoFunkcjeKanalow(FALSE)
	, m_bZmodyfikowanoTelemetrie(FALSE)
	, m_sBackupOkresuTelemetrii{0}
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
	DDX_Control(pDX, IDC_COMBO_RC1, m_ctlOdbiornikRC1);
	DDX_Control(pDX, IDC_COMBO_RC2, m_ctlOdbiornikRC2);

	DDX_Text(pDX, IDC_STAT_MIN_MAX01, strWejscie[0]);
	DDX_Text(pDX, IDC_STAT_MIN_MAX02, strWejscie[1]);
	DDX_Text(pDX, IDC_STAT_MIN_MAX03, strWejscie[2]);
	DDX_Text(pDX, IDC_STAT_MIN_MAX04, strWejscie[3]);
	DDX_Text(pDX, IDC_STAT_MIN_MAX05, strWejscie[4]);
	DDX_Text(pDX, IDC_STAT_MIN_MAX06, strWejscie[5]);
	DDX_Text(pDX, IDC_STAT_MIN_MAX07, strWejscie[6]);
	DDX_Text(pDX, IDC_STAT_MIN_MAX08, strWejscie[7]);
	DDX_Text(pDX, IDC_STAT_MIN_MAX09, strWejscie[8]);
	DDX_Text(pDX, IDC_STAT_MIN_MAX10, strWejscie[9]);
	DDX_Text(pDX, IDC_STAT_MIN_MAX11, strWejscie[10]);
	DDX_Text(pDX, IDC_STAT_MIN_MAX12, strWejscie[11]);
	DDX_Text(pDX, IDC_STAT_MIN_MAX13, strWejscie[12]);
	DDX_Text(pDX, IDC_STAT_MIN_MAX14, strWejscie[13]);
	DDX_Text(pDX, IDC_STAT_MIN_MAX15, strWejscie[14]);
	DDX_Text(pDX, IDC_STAT_MIN_MAX16, strWejscie[15]);

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
}


BEGIN_MESSAGE_MAP(OdbiornikiRC, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_RC1, &OdbiornikiRC::OnCbnSelchangeComboRc1)
	ON_CBN_SELCHANGE(IDC_COMBO_RC2, &OdbiornikiRC::OnCbnSelchangeComboRc2)
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
	ON_BN_CLICKED(IDC_BUT_NORMALIZUJ, &OdbiornikiRC::OnBnClickedButNormalizuj)
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
			if ((n >= TID_RC_KAN1) && (n <= TID_SERWO16))
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
	m_ctlRC1Kan1.SetRange(0, WE_RC_MAX);
	m_ctlRC1Kan2.SetRange(0, WE_RC_MAX);
	m_ctlRC1Kan3.SetRange(0, WE_RC_MAX);
	m_ctlRC1Kan4.SetRange(0, WE_RC_MAX);
	m_ctlRC1Kan5.SetRange(0, WE_RC_MAX);
	m_ctlRC1Kan6.SetRange(0, WE_RC_MAX);
	m_ctlRC1Kan7.SetRange(0, WE_RC_MAX);
	m_ctlRC1Kan8.SetRange(0, WE_RC_MAX);
	m_ctlRC1Kan9.SetRange(0, WE_RC_MAX);
	m_ctlRC1Kan10.SetRange(0, WE_RC_MAX);
	m_ctlRC1Kan11.SetRange(0, WE_RC_MAX);
	m_ctlRC1Kan12.SetRange(0, WE_RC_MAX);
	m_ctlRC1Kan13.SetRange(0, WE_RC_MAX);
	m_ctlRC1Kan14.SetRange(0, WE_RC_MAX);
	m_ctlRC1Kan15.SetRange(0, WE_RC_MAX);
	m_ctlRC1Kan16.SetRange(0, WE_RC_MAX);
	
	m_ctlOdbiornikRC1.InsertString(0, _T("CPPM1"));
	m_ctlOdbiornikRC1.InsertString(1, _T("S-Bus1"));
	m_ctlOdbiornikRC2.InsertString(0, _T("CPPM2"));
	m_ctlOdbiornikRC2.InsertString(1, _T("S-Bus2"));

	for (int n = 0; n < 4; n++)
	{
		strNapis.Format(_T("Kanał %d"), n+1);
		m_ctlKanalPrzechylenia.InsertString(n, strNapis);
		m_ctlKanalPochylenia.InsertString(n, strNapis);
		m_ctlKanalOdchylenia.InsertString(n, strNapis);
		m_ctlKanalWysokosci.InsertString(n, strNapis);
	}

	//wypełnij comboboxy funkcji wejść RC
	for (int n = 0; n < LICZBA_FUNKCJI_RC; n++)
	{
		switch (n)
		{
		case FRC_NIE_TOB_NIC:		strNapis.Format(_T("Nie rób nic")); break;
		case FRC_WLACZ_OD1:			strNapis.Format(_T("Wyjście OD1")); break;		//aktuwuj wyjście otwarty dren 1
		case FRC_WLACZ_OD2:			strNapis.Format(_T("Wyjście OD2")); break;
		case FRC_STROJ_PID_PARAM1:	strNapis.Format(_T("Strojenie PID 1")); break;	//kanał służy do zmiany wybranego parametru 1 regulatorów PID
		case FRC_STROJ_PID_PARAM2:	strNapis.Format(_T("Strojenie PID 2")); break;	//kanał służy do zmiany wybranego parametru 1 regulatorów PID
		case FRC_MOW_WYSOKOSC:		strNapis.Format(_T("Kom. o wysokości")); break;
		case FRC_MOW_NAPIECIE:		strNapis.Format(_T("Kom. o napięciu")); break;
		case FRC_MOW_TEMPERAT:		strNapis.Format(_T("Kom. o temperat.")); break;
		case FRC_MOW_PREDKOSC:		strNapis.Format(_T("Kom. o prędkości")); break;
		case FRC_MOW_KIERUNEK:		strNapis.Format(_T("Kom. o kierunku")); break;
		default:					strNapis.Format(_T("Błędna wartość")); break;
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
	chErr = getKomunikacja().CzytajU8FRAM(chDane, 1, FAU_KONF_ODB_RC);
	if (chErr == ERR_OK)
	{
		//konfiguracja odbiorników RC: Bity 0..3 = RC1, bity 4..7 = RC2: 0=PPM, 1=S-Bus, 2=inne (CS QSPI dla RC2)
		m_ctlOdbiornikRC1.SetCurSel(chDane[0] & 0x0F);
		m_ctlOdbiornikRC2.SetCurSel((chDane[0] & 0xF0) >> 4);

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
	m_ctlRC1Kan1.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TID_RC_KAN1]);
	m_ctlRC1Kan2.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TID_RC_KAN2]);
	m_ctlRC1Kan3.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TID_RC_KAN3]);
	m_ctlRC1Kan4.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TID_RC_KAN4]);
	m_ctlRC1Kan5.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TID_RC_KAN5]);
	m_ctlRC1Kan6.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TID_RC_KAN6]);
	m_ctlRC1Kan7.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TID_RC_KAN7]);
	m_ctlRC1Kan8.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TID_RC_KAN8]);
	m_ctlRC1Kan9.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TID_RC_KAN9]);
	m_ctlRC1Kan10.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TID_RC_KAN10]);
	m_ctlRC1Kan11.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TID_RC_KAN11]);
	m_ctlRC1Kan12.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TID_RC_KAN12]);
	m_ctlRC1Kan13.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TID_RC_KAN13]);
	m_ctlRC1Kan14.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TID_RC_KAN14]);
	m_ctlRC1Kan15.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TID_RC_KAN15]);
	m_ctlRC1Kan16.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TID_RC_KAN16]);

	//aktualizuj wartości min i max
	for (int n = 0; n < KANALY_ODB_RC; n++)
	{
		nWartoscRC = (int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TID_RC_KAN1 + n];
		strWejscie[n].Format(_T("%d"), nWartoscRC);
	}
	UpdateData(FALSE);
	return ERR_OK;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na upływ czasu timera powodująca odświeżenia pasków kanałówRC w oknie
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void OdbiornikiRC::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Dodaj tutaj swój kod procedury obsługi komunikatów i/lub wywołaj domyślny
	WstawDaneKanalow();
	//UstawMinMax();
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
		chErr = getKomunikacja().ZapiszDaneU8FRAM(chDane, 1, FAU_KONF_ODB_RC);
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

	
	chErr = getKomunikacja().RekonfigurujWeRC();	//przeładuj konfigurację
	if (chErr != ERR_OK)
	{
		strKomunikat.Format(_T("Błąd nr %d zapisu konfiguracji"), chErr);
		MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojoj!"), MB_ICONWARNING, 0);
		CDialogEx::OnOK();
	}

	//sprawdź czy była zmiana i czy należy zresetować CM4
	/*if (m_bZmienionoUstawienie | m_bZmienionoKanalDrazkow | m_bZmienionoKanalDrazkow | m_bZmienionoFunkcjeKanalow)
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
	}*/


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






///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na naciśnięcie przycisku Normalizuje wejście
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void OdbiornikiRC::OnBnClickedButNormalizuj()
{
	uint8_t chBłąd;
	int nBłąd;
	CString strKomunikat;

	//włącz zbieranie ekstremów RC w CM4. W aplikacji dane są tylko w celach poglądowych
	chBłąd = getKomunikacja().ZbierajEkstremaWejscRC();
	if (chBłąd != ERR_OK)
	{
		strKomunikat.Format(_T("Błąd nr %d włączenia zbierania ekstremów RC"), chBłąd);
		MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojoj!"), MB_ICONWARNING, 0);
		return;
	}
	chBłąd = getKomunikacja().WyłaczWykonywaniePoleceniaCM4();	//wyłącza cykliczne polecenie właczenia zbierania ekstremów RC. Zbieranie trwa nadal.

	strKomunikat.Format(_T("Poruszaj wszystkimi drążkami, potencjometrami i przełącznikami do obu wartosci skrajnych\n Gdy zakończysz naciśnij OK"));
	nBłąd = MessageBoxExW(this->m_hWnd, strKomunikat, _T("Normalizacja wejść RC!"), MB_OK, 0);
	if (nBłąd == IDOK)
	{
		chBłąd = getKomunikacja().ZapiszEkstremaWejscRC();
		if (chBłąd != ERR_OK)
		{
			strKomunikat.Format(_T("Błąd nr %d zapisu ekstremów RC"), chBłąd);
			MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojoj!"), MB_ICONWARNING, 0);
			return;
		}
	}
	
}
