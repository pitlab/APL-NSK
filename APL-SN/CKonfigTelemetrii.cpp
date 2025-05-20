// CKonfigTelemetrii.cpp: plik implementacji
//

#include "pch.h"
#include "APL-SN.h"
#include "CKonfigTelemetrii.h"
#include "afxdialogex.h"
#include "definicje_telemetrii.h"

// Okno dialogowe CKonfigTelemetrii

IMPLEMENT_DYNAMIC(CKonfigTelemetrii, CDialogEx)

CKonfigTelemetrii::CKonfigTelemetrii(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TELEMETRIA, pParent)
{

}

CKonfigTelemetrii::~CKonfigTelemetrii()
{
}

void CKonfigTelemetrii::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OKRES_TELEMETRII, m_ctlOkresTelemetrii);
}


BEGIN_MESSAGE_MAP(CKonfigTelemetrii, CDialogEx)
	ON_BN_CLICKED(IDOK, &CKonfigTelemetrii::OnBnClickedOk)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_LBN_SELCHANGE(IDC_OKRES_TELEMETRII, &CKonfigTelemetrii::OnLbnSelchangeOkresTelemetrii)
END_MESSAGE_MAP()


// Procedury obsługi komunikatów CKonfigTelemetrii


void CKonfigTelemetrii::OnBnClickedOk()
{
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
	CString strNapis;

	int nMiejsceNaLiscie = m_ctlOkresTelemetrii.GetCurSel();
	m_ctlOkresTelemetrii.GetText(nMiejsceNaLiscie, strNapis);
	double dCzestotliwosc = _ttof(strNapis);	//wyciagnij częstotliwość z napisu
	m_nOkres = (int)(KWANT_CZASU_TELEMETRII * 10000 / dCzestotliwosc) - 1;
	CDialogEx::OnOK();
}


int CKonfigTelemetrii::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Dodaj tutaj swój wyspecjalizowany kod tworzenia

	return 0;
}

void CKonfigTelemetrii::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)

{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);
	int nPozycja, nPoprzedniaPoz = -1;
	int nMiejsceNaLiscie = 0;
	uint8_t chOkres;
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
					strNapis.Format(_T("%.1f Hz"), 100.0f / (chOkres +1));
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
}

	


void CKonfigTelemetrii::OnLbnSelchangeOkresTelemetrii()
{
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
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
///  Umożliwia pobranie zaznaczonego okresu telemetrii
/// </summary>
/// <param name=""></param>
/// <returns></returns>
uint8_t CKonfigTelemetrii::PobierzOkresTelem(void)
{
	return (uint8_t)m_nOkres;
}