// CKonfigTelemetrii.cpp: plik implementacji
//

#include "pch.h"
#include "APL-SN.h"
#include "CKonfigTelemetrii.h"
#include "afxdialogex.h"
#include "definicje_telemetrii.h"
#include "APL-SNDoc.h"
#include "Komunikacja.h"

// Okno dialogowe CKonfigTelemetrii

IMPLEMENT_DYNAMIC(CKonfigTelemetrii, CDialogEx)

CKonfigTelemetrii::CKonfigTelemetrii(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TELEMETRIA, pParent)
	, m_bZmieniono(FALSE)
	, m_chIdZmiennej(0)
	, m_nOkres(0)
{

}

CKonfigTelemetrii::~CKonfigTelemetrii()
{
}

void CKonfigTelemetrii::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OKRES_TELEMETRII, m_ctlOkresTelemetrii);
	DDX_Control(pDX, IDC_LIST_ZMIENNE_TELE, m_ctlListaTele);
}


BEGIN_MESSAGE_MAP(CKonfigTelemetrii, CDialogEx)
	ON_BN_CLICKED(IDOK, &CKonfigTelemetrii::OnBnClickedOk)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ZMIENNE_TELE, &CKonfigTelemetrii::OnLvnItemchangedListZmienneTele)
	ON_LBN_SELCHANGE(IDC_OKRES_TELEMETRII, &CKonfigTelemetrii::OnLbnSelchangeOkresTelemetrii)
END_MESSAGE_MAP()


// Procedury obsługi komunikatów CKonfigTelemetrii


void CKonfigTelemetrii::OnBnClickedOk()
{
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
	if (m_bZmieniono)
	{
		uint8_t chErr = getKomunikacja().ZapiszOkresTelemetrii(m_chOkresTelemetrii, LICZBA_ZMIENNYCH_TELEMETRYCZNYCH);
		if (chErr)
		{
			CString strKomunikat;
			strKomunikat.Format(_T("Nie można zapisać danych do APL3! \nKod błędu: %d"), chErr);
			MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojojoj!"), MB_ICONEXCLAMATION, 0);
			CDialogEx::OnCancel();
		}
	}
	CDialogEx::OnOK();
}



/// <summary>
/// Funkcja oblicza numer pozycji na liście zmiennych dla podanego okresu
/// </summary>
/// <param name="chOkres"></param>
/// <returns>Obliczony numer pozycji na liście</returns>
int CKonfigTelemetrii::PozycjaDlaOkresu(uint8_t chOkres, uint8_t *chZaokraglonyOkres)
{
	if (chOkres < 5)
	{
		*chZaokraglonyOkres = chOkres;
		return chOkres;
	}
	if (chOkres < 7)	//14,2Hz
	{
		*chZaokraglonyOkres = 6;
		return 5;	
	}
	if (chOkres < 10)	//10Hz
	{
		*chZaokraglonyOkres = 9;
		return 6;	
	}
	if (chOkres < 20)	//5Hz
	{
		*chZaokraglonyOkres = 19;
		return 7;	
	}
	if (chOkres < 50)	//2Hz
	{
		*chZaokraglonyOkres = 49;	
		return 8;	
	}
	if (chOkres < 100)	//1Hz
	{
		*chZaokraglonyOkres = 99;
		return 9;	
	}
	if (chOkres < 200)	//0,5Hz
	{
		*chZaokraglonyOkres = 199;
		return 10;	
	}
	if (chOkres < 255)	//0,4Hz
	{
		*chZaokraglonyOkres = 249;
		return 11;	
	}
	else               //wyłączone
	{
		*chZaokraglonyOkres = 255;
		return 12;
	}
}



/// <summary>
/// Zwraca wartość okresu na podstawie pozycji na liście częstotliwosci 
/// </summary>
/// <param name="nPozycja"></param>
/// <returns></returns>
uint8_t CKonfigTelemetrii::OkresDlaPozycji(int nPozycja)
{
	uint8_t chOkres;
	switch (nPozycja)
	{
	case 0: chOkres = 0;	break;
	case 1: chOkres = 1;	break;
	case 2: chOkres = 2;	break;
	case 3: chOkres = 3;	break;
	case 4: chOkres = 4;	break;
	case 5: chOkres = 6;	break;
	case 6: chOkres = 9;	break;
	case 7: chOkres = 19;	break;
	case 8: chOkres = 49;	break;
	case 9: chOkres = 99;	break;
	case 10: chOkres = 199;	break;
	case 11: chOkres = 249;	break;
	case 12: chOkres = 255;	break;
	}
	return chOkres;
}



/// <summary>
/// Budowanie zawartosci okna konfiguracji telemetrii
/// </summary>
/// <returns></returns>
BOOL CKonfigTelemetrii::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Dodaj tutaj dodatkową inicjację
	int nPozycja, nPoprzedniaPoz = -1;
	int nMiejsceNaLiscie = 0;
	uint8_t chOkres;
	uint8_t chErr = 1;
	CString strNapis;
	

	// TODO: Dodaj tutaj swój kod procedury obsługi komunikatów
	nPozycja = m_ctlOkresTelemetrii.GetCount();	//liczba pozycji na liście
	if (nPozycja == 0)
	{
		//jeżeli nie ma pozycji to je wstaw
		for (int n = 0; n < 256; n++)
		{
			nPozycja = PozycjaDlaOkresu(n, &chOkres);
			if (nPoprzedniaPoz != nPozycja)
			{
				if (chOkres < 255)
					strNapis.Format(_T("%.1f Hz"), 100.0f / (chOkres + 1));
				else
					strNapis.Format(_T("Wyłączone"));
				m_ctlOkresTelemetrii.InsertString(nPozycja, strNapis);
				nPoprzedniaPoz = nPozycja;
			}
		}
	}

	//zaznacz na liście bieżącą pozycję 
	nPozycja = PozycjaDlaOkresu(m_nOkres, &chOkres);
	m_ctlOkresTelemetrii.SetCurSel(nPozycja);

	//wstaw listę zmiennych telemetrycznych
	m_ctlListaTele.InsertColumn(0, _T("Nazwa zmiennej"), 0, 120);
	m_ctlListaTele.InsertColumn(1, _T("Częstotliwość"), 0, 80);

	m_ctlListaTele.InsertItem(TELEID_AKCEL1X, _T("Akcelerometr1 X"), 2);
	m_ctlListaTele.InsertItem(TELEID_AKCEL1Y, _T("Akcelerometr1 Y"), 3);
	m_ctlListaTele.InsertItem(TELEID_AKCEL1Z, _T("Akcelerometr1 Z"), 5);
	m_ctlListaTele.InsertItem(TELEID_AKCEL2X, _T("Akcelerometr2 X"), 6);
	m_ctlListaTele.InsertItem(TELEID_AKCEL2Y, _T("Akcelerometr2 Y"));
	m_ctlListaTele.InsertItem(TELEID_AKCEL2Z, _T("Akcelerometr2 Z"));
	m_ctlListaTele.InsertItem(TELEID_ZYRO1P, _T("Żyroskop1 P"));
	m_ctlListaTele.InsertItem(TELEID_ZYRO1Q, _T("Żyroskop1 Q"));
	m_ctlListaTele.InsertItem(TELEID_ZYRO1R, _T("Żyroskop1 R"));
	m_ctlListaTele.InsertItem(TELEID_ZYRO2P, _T("Żyroskop2 P"));
	m_ctlListaTele.InsertItem(TELEID_ZYRO2Q, _T("Żyroskop2 Q"));
	m_ctlListaTele.InsertItem(TELEID_ZYRO2R, _T("Żyroskop2 R"));

	m_ctlListaTele.InsertItem(TELEID_MAGNE1X, _T("Magnetometr1 X"));
	m_ctlListaTele.InsertItem(TELEID_MAGNE1Y, _T("Magnetometr1 Y"));
	m_ctlListaTele.InsertItem(TELEID_MAGNE1Z, _T("Magnetometr1 Z"));
	m_ctlListaTele.InsertItem(TELEID_MAGNE2X, _T("Magnetometr2 X"));
	m_ctlListaTele.InsertItem(TELEID_MAGNE2Y, _T("Magnetometr2 Y"));
	m_ctlListaTele.InsertItem(TELEID_MAGNE2Z, _T("Magnetometr2 Z"));
	m_ctlListaTele.InsertItem(TELEID_MAGNE3X, _T("Magnetometr3 X"));
	m_ctlListaTele.InsertItem(TELEID_MAGNE3Y, _T("Magnetometr3 Y"));
	m_ctlListaTele.InsertItem(TELEID_MAGNE3Z, _T("Magnetometr3 Z"));

	m_ctlListaTele.InsertItem(TELEID_KAT_IMU1X, _T("AHRS tryg Phi"));
	m_ctlListaTele.InsertItem(TELEID_KAT_IMU1Y, _T("AHRS tryg Theta"));
	m_ctlListaTele.InsertItem(TELEID_KAT_IMU1Z, _T("AHRS tryg Psi"));
	m_ctlListaTele.InsertItem(TELEID_KAT_IMU2X, _T("AHRS kwat Phi"));
	m_ctlListaTele.InsertItem(TELEID_KAT_IMU2Y, _T("AHRS kwat Theta"));
	m_ctlListaTele.InsertItem(TELEID_KAT_IMU2Z, _T("AHRS kwat Psi"));
	m_ctlListaTele.InsertItem(TELEID_KAT_ZYRO1X, _T("AHRS żyro Phi"));
	m_ctlListaTele.InsertItem(TELEID_KAT_ZYRO1Y, _T("AHRS żyro Theta"));
	m_ctlListaTele.InsertItem(TELEID_KAT_ZYRO1Z, _T("AHRS żyro Psi"));

	m_ctlListaTele.InsertItem(TELEID_CISBEZW1, _T("Cisn. bzwzgl. 1"));
	m_ctlListaTele.InsertItem(TELEID_CISBEZW2, _T("Cisn. bzwzgl. 2"));
	m_ctlListaTele.InsertItem(TELEID_WYSOKOSC1, _T("Wysokosć AGL 1"));
	m_ctlListaTele.InsertItem(TELEID_WYSOKOSC2, _T("Wysokosć AGL 2"));
	m_ctlListaTele.InsertItem(TELEID_CISROZN1, _T("Cisn. różn. 1"));
	m_ctlListaTele.InsertItem(TELEID_CISROZN2, _T("Cisn. różn. 2"));
	m_ctlListaTele.InsertItem(TELEID_PREDIAS1, _T("Prędkość IAS 1"));
	m_ctlListaTele.InsertItem(TELEID_PREDIAS2, _T("Prędkość IAS 1"));
	m_ctlListaTele.InsertItem(TELEID_TEMPCIS1, _T("Temp.czuj.CiśnBzwg. 1"));
	m_ctlListaTele.InsertItem(TELEID_TEMPCIS2, _T("Temp.czuj.ciśn.bzwg.2"));
	m_ctlListaTele.InsertItem(TELEID_TEMPIMU1, _T("Temperatura IMU 1"));
	m_ctlListaTele.InsertItem(TELEID_TEMPIMU2, _T("Temperatura IMU 2"));
	m_ctlListaTele.InsertItem(TELEID_TEMPCISR1, _T("Temp. CzujCiśRożn. 1"));
	m_ctlListaTele.InsertItem(TELEID_TEMPCISR2, _T("Temper. Ciśn.Rożn. 2"));

	//Odczytaj z urządzenia liste telemetrii
	chErr = getKomunikacja().CzytajOkresTelemetrii(m_chOkresTelemetrii, LICZBA_ZMIENNYCH_TELEMETRYCZNYCH);
	if (chErr)
	{
		CString strKomunikat;
		strKomunikat.Format(_T("Nie można odebrać danych z APL3!\nKod błędu: %d"), chErr);
		MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojojoj!"), MB_ICONEXCLAMATION, 0);
		return FALSE;
	}

	for (int n = 0; n < LICZBA_ZMIENNYCH_TELE; n++)
	{
		strNapis.Format(_T("%.1f Hz"), MAX_CZESTOTLIWOSC_TELEMETRII / m_chOkresTelemetrii[n]);
		m_ctlListaTele.SetItemText(n, 1, strNapis);
		//m_ctlListaTele.get
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // WYJĄTEK: Strona właściwości OCX powinna zwrócić FALSE
}

/// <summary>
/// Klikięto na listę zmiennych. Pobierz bieżący okres i podświetl na liście częstotliwości
/// </summary>
/// <param name="pNMHDR"></param>
/// <param name="pResult"></param>
void CKonfigTelemetrii::OnLvnItemchangedListZmienneTele(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
	uint8_t chZaokraglonyokres;
	uint8_t chPozycjaCzestotliwosci;
	
	m_nIndeksZmiennej = pNMLV->iItem;	//zapamiętaj indeks modyfikowanej zmiennej

	chPozycjaCzestotliwosci = PozycjaDlaOkresu(m_chOkresTelemetrii[m_nIndeksZmiennej], &chZaokraglonyokres);
	m_ctlOkresTelemetrii.SetCurSel(chPozycjaCzestotliwosci);
	UpdateData(FALSE);	
	*pResult = 0;
}



/// <summary>
/// Kliknięto na liste częstotliwości
/// </summary>
void CKonfigTelemetrii::OnLbnSelchangeOkresTelemetrii()
{
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
	CString strNapis;

	m_nIndeksOkresu = m_ctlOkresTelemetrii.GetCurSel();	
	m_chOkresTelemetrii[m_nIndeksZmiennej] = OkresDlaPozycji(m_nIndeksOkresu);
	if (m_chOkresTelemetrii[m_nIndeksZmiennej] == 255)
		strNapis.Format(_T("Wyłączone"));
	else
		strNapis.Format(_T("%.1f Hz"), MAX_CZESTOTLIWOSC_TELEMETRII / (1 + m_chOkresTelemetrii[m_nIndeksZmiennej]));
	m_ctlListaTele.SetItemText(m_nIndeksZmiennej, 1, strNapis);
	m_bZmieniono = TRUE;
	UpdateData(FALSE);
}


