// CKonfigTelemetrii.cpp: plik implementacji
//

#include "pch.h"
#include "APL-SN.h"
#include "CKonfigTelemetrii.h"
#include "afxdialogex.h"


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
	CString strNapis;
	

	// TODO: Dodaj tutaj swój kod procedury obsługi komunikatów
	nPozycja = m_ctlOkresTelemetrii.GetCount();	//liczba pozycji na liście
	if (nPozycja == 0)
	{
		//nie ma komunikatów to je wstaw
		for (int n = 0; n < 251; n++)
		{
			nPozycja = PozycjaDlaOkresu(n);
			if (nPoprzedniaPoz != nPozycja)
			{
				strNapis.Format(_T("%.1f Hz"), 100.0f / (n+1));
				m_ctlOkresTelemetrii.InsertString(nPozycja, strNapis);
				nPoprzedniaPoz = nPozycja;
			}
		}
		/*m_ctlOkresTelemetrii.InsertString(0, L"100 Hz");
		m_ctlOkresTelemetrii.InsertString(1, L"50 Hz");
		m_ctlOkresTelemetrii.InsertString(2, L"33,3 Hz");
		m_ctlOkresTelemetrii.InsertString(3, L"25 Hz");
		m_ctlOkresTelemetrii.InsertString(4, L"20 Hz");
		m_ctlOkresTelemetrii.InsertString(5, L"14,3 Hz");
		m_ctlOkresTelemetrii.InsertString(6, L"10 Hz");
		m_ctlOkresTelemetrii.InsertString(7, L"5 Hz");
		m_ctlOkresTelemetrii.InsertString(8, L"2 Hz");
		m_ctlOkresTelemetrii.InsertString(9, L"1 Hz");
		m_ctlOkresTelemetrii.InsertString(10, L"0,5 Hz");
		m_ctlOkresTelemetrii.InsertString(11, L"0,4 Hz");
		m_ctlOkresTelemetrii.InsertString(12, L"Wyłączony");*/
	}
	m_ctlOkresTelemetrii.SetCurSel(5);
	//m_ctlOkresTelemetrii.SelectString(0, )
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
int CKonfigTelemetrii::PozycjaDlaOkresu(uint8_t chOkres)
{
	if (chOkres < 5)
		return chOkres;
	if (chOkres < 7)
		return 6;	//14,2Hz
	if (chOkres < 10)
		return 9;	//10Hz
	if (chOkres < 20)
		return 19;	//5Hz
	if (chOkres < 50)
		return 49;	//2Hz
	if (chOkres < 100)
		return 99;	//1Hz
	if (chOkres < 200)
		return 199;	//0,5Hz
	if (chOkres < 250)
		return 249;	//0,4Hz
}