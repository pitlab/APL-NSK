// Wskaznik_LED.cpp: plik implementacji
//

#include "pch.h"
#include "APL-SN.h"
#include "Wskaznik_LED.h"
#include "afxdialogex.h"


// Okno dialogowe Wskaznik_LED

IMPLEMENT_DYNAMIC(Wskaznik_LED, CDialogEx)

Wskaznik_LED::Wskaznik_LED(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WSKAZNIK_LED, pParent)
	, m_strLiczbaLED(_T(""))
	, m_nSzerokoscPunktu(0)
	, m_nJasnoscTla(0)
	, m_strMinCzerwony(_T(""))
	, m_strMaxCzerwony(_T(""))
	, m_strMinZielony(_T(""))
	, m_strMaxZielony(_T(""))
	, m_strMinNiebieski(_T(""))
	, m_strMaxNiebieski(_T(""))
	, m_strMinZmiennej(_T(""))
	, m_strMaxZmiennej(_T(""))
	, m_strSzerokoscPunktu(_T(""))
	, m_strJasnoscTla(_T(""))
{

}

Wskaznik_LED::~Wskaznik_LED()
{
}

void Wskaznik_LED::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LICZBA_LED, m_strLiczbaLED);
	DDX_Slider(pDX, IDC_SLIDER_SZEROKOSC_PUNKTU, m_nSzerokoscPunktu);
	DDX_Slider(pDX, IDC_SLIDER_JASNOSC_TLA, m_nJasnoscTla);
	DDX_Text(pDX, IDC_EDIT_MIN_CZERWONY, m_strMinCzerwony);
	DDX_Text(pDX, IDC_EDIT_MAX_CZERWONY, m_strMaxCzerwony);
	DDX_Text(pDX, IDC_MIN_ZIELONY, m_strMinZielony);
	DDX_Text(pDX, IDC_EDIT_MAX_ZIELONY, m_strMaxZielony);
	DDX_Text(pDX, IDC_EDITMIN_NIEBIESKI, m_strMinNiebieski);
	DDX_Text(pDX, IDC_EDIT_MAX_NIEBIESKI, m_strMaxNiebieski);
	DDX_Control(pDX, IDC_COMBO_ZMIENNA, m_ctlZmienna);
	DDX_Text(pDX, IDC_EDIT_ZMIENNA_MIN, m_strMinZmiennej);
	DDX_Text(pDX, IDC_EDIT_ZMIENNA_MAX, m_strMaxZmiennej);
	DDX_Control(pDX, IDC_SLIDER_SZEROKOSC_PUNKTU, m_ctlSzerokoscPunktu);
	DDX_Control(pDX, IDC_SLIDER_JASNOSC_TLA, m_ctlJasnoscTla);
	DDX_Text(pDX, IDC_STATIC_SZEROKOSC_PUNKTU, m_strSzerokoscPunktu);
	DDX_Text(pDX, IDC_STATIC_JASNOSC_TLA, m_strJasnoscTla);
}


BEGIN_MESSAGE_MAP(Wskaznik_LED, CDialogEx)
	ON_BN_CLICKED(IDOK, &Wskaznik_LED::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_LICZBA_LED, &Wskaznik_LED::OnEnChangeEditLiczbaLed)
	ON_EN_CHANGE(IDC_EDIT_MIN_CZERWONY, &Wskaznik_LED::OnEnChangeEditMinCzerwony)
	ON_EN_CHANGE(IDC_EDIT_MAX_CZERWONY, &Wskaznik_LED::OnEnChangeEditMaxCzerwony)
	ON_EN_CHANGE(IDC_MIN_ZIELONY, &Wskaznik_LED::OnEnChangeMinZielony)
	ON_EN_CHANGE(IDC_EDIT_MAX_ZIELONY, &Wskaznik_LED::OnEnChangeEditMaxZielony)
	ON_EN_CHANGE(IDC_EDITMIN_NIEBIESKI, &Wskaznik_LED::OnEnChangeEditminNiebieski)
	ON_EN_CHANGE(IDC_EDIT_MAX_NIEBIESKI, &Wskaznik_LED::OnEnChangeEditMaxNiebieski)
	ON_CBN_SELCHANGE(IDC_COMBO_ZMIENNA, &Wskaznik_LED::OnCbnSelchangeComboZmienna)
	ON_EN_CHANGE(IDC_EDIT_ZMIENNA_MIN, &Wskaznik_LED::OnEnChangeEditZmiennaMin)
	ON_EN_CHANGE(IDC_EDIT_ZMIENNA_MAX, &Wskaznik_LED::OnEnChangeEditZmiennaMax)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SZEROKOSC_PUNKTU, &Wskaznik_LED::OnNMCustomdrawSliderSzerokoscPunktu)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_JASNOSC_TLA, &Wskaznik_LED::OnNMCustomdrawSliderJasnoscTla)
END_MESSAGE_MAP()


// Procedury obsługi komunikatów Wskaznik_LED

///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na naciśnięcie przycisku OK - zapisuje konfigurcję jeżeli została zmieniona
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void Wskaznik_LED::OnBnClickedOk()
{
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
	CDialogEx::OnOK();
}


BOOL Wskaznik_LED::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//wstaw nazwy zmiennych do kontrolki combo
	m_ctlZmienna.InsertString(0, _T("BSP.Pochylenie"));
	m_ctlZmienna.InsertString(1, _T("BSP.Przechylenie"));
	m_ctlZmienna.InsertString(2, _T("BSP.Odchylenie"));
	m_ctlZmienna.InsertString(3, _T("BSP.Wysokosc AGL"));
	m_ctlZmienna.InsertString(4, _T("Napiecie zasilania"));

	m_ctlSzerokoscPunktu.SetRangeMin(1);
	m_ctlSzerokoscPunktu.SetRangeMax(5);

	m_ctlJasnoscTla.SetRangeMin(2);
	m_ctlJasnoscTla.SetRangeMax(20);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // WYJĄTEK: Strona właściwości OCX powinna zwrócić FALSE
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na edycję liczby LED-ów
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void Wskaznik_LED::OnEnChangeEditLiczbaLed()
{
	UpdateData();
	m_nLiczbaLED = (int)_wtoi(m_strLiczbaLED);
	m_bBylaZmianaLiczbyLed = TRUE;
	Invalidate();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na przesunięcie suwaka szerokości świecącego punktu
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void Wskaznik_LED::OnNMCustomdrawSliderSzerokoscPunktu(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
	UpdateData(TRUE);
	m_nSzerokoscPunktu = m_ctlSzerokoscPunktu.GetPos();
	m_strSzerokoscPunktu.Format(_T("Szerokość punktu = %d"), m_nSzerokoscPunktu);
	*pResult = 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na przesunięcie suwaka jasności tła
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void Wskaznik_LED::OnNMCustomdrawSliderJasnoscTla(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
	UpdateData(TRUE);
	m_nJasnoscTla = m_ctlJasnoscTla.GetPos();
	m_strJasnoscTla.Format(_T("Jasność Tła = %d"), m_nJasnoscTla);
	*pResult = 0;
}



void Wskaznik_LED::OnEnChangeEditMinCzerwony()
{
	UpdateData();
	m_nMinCzerwony = (int)_wtoi(m_strMinCzerwony);
	m_bBylaZmianaKolorow = TRUE;
	Invalidate();
}


void Wskaznik_LED::OnEnChangeEditMaxCzerwony()
{
	UpdateData();
	m_nMaxCzerwony = (int)_wtoi(m_strMaxCzerwony);
	m_bBylaZmianaKolorow = TRUE;
	Invalidate();
}


void Wskaznik_LED::OnEnChangeMinZielony()
{
	UpdateData();
	m_nMinZielony = (int)_wtoi(m_strMinZielony);
	m_bBylaZmianaKolorow = TRUE;
	Invalidate();
}


void Wskaznik_LED::OnEnChangeEditMaxZielony()
{
	UpdateData();
	m_nMaxZielony = (int)_wtoi(m_strMaxZielony);
	m_bBylaZmianaKolorow = TRUE;
	Invalidate();
}


void Wskaznik_LED::OnEnChangeEditminNiebieski()
{
	UpdateData();
	m_nMinNiebieski = (int)_wtoi(m_strMinNiebieski);
	m_bBylaZmianaKolorow = TRUE;
	Invalidate();
}


void Wskaznik_LED::OnEnChangeEditMaxNiebieski()
{
	UpdateData();
	m_nMaxNiebieski = (int)_wtoi(m_strMaxNiebieski);
	m_bBylaZmianaKolorow = TRUE;
	Invalidate();
}


void Wskaznik_LED::OnCbnSelchangeComboZmienna()
{
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
}


void Wskaznik_LED::OnEnChangeEditZmiennaMin()
{
	// TODO:  Jeżeli to jest kontrolka RICHEDIT, to kontrolka nie będzie
	// wyślij to powiadomienie, chyba że przesłonisz element CDialogEx::OnInitDialog()
	// funkcja i wywołanie CRichEditCtrl().SetEventMask()
	// z flagą ENM_CHANGE zsumowaną logicznie z maską.

	// TODO:  Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
}


void Wskaznik_LED::OnEnChangeEditZmiennaMax()
{
	// TODO:  Jeżeli to jest kontrolka RICHEDIT, to kontrolka nie będzie
	// wyślij to powiadomienie, chyba że przesłonisz element CDialogEx::OnInitDialog()
	// funkcja i wywołanie CRichEditCtrl().SetEventMask()
	// z flagą ENM_CHANGE zsumowaną logicznie z maską.

	// TODO:  Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
}




