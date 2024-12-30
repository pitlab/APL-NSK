// UstawieniaKameryDlg.cpp: plik implementacji
//

#include "pch.h"
#include "APL-SN.h"
#include "UstawieniaKameryDlg.h"
#include "afxdialogex.h"
#include "Errors.h"
#include "Komunikacja.h"

// Okno dialogowe UstawieniaKameryDlg

//IMPLEMENT_DYNAMIC(CUstawieniaKameryDlg, CDialogEx)
IMPLEMENT_DYNAMIC(CUstawieniaKameryDlg, CDialog)

CUstawieniaKameryDlg::CUstawieniaKameryDlg(CWnd* pParent /*=nullptr*/)
	//: CDialogEx(IDD_USTAWIENIA_KAMERY, pParent)
	: CDialog(IDD_USTAWIENIA_KAMERY, pParent)
	, m_nSzerokoscZdjecia(0)
	, m_nWysokoscZdjecia(0)
	, m_nPowiekszenieZdjecia(0)
	, m_bOdwrocPionowo(FALSE)
	, m_StrSzerokosc(_T(""))
	, m_strWysokosc(_T(""))
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
	DDX_Control(pDX, IDC_SLID_ZOOM_ZDJECIA2, m_ctlPowiekszenie);

	DDX_Control(pDX, IDC_CHECK_ODWR_POZ, m_ctlOdwrocPoziomo);
	DDX_Control(pDX, IDC_CHECK_ODWR_PION, m_ctlOdwrocPionowo);
	DDX_Control(pDX, IDC_COMBO_TRYB_DIAGNOSTYCZNY, m_ctlTrybDiagnostyczny);


	DDX_Text(pDX, IDC_STATIC_SZEROKOSC, m_StrSzerokosc);
	DDX_Text(pDX, IDC_STATIC_WYSOKOSC, m_strWysokosc);
}


//BEGIN_MESSAGE_MAP(CUstawieniaKameryDlg, CDialogEx)
BEGIN_MESSAGE_MAP(CUstawieniaKameryDlg, CDialog)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_SZER_ZDJECIA, &CUstawieniaKameryDlg::OnNMCustomdrawSlidSzerZdjecia)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_WYS_ZDJECIA, &CUstawieniaKameryDlg::OnNMCustomdrawSlidWysZdjecia)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_ZOOM_ZDJECIA2, &CUstawieniaKameryDlg::OnNMCustomdrawSlidZoomZdjecia2)
	ON_BN_CLICKED(IDC_CHECK_ODWR_POZ, &CUstawieniaKameryDlg::OnBnClickedCheckOdwrPoz)
	ON_BN_CLICKED(IDC_CHECK_ODWR_PION, &CUstawieniaKameryDlg::OnBnClickedCheckOdwrPion)
	ON_CBN_SELCHANGE(IDC_COMBO_TRYB_DIAGNOSTYCZNY, &CUstawieniaKameryDlg::OnCbnSelchangeComboTrybDiagnostyczny)
	ON_BN_CLICKED(IDOK, &CUstawieniaKameryDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// Procedury obsługi komunikatów UstawieniaKameryDlg
BOOL CUstawieniaKameryDlg::OnInitDialog()
{
	uint8_t chErr, chSzer, chWys, chZoom, chSzerZoom, chWysZoom, chTrybDiagn, chFlagi;

	//CDialogEx::OnInitDialog();
	CDialog::OnInitDialog();

	chSzer = 320 / SKALA_ROZDZ_KAM;
	chWys = 240 / SKALA_ROZDZ_KAM;
	chSzerZoom = 640 / SKALA_ROZDZ_KAM;
	chWysZoom = 480 / SKALA_ROZDZ_KAM;
	chTrybDiagn = 0;
	chFlagi = 0;
	chErr = 0;
	//chErr = getKomunikacja().PobierzKamere(2, &chSzer, &chWys, &chSzerZoom, &chWysZoom, &chTrybDiagn, &chFlagi);
	if (chErr == ERR_OK)
	{
		m_ctlSzerokoscZdjecia.SetRange(4, 2592 / SKALA_ROZDZ_KAM, TRUE);
		m_ctlSzerokoscZdjecia.SetPos(chSzer);
		m_nSzerokoscZdjecia = chSzer * SKALA_ROZDZ_KAM;

		m_ctlWysokoscZdjecia.SetRange(4, 1944 / SKALA_ROZDZ_KAM, TRUE);
		m_ctlWysokoscZdjecia.SetPos(chWys);
		m_nWysokoscZdjecia = chWys * SKALA_ROZDZ_KAM;

		uint8_t chZakresZoom = (MAX_SZER_KAM / SKALA_ROZDZ_KAM) / chSzer;
		chZoom = chSzerZoom / chSzer;
		m_ctlPowiekszenie.SetRange(1, chZakresZoom, FALSE);
		m_ctlPowiekszenie.SetPos(chZoom);

		//0 - odwróć w poziomie, 1 - odwróć w pionie,
		m_ctlOdwrocPoziomo.SetCheck(chFlagi & 0x01);
		m_ctlOdwrocPionowo.SetCheck(chFlagi & 0x02);

		m_ctlTrybDiagnostyczny.AddString(L"Rzeczywisty obraz");
		m_ctlTrybDiagnostyczny.AddString(L"Krata C/B");
		m_ctlTrybDiagnostyczny.AddString(L"Kolorowe Pasy 1");
		m_ctlTrybDiagnostyczny.AddString(L"Kolorowe Pasy 2");
		m_ctlTrybDiagnostyczny.SetCurSel(chTrybDiagn);

		m_StrSzerokosc.Format(L"Szerokość zdjęcia %d [pix]", m_nSzerokoscZdjecia);
		m_strWysokosc.Format(L"Wysokość zdjęcia %d [pix]", m_nWysokoscZdjecia);
	}
	return TRUE;  
}



void CUstawieniaKameryDlg::OnNMCustomdrawSlidSzerZdjecia(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
	m_nSzerokoscZdjecia = m_ctlSzerokoscZdjecia.GetPos() * SKALA_ROZDZ_KAM;
	m_StrSzerokosc.Format(L"Szerokość zdjęcia %d [pix]", m_nSzerokoscZdjecia);
	UpdateData(FALSE);
	*pResult = 0;
}


void CUstawieniaKameryDlg::OnNMCustomdrawSlidWysZdjecia(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
	m_nWysokoscZdjecia = m_ctlWysokoscZdjecia.GetPos() * SKALA_ROZDZ_KAM;
	m_strWysokosc.Format(L"Wysokość zdjęcia %d [pix]", m_nWysokoscZdjecia);
	UpdateData(FALSE);
	*pResult = 0;
}


void CUstawieniaKameryDlg::OnNMCustomdrawSlidZoomZdjecia2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
	*pResult = 0;
}


void CUstawieniaKameryDlg::OnBnClickedCheckOdwrPoz()
{
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
}


void CUstawieniaKameryDlg::OnBnClickedCheckOdwrPion()
{
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
}


void CUstawieniaKameryDlg::OnCbnSelchangeComboTrybDiagnostyczny()
{
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zamknięcie okna przyciskiem OK
// Wysyła polecenie zapisu konfiguracji kamery
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUstawieniaKameryDlg::OnBnClickedOk()
{
	uint8_t chSzer, chWys, chZoom, chSzerZoom, chWysZoom, chTrybDiagn, chFlagi;

	UpdateData(TRUE);
	chSzer = (uint8_t)m_ctlSzerokoscZdjecia.GetPos();
	chWys = (uint8_t)m_ctlWysokoscZdjecia.GetPos();
	chZoom = (uint8_t)m_ctlPowiekszenie.GetPos();

	if (chSzer * chZoom <= MAX_SZER_KAM / SKALA_ROZDZ_KAM)
		chSzerZoom = chSzer * chZoom;
	else
		chSzerZoom = MAX_SZER_KAM / SKALA_ROZDZ_KAM;

	if (chWys * chZoom <= MAX_WYS_KAM / SKALA_ROZDZ_KAM)
		chWysZoom = chSzer * chZoom;
	else
		chWysZoom = MAX_WYS_KAM / SKALA_ROZDZ_KAM;

	chTrybDiagn = (uint8_t)m_ctlTrybDiagnostyczny.GetCurSel();

	//0 - odwróć w poziomie, 1 - odwróć w pionie,
	chFlagi = 0x01 * m_ctlOdwrocPoziomo.GetState() + 0x02 * m_ctlOdwrocPionowo.GetState();
		
	//getKomunikacja().UstawKamere(2, chSzer, chWys, chSzerZoom, chWysZoom, chTrybDiagn, chFlagi);
	//CDialogEx::OnOK();
	CDialog::OnOK();
}


