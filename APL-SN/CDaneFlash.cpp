﻿// CDaneFlash.cpp: plik implementacji
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
	getKomunikacja().m_chAdresAutopilota = 2;
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
	ON_BN_CLICKED(IDC_BUT_CZYTAJ_FLASH, &CDaneFlash::OnBnClickedButCzytajFlash)
	ON_BN_CLICKED(IDC_BUTTON1, &CDaneFlash::OnBnClickedButton1)
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
	char chBuforPliku[30];
	int16_t sSampel[2];

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
		MessageBoxW(_T("Nie mogę odczytać pliku komunikatów: komunikaty_pl.txt!"), _T("Ojojoj!"), MB_ICONEXCLAMATION);
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
		m_vPamiecKomunikatow.push_back(0xADAD);	//adres
		m_vPamiecKomunikatow.push_back(n);
		m_vPamiecKomunikatow.push_back(0xCDAB);	//rozmiar
		m_vPamiecKomunikatow.push_back(n);
	}
	

	nr_sampla = 0;
	nErrCnt = 0;
	while (!feof(pPlikKomunikatow))
	{
		n = fwscanf_s(pPlikKomunikatow, _T("%s"), chNazwaSampla, (uint32_t)_countof(chNazwaSampla));
		//n = fscanf_s(pPlikKomunikatow, "%s", chNazwaSampla, _countof(chNazwaSampla));
		//n = fwscanf_s(pPlikKomunikatow, _T("%s"), chNazwaSampla);
		if (n > 0)
		{
			Error = _wfopen_s(&pPlikSampla, chNazwaSampla, _T("rb"));
			if (Error != NULL)
			{
				strNapis.Format(L"Nie mogę odczytać pliku %s", chNazwaSampla);
				MessageBoxW(strNapis, _T("Ojojoj!"), MB_ICONEXCLAMATION);
				break;
			}

			//odczytaj nagłówek pliku do unii bufora i struktury nagłówka
			//fgets((char*)m_uNaglowekWav.chBufor, 44+1, pPlikSampla);
			fread((char*)m_uNaglowekWav.chBufor, 44, 1, pPlikSampla);
			
			//walidacja napisów RIFF i WAVE
			if ((m_uNaglowekWav.strPlikWav.chRiff[0] != 'R') || (m_uNaglowekWav.strPlikWav.chWave[0] != 'W'))
			{
				strNapis.Format(L"Nie rozpoznaję formatu pliku %s", chNazwaSampla);
				MessageBoxW(strNapis, _T("Ojojoj!"), MB_ICONEXCLAMATION);
				break;
			}

			//walidacja formatu danych: jeden kanał 16kHz
			if ((m_uNaglowekWav.strPlikWav.nCzestProbkowania != 16000) || (m_uNaglowekWav.strPlikWav.sLiczbaKanalow != 1))
			{
				strNapis.Format(L"Plik %s musi mieć 1 kanał i częstotliwość próbkowania 16kHz", chNazwaSampla);
				MessageBoxW(strNapis, _T("Ojojoj!"), MB_ICONEXCLAMATION);
				break;	
			}

			//walidacja kompresji
			if (m_uNaglowekWav.strPlikWav.sTypFormatu != 1)
			{
				strNapis.Format(L"Dane nie mogą być skompresowane: %s!", chNazwaSampla);
				MessageBoxW(strNapis, _T("Ojojoj!"), MB_ICONEXCLAMATION);
				break;
			}
			nRozmiarSampla = m_uNaglowekWav.strPlikWav.nRozmiarPliku - 36;

			//wczytaj z pliku "extra format bytes" z pola nLiczbaDanych
			if (m_uNaglowekWav.strPlikWav.nRozmiarFormatuDanych < sizeof(chBuforPliku))
				fread(chBuforPliku, m_uNaglowekWav.strPlikWav.nRozmiarFormatuDanych, 1, pPlikSampla);
			else
			{
				strNapis.Format(L"Nieoczekiwany formatu pliku %s!", chNazwaSampla);
				MessageBoxW(strNapis, _T("Ojojoj!"), MB_ICONEXCLAMATION);
				break;
			}

			//odczytaj ostatni "chunk" = "data" i nastepujący po nim rozmiar danych sampla
			fread(chBuforPliku, 8, 1, pPlikSampla);
			if ((chBuforPliku[0] == 'd') && (chBuforPliku[1] == 'a') && (chBuforPliku[2] == 't') && (chBuforPliku[3] == 'a'))
			{
				for (int n = 0; n < 4; n++)
					getKomunikacja().m_unia8_32.dane8[n] = chBuforPliku[n + 4];
				nRozmiarSampla = getKomunikacja().m_unia8_32.dane32;	//to jest poprawny rozmiar sampla w bajtach
			}

			//odczytaj resztę pliku i przepisz do wektora
			for (uint32_t n = 0; n < nRozmiarSampla / 2; n++)
			{
				fread((char*)sSampel, 2, 1, pPlikSampla);	
				m_vPamiecKomunikatow.push_back(sSampel[0]);
			}
			fclose(pPlikSampla);
			
			//dodaj adres i długość sampla do spisu komunikatów
			getKomunikacja().m_unia8_32.dane32 = nAdresPoczatkuSampla;
			m_vPamiecKomunikatow[nr_sampla * 4 + 0] = getKomunikacja().m_unia8_32.dane16[0];
			m_vPamiecKomunikatow[nr_sampla * 4 + 1] = getKomunikacja().m_unia8_32.dane16[1];
			getKomunikacja().m_unia8_32.dane32 = nRozmiarSampla;
			m_vPamiecKomunikatow[nr_sampla * 4 + 2] = getKomunikacja().m_unia8_32.dane16[0];
			m_vPamiecKomunikatow[nr_sampla * 4 + 3] = getKomunikacja().m_unia8_32.dane16[1];

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
	GetDlgItem(IDC_BUT_ZAPISZ_FLASH)->EnableWindow(TRUE);	//włącz przycisk zapisu flash
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Zapisuje liste komunikatów do pamięci flash
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CDaneFlash::OnBnClickedButZapiszFlash()
{
#define ROZMIAR_PACZKI	200	//W ramce oprócz danych idzie jeszcze 4 bajty adresu i 1 bajt rozmiaru danych, więc wysyłaj odpowiednio mniej danych aby nie przepełnić ramki max 256 bajtów
	CString strNapis;
	int32_t nIloscSlowDoWyslania = (int32_t)m_vPamiecKomunikatow.size();	//ilość słów
	uint32_t nAdresFlash = (uint32_t)ADRES_POCZATKU_KOMUNIKATOW;
	uint16_t sAdresBufora = 0;
	uint8_t chErr;
	uint8_t chRozmarWysylanychDanych;
	uint8_t chPaczka[ROZMIAR_PACZKI];
	int32_t nIloscWyslanychSlow = 0;

	//dane wejsciowe są liczbami 16-bitowymi, więc nIloscSlowDoWyslania jest liczbą zbyt dużą więc podziel przez 256
	m_ctlPasekPostepu.SetRange(0, (uint16_t)((nIloscSlowDoWyslania >>8) -1));
	GetDlgItem(IDC_PROGRESS1)->EnableWindow(TRUE);

	do
	{		
		//sprawdź czy czy są jeszcze dane do wysłania
		if ((nIloscSlowDoWyslania - nIloscWyslanychSlow) < ROZMIAR_PACZKI / 2)
			chRozmarWysylanychDanych = 2*(nIloscSlowDoWyslania - nIloscWyslanychSlow);
		else
			chRozmarWysylanychDanych = ROZMIAR_PACZKI;

		//sprawdź czy jest jeszcze miejsce w buforze
		if (sAdresBufora + chRozmarWysylanychDanych > ROZMIAR_BUFORA_FLASH)
			chRozmarWysylanychDanych = ROZMIAR_BUFORA_FLASH - sAdresBufora;

		//przepisz słowa z wektora na bajty do wysłania ramką
		for (uint8_t n = 0; n < chRozmarWysylanychDanych/2; n++)
		{
			getKomunikacja().m_unia8_32.dane16[0] = m_vPamiecKomunikatow[n + nIloscWyslanychSlow];
			chPaczka[2 * n + 0] = getKomunikacja().m_unia8_32.dane8[0];
			chPaczka[2 * n + 1] = getKomunikacja().m_unia8_32.dane8[1];
		}

		chErr = getKomunikacja().ZapiszBuforFlash(sAdresBufora, chPaczka, chRozmarWysylanychDanych);
		if (chErr != ERR_OK)		
		{
			strNapis.Format(L"Wystąpił błąd wysyłania polecenia nr %d", chErr);
			MessageBoxW(strNapis, _T("Ojojoj!"), MB_ICONEXCLAMATION);
			m_ctlPasekPostepu.SetPos(0);
			return;
		}			
		sAdresBufora += chRozmarWysylanychDanych;		
		nIloscWyslanychSlow += chRozmarWysylanychDanych / 2;

		m_ctlPasekPostepu.SetPos((uint16_t)(nIloscWyslanychSlow>>8)+1);

		//jezeli pełen bufor lub koniec danych to zapisz bufor do flash
		if ((sAdresBufora == ROZMIAR_BUFORA_FLASH) || (nIloscWyslanychSlow == nIloscSlowDoWyslania))
		{
			chErr = getKomunikacja().ZapiszFlash(nAdresFlash);	//Adresem jest początek obszaru
			if (chErr != ERR_OK)
			{
				strNapis.Format(L"Wystąpił błąd  nr %d zapisu sektora flash", chErr);
				MessageBoxW(strNapis, _T("Ojojoj!"), MB_ICONEXCLAMATION);
				m_ctlPasekPostepu.SetPos(0);
				GetDlgItem(IDC_PROGRESS1)->EnableWindow(FALSE);
				return;
			}
			nAdresFlash += sAdresBufora;
			sAdresBufora = 0;	//zacznij napełnianie kolejnego bufora				
		}
	} while (nIloscWyslanychSlow < nIloscSlowDoWyslania);
	return;
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
		chErr = getKomunikacja().SkasujSektorFlash(ADRES_POCZATKU_KOMUNIKATOW + n * ROZMIAR_SEKTORA_FLASH);
		if (chErr == ERR_OK)
		{			
			m_ctlPasekPostepu.SetPos(n+1);
			UpdateData(FALSE);
		}
		else
		{
			strNapis.Format(L"Wystąpił błąd wysyłania polecenia nr %d", chErr);
			MessageBoxW(strNapis, _T("Ojojoj!"), MB_ICONEXCLAMATION);
			m_ctlPasekPostepu.SetPos(0);
			return;
		}
	}
	m_ctlPasekPostepu.SetPos(0);
	UpdateData(FALSE);
}




void CDaneFlash::OnBnClickedButCzytajFlash()
{
#define ROZMIAR_BUF_SEKT	64
	FILE* pPlikSektora; 
	long Error;
	uint8_t chErr;
	CString strNapis;

	uint16_t sBufor[ROZMIAR_BUF_SEKT];

	Error = _wfopen_s(&pPlikSektora, L"sektor.bin", L"wb");
	if (!Error)
	{
		m_ctlPasekPostepu.SetRange(0, (ROZMIAR_SEKTORA16_FLASH / ROZMIAR_BUF_SEKT) - 1);
		for (uint32_t n = 0; n < ROZMIAR_SEKTORA16_FLASH / ROZMIAR_BUF_SEKT; n++)
		{
			chErr = getKomunikacja().CzytajFlash(ADRES_POCZATKU_KOMUNIKATOW + (n * ROZMIAR_BUF_SEKT), sBufor, ROZMIAR_BUF_SEKT);
			if (chErr == ERR_OK)
			{
				fwrite(sBufor, sizeof(uint16_t), ROZMIAR_BUF_SEKT, pPlikSektora);
				m_ctlPasekPostepu.SetPos(n+1);
				UpdateData(FALSE);
			}
			else
			{
				strNapis.Format(L"Wystąpił błąd nr %d wysyłania polecenia", chErr);
				MessageBoxW(strNapis, _T("Ojojoj!"), MB_ICONEXCLAMATION);
				m_ctlPasekPostepu.SetPos(0);
				return;
			}
		}
		fclose(pPlikSektora);
	}
	m_ctlPasekPostepu.SetPos(0);
	UpdateData(FALSE);
}


BOOL CDaneFlash::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Dodaj tutaj dodatkową inicjację
	GetDlgItem(IDC_PROGRESS1)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUT_ZAPISZ_FLASH)->EnableWindow(FALSE);
	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // WYJĄTEK: Strona właściwości OCX powinna zwrócić FALSE
}



//generuje syntetyczny plik wav do testów poprawnosci transferu
void CDaneFlash::OnBnClickedButton1()
{
	FILE* pPlikWav;
	long Error;
	//char chBufor[78] = { "RIFFfk  WAVEfmt 10001010830007002010LIST4676INFOISFTe000Lavf58.29.1000data k"};
	uint8_t chBufor[78] = { "RIFFfk  WAVEfmt 10001010830007002010LIST4676INFOISFTe000Lavf58.29.1000data k" };
	CString strNapis;
	uint16_t sBufor;

	Error = _wfopen_s(&pPlikWav, L"test.wav.bin", L"wb");
	if (!Error && pPlikWav)
	{
		//pierwszy wiersz
		chBufor[6] = 1;
		chBufor[7] = 1;

		//2 wiersz
		chBufor[16] = 0x10;
		chBufor[17] = 0x00;
		chBufor[18] = 0x00;
		chBufor[19] = 0x00;
		chBufor[20] = 0x01;
		chBufor[21] = 0x00;
		chBufor[22] = 0x01;
		chBufor[23] = 0x00;

		chBufor[24] = 0x80;
		chBufor[25] = 0x3E;
		chBufor[26] = 0x00;
		chBufor[27] = 0x00;
		chBufor[28] = 0x00;
		chBufor[29] = 0x7D;
		chBufor[30] = 0x00;
		chBufor[31] = 0x00;

		//3 wiersz
		chBufor[32] = 0x02;
		chBufor[33] = 0x00;
		chBufor[34] = 0x10;
		chBufor[35] = 0x00;

		chBufor[40] = 0x1A;
		chBufor[41] = 0x00;
		chBufor[42] = 0x00;
		chBufor[43] = 0x00;

		//4 wiersz
		chBufor[52] = 0x0E;
		chBufor[53] = 0x00;
		chBufor[54] = 0x00;
		chBufor[55] = 0x00;

		//5 wiersz
		chBufor[67] = 0x00;
		chBufor[76] = 0x01;
		chBufor[77] = 0x00;

		fwrite(chBufor, sizeof(uint8_t), 78, pPlikWav);

		for (uint32_t n = 0; n < ROZMIAR_SEKTORA16_FLASH; n++)
		{
			sBufor = n & 0xFFFF;
			fwrite(&sBufor, sizeof(uint16_t), 1, pPlikWav);
		}
		fclose(pPlikWav);
	}

}
