// OdbiornikiRC.cpp: plik implementacji
//

#include "pch.h"
#include "APL-SN.h"
#include "OdbiornikiRC.h"
#include "afxdialogex.h"
#include "sys_def_wspolny.h"
#include "konfig_fram.h"
#include "Errors.h"
#include "Komunikacja/Komunikacja.h"


// Okno dialogowe OdbiornikiRC

IMPLEMENT_DYNAMIC(OdbiornikiRC, CDialogEx)

OdbiornikiRC::OdbiornikiRC(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ODBIORNIKI_RC, pParent)
	, m_bZmienionoUstawienie(FALSE)
{

}

OdbiornikiRC::~OdbiornikiRC()
{
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
	DDX_Control(pDX, IDC_PROGRESS17, m_ctlRC2Kan1);
	DDX_Control(pDX, IDC_PROGRESS18, m_ctlRC2Kan2);
	DDX_Control(pDX, IDC_PROGRESS19, m_ctlRC2Kan3);
	DDX_Control(pDX, IDC_PROGRESS20, m_ctlRC2Kan4);
	DDX_Control(pDX, IDC_PROGRESS21, m_ctlRC2Kan5);
	DDX_Control(pDX, IDC_PROGRESS22, m_ctlRC2Kan6);
	DDX_Control(pDX, IDC_PROGRESS23, m_ctlRC2Kan7);
	DDX_Control(pDX, IDC_PROGRESS24, m_ctlRC2Kan8);
	DDX_Control(pDX, IDC_PROGRESS25, m_ctlRC2Kan9);
	DDX_Control(pDX, IDC_PROGRESS26, m_ctlRC2Kan10);
	DDX_Control(pDX, IDC_PROGRESS27, m_ctlRC2Kan11);
	DDX_Control(pDX, IDC_PROGRESS28, m_ctlRC2Kan12);
	DDX_Control(pDX, IDC_PROGRESS29, m_ctlRC2Kan13);
	DDX_Control(pDX, IDC_PROGRESS30, m_ctlRC2Kan14);
	DDX_Control(pDX, IDC_PROGRESS31, m_ctlRC2Kan15);
	DDX_Control(pDX, IDC_PROGRESS32, m_ctlRC2Kan16);
	DDX_Control(pDX, IDC_RADIO_PPM1, m_ctlPPM1);
	DDX_Control(pDX, IDC_RADIO_SBUS1, m_ctlSBus1);
	DDX_Control(pDX, IDC_RADIO_PPM2, m_ctlPPM2);
	DDX_Control(pDX, IDC_RADIO_SBUS2, m_ctlSbus2);
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
	DDX_Control(pDX, IDC_COMBO1, m_ctlTypWyjscia1);
	DDX_Control(pDX, IDC_COMBO2, m_ctlTypWyjscia2);
	DDX_Control(pDX, IDC_COMBO3, m_ctlTypWyjscia3);
	DDX_Control(pDX, IDC_COMBO4, m_ctlTypWyjscia4);
	DDX_Control(pDX, IDC_COMBO5, m_ctlTypWyjscia5);
	DDX_Control(pDX, IDC_COMBO6, m_ctlTypWyjscia6);
	DDX_Control(pDX, IDC_COMBO7, m_ctlTypWyjscia7);
	DDX_Control(pDX, IDC_COMBO8, m_ctlTypWyjscia8);
	DDX_Control(pDX, IDC_COMBO9, m_ctlTypWyjscia9_16);
	DDX_Control(pDX, IDC_RADIO_RC23, m_ctlRC2_QSPI);
}


BEGIN_MESSAGE_MAP(OdbiornikiRC, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_PPM1, &OdbiornikiRC::OnBnClickedRadioPpm1)
	ON_BN_CLICKED(IDC_RADIO_SBUS1, &OdbiornikiRC::OnBnClickedRadioSbus1)
	ON_BN_CLICKED(IDC_RADIO_PPM2, &OdbiornikiRC::OnBnClickedRadioPpm2)
	ON_BN_CLICKED(IDC_RADIO_SBUS2, &OdbiornikiRC::OnBnClickedRadioSbus2)
	ON_BN_CLICKED(IDC_RADIO_RC23, &OdbiornikiRC::OnBnClickedRadioRc23)
	ON_BN_CLICKED(IDOK, &OdbiornikiRC::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO1, &OdbiornikiRC::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, &OdbiornikiRC::OnCbnSelchangeCombo2)
	ON_CBN_SELCHANGE(IDC_COMBO3, &OdbiornikiRC::OnCbnSelchangeCombo3)
	ON_CBN_SELCHANGE(IDC_COMBO4, &OdbiornikiRC::OnCbnSelchangeCombo4)
	ON_CBN_SELCHANGE(IDC_COMBO5, &OdbiornikiRC::OnCbnSelchangeCombo5)
	ON_CBN_SELCHANGE(IDC_COMBO6, &OdbiornikiRC::OnCbnSelchangeCombo6)
	ON_CBN_SELCHANGE(IDC_COMBO7, &OdbiornikiRC::OnCbnSelchangeCombo7)
	ON_CBN_SELCHANGE(IDC_COMBO8, &OdbiornikiRC::OnCbnSelchangeCombo8)
	ON_CBN_SELCHANGE(IDC_COMBO9, &OdbiornikiRC::OnCbnSelchangeCombo9)
	
END_MESSAGE_MAP()


// Procedury obsługi komunikatów OdbiornikiRC

///////////////////////////////////////////////////////////////////////////////////////////////////
// Inicjalizacja kontrolek okna
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL OdbiornikiRC::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	uint8_t chErr, chDane[4];	//dane są odczytwane czwórkami, więc trzeba zarezerwowac na odczyt minimum 4 bajty


	//odczytaj konfigurację odbiorników
	chErr = getKomunikacja().CzytajFRAM(chDane, 1, FAU_KONF_ODB_RC);
	if (chErr == ERR_OK)
	{
		m_ctlPPM1.SetCheck(chDane[0] & 0x01);
		m_ctlSBus1.SetCheck(chDane[0] & 0x02);
		m_ctlPPM2.SetCheck(chDane[0] & 0x10);
		m_ctlSbus2.SetCheck(chDane[0] & 0x20);
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
	m_ctlRC2Kan1.SetRange(PPM_MIN, PPM_MAX);
	m_ctlRC2Kan2.SetRange(PPM_MIN, PPM_MAX);
	m_ctlRC2Kan3.SetRange(PPM_MIN, PPM_MAX);
	m_ctlRC2Kan4.SetRange(PPM_MIN, PPM_MAX);
	m_ctlRC2Kan5.SetRange(PPM_MIN, PPM_MAX);
	m_ctlRC2Kan6.SetRange(PPM_MIN, PPM_MAX);
	m_ctlRC2Kan7.SetRange(PPM_MIN, PPM_MAX);
	m_ctlRC2Kan8.SetRange(PPM_MIN, PPM_MAX);
	m_ctlRC2Kan9.SetRange(PPM_MIN, PPM_MAX);
	m_ctlRC2Kan10.SetRange(PPM_MIN, PPM_MAX);
	m_ctlRC2Kan11.SetRange(PPM_MIN, PPM_MAX);
	m_ctlRC2Kan12.SetRange(PPM_MIN, PPM_MAX);
	m_ctlRC2Kan13.SetRange(PPM_MIN, PPM_MAX);
	m_ctlRC2Kan14.SetRange(PPM_MIN, PPM_MAX);
	m_ctlRC2Kan15.SetRange(PPM_MIN, PPM_MAX);
	m_ctlRC2Kan16.SetRange(PPM_MIN, PPM_MAX);

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


	m_ctlTypWyjscia1.InsertString(0, _T("ESC1 400Hz"));
	m_ctlTypWyjscia1.InsertString(1, _T("Wyjście S-Bus1"));
	m_ctlTypWyjscia1.SetCurSel(0);

	m_ctlTypWyjscia2.InsertString(0, _T("ESC2 400Hz"));
	m_ctlTypWyjscia2.InsertString(1, _T("Wyjście S-Bus2"));
	m_ctlTypWyjscia2.SetCurSel(0);

	m_ctlTypWyjscia3.InsertString(0, _T("ESC3 400Hz"));
	m_ctlTypWyjscia3.SetCurSel(0);

	m_ctlTypWyjscia4.InsertString(0, _T("ESC4 400Hz"));
	m_ctlTypWyjscia4.InsertString(1, _T("ADC? "));
	m_ctlTypWyjscia4.SetCurSel(0);

	m_ctlTypWyjscia5.InsertString(0, _T("ESC5 400Hz"));
	m_ctlTypWyjscia5.InsertString(1, _T("ADC? "));
	m_ctlTypWyjscia5.SetCurSel(0);

	m_ctlTypWyjscia6.InsertString(0, _T("ESC6 400Hz"));
	m_ctlTypWyjscia6.SetCurSel(0);

	m_ctlTypWyjscia7.InsertString(0, _T("ESC7 400Hz"));
	m_ctlTypWyjscia7.SetCurSel(0);

	m_ctlTypWyjscia8.InsertString(0, _T("ESC8 400Hz"));
	m_ctlTypWyjscia8.SetCurSel(0);

	m_ctlTypWyjscia9_16.InsertString(0, _T("Serwa 9-16 50Hz"));
	m_ctlTypWyjscia9_16.InsertString(1, _T("ESC 9-12 100Hz"));
	m_ctlTypWyjscia9_16.InsertString(2, _T("ESC 9-10 200Hz"));
	m_ctlTypWyjscia9_16.InsertString(3, _T("ESC 9 400Hz"));
	m_ctlTypWyjscia9_16.SetCurSel(0);
	WstawDaneKanalow();
	

	UpdateData(FALSE);
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
	m_ctlRC1Kan1.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC1_KAN1]);
	m_ctlRC1Kan2.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC1_KAN2]);
	m_ctlRC1Kan3.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC1_KAN3]);
	m_ctlRC1Kan4.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC1_KAN4]);
	m_ctlRC1Kan5.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC1_KAN5]);
	m_ctlRC1Kan6.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC1_KAN6]);
	m_ctlRC1Kan7.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC1_KAN7]);
	m_ctlRC1Kan8.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC1_KAN8]);
	m_ctlRC1Kan9.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC1_KAN9]);
	m_ctlRC1Kan10.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC1_KAN10]);
	m_ctlRC1Kan11.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC1_KAN11]);
	m_ctlRC1Kan12.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC1_KAN12]);
	m_ctlRC1Kan13.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC1_KAN13]);
	m_ctlRC1Kan14.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC1_KAN14]);
	m_ctlRC1Kan15.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC1_KAN15]);
	m_ctlRC1Kan16.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC1_KAN16]);

	m_ctlRC2Kan1.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC2_KAN1]);
	m_ctlRC2Kan2.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC2_KAN2]);
	m_ctlRC2Kan3.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC2_KAN3]);
	m_ctlRC2Kan4.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC2_KAN4]);
	m_ctlRC2Kan5.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC2_KAN5]);
	m_ctlRC2Kan6.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC2_KAN6]);
	m_ctlRC2Kan7.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC2_KAN7]);
	m_ctlRC2Kan8.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC2_KAN8]);
	m_ctlRC2Kan9.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC2_KAN9]);
	m_ctlRC2Kan10.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC2_KAN10]);
	m_ctlRC2Kan11.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC2_KAN11]);
	m_ctlRC2Kan12.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC2_KAN12]);
	m_ctlRC2Kan13.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC2_KAN13]);
	m_ctlRC2Kan14.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC2_KAN14]);
	m_ctlRC2Kan15.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC2_KAN15]);
	m_ctlRC2Kan16.SetPos((int)getProtokol().m_vDaneTelemetryczne[nIndeksTele].dane[TELEID_RC2_KAN16]);

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

	return ERR_OK;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na naciśnięcie radiobuttona PPM1
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void OdbiornikiRC::OnBnClickedRadioPpm1()
{
	m_bZmienionoUstawienie = TRUE;
	m_ctlPPM1.SetCheck(1);
	m_ctlSBus1.SetCheck(0);
	UpdateData(FALSE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na naciśnięcie radiobuttona SBus1
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void OdbiornikiRC::OnBnClickedRadioSbus1()
{
	m_bZmienionoUstawienie = TRUE;
	m_ctlPPM1.SetCheck(0);
	m_ctlSBus1.SetCheck(1);
	UpdateData(FALSE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na naciśnięcie radiobuttona PPM2
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void OdbiornikiRC::OnBnClickedRadioPpm2()
{
	m_bZmienionoUstawienie = TRUE;
	m_ctlPPM2.SetCheck(1);
	m_ctlSbus2.SetCheck(0);
	m_ctlRC2_QSPI.SetCheck(0);
	UpdateData(FALSE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na naciśnięcie radiobuttona SBus2
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void OdbiornikiRC::OnBnClickedRadioSbus2()
{
	m_bZmienionoUstawienie = TRUE;
	m_ctlPPM2.SetCheck(0);
	m_ctlSbus2.SetCheck(1);
	m_ctlRC2_QSPI.SetCheck(0);
	UpdateData(FALSE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na naciśnięcie radiobuttona RC2 CS QSPI
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void OdbiornikiRC::OnBnClickedRadioRc23()
{
	m_bZmienionoUstawienie = TRUE;
	m_ctlPPM2.SetCheck(0);
	m_ctlSbus2.SetCheck(0);
	m_ctlRC2_QSPI.SetCheck(1);
	UpdateData(FALSE);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na naciśnięcie przycisku OK
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void OdbiornikiRC::OnBnClickedOk()
{
	if (m_bZmienionoUstawienie)
	{

	}

	CDialogEx::OnOK();
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



