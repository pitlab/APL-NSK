// Rejestrator.cpp: plik implementacji
//

#include "pch.h"
#include "APL-SN.h"
#include "Rejestrator.h"
#include "afxdialogex.h"


// Okno dialogowe Rejestrator

IMPLEMENT_DYNAMIC(Rejestrator, CDialogEx)

Rejestrator::Rejestrator(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_REJESTRATOR, pParent)
{

}

Rejestrator::~Rejestrator()
{
}

void Rejestrator::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Rejestrator, CDialogEx)
END_MESSAGE_MAP()


// Procedury obsługi komunikatów Rejestrator
