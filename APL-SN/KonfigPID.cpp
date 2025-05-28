// KonfigPID.cpp: plik implementacji
//

#include "pch.h"
#include "APL-SN.h"
#include "KonfigPID.h"
#include "afxdialogex.h"
#include <math.h>

// Okno dialogowe KonfigPID

IMPLEMENT_DYNAMIC(KonfigPID, CDialogEx)

KonfigPID::KonfigPID(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KONFIG_PID, pParent)
{

}

KonfigPID::~KonfigPID()
{
}

void KonfigPID::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_KANAL_PID, m_ctrlKanalPID);
}


BEGIN_MESSAGE_MAP(KonfigPID, CDialogEx)
	ON_BN_CLICKED(IDOK, &KonfigPID::OnBnClickedOk)
END_MESSAGE_MAP()


// Procedury obsługi komunikatów KonfigPID


///////////////////////////////////////////////////////////////////////////////////////////////////
// Inicjalizacja zawartości okna dialogowego
// Zwraca: TRUE
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL KonfigPID::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Dodaj tutaj dodatkową inicjację
	m_ctrlKanalPID.InsertItem(0, _T("Przechylenie"));
	m_ctrlKanalPID.InsertItem(1, _T("Pochylenie"));
	m_ctrlKanalPID.InsertItem(2, _T("Odchylenie"));
	m_ctrlKanalPID.InsertItem(3, _T("Wysokość"));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // WYJĄTEK: Strona właściwości OCX powinna zwrócić FALSE
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Zamknięcie okna przyciskiem OK
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void KonfigPID::OnBnClickedOk()
{
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
	CDialogEx::OnOK();
}


