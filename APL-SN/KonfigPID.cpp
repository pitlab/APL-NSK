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
	, m_strMinWyj1(_T(""))
	, m_strMaxWyj1(_T(""))
	, m_strMinWyj2(_T(""))
	, m_strMaxWyj2(_T(""))
	, m_strSkalaWartZadanejStab(_T(""))
	, m_strSkalaWartZadanejAkro(_T(""))
	, m_bKatowy(FALSE)
	, m_PodstFiltraD1(0)
	, m_PodstFiltraD2(0)
	, m_strPodstFiltraD1(_T(""))
	, m_strPodstFiltraD2(_T(""))
	, m_bTrybRegulacjiWylaczony(FALSE)
	, m_bTrybRegulacjiReczny(FALSE)
	, m_bTrybRegulacjiAkro(FALSE)
	, m_bTrybRegulacjiStab(FALSE)
	, m_bTrybRegulacjiAuto(FALSE)
	, m_strJednostkaStab(_T(""))
	, m_strJednostkaAkro(_T(""))
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
	DDX_Text(pDX, IDC_EDIT_TD1, m_strTD1);
	DDX_Text(pDX, IDC_EDIT_TD2, m_strTD2);
	DDX_Text(pDX, IDC_EDIT_LIMIT_CALKI1, m_strOgrCalki1);
	DDX_Text(pDX, IDC_EDIT_LIMIT_CALKI2, m_strOgrCalki2);
	DDX_Text(pDX, IDC_EDIT_MIN_WY1, m_strMinWyj1);
	DDX_Text(pDX, IDC_EDIT_MAX_WY1, m_strMaxWyj1);
	DDX_Text(pDX, IDC_EDIT_MIN_WY2, m_strMinWyj2);
	DDX_Text(pDX, IDC_EDIT_MAX_WY2, m_strMaxWyj2);
	DDX_Text(pDX, IDC_EDIT_SKALA_WART_ZAD_STAB, m_strSkalaWartZadanejStab);
	DDX_Text(pDX, IDC_EDIT_SKALA_WART_ZAD_AKRO, m_strSkalaWartZadanejAkro);
	DDX_Check(pDX, IDC_CHECK_KATOWY1, m_bKatowy);
	DDX_Text(pDX, IDC_STATIC_FILTR_D1, m_strPodstFiltraD1);
	DDX_Text(pDX, IDC_STATIC_FILTR_D2, m_strPodstFiltraD2);
	DDX_Control(pDX, IDC_SLIDER_FILTR_D1, m_ctlSlidPOdstCzasuFiltraD1);
	DDX_Control(pDX, IDC_SLIDER_FILTR_D2, m_ctlSlidPOdstCzasuFiltraD2);
	DDX_Check(pDX, IDC_RADIO_REG_WYLACZ, m_bTrybRegulacjiWylaczony);
	DDX_Check(pDX, IDC_RADIO_REG_RECZNY, m_bTrybRegulacjiReczny);
	DDX_Check(pDX, IDC_RADIO_REG_AKRO, m_bTrybRegulacjiAkro);
	DDX_Check(pDX, IDC_RADIO_REG_STAB, m_bTrybRegulacjiStab);
	DDX_Check(pDX, IDC_RADIO_REG_AUTO, m_bTrybRegulacjiAuto);
	DDX_Text(pDX, IDC_STAT_JEDNOSTKA_STAB, m_strJednostkaStab);
	DDX_Text(pDX, IDC_STAT_JEDNOSTKA_AKRO, m_strJednostkaAkro);

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
	ON_EN_CHANGE(IDC_EDIT_SKALA_WART_ZAD_STAB, &KonfigPID::OnEnChangeEditSkalaWartZadStab)
	ON_EN_CHANGE(IDC_EDIT_SKALA_WART_ZAD_AKRO, &KonfigPID::OnEnChangeEditSkalaWartZadAkro)
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
	uint8_t chDane[LICZBA_REG_PARAM];
	uint8_t chErr;
	CString strKomunikat;

	// TODO:  Dodaj tutaj dodatkową inicjację
	m_ctrlKanalPID.InsertItem(PRZE, _T("Przechylenie"));	//regulator sterowania przechyleniem (lotkami w samolocie)
	m_ctrlKanalPID.InsertItem(POCH, _T("Pochylenie"));		//regulator sterowania pochyleniem (sterem wysokości)
	m_ctrlKanalPID.InsertItem(ODCH, _T("Odchylenie"));		//regulator sterowania obrotem (sterem kierunku)
	m_ctrlKanalPID.InsertItem(WYSO, _T("Wysokość"));		//regulator sterowania wysokością
	m_ctrlKanalPID.InsertItem(POZN, _T("Nawigacja N"));		//regulator sterowania prędkością i położeniem północnym
	m_ctrlKanalPID.InsertItem(POZE, _T("Nawigacja E"));		//regulator sterowania prędkością i położeniem wschodnim

 

	m_nBiezacyParametr = m_ctrlKanalPID.GetCurSel();

	for (int n = 0; n < LICZBA_PID; n++)
	{
		chErr = getKomunikacja().CzytajFloatFRAM(fDane, ROZMIAR_REG_PID/4, FAU_PID_P0 + n * ROZMIAR_REG_PID);
		if (chErr == ERR_OK)
		{
			m_stPID[n].fKp = fDane[0];			//FA_USER_PID+0   //4U wzmocnienienie członu P regulatora 0
			m_stPID[n].fTi = fDane[1];			//FA_USER_PID+4   //4U wzmocnienienie członu I regulatora 0
			m_stPID[n].fTd = fDane[2];			//FA_USER_PID+8   //4U wzmocnienienie członu D regulatora 0
			m_stPID[n].fOgrCalki = fDane[3];	//FA_USER_PID+12  //4U górna granica wartości całki członu I regulatora 0
			m_stPID[n].fMinWyj = fDane[4];		//FA_USER_PID+16  //4U 
			m_stPID[n].fMaxWyj = fDane[5];		//FA_USER_PID+20  //4U 
			m_stPID[n].fSkalaWartZadanej = fDane[6];
			m_stPID[n].bZmieniony = FALSE;

			getKomunikacja().RozpakujFloatDoU8(&fDane[7], 1, chDane);
			m_stPID[n].chPodstFiltraD = chDane[0] & 0x3F;
			m_stPID[n].bWylaczony = (chDane[0] & 0x40) >> 6;
			if (n & 0x01)	//regulatory nieparzyste obsługują pochodną, czyli prędkość katową lub liniową, więc nie mogą być kątowe
				m_stPID[n].bKatowy = FALSE;
			else
				m_stPID[n].bKatowy = (chDane[0] & 0x80) >> 7;
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
	
	chErr = getKomunikacja().CzytajU8FRAM(chDane, LICZBA_REG_PARAM, FA_TRYB_REG);
	if (chErr == ERR_OK)
	{
		for (uint8_t n = 0; n < LICZBA_REG_PARAM; n++)
			m_chTrybRegulacji[n] = chDane[n];
		m_bZmienionyTrybRegulacji = FALSE;
	}
	m_ctlSlidPOdstCzasuFiltraD1.SetRange(0, 64);
	m_ctlSlidPOdstCzasuFiltraD2.SetRange(0, 64);
	UpdateData(FALSE);
	UstawKontrolki(m_nBiezacyParametr);
	WlaczKontrolki(m_chTrybRegulacji[m_nBiezacyParametr]);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // WYJĄTEK: Strona właściwości OCX powinna zwrócić FALSE
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Zmienia stan włączenia kontrolek okna dialogowego
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::WlaczKontrolki(uint8_t chTrybPracyRegulatora)
{
	GetDlgItem(IDC_EDIT_KP1)->EnableWindow(chTrybPracyRegulatora >= REG_STAB);
	GetDlgItem(IDC_EDIT_TI1)->EnableWindow(chTrybPracyRegulatora >= REG_STAB);
	GetDlgItem(IDC_EDIT_TD1)->EnableWindow(chTrybPracyRegulatora >= REG_STAB);
	GetDlgItem(IDC_EDIT_MIN_WY1)->EnableWindow(chTrybPracyRegulatora >= REG_STAB);
	GetDlgItem(IDC_EDIT_MAX_WY1)->EnableWindow(chTrybPracyRegulatora >= REG_STAB);
	GetDlgItem(IDC_SLIDER_FILTR_D1)->EnableWindow(chTrybPracyRegulatora >= REG_STAB);
	GetDlgItem(IDC_EDIT_LIMIT_CALKI1)->EnableWindow(chTrybPracyRegulatora >= REG_STAB);
	GetDlgItem(IDC_CHECK_KATOWY1)->EnableWindow(chTrybPracyRegulatora >= REG_STAB);
	GetDlgItem(IDC_EDIT_SKALA_WART_ZAD_STAB)->EnableWindow(chTrybPracyRegulatora >= REG_STAB);

	GetDlgItem(IDC_EDIT_KP2)->EnableWindow(chTrybPracyRegulatora >= REG_AKRO);
	GetDlgItem(IDC_EDIT_TI2)->EnableWindow(chTrybPracyRegulatora >= REG_AKRO);
	GetDlgItem(IDC_EDIT_TD2)->EnableWindow(chTrybPracyRegulatora >= REG_AKRO);
	GetDlgItem(IDC_EDIT_MIN_WY2)->EnableWindow(chTrybPracyRegulatora >= REG_RECZNA);
	GetDlgItem(IDC_EDIT_MAX_WY2)->EnableWindow(chTrybPracyRegulatora >= REG_RECZNA);
	GetDlgItem(IDC_SLIDER_FILTR_D2)->EnableWindow(chTrybPracyRegulatora >= REG_AKRO);
	GetDlgItem(IDC_EDIT_LIMIT_CALKI2)->EnableWindow(chTrybPracyRegulatora >= REG_AKRO);
	GetDlgItem(IDC_EDIT_SKALA_WART_ZAD_AKRO)->EnableWindow(chTrybPracyRegulatora >= REG_RECZNA);
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

	WlaczKontrolki(m_chTrybRegulacji[nParametr]);

	m_strKP1.Format(_T("%.2f"), m_stPID[nRegGlow].fKp);
	m_strKP1.Replace(_T('.'), _T(','));
	m_strKP2.Format(_T("%.2f"), m_stPID[nRegPoch].fKp);
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
	m_strOgrCalki1.Format(_T("%.0f"), m_stPID[nRegGlow].fOgrCalki);
	m_strOgrCalki1.Replace(_T('.'), _T(','));
	m_strOgrCalki2.Format(_T("%.0f"), m_stPID[nRegPoch].fOgrCalki);
	m_strOgrCalki2.Replace(_T('.'), _T(','));
	UpdateData(FALSE);
	m_strMinWyj1.Format(_T("%.0f"), m_stPID[nRegGlow].fMinWyj);
	m_strMinWyj1.Replace(_T('.'), _T(','));
	m_strMinWyj2.Format(_T("%.0f"), m_stPID[nRegPoch].fMinWyj);
	m_strMinWyj2.Replace(_T('.'), _T(','));
	UpdateData(FALSE);
	m_strMaxWyj1.Format(_T("%.0f"), m_stPID[nRegGlow].fMaxWyj);
	m_strMaxWyj1.Replace(_T('.'), _T(','));
	m_strMaxWyj2.Format(_T("%.0f"), m_stPID[nRegPoch].fMaxWyj);
	m_strMaxWyj2.Replace(_T('.'), _T(','));
	UpdateData(FALSE);
	m_strSkalaWartZadanejStab.Format(_T("%.4f"), m_stPID[nRegGlow].fSkalaWartZadanej);
	m_strSkalaWartZadanejStab.Replace(_T('.'), _T(','));
	m_strSkalaWartZadanejAkro.Format(_T("%.4f"), m_stPID[nRegPoch].fSkalaWartZadanej);
	m_strSkalaWartZadanejAkro.Replace(_T('.'), _T(','));
	UpdateData(FALSE);
	m_ctlSlidPOdstCzasuFiltraD1.SetPos(m_stPID[nRegGlow].chPodstFiltraD);
	m_ctlSlidPOdstCzasuFiltraD2.SetPos(m_stPID[nRegPoch].chPodstFiltraD);
	m_bKatowy = m_stPID[nRegGlow].bKatowy;
	UpdateData(FALSE);

	UstawJednostke(nParametr);
	UstawTrybRegulacji(nParametr);
}



void KonfigPID::UstawTrybRegulacji(int nParametr)
{
	/*int nIndeksRegulatora;
	if (nParametr < ROZMIAR_DRAZKOW)
		nIndeksRegulatora = nParametr;
	else
		nIndeksRegulatora = ROZMIAR_DRAZKOW - 1;*/
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
	uint8_t chPodstawaFiltraiBity;
	uint8_t chErr = ERR_OK;
	CString strKomunikat;

	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
	for (int n = 0; n < LICZBA_PID; n++)
	{
		if (m_stPID[n].bZmieniony)	//zapisz tylko te regulatory, które były zmienione
		{		
			chPodstawaFiltraiBity = (m_stPID[n].chPodstFiltraD & 0x3F) + (m_stPID[n].bKatowy * 0x80) + (m_stPID[n].bWylaczony * 0x40);
			chErr |= getKomunikacja().ZapiszKonfiguracjePID(n, m_stPID[n].fKp, m_stPID[n].fTi, m_stPID[n].fTd, m_stPID[n].fOgrCalki, m_stPID[n].fMinWyj, m_stPID[n].fMaxWyj, m_stPID[n].fSkalaWartZadanej, chPodstawaFiltraiBity);
		}
	}

	if (m_bZmienionyTrybRegulacji)
	{
		chErr |= getKomunikacja().ZapiszTrybRegulacji(m_chTrybRegulacji);
	}

	if (chErr != ERR_OK)
	{
		strKomunikat.Format(_T("Błąd nr %d zapisu konfiguracji"), chErr);
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
		m_stPID[n].fTd = 0.01f;
		m_stPID[n].fOgrCalki = 20.0f;
		m_stPID[n].fMinWyj = -100.0f;
		m_stPID[n].fMaxWyj = 100.0f;
		m_stPID[n].bZmieniony = TRUE;
		m_stPID[n].chPodstFiltraD = 8;
	}

	//regulator sterowania przechyleniem (lotkami w samolocie)
	m_stPID[PID_PRZE].fSkalaWartZadanej = 30.0f * DEG2RAD;
	m_stPID[PID_PRZE].bKatowy = TRUE;

	//regulator sterowania prędkością kątową przechylenia (żyroskop P)
	m_stPID[PID_PK_PRZE].fSkalaWartZadanej = 60.0f * DEG2RAD;
	m_stPID[PID_PK_PRZE].bKatowy = FALSE;

	//regulator sterowania pochyleniem (sterem wysokości)
	m_stPID[PID_POCH].fSkalaWartZadanej = 30.0f * DEG2RAD;
	m_stPID[PID_POCH].bKatowy = TRUE;

	//regulator sterowania prędkością kątową pochylenia (żyroskop Q)
	m_stPID[PID_PK_POCH].fSkalaWartZadanej = 60.0f * DEG2RAD;
	m_stPID[PID_PK_POCH].bKatowy = FALSE;

	//regulator sterowania odchyleniem (sterem kierunku)
	m_stPID[PID_ODCH].fSkalaWartZadanej = 180.0f * DEG2RAD;
	m_stPID[PID_ODCH].bKatowy = TRUE;

	//regulator sterowania prędkością kątową odchylenia (żyroskop R)
	m_stPID[PID_PK_ODCH].fSkalaWartZadanej = 20.0f * DEG2RAD;	//[rad/s]
	m_stPID[PID_PK_ODCH].bKatowy = FALSE;

	//regulator sterowania wysokością
	m_stPID[PID_WYSO].fSkalaWartZadanej = 20.0f;	//[m]
	m_stPID[PID_WYSO].bKatowy = FALSE;

	//regulator sterowani prędkością wznoszenia (wario)
	m_stPID[PID_WARIO].fSkalaWartZadanej = 5.0f;	//[m/s]
	m_stPID[PID_WARIO].bKatowy = FALSE;

	//regulator sterowania nawigacją w kierunku północnym
	m_stPID[PID_NAW_N].fSkalaWartZadanej = 0.01f * DEG2RAD;
	m_stPID[PID_NAW_N].bKatowy = TRUE;

	//regulator sterowania prędkością w kierunku północnym
	m_stPID[PID_PRE_N].fSkalaWartZadanej = 0.001f;
	m_stPID[PID_PRE_N].bKatowy = FALSE;

	//regulator sterowania nawigacją w kierunku wschodnim
	m_stPID[PID_NAW_E].fSkalaWartZadanej = 0.01f * DEG2RAD;
	m_stPID[PID_NAW_E].bKatowy = TRUE;

	//regulator sterowania prędkością w kierunku wschodnim
	m_stPID[PID_PRE_E].fSkalaWartZadanej = 0.001f;
	m_stPID[PID_PRE_E].bKatowy = FALSE;

	//tryby regulacji
	m_chTrybRegulacji[PRZE] = REG_STAB;		//regulator sterowania przechyleniem (lotkami w samolocie)    
	m_chTrybRegulacji[POCH] = REG_STAB;		//regulator sterowania pochyleniem (sterem wysokości)
	m_chTrybRegulacji[ODCH] = REG_STAB;     //regulator sterowania obrotem (sterem kierunku)
	m_chTrybRegulacji[WYSO] = REG_AKRO;		//regulator sterowania wysokością
	m_chTrybRegulacji[POZN] = REG_AUTO;		//regulator sterowania prędkością i położeniem północnym
	m_chTrybRegulacji[POZE] = REG_AUTO;		//regulator sterowania prędkością i położeniem wschodnim
	m_bZmienionyTrybRegulacji = TRUE;
	OnBnClickedOk();
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
		m_strJednostkaStab = _T("[rad]");
		m_strJednostkaAkro = _T("[rad/s]");
		break;

	case WYSO:
		m_strJednostkaStab = _T("[m]");
		m_strJednostkaAkro = _T("[m/s]");
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
	WlaczKontrolki(m_chTrybRegulacji[m_nBiezacyParametr]);

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
// Edycja kontrolki skalowania wartości zadanej regulatora podstawowego
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnEnChangeEditSkalaWartZadStab()
{
	UpdateData(TRUE);
	m_stPID[2 * m_nBiezacyParametr + 0].fSkalaWartZadanej = ZamienStrNaFloat(m_strSkalaWartZadanejStab.GetString());
	m_stPID[2 * m_nBiezacyParametr + 0].bZmieniony = TRUE;
	UpdateData(FALSE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Edycja kontrolki skalowania wartości zadanej regulatora pochodnej
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnEnChangeEditSkalaWartZadAkro()
{
	UpdateData(TRUE);
	m_stPID[2 * m_nBiezacyParametr + 1].fSkalaWartZadanej = ZamienStrNaFloat(m_strSkalaWartZadanejAkro.GetString());
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
	float fCzas;

	UpdateData(TRUE);
	m_PodstFiltraD1 = m_ctlSlidPOdstCzasuFiltraD1.GetPos();
	m_stPID[2 * m_nBiezacyParametr + 0].chPodstFiltraD = m_PodstFiltraD1;	//ponieważ przesuwanie kursorami nie wywołuje metody OnNMReleasedcaptureSliderFiltrD1, więc aktualizuj również tutaj podczas odświeżania
	if (m_PodstFiltraD1)
		fCzas = m_PodstFiltraD1 / 200.f * 1000;
	else
		fCzas = 0.0f;
	m_strPodstFiltraD1.Format(_T("Filtr D: %d (%.0f ms)"), m_PodstFiltraD1, fCzas);
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
	m_PodstFiltraD1 = m_ctlSlidPOdstCzasuFiltraD1.GetPos();
	m_stPID[2 * m_nBiezacyParametr + 0].chPodstFiltraD = m_PodstFiltraD1;
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
	float fCzas;

	UpdateData(TRUE);
	m_PodstFiltraD2 = m_ctlSlidPOdstCzasuFiltraD2.GetPos();
	m_stPID[2 * m_nBiezacyParametr + 1].chPodstFiltraD = m_PodstFiltraD2;		//ponieważ przesuwanie kursorami nie wywołuje metody OnNMReleasedcaptureSliderFiltrD2, więc aktualizuj również tutaj
	if (m_PodstFiltraD2)
		fCzas = m_PodstFiltraD2 / 200.f * 1000;
	else
		fCzas = 0.0f;
	m_strPodstFiltraD2.Format(_T("Filtr D: %d (%.0f ms)"), m_PodstFiltraD2, fCzas);	
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
	m_PodstFiltraD2 = m_ctlSlidPOdstCzasuFiltraD2.GetPos();
	m_stPID[2 * m_nBiezacyParametr + 1].chPodstFiltraD = m_PodstFiltraD2;
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
	m_stPID[2 * m_nBiezacyParametr + 0].bWylaczony = TRUE;
	m_stPID[2 * m_nBiezacyParametr + 1].bWylaczony = TRUE;
	UstawTrybRegulacji(m_nBiezacyParametr);
	WlaczKontrolki(m_chTrybRegulacji[m_nBiezacyParametr]);
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
	WlaczKontrolki(m_chTrybRegulacji[m_nBiezacyParametr]);
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
	WlaczKontrolki(m_chTrybRegulacji[m_nBiezacyParametr]);
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
	WlaczKontrolki(m_chTrybRegulacji[m_nBiezacyParametr]);
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
	WlaczKontrolki(m_chTrybRegulacji[m_nBiezacyParametr]);
	m_bZmienionyTrybRegulacji = TRUE;
	UpdateData(FALSE);
}



