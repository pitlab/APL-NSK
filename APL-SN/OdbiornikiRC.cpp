// OdbiornikiRC.cpp: plik implementacji
//

#include "pch.h"
#include "APL-SN.h"
#include "OdbiornikiRC.h"
#include "afxdialogex.h"
#include "sys_def_wspolny.h"
#include "konfig_fram.h"
#include "Errors.h"



// Okno dialogowe OdbiornikiRC

IMPLEMENT_DYNAMIC(OdbiornikiRC, CDialogEx)

OdbiornikiRC::OdbiornikiRC(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ODBIORNIKI_RC, pParent)
	, m_bZmienionoUstawienie(FALSE)
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
END_MESSAGE_MAP()


// Procedury obsługi komunikatów OdbiornikiRC

///////////////////////////////////////////////////////////////////////////////////////////////////
// Inicjalizacja kontrolek okna
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL OdbiornikiRC::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	uint8_t chErr, chDane[6];	//dane są odczytwane czwórkami, więc trzeba zarezerwowac na odczyt minimum 4 bajty
	int nIndeksDronaWRoju;
	uint16_t sOkresTelemetrii[LICZBA_ZMIENNYCH_TELEMETRYCZNYCH];

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
		uint8_t chErr = getKomunikacja().ZapiszOkresTelemetrii(sOkresTelemetrii, LICZBA_ZMIENNYCH_TELEMETRYCZNYCH);
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

	m_ctlTypWyjscia1.InsertString(0, _T("ESC1 400Hz"));
	m_ctlTypWyjscia1.InsertString(1, _T("Serwo1 50Hz"));
	m_ctlTypWyjscia1.InsertString(2, _T("Port I/O PB9"));
	m_ctlTypWyjscia1.InsertString(3, _T("Wyjście S-Bus"));

	m_ctlTypWyjscia2.InsertString(0, _T("ESC2 400Hz"));
	m_ctlTypWyjscia2.InsertString(1, _T("Serwo2 50Hz"));
	m_ctlTypWyjscia2.InsertString(2, _T("Port I/O PB10"));
	m_ctlTypWyjscia2.InsertString(3, _T("CS QSPI"));

	m_ctlTypWyjscia3.InsertString(0, _T("ESC3 400Hz"));
	m_ctlTypWyjscia3.InsertString(1, _T("Serwo2 50Hz"));
	m_ctlTypWyjscia3.InsertString(2, _T("Port I/O PA15"));

	m_ctlTypWyjscia4.InsertString(0, _T("ESC4 400Hz"));
	m_ctlTypWyjscia4.InsertString(1, _T("Serwo2 50Hz"));
	m_ctlTypWyjscia4.InsertString(2, _T("Port I/O PB0"));
	m_ctlTypWyjscia4.InsertString(3, _T("ADC1_INP9"));

	m_ctlTypWyjscia5.InsertString(0, _T("ESC5 400Hz"));
	m_ctlTypWyjscia5.InsertString(1, _T("Serwo2 50Hz"));
	m_ctlTypWyjscia5.InsertString(2, _T("Port I/O PB1"));
	m_ctlTypWyjscia5.InsertString(3, _T("ADC1_INP5"));

	m_ctlTypWyjscia6.InsertString(0, _T("ESC6 400Hz"));
	m_ctlTypWyjscia6.InsertString(1, _T("Serwo2 50Hz"));
	m_ctlTypWyjscia6.InsertString(2, _T("Port I/O PI5"));
	m_ctlTypWyjscia6.InsertString(3, _T("ADC1_INP2"));

	m_ctlTypWyjscia7.InsertString(0, _T("ESC7 400Hz"));
	m_ctlTypWyjscia7.InsertString(1, _T("Serwo2 50Hz"));
	m_ctlTypWyjscia7.InsertString(2, _T("Port I/O PI10"));

	m_ctlTypWyjscia8.InsertString(0, _T("ESC8 400Hz"));
	m_ctlTypWyjscia8.InsertString(1, _T("Serwo2 50Hz"));
	m_ctlTypWyjscia8.InsertString(2, _T("Port I/O PH15"));

	m_ctlTypWyjscia9_16.InsertString(0, _T("ESC 9 400Hz"));
	m_ctlTypWyjscia9_16.InsertString(1, _T("Serwa 9-16 50Hz"));
	m_ctlTypWyjscia9_16.InsertString(2, _T("Port I/O PA8"));
	m_ctlTypWyjscia9_16.InsertString(3, _T("ESC 9-10 200Hz"));
	m_ctlTypWyjscia9_16.InsertString(4, _T("ESC 9-12 100Hz"));

	//odczytaj konfigurację wejść odbiorników i wyjść serw.
	chErr = getKomunikacja().CzytajU8FRAM(chDane, 6, FAU_KONF_ODB_RC);
	if (chErr == ERR_OK)
	{
		//konfiguracja odbiorników RC: Bity 0..3 = RC1, bity 4..7 = RC2: 0=PPM, 1=S-Bus, 2=inne (CS QSPI dla RC2)
		m_ctlOdbiornikRC1.SetCurSel(chDane[0] & 0x0F);
		m_ctlOdbiornikRC2.SetCurSel((chDane[0] & 0xF0) >> 4);

		//1U konfiguracja wyjść: Bity 0..3 = Wyjście 1, bity 4..7 = Wyjście 2: 0=PWM 400Hz, 1=SBus
		m_ctlTypWyjscia1.SetCurSel(chDane[1] & 0x0F);
		m_ctlTypWyjscia2.SetCurSel((chDane[1] & 0xF0)>>4);
		
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
void OdbiornikiRC::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Dodaj tutaj swój kod procedury obsługi komunikatów i/lub wywołaj domyślny
	WstawDaneKanalow();
	CDialogEx::OnTimer(nIDEvent);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na naciśnięcie przycisku OK - zapisuje konfigurcję jeżeli została zmieniona
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void OdbiornikiRC::OnBnClickedOk()
{
	uint8_t chErr, chDane[6];

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
