// CDaneFlash.cpp: plik implementacji
//

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

}

CDaneFlash::~CDaneFlash()
{
}

void CDaneFlash::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTA_KOMUNIKATOW, m_ctlListaKomunikatow);
}


BEGIN_MESSAGE_MAP(CDaneFlash, CDialogEx)
	ON_BN_CLICKED(IDC_BUT_CZYTAJ_PLIK, &CDaneFlash::OnBnClickedButCzytajPlik)
END_MESSAGE_MAP()


// Procedury obsługi komunikatów CDaneFlash

///////////////////////////////////////////////////////////////////////////////////////////////////
// Oczytuje plik i umieszcza go na liście
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CDaneFlash::OnBnClickedButCzytajPlik()
{
	//FILE* plik;

	OPENFILENAME ofn;
	wchar_t szFile[_MAX_PATH];
	FILE* PlikKomunikatow;
	long Error;
	HKEY hKey;
	DWORD dwResult;

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

	/*ofn.lpstrFilter = _T("Komunikaty głosowe\0komunikaty*.wav\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST;
	ofn.lpstrDefExt = _T("wav");
	ofn.lpstrTitle = _T("Dodaj plik z komunikatem głosowym");*/

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

	//otwórz dialog i wskaż plik z językiem
	/*if (GetOpenFileName(&ofn) == TRUE)
	{
		for (int x = 0; x < _MAX_PATH; x++)
		{
			szFile[x] = *ofn.lpstrFile++;
		}

		//m_strPath = szFile;
		UpdateData(FALSE);
	}
	else
		MessageBoxW(_T("Nie mogę znaleźć pliku komunikatów: komunikaty_pl.txt!"), _T("Błąd!"), MB_ICONEXCLAMATION);*/

	Error = _wfopen_s(&PlikKomunikatow, szFile, _T("r"));
	if (Error != NULL)
	{
		MessageBoxW(_T("Nie mogę odczytać pliku komunikatów: komunikaty_pl.txt!"), _T("Błąd!"), MB_ICONEXCLAMATION);
		return;
	}

	int x, n, nr_sampla, nErrCnt, nErr;
	unsigned short sAdres, sDlugosc;
	wchar_t chNazwaSampla[250];
	wchar_t chNumerIndeksu[4];
	wchar_t chAdresSampla[8];
	wchar_t chDlugoscSampla[9];
	wchar_t chWypelnienie[6];
	float fWyp;


	m_ctlListaKomunikatow.InsertColumn(0, L"Komunikat");
	m_ctlListaKomunikatow.SetColumnWidth(0, 140);

	m_ctlListaKomunikatow.InsertColumn(1, L"Adres ");
	m_ctlListaKomunikatow.SetColumnWidth(1, 60);

	m_ctlListaKomunikatow.InsertColumn(2, L"Rozmiar");
	m_ctlListaKomunikatow.SetColumnWidth(2, 60);

	m_ctlListaKomunikatow.InsertColumn(3, L"Wypełnienie");
	m_ctlListaKomunikatow.SetColumnWidth(3, 60);



	nr_sampla = 0;
	nErrCnt = 0;
	while (!feof(PlikKomunikatow))
	{
		n = fwscanf_s(PlikKomunikatow, _T("%s"), chNazwaSampla, _countof(chNazwaSampla));
		if (n > 0)
		{
			if (nErrCnt < 3)
			{
				//nErr = m_cNet.PobierzDaneSampla((unsigned char)nr_sampla, &sAdres, &sDlugosc);
				//if (Err != ERR_OK)
				{
					sAdres = 0;
					sDlugosc = 0;
					nErrCnt++;
				}
			}

			swprintf_s(chNumerIndeksu, _T("%.2d"), nr_sampla);
			swprintf_s(chAdresSampla, _T("0x%.3X"), sAdres);
			swprintf_s(chDlugoscSampla, _T("%d"), sDlugosc);
			x = sDlugosc / 512;
			fWyp = x + ((float)sDlugosc - (x * 512)) / 512;
			swprintf_s(chWypelnienie, _T("%.2f"), fWyp);
			
		
			m_ctlListaKomunikatow.InsertItem(nr_sampla, chNumerIndeksu);
			m_ctlListaKomunikatow.SetItemText(nr_sampla, 0, chNazwaSampla);
			m_ctlListaKomunikatow.SetItemText(nr_sampla, 1, chAdresSampla);
			m_ctlListaKomunikatow.SetItemText(nr_sampla, 2, chDlugoscSampla);
			m_ctlListaKomunikatow.SetItemText(nr_sampla, 3, chWypelnienie);

			nr_sampla++;
		}
	}
	fclose(PlikKomunikatow);
	m_ctlListaKomunikatow.SetFocus();
	m_ctlListaKomunikatow.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);

}
