// NapedStrojenie.cpp: plik implementacji
//

#include "pch.h"
#include "APL-SN.h"
#include "NapedStrojenie.h"
#include "afxdialogex.h"
#include "Errors.h"
#include "sys_def_wspolny.h"
#include <math.h>

// Okno dialogowe NapedStrojenie

IMPLEMENT_DYNAMIC(NapedStrojenie, CDialogEx)

NapedStrojenie::NapedStrojenie(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NAPED_I_STROJENIE, pParent)
	, m_strObrotyMin(_T(""))
	, m_strObrotyMax(_T(""))
	, m_strObrotyZawis(_T(""))
	, m_strWysterowanieSilnika(_T(""))
	, m_strCzasIdentyfikacji(_T(""))
	, m_bBylaZmianaObrotow(FALSE)
	, m_bBylaZmianaIdentyfikacji(FALSE)
{

}
CString m_strZakresPrzechyleniaAkroStopnie;

NapedStrojenie::~NapedStrojenie()
{
}

void NapedStrojenie::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OBR_MIN, m_strObrotyMin);
	DDX_Text(pDX, IDC_EDIT_OBR_MAX, m_strObrotyMax);
	DDX_Text(pDX, IDC_EDIT_OBR_ZAWIS, m_strObrotyZawis);
	DDX_Control(pDX, IDC_SLID_WYSTEROWANIE_SILNIKA, m_ctlWysterownieSilnika);
	DDX_Control(pDX, IDC_SLID_CZAS_IDENTYFIKACJI, m_ctlCzasIdentyfikacji);
	DDX_Text(pDX, IDC_STATIC_WYSTEROWANIE_SILNIKA, m_strWysterowanieSilnika);
	DDX_Text(pDX, IDC_STATIC_CZAS__IDENTYFIKACJI, m_strCzasIdentyfikacji);
}



BEGIN_MESSAGE_MAP(NapedStrojenie, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_OBR_MIN, &NapedStrojenie::OnEnChangeEditObrMin)
	ON_EN_CHANGE(IDC_EDIT_OBR_ZAWIS, &NapedStrojenie::OnEnChangeEditObrZawis)
	ON_EN_CHANGE(IDC_EDIT_OBR_MAX, &NapedStrojenie::OnEnChangeEditObrMax)
	ON_BN_CLICKED(IDOK, &NapedStrojenie::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUT_ROZPOCZNIJ_IDENTYFIKACJE, &NapedStrojenie::OnBnClickedButRozpocznijIdentyfikacje)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_WYSTEROWANIE_SILNIKA, &NapedStrojenie::OnNMCustomdrawSlidWysterowanieSilnika)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_CZAS_IDENTYFIKACJI, &NapedStrojenie::OnNMCustomdrawSlidCzasIdentyfikacji)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLID_CZAS_IDENTYFIKACJI, &NapedStrojenie::OnReleasedcaptureSlidCzasIdentyfikacji)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLID_WYSTEROWANIE_SILNIKA, &NapedStrojenie::OnReleasedcaptureSlidWysterowanieSilnika)
END_MESSAGE_MAP()



///////////////////////////////////////////////////////////////////////////////////////////////////
// Inicjowanie zawartosci okna po uruchomieniu. Wczytuje nastawy z APL3
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL NapedStrojenie::OnInitDialog()
{
	uint8_t cDane[10];	//5 zmiennych 16 bitowych
	uint8_t cBłąd;

	CDialogEx::OnInitDialog();

	m_ctlWysterownieSilnika.SetRange(0, 20);
	m_ctlCzasIdentyfikacji.SetRange(0, 20);

	cBłąd = getKomunikacja().CzytajU8FRAM(cDane, 10, FAU_RC_WY_MIN);
	if (cBłąd == ERR_OK)
	{
		m_nObrotyMin = cDane[0] + cDane[1] * 0x100;
		m_strObrotyMin.Format(_T("%ld"), m_nObrotyMin);
		m_nObrotyMax = cDane[2] + cDane[3] * 0x100;
		m_strObrotyMax.Format(_T("%ld"), m_nObrotyMax);
		m_nObrotyZawis = cDane[4] + cDane[5] * 0x100;
		m_strObrotyZawis.Format(_T("%ld"), m_nObrotyZawis);

		m_nWysterowanieSilnika = cDane[6] + cDane[7] * 0x100;	//2U wysterowanie regulatorów podczas identyfikacji w jednostkach standardowych [0..1999]
		m_nCzasIdentyfikacji = cDane[8] + cDane[9] * 0x100;		//2U czas identyfikacji każdego silnika w milisekundach
		m_ctlWysterownieSilnika.SetPos(m_nWysterowanieSilnika / KROK_WYSTER_IDENTYFIKCJI);		
		m_ctlCzasIdentyfikacji.SetPos(m_nCzasIdentyfikacji / KROK_CZASU_IDENTYFIKCJI);
	}
	UpdateData(FALSE);
	m_bBylaZmianaIdentyfikacji = FALSE;
	m_bBylaZmianaObrotow = FALSE;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // WYJĄTEK: Strona właściwości OCX powinna zwrócić FALSE
}



void NapedStrojenie::OnEnChangeEditObrMin()
{
	UpdateData();
	m_nObrotyMin = (int)_wtoi(m_strObrotyMin);
	m_bBylaZmianaObrotow = TRUE;
	Invalidate();
}



void NapedStrojenie::OnEnChangeEditObrZawis()
{
	UpdateData();
	m_nObrotyZawis = (int)_wtoi(m_strObrotyZawis);
	m_bBylaZmianaObrotow = TRUE;
	Invalidate();
}



void NapedStrojenie::OnEnChangeEditObrMax()
{
	UpdateData();
	m_nObrotyMax = (int)_wtoi(m_strObrotyMax);
	m_bBylaZmianaObrotow = TRUE;
	Invalidate();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na przycisk OK. Jeżeli była zmiana konfiguracji to zapisuje zmiany 
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void NapedStrojenie::OnBnClickedOk()
{
	CString strKomunikat;
	uint8_t chErr;
	uint8_t cDane[4];

	if (m_bBylaZmianaObrotow)
	{
		chErr = getKomunikacja().ZapiszWysterowanieObrotow(m_nObrotyMin, m_nObrotyMax, m_nObrotyZawis);
		if (chErr != ERR_OK)
		{
			strKomunikat.Format(_T("Błąd zapisu konfiguracji wysterowania nr %d"), chErr);
			MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojoj!"), MB_ICONWARNING, 0);
		}
	}

	if (m_bBylaZmianaIdentyfikacji)
	{
		 getKomunikacja().m_unia8_32.dane16[0] = m_nWysterowanieSilnika;	//2U wysterowanie regulatorów podczas identyfikacji w jednostkach standardowych [0..1999]
		 getKomunikacja().m_unia8_32.dane16[1] = m_nCzasIdentyfikacji;		//2U czas identyfikacji każdego silnika w milisekundach

		 for (uint8_t n = 0; n < 4; n++)
			 cDane[n] = getKomunikacja().m_unia8_32.dane8[n];
		 chErr = getKomunikacja().ZapiszDaneU8FRAM(cDane, 4, FAU_RC_WY_IDENT);
	}
	
	CDialogEx::OnOK();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Zamienia string parametru na liczbę float
// Zwraca: liczbę float
///////////////////////////////////////////////////////////////////////////////////////////////////
float NapedStrojenie::ZamienStrNaFloat(CString strLiczba)
{
	strLiczba.Replace(_T(','), _T('.'));
	return (float)_wtof(strLiczba);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Obsługa przycisku rozpoczynającego identyfikację silników
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void NapedStrojenie::OnBnClickedButRozpocznijIdentyfikacje()
{
	CString strKomunikat;
	uint8_t chErr;

	chErr = getKomunikacja().RozpocznijIdentyfikacjeSilnikow(m_nWysterowanieSilnika, m_nCzasIdentyfikacji);
	if (chErr != ERR_OK)
	{
		strKomunikat.Format(_T("Błąd nr %d uruchomienia polecenia identyfikacji"), chErr);
		MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojoj!"), MB_ICONWARNING, 0);
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na potrzebę przerysowania suwaka definiującego wysterowanie silników podczas identyfikacji
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void NapedStrojenie::OnNMCustomdrawSlidWysterowanieSilnika(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	m_nWysterowanieSilnika = KROK_WYSTER_IDENTYFIKCJI * m_ctlWysterownieSilnika.GetPos();
	m_strWysterowanieSilnika.Format(_T("Wysterowanie silnika: %d "), m_nWysterowanieSilnika);
	UpdateData(FALSE);
	*pResult = 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmianę położenia suwaka definiującego czas trwania identyfikacji
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void NapedStrojenie::OnReleasedcaptureSlidWysterowanieSilnika(NMHDR* pNMHDR, LRESULT* pResult)
{
	UpdateData(TRUE);
	m_nWysterowanieSilnika = KROK_WYSTER_IDENTYFIKCJI * m_ctlWysterownieSilnika.GetPos();
	m_bBylaZmianaIdentyfikacji = TRUE;
	*pResult = 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na potrzebę przerysowania suwaka  definiującego czas trwania identyfikacji
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void NapedStrojenie::OnNMCustomdrawSlidCzasIdentyfikacji(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	m_nCzasIdentyfikacji = KROK_CZASU_IDENTYFIKCJI * m_ctlCzasIdentyfikacji.GetPos();
	m_strCzasIdentyfikacji.Format(_T("Czas identyfikacji: %d ms "), m_nCzasIdentyfikacji);
	UpdateData(FALSE);
	*pResult = 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmianę położenia suwaka definiującego czas trwania identyfikacji
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void NapedStrojenie::OnReleasedcaptureSlidCzasIdentyfikacji(NMHDR* pNMHDR, LRESULT* pResult)
{
	UpdateData(TRUE);
	m_nCzasIdentyfikacji = KROK_CZASU_IDENTYFIKCJI * m_ctlCzasIdentyfikacji.GetPos();
	m_bBylaZmianaIdentyfikacji = TRUE;
	*pResult = 0;
}

