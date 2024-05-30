// OknoPolacz.cpp: plik implementacji
//

#include "pch.h"
#include "APL-SN.h"
#include "KonfigPolacz.h"
#include "afxdialogex.h"


// Okno dialogowe OknoPolacz

IMPLEMENT_DYNAMIC(CKonfigPolacz, CDialogEx)

CKonfigPolacz::CKonfigPolacz(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PORT, pParent)
{

}

CKonfigPolacz::~CKonfigPolacz()
{
}

void CKonfigPolacz::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CKonfigPolacz, CDialogEx)
	ON_BN_CLICKED(IDOK, &CKonfigPolacz::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_PORT_COM, &CKonfigPolacz::OnCbnSelchangeComboPortCom)
	ON_CBN_SELCHANGE(IDC_COMBO_PREDKOSC, &CKonfigPolacz::OnCbnSelchangeComboPredkosc)
	ON_EN_CHANGE(IDC_EDIT_ADRES, &CKonfigPolacz::OnEnChangeEditAdres)
	ON_EN_CHANGE(IDC_EDIT_PORT_ETH, &CKonfigPolacz::OnEnChangeEditPortEth)
END_MESSAGE_MAP()


// Procedury obsługi komunikatów OknoPolacz


void CKonfigPolacz::OnBnClickedOk()
{
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
	CDialogEx::OnOK();
}


void CKonfigPolacz::OnCbnSelchangeComboPortCom()
{
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
}


void CKonfigPolacz::OnCbnSelchangeComboPredkosc()
{
	// TODO: Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
}


void CKonfigPolacz::OnEnChangeEditAdres()
{
	// TODO:  Jeżeli to jest kontrolka RICHEDIT, to kontrolka nie będzie
	// wyślij to powiadomienie, chyba że przesłonisz element CDialogEx::OnInitDialog()
	// funkcja i wywołanie CRichEditCtrl().SetEventMask()
	// z flagą ENM_CHANGE zsumowaną logicznie z maską.

	// TODO:  Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
}


void CKonfigPolacz::OnEnChangeEditPortEth()
{
	// TODO:  Jeżeli to jest kontrolka RICHEDIT, to kontrolka nie będzie
	// wyślij to powiadomienie, chyba że przesłonisz element CDialogEx::OnInitDialog()
	// funkcja i wywołanie CRichEditCtrl().SetEventMask()
	// z flagą ENM_CHANGE zsumowaną logicznie z maską.

	// TODO:  Dodaj tutaj swój kod procedury obsługi powiadamiania kontrolki
}
