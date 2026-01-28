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
	, m_strObrotyJalowe(_T(""))
	, m_strObrotyMin(_T(""))
	, m_strObrotyMax(_T(""))
	, m_strObrotyZawis(_T(""))
{

}
CString m_strZakresPrzechyleniaAkroStopnie;

NapedStrojenie::~NapedStrojenie()
{
}

void NapedStrojenie::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OBR_JALOWE, m_strObrotyJalowe);
	DDX_Text(pDX, IDC_EDIT_OBR_MIN, m_strObrotyMin);
	DDX_Text(pDX, IDC_EDIT_OBR_ZAWIS, m_strObrotyMax);
	DDX_Text(pDX, IDC_EDIT_OBR_MAX, m_strObrotyMax);
	DDX_Text(pDX, IDC_EDIT_OBR_ZAWIS, m_strObrotyZawis);
}


BEGIN_MESSAGE_MAP(NapedStrojenie, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_OBR_JALOWE, &NapedStrojenie::OnEnChangeEditObrJalowe)
	ON_EN_CHANGE(IDC_EDIT_OBR_MIN, &NapedStrojenie::OnEnChangeEditObrMin)
	ON_EN_CHANGE(IDC_EDIT_OBR_ZAWIS, &NapedStrojenie::OnEnChangeEditObrZawis)
	ON_EN_CHANGE(IDC_EDIT_OBR_MAX, &NapedStrojenie::OnEnChangeEditObrMax)
	ON_BN_CLICKED(IDOK, &NapedStrojenie::OnBnClickedOk)
END_MESSAGE_MAP()




///////////////////////////////////////////////////////////////////////////////////////////////////
// Inicjowanie zawartosci okna po uruchomieniu. Wczytuje nastawy z APL3
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL NapedStrojenie::OnInitDialog()
{
	float fDane[ROZMIAR_DRAZKOW];
	uint8_t chDane[2*ROZMIAR_PWM];
	uint8_t chErr;

	CDialogEx::OnInitDialog();
	m_bBylaZmianaObrotow = FALSE;


	chErr = getKomunikacja().CzytajU8FRAM(chDane, 2*ROZMIAR_PWM, FAU_PWM_JALOWY);
	if (chErr == ERR_OK)
	{
		m_nObrotyJalowe = chDane[0] + chDane[1] * 0x100;
		m_strObrotyJalowe.Format(_T("%d"), m_nObrotyJalowe);
		m_nObrotyMin = chDane[2] + chDane[3] * 0x100;
		m_strObrotyMin.Format(_T("%d"), m_nObrotyMin);
		m_nObrotyZawis = chDane[4] + chDane[5] * 0x100;
		m_strObrotyZawis.Format(_T("%d"), m_nObrotyZawis);
		m_nObrotyMax = chDane[6] + chDane[7] * 0x100;
		m_strObrotyMax.Format(_T("%d"), m_nObrotyMax);
	}
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // WYJĄTEK: Strona właściwości OCX powinna zwrócić FALSE
}





void NapedStrojenie::OnEnChangeEditObrJalowe()
{
	UpdateData();
	m_nObrotyJalowe = (int)_wtoi(m_strObrotyJalowe);
	m_bBylaZmianaObrotow = TRUE;
	Invalidate();
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


	if (m_bBylaZmianaObrotow)
	{
		chErr = getKomunikacja().ZapiszWysterowanieObrotow(m_nObrotyJalowe, m_nObrotyMin, m_nObrotyZawis, m_nObrotyMax);
		if (chErr != ERR_OK)
		{
			strKomunikat.Format(_T("Błąd zapisu konfiguracji wysterowania nr %d"), chErr);
			MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojoj!"), MB_ICONWARNING, 0);
		}
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

