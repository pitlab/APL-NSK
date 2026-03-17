// Wskaznik_LED.cpp: plik implementacji
//

#include "pch.h"
#include "APL-SN.h"
#include "Wskaznik_LED.h"
#include "afxdialogex.h"
#include "Errors.h"

// Okno dialogowe Wskaznik_LED

IMPLEMENT_DYNAMIC(Wskaznik_LED, CDialogEx)

Wskaznik_LED::Wskaznik_LED(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WSKAZNIK_LED, pParent)
	, m_strLiczbaLED(_T(""))
	, m_strMinCzerwony(_T(""))
	, m_strMaxCzerwony(_T(""))
	, m_strMinZielony(_T(""))
	, m_strMaxZielony(_T(""))
	, m_strMinNiebieski(_T(""))
	, m_strMaxNiebieski(_T(""))
	, m_strMinZmiennej(_T(""))
	, m_strMaxZmiennej(_T(""))
	, m_strSzerokoscWskaznika(_T(""))
	, m_strKontrastTla(_T(""))
{

}

Wskaznik_LED::~Wskaznik_LED()
{
}

void Wskaznik_LED::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LICZBA_LED, m_strLiczbaLED);
	//DDX_Slider(pDX, IDC_SLIDER_SZER_WSKAZN, (int&)m_stWskaznikLED[m_nIndeksWskaznikaLed].chSzerWskaznika);
	//DDX_Slider(pDX, IDC_SLIDER_JASNOSC_TLA, (int&)m_stWskaznikLED[m_nIndeksWskaznikaLed].chKontrastTla);
	DDX_Text(pDX, IDC_EDIT_MIN_CZERWONY, m_strMinCzerwony);
	DDX_Text(pDX, IDC_EDIT_MAX_CZERWONY, m_strMaxCzerwony);
	DDX_Text(pDX, IDC_EDIT_MIN_ZIELONY, m_strMinZielony);
	DDX_Text(pDX, IDC_EDIT_MAX_ZIELONY, m_strMaxZielony);
	DDX_Text(pDX, IDC_EDIT_MIN_NIEBIESKI, m_strMinNiebieski);
	DDX_Text(pDX, IDC_EDIT_MAX_NIEBIESKI, m_strMaxNiebieski);
	DDX_Control(pDX, IDC_COMBO_ZMIENNA, m_ctlZmienna);
	DDX_Text(pDX, IDC_EDIT_ZMIENNA_MIN, m_strMinZmiennej);
	DDX_Text(pDX, IDC_EDIT_ZMIENNA_MAX, m_strMaxZmiennej);
	DDX_Control(pDX, IDC_SLID_SZER_WSKAZN, m_ctlSzerWskazn);
	DDX_Control(pDX, IDC_SLIDER_JASNOSC_TLA, m_ctlKontrastTla);
	DDX_Text(pDX, IDC_STATIC_SZEROKOSC_WSKAZNIKA, m_strSzerokoscWskaznika);
	DDX_Text(pDX, IDC_STATIC_JASNOSC_TLA, m_strKontrastTla);
	DDX_Control(pDX, IDC_TAB_NR_WSKAZNIKA, m_ctlIndeksWskaznika);
	DDX_Control(pDX, IDC_COMBO_TYP_LED, m_ctlTypLed);
	
}


BEGIN_MESSAGE_MAP(Wskaznik_LED, CDialogEx)
	ON_BN_CLICKED(IDOK, &Wskaznik_LED::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_LICZBA_LED, &Wskaznik_LED::OnEnChangeEditLiczbaLed)
	ON_EN_CHANGE(IDC_EDIT_MIN_CZERWONY, &Wskaznik_LED::OnEnChangeEditMinCzerwony)
	ON_EN_CHANGE(IDC_EDIT_MAX_CZERWONY, &Wskaznik_LED::OnEnChangeEditMaxCzerwony)
	ON_EN_CHANGE(IDC_EDIT_MIN_ZIELONY, &Wskaznik_LED::OnEnChangeMinZielony)
	ON_EN_CHANGE(IDC_EDIT_MAX_ZIELONY, &Wskaznik_LED::OnEnChangeEditMaxZielony)
	ON_EN_CHANGE(IDC_EDIT_MIN_NIEBIESKI, &Wskaznik_LED::OnEnChangeEditminNiebieski)
	ON_EN_CHANGE(IDC_EDIT_MAX_NIEBIESKI, &Wskaznik_LED::OnEnChangeEditMaxNiebieski)
	ON_CBN_SELCHANGE(IDC_COMBO_ZMIENNA, &Wskaznik_LED::OnCbnSelchangeComboZmienna)
	ON_CBN_SELCHANGE(IDC_COMBO_TYP_LED, &Wskaznik_LED::OnCbnSelchangeComboTypLed)
	ON_EN_CHANGE(IDC_EDIT_ZMIENNA_MIN, &Wskaznik_LED::OnEnChangeEditZmiennaMin)
	ON_EN_CHANGE(IDC_EDIT_ZMIENNA_MAX, &Wskaznik_LED::OnEnChangeEditZmiennaMax)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_JASNOSC_TLA, &Wskaznik_LED::OnNMCustomdrawSliderJasnoscTla)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_NR_WSKAZNIKA, &Wskaznik_LED::OnTcnSelchangeTabNrWskaznika)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_SZER_WSKAZN, &Wskaznik_LED::OnNMCustomdrawSlidSzerWskazn)
END_MESSAGE_MAP()


// Procedury obsługi komunikatów Wskaznik_LED
BOOL Wskaznik_LED::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	uint8_t chBłąd;
	uint8_t chDane[ROZMIAR_WSKAZNIKA_LED - 8];
	float fDane[2];

	//wstaw nazwy zmiennych do kontrolki combo
	m_ctlZmienna.InsertString(0, _T("BSP.Pochylenie"));
	m_ctlZmienna.InsertString(1, _T("BSP.Przechylenie"));
	m_ctlZmienna.InsertString(2, _T("BSP.Odchylenie"));
	m_ctlZmienna.InsertString(3, _T("BSP.Wysokosc AGL"));
	m_ctlZmienna.InsertString(4, _T("Napiecie zasilania"));

	m_ctlSzerWskazn.SetRangeMin(1);
	m_ctlSzerWskazn.SetRangeMax(6);
	UpdateData(FALSE);

	m_ctlKontrastTla.SetRangeMin(2);
	m_ctlKontrastTla.SetRangeMax(20);
	UpdateData(FALSE);

	m_ctlIndeksWskaznika.InsertItem(0, _T("Wskaźnik lewy"));
	m_ctlIndeksWskaznika.InsertItem(1, _T("Wskaźnik prawy"));

	m_ctlTypLed.InsertString(0, _T("WS2811"));
	m_ctlTypLed.InsertString(1, _T("WS2813"));

	//czytaj i ustaw konfigurację typu używanych LED
	chBłąd = getKomunikacja().CzytajU8FRAM(chDane, 1, FAU_WSKLED_TYP_LED);
	if (chBłąd == ERR_OK)
	{
		m_nTypLed = chDane[0];		
		m_bBylaZmianaTypuLed = FALSE;
	}
	else
		return FALSE;

	m_ctlTypLed.SetCurSel(m_nTypLed);
	UpdateData(FALSE);

	m_nIndeksWskaznikaLed = 0;

	//odczytaj z APL3 konfigurację dla wszystkich wskaźników
	for (int n = 0; n < LICZBA_WSKAZNIKOW_LED; n++)
	{
		chBłąd = getKomunikacja().CzytajFloatFRAM(fDane, 2, FAU_WSKLED1_MIN_ZMIENNEJ + n * ROZMIAR_WSKAZNIKA_LED);
		if (chBłąd == ERR_OK)
		{
			m_stWskaznikLED[n].fMinZmiennej = fDane[0];
			m_stWskaznikLED[n].fMaxZmiennej = fDane[1];
			m_bBylaZmianaLiczbFloat[n] = FALSE;
		}
		else
			return FALSE;

		chBłąd = getKomunikacja().CzytajU8FRAM(chDane, 10, FAU_WSKLED1_NUM_ZMIENNEJ + n * ROZMIAR_WSKAZNIKA_LED);
		if (chBłąd == ERR_OK)
		{
			m_stWskaznikLED[n].chNumZmiennej = chDane[0];
			m_stWskaznikLED[n].chSzerWskaznika = chDane[1];
			m_stWskaznikLED[n].chKontrastTla = chDane[2];
			m_stWskaznikLED[n].chMinCzer = chDane[3];
			m_stWskaznikLED[n].chMaxCzer = chDane[4];
			m_stWskaznikLED[n].chMinZiel = chDane[5];
			m_stWskaznikLED[n].chMaxZiel = chDane[6];
			m_stWskaznikLED[n].chMinNieb = chDane[7];
			m_stWskaznikLED[n].chMaxNieb = chDane[8];
			m_stWskaznikLED[n].chLiczbaLed = chDane[9];
			m_bBylaZmianaLiczbUint8[n] = FALSE;
		}
		else
			return FALSE;
	}
	WstawDaneDoKontrolek(m_stWskaznikLED, m_nIndeksWskaznikaLed);
	return TRUE;  
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// funkcja wypełnia kontrolki danymi
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void Wskaznik_LED::WstawDaneDoKontrolek(stWskaznikLED_t *WskLed, int nIndeks)
{
	m_strLiczbaLED.Format(_T("%d"), WskLed[nIndeks].chLiczbaLed);
	m_strMinCzerwony.Format(_T("%d"), WskLed[nIndeks].chMinCzer);
	m_strMaxCzerwony.Format(_T("%d"), WskLed[nIndeks].chMaxCzer);
	m_strMinZielony.Format(_T("%d"), WskLed[nIndeks].chMinZiel);
	m_strMaxZielony.Format(_T("%d"), WskLed[nIndeks].chMaxZiel);
	m_strMinNiebieski.Format(_T("%d"), WskLed[nIndeks].chMinNieb);
	m_strMaxNiebieski.Format(_T("%d"), WskLed[nIndeks].chMaxNieb);
	m_strMinZmiennej.Format(_T("%.4f"), WskLed[nIndeks].fMinZmiennej);
	m_strMaxZmiennej.Format(_T("%.4f"), WskLed[nIndeks].fMaxZmiennej);
	UpdateData(FALSE);
	m_ctlSzerWskazn.SetPos(WskLed[nIndeks].chSzerWskaznika);
	m_ctlKontrastTla.SetPos(WskLed[nIndeks].chKontrastTla);
	UpdateData(FALSE);
	m_ctlZmienna.SetCurSel(WskLed[nIndeks].chNumZmiennej);
	UpdateData(FALSE);
	m_strSzerokoscWskaznika.Format(_T("Szerokość wskaźn: %d"), WskLed[nIndeks].chSzerWskaznika);
	m_strKontrastTla.Format(_T("Kontrast tła: %d"), WskLed[nIndeks].chKontrastTla);
	UpdateData(FALSE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na naciśnięcie przycisku OK - zapisuje konfigurację jeżeli została zmieniona
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void Wskaznik_LED::OnBnClickedOk()
{
	uint8_t chBłąd;
	uint8_t chDane[ROZMIAR_WSKAZNIKA_LED - 8];
	float fDane[2];
	CString strKomunikat;

	if (m_bBylaZmianaTypuLed)
	{
		chDane[0] = m_nTypLed;
		chBłąd = getKomunikacja().ZapiszDaneU8FRAM(chDane, 1, FAU_WSKLED_TYP_LED);
		if (chBłąd)
		{
			strKomunikat.Format(_T("Błąd nr %d zapisu konfiguracji"), chBłąd);
			MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojoj!"), MB_ICONWARNING, 0);
			CDialogEx::OnOK();
		}
	}

	for (int n = 0; n < LICZBA_WSKAZNIKOW_LED; n++)
	{
		if (m_bBylaZmianaLiczbUint8[n])
		{
			chDane[0] = m_stWskaznikLED[n].chNumZmiennej;
			chDane[1] = m_stWskaznikLED[n].chSzerWskaznika;
			chDane[2] = m_stWskaznikLED[n].chKontrastTla;
			chDane[3] = m_stWskaznikLED[n].chMinCzer;
			chDane[4] = m_stWskaznikLED[n].chMaxCzer;
			chDane[5] = m_stWskaznikLED[n].chMinZiel;
			chDane[6] = m_stWskaznikLED[n].chMaxZiel;
			chDane[7] = m_stWskaznikLED[n].chMinNieb;
			chDane[8] = m_stWskaznikLED[n].chMaxNieb;
			chDane[9] = m_stWskaznikLED[n].chLiczbaLed;
			chBłąd = getKomunikacja().ZapiszDaneU8FRAM(chDane, 10, FAU_WSKLED1_NUM_ZMIENNEJ + n * ROZMIAR_WSKAZNIKA_LED);
			if (chBłąd)
			{
				strKomunikat.Format(_T("Błąd nr %d zapisu konfiguracji"), chBłąd);
				MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojoj!"), MB_ICONWARNING, 0);
				CDialogEx::OnOK();
			}
		}

		if (m_bBylaZmianaLiczbFloat[n])
		{
			fDane[0] = m_stWskaznikLED[n].fMinZmiennej;
			fDane[1] = m_stWskaznikLED[n].fMaxZmiennej;
			chBłąd = getKomunikacja().ZapiszDaneFloatFRAM(fDane, 2, FAU_WSKLED1_MIN_ZMIENNEJ + n * ROZMIAR_WSKAZNIKA_LED);
			if (chBłąd)
			{
				strKomunikat.Format(_T("Błąd nr %d zapisu konfiguracji"), chBłąd);
				MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojoj!"), MB_ICONWARNING, 0);
				CDialogEx::OnOK();
			}
		}
	}
	CDialogEx::OnOK();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na edycję liczby LED-ów
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void Wskaznik_LED::OnEnChangeEditLiczbaLed()
{
	UpdateData();
	m_stWskaznikLED[m_nIndeksWskaznikaLed].chLiczbaLed = (uint8_t)(_wtoi(m_strLiczbaLED) & 0xFF);
	m_bBylaZmianaLiczbUint8[m_nIndeksWskaznikaLed] = TRUE;
	Invalidate();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na przesunięcie suwaka szerokości świecącego punktu
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void Wskaznik_LED::OnNMCustomdrawSlidSzerWskazn(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	UpdateData(TRUE);
	m_stWskaznikLED[m_nIndeksWskaznikaLed].chSzerWskaznika = (uint8_t)(m_ctlSzerWskazn.GetPos() & 0xFF);
	m_strSzerokoscWskaznika.Format(_T("Szerokość wskaźn: %d"), m_stWskaznikLED[m_nIndeksWskaznikaLed].chSzerWskaznika);
	UpdateData(FALSE);
	m_bBylaZmianaLiczbUint8[m_nIndeksWskaznikaLed] = TRUE;
	* pResult = 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na przesunięcie suwaka jasności tła
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void Wskaznik_LED::OnNMCustomdrawSliderJasnoscTla(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	
	UpdateData(TRUE);
	m_stWskaznikLED[m_nIndeksWskaznikaLed].chKontrastTla = (uint8_t)(m_ctlKontrastTla.GetPos() & 0xFF);
	m_strKontrastTla.Format(_T("Kontrast tła: %d"), m_stWskaznikLED[m_nIndeksWskaznikaLed].chKontrastTla);
	UpdateData(FALSE);
	m_bBylaZmianaLiczbUint8[m_nIndeksWskaznikaLed] = TRUE;
	*pResult = 0;
}



void Wskaznik_LED::OnEnChangeEditMinCzerwony()
{
	UpdateData(TRUE);
	m_stWskaznikLED[m_nIndeksWskaznikaLed].chMinCzer = (uint8_t)(_wtoi(m_strMinCzerwony) & 0xFF);
	m_bBylaZmianaLiczbUint8[m_nIndeksWskaznikaLed] = TRUE;
	Invalidate();
}


void Wskaznik_LED::OnEnChangeEditMaxCzerwony()
{
	UpdateData(TRUE);
	m_stWskaznikLED[m_nIndeksWskaznikaLed].chMaxCzer = (uint8_t)(_wtoi(m_strMaxCzerwony) & 0xFF);
	m_bBylaZmianaLiczbUint8[m_nIndeksWskaznikaLed] = TRUE;
	Invalidate();
}


void Wskaznik_LED::OnEnChangeMinZielony()
{
	UpdateData(TRUE);
	m_stWskaznikLED[m_nIndeksWskaznikaLed].chMinZiel = (uint8_t)(_wtoi(m_strMinZielony) & 0xFF);
	m_bBylaZmianaLiczbUint8[m_nIndeksWskaznikaLed] = TRUE;
	Invalidate();
}


void Wskaznik_LED::OnEnChangeEditMaxZielony()
{
	UpdateData(TRUE);
	m_stWskaznikLED[m_nIndeksWskaznikaLed].chMaxZiel = (uint8_t)(_wtoi(m_strMaxZielony) & 0xFF);
	m_bBylaZmianaLiczbUint8[m_nIndeksWskaznikaLed] = TRUE;
	Invalidate();
}


void Wskaznik_LED::OnEnChangeEditminNiebieski()
{
	UpdateData(TRUE);
	m_stWskaznikLED[m_nIndeksWskaznikaLed].chMinNieb = (uint8_t)(_wtoi(m_strMinNiebieski) & 0xFF);
	m_bBylaZmianaLiczbUint8[m_nIndeksWskaznikaLed] = TRUE;
	Invalidate();
}


void Wskaznik_LED::OnEnChangeEditMaxNiebieski()
{
	UpdateData(TRUE);
	m_stWskaznikLED[m_nIndeksWskaznikaLed].chMaxNieb = (uint8_t)(_wtoi(m_strMaxNiebieski) & 0xFF);
	m_bBylaZmianaLiczbUint8[m_nIndeksWskaznikaLed] = TRUE;
	Invalidate();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmianę comboBox zawerajacej typ zmiennej uzywanej do wizualizacji
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void Wskaznik_LED::OnCbnSelchangeComboZmienna()
{
	UpdateData(TRUE);
	m_stWskaznikLED[m_nIndeksWskaznikaLed].chNumZmiennej = (uint8_t)(m_ctlZmienna.GetCurSel() & 0xFF);
	m_bBylaZmianaLiczbUint8[m_nIndeksWskaznikaLed] = TRUE;
	Invalidate();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na edycję stringu minimalnej zmiennej
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void Wskaznik_LED::OnEnChangeEditZmiennaMin()
{

	UpdateData(TRUE);
	m_strMinZmiennej.Replace(_T(','), _T('.'));
	m_stWskaznikLED[m_nIndeksWskaznikaLed].fMinZmiennej = (float)_ttof(m_strMinZmiennej);
	m_bBylaZmianaLiczbFloat[m_nIndeksWskaznikaLed] = TRUE;
	Invalidate();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na edycję stringu maksymalnej zmiennej
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void Wskaznik_LED::OnEnChangeEditZmiennaMax()
{
	UpdateData(TRUE);
	m_strMaxZmiennej.Replace(_T(','), _T('.'));
	m_stWskaznikLED[m_nIndeksWskaznikaLed].fMaxZmiennej = (float)_ttof(m_strMaxZmiennej);
	m_bBylaZmianaLiczbFloat[m_nIndeksWskaznikaLed] = TRUE;
	Invalidate();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na kliknięcie zakładki indeksu wskaźnika
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void Wskaznik_LED::OnTcnSelchangeTabNrWskaznika(NMHDR* pNMHDR, LRESULT* pResult)
{
	UpdateData(TRUE);
	m_nIndeksWskaznikaLed = m_ctlIndeksWskaznika.GetCurSel();
	WstawDaneDoKontrolek(m_stWskaznikLED, m_nIndeksWskaznikaLed);
	//UpdateData(FALSE);	
	*pResult = 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmianę ComboBox z typem LED-ów
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void Wskaznik_LED::OnCbnSelchangeComboTypLed()
{
	UpdateData(TRUE);
	m_nTypLed = m_ctlTypLed.GetCurSel();	
	m_bBylaZmianaTypuLed = TRUE;
}


