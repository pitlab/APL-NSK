// CKonfigTelemetrii.cpp: plik implementacji
//

#include "pch.h"
#include "APL-SN.h"
#include "CKonfigTelemetrii.h"
#include "afxdialogex.h"
#include "APL-SNDoc.h"
#include "komunikacja/Komunikacja.h"

// Okno dialogowe CKonfigTelemetrii

IMPLEMENT_DYNAMIC(CKonfigTelemetrii, CDialogEx)

CKonfigTelemetrii::CKonfigTelemetrii(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KONFIG_TELEMETRII, pParent)
	, m_bZmieniono(FALSE)
	, m_chIdZmiennej(0)
	, m_nOkres(0)
	, m_strZajetosc(_T(""))
	, m_nIndeksZmiennej(0)
	, m_nIndeksOkresu(0)
	, m_nIndeksDronaWRoju(0)
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
	DDX_Text(pDX, IDC_STATIC_ZAJETOSC, m_strZajetosc);
	DDX_Control(pDX, IDC_PROGRESS_ZAJETOSC, m_ctrlZajetosc);
}


BEGIN_MESSAGE_MAP(CKonfigTelemetrii, CDialogEx)
	ON_BN_CLICKED(IDOK, &CKonfigTelemetrii::OnBnClickedOk)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ZMIENNE_TELE, &CKonfigTelemetrii::OnLvnItemchangedListZmienneTele)
	ON_LBN_SELCHANGE(IDC_LISTA_CZESTOTLIWOSCI, &CKonfigTelemetrii::OnLbnSelchangeListaCzestotliwosci)
END_MESSAGE_MAP()



// Procedury obsługi komunikatów CKonfigTelemetrii


///////////////////////////////////////////////////////////////////////////////////////////////////
// Buduje zawartość okna konfiguracji telemetrii
// Parametry: brak
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CKonfigTelemetrii::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	int nPozycja, nPoprzedniaPoz = -1;
	int nMiejsceNaLiscie = 0;
	uint16_t sOkres;
	uint8_t chErr = 1;
	CString strNapis;
	float fCzestotliwosc;

	//Zbuduj listę częstotliwości
	nPozycja = m_ctlListaCzestotliwosciTelemetrii.GetCount();	//liczba pozycji na liście
	if (nPozycja == 0)
	{
		//jeżeli nie ma pozycji to je wstaw
		for (int n = 1; n < MAX_OKRES_TELEMETRII+2; n++)
		{
			nPozycja = PozycjaDlaOkresu(n, &sOkres);
			if (nPoprzedniaPoz != nPozycja)
			{
				if (sOkres < TEMETETRIA_WYLACZONA)
					strNapis.Format(_T("%.2f Hz (%.2f s)"), MAX_CZESTOTLIWOSC_TELEMETRII / sOkres, (float)sOkres / MAX_CZESTOTLIWOSC_TELEMETRII);
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
		if (getKomunikacja().m_cRoj.vWron.size())
		{
			m_sOkresTelemetrii[n] = getKomunikacja().m_cRoj.vWron[m_nIndeksDronaWRoju].m_sOkresTelemetrii[n];
			fCzestotliwosc = getKomunikacja().m_cRoj.vWron[m_nIndeksDronaWRoju].PobierzCzestotliwoscTelemetrii(n);
		}
		else
		{
			m_sOkresTelemetrii[n] = TEMETETRIA_WYLACZONA;
			fCzestotliwosc = 0;
		}

		m_ctlOkresTelemetrii.InsertItem(n, getKomunikacja().m_strNazwyZmiennychTele[n]);
		if (fCzestotliwosc)
			strNapis.Format(_T("%.2f Hz"), fCzestotliwosc);
		else
			strNapis.Format(_T("Wyłączone"));
		m_ctlOkresTelemetrii.SetItemText(n, 1, strNapis);
	}

	m_ctrlZajetosc.SetRange(0, 100);
	AktualizujZajetoscLacza();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // WYJĄTEK: Strona właściwości OCX powinna zwrócić FALSE
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Reakcja na naciśnięcie OK
// Parametry: brak
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CKonfigTelemetrii::OnBnClickedOk()
{
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
	if (m_bZmieniono)
	{
		//zapisz do roju oraz wyślij ramkę do wrona
		if (getKomunikacja().m_cRoj.vWron.size() == 0)
		{
			CDialogEx::OnCancel();
			return;
		}
		
		for (int n = 0; n < LICZBA_ZMIENNYCH_TELEMETRYCZNYCH; n++)
			getKomunikacja().m_cRoj.vWron[m_nIndeksDronaWRoju].m_sOkresTelemetrii[n] = m_sOkresTelemetrii[n];

		uint8_t chErr = getKomunikacja().ZapiszOkresTelemetrii(m_sOkresTelemetrii, LICZBA_ZMIENNYCH_TELEMETRYCZNYCH);
		if (chErr)
		{
			CString strKomunikat;
			strKomunikat.Format(_T("Nie można zapisać danych do APL3! \nKod błędu: %d"), chErr);
			MessageBoxExW(this->m_hWnd, strKomunikat, _T("Ojojojoj!"), MB_ICONEXCLAMATION, 0);
			CDialogEx::OnCancel();
			return;
		}
	}
	CDialogEx::OnOK();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Oblicza numer pozycji na liście zmiennych dla podanego okresu
// Parametry: 
//  chOkres - wartość przeładowania licznika okresu odczytana z APL
//	*sZaokraglonyOkres - wskaźnik na zwracaną wartość okresu po zaokrągleniu dla eleganckiego wyświetlania
// zwraca: Obliczony numer pozycji na liście
///////////////////////////////////////////////////////////////////////////////////////////////////
int CKonfigTelemetrii::PozycjaDlaOkresu(int nOkres, uint16_t* sZaokraglonyOkres)
{
	if (nOkres == 0)
	{
		*sZaokraglonyOkres = TEMETETRIA_WYLACZONA;
		return 12;
	}
	if (nOkres <= 5)
	{
		*sZaokraglonyOkres = (uint16_t)nOkres;
		return nOkres - 1;
	}
	if (nOkres <= 7)	//14,2Hz
	{
		*sZaokraglonyOkres = 7;
		return 5;	
	}
	if (nOkres <= 10)	//10Hz
	{
		*sZaokraglonyOkres = 10;
		return 6;	
	}
	if (nOkres <= 20)	//5Hz
	{
		*sZaokraglonyOkres = 20;
		return 7;	
	}
	if (nOkres <= 50)	//2Hz
	{
		*sZaokraglonyOkres = 50;
		return 8;	
	}
	if (nOkres <= 100)	//1Hz
	{
		*sZaokraglonyOkres = 100;
		return 9;	
	}
	if (nOkres <= 200)	//0,5Hz -> 1/2
	{
		*sZaokraglonyOkres = 200;
		return 10;	
	}
	if (nOkres <= 500)	//0,2Hz -> 1/5
	{
		*sZaokraglonyOkres = 500;
		return 11;	
	}
	if (nOkres <= 1000)	//0,1Hz -> 1/10
	{
		*sZaokraglonyOkres = 1000;
		return 12;
	}
	if (nOkres <= 2000)	//0,05Hz -> 1/20
	{
		*sZaokraglonyOkres = 2000;
		return 13;
	}
	if (nOkres <= MAX_OKRES_TELEMETRII)	//0,02Hz -> 1/50
	{
		*sZaokraglonyOkres = MAX_OKRES_TELEMETRII;
		return 14;
	}
	else               //wyłączone
	{
		*sZaokraglonyOkres = TEMETETRIA_WYLACZONA;
		return 15;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Zwraca wartość okresu na podstawie pozycji na liście częstotliwosci 
// Parametry: 
//  nPozycja - pozycja na liście
// zwraca: Obliczony okres
///////////////////////////////////////////////////////////////////////////////////////////////////
uint16_t CKonfigTelemetrii::OkresDlaPozycji(int nPozycja)
{
	uint16_t sOkres = TEMETETRIA_WYLACZONA;
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
	case 9: sOkres = 100;	break;	//1s
	case 10: sOkres = 200;	break;	//2s
	case 11: sOkres = 500;	break;	//5s
	case 12: sOkres = 1000;	break;	//10s
	case 13: sOkres = 2000;	break;	//20s
	case 14: sOkres = 5000;	break;	//50s
	case 15: 
	default: sOkres = TEMETETRIA_WYLACZONA;	break;
	}
	return sOkres;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Kliknięto na listę zmiennych. Pobierz bieżący okres i podświetl na liście częstotliwości
// Parametry: 
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CKonfigTelemetrii::OnLvnItemchangedListZmienneTele(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	uint16_t sZaokraglonyokres;
	uint8_t chPozycjaCzestotliwosci;
	
	m_nIndeksZmiennej = pNMLV->iItem;	//zapamiętaj indeks modyfikowanej zmiennej

	chPozycjaCzestotliwosci = PozycjaDlaOkresu(m_sOkresTelemetrii[m_nIndeksZmiennej], &sZaokraglonyokres);
	m_ctlListaCzestotliwosciTelemetrii.SetCurSel(chPozycjaCzestotliwosci);
	UpdateData(FALSE);	
	*pResult = 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Kliknięto na liste częstotliwości
// Parametry: 
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CKonfigTelemetrii::OnLbnSelchangeListaCzestotliwosci()
{
	CString strNapis;

	m_nIndeksOkresu = m_ctlListaCzestotliwosciTelemetrii.GetCurSel();
	m_sOkresTelemetrii[m_nIndeksZmiennej] = OkresDlaPozycji(m_nIndeksOkresu);
	if (m_sOkresTelemetrii[m_nIndeksZmiennej] == TEMETETRIA_WYLACZONA)
		strNapis.Format(_T("Wyłączone"));
	else
		strNapis.Format(_T("%.2f Hz"), MAX_CZESTOTLIWOSC_TELEMETRII / m_sOkresTelemetrii[m_nIndeksZmiennej]);
	m_ctlOkresTelemetrii.SetItemText(m_nIndeksZmiennej, 1, strNapis);
	m_bZmieniono = TRUE;
	AktualizujZajetoscLacza();
	
	UpdateData(FALSE);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Oblicza zajetość łącza bieżącą telemetrią
// Parametry: 
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CKonfigTelemetrii::AktualizujZajetoscLacza()
{
	struct
	{
		int nLiczbaZmiennych;
		int nOkres;
	} stOkresy[ILOSC_OKRESOW];
	uint16_t sOkresZaokr;
	int nOkres, nIndeks;
	int nLiczbaBitowNaSek;
	int nLiczbaBitow;

	for (int n = 0; n < ILOSC_OKRESOW; n++)
		stOkresy[n].nLiczbaZmiennych = 0;


	//zbierz liczbę zmienych wysyłanych we wszystkich okresach
	for (int n = 0; n < LICZBA_ZMIENNYCH_TELEMETRYCZNYCH; n++)
	{
		nOkres = m_sOkresTelemetrii[n];
		nIndeks = PozycjaDlaOkresu(nOkres, &sOkresZaokr);
		if (nIndeks < ILOSC_OKRESOW)
		{
			stOkresy[nIndeks].nLiczbaZmiennych++;
			stOkresy[nIndeks].nOkres = nOkres;
		}
	}

	//policz ile bitów ma być wysyłane na sekundę
	nLiczbaBitowNaSek = 0;
	for (int n = 0; n < ILOSC_OKRESOW; n++)
	{
		if ((stOkresy[n].nLiczbaZmiennych) && (stOkresy[n].nOkres))
		{
			nLiczbaBitow = 10 * (ROZM_CIALA_RAMKI + LICZBA_BAJTOW_ID_TELEMETRII + 2 * stOkresy[n].nLiczbaZmiennych);
			nLiczbaBitowNaSek += (int)(nLiczbaBitow / (KWANT_CZASU_TELEMETRII * stOkresy[n].nOkres));
		}
	}

	m_strZajetosc.Format(_T("Zajętość: %.1f Kb/s"), nLiczbaBitowNaSek/1024.f);
	float fPos = nLiczbaBitowNaSek / 115200.0f;
	m_ctrlZajetosc.SetPos((int)(fPos* 100));
	UpdateData(FALSE);
}