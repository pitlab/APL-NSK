// NapedStrojenie.cpp: plik implementacji
//

#include "pch.h"
#include "APL-SN.h"
#include "NapedStrojenie.h"
#include "afxdialogex.h"
#include "Errors.h"

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

	chErr = getKomunikacja().CzytajFloatFRAM(fDane, ROZMIAR_DRAZKOW, FAU_ZAKR_DRAZKOW_AKRO);
	if (chErr == ERR_OK)
	{
		m_fZakresPrzechyleniaAkro = fDane[0];
		m_strZakresPrzechyleniaAkro.Format(_T("%.2f"), m_fZakresPrzechyleniaAkro);
		m_fZakresPochyleniaAkro = fDane[1]; 
		m_strZakresPochyleniaAkro.Format(_T("%.2f"), m_fZakresPochyleniaAkro);
		m_fZakresOdchyleniaAkro = fDane[2];
		m_strZakresOdchyleniaAkro.Format(_T("%.2f"), m_fZakresOdchyleniaAkro);
		m_fZakresWysokosciAkro = fDane[3];
		m_strZakresWysokosciAkro.Format(_T("%.2f"), m_fZakresWysokosciAkro);
	}

	chErr = getKomunikacja().CzytajFloatFRAM(fDane, ROZMIAR_DRAZKOW, FAU_ZAKR_DRAZKOW_STAB);
	if (chErr == ERR_OK)
	{
		m_fZakresPrzechyleniaStab = fDane[0];
		m_strZakresPrzechyleniaStab.Format(_T("%.2f"), m_fZakresPrzechyleniaStab);
		m_fZakresPochyleniaStab = fDane[1];
		m_strZakresPochyleniaStab.Format(_T("%.2f"), m_fZakresPochyleniaStab);
		m_fZakresOdchyleniaStab = fDane[2];
		m_strZakresOdchyleniaStab.Format(_T("%.2f"), m_fZakresOdchyleniaStab);
		m_fZakresWysokosciStab = fDane[3];
		m_strZakresWysokosciStab.Format(_T("%.2f"), m_fZakresWysokosciStab);
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
	m_fZakresPrzechyleniaAkro = (float)_wtof(m_strZakresPrzechyleniaAkro);
	m_bBylaZmianaWychyleniaDrazkowAkro = TRUE;
	Invalidate();
}



void NapedStrojenie::OnEnChangeEditPochylenieAkro()
{
	UpdateData();
	m_fZakresPochyleniaAkro = (float)_wtof(m_strZakresPochyleniaAkro);
	m_bBylaZmianaWychyleniaDrazkowAkro = TRUE;
	Invalidate();
}


void NapedStrojenie::OnEnChangeEditOdchylenieAkro()
{
	UpdateData();
	m_fZakresOdchyleniaAkro = (float)_wtof(m_strZakresOdchyleniaAkro);
	m_bBylaZmianaWychyleniaDrazkowAkro = TRUE;
	Invalidate();
}


void NapedStrojenie::OnEnChangeEditWysokoscAkro()
{
	UpdateData();
	m_fZakresWysokosciAkro = (float)_wtof(m_strZakresWysokosciAkro);
	m_bBylaZmianaWychyleniaDrazkowAkro = TRUE;
	Invalidate();
}

void NapedStrojenie::OnEnChangeEditPrzechylenieStab()
{
	UpdateData();
	m_fZakresPrzechyleniaStab = (float)_wtof(m_strZakresPrzechyleniaStab);
	m_bBylaZmianaWychyleniaDrazkowStab = TRUE;
	Invalidate();
}


void NapedStrojenie::OnEnChangeEditPochylenieStab()
{
	UpdateData();
	m_fZakresPochyleniaStab = (float)_wtof(m_strZakresPochyleniaStab);
	m_bBylaZmianaWychyleniaDrazkowStab = TRUE;
	Invalidate();
}


void NapedStrojenie::OnEnChangeEditOdchylenieStab()
{
	UpdateData();
	m_fZakresOdchyleniaStab = (float)_wtof(m_strZakresOdchyleniaStab);
	m_bBylaZmianaWychyleniaDrazkowStab = TRUE;
	Invalidate();
}


void NapedStrojenie::OnEnChangeEditWysokoscStab()
{
	UpdateData();
	m_fZakresWysokosciStab = (float)_wtof(m_strZakresWysokosciStab);
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
	if (m_bBylaZmianaWychyleniaDrazkowAkro)
	{

	}

	if (m_bBylaZmianaWychyleniaDrazkowAkro)
	{

	}

	if (m_bBylaZmianaObrotow)
	{

	}
	CDialogEx::OnOK();
}





