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
	, m_strZakresPrzechyleniaAkro(_T(""))
	, m_strZakresPochyleniaAkro(_T(""))
	, m_strZakresOdchyleniaAkro(_T(""))
	, m_strZakresWysokosciAkro(_T(""))
	, m_strObrotyJalowe(_T(""))
	, m_strObrotyMin(_T(""))
	, m_strObrotyMax(_T(""))
	, m_strObrotyZawis(_T(""))
	, m_strZakresPrzechyleniaStab(_T(""))
	, m_strZakresPochyleniaStab(_T(""))
	, m_strZakresOdchyleniaStab(_T(""))
	, m_strZakresWysokosciStab(_T(""))
{

}

NapedStrojenie::~NapedStrojenie()
{
}

void NapedStrojenie::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PRZECHYLENIE_AKRO, m_strZakresPrzechyleniaAkro);
	DDX_Text(pDX, IDC_EDIT_POCHYLENIE_AKRO, m_strZakresPochyleniaAkro);
	DDX_Text(pDX, IDC_EDIT_ODCHYLENIE_AKRO, m_strZakresOdchyleniaAkro);
	DDX_Text(pDX, IDC_EDIT_WYSOKOSC_AKRO, m_strZakresWysokosciAkro);
	DDX_Text(pDX, IDC_EDIT_OBR_JALOWE, m_strObrotyJalowe);
	DDX_Text(pDX, IDC_EDIT_OBR_MIN, m_strObrotyMin);
	DDX_Text(pDX, IDC_EDIT_OBR_ZAWIS, m_strObrotyMax);
	DDX_Text(pDX, IDC_EDIT_OBR_MAX, m_strObrotyMax);
	DDX_Text(pDX, IDC_EDIT_OBR_ZAWIS, m_strObrotyZawis);
	DDX_Text(pDX, IDC_EDIT_PRZECHYLENIE_STAB, m_strZakresPrzechyleniaStab);
	DDX_Text(pDX, IDC_EDIT_POCHYLENIE_STAB, m_strZakresPochyleniaStab);
	DDX_Text(pDX, IDC_EDIT_ODCHYLENIE_STAB, m_strZakresOdchyleniaStab);
	DDX_Text(pDX, IDC_EDIT_WYSOKOSC_STAB, m_strZakresWysokosciStab);
}


BEGIN_MESSAGE_MAP(NapedStrojenie, CDialogEx)

	ON_EN_CHANGE(IDC_EDIT_PRZECHYLENIE_AKRO, &NapedStrojenie::OnEnChangeEditPrzechylenieAkro)
	ON_EN_CHANGE(IDC_EDIT_POCHYLENIE_AKRO, &NapedStrojenie::OnEnChangeEditPochylenieAkro)
	ON_EN_CHANGE(IDC_EDIT_ODCHYLENIE_AKRO, &NapedStrojenie::OnEnChangeEditOdchylenieAkro)
	ON_EN_CHANGE(IDC_EDIT_WYSOKOSC_AKRO, &NapedStrojenie::OnEnChangeEditWysokoscAkro)
	ON_EN_CHANGE(IDC_EDIT_OBR_JALOWE, &NapedStrojenie::OnEnChangeEditObrJalowe)
	ON_EN_CHANGE(IDC_EDIT_OBR_MIN, &NapedStrojenie::OnEnChangeEditObrMin)
	ON_EN_CHANGE(IDC_EDIT_OBR_ZAWIS, &NapedStrojenie::OnEnChangeEditObrZawis)
	ON_EN_CHANGE(IDC_EDIT_OBR_MAX, &NapedStrojenie::OnEnChangeEditObrMax)
	ON_BN_CLICKED(IDOK, &NapedStrojenie::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_PRZECHYLENIE_STAB, &NapedStrojenie::OnEnChangeEditPrzechylenieStab)
	ON_EN_CHANGE(IDC_EDIT_POCHYLENIE_STAB, &NapedStrojenie::OnEnChangeEditPochylenieStab)
	ON_EN_CHANGE(IDC_EDIT_ODCHYLENIE_STAB, &NapedStrojenie::OnEnChangeEditOdchylenieStab)
	ON_EN_CHANGE(IDC_EDIT_WYSOKOSC_STAB, &NapedStrojenie::OnEnChangeEditWysokoscStab)
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
	m_bBylaZmianaWychyleniaDrazkowAkro = FALSE;
	m_bBylaZmianaObrotow = FALSE;


	//4x4F wartość zadana z drążków aparatury dla regulatora Akro 
	chErr = getKomunikacja().CzytajFloatFRAM(fDane, ROZMIAR_DRAZKOW, FAU_ZADANA_AKRO);
	if (chErr == ERR_OK)
	{
		m_fSkalaWartosciZadanejAkro[PRZE] = RAD2DEG * fDane[0];
		m_strZakresPrzechyleniaAkro.Format(_T("%.2f"), m_fSkalaWartosciZadanejAkro[PRZE]);
		m_fSkalaWartosciZadanejAkro[POCH] = RAD2DEG * fDane[1];
		m_strZakresPochyleniaAkro.Format(_T("%.2f"), m_fSkalaWartosciZadanejAkro[POCH]);
		m_fSkalaWartosciZadanejAkro[ODCH] = RAD2DEG * fDane[2];
		m_strZakresOdchyleniaAkro.Format(_T("%.2f"), m_fSkalaWartosciZadanejAkro[ODCH]);
		m_fSkalaWartosciZadanejAkro[WYSO] = fDane[3];
		m_strZakresWysokosciAkro.Format(_T("%.2f"), m_fSkalaWartosciZadanejAkro[WYSO]);
	}
	
	//4x4F wartość zadana z drążków aparatury dla regulatora Stab
	chErr = getKomunikacja().CzytajFloatFRAM(fDane, ROZMIAR_DRAZKOW, FAU_ZADANA_STAB);
	if (chErr == ERR_OK)
	{
		m_fSkalaWartosciZadanejStab[PRZE] = RAD2DEG * fDane[0];
		m_strZakresPrzechyleniaStab.Format(_T("%.2f"), m_fSkalaWartosciZadanejStab[PRZE]);
		m_fSkalaWartosciZadanejStab[POCH] = RAD2DEG * fDane[1];
		m_strZakresPochyleniaStab.Format(_T("%.2f"), m_fSkalaWartosciZadanejStab[POCH]);
		m_fSkalaWartosciZadanejStab[ODCH] = RAD2DEG * fDane[2];
		m_strZakresOdchyleniaStab.Format(_T("%.2f"), m_fSkalaWartosciZadanejStab[ODCH]);
		m_fSkalaWartosciZadanejStab[WYSO] = fDane[3];
		m_strZakresWysokosciStab.Format(_T("%.2f"), m_fSkalaWartosciZadanejStab[WYSO]);
	}

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



void NapedStrojenie::OnEnChangeEditPrzechylenieAkro()
{
	UpdateData();
	m_fSkalaWartosciZadanejAkro[PRZE] = DEG2RAD * ZamienStrNaFloat(m_strZakresPrzechyleniaAkro);
	m_bBylaZmianaWychyleniaDrazkowAkro = TRUE;
	Invalidate();
}



void NapedStrojenie::OnEnChangeEditPochylenieAkro()
{
	UpdateData();
	m_fSkalaWartosciZadanejAkro[POCH] = DEG2RAD * ZamienStrNaFloat(m_strZakresPochyleniaAkro);
	m_bBylaZmianaWychyleniaDrazkowAkro = TRUE;
	Invalidate();
}


void NapedStrojenie::OnEnChangeEditOdchylenieAkro()
{
	UpdateData();
	m_fSkalaWartosciZadanejAkro[ODCH] = DEG2RAD * ZamienStrNaFloat(m_strZakresOdchyleniaAkro);
	m_bBylaZmianaWychyleniaDrazkowAkro = TRUE;
	Invalidate();
}


void NapedStrojenie::OnEnChangeEditWysokoscAkro()
{
	UpdateData();
	m_fSkalaWartosciZadanejAkro[WYSO] = ZamienStrNaFloat(m_strZakresWysokosciAkro);
	m_bBylaZmianaWychyleniaDrazkowAkro = TRUE;
	Invalidate();
}

void NapedStrojenie::OnEnChangeEditPrzechylenieStab()
{
	UpdateData();
	m_fSkalaWartosciZadanejStab[PRZE] = DEG2RAD * ZamienStrNaFloat(m_strZakresPrzechyleniaStab);
	m_bBylaZmianaWychyleniaDrazkowStab = TRUE;
	Invalidate();
}


void NapedStrojenie::OnEnChangeEditPochylenieStab()
{
	UpdateData();
	m_fSkalaWartosciZadanejStab[POCH] = DEG2RAD * ZamienStrNaFloat(m_strZakresPochyleniaStab);
	m_bBylaZmianaWychyleniaDrazkowStab = TRUE;
	Invalidate();
}


void NapedStrojenie::OnEnChangeEditOdchylenieStab()
{
	UpdateData();
	m_fSkalaWartosciZadanejStab[ODCH] = DEG2RAD * ZamienStrNaFloat(m_strZakresOdchyleniaStab);
	m_bBylaZmianaWychyleniaDrazkowStab = TRUE;
	Invalidate();
}


void NapedStrojenie::OnEnChangeEditWysokoscStab()
{
	UpdateData();
	m_fSkalaWartosciZadanejStab[WYSO] = ZamienStrNaFloat(m_strZakresWysokosciStab);
	m_bBylaZmianaWychyleniaDrazkowStab = TRUE;
	Invalidate();
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

	if (m_bBylaZmianaWychyleniaDrazkowAkro)
	{
		chErr = getKomunikacja().ZapiszSkaleWartosciZadanejAkro(m_fSkalaWartosciZadanejAkro);
		if (chErr != ERR_OK)
		{
			strKomunikat.Format(_T("Błąd zapisu konfiguracji Akro nr %d"), chErr);
			MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojoj!"), MB_ICONWARNING, 0);
		}
	}

	if (m_bBylaZmianaWychyleniaDrazkowAkro)
	{
		chErr = getKomunikacja().ZapiszSkaleWartosciZadanejStab(m_fSkalaWartosciZadanejStab);
		if (chErr != ERR_OK)
		{
			strKomunikat.Format(_T("Błąd zapisu konfiguracji Stab nr %d"), chErr);
			MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojoj!"), MB_ICONWARNING, 0);
		}
	}

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

