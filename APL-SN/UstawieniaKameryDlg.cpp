// UstawieniaKameryDlg.cpp: plik implementacji
//

#include "pch.h"
#include "APL-SN.h"
#include "UstawieniaKameryDlg.h"
#include "afxdialogex.h"
#include "Errors.h"
#include "komunikacja/Komunikacja.h"
#include "komunikacja/polecenia_komunikacyjne.h"

// Okno dialogowe UstawieniaKameryDlg

//IMPLEMENT_DYNAMIC(CUstawieniaKameryDlg, CDialogEx)
IMPLEMENT_DYNAMIC(CUstawieniaKameryDlg, CDialog)

CUstawieniaKameryDlg::CUstawieniaKameryDlg(CWnd* pParent /*=nullptr*/)
	//: CDialogEx(IDD_USTAWIENIA_KAMERY, pParent)
	: CDialog(IDD_USTAWIENIA_KAMERY, pParent)
	, m_nSzerokoscZdjecia(0)
	, m_nWysokoscZdjecia(0)
	, m_nPowiekszenieZdjecia(0)
	, m_StrSzerokosc(_T(""))
	, m_strWysokosc(_T(""))
	, m_bOdwrocPionowo(FALSE)
	, m_bOdwrocPoziomo(FALSE)
{
	
}

CUstawieniaKameryDlg::~CUstawieniaKameryDlg()
{
}

void CUstawieniaKameryDlg::DoDataExchange(CDataExchange* pDX)
{
	//	CDialogEx::DoDataExchange(pDX);
	CDialog::DoDataExchange(pDX);
	//DDX_Slider(pDX, IDC_SLID_SZER_ZDJECIA, m_nSzerokoscZdjecia);
	//DDX_Slider(pDX, IDC_SLID_WYS_ZDJECIA, m_nWysokoscZdjecia);
	//DDX_Slider(pDX, IDC_SLID_ZOOM_ZDJECIA2, m_nPowiekszenieZdjecia);

	DDX_Control(pDX, IDC_SLID_SZER_ZDJECIA, m_ctlSzerokoscZdjecia);
	DDX_Control(pDX, IDC_SLID_WYS_ZDJECIA, m_ctlWysokoscZdjecia);
	DDX_Control(pDX, IDC_SLID_ZOOM_ZDJECIA, m_ctlPowiekszenie);
	DDX_Control(pDX, IDC_COMBO_TRYB_DIAGNOSTYCZNY, m_ctlTrybDiagnostyczny);


	DDX_Text(pDX, IDC_STATIC_SZEROKOSC, m_StrSzerokosc);
	DDX_Text(pDX, IDC_STATIC_WYSOKOSC, m_strWysokosc);
	DDX_Check(pDX, IDC_CHECK_ODWR_PION, m_bOdwrocPionowo);
	DDX_Check(pDX, IDC_CHECK_ODWR_POZ, m_bOdwrocPoziomo);
	DDX_Control(pDX, IDC_SLID_PRZES_POZIOME, m_ctlPrzesunieciePoziome);
	DDX_Control(pDX, IDC_SLID_PRZES_PIONOWE, m_ctlPrzesunieciePionowe);
}


//BEGIN_MESSAGE_MAP(CUstawieniaKameryDlg, CDialogEx)
BEGIN_MESSAGE_MAP(CUstawieniaKameryDlg, CDialog)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_SZER_ZDJECIA, &CUstawieniaKameryDlg::OnNMCustomdrawSlidSzerZdjecia)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_WYS_ZDJECIA, &CUstawieniaKameryDlg::OnNMCustomdrawSlidWysZdjecia)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_ZOOM_ZDJECIA, &CUstawieniaKameryDlg::OnNMCustomdrawSlidZoomZdjecia)
	ON_BN_CLICKED(IDC_CHECK_ODWR_POZ, &CUstawieniaKameryDlg::OnBnClickedCheckOdwrPoz)
	ON_BN_CLICKED(IDC_CHECK_ODWR_PION, &CUstawieniaKameryDlg::OnBnClickedCheckOdwrPion)
	ON_CBN_SELCHANGE(IDC_COMBO_TRYB_DIAGNOSTYCZNY, &CUstawieniaKameryDlg::OnCbnSelchangeComboTrybDiagnostyczny)
	ON_BN_CLICKED(IDOK, &CUstawieniaKameryDlg::OnBnClickedOk)
	
END_MESSAGE_MAP()


// Procedury obsługi komunikatów UstawieniaKameryDlg
BOOL CUstawieniaKameryDlg::OnInitDialog()
{
	uint8_t chErr, chZoom;

	CDialog::OnInitDialog();

	chErr = getKomunikacja().PobierzKamere(&m_stKonfKamery);
	if (chErr == ERR_OK)
	{
		m_ctlSzerokoscZdjecia.SetRange(4, 2592 / KROK_ROZDZ_KAM, TRUE);
		m_ctlSzerokoscZdjecia.SetPos(m_stKonfKamery.chSzerWy);
		m_nSzerokoscZdjecia = m_stKonfKamery.chSzerWy * KROK_ROZDZ_KAM;

		m_ctlWysokoscZdjecia.SetRange(4, 1944 / KROK_ROZDZ_KAM, TRUE);
		m_ctlWysokoscZdjecia.SetPos(m_stKonfKamery.chWysWy);
		m_nWysokoscZdjecia = m_stKonfKamery.chWysWy * KROK_ROZDZ_KAM;

		uint8_t chZakresZoom = (MAX_SZER_KAM / KROK_ROZDZ_KAM) / m_stKonfKamery.chSzerWy;
		chZoom = m_stKonfKamery.chSzerWe / m_stKonfKamery.chSzerWy;
		m_ctlPowiekszenie.SetRange(1, chZakresZoom, FALSE);
		m_ctlPowiekszenie.SetPos(chZoom);

		m_bOdwrocPionowo = m_stKonfKamery.chObracanieObrazu & 0x40;
		m_bOdwrocPoziomo = m_stKonfKamery.chObracanieObrazu & 0x20;


		m_ctlTrybDiagnostyczny.AddString(L"Rzeczywisty obraz");
		m_ctlTrybDiagnostyczny.AddString(L"Krata C/B");
		m_ctlTrybDiagnostyczny.AddString(L"Kolorowe Pasy 1");
		m_ctlTrybDiagnostyczny.AddString(L"Kolorowe Pasy 2");
		m_ctlTrybDiagnostyczny.SetCurSel(m_stKonfKamery.chTrybDiagn);

		m_StrSzerokosc.Format(L"Szerokość zdjęcia %d [pix]", m_nSzerokoscZdjecia);
		m_strWysokosc.Format(L"Wysokość zdjęcia %d [pix]", m_nWysokoscZdjecia);
	}
	return TRUE;  
}



void CUstawieniaKameryDlg::OnNMCustomdrawSlidSzerZdjecia(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	m_nSzerokoscZdjecia = m_ctlSzerokoscZdjecia.GetPos() * KROK_ROZDZ_KAM;
	m_StrSzerokosc.Format(L"Szerokość zdjęcia %d [pix]", m_nSzerokoscZdjecia);
	m_stKonfKamery.chSzerWy = (uint8_t)m_ctlSzerokoscZdjecia.GetPos();
	m_stKonfKamery.chSzerWe = (uint8_t)m_ctlSzerokoscZdjecia.GetPos() * m_ctlPowiekszenie.GetPos();
	if (m_stKonfKamery.chSzerWe > MAX_SZER_KAM / KROK_ROZDZ_KAM)
		m_stKonfKamery.chSzerWe = MAX_SZER_KAM / KROK_ROZDZ_KAM;
	UpdateData(FALSE);
	WyslijDoKamery();
	*pResult = 0;
}



void CUstawieniaKameryDlg::OnNMCustomdrawSlidWysZdjecia(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	m_nWysokoscZdjecia = m_ctlWysokoscZdjecia.GetPos() * KROK_ROZDZ_KAM;
	m_strWysokosc.Format(L"Wysokość zdjęcia %d [pix]", m_nWysokoscZdjecia);
	m_stKonfKamery.chWysWy = (uint8_t)m_ctlWysokoscZdjecia.GetPos();
	m_stKonfKamery.chWysWe = (uint8_t)m_ctlWysokoscZdjecia.GetPos() * m_ctlPowiekszenie.GetPos();
	if (m_stKonfKamery.chWysWe > MAX_WYS_KAM / KROK_ROZDZ_KAM)
		m_stKonfKamery.chWysWe = MAX_WYS_KAM / KROK_ROZDZ_KAM;
	UpdateData(FALSE);
	WyslijDoKamery();
	*pResult = 0;
}



void CUstawieniaKameryDlg::OnNMCustomdrawSlidZoomZdjecia(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
	UpdateData(TRUE);
	m_nPowiekszenieZdjecia = m_ctlPowiekszenie.GetPos();
	m_strPowiekszenie.Format(L"Powiekszenie zdjęcia %d ", m_nPowiekszenieZdjecia);
	
	m_stKonfKamery.chSzerWe = (uint8_t)m_ctlSzerokoscZdjecia.GetPos() * m_ctlPowiekszenie.GetPos();
	if (m_stKonfKamery.chSzerWe > MAX_SZER_KAM / KROK_ROZDZ_KAM)
		m_stKonfKamery.chSzerWe = MAX_SZER_KAM / KROK_ROZDZ_KAM;

	m_stKonfKamery.chWysWe = (uint8_t)m_ctlWysokoscZdjecia.GetPos() * m_ctlPowiekszenie.GetPos();
	if (m_stKonfKamery.chWysWe > MAX_WYS_KAM / KROK_ROZDZ_KAM)
		m_stKonfKamery.chWysWe = MAX_WYS_KAM / KROK_ROZDZ_KAM;

	UpdateData(FALSE);
	WyslijDoKamery();
	*pResult = 0;
}



void CUstawieniaKameryDlg::OnBnClickedCheckOdwrPoz()
{
	UpdateData(TRUE);
	m_stKonfKamery.chObracanieObrazu &= ~0x40;	//maska bitu 6 Mirror On/Off
	m_stKonfKamery.chObracanieObrazu |= m_bOdwrocPoziomo * 0x40;
	WyslijDoKamery();
}



void CUstawieniaKameryDlg::OnBnClickedCheckOdwrPion()
{
	UpdateData(TRUE);
	m_stKonfKamery.chObracanieObrazu &= ~0x20;	//maska bitu 5 Vertical Flip On/Off
	m_stKonfKamery.chObracanieObrazu |= m_bOdwrocPionowo * 0x20;
	WyslijDoKamery();
}



void CUstawieniaKameryDlg::OnCbnSelchangeComboTrybDiagnostyczny()
{
	UpdateData(TRUE);
	m_stKonfKamery.chTrybDiagn = m_ctlTrybDiagnostyczny.GetCurSel();	//Tryby Diagnostyczne Kamery
	WyslijDoKamery();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wysyła ramkę z bieżącymi nastawami do kamery
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
int CUstawieniaKameryDlg::WyslijDoKamery()
{
	return getKomunikacja().UstawKamere(&m_stKonfKamery);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zamknięcie okna przyciskiem OK
// Wysyła polecenie zapisu konfiguracji kamery
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUstawieniaKameryDlg::OnBnClickedOk()
{
	WyslijDoKamery();
	CDialog::OnOK();
}




