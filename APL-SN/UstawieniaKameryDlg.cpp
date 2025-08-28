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
	, m_strPrzesunieciePionowe(_T(""))
	, m_strPrzesunieciePoziome(_T(""))
	, m_bRecznyBalansBieli(FALSE)
	, m_strBalansBieliCzerwona(_T(""))
	, m_strBalansBieliZielona(_T(""))
	, m_strBalansBieliNiebieska(_T(""))
	, m_strCzasEkspozycji(_T(""))
	, m_strGornaGranicaEkspozycji(_T(""))
	, m_strDolnaGranicaEkspozycji(_T(""))
	, m_bKorekcjaLENC(FALSE)
	, m_bGammaYUV(FALSE)
	, m_bGammaSurowa(FALSE)
	, m_bUsuwaUsrednianie(FALSE)
	, m_bOdszumianie(FALSE)
	, m_bUsuwanieCzarnychPikseli(FALSE)
	, m_bUsuwanieBialychPikseli(FALSE)
	, m_bInterpolacja(FALSE)
	, m_bCyfroweEfektySpecjalne(FALSE)
	, m_bUstawienieChrominancji(FALSE)
	, m_bSkalowaniePionowe(FALSE)
	, m_bSkalowaniePoziome(FALSE)
	, m_bRozciaganieLinii(FALSE)
	, m_bUsrednianieChrominancji(FALSE)
	, m_bMacierzKolorow(FALSE)
	, m_bAutomatycznyBalansBieli(FALSE)
	, m_bRecznyCzasEkspozycji(FALSE)
	, m_strProgUsuwaniaSredniejKolumnParzystych(_T(""))
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
	DDX_Text(pDX, IDC_STATIC_SZEROKOSC, m_StrSzerokosc);
	DDX_Text(pDX, IDC_STATIC_WYSOKOSC, m_strWysokosc);
	DDX_Check(pDX, IDC_CHECK_ODWR_PION, m_bOdwrocPionowo);
	DDX_Check(pDX, IDC_CHECK_ODWR_POZ, m_bOdwrocPoziomo);
	DDX_Control(pDX, IDC_SLID_PRZES_POZIOME, m_ctlPrzesunieciePoziome);
	DDX_Control(pDX, IDC_SLID_PRZES_PIONOWE, m_ctlPrzesunieciePionowe);
	DDX_Text(pDX, IDC_STATIC_PRZES_POZOME, m_strPrzesunieciePionowe);
	DDX_Text(pDX, IDC_STATIC_PRZES_PIONOWE, m_strPrzesunieciePoziome);
	DDX_Check(pDX, IDC_CHECK_BALANS_RECZNY, m_bRecznyBalansBieli);
	DDX_Control(pDX, IDC_SLID_AWB_CZERWONY, m_ctlBalansBieli_Czerwony);
	DDX_Control(pDX, IDC_SLID_AWB_ZIELONY, m_ctlBalansBieli_Zielony);
	DDX_Control(pDX, IDC_SLID_AWB_NIEBIESKI, m_ctlBalansBieli_Niebieski);
	DDX_Text(pDX, IDC_STATIC_AWB_CZERWONA, m_strBalansBieliCzerwona);
	DDX_Text(pDX, IDC_STATIC_AWB_ZIELONA, m_strBalansBieliZielona);
	DDX_Text(pDX, IDC_STATIC_AWB_NIEBIESKA, m_strBalansBieliNiebieska);
	DDX_Control(pDX, IDC_SLID_AEC_CZAS, m_ctlCzasEkspozycji);
	DDX_Text(pDX, IDC_STATIC_CZAS_EKSPOZYCJI, m_strCzasEkspozycji);
	DDX_Control(pDX, IDC_SLID_AEC_GORNA_GRANICA, m_ctlGornaGranicaEkspozycji);
	DDX_Control(pDX, IDC_SLID_AEC_DOLNA_GRANICA, m_ctlDolnaGranicaEkspozycji);
	DDX_Text(pDX, IDC_STATIC_GORNA_GRANICA_EKSPOZYCJI, m_strGornaGranicaEkspozycji);
	DDX_Text(pDX, IDC_STATIC_DOLNA_GRANICA_EKSPOZYCJI, m_strDolnaGranicaEkspozycji);
	DDX_Check(pDX, IDC_CHECK_KOREKCJA_LENC, m_bKorekcjaLENC);
	DDX_Check(pDX, IDC_CHECK_GAMA_YUV, m_bGammaYUV);
	DDX_Check(pDX, IDC_CHECK_GAMMA_SUROWA, m_bGammaSurowa);
	DDX_Check(pDX, IDC_CHECK_ISP0_USUWA_USRED, m_bUsuwaUsrednianie);
	DDX_Control(pDX, IDC_SLID_PROG_USREDNIANIA_KOLUMN, m_ctlProgUsuwaniaUsrednianiaParzystychKolumn);
	DDX_Check(pDX, IDC_CHECK_ISP0_ODSZUMIANIE, m_bOdszumianie);
	DDX_Check(pDX, IDC_CHECK_ISP0_USUWANIE_CZARNYCH, m_bUsuwanieCzarnychPikseli);
	DDX_Check(pDX, IDC_CHECK_ISP0_USUWANIE_BIALYCH, m_bUsuwanieBialychPikseli);
	DDX_Check(pDX, IDC_CHECK_ISP0_INTERPOLACJA, m_bInterpolacja);
	DDX_Check(pDX, IDC_CHECK_ISP1_CYFROWE_EFEKTY_SPECJALNE, m_bCyfroweEfektySpecjalne);
	DDX_Check(pDX, IDC_CHECK_ISP1_USTAWIENIE_CHROMINANCJI, m_bUstawienieChrominancji);
	DDX_Check(pDX, IDC_CHECK_ISP1_SKALOWANIE_PIONOWE, m_bSkalowaniePionowe);
	DDX_Check(pDX, IDC_CHECK_ISP1_SKALOWANIE_POZIOME, m_bSkalowaniePoziome);
	DDX_Check(pDX, IDC_CHECK_ISP1_ROZCIAGANIE_LINI, m_bRozciaganieLinii);
	DDX_Check(pDX, IDC_CHECK_ISP1_USREDNIANIE_CHROMINANCJI, m_bUsrednianieChrominancji);
	DDX_Check(pDX, IDC_CHECK_ISP1_MACIERZ_KOLOROW, m_bMacierzKolorow);
	DDX_Check(pDX, IDC_CHECK_ISP1_AUTO_BALANS_BIELI, m_bAutomatycznyBalansBieli);
	DDX_Check(pDX, IDC_CHECK_EKSPOZYCJA_RECZNA, m_bRecznyCzasEkspozycji);
	DDX_Text(pDX, IDC_STATIC_PROG_USUWANIA, m_strProgUsuwaniaSredniejKolumnParzystych);
}


//BEGIN_MESSAGE_MAP(CUstawieniaKameryDlg, CDialogEx)
BEGIN_MESSAGE_MAP(CUstawieniaKameryDlg, CDialog)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_SZER_ZDJECIA, &CUstawieniaKameryDlg::OnNMCustomdrawSlidSzerZdjecia)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_WYS_ZDJECIA, &CUstawieniaKameryDlg::OnNMCustomdrawSlidWysZdjecia)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_ZOOM_ZDJECIA, &CUstawieniaKameryDlg::OnNMCustomdrawSlidZoomZdjecia)
	ON_BN_CLICKED(IDC_CHECK_ODWR_POZ, &CUstawieniaKameryDlg::OnBnClickedCheckOdwrPoz)
	ON_BN_CLICKED(IDC_CHECK_ODWR_PION, &CUstawieniaKameryDlg::OnBnClickedCheckOdwrPion)
	ON_BN_CLICKED(IDOK, &CUstawieniaKameryDlg::OnBnClickedOk)
	
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_PRZES_POZIOME, &CUstawieniaKameryDlg::OnNMCustomdrawSlidPrzesPoziome)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_PRZES_PIONOWE, &CUstawieniaKameryDlg::OnNMCustomdrawSlidPrzesPionowe)
	ON_BN_CLICKED(IDC_CHECK_BALANS_RECZNY, &CUstawieniaKameryDlg::OnBnClickedCheckRecznyBalansBieli)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_AWB_CZERWONY, &CUstawieniaKameryDlg::OnNMCustomdrawSliderBalansBieli_Czerwony)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_AWB_ZIELONY, &CUstawieniaKameryDlg::OnNMCustomdrawSliderBalansBieli_Zielony)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_AWB_NIEBIESKI, &CUstawieniaKameryDlg::OnNMCustomdrawSliderBalansBieli_Niebieski)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_AEC_CZAS, &CUstawieniaKameryDlg::OnNMCustomdrawSlidAecCzas)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_AEC_GORNA_GRANICA, &CUstawieniaKameryDlg::OnNMCustomdrawSlidAecGornaGranica)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_AEC_DOLNA_GRANICA, &CUstawieniaKameryDlg::OnNMCustomdrawSlidAecDolnaGranica)
	ON_BN_CLICKED(IDC_CHECK_KOREKCJA_LENC, &CUstawieniaKameryDlg::OnBnClickedCheckKorekcjaLenc)
	ON_BN_CLICKED(IDC_CHECK_GAMA_YUV, &CUstawieniaKameryDlg::OnBnClickedCheckGamaYuv)
	ON_BN_CLICKED(IDC_CHECK_GAMMA_SUROWA, &CUstawieniaKameryDlg::OnBnClickedCheckGammaSurowa)
	ON_BN_CLICKED(IDC_CHECK_ISP0_USUWA_USRED, &CUstawieniaKameryDlg::OnBnClickedCheckIsp0UsuwaUsred)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_PROG_USREDNIANIA_KOLUMN, &CUstawieniaKameryDlg::OnNMCustomdrawSlider1ProgUsrednianiaKolumn)
	ON_BN_CLICKED(IDC_CHECK_ISP0_ODSZUMIANIE, &CUstawieniaKameryDlg::OnBnClickedCheckOdszumianie)
	ON_BN_CLICKED(IDC_CHECK_ISP0_USUWANIE_CZARNYCH, &CUstawieniaKameryDlg::OnBnClickedCheckIsp0UsuwanieCzarnych)
	ON_BN_CLICKED(IDC_CHECK_ISP0_USUWANIE_BIALYCH, &CUstawieniaKameryDlg::OnBnClickedCheckIsp0UsuwanieBialych)
	ON_BN_CLICKED(IDC_CHECK_ISP0_INTERPOLACJA, &CUstawieniaKameryDlg::OnBnClickedCheckIsp0Interpolacja)
	ON_BN_CLICKED(IDC_CHECK_ISP1_CYFROWE_EFEKTY_SPECJALNE, &CUstawieniaKameryDlg::OnBnClickedCheckIsp1CyfroweEfektySpecjalne)
	ON_BN_CLICKED(IDC_CHECK_ISP1_USTAWIENIE_CHROMINANCJI, &CUstawieniaKameryDlg::OnBnClickedCheckIsp1UstawienieChrominancji)
	ON_BN_CLICKED(IDC_CHECK_ISP1_SKALOWANIE_PIONOWE, &CUstawieniaKameryDlg::OnBnClickedCheckIsp1SkalowaniePionowe)
	ON_BN_CLICKED(IDC_CHECK_ISP1_SKALOWANIE_POZIOME, &CUstawieniaKameryDlg::OnBnClickedCheckIsp1SkalowaniePoziome)
	ON_BN_CLICKED(IDC_CHECK_ISP1_ROZCIAGANIE_LINI, &CUstawieniaKameryDlg::OnBnClickedCheckIsp1RozciaganieLini)
	ON_BN_CLICKED(IDC_CHECK_ISP1_USREDNIANIE_CHROMINANCJI, &CUstawieniaKameryDlg::OnBnClickedCheckIsp1UsrednianieChrominancji)
	ON_BN_CLICKED(IDC_CHECK_ISP1_MACIERZ_KOLOROW, &CUstawieniaKameryDlg::OnBnClickedCheckIsp1MacierzKolorow)
	ON_BN_CLICKED(IDC_CHECK_ISP1_AUTO_BALANS_BIELI, &CUstawieniaKameryDlg::OnBnClickedCheck1Isp1AutoBalansBieli)
	ON_BN_CLICKED(IDC_CHECK_EKSPOZYCJA_RECZNA, &CUstawieniaKameryDlg::OnBnClickedCheckEkspozycjaReczna)
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
		m_StrSzerokosc.Format(L"Szerokość zdjęcia %d [pix]", m_nSzerokoscZdjecia);

		m_ctlWysokoscZdjecia.SetRange(4, 1944 / KROK_ROZDZ_KAM, TRUE);
		m_ctlWysokoscZdjecia.SetPos(m_stKonfKamery.chWysWy);
		m_nWysokoscZdjecia = m_stKonfKamery.chWysWy * KROK_ROZDZ_KAM;
		m_strWysokosc.Format(L"Wysokość zdjęcia %d [pix]", m_nWysokoscZdjecia);

		m_ctlPrzesunieciePoziome.SetRange(0, m_stKonfKamery.chSzerWe - m_stKonfKamery.chSzerWy);
		m_ctlPrzesunieciePoziome.SetPos(m_stKonfKamery.chPrzesWyPoz);
		m_strPrzesunieciePoziome.Format(L"Przesuniecie w poziomie %d", m_stKonfKamery.chPrzesWyPoz * KROK_ROZDZ_KAM);

		m_ctlPrzesunieciePionowe.SetRange(0, m_stKonfKamery.chWysWe - m_stKonfKamery.chWysWy);
		m_ctlPrzesunieciePionowe.SetPos(m_stKonfKamery.chPrzesWyPio);
		m_strPrzesunieciePionowe.Format(L"Przesuniecie w pionie %d", m_stKonfKamery.chPrzesWyPio * KROK_ROZDZ_KAM);

		//balans ręczny jest w zakresie 0..1 a konkretnie 0/0x400..0x400/0x400 
		m_ctlBalansBieli_Czerwony.SetRange(0, 0x400);
		m_ctlBalansBieli_Zielony.SetRange(0, 0x400);
		m_ctlBalansBieli_Niebieski.SetRange(0, 0x400);
		m_ctlBalansBieli_Czerwony.SetPos(m_stKonfKamery.sWzmocnienieR);
		m_ctlBalansBieli_Zielony.SetPos(m_stKonfKamery.sWzmocnienieG);
		m_ctlBalansBieli_Niebieski.SetPos(m_stKonfKamery.sWzmocnienieB);

		//włącz lub wyłącz kontrlki w zależności od stanu bitu je waczajacego
		m_bRecznyCzasEkspozycji = m_stKonfKamery.chKontrolaExpo & 0x01;
		GetDlgItem(IDC_SLID_AEC_CZAS)->EnableWindow(m_bRecznyCzasEkspozycji);
		GetDlgItem(IDC_SLID_AEC_GORNA_GRANICA)->EnableWindow(m_bRecznyCzasEkspozycji);
		GetDlgItem(IDC_SLID_AEC_DOLNA_GRANICA)->EnableWindow(m_bRecznyCzasEkspozycji);
		GetDlgItem(IDC_STATIC_CZAS_EKSPOZYCJI)->EnableWindow(m_bRecznyCzasEkspozycji);
		GetDlgItem(IDC_STATIC_GORNA_GRANICA_EKSPOZYCJI)->EnableWindow(m_bRecznyCzasEkspozycji);
		GetDlgItem(IDC_STATIC_DOLNA_GRANICA_EKSPOZYCJI)->EnableWindow(m_bRecznyCzasEkspozycji);

		m_bRecznyBalansBieli = m_stKonfKamery.chKontrolaBalansuBieli & 0x01;
		GetDlgItem(IDC_SLID_AWB_CZERWONY)->EnableWindow(m_bRecznyBalansBieli);
		GetDlgItem(IDC_SLID_AWB_ZIELONY)->EnableWindow(m_bRecznyBalansBieli);
		GetDlgItem(IDC_SLID_AWB_NIEBIESKI)->EnableWindow(m_bRecznyBalansBieli);
		GetDlgItem(IDC_STATIC_AWB_CZERWONA)->EnableWindow(m_bRecznyBalansBieli);
		GetDlgItem(IDC_STATIC_AWB_ZIELONA)->EnableWindow(m_bRecznyBalansBieli);
		GetDlgItem(IDC_STATIC_AWB_NIEBIESKA)->EnableWindow(m_bRecznyBalansBieli);

		m_bUsuwaUsrednianie = m_stKonfKamery.chKontrolaISP0 & 0x10;
		GetDlgItem(IDC_SLID_PROG_USREDNIANIA_KOLUMN)->EnableWindow(m_bUsuwaUsrednianie);
		GetDlgItem(IDC_STATIC_PROG_USUWANIA)->EnableWindow(m_bUsuwaUsrednianie);

		uint8_t chZakresZoom = (MAX_SZER_KAM / KROK_ROZDZ_KAM) / m_stKonfKamery.chSzerWy;
		chZoom = m_stKonfKamery.chSzerWe / m_stKonfKamery.chSzerWy;
		m_ctlPowiekszenie.SetRange(1, chZakresZoom, FALSE);
		m_ctlPowiekszenie.SetPos(chZoom);

		m_bOdwrocPionowo = m_stKonfKamery.chObracanieObrazu & 0x40;
		m_bOdwrocPoziomo = m_stKonfKamery.chObracanieObrazu & 0x20;	

		m_bKorekcjaLENC = m_stKonfKamery.chKontrolaISP0 & 0x80;
		m_bGammaYUV = m_stKonfKamery.chKontrolaISP0 & 0x40;
		m_bGammaSurowa = m_stKonfKamery.chKontrolaISP0 & 0x20;
		m_bUsuwaUsrednianie = m_stKonfKamery.chKontrolaISP0 & 0x10;
		m_bOdszumianie = m_stKonfKamery.chKontrolaISP0 & 0x08;
		m_bUsuwanieCzarnychPikseli = m_stKonfKamery.chKontrolaISP0 & 0x04;
		m_bUsuwanieBialychPikseli = m_stKonfKamery.chKontrolaISP0 & 0x02;
		m_bInterpolacja = m_stKonfKamery.chKontrolaISP0 & 0x01;
		m_bCyfroweEfektySpecjalne = m_stKonfKamery.chKontrolaISP1 & 0x80;
		m_bUstawienieChrominancji = m_stKonfKamery.chKontrolaISP1 & 0x40;
		m_bSkalowaniePionowe = m_stKonfKamery.chKontrolaISP1 & 0x20;
		m_bSkalowaniePoziome = m_stKonfKamery.chKontrolaISP1 & 0x10;
		m_bRozciaganieLinii = m_stKonfKamery.chKontrolaISP1 & 0x08;
		m_bUsrednianieChrominancji = m_stKonfKamery.chKontrolaISP1 & 0x04;
		m_bMacierzKolorow = m_stKonfKamery.chKontrolaISP1 & 0x02;
		m_bAutomatycznyBalansBieli = m_stKonfKamery.chKontrolaISP1 & 0x01;		
	}
	return TRUE;  
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Obsługa zmiany położenia suwaka szerokości obrazu
// parametry: ?
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
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



///////////////////////////////////////////////////////////////////////////////////////////////////
// Obsługa zmiany położenia suwaka wysokości obrazu
// parametry: ?
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
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



///////////////////////////////////////////////////////////////////////////////////////////////////
// Obsługa zmiany położenia suwaka pomnijeszenia obrazu
// parametry: ?
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUstawieniaKameryDlg::OnNMCustomdrawSlidZoomZdjecia(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
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



///////////////////////////////////////////////////////////////////////////////////////////////////
// Obsługa zmiany położenia suwaka przesunięcia obrazu w poziomie względem początku matrycy
// parametry: ?
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUstawieniaKameryDlg::OnNMCustomdrawSlidPrzesPoziome(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	m_stKonfKamery.chPrzesWyPoz = m_ctlPrzesunieciePoziome.GetPos();
	UpdateData(FALSE);
	WyslijDoKamery();
	*pResult = 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Obsługa zmiany położenia suwaka przesunięcia obrazu w pionie względem początku matrycy
// parametry: ?
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUstawieniaKameryDlg::OnNMCustomdrawSlidPrzesPionowe(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	m_stKonfKamery.chPrzesWyPio = m_ctlPrzesunieciePionowe.GetPos();
	UpdateData(FALSE);
	WyslijDoKamery();
	*pResult = 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Obsługa zmiany stanu chechkoxa odpowiedzialnego za odbicie lustrzane obrazu
// parametry: nic
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUstawieniaKameryDlg::OnBnClickedCheckOdwrPoz()
{
	UpdateData(TRUE);
	m_stKonfKamery.chObracanieObrazu &= ~0x40;	//maska bitu 6 Mirror On/Off
	m_stKonfKamery.chObracanieObrazu |= m_bOdwrocPoziomo * 0x40;
	WyslijDoKamery();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Obsługa zmiany stanu chechkoxa odpowiedzialnego za odwrócenie obrazu góra-dół
// parametry: nic
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUstawieniaKameryDlg::OnBnClickedCheckOdwrPion()
{
	UpdateData(TRUE);
	m_stKonfKamery.chObracanieObrazu &= ~0x20;	//maska bitu 5 Vertical Flip On/Off
	m_stKonfKamery.chObracanieObrazu |= m_bOdwrocPionowo * 0x20;
	WyslijDoKamery();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na kliknięcie checkbox Ręczny balans Bieli
// Ustawia bit 0 w rejestrze 0x3406, konf->chKontrolaBalansuBieli
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUstawieniaKameryDlg::OnBnClickedCheckRecznyBalansBieli()
{
	UpdateData(TRUE);
	m_stKonfKamery.chKontrolaBalansuBieli = 0x01 * m_bRecznyBalansBieli;
	WyslijDoKamery();
	GetDlgItem(IDC_SLID_AWB_CZERWONY)->EnableWindow(m_bRecznyBalansBieli);
	GetDlgItem(IDC_SLID_AWB_ZIELONY)->EnableWindow(m_bRecznyBalansBieli);
	GetDlgItem(IDC_SLID_AWB_NIEBIESKI)->EnableWindow(m_bRecznyBalansBieli);
	GetDlgItem(IDC_STATIC_AWB_CZERWONA)->EnableWindow(m_bRecznyBalansBieli);
	GetDlgItem(IDC_STATIC_AWB_ZIELONA)->EnableWindow(m_bRecznyBalansBieli);
	GetDlgItem(IDC_STATIC_AWB_NIEBIESKA)->EnableWindow(m_bRecznyBalansBieli);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wysyła ramkę z bieżącymi nastawami do kamery
// // parametry: nic
// zwraca: kod błędu
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



///////////////////////////////////////////////////////////////////////////////////////////////////
// Obsługa zmiany położenia suwaka wzmocnienia składowej czerwonej przy balansie ręcznym
// parametry: ?
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUstawieniaKameryDlg::OnNMCustomdrawSliderBalansBieli_Czerwony(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	int nBalans = m_ctlBalansBieli_Czerwony.GetPos();
	m_stKonfKamery.sWzmocnienieR = (uint16_t)(nBalans & 0xFFF);	
	m_strBalansBieliCzerwona.Format(L"Składowa czerwona: %d", nBalans);
	UpdateData(FALSE);
	WyslijDoKamery();
	*pResult = 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Obsługa zmiany położenia suwaka wzmocnienia składowej zielonej przy balansie ręcznym
// parametry: ?
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUstawieniaKameryDlg::OnNMCustomdrawSliderBalansBieli_Zielony(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	int nBalans = m_ctlBalansBieli_Zielony.GetPos();
	m_stKonfKamery.sWzmocnienieG = (uint16_t)(nBalans & 0xFFF);
	m_strBalansBieliZielona.Format(L"Składowa zielona: %d", nBalans);
	UpdateData(FALSE);
	WyslijDoKamery();
	*pResult = 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Obsługa zmiany położenia suwaka wzmocnienia składowej niebieskiej przy balansie ręcznym
// parametry: ?
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUstawieniaKameryDlg::OnNMCustomdrawSliderBalansBieli_Niebieski(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	int nBalans = m_ctlBalansBieli_Niebieski.GetPos();
	m_stKonfKamery.sWzmocnienieB = (uint16_t)(nBalans & 0xFFF);
	m_strBalansBieliNiebieska.Format(L"Składowa niebieska: %d", nBalans);
	UpdateData(FALSE);
	WyslijDoKamery();
	*pResult = 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na kliknięcie checkbox Ręczny czas ekspozycji
// Ustawia bit 0 w rejestrze 0x3503, konf->chKontrolaExpo
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUstawieniaKameryDlg::OnBnClickedCheckEkspozycjaReczna()
{
	UpdateData(TRUE);
	m_stKonfKamery.chKontrolaExpo = 0x01 * m_bRecznyCzasEkspozycji;
	WyslijDoKamery();
	GetDlgItem(IDC_SLID_AEC_CZAS)->EnableWindow(m_bRecznyCzasEkspozycji);
	GetDlgItem(IDC_SLID_AEC_GORNA_GRANICA)->EnableWindow(m_bRecznyCzasEkspozycji);
	GetDlgItem(IDC_SLID_AEC_DOLNA_GRANICA)->EnableWindow(m_bRecznyCzasEkspozycji);
	GetDlgItem(IDC_STATIC_CZAS_EKSPOZYCJI)->EnableWindow(m_bRecznyCzasEkspozycji);
	GetDlgItem(IDC_STATIC_GORNA_GRANICA_EKSPOZYCJI)->EnableWindow(m_bRecznyCzasEkspozycji);
	GetDlgItem(IDC_STATIC_DOLNA_GRANICA_EKSPOZYCJI)->EnableWindow(m_bRecznyCzasEkspozycji);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Obsługa zmiany położenia suwaka ręcznego czasu ekspozycji
// parametry: ?
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUstawieniaKameryDlg::OnNMCustomdrawSlidAecCzas(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	int nCzasEkspozycji = m_ctlCzasEkspozycji.GetPos();
	m_stKonfKamery.nEkspozycjaReczna = nCzasEkspozycji & 0xFFFFF;
	m_strCzasEkspozycji.Format(L"Czas ekspozycji: %d", nCzasEkspozycji);
	UpdateData(FALSE);
	WyslijDoKamery();	
	*pResult = 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Obsługa zmiany położenia suwaka górnej granicy czasu ekspozycji
// parametry: ?
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUstawieniaKameryDlg::OnNMCustomdrawSlidAecGornaGranica(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	int nGranicaEkspozycji = m_ctlGornaGranicaEkspozycji.GetPos();
	m_stKonfKamery.nEkspozycjaReczna = nGranicaEkspozycji & 0xFFFFF;
	m_strGornaGranicaEkspozycji.Format(L"Górna granica czasu ekspozycji: %d", nGranicaEkspozycji);
	UpdateData(FALSE);
	WyslijDoKamery();
	*pResult = 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Obsługa zmiany położenia suwaka dolnej granicy czasu ekspozycji
// parametry: ?
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUstawieniaKameryDlg::OnNMCustomdrawSlidAecDolnaGranica(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	int nGranicaEkspozycji = m_ctlDolnaGranicaEkspozycji.GetPos();
	m_stKonfKamery.nEkspozycjaReczna = nGranicaEkspozycji & 0xFFFFF;
	m_strDolnaGranicaEkspozycji.Format(L"Dolna granica czasu ekspozycji: %d", nGranicaEkspozycji);
	UpdateData(FALSE);
	WyslijDoKamery();
	*pResult = 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Obsługa zmiany bitów zmiennej ISP_CONTROL 00
// parametry: brak
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUstawieniaKameryDlg::OnBnClickedCheckKorekcjaLenc()
{
	UpdateData(TRUE);
	m_stKonfKamery.chKontrolaISP0 &= ~0x80;
	m_stKonfKamery.chKontrolaISP0 |= m_bKorekcjaLENC * 0x80;
	WyslijDoKamery();	
}


void CUstawieniaKameryDlg::OnBnClickedCheckGamaYuv()
{
	UpdateData(TRUE);
	m_stKonfKamery.chKontrolaISP0 &= ~0x40;
	m_stKonfKamery.chKontrolaISP0 |= m_bGammaYUV * 0x40;
	WyslijDoKamery();
}


void CUstawieniaKameryDlg::OnBnClickedCheckGammaSurowa()
{
	UpdateData(TRUE);
	m_stKonfKamery.chKontrolaISP0 &= ~0x20;
	m_stKonfKamery.chKontrolaISP0 |= m_bGammaSurowa * 0x20;
	WyslijDoKamery();
}


void CUstawieniaKameryDlg::OnBnClickedCheckIsp0UsuwaUsred()
{
	UpdateData(TRUE);
	m_stKonfKamery.chKontrolaISP0 &= ~0x10;
	m_stKonfKamery.chKontrolaISP0 |= m_bUsuwaUsrednianie * 0x10;
	WyslijDoKamery();
	GetDlgItem(IDC_SLIDER1_PROG_USREDNIANIA_KOLUMN)->EnableWindow(m_bUsuwaUsrednianie);
	GetDlgItem(IDC_STATIC_PROG_USUWANIA)->EnableWindow(m_bUsuwaUsrednianie);
}


void CUstawieniaKameryDlg::OnBnClickedCheckOdszumianie()
{
	UpdateData(TRUE);
	m_stKonfKamery.chKontrolaISP0 &= ~0x08;
	m_stKonfKamery.chKontrolaISP0 |= m_bOdszumianie * 0x08;
	WyslijDoKamery();
}


void CUstawieniaKameryDlg::OnBnClickedCheckIsp0UsuwanieCzarnych()
{
	UpdateData(TRUE);
	m_stKonfKamery.chKontrolaISP0 &= ~0x04;
	m_stKonfKamery.chKontrolaISP0 |= m_bUsuwanieCzarnychPikseli * 0x04;
	WyslijDoKamery();
}


void CUstawieniaKameryDlg::OnBnClickedCheckIsp0UsuwanieBialych()
{
	UpdateData(TRUE);
	m_stKonfKamery.chKontrolaISP0 &= ~0x02;
	m_stKonfKamery.chKontrolaISP0 |= m_bUsuwanieBialychPikseli * 0x02;
	WyslijDoKamery(); 
}


void CUstawieniaKameryDlg::OnBnClickedCheckIsp0Interpolacja()
{
	UpdateData(TRUE);
	m_stKonfKamery.chKontrolaISP0 &= ~0x01;
	m_stKonfKamery.chKontrolaISP0 |= m_bInterpolacja * 0x01;
	WyslijDoKamery(); 
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Obsługa zmiany położenia suwaka progu usuwanie uśredniania między parzystymi i nieparzystymi kolumnami
// parametry: ?
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUstawieniaKameryDlg::OnNMCustomdrawSlider1ProgUsrednianiaKolumn(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	int nProg = m_ctlProgUsuwaniaUsrednianiaParzystychKolumn.GetPos();
	m_stKonfKamery.chProgUsuwania = (uint8_t)(nProg & 0xFF);
	m_strProgUsuwaniaSredniejKolumnParzystych.Format(L"Próg usuwania: %d", nProg);	
	UpdateData(FALSE);
	WyslijDoKamery();
	*pResult = 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Obsługa zmiany bitów zmiennej ISP_CONTROL 01
// parametry: brak
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUstawieniaKameryDlg::OnBnClickedCheckIsp1CyfroweEfektySpecjalne()
{
	UpdateData(TRUE);
	m_stKonfKamery.chKontrolaISP1 &= ~0x80;
	m_stKonfKamery.chKontrolaISP1 |= m_bCyfroweEfektySpecjalne * 0x80;
	WyslijDoKamery(); 
}


void CUstawieniaKameryDlg::OnBnClickedCheckIsp1UstawienieChrominancji()
{
	UpdateData(TRUE);
	m_stKonfKamery.chKontrolaISP1 &= ~0x40;
	m_stKonfKamery.chKontrolaISP1 |= m_bUstawienieChrominancji * 0x40;
	WyslijDoKamery(); 
}


void CUstawieniaKameryDlg::OnBnClickedCheckIsp1SkalowaniePionowe()
{
	UpdateData(TRUE);
	m_stKonfKamery.chKontrolaISP1 &= ~0x20;
	m_stKonfKamery.chKontrolaISP1 |= m_bSkalowaniePionowe * 0x20;
	WyslijDoKamery(); 
}


void CUstawieniaKameryDlg::OnBnClickedCheckIsp1SkalowaniePoziome()
{
	UpdateData(TRUE);
	m_stKonfKamery.chKontrolaISP1 &= ~0x10;
	m_stKonfKamery.chKontrolaISP1 |= m_bSkalowaniePoziome * 0x10;
	WyslijDoKamery(); 
}


void CUstawieniaKameryDlg::OnBnClickedCheckIsp1RozciaganieLini()
{
	UpdateData(TRUE);
	m_stKonfKamery.chKontrolaISP1 &= ~0x08;
	m_stKonfKamery.chKontrolaISP1 |= m_bRozciaganieLinii * 0x08;
	WyslijDoKamery(); 
}


void CUstawieniaKameryDlg::OnBnClickedCheckIsp1UsrednianieChrominancji()
{
	UpdateData(TRUE);
	m_stKonfKamery.chKontrolaISP1 &= ~0x04;
	m_stKonfKamery.chKontrolaISP1 |= m_bUsrednianieChrominancji * 0x04;
	WyslijDoKamery(); 
}


void CUstawieniaKameryDlg::OnBnClickedCheckIsp1MacierzKolorow()
{
	UpdateData(TRUE);
	m_stKonfKamery.chKontrolaISP1 &= ~0x02;
	m_stKonfKamery.chKontrolaISP1 |= m_bMacierzKolorow * 0x02;
	WyslijDoKamery(); 
}


void CUstawieniaKameryDlg::OnBnClickedCheck1Isp1AutoBalansBieli()
{
	UpdateData(TRUE);
	m_stKonfKamery.chKontrolaISP1 &= ~0x01;
	m_stKonfKamery.chKontrolaISP1 |= m_bAutomatycznyBalansBieli * 0x01;
	WyslijDoKamery(); 
}



