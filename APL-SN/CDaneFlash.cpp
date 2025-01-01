// CDaneFlash.cpp: plik implementacji
// Zapisuje listę komunikatów audio do pamięci Flash począwszy od adresu 0x68000000 do 0x6801FFFF
// Na poczatku pobszaru jest spis treści zawierający 32-bitowy adres i 32-bitową długość komunikatu
// Wstępnie ustalam długość spisu treści na 256 próbek

#include "pch.h"
#include "APL-SN.h"
#include "CDaneFlash.h"
#include "afxdialogex.h"
#include "errors.h"


// Okno dialogowe CDaneFlash

IMPLEMENT_DYNAMIC(CDaneFlash, CDialogEx)

CDaneFlash::CDaneFlash(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ZAPISZ_FLASH, pParent)
{
	cKomunikacja.m_chAdresAutopilota = 2;
}

CDaneFlash::~CDaneFlash()
{
}

void CDaneFlash::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTA_KOMUNIKATOW, m_ctlListaKomunikatow);
	DDX_Control(pDX, IDC_PROGRESS1, m_ctlPasekPostepu);
}


BEGIN_MESSAGE_MAP(CDaneFlash, CDialogEx)
	ON_BN_CLICKED(IDC_BUT_CZYTAJ_PLIK, &CDaneFlash::OnBnClickedButCzytajPlik)
	ON_BN_CLICKED(IDC_BUT_ZAPISZ_FLASH, &CDaneFlash::OnBnClickedButZapiszFlash)
	ON_BN_CLICKED(IDC_BUT_KASUJ_FLASH, &CDaneFlash::OnBnClickedButKasujFlash)
END_MESSAGE_MAP()


// Procedury obsługi komunikatów CDaneFlash

///////////////////////////////////////////////////////////////////////////////////////////////////
// Oczytuje plik i umieszcza go na liście
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CDaneFlash::OnBnClickedButCzytajPlik()
{
	OPENFILENAME ofn;
	wchar_t szFile[_MAX_PATH];
	FILE* pPlikKomunikatow;
	FILE* pPlikSampla;
	long Error;
	HKEY hKey;
	DWORD dwResult;
	uint32_t nAdresPoczatkuSampla = ADRES_POCZATKU_KOMUNIKATOW + ROZMIAR_SPISU_KOMUNIKATOW * 8;
	uint32_t nRozmiarSampla;
	CString strNapis;


	szFile[0] = '\0';
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = this->m_hWnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = _T("Lista komunikatów głosowych\0komunikaty*.txt\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST;
	ofn.lpstrDefExt = _T("txt");
	ofn.lpstrTitle = _T("Wskaż plik z listą komunikatów głosowych");

	//otwórz dialog i wskaż plik z językiem
	if (GetOpenFileName(&ofn) != TRUE)
		return;

	// zapisz scieżkę odczytanego pliku do rejestru
	Error = RegCreateKeyExW(HKEY_CURRENT_USER, _T("SOFTWARE\\PitLab\\APL-SN\\Settings"), 0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwResult);
	if (Error == ERROR_SUCCESS)
	{
		Error = RegSetValueExW(hKey, _T("PlikKomunikatow"), 0, REG_SZ, (LPBYTE)szFile, sizeof(szFile));
		RegCloseKey(hKey);
	}

	Error = _wfopen_s(&pPlikKomunikatow, szFile, _T("r"));
	if (Error != NULL)
	{
		MessageBoxW(_T("Nie mogę odczytać pliku komunikatów: komunikaty_pl.txt!"), _T("Błąd!"), MB_ICONEXCLAMATION);
		return;
	}


	int n, nr_sampla, nErrCnt;
	wchar_t chNazwaSampla[250];
	wchar_t chNumerIndeksu[4];
	wchar_t chAdresSampla[12];
	wchar_t chDlugoscSampla[9];

	m_ctlListaKomunikatow.InsertColumn(0, L"Komunikat");
	m_ctlListaKomunikatow.SetColumnWidth(0, 150);

	m_ctlListaKomunikatow.InsertColumn(1, L"Adres ");
	m_ctlListaKomunikatow.SetColumnWidth(1, 100);

	m_ctlListaKomunikatow.InsertColumn(2, L"Rozmiar");
	m_ctlListaKomunikatow.SetColumnWidth(2, 80);

	//wypełnij zerami spis komunikatów w wektorze, tak aby można było dopisywać na jego końcu kolejne dane
	m_vPamiecKomunikatow.clear();
	for (uint16_t n = 0; n < ROZMIAR_SPISU_KOMUNIKATOW; n++)
	{
		m_vPamiecKomunikatow.push_back(0xABCD);	//adres
		m_vPamiecKomunikatow.push_back(n);
		m_vPamiecKomunikatow.push_back(0xCDEF);	//rozmiar
		m_vPamiecKomunikatow.push_back(n);
	}
	

	nr_sampla = 0;
	nErrCnt = 0;
	while (!feof(pPlikKomunikatow))
	{
		n = fwscanf_s(pPlikKomunikatow, _T("%s"), chNazwaSampla, _countof(chNazwaSampla));
		//n = fwscanf_s(pPlikKomunikatow, _T("%s"), chNazwaSampla);
		if (n > 0)
		{
			Error = _wfopen_s(&pPlikSampla, chNazwaSampla, _T("rb"));
			if (Error != NULL)
			{
				strNapis.Format(L"Nie mogę odczytać pliku %s", chNazwaSampla);
				MessageBoxW(strNapis, _T("Błąd!"), MB_ICONEXCLAMATION);
				break;
			}

			//odczytaj nagłówek pliku do unii bufora i struktury nagłówka
			fgets((char*)m_uNaglowekWav.chBufor, 44, pPlikSampla);
			
			//walidacja napisów RIFF i WAVE
			if ((m_uNaglowekWav.strPlikWav.chRiff[0] != 'R') || (m_uNaglowekWav.strPlikWav.chWave[0] != 'W'))
			{
				strNapis.Format(L"Nie rozpoznaję formatu pliku %s", chNazwaSampla);
				MessageBoxW(strNapis, _T("Błąd!"), MB_ICONEXCLAMATION);
				break;
			}

			//walidacja formatu danych: jeden kanał 16kHz
			if ((m_uNaglowekWav.strPlikWav.nCzestProbkowania != 16000) || (m_uNaglowekWav.strPlikWav.sLiczbaKanalow != 1))
			{
				strNapis.Format(L"Plik %s musi mieć 1 kanał i częstotliwość próbkowania 16kHz", chNazwaSampla);
				MessageBoxW(strNapis, _T("Błąd!"), MB_ICONEXCLAMATION);
				break;	
			}
			nRozmiarSampla = m_uNaglowekWav.strPlikWav.nRozmiarPliku - 36;

			//odczytaj resztę pliku i przepisz do wektora
			for (uint32_t n = 0; n < nRozmiarSampla / 2; n++)
			{
				fgets((char*)cKomunikacja.m_unia8_16.dane8, 2, pPlikSampla);
				m_vPamiecKomunikatow.push_back(cKomunikacja.m_unia8_16.dane16);
			}
			fclose(pPlikSampla);
			
			//dodaj adres i długość sampla do spisu komunikatów
			cKomunikacja.m_unia8_32.dane32 = nAdresPoczatkuSampla;
			m_vPamiecKomunikatow[nr_sampla * 4 + 0] = cKomunikacja.m_unia8_32.dane16[0];
			m_vPamiecKomunikatow[nr_sampla * 4 + 1] = cKomunikacja.m_unia8_32.dane16[1];
			cKomunikacja.m_unia8_32.dane32 = nRozmiarSampla;
			m_vPamiecKomunikatow[nr_sampla * 4 + 2] = cKomunikacja.m_unia8_32.dane16[0];
			m_vPamiecKomunikatow[nr_sampla * 4 + 3] = cKomunikacja.m_unia8_32.dane16[1];

			//dodaj wpis do listy komunikatów
			swprintf_s(chNumerIndeksu, _T("%.2d"), nr_sampla);
			swprintf_s(chAdresSampla, _T("0x%.8X"), nAdresPoczatkuSampla);
			swprintf_s(chDlugoscSampla, _T("%d"), nRozmiarSampla);
			m_ctlListaKomunikatow.InsertItem(nr_sampla, chNumerIndeksu);
			m_ctlListaKomunikatow.SetItemText(nr_sampla, 0, chNazwaSampla);
			m_ctlListaKomunikatow.SetItemText(nr_sampla, 1, chAdresSampla);
			m_ctlListaKomunikatow.SetItemText(nr_sampla, 2, chDlugoscSampla);

			nr_sampla++;
			nAdresPoczatkuSampla += nRozmiarSampla;			
		}
	}
	fclose(pPlikKomunikatow);
	m_ctlListaKomunikatow.SetFocus();
	m_ctlListaKomunikatow.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	m_SpisKomunikatow[0].nAdres = ADRES_POCZATKU_KOMUNIKATOW + ROZMIAR_SPISU_KOMUNIKATOW * 8;

}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Zapisuje liste komunikatów do pamięci flash
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CDaneFlash::OnBnClickedButZapiszFlash()
{
#define ROZMIAR_PACZKI	128	//wyrażony w słowach
	CString strNapis;
	uint32_t nRozmiar = (uint32_t)(m_vPamiecKomunikatow.size() / ROZMIAR_PACZKI);
	uint32_t nAdres = ADRES_POCZATKU_KOMUNIKATOW;
	uint8_t chErr;	
	m_ctlPasekPostepu.SetRange(0, nRozmiar-1);

	//wysyłaj paczkami o rozmiarze ROZMIAR_PACZKI
	for (uint32_t n = 0; n < nRozmiar; n++)
	{
		chErr = cKomunikacja.ZapiszFlash(nAdres, (uint16_t*)&m_vPamiecKomunikatow[n* ROZMIAR_PACZKI], ROZMIAR_PACZKI);
		if (chErr == ERR_OK)
		{
			nAdres += ROZMIAR_PACZKI;
			nRozmiar--;
			m_ctlPasekPostepu.SetPos(n);
		}
		else
		{
			strNapis.Format(L"Wystąpił błąd wysyłania polecenia nr %d", chErr);
			MessageBoxW(strNapis, _T("Błąd!"), MB_ICONEXCLAMATION);
			m_ctlPasekPostepu.SetPos(0);
			return;
		}	
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Kasuje sektory flasha 128kB w którym znajdują się komunikaty głosowe
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CDaneFlash::OnBnClickedButKasujFlash()
{
	CString strNapis;
	uint8_t chErr;

	m_ctlPasekPostepu.SetRange(0, LICZBA_SEKTOROW_KOMUNIKATOW-1);
	for (uint8_t n = 0; n < LICZBA_SEKTOROW_KOMUNIKATOW; n++)
	{
		chErr = cKomunikacja.SkasujSektorFlash(ADRES_POCZATKU_KOMUNIKATOW + n * 0x10000);
		if (chErr == ERR_OK)
		{			
			m_ctlPasekPostepu.SetPos(n);
			UpdateData(FALSE);
		}
		else
		{
			strNapis.Format(L"Wystąpił błąd wysyłania polecenia nr %d", chErr);
			MessageBoxW(strNapis, _T("Błąd!"), MB_ICONEXCLAMATION);
			m_ctlPasekPostepu.SetPos(0);
			return;
		}
	}
	m_ctlPasekPostepu.SetPos(0);
	UpdateData(FALSE);
}
