// KonfigPID.cpp: plik implementacji
//

#include "pch.h"
#include "APL-SN.h"
#include "KonfigPID.h"
#include "afxdialogex.h"
#include <math.h>
#include "Errors.h"
#include "pid_kanaly.h"
#include "sys_def_wspolny.h"
#include <assert.h>

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
	, m_strMinWyj1(_T(""))
	, m_strMaxWyj1(_T(""))
	, m_strMinWyj2(_T(""))
	, m_strMaxWyj2(_T(""))
	, m_strPrzesunięcieWartościZadanej1(_T(""))
	, m_strPrzesunięcieWartościZadanej2(_T(""))
	, m_strSkalaWartZadanejStab(_T(""))
	, m_strSkalaWartZadanejAkro(_T(""))
	, m_bKatowy(FALSE)
	, m_nBiezacyParametr(0)
	, m_nPodstFiltraD1(0)
	, m_nPodstFiltraD2(0)
	, m_nProcWyprzedzenia1(0)
	, m_nProcWyprzedzenia2(0)
	, m_nPodstFiltraWartZadanej1(0)
	, m_nPodstFiltraWartZadanej2(0)
	, m_strPodstFiltraD1(_T(""))
	, m_strPodstFiltraD2(_T(""))
	, m_bTrybRegulacjiWylaczony(FALSE)
	, m_bTrybRegulacjiReczny(FALSE)
	, m_bTrybRegulacjiAkro(FALSE)
	, m_bTrybRegulacjiStab(FALSE)
	, m_bTrybRegulacjiAuto(FALSE)
	, m_strJednostkaStab(_T(""))
	, m_strJednostkaAkro(_T(""))
	, m_strWartoscMinParametru1(_T(""))
	, m_strWartoscMaxParametru1(_T(""))
	, m_strWartoscMinParametru2(_T(""))
	, m_strWartoscMaxParametru2(_T(""))
	, m_bZmienionyTrybRegulacji(FALSE)
	, m_bZmienioneParametryStrojenia(FALSE)
	, m_bZmienioneWartościStrojenia(FALSE)
	, m_strOpisParametru1(_T(""))
	, m_strOpisParametru2(_T(""))
{

}

KonfigPID::~KonfigPID()
{
}

void KonfigPID::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_KANAL_PID, m_ctrlKanalPID);
	DDX_Text(pDX, IDC_EDIT_KP1, m_strKP1);
	DDX_Text(pDX, IDC_EDIT_KP2, m_strKP2);
	DDX_Text(pDX, IDC_EDIT_TI1, m_strTI1);
	DDX_Text(pDX, IDC_EDIT_TI2, m_strTI2);	
	DDX_Text(pDX, IDC_EDIT_TD1, m_strTD1);
	DDX_Text(pDX, IDC_EDIT_TD2, m_strTD2);
	DDX_Text(pDX, IDC_EDIT_LIMIT_CALKI1, m_strOgrCalki1);
	DDX_Text(pDX, IDC_EDIT_LIMIT_CALKI2, m_strOgrCalki2);
	DDX_Text(pDX, IDC_EDIT_MIN_WY1, m_strMinWyj1);
	DDX_Text(pDX, IDC_EDIT_MAX_WY1, m_strMaxWyj1);
	DDX_Text(pDX, IDC_EDIT_MIN_WY2, m_strMinWyj2);
	DDX_Text(pDX, IDC_EDIT_MAX_WY2, m_strMaxWyj2);
	DDX_Text(pDX, IDC_EDIT_PARAMETR_MIN1, m_strWartoscMinParametru1);
	DDX_Text(pDX, IDC_EDIT_PARAMETR_MIN2, m_strWartoscMinParametru2);
	DDX_Text(pDX, IDC_EDIT_PARAMETR_MAX1, m_strWartoscMaxParametru1);
	DDX_Text(pDX, IDC_EDIT_PARAMETR_MAX2, m_strWartoscMaxParametru2);
	DDX_Text(pDX, IDC_EDIT_MNOZNIK_WART_ZAD_STAB, m_strSkalaWartZadanejStab);
	DDX_Text(pDX, IDC_EDIT_MNOZNIK_WART_ZAD_AKRO, m_strSkalaWartZadanejAkro);
	DDX_Text(pDX, IDC_EDIT_PRZESUNIECIE_WART_ZADANEJ1, m_strPrzesunięcieWartościZadanej1);
	DDX_Text(pDX, IDC_EDIT_PRZESUNIECIE_WART_ZADANEJ2, m_strPrzesunięcieWartościZadanej2);
	DDX_Check(pDX, IDC_CHECK_KATOWY1, m_bKatowy);

	DDX_Text(pDX, IDC_STATIC_FILTR_D1, m_strPodstFiltraD1);
	DDX_Text(pDX, IDC_STATIC_FILTR_D2, m_strPodstFiltraD2);
	DDX_Control(pDX, IDC_SLIDER_FILTR_D1, m_ctlSlidPodstCzasuFiltraD1);
	DDX_Control(pDX, IDC_SLIDER_FILTR_D2, m_ctlSlidPodstCzasuFiltraD2);

	DDX_Text(pDX, IDC_STATIC_FILTR_WZAD1, m_strPodstFiltraWZad1);
	DDX_Text(pDX, IDC_STATIC_FILTR_WZAD2, m_strPodstFiltraWZad2);
	DDX_Control(pDX, IDC_SLIDER_FILTR_WZAD1, m_ctlSlidPodstFiltraWartZad1);
	DDX_Control(pDX, IDC_SLIDER_FILTR_WZAD2, m_ctlSlidPodstFiltraWartZad2);

	DDX_Text(pDX, IDC_STATIC_PROC_WYPRZEDZENIA1, m_strProcWyprzedzenia1);
	DDX_Control(pDX, IDC_SLIDER_PROC_WYPRZEDZENIA1, m_ctlSlidProcWyprzedzenia1);
	DDX_Text(pDX, IDC_STATIC_PROC_WYPRZEDZENIA2, m_strProcWyprzedzenia2);
	DDX_Control(pDX, IDC_SLIDER_PROC_WYPRZEDZENIA2, m_ctlSlidProcWyprzedzenia2);
	
	DDX_Check(pDX, IDC_RADIO_REG_WYLACZ, m_bTrybRegulacjiWylaczony);
	DDX_Check(pDX, IDC_RADIO_REG_RECZNY, m_bTrybRegulacjiReczny);
	DDX_Check(pDX, IDC_RADIO_REG_AKRO, m_bTrybRegulacjiAkro);
	DDX_Check(pDX, IDC_RADIO_REG_STAB, m_bTrybRegulacjiStab);
	DDX_Check(pDX, IDC_RADIO_REG_AUTO, m_bTrybRegulacjiAuto);
	DDX_Text(pDX, IDC_STAT_JEDNOSTKA_STAB, m_strJednostkaStab);
	DDX_Text(pDX, IDC_STAT_JEDNOSTKA_STAB2, m_strJednostkaStab2);
	DDX_Text(pDX, IDC_STAT_JEDNOSTKA_AKRO, m_strJednostkaAkro);	

	DDX_Control(pDX, IDC_COMBO_STROJONY_PARAMETR1, m_ctlStrojonyParametr1);
	DDX_Control(pDX, IDC_COMBO_STROJONY_PARAMETR2, m_ctlStrojonyParametr2);
	DDX_Text(pDX, IDC_STATIC_PARAMETR1, m_strOpisParametru1);
	DDX_Text(pDX, IDC_STATIC_PARAMETR2, m_strOpisParametru2);
}	


BEGIN_MESSAGE_MAP(KonfigPID, CDialogEx)
	ON_BN_CLICKED(IDOK, &KonfigPID::OnBnClickedOk)
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
	ON_EN_CHANGE(IDC_EDIT_MNOZNIK_WART_ZAD_STAB, &KonfigPID::OnEnChangeEditSkalaWartZadStab)
	ON_EN_CHANGE(IDC_EDIT_MNOZNIK_WART_ZAD_AKRO, &KonfigPID::OnEnChangeEditSkalaWartZadAkro)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_FILTR_D1, &KonfigPID::OnNMCustomdrawSliderFiltrD1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_FILTR_D2, &KonfigPID::OnNMCustomdrawSliderFiltrD2)
	ON_EN_CHANGE(IDC_EDIT_LIMIT_CALKI2, &KonfigPID::OnEnChangeEditLimitCalki2)
	ON_EN_CHANGE(IDC_EDIT_LIMIT_CALKI1, &KonfigPID::OnEnChangeEditLimitCalki1)
	ON_BN_CLICKED(IDC_CHECK_KATOWY1, &KonfigPID::OnBnClickedCheckKatowy)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_FILTR_D1, &KonfigPID::OnNMReleasedcaptureSliderFiltrD1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_FILTR_D2, &KonfigPID::OnNMReleasedcaptureSliderFiltrD2)
	ON_BN_CLICKED(IDC_RADIO_REG_WYLACZ, &KonfigPID::OnBnClickedRadioRegWylacz)
	ON_BN_CLICKED(IDC_RADIO_REG_RECZNY, &KonfigPID::OnBnClickedRadioRegReczny)
	ON_BN_CLICKED(IDC_RADIO_REG_AKRO, &KonfigPID::OnBnClickedRadioRegAkro)
	ON_BN_CLICKED(IDC_RADIO_REG_STAB, &KonfigPID::OnBnClickedRadioRegStab)
	ON_BN_CLICKED(IDC_RADIO_REG_AUTO, &KonfigPID::OnBnClickedRadioRegAuto)
	ON_BN_CLICKED(IDC_BUT_USTAW_DOMYSLNE, &KonfigPID::OnBnClickedButUstawDomyslne)
	ON_CBN_SELCHANGE(IDC_COMBO_STROJONY_PARAMETR1, &KonfigPID::OnCbnSelchangeComboStrojonyParametr1)
	ON_EN_CHANGE(IDC_EDIT_PARAMETR_MIN1, &KonfigPID::OnEnChangeEditParametrMin1)
	ON_EN_CHANGE(IDC_EDIT_PARAMETR_MAX1, &KonfigPID::OnEnChangeEditParametrMax1)
	ON_EN_CHANGE(IDC_EDIT_PARAMETR_MIN2, &KonfigPID::OnEnChangeEditParametrMin2)
	ON_EN_CHANGE(IDC_EDIT_PARAMETR_MAX2, &KonfigPID::OnEnChangeEditParametrMax2)
	ON_CBN_SELCHANGE(IDC_COMBO_STROJONY_PARAMETR2, &KonfigPID::OnCbnSelchangeComboStrojonyParametr2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_FILTR_WZAD1, &KonfigPID::OnNMCustomdrawSliderFiltrWzad1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_FILTR_WZAD2, &KonfigPID::OnNMCustomdrawSliderFiltrWzad2)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_FILTR_WZAD1, &KonfigPID::OnReleasedcaptureSliderFiltrWzad1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_FILTR_WZAD2, &KonfigPID::OnReleasedcaptureSliderFiltrWzad2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_PROC_WYPRZEDZENIA1, &KonfigPID::OnNMCustomdrawSliderProcWyprzedzenia1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_PROC_WYPRZEDZENIA2, &KonfigPID::OnNMCustomdrawSliderProcWyprzedzenia2)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_PROC_WYPRZEDZENIA1, &KonfigPID::OnReleasedcaptureSliderProcWyprzedzenia1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_PROC_WYPRZEDZENIA2, &KonfigPID::OnReleasedcaptureSliderProcWyprzedzenia2)
	ON_EN_CHANGE(IDC_EDIT_PRZESUNIECIE_WART_ZADANEJ1, &KonfigPID::OnEnChangeEditPrzesuniecieWartZadanej1)
	ON_EN_CHANGE(IDC_EDIT_PRZESUNIECIE_WART_ZADANEJ2, &KonfigPID::OnEnChangeEditPrzesuniecieWartZadanej2)
END_MESSAGE_MAP()


// Procedury obsługi komunikatów KonfigPID


///////////////////////////////////////////////////////////////////////////////////////////////////
// Inicjalizacja zawartości okna dialogowego
// Zwraca: TRUE
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL KonfigPID::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	float fDane[ROZMIAR_REG_PID / 4];
	uint8_t chDane[KANALY_FUNKCYJNE];
	uint8_t chErr;
	CString strKomunikat, strParametr;
	uint8_t chKanałParametru1 = 0, chKanałParametru2 = 0;

	m_ctrlKanalPID.InsertItem(PRZE, _T("Przechylenie"));	//regulator sterowania przechyleniem (lotkami w samolocie)
	m_ctrlKanalPID.InsertItem(POCH, _T("Pochylenie"));		//regulator sterowania pochyleniem (sterem wysokości)
	m_ctrlKanalPID.InsertItem(ODCH, _T("Odchylenie"));		//regulator sterowania obrotem (sterem kierunku)
	m_ctrlKanalPID.InsertItem(WYSO, _T("Wysokość"));		//regulator sterowania wysokością
	m_ctrlKanalPID.InsertItem(POZN, _T("Nawigacja N"));		//regulator sterowania prędkością i położeniem północnym
	m_ctrlKanalPID.InsertItem(POZE, _T("Nawigacja E"));		//regulator sterowania prędkością i położeniem wschodnim 

	m_nBiezacyParametr = m_ctrlKanalPID.GetCurSel();

	for (int n = 0; n < LICZBA_PID; n++)
	{
		//polecenie może odczytywać 48 bajty (12 floatów)
		chErr = getKomunikacja().CzytajFloatFRAM(fDane, 10, FAU_PID_KP + n * ROZMIAR_REG_PID);		
		if (chErr == ERR_OK)
		{
			m_stPID[n].fKp = fDane[0];			//FAU_PID_P0 wzmocnienienie członu P regulatora 0
			if (n & 0x01)
				m_stPID[n].fTi = 0;			//ani członu całkującego
			else
				m_stPID[n].fTi = fDane[1];			//FAU_PID_I0 wzmocnienienie członu I regulatora 0

			m_stPID[n].fTd = fDane[2];			//FAU_PID_D0 wzmocnienienie członu D regulatora 0
			m_stPID[n].fOgrCalki = fDane[3];	//FAU_PID_OGR_I0 górna granica wartości całki członu I regulatora 0
			m_stPID[n].fMinWyj = fDane[4];		//FAU_PID_MIN_WY0 minimalna wartość wyjścia
			m_stPID[n].fMaxWyj = fDane[5];		//FAU_PID_MAX_WY0 maksymalna wartość wyjścia
			m_stPID[n].fMnożnikWartZadanej = fDane[6];	 //FAU_MNOZN_WZAD  mnożnik wartości zadanej
			if (n & 0x01)
				m_stPID[n].fPrzesunięcieWartościZadanej = 0;	//nieparzyste PID czyli regulatory pochodnej nie mają stałego przesunięcia
			else
				m_stPID[n].fPrzesunięcieWartościZadanej = fDane[7];	//FAU_PID_STALE_WYPRZ stała wartość podawana na wejście wyprzedzające (umożliwia lot pod niezerowym kątem)
			m_stPID[n].fWolne1 = fDane[8];					//FAU_PID1 wolne

			m_unia8_32.daneFloat = fDane[9];	//w ostatniej liczbie typu float prześlij 4 zmienne 8 bitowe. Dostęp do nich jest przez unię
			if (n & 0x01)	//regulatory nieparzyste obsługują pochodną, czyli prędkość katową lub liniową, więc nie mogą być kątowe
				m_stPID[n].bKatowy = FALSE;
			else
				m_stPID[n].bKatowy = ((m_unia8_32.dane8[0] & PID_KATOWY) == PID_KATOWY);
			m_stPID[n].cPodstFiltraD = m_unia8_32.dane8[1];				//1U Podstawa filtra IIR błędu do liczenia członu różniczkującego
			m_stPID[n].cPodstawaFiltraWartZad = m_unia8_32.dane8[2];	//1U Podstawa filtra IIR wartości zadanej do liczenia członu wyprzedzajacego 
			m_stPID[n].cProcWartZadWyprz = m_unia8_32.dane8[3];			//1U procentowa wartość zmiany wartości zadanej podawana na wejście wyprzedzenia			
			m_stPID[n].bZmieniony = FALSE;
		}
		else
		{
			if (chErr == ERR_BRAK_POTWIERDZ)
			{
				strKomunikat.Format(_T("Błąd odczytu konfiguracji, brak potwierdzenia"));
				MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojoj!"), MB_ICONWARNING, 0);
			}
			return FALSE;
		}
	}
	
	chErr = getKomunikacja().CzytajU8FRAM(chDane, LICZBA_REG_PARAM, FAU_TRYB_REG);
	if (chErr == ERR_OK)
	{
		for (uint8_t n = 0; n < LICZBA_REG_PARAM; n++)
			m_chTrybRegulacji[n] = chDane[n];
		m_bZmienionyTrybRegulacji = FALSE;
	}
	m_ctlSlidPodstCzasuFiltraD1.SetRange(0, 255);
	m_ctlSlidPodstCzasuFiltraD2.SetRange(0, 255);
	m_ctlSlidPodstFiltraWartZad1.SetRange(0, 255);
	m_ctlSlidPodstFiltraWartZad2.SetRange(0, 255);
	m_ctlSlidProcWyprzedzenia2.SetRange(0, 100);

	//inicjalizacja strojenia parametrów PID
	strParametr.Format(_T("Strojenie wyłączone"));
	m_ctlStrojonyParametr1.InsertString(STRP_NIC, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_NIC, strParametr);
	strParametr.Format(_T("KP kąta przechylenia"));
	m_ctlStrojonyParametr1.InsertString(STRP_KATA_PRZE_KP, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_KATA_PRZE_KP, strParametr);
	strParametr.Format(_T("TI kąta przechylenia"));
	m_ctlStrojonyParametr1.InsertString(STRP_KATA_PRZE_TI, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_KATA_PRZE_TI, strParametr);
	strParametr.Format(_T("TD kąta przechylenia"));
	m_ctlStrojonyParametr1.InsertString(STRP_KATA_PRZE_TD, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_KATA_PRZE_TD, strParametr);
	strParametr.Format(_T("Podst.filtra D kąta przech"));
	m_ctlStrojonyParametr1.InsertString(STRP_KATA_PRZE_FD, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_KATA_PRZE_FD, strParametr);
	strParametr.Format(_T("Podst.filtra W.Zad kąta prze"));
	m_ctlStrojonyParametr1.InsertString(STRP_KATA_PRZE_FWZ, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_KATA_PRZE_FWZ, strParametr);
	strParametr.Format(_T("%% Wyprz. kąta przechylenia"));
	m_ctlStrojonyParametr1.InsertString(STRP_KATA_PRZE_WYPRZ, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_KATA_PRZE_WYPRZ, strParametr);

	strParametr.Format(_T("KP prędk.kąt. przechylenia"));
	m_ctlStrojonyParametr1.InsertString(STRP_PRED_PRZE_KP, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_PRED_PRZE_KP, strParametr);
	strParametr.Format(_T("TI prędk.kąt. przechylenia"));
	m_ctlStrojonyParametr1.InsertString(STRP_PRED_PRZE_TI, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_PRED_PRZE_TI, strParametr);
	strParametr.Format(_T("TD prędk.kąt. przechylenia"));
	m_ctlStrojonyParametr1.InsertString(STRP_PRED_PRZE_TD, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_PRED_PRZE_TD, strParametr);
	strParametr.Format(_T("Podst.filtra D prędk.kąt.prze"));
	m_ctlStrojonyParametr1.InsertString(STRP_PRED_PRZE_FD, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_PRED_PRZE_FD, strParametr);
	strParametr.Format(_T("Podst.filtra W.Zad prędk.kąt.prze"));
	m_ctlStrojonyParametr1.InsertString(STRP_PRED_PRZE_FWZ, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_PRED_PRZE_FWZ, strParametr);
	strParametr.Format(_T("%% Wyprz.prędk.kąt. prze"));
	m_ctlStrojonyParametr1.InsertString(STRP_PRED_PRZE_WYPRZ, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_PRED_PRZE_WYPRZ, strParametr);


	strParametr.Format(_T("KP kąta pochylenia"));
	m_ctlStrojonyParametr1.InsertString(STRP_KATA_POCH_KP, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_KATA_POCH_KP, strParametr);
	strParametr.Format(_T("TI kąta pochylenia"));
	m_ctlStrojonyParametr1.InsertString(STRP_KATA_POCH_TI, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_KATA_POCH_TI, strParametr);
	strParametr.Format(_T("TD kąta pochylenia"));
	m_ctlStrojonyParametr1.InsertString(STRP_KATA_POCH_TD, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_KATA_POCH_TD, strParametr);
	strParametr.Format(_T("Podst.filtra D kąta poch"));
	m_ctlStrojonyParametr1.InsertString(STRP_KATA_POCH_FD, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_KATA_POCH_FD, strParametr);
	strParametr.Format(_T("Podst.filtra W.Zad kąta poch"));
	m_ctlStrojonyParametr1.InsertString(STRP_KATA_POCH_FWZ, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_KATA_POCH_FWZ, strParametr);
	strParametr.Format(_T("%% Wyprzedzenia kąta poch"));
	m_ctlStrojonyParametr1.InsertString(STRP_KATA_POCH_WYPRZ, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_KATA_POCH_WYPRZ, strParametr);

	strParametr.Format(_T("KP prędk.kąt. pochylenia"));
	m_ctlStrojonyParametr1.InsertString(STRP_PRED_POCH_KP, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_PRED_POCH_KP, strParametr);
	strParametr.Format(_T("TI prędk.kąt. pochylenia"));
	m_ctlStrojonyParametr1.InsertString(STRP_PRED_POCH_TI, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_PRED_POCH_TI, strParametr);
	strParametr.Format(_T("TD prędk.kąt. pochylenia"));
	m_ctlStrojonyParametr1.InsertString(STRP_PRED_POCH_TD, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_PRED_POCH_TD, strParametr);
	strParametr.Format(_T("Podst.filtra D prędk.kąt.poch"));
	m_ctlStrojonyParametr1.InsertString(STRP_PRED_POCH_FD, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_PRED_POCH_FD, strParametr);
	strParametr.Format(_T("Podst.filtra W.Zad prędk.kąt.poch"));
	m_ctlStrojonyParametr1.InsertString(STRP_PRED_POCH_FWZ, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_PRED_POCH_FWZ, strParametr);
	strParametr.Format(_T("%% Wyprzedzenia prędk.kąt.poch"));
	m_ctlStrojonyParametr1.InsertString(STRP_PRED_POCH_WYPRZ, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_PRED_POCH_WYPRZ, strParametr);

	strParametr.Format(_T("KP kąta odchylenia"));
	m_ctlStrojonyParametr1.InsertString(STRP_KATA_ODCH_KP, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_KATA_ODCH_KP, strParametr);
	strParametr.Format(_T("TI kąta odchylenia"));
	m_ctlStrojonyParametr1.InsertString(STRP_KATA_ODCH_TI, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_KATA_ODCH_TI, strParametr);
	strParametr.Format(_T("TD kąta odchylenia"));
	m_ctlStrojonyParametr1.InsertString(STRP_KATA_ODCH_TD, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_KATA_ODCH_TD, strParametr);
	strParametr.Format(_T("Podst.filtra D kąta odch"));
	m_ctlStrojonyParametr1.InsertString(STRP_KATA_ODCH_FD, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_KATA_ODCH_FD, strParametr);
	strParametr.Format(_T("Podst.filtra W.Zad kąta odch"));
	m_ctlStrojonyParametr1.InsertString(STRP_KATA_ODCH_FWZ, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_KATA_ODCH_FWZ, strParametr);
	strParametr.Format(_T("%% Wyprzedzenia kąta odch"));
	m_ctlStrojonyParametr1.InsertString(STRP_KATA_ODCH_WYPRZ, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_KATA_ODCH_WYPRZ, strParametr);

	strParametr.Format(_T("KP prędk.kąt. odchylenia"));
	m_ctlStrojonyParametr1.InsertString(STRP_PRED_ODCH_KP, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_PRED_ODCH_KP, strParametr);
	strParametr.Format(_T("TI prędk.kąt. odchylenia"));
	m_ctlStrojonyParametr1.InsertString(STRP_PRED_ODCH_TI, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_PRED_ODCH_TI, strParametr);
	strParametr.Format(_T("TD prędk.kąt. odchylenia"));
	m_ctlStrojonyParametr1.InsertString(STRP_PRED_ODCH_TD, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_PRED_ODCH_TD, strParametr);
	strParametr.Format(_T("Podst.filtra D prędk.kąt.odch"));
	m_ctlStrojonyParametr1.InsertString(STRP_PRED_ODCH_FD, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_PRED_ODCH_FD, strParametr);
	strParametr.Format(_T("Podst.filtra W.Zad prędk.kąt.odch"));
	m_ctlStrojonyParametr1.InsertString(STRP_PRED_ODCH_FWZ, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_PRED_ODCH_FWZ, strParametr);
	strParametr.Format(_T("%% Wyprzedzenia prędk.kąt.odch"));
	m_ctlStrojonyParametr1.InsertString(STRP_PRED_ODCH_WYPRZ, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_PRED_ODCH_WYPRZ, strParametr);

	strParametr.Format(_T("KP wysokości"));
	m_ctlStrojonyParametr1.InsertString(STRP_WYSOKOSCI_KP, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_WYSOKOSCI_KP, strParametr);
	strParametr.Format(_T("TI wysokości"));
	m_ctlStrojonyParametr1.InsertString(STRP_WYSOKOSCI_TI, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_WYSOKOSCI_TI, strParametr);
	strParametr.Format(_T("TD wysokości"));
	m_ctlStrojonyParametr1.InsertString(STRP_WYSOKOSCI_TD, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_WYSOKOSCI_TD, strParametr);
	strParametr.Format(_T("Podst.filtra D wysokości"));
	m_ctlStrojonyParametr1.InsertString(STRP_WYSOKOSCI_FD, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_WYSOKOSCI_FD, strParametr);
	strParametr.Format(_T("Podst.filtra W.Zad wysokości"));
	m_ctlStrojonyParametr1.InsertString(STRP_WYSOKOSCI_FWZ, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_WYSOKOSCI_FWZ, strParametr);
	strParametr.Format(_T("%% Wyprzedzenia wysokości"));
	m_ctlStrojonyParametr1.InsertString(STRP_WYSOKOSCI_WYPRZ, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_WYSOKOSCI_WYPRZ, strParametr);

	strParametr.Format(_T("KP prędk.zmiany wysokości"));
	m_ctlStrojonyParametr1.InsertString(STRP_PRED_ZWYS_KP, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_PRED_ZWYS_KP, strParametr);
	strParametr.Format(_T("TI prędk.zmiany wysokości"));
	m_ctlStrojonyParametr1.InsertString(STRP_PRED_ZWYS_TI, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_PRED_ZWYS_TI, strParametr);
	strParametr.Format(_T("TD prędk.zmiany wysokości"));
	m_ctlStrojonyParametr1.InsertString(STRP_PRED_ZWYS_TD, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_PRED_ZWYS_TD, strParametr);
	strParametr.Format(_T("Podst.filtra D prędk.zm.wys."));
	m_ctlStrojonyParametr1.InsertString(STRP_PRED_ZWYS_FD, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_PRED_ZWYS_FD, strParametr);
	strParametr.Format(_T("Podst.filtra W.Zad prędk.zm.wys."));
	m_ctlStrojonyParametr1.InsertString(STRP_PRED_ZWYS_FWZ, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_PRED_ZWYS_FWZ, strParametr);
	strParametr.Format(_T("%% Wyprzedzenia prędk.zm.wys"));
	m_ctlStrojonyParametr1.InsertString(STRP_PRED_ZWYS_WYPRZ, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_PRED_ZWYS_WYPRZ, strParametr);

	strParametr.Format(_T("KP nawig.drogi północnej"));
	m_ctlStrojonyParametr1.InsertString(STRP_NAWI_POLN_KP, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_NAWI_POLN_KP, strParametr);
	strParametr.Format(_T("TI nawig.drogi północnej"));
	m_ctlStrojonyParametr1.InsertString(STRP_NAWI_POLN_TI, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_NAWI_POLN_TI, strParametr);
	strParametr.Format(_T("TD nawig.drogi północnej"));
	m_ctlStrojonyParametr1.InsertString(STRP_NAWI_POLN_TD, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_NAWI_POLN_TD, strParametr);
	strParametr.Format(_T("KP nawig.prędk.północnej"));
	m_ctlStrojonyParametr1.InsertString(STRP_PRED_POLN_KP, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_PRED_POLN_KP, strParametr);
	strParametr.Format(_T("TI nawig.prędk.północnej"));
	m_ctlStrojonyParametr1.InsertString(STRP_PRED_POLN_TI, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_PRED_POLN_TI, strParametr);
	strParametr.Format(_T("TD nawig.prędk.północnej"));
	m_ctlStrojonyParametr1.InsertString(STRP_PRED_POLN_TD, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_PRED_POLN_TD, strParametr);

	strParametr.Format(_T("KP nawig.drogi wschodniej"));
	m_ctlStrojonyParametr1.InsertString(STRP_NAWI_WSCH_KP, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_NAWI_WSCH_KP, strParametr);
	strParametr.Format(_T("TI nawig.drogi wschodniej"));
	m_ctlStrojonyParametr1.InsertString(STRP_NAWI_WSCH_TI, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_NAWI_WSCH_TI, strParametr);
	strParametr.Format(_T("TD nawig.drogi wschodniej"));
	m_ctlStrojonyParametr1.InsertString(STRP_NAWI_WSCH_TD, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_NAWI_WSCH_TD, strParametr);
	strParametr.Format(_T("KP nawig.prędk.wschodniej"));
	m_ctlStrojonyParametr1.InsertString(STRP_PRED_WSCH_KP, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_PRED_WSCH_KP, strParametr);
	strParametr.Format(_T("TI nawig.prędk.wschodniej"));
	m_ctlStrojonyParametr1.InsertString(STRP_PRED_WSCH_TI, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_PRED_WSCH_TI, strParametr);
	strParametr.Format(_T("TD nawig.prędk.wschodniej"));
	m_ctlStrojonyParametr1.InsertString(STRP_PRED_WSCH_TD, strParametr);
	m_ctlStrojonyParametr2.InsertString(STRP_PRED_WSCH_TD, strParametr);

	chErr = getKomunikacja().CzytajU8FRAM(chDane, LICZBA_KAN_RC_DO_STROJENIA_PID, FAU_STROJ1_PARAMETR);
	m_ctlStrojonyParametr1.SetCurSel(chDane[0]);
	m_ctlStrojonyParametr2.SetCurSel(chDane[1]);

	chErr = getKomunikacja().CzytajFloatFRAM(m_fWartośćParametru, 2 * LICZBA_KAN_RC_DO_STROJENIA_PID, FAU_STROJ1_WART_MIN);
	m_strWartoscMinParametru1.Format(_T("%.3f"), m_fWartośćParametru[0]);
	m_strWartoscMinParametru1.Replace(_T('.'), _T(','));
	m_strWartoscMinParametru2.Format(_T("%.3f"), m_fWartośćParametru[1]);
	m_strWartoscMinParametru2.Replace(_T('.'), _T(','));
	m_strWartoscMaxParametru1.Format(_T("%.3f"), m_fWartośćParametru[2]);
	m_strWartoscMaxParametru1.Replace(_T('.'), _T(','));
	m_strWartoscMaxParametru2.Format(_T("%.3f"), m_fWartośćParametru[3]);
	m_strWartoscMaxParametru2.Replace(_T('.'), _T(','));
	UpdateData(FALSE);

	//czytaj funkcje kanałów aby wyświetlić informacje o tym które kanały sterują parametrami PID
	chErr = getKomunikacja().CzytajU8FRAM(chDane, KANALY_FUNKCYJNE, FAU_FUNKCJA_KAN_RC);
	if (chErr == ERR_OK)
	{
		for (uint8_t n = 0; n < KANALY_FUNKCYJNE; n++)
		{
			if (chDane[n] == FRC_STROJ_PID_PARAM1)	//kanał służy do zmiany wybranego parametru 1 regulatorów PID
				chKanałParametru1 = n + 1 + LICZBA_DRAZKOW;
			if (chDane[n] == FRC_STROJ_PID_PARAM2)	//kanał służy do zmiany wybranego parametru 2 regulatorów PID
				chKanałParametru2 = n + 1 + LICZBA_DRAZKOW;
		}
	}
	m_strOpisParametru1.Format(_T("Parametr 1 strojony kanałem %d"), chKanałParametru1);
	m_strOpisParametru2.Format(_T("Parametr 2 strojony kanałem %d"), chKanałParametru2);
	UpdateData(FALSE);

	UstawKontrolki(m_nBiezacyParametr);
	WlaczKontrolki(m_chTrybRegulacji[m_nBiezacyParametr], m_nBiezacyParametr);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // WYJĄTEK: Strona właściwości OCX powinna zwrócić FALSE
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Zmienia stan włączenia kontrolek okna dialogowego
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::WlaczKontrolki(uint8_t cTrybPracyRegulatora, uint8_t cKanal)
{
	//regulatory stab
	GetDlgItem(IDC_EDIT_KP1)->EnableWindow(cTrybPracyRegulatora >= REG_STAB);
	GetDlgItem(IDC_EDIT_TI1)->EnableWindow(cTrybPracyRegulatora >= REG_STAB);
	GetDlgItem(IDC_EDIT_TD1)->EnableWindow(cTrybPracyRegulatora >= REG_STAB);
	GetDlgItem(IDC_EDIT_LIMIT_CALKI1)->EnableWindow(cTrybPracyRegulatora >= REG_STAB);
	GetDlgItem(IDC_EDIT_MIN_WY1)->EnableWindow(cTrybPracyRegulatora >= REG_STAB);
	GetDlgItem(IDC_EDIT_MAX_WY1)->EnableWindow(cTrybPracyRegulatora >= REG_STAB);
	GetDlgItem(IDC_EDIT_MNOZNIK_WART_ZAD_STAB)->EnableWindow((cTrybPracyRegulatora >= REG_STAB) & (cTrybPracyRegulatora < REG_AUTO) & (cKanal < 4));
	GetDlgItem(IDC_EDIT_PRZESUNIECIE_WART_ZADANEJ1)->EnableWindow((cTrybPracyRegulatora >= REG_STAB) & (cKanal < 4));
	GetDlgItem(IDC_CHECK_KATOWY1)->EnableWindow(cTrybPracyRegulatora >= REG_STAB);
	GetDlgItem(IDC_SLIDER_FILTR_D1)->EnableWindow(cTrybPracyRegulatora >= REG_STAB);
	GetDlgItem(IDC_SLIDER_FILTR_WZAD1)->EnableWindow((cTrybPracyRegulatora >= REG_STAB) & (cTrybPracyRegulatora < REG_AUTO) & (cKanal < 4));
	GetDlgItem(IDC_SLIDER_PROC_WYPRZEDZENIA1)->EnableWindow((cTrybPracyRegulatora >= REG_STAB) & (cTrybPracyRegulatora < REG_AUTO) & (cKanal < 4));
	GetDlgItem(IDC_STATIC_FILTR_D1)->EnableWindow(cTrybPracyRegulatora >= REG_STAB);
	GetDlgItem(IDC_STATIC_FILTR_WZAD1)->EnableWindow((cTrybPracyRegulatora >= REG_STAB) & (cTrybPracyRegulatora < REG_AUTO) & (cKanal < 4));
	GetDlgItem(IDC_STATIC_PROC_WYPRZEDZENIA1)->EnableWindow((cTrybPracyRegulatora >= REG_STAB) & (cTrybPracyRegulatora < REG_AUTO) & (cKanal < 4));
	
	//regulatory akro
	GetDlgItem(IDC_EDIT_KP2)->EnableWindow(cTrybPracyRegulatora >= REG_AKRO);
	GetDlgItem(IDC_EDIT_TI2)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_TD2)->EnableWindow(cTrybPracyRegulatora >= REG_AKRO);
	GetDlgItem(IDC_EDIT_LIMIT_CALKI2)->EnableWindow(cTrybPracyRegulatora >= REG_AKRO);
	GetDlgItem(IDC_EDIT_MIN_WY2)->EnableWindow(cTrybPracyRegulatora >= REG_RECZNA);
	GetDlgItem(IDC_EDIT_MAX_WY2)->EnableWindow(cTrybPracyRegulatora >= REG_RECZNA);
	GetDlgItem(IDC_EDIT_MNOZNIK_WART_ZAD_AKRO)->EnableWindow((cTrybPracyRegulatora >= REG_AKRO) & (cTrybPracyRegulatora < REG_AUTO) & (cKanal < 4));
	GetDlgItem(IDC_EDIT_PRZESUNIECIE_WART_ZADANEJ2)->EnableWindow((cTrybPracyRegulatora >= REG_STAB) & (cKanal < 4));
	GetDlgItem(IDC_SLIDER_FILTR_D2)->EnableWindow(cTrybPracyRegulatora >= REG_AKRO);	
	GetDlgItem(IDC_SLIDER_FILTR_WZAD2)->EnableWindow((cTrybPracyRegulatora >= REG_AKRO) & (cTrybPracyRegulatora < REG_AUTO) & (cKanal < 4));
	GetDlgItem(IDC_SLIDER_PROC_WYPRZEDZENIA2)->EnableWindow((cTrybPracyRegulatora >= REG_AKRO) & (cTrybPracyRegulatora < REG_AUTO) & (cKanal < 4));
	GetDlgItem(IDC_STATIC_FILTR_D2)->EnableWindow(cTrybPracyRegulatora >= REG_AKRO);
	GetDlgItem(IDC_STATIC_FILTR_WZAD2)->EnableWindow((cTrybPracyRegulatora >= REG_AKRO) & (cTrybPracyRegulatora < REG_AUTO) & (cKanal < 4));
	GetDlgItem(IDC_STATIC_PROC_WYPRZEDZENIA2)->EnableWindow((cTrybPracyRegulatora >= REG_AKRO) & (cTrybPracyRegulatora < REG_AUTO) & (cKanal < 4));
}




///////////////////////////////////////////////////////////////////////////////////////////////////
// Wstawia zawartość do pól edycyjnych
// Nie jest zrozumiałe dlaczego potrzeba przy każdej kontrolce dawa updateDate(flase), ale bez tego kontrolki nie są uaktualniane - sprawa do wyjaśnienia
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::UstawKontrolki(int nParametr)
{
	int nRegGlow = 2 * nParametr + 0;
	int nRegPoch = 2 * nParametr + 1;

	WlaczKontrolki(m_chTrybRegulacji[nParametr], nParametr);

	m_strKP1.Format(_T("%.3f"), m_stPID[nRegGlow].fKp);
	m_strKP1.Replace(_T('.'), _T(','));
	m_strKP2.Format(_T("%.3f"), m_stPID[nRegPoch].fKp);
	m_strKP2.Replace(_T('.'), _T(','));
	UpdateData(FALSE);
	m_strTI1.Format(_T("%.4f"), m_stPID[nRegGlow].fTi);
	m_strTI1.Replace(_T('.'), _T(','));
	m_strTI2.Format(_T("%.4f"), m_stPID[nRegPoch].fTi);
	m_strTI2.Replace(_T('.'), _T(','));
	UpdateData(FALSE);
	m_strTD1.Format(_T("%.4f"), m_stPID[nRegGlow].fTd);
	m_strTD1.Replace(_T('.'), _T(','));
	m_strTD2.Format(_T("%.4f"), m_stPID[nRegPoch].fTd);
	m_strTD2.Replace(_T('.'), _T(','));
	UpdateData(FALSE);
	m_strOgrCalki1.Format(_T("%.1f"), m_stPID[nRegGlow].fOgrCalki);
	m_strOgrCalki1.Replace(_T('.'), _T(','));
	m_strOgrCalki2.Format(_T("%.1f"), m_stPID[nRegPoch].fOgrCalki);
	m_strOgrCalki2.Replace(_T('.'), _T(','));
	UpdateData(FALSE);
	m_strMinWyj1.Format(_T("%.1f"), m_stPID[nRegGlow].fMinWyj);
	m_strMinWyj1.Replace(_T('.'), _T(','));
	m_strMinWyj2.Format(_T("%.1f"), m_stPID[nRegPoch].fMinWyj);
	m_strMinWyj2.Replace(_T('.'), _T(','));
	UpdateData(FALSE);
	m_strMaxWyj1.Format(_T("%.1f"), m_stPID[nRegGlow].fMaxWyj);
	m_strMaxWyj1.Replace(_T('.'), _T(','));
	m_strMaxWyj2.Format(_T("%.1f"), m_stPID[nRegPoch].fMaxWyj);
	m_strMaxWyj2.Replace(_T('.'), _T(','));
	UpdateData(FALSE);
	m_strSkalaWartZadanejStab.Format(_T("%.6f"), m_stPID[nRegGlow].fMnożnikWartZadanej);
	m_strSkalaWartZadanejStab.Replace(_T('.'), _T(','));
	m_strSkalaWartZadanejAkro.Format(_T("%.6f"), m_stPID[nRegPoch].fMnożnikWartZadanej);
	m_strSkalaWartZadanejAkro.Replace(_T('.'), _T(','));
	UpdateData(FALSE);
	m_strPrzesunięcieWartościZadanej1.Format(_T("%.4f"), m_stPID[nRegGlow].fPrzesunięcieWartościZadanej);
	m_strPrzesunięcieWartościZadanej1.Replace(_T('.'), _T(','));
	m_strPrzesunięcieWartościZadanej2.Format(_T("%.4f"), m_stPID[nRegPoch].fPrzesunięcieWartościZadanej);
	m_strPrzesunięcieWartościZadanej2.Replace(_T('.'), _T(','));
	UpdateData(FALSE);
	m_ctlSlidPodstCzasuFiltraD1.SetPos(m_stPID[nRegGlow].cPodstFiltraD);
	m_ctlSlidPodstCzasuFiltraD2.SetPos(m_stPID[nRegPoch].cPodstFiltraD);
	m_ctlSlidProcWyprzedzenia1.SetPos(m_stPID[nRegGlow].cProcWartZadWyprz);
	m_ctlSlidProcWyprzedzenia2.SetPos(m_stPID[nRegPoch].cProcWartZadWyprz);
	m_ctlSlidPodstFiltraWartZad1.SetPos(m_stPID[nRegGlow].cPodstawaFiltraWartZad);
	m_ctlSlidPodstFiltraWartZad2.SetPos(m_stPID[nRegPoch].cPodstawaFiltraWartZad);
	m_bKatowy = m_stPID[nRegGlow].bKatowy;
	UpdateData(FALSE);
	UstawJednostke(nParametr);
	UstawTrybRegulacji(nParametr);
}



void KonfigPID::UstawTrybRegulacji(int nParametr)
{
	assert(nParametr < LICZBA_REG_PARAM);

	m_bTrybRegulacjiWylaczony = FALSE;
	m_bTrybRegulacjiReczny = FALSE;
	m_bTrybRegulacjiAkro = FALSE;
	m_bTrybRegulacjiStab = FALSE;
	m_bTrybRegulacjiAuto = FALSE;

	switch (m_chTrybRegulacji[nParametr])
	{
	case REG_WYLACZ: m_bTrybRegulacjiWylaczony = TRUE;	break;
	case REG_RECZNA: m_bTrybRegulacjiReczny = TRUE;	break;
	case REG_AKRO: m_bTrybRegulacjiAkro = TRUE;	break;
	case REG_STAB: m_bTrybRegulacjiStab = TRUE;	break;
	case REG_AUTO: m_bTrybRegulacjiAuto = TRUE;	break;
	}
	
	UpdateData(FALSE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Zamknięcie okna przyciskiem OK
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnBnClickedOk()
{
	uint8_t chErr = ERR_OK;
	uint8_t chParametr[LICZBA_KAN_RC_DO_STROJENIA_PID];
	CString strKomunikat;
	BOOL m_bZmienionoKonfigurację = FALSE;

	for (int n = 0; n < LICZBA_PID; n++)
	{
		if (m_stPID[n].bZmieniony)	//zapisz tylko te regulatory, które były zmienione
		{		
			m_unia8_32.dane8[0] = m_stPID[n].bKatowy * PID_KATOWY;	//1U flagi
			m_unia8_32.dane8[1] = m_stPID[n].cPodstFiltraD;			//1U Podstawa filtra IIR błędu do liczenia członu różniczkującego
			m_unia8_32.dane8[2] = m_stPID[n].cPodstawaFiltraWartZad;	//1U Podstawa filtra IIR wartości zadanej do liczenia członu wyprzedzajacego 
			m_unia8_32.dane8[3] = m_stPID[n].cProcWartZadWyprz;		//1U procentowa wartość zmiany wartości zadanej podawana na wejście wyprzedzenia
			chErr |= getKomunikacja().ZapiszKonfiguracjePID(n, m_stPID[n].fKp, m_stPID[n].fTi, m_stPID[n].fTd, m_stPID[n].fOgrCalki, m_stPID[n].fMinWyj, m_stPID[n].fMaxWyj, m_stPID[n].fMnożnikWartZadanej, m_stPID[n].fPrzesunięcieWartościZadanej, m_unia8_32.daneFloat);
			m_bZmienionoKonfigurację = FALSE;	//to polecenie nie wymaga przeładowania całosci, bo oprócz zapisu do FRAM ładuje też do zmiennych
		}
	}
	if (chErr != ERR_OK)
	{
		strKomunikat.Format(_T("Błąd nr %d zapisu konfiguracji PID"), chErr);
		MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojoj!"), MB_ICONWARNING, 0);
		CDialogEx::OnOK();
	}

	if (m_bZmienionyTrybRegulacji)
	{
		chErr |= getKomunikacja().ZapiszTrybRegulacji(m_chTrybRegulacji);
		if (chErr != ERR_OK)
		{
			strKomunikat.Format(_T("Błąd nr %d zapisu trybów regulacji"), chErr);
			MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojoj!"), MB_ICONWARNING, 0);
			CDialogEx::OnOK();
		}
		m_bZmienionoKonfigurację = TRUE;
	}

	if (m_bZmienioneParametryStrojenia)
	{
		chParametr[0] = (uint8_t)m_ctlStrojonyParametr1.GetCurSel();
		chParametr[1] = (uint8_t)m_ctlStrojonyParametr2.GetCurSel();
		chErr |= getKomunikacja().ZapiszDaneU8FRAM(chParametr, LICZBA_KAN_RC_DO_STROJENIA_PID, FAU_STROJ1_PARAMETR);
		m_bZmienionoKonfigurację = TRUE;
	}

	if (m_bZmienioneWartościStrojenia)
	{
		chErr |= getKomunikacja().ZapiszDaneFloatFRAM(m_fWartośćParametru, 2 * LICZBA_KAN_RC_DO_STROJENIA_PID, FAU_STROJ1_WART_MIN);
		m_bZmienionoKonfigurację = TRUE;
	}
	if (chErr != ERR_OK)
	{
		strKomunikat.Format(_T("Błąd nr %d zapisu parametrów strojenia PID"), chErr);
		MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojoj!"), MB_ICONWARNING, 0);
		CDialogEx::OnOK();
	}


	if (m_bZmienionoKonfigurację)
		chErr |= getKomunikacja().PrzeładujKonfiguracjePID();
	
	if (chErr != ERR_OK)
	{
		strKomunikat.Format(_T("Błąd nr %d przeładowania konfiguracji PID"), chErr);
		MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojoj!"), MB_ICONWARNING, 0);
	}

	CDialogEx::OnOK();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// obsługa nacisniecia przycisku "Ustaw domyślne"
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnBnClickedButUstawDomyslne()
{
	//nastawy wspólne dla wszystkich regulatorów
	for (int n = 0; n < LICZBA_PID; n++)
	{
		m_stPID[n].fKp = 1.0f;
		m_stPID[n].fTi = 0.1f;
		if (n & 0x01)	//regulator akro
		{
			m_stPID[n].fTi = 0.0f;
			m_stPID[n].fTd = 0.01f;				
			m_stPID[n].cProcWartZadWyprz = 10;
		}
		else            //regulator stab
		{
			m_stPID[n].fTi = 0.1f;
			m_stPID[n].fTd = 0.001f;	
			m_stPID[n].cProcWartZadWyprz = 0;
		}
		m_stPID[n].fOgrCalki = 20.0f;
		m_stPID[n].fMinWyj = -100.0f;
		m_stPID[n].fMaxWyj = 100.0f;
		m_stPID[n].fPrzesunięcieWartościZadanej = 0.0f;
		m_stPID[n].fWolne1 = 0.1f;
		m_stPID[n].cPodstFiltraD = 8;
		m_stPID[n].bZmieniony = TRUE;
	}

	//regulator sterowania przechyleniem (lotkami w samolocie)
	m_stPID[PID_KATA_PRZE].fMnożnikWartZadanej = (float)(30.0f * DEG2RAD);
	m_stPID[PID_KATA_PRZE].bKatowy = TRUE;

	//regulator sterowania prędkością kątową przechylenia (żyroskop P)
	m_stPID[PID_PRED_PRZE].fMnożnikWartZadanej = (float)(60.0f * DEG2RAD);
	m_stPID[PID_PRED_PRZE].bKatowy = FALSE;

	//regulator sterowania pochyleniem (sterem wysokości)
	m_stPID[PID_KATA_POCH].fMnożnikWartZadanej = (float)(30.0f * DEG2RAD);
	m_stPID[PID_KATA_POCH].bKatowy = TRUE;

	//regulator sterowania prędkością kątową pochylenia (żyroskop Q)
	m_stPID[PID_PRED_POCH].fMnożnikWartZadanej = (float)(60.0f * DEG2RAD);
	m_stPID[PID_PRED_POCH].bKatowy = FALSE;

	//regulator sterowania odchyleniem (sterem kierunku)
	m_stPID[PID_KATA_ODCH].fMnożnikWartZadanej = (float)(180.0f * DEG2RAD);
	m_stPID[PID_KATA_ODCH].bKatowy = TRUE;

	//regulator sterowania prędkością kątową odchylenia (żyroskop R)
	m_stPID[PID_PRED_ODCH].fMnożnikWartZadanej = (float)(20.0f * DEG2RAD);	//[rad/s]
	m_stPID[PID_PRED_ODCH].bKatowy = FALSE;

	//regulator sterowania wysokością
	m_stPID[PID_WYSOKOSCI].fMnożnikWartZadanej = 20.0f;	//[m]
	m_stPID[PID_WYSOKOSCI].bKatowy = FALSE;

	//regulator sterowani prędkością wznoszenia (wario)
	m_stPID[PID_WARIO].fMnożnikWartZadanej = 5.0f;	//[m/s]
	m_stPID[PID_WARIO].bKatowy = FALSE;

	//regulator sterowania nawigacją w kierunku północnym
	m_stPID[PID_NAWIG_N].fMnożnikWartZadanej = (float)(0.1f * DEG2RAD);
	m_stPID[PID_NAWIG_N].bKatowy = TRUE;

	//regulator sterowania prędkością w kierunku północnym
	m_stPID[PID_PREDK_N].fMnożnikWartZadanej = (float)(0.01f * DEG2RAD);
	m_stPID[PID_PREDK_N].bKatowy = FALSE;

	//regulator sterowania nawigacją w kierunku wschodnim
	m_stPID[PID_NAWIG_E].fMnożnikWartZadanej = (float)(0.1f * DEG2RAD);
	m_stPID[PID_NAWIG_E].bKatowy = TRUE;

	//regulator sterowania prędkością w kierunku wschodnim
	m_stPID[PID_PREDK_E].fMnożnikWartZadanej = (float)(0.01f * DEG2RAD);
	m_stPID[PID_PREDK_E].bKatowy = FALSE;

	//tryby regulacji
	m_chTrybRegulacji[PRZE] = REG_STAB;		//regulator sterowania przechyleniem (lotkami w samolocie)    
	m_chTrybRegulacji[POCH] = REG_STAB;		//regulator sterowania pochyleniem (sterem wysokości)
	m_chTrybRegulacji[ODCH] = REG_STAB;     //regulator sterowania obrotem (sterem kierunku)
	m_chTrybRegulacji[WYSO] = REG_AKRO;		//regulator sterowania wysokością
	m_chTrybRegulacji[POZN] = REG_AUTO;		//regulator sterowania prędkością i położeniem północnym
	m_chTrybRegulacji[POZE] = REG_AUTO;		//regulator sterowania prędkością i położeniem wschodnim
	m_bZmienionyTrybRegulacji = TRUE;

	UstawKontrolki(m_nBiezacyParametr);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Ustawia nazwę jednostki wartości zadanej
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::UstawJednostke(int nParametr)
{
	switch (nParametr)
	{
	case PRZE:
	case POCH:
	case ODCH:
	case POZN:
	case POZE:
		m_strJednostkaStab = m_strJednostkaStab2 = _T("[rad]");
		m_strJednostkaAkro = _T("[rad/s]");
		break;

	case WYSO:
		m_strJednostkaStab = m_strJednostkaStab2 = _T("[m]");
		m_strJednostkaAkro = _T("[m/s]");
		break;

	default:
		m_strJednostkaStab = m_strJednostkaStab2 = _T("błąd");
		m_strJednostkaAkro = _T("błąd");
		break;
	}
	UpdateData(FALSE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Kliknięto na zakłądke zmany kanału regulatorów
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnTcnSelchangeTabKanalPid(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
	UpdateData(TRUE);
	m_nBiezacyParametr = m_ctrlKanalPID.GetCurSel();
	UstawKontrolki(m_nBiezacyParametr);
	//WylaczStab(m_stPID[2 * m_nBiezacyParametr + 0].bWylaczony);
	//WylaczAkro(m_stPID[2 * m_nBiezacyParametr + 1].bWylaczony);
	WlaczKontrolki(m_chTrybRegulacji[m_nBiezacyParametr], m_nBiezacyParametr);

	//WylaczTrybRegulacji(m_nBiezacyParametr >= POZN);	//wybór trybu jest niemożliwy dla regulatora pozycji - zawsze jest AUTO
	UpdateData(FALSE);
	*pResult = 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Zamienia string parametru na liczbę float
// Zwraca: liczbę float
///////////////////////////////////////////////////////////////////////////////////////////////////
float KonfigPID::ZamienStrNaFloat(CString strLiczba)
{
	strLiczba.Replace(_T(','), _T('.'));
	return (float)_wtof(strLiczba);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Edycja kontrolki wzmocnienia regulatora pochodnej
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnEnChangeEditKp1()
{
	UpdateData(TRUE);
	m_stPID[2 * m_nBiezacyParametr + 0].fKp = ZamienStrNaFloat(m_strKP1.GetString());
	m_stPID[2 * m_nBiezacyParametr + 0].bZmieniony = TRUE;
	UpdateData(FALSE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Edycja kontrolki wzmocnienia regulatora kąta
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnEnChangeEditKp2()
{
	UpdateData(TRUE);
	m_stPID[2 * m_nBiezacyParametr + 1].fKp = ZamienStrNaFloat(m_strKP2.GetString());
	m_stPID[2 * m_nBiezacyParametr + 1].bZmieniony = TRUE;
	UpdateData(FALSE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Edycja kontrolki Ti regulatora pochodnej
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnEnChangeEditTi1()
{
	UpdateData(TRUE);
	m_stPID[2 * m_nBiezacyParametr + 0].fTi = ZamienStrNaFloat(m_strTI1.GetString());
	m_stPID[2 * m_nBiezacyParametr + 0].bZmieniony = TRUE;
	UpdateData(FALSE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Edycja kontrolki Ti regulatora kąta
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnEnChangeEditTi2()
{
	UpdateData(TRUE);
	m_stPID[2 * m_nBiezacyParametr + 1].fTi = ZamienStrNaFloat(m_strTI2.GetString());
	m_stPID[2 * m_nBiezacyParametr + 1].bZmieniony = TRUE;
	UpdateData(FALSE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Edycja kontrolki Td regulatora pochodnej
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnEnChangeEditTd1()
{
	UpdateData(TRUE);
	m_stPID[2 * m_nBiezacyParametr + 0].fTd = ZamienStrNaFloat(m_strTD1.GetString());
	m_stPID[2 * m_nBiezacyParametr + 0].bZmieniony = TRUE;
	UpdateData(FALSE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Edycja kontrolki Td regulatora kąta
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnEnChangeEditTd2()
{
	UpdateData(TRUE);
	m_stPID[2 * m_nBiezacyParametr + 1].fTd = ZamienStrNaFloat(m_strTD2.GetString());
	m_stPID[2 * m_nBiezacyParametr + 1].bZmieniony = TRUE;
	UpdateData(FALSE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Edycja kontrolki minimum wyjścia regulatora pochodnej
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnEnChangeEditMinWy1()
{
	UpdateData(TRUE);
	m_stPID[2 * m_nBiezacyParametr + 0].fMinWyj = ZamienStrNaFloat(m_strMinWyj1.GetString());
	m_stPID[2 * m_nBiezacyParametr + 0].bZmieniony = TRUE;
	UpdateData(FALSE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Edycja kontrolki minimum wyjścia regulatora kąta
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnEnChangeEditMinWy2()
{
	UpdateData(TRUE);
	m_stPID[2 * m_nBiezacyParametr + 1].fMinWyj = ZamienStrNaFloat(m_strMinWyj2.GetString());
	m_stPID[2 * m_nBiezacyParametr + 1].bZmieniony = TRUE;
	UpdateData(FALSE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Edycja kontrolki maksimum wyjścia regulatora pochodnej
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnEnChangeEditMaxWy1()
{
	UpdateData(TRUE);
	m_stPID[2 * m_nBiezacyParametr + 0].fMaxWyj = ZamienStrNaFloat(m_strMaxWyj1.GetString());
	m_stPID[2 * m_nBiezacyParametr + 0].bZmieniony = TRUE;
	UpdateData(FALSE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Edycja kontrolki maksimum wyjścia regulatora kąta
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnEnChangeEditMaxWy2()
{
	UpdateData(TRUE);
	m_stPID[2 * m_nBiezacyParametr + 1].fMaxWyj = ZamienStrNaFloat(m_strMaxWyj2.GetString());
	m_stPID[2 * m_nBiezacyParametr + 1].bZmieniony = TRUE;
	UpdateData(FALSE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Edycja kontrolki ograniczenia całki 1
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnEnChangeEditLimitCalki1()
{
	UpdateData(TRUE);
	m_stPID[2 * m_nBiezacyParametr + 0].fOgrCalki = ZamienStrNaFloat(m_strOgrCalki1.GetString());
	m_stPID[2 * m_nBiezacyParametr + 0].bZmieniony = TRUE;
	UpdateData(FALSE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Edycja kontrolki ograniczenia całki 2
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnEnChangeEditLimitCalki2()
{
	UpdateData(TRUE);
	m_stPID[2 * m_nBiezacyParametr + 1].fOgrCalki = ZamienStrNaFloat(m_strOgrCalki2.GetString());
	m_stPID[2 * m_nBiezacyParametr + 1].bZmieniony = TRUE;
	UpdateData(FALSE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Edycja kontrolki mnonika wartości zadanej regulatora podstawowego
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnEnChangeEditSkalaWartZadStab()
{
	UpdateData(TRUE);
	m_stPID[2 * m_nBiezacyParametr + 0].fMnożnikWartZadanej = ZamienStrNaFloat(m_strSkalaWartZadanejStab.GetString());
	m_stPID[2 * m_nBiezacyParametr + 0].bZmieniony = TRUE;
	UpdateData(FALSE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Edycja kontrolki przesunięcia wartości zadanej 1
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnEnChangeEditPrzesuniecieWartZadanej1()
{
	UpdateData(TRUE);
	m_stPID[2 * m_nBiezacyParametr + 0].fPrzesunięcieWartościZadanej = ZamienStrNaFloat(m_strPrzesunięcieWartościZadanej1.GetString());
	m_stPID[2 * m_nBiezacyParametr + 0].bZmieniony = TRUE;
	UpdateData(FALSE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Edycja kontrolki przesunięcia wyjścia 1
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnEnChangeEditPrzesuniecieWartZadanej2()
{
	UpdateData(TRUE);
	m_stPID[2 * m_nBiezacyParametr + 1].fPrzesunięcieWartościZadanej = ZamienStrNaFloat(m_strPrzesunięcieWartościZadanej2.GetString());
	m_stPID[2 * m_nBiezacyParametr + 1].bZmieniony = TRUE;
	UpdateData(FALSE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Edycja kontrolki skalowania wartości zadanej regulatora pochodnej
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnEnChangeEditSkalaWartZadAkro()
{
	UpdateData(TRUE);
	m_stPID[2 * m_nBiezacyParametr + 1].fMnożnikWartZadanej = ZamienStrNaFloat(m_strSkalaWartZadanejAkro.GetString());
	m_stPID[2 * m_nBiezacyParametr + 1].bZmieniony = TRUE;
	UpdateData(FALSE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na potrzebę przerysowania suwaka filtra D
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnNMCustomdrawSliderFiltrD1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	UpdateData(TRUE);
	m_nPodstFiltraD1 = m_ctlSlidPodstCzasuFiltraD1.GetPos();
	m_stPID[2 * m_nBiezacyParametr + 0].cPodstFiltraD = m_nPodstFiltraD1;	//ponieważ przesuwanie kursorami nie wywołuje metody OnNMReleasedcaptureSliderFiltrD1, więc aktualizuj również tutaj podczas odświeżania
	m_strPodstFiltraD1.Format(_T("Filtr D: %d "), m_nPodstFiltraD1);
	UpdateData(FALSE);
	*pResult = 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmianę położenia suwaka filtra D regulatora głównego
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnNMReleasedcaptureSliderFiltrD1(NMHDR* pNMHDR, LRESULT* pResult)
{
	UpdateData(TRUE);
	m_nPodstFiltraD1 = m_ctlSlidPodstCzasuFiltraD1.GetPos();
	m_stPID[2 * m_nBiezacyParametr + 0].cPodstFiltraD = m_nPodstFiltraD1;
	m_stPID[2 * m_nBiezacyParametr + 0].bZmieniony = TRUE;
	*pResult = 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na potrzebę przerysowania suwaka filtra D
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnNMCustomdrawSliderFiltrD2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	UpdateData(TRUE);
	m_nPodstFiltraD2 = m_ctlSlidPodstCzasuFiltraD2.GetPos();
	m_stPID[2 * m_nBiezacyParametr + 1].cPodstFiltraD = m_nPodstFiltraD2;		//ponieważ przesuwanie kursorami nie wywołuje metody OnNMReleasedcaptureSliderFiltrD2, więc aktualizuj również tutaj
	m_strPodstFiltraD2.Format(_T("Filtr D: %d "), m_nPodstFiltraD2);	
	UpdateData(FALSE);
	*pResult = 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmianę położenia suwaka filtra D regulatora pochodnej
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnNMReleasedcaptureSliderFiltrD2(NMHDR* pNMHDR, LRESULT* pResult)
{
	UpdateData(TRUE);
	m_nPodstFiltraD2 = m_ctlSlidPodstCzasuFiltraD2.GetPos();
	m_stPID[2 * m_nBiezacyParametr + 1].cPodstFiltraD = m_nPodstFiltraD2;
	m_stPID[2 * m_nBiezacyParametr + 1].bZmieniony = TRUE;
	*pResult = 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmianę stanu checkboxa właczajcego regulator kąta
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnBnClickedCheckKatowy()
{
	UpdateData(TRUE);
	m_stPID[2 * m_nBiezacyParametr + 0].bKatowy = m_bKatowy;
	m_stPID[2 * m_nBiezacyParametr + 0].bZmieniony = TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// obsługa nacisniecia radiobuttona "Wyłacz"
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnBnClickedRadioRegWylacz()
{
	m_chTrybRegulacji[m_nBiezacyParametr] = REG_WYLACZ;		//regultor wyłączony
	UstawTrybRegulacji(m_nBiezacyParametr);
	WlaczKontrolki(m_chTrybRegulacji[m_nBiezacyParametr], m_nBiezacyParametr);
	m_bZmienionyTrybRegulacji = TRUE;
	UpdateData(FALSE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// obsługa nacisniecia radiobuttona "Reczny"
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnBnClickedRadioRegReczny()
{
	m_chTrybRegulacji[m_nBiezacyParametr] = REG_RECZNA;		//regulacja ręczna, bezpośrednio z drążków aparatury	
	UstawTrybRegulacji(m_nBiezacyParametr);
	WlaczKontrolki(m_chTrybRegulacji[m_nBiezacyParametr], m_nBiezacyParametr);
	m_bZmienionyTrybRegulacji = TRUE;
	UpdateData(FALSE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// obsługa nacisniecia radiobuttona "Akro"
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnBnClickedRadioRegAkro()
{
	m_chTrybRegulacji[m_nBiezacyParametr] = REG_AKRO;	//regulacja akrobacyjna, steruje pochodną parametru głównego: prędkością kątową lub prędkości zmiany wysokości
	UstawTrybRegulacji(m_nBiezacyParametr);
	WlaczKontrolki(m_chTrybRegulacji[m_nBiezacyParametr], m_nBiezacyParametr);
	m_bZmienionyTrybRegulacji = TRUE;
	UpdateData(FALSE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// obsługa nacisniecia radiobuttona "Stab"
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnBnClickedRadioRegStab()
{
	m_chTrybRegulacji[m_nBiezacyParametr] = REG_STAB;	//regulacja stabilizująca, steruje parametrem głównym: kątem lub wysokością
	UstawTrybRegulacji(m_nBiezacyParametr);
	WlaczKontrolki(m_chTrybRegulacji[m_nBiezacyParametr], m_nBiezacyParametr);
	m_bZmienionyTrybRegulacji = TRUE;
	UpdateData(FALSE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// obsługa nacisniecia radiobuttona "Auto"
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnBnClickedRadioRegAuto()
{
	m_chTrybRegulacji[m_nBiezacyParametr] = REG_AUTO;	//regulacja automatyczna, steruje wartością nadrzędną czyli nawigacją po wspołrzędnych geograficznych
	UstawTrybRegulacji(m_nBiezacyParametr);
	WlaczKontrolki(m_chTrybRegulacji[m_nBiezacyParametr], m_nBiezacyParametr);
	m_bZmienionyTrybRegulacji = TRUE;
	UpdateData(FALSE);
}




///////////////////////////////////////////////////////////////////////////////////////////////////
// obsługa zmiany combo box z rodzajem strojonego parametru 1
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnCbnSelchangeComboStrojonyParametr1()
{
	m_bZmienioneParametryStrojenia = TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// obsługa zmiany combo box z rodzajem strojonego parametru 2
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnCbnSelchangeComboStrojonyParametr2()
{
	m_bZmienioneParametryStrojenia = TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// obsługa zmiany edit boxa z minimalną wartoscią parametru 1
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnEnChangeEditParametrMin1()
{
	UpdateData(TRUE);
	m_fWartośćParametru[0] = ZamienStrNaFloat(m_strWartoscMinParametru1.GetString());
	m_bZmienioneWartościStrojenia = TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// obsługa zmiany edit boxa z maksymalną wartoscią parametru 1
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnEnChangeEditParametrMax1()
{
	UpdateData(TRUE);
	m_fWartośćParametru[2] = ZamienStrNaFloat(m_strWartoscMaxParametru1.GetString());
	m_bZmienioneWartościStrojenia = TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// obsługa zmiany edit boxa z minimalną wartoscią parametru 2
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnEnChangeEditParametrMin2()
{
	UpdateData(TRUE);
	m_fWartośćParametru[1] = ZamienStrNaFloat(m_strWartoscMinParametru2.GetString());
	m_bZmienioneWartościStrojenia = TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// obsługa zmiany edit boxa z maksymalną wartoscią parametru 2
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnEnChangeEditParametrMax2()
{
	UpdateData(TRUE);
	m_fWartośćParametru[3] = ZamienStrNaFloat(m_strWartoscMaxParametru2.GetString());
	m_bZmienioneWartościStrojenia = TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmianę położenia suwaka podstawy filtra wartosci zadanej regulatora głównego
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnNMCustomdrawSliderFiltrWzad1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	m_nPodstFiltraWartZadanej1 = m_ctlSlidPodstFiltraWartZad1.GetPos();
	m_stPID[2 * m_nBiezacyParametr + 0].cPodstawaFiltraWartZad = m_nPodstFiltraWartZadanej1;	
	m_strPodstFiltraWZad1.Format(_T("Podstawa filtra wart.zad.: %d"), m_nPodstFiltraWartZadanej1);
	UpdateData(FALSE);
	*pResult = 0;
}

void KonfigPID::OnReleasedcaptureSliderFiltrWzad1(NMHDR* pNMHDR, LRESULT* pResult)
{
	UpdateData(TRUE);
	m_nPodstFiltraWartZadanej1 = m_ctlSlidPodstFiltraWartZad1.GetPos();
	m_stPID[2 * m_nBiezacyParametr + 0].bZmieniony = TRUE;
	*pResult = 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmianę położenia suwaka podstawy filtra wartosci zadanej regulatora pochodnej
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnNMCustomdrawSliderFiltrWzad2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	m_nPodstFiltraWartZadanej2 = m_ctlSlidPodstFiltraWartZad2.GetPos();
	m_stPID[2 * m_nBiezacyParametr + 1].cPodstawaFiltraWartZad = m_nPodstFiltraWartZadanej2;
	m_strPodstFiltraWZad2.Format(_T("Podstawa filtra wart.zad.: %d"), m_nPodstFiltraWartZadanej2);
	UpdateData(FALSE);
	*pResult = 0;
}

void KonfigPID::OnReleasedcaptureSliderFiltrWzad2(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_nPodstFiltraWartZadanej2 = m_ctlSlidPodstFiltraWartZad2.GetPos();
	m_stPID[2 * m_nBiezacyParametr + 1].cPodstawaFiltraWartZad = m_nPodstFiltraWartZadanej2;
	m_stPID[2 * m_nBiezacyParametr + 1].bZmieniony = TRUE;
	*pResult = 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmianę położenia suwaka wielkości akcji wyprzedzającej regulatora głównego
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnNMCustomdrawSliderProcWyprzedzenia1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	m_nProcWyprzedzenia1 = m_ctlSlidProcWyprzedzenia1.GetPos();
	m_stPID[2 * m_nBiezacyParametr + 0].cProcWartZadWyprz = m_nProcWyprzedzenia1;		//ponieważ przesuwanie kursorami nie wywołuje metody OnNMReleasedcaptureSliderFiltrD2, więc aktualizuj również tutaj
	m_strProcWyprzedzenia1.Format(_T("Wielkość akcji wyprzedzającej: %d%% "), m_nProcWyprzedzenia1);
	UpdateData(FALSE);
	*pResult = 0;
}

void KonfigPID::OnReleasedcaptureSliderProcWyprzedzenia1(NMHDR* pNMHDR, LRESULT* pResult)
{
	UpdateData(TRUE);
	m_nProcWyprzedzenia2 = m_ctlSlidProcWyprzedzenia2.GetPos();
	m_stPID[2 * m_nBiezacyParametr + 0].cProcWartZadWyprz = m_nProcWyprzedzenia1;
	m_stPID[2 * m_nBiezacyParametr + 0].bZmieniony = TRUE;
	*pResult = 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmianę położenia suwaka wielkości akcji wyprzedzającej regulatora pochodnej
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnNMCustomdrawSliderProcWyprzedzenia2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	m_nProcWyprzedzenia2 = m_ctlSlidProcWyprzedzenia2.GetPos();
	m_stPID[2 * m_nBiezacyParametr + 1].cProcWartZadWyprz = m_nProcWyprzedzenia2;		//ponieważ przesuwanie kursorami nie wywołuje metody OnNMReleasedcaptureSliderFiltrD2, więc aktualizuj również tutaj
	m_strProcWyprzedzenia2.Format(_T("Wielkość akcji wyprzedzającej: %d%% "), m_nProcWyprzedzenia2);
	UpdateData(FALSE);
	*pResult = 0;
}

void KonfigPID::OnReleasedcaptureSliderProcWyprzedzenia2(NMHDR* pNMHDR, LRESULT* pResult)
{
	UpdateData(TRUE);
	m_nProcWyprzedzenia2 = m_ctlSlidProcWyprzedzenia2.GetPos();
	m_stPID[2 * m_nBiezacyParametr + 1].cProcWartZadWyprz = m_nProcWyprzedzenia2;
	m_stPID[2 * m_nBiezacyParametr + 1].bZmieniony = TRUE;
	*pResult = 0;
}


