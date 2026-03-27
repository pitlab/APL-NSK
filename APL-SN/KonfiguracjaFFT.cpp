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
	DDX_Text(pDX, IDC_STAT_ROZDZIELCZOSC_FFT, m_strRozdzielczośćFFT);
	DDX_Text(pDX, IDC_EDIT2, m_strWysterowanie);
}


BEGIN_MESSAGE_MAP(KonfiguracjaFFT, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_ROZDZIELCZOSC_FFT, &KonfiguracjaFFT::OnNMCustomdrawSlidRozdzielczoscFft)
	ON_EN_CHANGE(IDC_EDIT_WYSTEROWANIE, &KonfiguracjaFFT::OnEnChangeEditWysterowanie)
	ON_BN_CLICKED(IDOK, &KonfiguracjaFFT::OnBnClickedOk)
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


	chBłąd = getKomunikacja().CzytajParametryFFT(&m_chWykładnikPotęgi, &m_chRodzajOkna, &m_chAktywneSilniki, &m_sWysterowanie);
	if (chBłąd)
	{
		CString strKomunikat;
		strKomunikat.Format(_T("Nie można odczytać konfiguracji FFT! \nKod błędu: %d"), chBłąd);
		MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojojoj!"), MB_ICONEXCLAMATION, 0);
		CDialogEx::OnCancel();
		return FALSE;
	}

	m_ctlRozdzielczoscFFT.SetRangeMin(6);
	m_ctlRozdzielczoscFFT.SetRangeMax(14);
	m_ctlRozdzielczoscFFT.SetPos(m_chWykładnikPotęgi);
	m_strRozdzielczośćFFT.Format(_T("Rozdzielczość FFT = %0.f"), pow(2.0f, m_chWykładnikPotęgi));

	m_ctlRodzajOkna.InsertString(0, _T("Okno prostokątne"));
	m_ctlRodzajOkna.InsertString(1, _T("Okno Hamminga"));
	m_ctlRodzajOkna.InsertString(2, _T("Okno Hana"));
	m_ctlRodzajOkna.InsertString(3, _T("Okno Blackmana-Harrisa"));
	m_ctlRodzajOkna.SetCurSel(m_chRodzajOkna);

	m_strWysterowanie.Format(_T("%d"), m_sWysterowanie / (PPM_MAX - PPM_MIN));
	UpdateData(FALSE);
	m_bZmienionoDane = FALSE;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // WYJĄTEK: Strona właściwości OCX powinna zwrócić FALSE
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
	m_strRozdzielczośćFFT.Format(_T("Rozdzielczość FFT = %0.f"), pow(2.0f, m_chWykładnikPotęgi));
	UpdateData(FALSE);
	m_bZmienionoDane = TRUE;
	*pResult = 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zmianę zawartości edit boxa
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfiguracjaFFT::OnEnChangeEditWysterowanie()
{
	UpdateData(TRUE);
	m_strWysterowanie.Replace(_T(','), _T('.'));
	m_sWysterowanie = _wtoi(m_strWysterowanie);
	m_bZmienionoDane = TRUE;
	UpdateData(FALSE);
}




///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na przycisk OK
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfiguracjaFFT::OnBnClickedOk()
{
	uint8_t chBłąd;

	if (m_bZmienionoDane)
	{
		chBłąd = getKomunikacja().ZapiszParametryFFT(m_chWykładnikPotęgi, m_chRodzajOkna, m_chAktywneSilniki, m_sWysterowanie);
		if (chBłąd)
		{
			CString strKomunikat;
			strKomunikat.Format(_T("Nie można zapisać konfiguracji FFT! \nKod błędu: %d"), chBłąd);
			MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojojoj!"), MB_ICONEXCLAMATION, 0);
			CDialogEx::OnCancel();
			return FALSE;
		}
	}
	CDialogEx::OnOK();
}
