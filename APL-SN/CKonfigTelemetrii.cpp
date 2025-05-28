// CKonfigTelemetrii.cpp: plik implementacji
//

#include "pch.h"
#include "APL-SN.h"
#include "CKonfigTelemetrii.h"
#include "afxdialogex.h"
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
	DDX_Control(pDX, IDC_LISTA_CZESTOTLIWOSCI, m_ctlListaCzestotliwosciTelemetrii);
	DDX_Control(pDX, IDC_LIST_ZMIENNE_TELE, m_ctlOkresTelemetrii);
}


BEGIN_MESSAGE_MAP(CKonfigTelemetrii, CDialogEx)
	ON_BN_CLICKED(IDOK, &CKonfigTelemetrii::OnBnClickedOk)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ZMIENNE_TELE, &CKonfigTelemetrii::OnLvnItemchangedListZmienneTele)
	ON_LBN_SELCHANGE(IDC_LISTA_CZESTOTLIWOSCI, &CKonfigTelemetrii::OnLbnSelchangeListaCzestotliwosci)
END_MESSAGE_MAP()


// Procedury obsługi komunikatów CKonfigTelemetrii


void CKonfigTelemetrii::OnBnClickedOk()
{
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
	if (m_bZmieniono)
	{
		//zapisz do roju oraz wyślij ramkę do wrona
		for (int n=0; n< LICZBA_ZMIENNYCH_TELEMETRYCZNYCH; n++)
			getKomunikacja().m_cRoj.vWron[m_nIndeksDronaWRoju].m_sOkresTelemetrii[n] = m_sOkresTelemetrii[n];

		uint8_t chErr = getKomunikacja().ZapiszOkresTelemetrii(m_sOkresTelemetrii, LICZBA_ZMIENNYCH_TELEMETRYCZNYCH);
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
	if (chOkres == 0)
	{
		*chZaokraglonyOkres = TEMETETRIA_WYLACZONA;
		return 12;
	}
	if (chOkres <= 5)
	{
		*chZaokraglonyOkres = chOkres;
		return chOkres - 1;
	}
	if (chOkres <= 7)	//14,2Hz
	{
		*chZaokraglonyOkres = 7;
		return 5;	
	}
	if (chOkres <= 10)	//10Hz
	{
		*chZaokraglonyOkres = 10;
		return 6;	
	}
	if (chOkres <= 20)	//5Hz
	{
		*chZaokraglonyOkres = 20;
		return 7;	
	}
	if (chOkres <= 50)	//2Hz
	{
		*chZaokraglonyOkres = 50;	
		return 8;	
	}
	if (chOkres <= 100)	//1Hz
	{
		*chZaokraglonyOkres = 100;
		return 9;	
	}
	if (chOkres <= 200)	//0,5Hz
	{
		*chZaokraglonyOkres = 200;
		return 10;	
	}
	if (chOkres < 255)	//0,4Hz
	{
		*chZaokraglonyOkres = 250;
		return 11;	
	}
	else               //wyłączone
	{
		*chZaokraglonyOkres = TEMETETRIA_WYLACZONA;
		return 12;
	}
}



/// <summary>
/// Zwraca wartość okresu na podstawie pozycji na liście częstotliwosci 
/// </summary>
/// <param name="nPozycja"></param>
/// <returns></returns>
uint16_t CKonfigTelemetrii::OkresDlaPozycji(int nPozycja)
{
	uint16_t sOkres;
	switch (nPozycja)
	{
	case 0: sOkres = 1;	break;
	case 1: sOkres = 2;	break;
	case 2: sOkres = 3;	break;
	case 3: sOkres = 4;	break;
	case 4: sOkres = 5;	break;
	case 5: sOkres = 7;	break;
	case 6: sOkres = 10;	break;
	case 7: sOkres = 20;	break;
	case 8: sOkres = 50;	break;
	case 9: sOkres = 100;	break;
	case 10: sOkres = 200;	break;
	case 11: sOkres = 250;	break;
	case 12: sOkres = 255;	break;
	}
	return sOkres;
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
	float fCzestotliwosc;
	

	// TODO: Dodaj tutaj swój kod procedury obsługi komunikatów

	//Zbuduj listę częstotliwości
	nPozycja = m_ctlListaCzestotliwosciTelemetrii.GetCount();	//liczba pozycji na liście
	if (nPozycja == 0)
	{
		//jeżeli nie ma pozycji to je wstaw
		for (int n = 1; n < 256; n++)
		{
			nPozycja = PozycjaDlaOkresu(n, &chOkres);
			if (nPoprzedniaPoz != nPozycja)
			{
				if (chOkres < TEMETETRIA_WYLACZONA)
					strNapis.Format(_T("%.1f Hz"), MAX_CZESTOTLIWOSC_TELEMETRII / chOkres);
				else
					strNapis.Format(_T("Wyłączone"));
				m_ctlListaCzestotliwosciTelemetrii.InsertString(nPozycja, strNapis);
				nPoprzedniaPoz = nPozycja;
			}
		}
	}

	//wstaw listę zmiennych telemetrycznych
	m_ctlOkresTelemetrii.InsertColumn(0, _T("Nazwa zmiennej"), 0, 120);
	m_ctlOkresTelemetrii.InsertColumn(1, _T("Częstotliwość"), 0, 80);

	//Odczytaj z roju liste telemetrii
	for (uint8_t n = 0; n < LICZBA_ZMIENNYCH_TELEMETRYCZNYCH; n++)
	{
		m_sOkresTelemetrii[n] = getKomunikacja().m_cRoj.vWron[m_nIndeksDronaWRoju].m_sOkresTelemetrii[n];
		m_ctlOkresTelemetrii.InsertItem(n, getKomunikacja().m_strNazwyZmiennychTele[n]);
		fCzestotliwosc = getKomunikacja().m_cRoj.vWron[m_nIndeksDronaWRoju].PobierzCzestotliwoscTelemetrii(n);
		if (fCzestotliwosc)
			strNapis.Format(_T("%.1f Hz"), fCzestotliwosc);
		else
			strNapis.Format(_T("Wyłączone"));
		m_ctlOkresTelemetrii.SetItemText(n, 1, strNapis);
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

	chPozycjaCzestotliwosci = PozycjaDlaOkresu(m_sOkresTelemetrii[m_nIndeksZmiennej], &chZaokraglonyokres);
	m_ctlListaCzestotliwosciTelemetrii.SetCurSel(chPozycjaCzestotliwosci);
	UpdateData(FALSE);	
	*pResult = 0;
}



/// <summary>
/// Kliknięto na liste częstotliwości
/// </summary>
void CKonfigTelemetrii::OnLbnSelchangeListaCzestotliwosci()
{
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
	CString strNapis;

	m_nIndeksOkresu = m_ctlListaCzestotliwosciTelemetrii.GetCurSel();
	m_sOkresTelemetrii[m_nIndeksZmiennej] = OkresDlaPozycji(m_nIndeksOkresu);
	if (m_sOkresTelemetrii[m_nIndeksZmiennej] == TEMETETRIA_WYLACZONA)
		strNapis.Format(_T("Wyłączone"));
	else
		strNapis.Format(_T("%.1f Hz"), MAX_CZESTOTLIWOSC_TELEMETRII / m_sOkresTelemetrii[m_nIndeksZmiennej]);
	m_ctlOkresTelemetrii.SetItemText(m_nIndeksZmiennej, 1, strNapis);
	m_bZmieniono = TRUE;
	UpdateData(FALSE);
}


