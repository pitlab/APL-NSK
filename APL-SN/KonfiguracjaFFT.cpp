// KonfiguracjaFFT.cpp: plik implementacji
//

#include "pch.h"
#include "APL-SN.h"
#include "KonfiguracjaFFT.h"
#include "afxdialogex.h"
#include "sys_def_wspolny.h"

// Okno dialogowe KonfiguracjaFFT

IMPLEMENT_DYNAMIC(KonfiguracjaFFT, CDialogEx)

KonfiguracjaFFT::KonfiguracjaFFT(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PARAMETRY_FFT, pParent)
	, m_strRozdzielczośćFFT(_T(""))
	, m_strWysterowanie(_T(""))
	, m_bZmienionoDane(FALSE)
	, m_bSilnik1(FALSE)
	, m_bSilnik2(FALSE)
	, m_bSilnik3(FALSE)
	, m_bSilnik4(FALSE)
	, m_bSilnik5(FALSE)
	, m_bSilnik6(FALSE)
	, m_chWykładnikPotęgi(0)
	, m_chRodzajOkna(0)
	, m_chWysterowanieMaxProcent(0)
	, m_chAktywneSilniki(0)
	, m_nRozmiarFFT(0)
	, m_strRozmiarFFT(_T(""))
{

}

KonfiguracjaFFT::~KonfiguracjaFFT()
{
}

void KonfiguracjaFFT::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLID_ROZDZIELCZOSC_FFT, m_ctlRozdzielczoscFFT);
	DDX_Control(pDX, IDC_COMBO_RODZAJ_OKNA, m_ctlRodzajOkna);
	DDX_Control(pDX, IDC_SLID_ZAKRES_WYSTEROWANIA, m_ctlZakresWysterowania);
	DDX_Text(pDX, IDC_STATIC_WYSTEROWANIE, m_strWysterowanie);
	DDX_Check(pDX, IDC_CHECK_SILNIK1, m_bSilnik1);
	DDX_Check(pDX, IDC_CHECK_SILNIK2, m_bSilnik2);
	DDX_Check(pDX, IDC_CHECK_SILNIK3, m_bSilnik3);
	DDX_Check(pDX, IDC_CHECK_SILNIK4, m_bSilnik4);
	DDX_Check(pDX, IDC_CHECK_SILNIK5, m_bSilnik5);
	DDX_Check(pDX, IDC_CHECK_SILNIK6, m_bSilnik6);
	DDX_Text(pDX, IDC_STAT_ROZMIAR_FFT, m_strRozmiarFFT);
}


BEGIN_MESSAGE_MAP(KonfiguracjaFFT, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_ROZDZIELCZOSC_FFT, &KonfiguracjaFFT::OnNMCustomdrawSlidRozdzielczoscFft)
	ON_BN_CLICKED(IDOK, &KonfiguracjaFFT::OnBnClickedOk)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_ZAKRES_WYSTEROWANIA, &KonfiguracjaFFT::OnNMCustomdrawSlidZakresWysterowania)
	ON_BN_CLICKED(IDC_CHECK_SILNIK1, &KonfiguracjaFFT::OnBnClickedCheckSilnik1)
	ON_BN_CLICKED(IDC_CHECK_SILNIK2, &KonfiguracjaFFT::OnBnClickedCheckSilnik2)
	ON_BN_CLICKED(IDC_CHECK_SILNIK3, &KonfiguracjaFFT::OnBnClickedCheckSilnik3)
	ON_BN_CLICKED(IDC_CHECK_SILNIK4, &KonfiguracjaFFT::OnBnClickedCheckSilnik4)
	ON_BN_CLICKED(IDC_CHECK_SILNIK5, &KonfiguracjaFFT::OnBnClickedCheckSilnik5)
	ON_BN_CLICKED(IDC_CHECK_SILNIK6, &KonfiguracjaFFT::OnBnClickedCheckSilnik6)
	ON_BN_CLICKED(IDC_BUT_POBIERZ_DANE, &KonfiguracjaFFT::OnBnClickedButPobierzDane)
	ON_BN_CLICKED(IDC_BUT_ROZPOCZNIJ_DIAGNOSTYKE, &KonfiguracjaFFT::OnBnClickedButRozpocznijDiagnostyke)
	ON_BN_CLICKED(IDC_BUT_ZATRZYMAJ, &KonfiguracjaFFT::OnBnClickedButZatrzymaj)
	ON_BN_CLICKED(IDC_BUT_ZAPISZ_KONFIG, &KonfiguracjaFFT::OnBnClickedButZapiszKonfig)
	ON_CBN_SELCHANGE(IDC_COMBO_RODZAJ_OKNA, &KonfiguracjaFFT::OnCbnSelchangeComboRodzajOkna)
END_MESSAGE_MAP()


// Procedury obsługi komunikatów KonfiguracjaFFT

///////////////////////////////////////////////////////////////////////////////////////////////////
// Inicjalizacja kontrolek okna
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL KonfiguracjaFFT::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	uint8_t chBłąd;

	chBłąd = getKomunikacja().CzytajParametryFFT(&m_chWykładnikPotęgi, &m_chRodzajOkna, &m_chAktywneSilniki, &m_chWysterowanieMaxProcent);
	if (chBłąd)
	{
		CString strKomunikat;
		strKomunikat.Format(_T("Nie można odczytać konfiguracji FFT! \nKod błędu: %d"), chBłąd);
		MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojojoj!"), MB_ICONEXCLAMATION, 0);
		CDialogEx::OnCancel();
		return FALSE;
	}

	m_ctlRozdzielczoscFFT.SetRangeMin(FFT_WYKLADNIK_MIN);
	m_ctlRozdzielczoscFFT.SetRangeMax(FFT_WYKLADNIK_MAX);
	m_ctlRozdzielczoscFFT.SetPos(m_chWykładnikPotęgi);

	m_ctlRodzajOkna.InsertString(0, _T("Prostokątne"));
	m_ctlRodzajOkna.InsertString(1, _T("Hamminga"));
	m_ctlRodzajOkna.InsertString(2, _T("Hana"));
	m_ctlRodzajOkna.InsertString(3, _T("Blackmana-Harrisa"));
	m_ctlRodzajOkna.SetCurSel(m_chRodzajOkna);

	m_ctlZakresWysterowania.SetRangeMin(0);
	m_ctlZakresWysterowania.SetRangeMax(100);
	m_ctlZakresWysterowania.SetPos((int)m_chWysterowanieMaxProcent);

	m_bSilnik1 = (m_chAktywneSilniki & 0x01) == 0x01;
	m_bSilnik2 = (m_chAktywneSilniki & 0x02) == 0x02;
	m_bSilnik3 = (m_chAktywneSilniki & 0x04) == 0x04;
	m_bSilnik4 = (m_chAktywneSilniki & 0x08) == 0x08;
	m_bSilnik5 = (m_chAktywneSilniki & 0x10) == 0x10;
	m_bSilnik6 = (m_chAktywneSilniki & 0x20) == 0x20;

	UpdateData(FALSE);
	m_bZmienionoDane = FALSE;
	GetDlgItem(IDC_BUT_ZAPISZ_KONFIG)->EnableWindow(m_bZmienionoDane);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // WYJĄTEK: Strona właściwości OCX powinna zwrócić FALSE
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na przycisk OK
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfiguracjaFFT::OnBnClickedOk()
{
	uint8_t chBłąd;

	UpdateData(TRUE);
	if (m_bZmienionoDane)
	{
		 m_chAktywneSilniki = m_bSilnik1 * 0x01 + m_bSilnik2 * 0x02 + m_bSilnik3 * 0x04 + m_bSilnik4 * 0x08 + m_bSilnik5 * 0x10 + m_bSilnik6 * 0x20;
		chBłąd = getKomunikacja().ZapiszParametryFFT(m_chWykładnikPotęgi, m_chRodzajOkna, m_chAktywneSilniki, m_chWysterowanieMaxProcent);
		if (chBłąd)
		{
			CString strKomunikat;
			strKomunikat.Format(_T("Nie można zapisać konfiguracji FFT! \nKod błędu: %d"), chBłąd);
			MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojojoj!"), MB_ICONEXCLAMATION, 0);
			CDialogEx::OnCancel();
			return;
		}
	}
	CDialogEx::OnOK();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmianę położenia slidera
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfiguracjaFFT::OnNMCustomdrawSlidRozdzielczoscFft(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	UpdateData();
	m_chWykładnikPotęgi = m_ctlRozdzielczoscFFT.GetPos();
	m_nRozmiarFFT = (int)pow(2.0f, m_chWykładnikPotęgi);
	m_strRozmiarFFT.Format(_T("Rozmiar FFT: %d"), m_nRozmiarFFT);
	GetDlgItem(IDC_BUT_ZAPISZ_KONFIG)->EnableWindow(m_bZmienionoDane);
	UpdateData(FALSE);
	m_bZmienionoDane = TRUE;
	*pResult = 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmianę slidera zakresy wysterowania0
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfiguracjaFFT::OnNMCustomdrawSlidZakresWysterowania(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	UpdateData(TRUE);
	m_chWysterowanieMaxProcent = (uint8_t)m_ctlZakresWysterowania.GetPos();
	m_strWysterowanie.Format(_T("Zakres wyster. silników: %d %%"), m_chWysterowanieMaxProcent);
	m_bZmienionoDane = TRUE;
	GetDlgItem(IDC_BUT_ZAPISZ_KONFIG)->EnableWindow(m_bZmienionoDane);
	*pResult = 0;
	UpdateData(FALSE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmianę checkoxów aktywacji silników w trakcie testu rezonansu
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfiguracjaFFT::OnBnClickedCheckSilnik1()
{
	UpdateData(TRUE);
	if (m_bSilnik1)
		m_chAktywneSilniki |= 0x01;
	else
		m_chAktywneSilniki &= ~0x01;
	m_bZmienionoDane = TRUE;
	GetDlgItem(IDC_BUT_ZAPISZ_KONFIG)->EnableWindow(m_bZmienionoDane);
}


void KonfiguracjaFFT::OnBnClickedCheckSilnik2()
{
	UpdateData(TRUE);
	if (m_bSilnik2)
		m_chAktywneSilniki |= 0x02;
	else
		m_chAktywneSilniki &= ~0x02;
	m_bZmienionoDane = TRUE;
	GetDlgItem(IDC_BUT_ZAPISZ_KONFIG)->EnableWindow(m_bZmienionoDane);
}


void KonfiguracjaFFT::OnBnClickedCheckSilnik3()
{
	UpdateData(TRUE);
	if (m_bSilnik3)
		m_chAktywneSilniki |= 0x04;
	else
		m_chAktywneSilniki &= ~0x04;
	m_bZmienionoDane = TRUE;
	GetDlgItem(IDC_BUT_ZAPISZ_KONFIG)->EnableWindow(m_bZmienionoDane);
}


void KonfiguracjaFFT::OnBnClickedCheckSilnik4()
{
	UpdateData(TRUE);
	if (m_bSilnik4)
		m_chAktywneSilniki |= 0x08;
	else
		m_chAktywneSilniki &= ~0x08;
	m_bZmienionoDane = TRUE;
	GetDlgItem(IDC_BUT_ZAPISZ_KONFIG)->EnableWindow(m_bZmienionoDane);
}


void KonfiguracjaFFT::OnBnClickedCheckSilnik5()
{
	UpdateData(TRUE);
	if (m_bSilnik5)
		m_chAktywneSilniki |= 0x10;
	else
		m_chAktywneSilniki &= ~0x10;
	m_bZmienionoDane = TRUE;
	GetDlgItem(IDC_BUT_ZAPISZ_KONFIG)->EnableWindow(m_bZmienionoDane);
}

void KonfiguracjaFFT::OnBnClickedCheckSilnik6()
{
	UpdateData(TRUE);
	if (m_bSilnik6)
		m_chAktywneSilniki |= 0x20;
	else
		m_chAktywneSilniki &= ~0x20;
	m_bZmienionoDane = TRUE;
	GetDlgItem(IDC_BUT_ZAPISZ_KONFIG)->EnableWindow(m_bZmienionoDane);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmianę typu okna
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfiguracjaFFT::OnCbnSelchangeComboRodzajOkna()
{
	UpdateData(TRUE);
	m_chRodzajOkna = (uint8_t)m_ctlRodzajOkna.GetCurSel();
	m_bZmienionoDane = TRUE;
	GetDlgItem(IDC_BUT_ZAPISZ_KONFIG)->EnableWindow(m_bZmienionoDane);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na naciśnięcie przycisku Pobierz dane
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfiguracjaFFT::OnBnClickedButPobierzDane()
{
	uint8_t chBłąd;
	float fDane[LICZB_FLOAT_WRAMCE];

	for (int nZmienna = 0; nZmienna < 6; nZmienna++)
	{
		for (int nPomiar = 0; nPomiar < LICZBA_TESTOW_FFT; nPomiar++)
		{
			for (int nRozmFFT = 0; nRozmFFT < m_nRozmiarFFT / 2 / LICZB_FLOAT_WRAMCE; nRozmFFT++)
			{
				for (int nIndeksWyniku = 0; nIndeksWyniku < LICZB_FLOAT_WRAMCE; nIndeksWyniku++)	//pobieraj po 32 liczby float jako mieszczące się w ramce i będące wielokrotniością minimalnego rozmiaru FFT
				{



					chBłąd = getKomunikacja().CzytajWynikiFFT((uint8_t)nZmienna, nIndeksWyniku * LICZB_FLOAT_WRAMCE, nPomiar, fDane, LICZB_FLOAT_WRAMCE);
				}
			}
		}
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na naciśnięcie przycisku Rozpocznij diagnostykę
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfiguracjaFFT::OnBnClickedButRozpocznijDiagnostyke()
{
	uint8_t chBłąd;

	chBłąd = getKomunikacja().RozpocznijDiagnostykeRezoanansu();
	if (chBłąd)
	{
		CString strKomunikat;
		strKomunikat.Format(_T("Nie można wysłać polecenia! \nKod błędu: %d"), chBłąd);
		MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojojoj!"), MB_ICONEXCLAMATION, 0);
		CDialogEx::OnCancel();
		return;
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na naciśnięcie przycisku Zatrzymaj silniki
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfiguracjaFFT::OnBnClickedButZatrzymaj()
{
	uint8_t chBłąd;

	chBłąd = getKomunikacja().ZatrzymajSilniki();
	if (chBłąd)
	{
		CString strKomunikat;
		strKomunikat.Format(_T("Nie można wysłać polecenia! \nKod błędu: %d"), chBłąd);
		MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojojoj!"), MB_ICONEXCLAMATION, 0);
		CDialogEx::OnCancel();
		return;
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na naciśnięcie przycisku Zapisz konfigurację
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfiguracjaFFT::OnBnClickedButZapiszKonfig()
{
	uint8_t chBłąd;

	UpdateData(TRUE);
	m_chAktywneSilniki = m_bSilnik1 * 0x01 + m_bSilnik2 * 0x02 + m_bSilnik3 * 0x04 + m_bSilnik4 * 0x08 + m_bSilnik5 * 0x10 + m_bSilnik6 * 0x20;
	chBłąd = getKomunikacja().ZapiszParametryFFT(m_chWykładnikPotęgi, m_chRodzajOkna, m_chAktywneSilniki, m_chWysterowanieMaxProcent);
	if (chBłąd)
	{
		CString strKomunikat;
		strKomunikat.Format(_T("Nie można zapisać konfiguracji FFT! \nKod błędu: %d"), chBłąd);
		MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojojoj!"), MB_ICONEXCLAMATION, 0);
		return;
	}
	else
	{
		m_bZmienionoDane = FALSE;
		GetDlgItem(IDC_BUT_ZAPISZ_KONFIG)->EnableWindow(m_bZmienionoDane);
	}		
}



