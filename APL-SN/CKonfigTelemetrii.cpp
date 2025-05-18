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

	// TODO: Dodaj tutaj swój kod procedury obsługi komunikatów
	m_ctlOkresTelemetrii.InsertString(0, L"100 Hz");
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
	m_ctlOkresTelemetrii.InsertString(12, L"Wyłączony");
}
