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
	, m_strWzmocnienie(_T(""))
	, m_strVTS(_T(""))
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
	, m_strSzerokoscPatrzenia(_T(""))
	, m_strWysokoscPatrzenia(_T(""))
	, m_strAGC_Adjust(_T(""))
	, m_bReczneWzmocnienie(FALSE)
	, m_bRecznyVTS(FALSE)
	, m_strNasycenie(_T(""))
	, m_strPoziomEkspozycji(_T(""))
{
	
}

CUstawieniaKameryDlg::~CUstawieniaKameryDlg()
{
}

void CUstawieniaKameryDlg::DoDataExchange(CDataExchange* pDX)
{
	//	CDialogEx::DoDataExchange(pDX);
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLID_SZER_ZDJECIA, m_ctlSzerokoscZdjecia);
	DDX_Control(pDX, IDC_SLID_WYS_ZDJECIA, m_ctlWysokoscZdjecia);
	DDX_Text(pDX, IDC_STATIC_SZEROKOSC, m_StrSzerokosc);
	DDX_Text(pDX, IDC_STATIC_WYSOKOSC, m_strWysokosc);
	DDX_Check(pDX, IDC_CHECK_ODWR_PION, m_bOdwrocPionowo);
	DDX_Check(pDX, IDC_CHECK_ODWR_POZ, m_bOdwrocPoziomo);
	DDX_Control(pDX, IDC_SLID_PRZES_POZIOME, m_ctlPrzesunieciePoziome);
	DDX_Control(pDX, IDC_SLID_PRZES_PIONOWE, m_ctlPrzesunieciePionowe);
	DDX_Text(pDX, IDC_STATIC_PRZES_POZOME, m_strPrzesunieciePoziome);
	DDX_Text(pDX, IDC_STATIC_PRZES_PIONOWE, m_strPrzesunieciePionowe);
	DDX_Check(pDX, IDC_CHECK_BALANS_RECZNY, m_bRecznyBalansBieli);
	DDX_Control(pDX, IDC_SLID_AWB_CZERWONY, m_ctlBalansBieli_Czerwony);
	DDX_Control(pDX, IDC_SLID_AWB_ZIELONY, m_ctlBalansBieli_Zielony);
	DDX_Control(pDX, IDC_SLID_AWB_NIEBIESKI, m_ctlBalansBieli_Niebieski);
	DDX_Text(pDX, IDC_STATIC_AWB_CZERWONA, m_strBalansBieliCzerwona);
	DDX_Text(pDX, IDC_STATIC_AWB_ZIELONA, m_strBalansBieliZielona);
	DDX_Text(pDX, IDC_STATIC_AWB_NIEBIESKA, m_strBalansBieliNiebieska);
	DDX_Control(pDX, IDC_SLID_AEC_CZAS, m_ctlCzasEkspozycji);
	DDX_Text(pDX, IDC_STATIC_CZAS_EKSPOZYCJI, m_strCzasEkspozycji);
	DDX_Control(pDX, IDC_SLID_WZMOCNIENIE, m_ctlAGCLongGain);
	DDX_Control(pDX, IDC_SLID_VTS, m_ctlAGC_VTS);
	DDX_Text(pDX, IDC_STATIC_WZMOCNIENIE, m_strWzmocnienie);
	DDX_Text(pDX, IDC_STATIC_VTS, m_strVTS);
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
	DDX_Control(pDX, IDC_SLID_SZEROKOSC_PATRZENIA, m_ctlSzerokoscPatrzenia);
	DDX_Control(pDX, IDC_SLID_WYSOKOSC_PATRZENIA, m_ctlWysokoscPatrzenia);
	DDX_Text(pDX, IDC_STATIC_SZEROKOSC_PATRZENIA, m_strSzerokoscPatrzenia);
	DDX_Text(pDX, IDC_STATIC_WYSOKOSC_PATRZENIA, m_strWysokoscPatrzenia);
	DDX_Control(pDX, IDC_SLID_AGC_ADJ, m_ctlAGC_Adjust);
	DDX_Text(pDX, IDC_STATIC_AGC_ADJ, m_strAGC_Adjust);
	DDX_Check(pDX, IDC_CHECK_RECZNE_WZMOCNIENIE, m_bReczneWzmocnienie);
	DDX_Check(pDX, IDC_CHECK_VTS, m_bRecznyVTS);
	DDX_Control(pDX, IDC_COMBO_TYP_OBRAZU, m_ctlTypObrazu);
	DDX_Text(pDX, IDC_STATIC_NASYCENIE, m_strNasycenie);
	DDX_Control(pDX, IDC_SLID_NASYCENIE, m_ctlNasycenie);
	DDX_Control(pDX, IDC_SLID_POZIOM_EXPOZYCJI, m_ctlPoziomEkspozycji);
	DDX_Text(pDX, IDC_STATIC_POZIOM_EKSPOZCJI, m_strPoziomEkspozycji);
}


//BEGIN_MESSAGE_MAP(CUstawieniaKameryDlg, CDialogEx)
BEGIN_MESSAGE_MAP(CUstawieniaKameryDlg, CDialog)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_SZER_ZDJECIA, &CUstawieniaKameryDlg::OnNMCustomdrawSlidSzerZdjecia)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_WYS_ZDJECIA, &CUstawieniaKameryDlg::OnNMCustomdrawSlidWysZdjecia)
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
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_WZMOCNIENIE, &CUstawieniaKameryDlg::OnNMCustomdrawSlidWzmocnienie)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_VTS, &CUstawieniaKameryDlg::OnNMCustomdrawSlidVts)
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
	ON_BN_CLICKED(IDC_BUT_WYSLIJ_DO_KAMERY, &CUstawieniaKameryDlg::OnBnClickedButWyslijDoKamery)
	ON_BN_CLICKED(IDCANCEL, &CUstawieniaKameryDlg::OnBnClickedCancel)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_SZEROKOSC_PATRZENIA, &CUstawieniaKameryDlg::OnNMCustomdrawSlidSzerokoscPatrzenia)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_WYSOKOSC_PATRZENIA, &CUstawieniaKameryDlg::OnNMCustomdrawSlidWysokoscPatrzenia)
	ON_BN_CLICKED(IDC_BUT_WYSLIJ_GRUPOWO, &CUstawieniaKameryDlg::OnBnClickedButWyslijGrupowo)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_AGC_ADJ, &CUstawieniaKameryDlg::OnNMCustomdrawSlidAgcAdj)
	ON_BN_CLICKED(IDC_CHECK_RECZNE_WZMOCNIENIE, &CUstawieniaKameryDlg::OnBnClickedCheckReczneWzmocnienie)
	ON_BN_CLICKED(IDC_CHECK_VTS, &CUstawieniaKameryDlg::OnBnClickedCheckVts)
	ON_CBN_SELCHANGE(IDC_COMBO_TYP_OBRAZU, &CUstawieniaKameryDlg::OnCbnSelchangeComboTypObrazu)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_NASYCENIE, &CUstawieniaKameryDlg::OnNMCustomdrawSlidNasycenie)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_POZIOM_EXPOZYCJI, &CUstawieniaKameryDlg::OnNMCustomdrawSlidPoziomExpozycji)
	ON_BN_CLICKED(IDC_BUT_RESET, &CUstawieniaKameryDlg::OnBnClickedButReset)
END_MESSAGE_MAP()


// Procedury obsługi komunikatów UstawieniaKameryDlg
BOOL CUstawieniaKameryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	PrzeladujDanezKamery();
	return TRUE;  
}



uint8_t CUstawieniaKameryDlg::PrzeladujDanezKamery()
{
	 uint8_t chErr = getKomunikacja().PobierzKamere(&m_stKonfKamery);
	if (chErr == ERR_OK)
	{
		m_bOdwrocPionowo = ((m_stKonfKamery.chObracanieObrazu & 0x40) == 0x40);
		m_bOdwrocPoziomo = ((m_stKonfKamery.chObracanieObrazu & 0x20) == 0x20);
		m_bKorekcjaLENC = ((m_stKonfKamery.chKontrolaISP0 & 0x80) == 0x80);
		m_bGammaYUV = ((m_stKonfKamery.chKontrolaISP0 & 0x40) == 0x40);
		m_bGammaSurowa = ((m_stKonfKamery.chKontrolaISP0 & 0x20) == 0x20);
		m_bUsuwaUsrednianie = ((m_stKonfKamery.chKontrolaISP0 & 0x10) == 0x10);
		m_bOdszumianie = ((m_stKonfKamery.chKontrolaISP0 & 0x08) == 0x08);
		m_bUsuwanieCzarnychPikseli = ((m_stKonfKamery.chKontrolaISP0 & 0x04) == 0x04);
		m_bUsuwanieBialychPikseli = ((m_stKonfKamery.chKontrolaISP0 & 0x02) == 0x02);
		m_bInterpolacja = ((m_stKonfKamery.chKontrolaISP0 & 0x01) == 0x01);
		m_bCyfroweEfektySpecjalne = ((m_stKonfKamery.chKontrolaISP1 & 0x80) == 0x80);
		GetDlgItem(IDC_SLID_NASYCENIE)->EnableWindow(m_bCyfroweEfektySpecjalne);
		GetDlgItem(IDC_STATIC_NASYCENIE)->EnableWindow(m_bCyfroweEfektySpecjalne);

		m_bUstawienieChrominancji = ((m_stKonfKamery.chKontrolaISP1 & 0x40) == 0x40);
		m_bSkalowaniePionowe = ((m_stKonfKamery.chKontrolaISP1 & 0x20) == 0x20);
		m_bSkalowaniePoziome = ((m_stKonfKamery.chKontrolaISP1 & 0x10) == 0x10);
		m_bRozciaganieLinii = ((m_stKonfKamery.chKontrolaISP1 & 0x08) == 0x08);
		m_bUsrednianieChrominancji = ((m_stKonfKamery.chKontrolaISP1 & 0x04) == 0x04);
		m_bMacierzKolorow = ((m_stKonfKamery.chKontrolaISP1 & 0x02) == 0x02);
		m_bAutomatycznyBalansBieli = ((m_stKonfKamery.chKontrolaISP1 & 0x01) == 0x01);

		m_stPierwotnaKonfiguracjaKamery = m_stKonfKamery;
		m_ctlSzerokoscZdjecia.SetRange(4, 2592 / KROK_ROZDZ_KAM, TRUE);
		m_ctlSzerokoscZdjecia.SetPos(m_stKonfKamery.chSzerWy);
		m_StrSzerokosc.Format(L"Szerokość zdjęcia %d [pix]", m_stKonfKamery.chSzerWy * KROK_ROZDZ_KAM);
		m_ctlWysokoscZdjecia.SetRange(4, 1944 / KROK_ROZDZ_KAM, TRUE);
		m_ctlWysokoscZdjecia.SetPos(m_stKonfKamery.chWysWy);
		m_strWysokosc.Format(L"Wysokość zdjęcia %d [pix]", m_stKonfKamery.chWysWy * KROK_ROZDZ_KAM);

		//rozmiar patrzenia nie może być mniejszy od rozmiaru obrazu wyjsciowego
		m_ctlSzerokoscPatrzenia.SetRange(m_stKonfKamery.chSzerWy, MAX_SZER_KAM / KROK_ROZDZ_KAM);
		m_ctlSzerokoscPatrzenia.SetPos(m_stKonfKamery.chSzerWe);
		m_strSzerokoscPatrzenia.Format(L"Szerokość patrzenia %d", m_stKonfKamery.chSzerWe * KROK_ROZDZ_KAM);
		GetDlgItem(IDC_SLID_SZEROKOSC_PATRZENIA)->EnableWindow(m_bSkalowaniePoziome);
		GetDlgItem(IDC_STATIC_SZEROKOSC_PATRZENIA)->EnableWindow(m_bSkalowaniePoziome);

		m_ctlWysokoscPatrzenia.SetRange(m_stKonfKamery.chWysWy, MAX_WYS_KAM / KROK_ROZDZ_KAM);
		m_ctlWysokoscPatrzenia.SetPos(m_stKonfKamery.chWysWe);
		m_strWysokoscPatrzenia.Format(L"Wysokość patrzenia %d", m_stKonfKamery.chWysWe * KROK_ROZDZ_KAM);
		GetDlgItem(IDC_SLID_WYSOKOSC_PATRZENIA)->EnableWindow(m_bSkalowaniePionowe);
		GetDlgItem(IDC_STATIC_WYSOKOSC_PATRZENIA)->EnableWindow(m_bSkalowaniePionowe);

		m_ctlPrzesunieciePoziome.SetRange(0, m_stKonfKamery.chSzerWe - m_stKonfKamery.chSzerWy);
		m_ctlPrzesunieciePoziome.SetPos(m_stKonfKamery.chPrzesWyPoz);
		m_strPrzesunieciePoziome.Format(L"Przesuniecie w poziomie %d", m_stKonfKamery.chPrzesWyPoz * KROK_ROZDZ_KAM);

		m_ctlPrzesunieciePionowe.SetRange(0, m_stKonfKamery.chWysWe - m_stKonfKamery.chWysWy);
		m_ctlPrzesunieciePionowe.SetPos(m_stKonfKamery.chPrzesWyPio);
		m_strPrzesunieciePionowe.Format(L"Przesuniecie w pionie %d", m_stKonfKamery.chPrzesWyPio * KROK_ROZDZ_KAM);

		//m_ctlCzasEkspozycji.SetRange(0, 0xFFFF / KROK_ROZDZ_KAM);
		m_ctlCzasEkspozycji.SetRange(5, 5000 / KROK_ROZDZ_KAM);	//w praktyce tyle wystarczy
		m_ctlCzasEkspozycji.SetPos(m_stKonfKamery.sCzasEkspozycji / KROK_ROZDZ_KAM);
		m_strCzasEkspozycji.Format(L"Czas ekspozycji: %d", m_stKonfKamery.sCzasEkspozycji);
		m_bRecznyCzasEkspozycji = ((m_stKonfKamery.chKontrolaExpo & 0x01) == 0x01);
		GetDlgItem(IDC_SLID_AEC_CZAS)->EnableWindow(m_bRecznyCzasEkspozycji);
		GetDlgItem(IDC_STATIC_CZAS_EKSPOZYCJI)->EnableWindow(m_bRecznyCzasEkspozycji);

		m_ctlAGCLongGain.SetRange(0, 0x1FF / KROK_ROZDZ_KAM);
		m_ctlAGCLongGain.SetPos(m_stKonfKamery.sAGCLong / KROK_ROZDZ_KAM);
		m_strWzmocnienie.Format(L"Wzmocnienie: %d", m_stKonfKamery.sAGCLong);
		m_bReczneWzmocnienie = ((m_stKonfKamery.chKontrolaExpo & 0x02) == 0x02);
		GetDlgItem(IDC_SLID_WZMOCNIENIE)->EnableWindow(m_bReczneWzmocnienie);
		GetDlgItem(IDC_STATIC_WZMOCNIENIE)->EnableWindow(m_bReczneWzmocnienie);

		m_ctlAGC_VTS.SetRange(0, 0x1FF / KROK_ROZDZ_KAM);
		m_ctlAGC_VTS.SetPos(m_stKonfKamery.sAGCVTS / KROK_ROZDZ_KAM);
		m_strVTS.Format(L"VTS: %d", m_stKonfKamery.sAGCVTS);
		m_bRecznyVTS = ((m_stKonfKamery.chKontrolaExpo & 0x04) == 0x04);
		GetDlgItem(IDC_SLID_VTS)->EnableWindow(m_bRecznyVTS);
		GetDlgItem(IDC_STATIC_VTS)->EnableWindow(m_bRecznyVTS);

		m_ctlAGC_Adjust.SetRange(0, 0x1FF / KROK_ROZDZ_KAM);
		m_ctlAGC_Adjust.SetPos(m_stKonfKamery.sAGCAdjust / KROK_ROZDZ_KAM);
		m_strAGC_Adjust.Format(L"AGC adjust: %d", m_stKonfKamery.sAGCAdjust);

		//ręczny balans bieli jest w zakresie 0..1 a konkretnie 0/0x400..0x400/0x400 
		m_ctlBalansBieli_Czerwony.SetRange(0, 0x400);
		m_ctlBalansBieli_Zielony.SetRange(0, 0x400);
		m_ctlBalansBieli_Niebieski.SetRange(0, 0x400);
		m_ctlBalansBieli_Czerwony.SetPos(m_stKonfKamery.sWzmocnienieR);
		m_ctlBalansBieli_Zielony.SetPos(m_stKonfKamery.sWzmocnienieG);
		m_ctlBalansBieli_Niebieski.SetPos(m_stKonfKamery.sWzmocnienieB);

		//włącz lub wyłącz kontrolki w zależności od stanu bitu je włączajacego
		m_bRecznyBalansBieli = m_stKonfKamery.chKontrolaBalansuBieli & 0x01;
		GetDlgItem(IDC_SLID_AWB_CZERWONY)->EnableWindow(m_bRecznyBalansBieli);
		GetDlgItem(IDC_SLID_AWB_ZIELONY)->EnableWindow(m_bRecznyBalansBieli);
		GetDlgItem(IDC_SLID_AWB_NIEBIESKI)->EnableWindow(m_bRecznyBalansBieli);
		GetDlgItem(IDC_STATIC_AWB_CZERWONA)->EnableWindow(m_bRecznyBalansBieli);
		GetDlgItem(IDC_STATIC_AWB_ZIELONA)->EnableWindow(m_bRecznyBalansBieli);
		GetDlgItem(IDC_STATIC_AWB_NIEBIESKA)->EnableWindow(m_bRecznyBalansBieli);

		m_bUsuwaUsrednianie = ((m_stKonfKamery.chKontrolaISP0 & 0x10) == 0x10);
		GetDlgItem(IDC_SLID_PROG_USREDNIANIA_KOLUMN)->EnableWindow(m_bUsuwaUsrednianie);
		GetDlgItem(IDC_STATIC_PROG_USUWANIA)->EnableWindow(m_bUsuwaUsrednianie);
		UpdateData(FALSE);

		m_ctlTypObrazu.InsertString(0, L"RGB 5-6-5");
		m_ctlTypObrazu.InsertString(1, L"RGB 5-5-5");
		m_ctlTypObrazu.InsertString(2, L"Czarno-Biały");
		m_ctlTypObrazu.InsertString(3, L"Nieznany!");
		switch (m_stKonfKamery.chFormatObrazu)
		{
		case 0x6F: m_ctlTypObrazu.SetCurSel(0); break;
		case 0x7F: m_ctlTypObrazu.SetCurSel(1); break;
		case 0x10: m_ctlTypObrazu.SetCurSel(2); break;
		default:   m_ctlTypObrazu.SetCurSel(3); break;
		}

		m_ctlNasycenie.SetRange(0, 0xFF / KROK_ROZDZ_KAM);
		m_ctlNasycenie.SetPos(m_stKonfKamery.chNasycenie / KROK_ROZDZ_KAM);
		m_strNasycenie.Format(L"Nasycenie koloru: %d", m_stKonfKamery.chNasycenie);

		m_ctlPoziomEkspozycji.SetRange(0, SKALA_POZIOMU_EKSPOZYCJI);
		m_ctlPoziomEkspozycji.SetPos(m_stKonfKamery.chPoziomEkspozycji);
		m_strPoziomEkspozycji.Format(L"Poziom ekspozycji: %.2fEV", 0.0);
	}
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Obsługa zmiany położenia suwaka szerokości obrazu
// Ustawia szerokość wyjsciową czyli rejestr 0x3808
// parametry: ?
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUstawieniaKameryDlg::OnNMCustomdrawSlidSzerZdjecia(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	m_stKonfKamery.chSzerWy = m_ctlSzerokoscZdjecia.GetPos();
	m_StrSzerokosc.Format(L"Szerokość zdjęcia %d [pix]", m_stKonfKamery.chSzerWy * KROK_ROZDZ_KAM);
	m_ctlSzerokoscPatrzenia.SetRangeMin(m_stKonfKamery.chSzerWy, TRUE);		//aktualizuj dolną granicę szerokości patrzenia
	UpdateData(FALSE);
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
	m_stKonfKamery.chWysWy = (uint8_t)m_ctlWysokoscZdjecia.GetPos();
	m_strWysokosc.Format(L"Wysokość zdjęcia %d [pix]", m_stKonfKamery.chWysWy * KROK_ROZDZ_KAM);
	m_ctlWysokoscPatrzenia.SetRangeMin(m_stKonfKamery.chWysWy, TRUE);	//aktualizuj dolną granicę wysokości patrzenia
	UpdateData(FALSE);
	*pResult = 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Obsługa zmiany położenia suwaka szerokości patrzenia
// parametry: ?
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUstawieniaKameryDlg::OnNMCustomdrawSlidSzerokoscPatrzenia(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	m_stKonfKamery.chSzerWe = m_ctlSzerokoscPatrzenia.GetPos();
	m_strSzerokoscPatrzenia.Format(L"Szerokość patrzenia %d", m_stKonfKamery.chSzerWe * KROK_ROZDZ_KAM);
	UpdateData(FALSE);
	*pResult = 0;
}


void CUstawieniaKameryDlg::OnNMCustomdrawSlidWysokoscPatrzenia(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	m_stKonfKamery.chWysWe = m_ctlWysokoscPatrzenia.GetPos();
	m_strWysokoscPatrzenia.Format(L"Wysokość patrzenia %d", m_stKonfKamery.chWysWe * KROK_ROZDZ_KAM);
	UpdateData(FALSE);
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
	m_strPrzesunieciePoziome.Format(L"Przesunięcie w poziomie %d", m_stKonfKamery.chPrzesWyPoz * KROK_ROZDZ_KAM);
	UpdateData(FALSE);
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
	m_strPrzesunieciePionowe.Format(L"Przesunięcie w pionie %d", m_stKonfKamery.chPrzesWyPio * KROK_ROZDZ_KAM);
	UpdateData(FALSE);
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
	GetDlgItem(IDC_SLID_AWB_CZERWONY)->EnableWindow(m_bRecznyBalansBieli);
	GetDlgItem(IDC_SLID_AWB_ZIELONY)->EnableWindow(m_bRecznyBalansBieli);
	GetDlgItem(IDC_SLID_AWB_NIEBIESKI)->EnableWindow(m_bRecznyBalansBieli);
	GetDlgItem(IDC_STATIC_AWB_CZERWONA)->EnableWindow(m_bRecznyBalansBieli);
	GetDlgItem(IDC_STATIC_AWB_ZIELONA)->EnableWindow(m_bRecznyBalansBieli);
	GetDlgItem(IDC_STATIC_AWB_NIEBIESKA)->EnableWindow(m_bRecznyBalansBieli);
}



int CUstawieniaKameryDlg::WyslijDoKamery()
{
	return getKomunikacja().UstawKamere(&m_stKonfKamery);
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
	if (m_bRecznyCzasEkspozycji)
		m_stKonfKamery.chKontrolaExpo |= 0x01;
	else
		m_stKonfKamery.chKontrolaExpo &= ~0x01;
	GetDlgItem(IDC_SLID_AEC_CZAS)->EnableWindow(m_bRecznyCzasEkspozycji);
	GetDlgItem(IDC_STATIC_CZAS_EKSPOZYCJI)->EnableWindow(m_bRecznyCzasEkspozycji);
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
	int nCzasEkspozycji = m_ctlCzasEkspozycji.GetPos() * KROK_ROZDZ_KAM;
	m_stKonfKamery.sCzasEkspozycji = nCzasEkspozycji & 0xFFFF;
	m_strCzasEkspozycji.Format(L"Czas ekspozycji: %d", nCzasEkspozycji);
	UpdateData(FALSE);
	*pResult = 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Obsługa zmiany położenia suwaka wzmocnienia
// parametry: ?
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUstawieniaKameryDlg::OnNMCustomdrawSlidWzmocnienie(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	int nWzmocnienie = m_ctlAGCLongGain.GetPos();
	m_stKonfKamery.sAGCLong = nWzmocnienie & 0x1FF;
	m_strWzmocnienie.Format(L"Wzmocnienie: %d", nWzmocnienie);
	UpdateData(FALSE);
	*pResult = 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na kliknięcie checkbox ręcznej regulacji wzmocnienia
// Ustawia bit 1 w rejestrze 0x3503, konf->chKontrolaExpo
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUstawieniaKameryDlg::OnBnClickedCheckReczneWzmocnienie()
{
	UpdateData(TRUE);
	if (m_bReczneWzmocnienie)
		m_stKonfKamery.chKontrolaExpo |=  0x02;
	else
		m_stKonfKamery.chKontrolaExpo &= ~0x02;
	GetDlgItem(IDC_SLID_WZMOCNIENIE)->EnableWindow(m_bReczneWzmocnienie);
	GetDlgItem(IDC_STATIC_WZMOCNIENIE)->EnableWindow(m_bReczneWzmocnienie);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Obsługa zmiany położenia suwaka VTS
// parametry: ?
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUstawieniaKameryDlg::OnNMCustomdrawSlidVts(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	int nVTS = m_ctlAGC_VTS.GetPos();
	m_stKonfKamery.sAGCVTS = nVTS & 0x1FF;
	m_strVTS.Format(L"AGC VTS: %d", nVTS);
	UpdateData(FALSE);
	*pResult = 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na kliknięcie checkbox ręcznej regulacji VTS
// Ustawia bit 2 w rejestrze 0x3503, konf->chKontrolaExpo
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUstawieniaKameryDlg::OnBnClickedCheckVts()
{
	UpdateData(TRUE);
	if (m_bRecznyVTS)
		m_stKonfKamery.chKontrolaExpo |= 0x04;
	else
		m_stKonfKamery.chKontrolaExpo &= ~0x04;
	GetDlgItem(IDC_SLID_VTS)->EnableWindow(m_bRecznyVTS);
	GetDlgItem(IDC_STATIC_VTS)->EnableWindow(m_bRecznyVTS);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Obsługa zmiany położenia suwaka AGC Adjust
// parametry: ?
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUstawieniaKameryDlg::OnNMCustomdrawSlidAgcAdj(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	int nAGCAdj = m_ctlAGC_Adjust.GetPos();
	m_stKonfKamery.sAGCAdjust = nAGCAdj & 0x1FF;
	m_strAGC_Adjust.Format(L"AGC adjust: %d", nAGCAdj);
	UpdateData(FALSE);
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
}


void CUstawieniaKameryDlg::OnBnClickedCheckGamaYuv()
{
	UpdateData(TRUE);
	m_stKonfKamery.chKontrolaISP0 &= ~0x40;
	m_stKonfKamery.chKontrolaISP0 |= m_bGammaYUV * 0x40;
}


void CUstawieniaKameryDlg::OnBnClickedCheckGammaSurowa()
{
	UpdateData(TRUE);
	m_stKonfKamery.chKontrolaISP0 &= ~0x20;
	m_stKonfKamery.chKontrolaISP0 |= m_bGammaSurowa * 0x20;
}


void CUstawieniaKameryDlg::OnBnClickedCheckIsp0UsuwaUsred()
{
	UpdateData(TRUE);
	m_stKonfKamery.chKontrolaISP0 &= ~0x10;
	m_stKonfKamery.chKontrolaISP0 |= m_bUsuwaUsrednianie * 0x10;
	GetDlgItem(IDC_SLID_PROG_USREDNIANIA_KOLUMN)->EnableWindow(m_bUsuwaUsrednianie);
	GetDlgItem(IDC_STATIC_PROG_USUWANIA)->EnableWindow(m_bUsuwaUsrednianie);
}


void CUstawieniaKameryDlg::OnBnClickedCheckOdszumianie()
{
	UpdateData(TRUE);
	m_stKonfKamery.chKontrolaISP0 &= ~0x08;
	m_stKonfKamery.chKontrolaISP0 |= m_bOdszumianie * 0x08;
}


void CUstawieniaKameryDlg::OnBnClickedCheckIsp0UsuwanieCzarnych()
{
	UpdateData(TRUE);
	m_stKonfKamery.chKontrolaISP0 &= ~0x04;
	m_stKonfKamery.chKontrolaISP0 |= m_bUsuwanieCzarnychPikseli * 0x04;
}


void CUstawieniaKameryDlg::OnBnClickedCheckIsp0UsuwanieBialych()
{
	UpdateData(TRUE);
	m_stKonfKamery.chKontrolaISP0 &= ~0x02;
	m_stKonfKamery.chKontrolaISP0 |= m_bUsuwanieBialychPikseli * 0x02;
}


void CUstawieniaKameryDlg::OnBnClickedCheckIsp0Interpolacja()
{
	UpdateData(TRUE);
	m_stKonfKamery.chKontrolaISP0 &= ~0x01;
	m_stKonfKamery.chKontrolaISP0 |= m_bInterpolacja * 0x01;
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
	GetDlgItem(IDC_SLID_NASYCENIE)->EnableWindow(m_bCyfroweEfektySpecjalne);
	GetDlgItem(IDC_STATIC_NASYCENIE)->EnableWindow(m_bCyfroweEfektySpecjalne);
}


void CUstawieniaKameryDlg::OnBnClickedCheckIsp1UstawienieChrominancji()
{
	UpdateData(TRUE);
	m_stKonfKamery.chKontrolaISP1 &= ~0x40;
	m_stKonfKamery.chKontrolaISP1 |= m_bUstawienieChrominancji * 0x40;
}


void CUstawieniaKameryDlg::OnBnClickedCheckIsp1SkalowaniePionowe()
{
	UpdateData(TRUE);
	m_stKonfKamery.chKontrolaISP1 &= ~0x20;
	m_stKonfKamery.chKontrolaISP1 |= m_bSkalowaniePionowe * 0x20;
	GetDlgItem(IDC_SLID_WYSOKOSC_PATRZENIA)->EnableWindow(m_bSkalowaniePionowe);
	GetDlgItem(IDC_STATIC_WYSOKOSC_PATRZENIA)->EnableWindow(m_bSkalowaniePionowe);
}


void CUstawieniaKameryDlg::OnBnClickedCheckIsp1SkalowaniePoziome()
{
	UpdateData(TRUE);
	m_stKonfKamery.chKontrolaISP1 &= ~0x10;
	m_stKonfKamery.chKontrolaISP1 |= m_bSkalowaniePoziome * 0x10;
	GetDlgItem(IDC_SLID_SZEROKOSC_PATRZENIA)->EnableWindow(m_bSkalowaniePoziome);
	GetDlgItem(IDC_STATIC_SZEROKOSC_PATRZENIA)->EnableWindow(m_bSkalowaniePoziome);
}


void CUstawieniaKameryDlg::OnBnClickedCheckIsp1RozciaganieLini()
{
	UpdateData(TRUE);
	m_stKonfKamery.chKontrolaISP1 &= ~0x08;
	m_stKonfKamery.chKontrolaISP1 |= m_bRozciaganieLinii * 0x08;
}


void CUstawieniaKameryDlg::OnBnClickedCheckIsp1UsrednianieChrominancji()
{
	UpdateData(TRUE);
	m_stKonfKamery.chKontrolaISP1 &= ~0x04;
	m_stKonfKamery.chKontrolaISP1 |= m_bUsrednianieChrominancji * 0x04;
}


void CUstawieniaKameryDlg::OnBnClickedCheckIsp1MacierzKolorow()
{
	UpdateData(TRUE);
	m_stKonfKamery.chKontrolaISP1 &= ~0x02;
	m_stKonfKamery.chKontrolaISP1 |= m_bMacierzKolorow * 0x02;
}


void CUstawieniaKameryDlg::OnBnClickedCheck1Isp1AutoBalansBieli()
{
	UpdateData(TRUE);
	m_stKonfKamery.chKontrolaISP1 &= ~0x01;
	m_stKonfKamery.chKontrolaISP1 |= m_bAutomatycznyBalansBieli * 0x01;	
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wysyła ramkę z bieżącymi nastawami do kamery
// // parametry: nic
// zwraca: kod błędu
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUstawieniaKameryDlg::OnBnClickedButWyslijDoKamery()
{
	getKomunikacja().UstawKamere(&m_stKonfKamery);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zamknięcie okna przyciskiem Anuluj
// Wysyła polecenie zapisu konfiguracji kamery
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUstawieniaKameryDlg::OnBnClickedOk()
{
	CDialog::OnOK();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na zamknięcie okna przyciskiem OK
// Wysyła polecenie zapisu konfiguracji kamery
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUstawieniaKameryDlg::OnBnClickedCancel()
{
	//jeżeli coś się zmieniło to przywróc oryginalną wersję
	if ((m_stPierwotnaKonfiguracjaKamery.chKontrolaISP0 != m_stKonfKamery.chKontrolaISP0) || 
		(m_stPierwotnaKonfiguracjaKamery.chKontrolaISP1 != m_stKonfKamery.chKontrolaISP1) ||
		(m_stPierwotnaKonfiguracjaKamery.chFormatObrazu != m_stKonfKamery.chFormatObrazu) ||
		(m_stPierwotnaKonfiguracjaKamery.sAGCLong != m_stKonfKamery.sAGCLong) ||
		(m_stPierwotnaKonfiguracjaKamery.sAGCVTS != m_stKonfKamery.sAGCVTS) ||
		(m_stPierwotnaKonfiguracjaKamery.sAGCAdjust != m_stKonfKamery.sAGCAdjust) ||
		(m_stPierwotnaKonfiguracjaKamery.chKontrolaBalansuBieli != m_stKonfKamery.chKontrolaBalansuBieli) ||
		(m_stPierwotnaKonfiguracjaKamery.chKontrolaExpo != m_stKonfKamery.chKontrolaExpo) ||
		(m_stPierwotnaKonfiguracjaKamery.chObracanieObrazu != m_stKonfKamery.chObracanieObrazu) ||
		(m_stPierwotnaKonfiguracjaKamery.chProgUsuwania != m_stKonfKamery.chProgUsuwania) ||
		(m_stPierwotnaKonfiguracjaKamery.chPrzesWyPio != m_stKonfKamery.chPrzesWyPio) ||
		(m_stPierwotnaKonfiguracjaKamery.chPrzesWyPoz != m_stKonfKamery.chPrzesWyPoz) ||
		(m_stPierwotnaKonfiguracjaKamery.chSzerWe != m_stKonfKamery.chSzerWe) ||
		(m_stPierwotnaKonfiguracjaKamery.chSzerWy != m_stKonfKamery.chSzerWy) ||
		(m_stPierwotnaKonfiguracjaKamery.chTrybyEkspozycji != m_stKonfKamery.chTrybyEkspozycji) ||
		(m_stPierwotnaKonfiguracjaKamery.chWysWe != m_stKonfKamery.chWysWe) ||
		(m_stPierwotnaKonfiguracjaKamery.chWysWy != m_stKonfKamery.chWysWy) ||
		(m_stPierwotnaKonfiguracjaKamery.sCzasEkspozycji != m_stKonfKamery.sCzasEkspozycji) ||
		(m_stPierwotnaKonfiguracjaKamery.sWzmocnienieB != m_stKonfKamery.sWzmocnienieB) ||
		(m_stPierwotnaKonfiguracjaKamery.sWzmocnienieG != m_stKonfKamery.sWzmocnienieG) ||
		(m_stPierwotnaKonfiguracjaKamery.sWzmocnienieR != m_stKonfKamery.sWzmocnienieR))
			getKomunikacja().UstawKamere(&m_stPierwotnaKonfiguracjaKamery);
	CDialog::OnCancel();
}




void CUstawieniaKameryDlg::OnBnClickedButWyslijGrupowo()
{
	getKomunikacja().UstawKamereGrupowo(&m_stKonfKamery);
}




///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na wybór formatu koloru
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUstawieniaKameryDlg::OnCbnSelchangeComboTypObrazu()
{
	UpdateData(TRUE);
	switch (m_ctlTypObrazu.GetCurSel())
	{
	case 0: m_stKonfKamery.chFormatObrazu = 0x6F;	break;
	case 1: m_stKonfKamery.chFormatObrazu = 0x7F;	break;
	case 2: m_stKonfKamery.chFormatObrazu = 0x10;	break;
	default: m_stKonfKamery.chFormatObrazu = 0x6F;	break;
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Obsługa zmiany położenia suwaka nasycenia koloru
// parametry: ?
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUstawieniaKameryDlg::OnNMCustomdrawSlidNasycenie(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	m_stKonfKamery.chNasycenie = m_ctlNasycenie.GetPos() * KROK_ROZDZ_KAM;
	m_strNasycenie.Format(L"Nasycenie koloru: %d", m_stKonfKamery.chNasycenie);
	UpdateData(FALSE);
	*pResult = 0;
}


void CUstawieniaKameryDlg::OnNMCustomdrawSlidPoziomExpozycji(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	m_stKonfKamery.chPoziomEkspozycji = m_ctlPoziomEkspozycji.GetPos();
	m_strPoziomEkspozycji.Format(L"Poziom ekspozycji: %.2fEV", (m_stKonfKamery.chPoziomEkspozycji - SKALA_POZIOMU_EKSPOZYCJI/2) * 1.7 / 40);	//wzór wyliczony na podstawie wartosci dla EV=0.0 i EV=1.7. (EV1.7 - EV0.0) * 1.7 / połowa różnicy wartości EV
	UpdateData(FALSE);
	*pResult = 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// naciśnieto przycisk resetowania kamery. Wysyła polecenie sprzętowo resetujace kamerę i pobiera domyślne nastawy
// parametry: brak
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUstawieniaKameryDlg::OnBnClickedButReset()
{
	uint8_t chErr;
	CString strOpis;
	chErr = getKomunikacja().ResetujKamere();
	if (chErr)
	{
		strOpis.Format(L"Funkcja resetująca zwróciła kod błędu: %d", chErr);
		MessageBoxExW(this->m_hWnd, strOpis, L"Ojojoj!", MB_ICONEXCLAMATION, 0);
	}
	else
	{
		chErr = getKomunikacja().PobierzKamere(&m_stKonfKamery);
		if (chErr)
		{
			strOpis.Format(L"Funkcja pobierające dane z kamery zwróciła kod błędu: %d", chErr);
			MessageBoxExW(this->m_hWnd, strOpis, L"Ojojoj!", MB_ICONEXCLAMATION, 0);
		}
		else
			PrzeladujDanezKamery();
	}
}
