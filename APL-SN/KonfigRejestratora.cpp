// KonfigRejestratora.cpp: plik implementacji
//

#include "pch.h"
#include "APL-SN.h"
#include "KonfigRejestratora.h"
#include "afxdialogex.h"


// Okno dialogowe KonfigRejestratora

IMPLEMENT_DYNAMIC(KonfigRejestratora, CDialogEx)

KonfigRejestratora::KonfigRejestratora(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KONFIG_REJESTRATORA, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

KonfigRejestratora::~KonfigRejestratora()
{
}

void KonfigRejestratora::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KonfigRejestratora, CDialogEx)
END_MESSAGE_MAP()


// Procedury obsługi komunikatów KonfigRejestratora
