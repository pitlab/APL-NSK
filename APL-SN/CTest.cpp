// CTest.cpp: plik implementacji
//

#include "pch.h"
#include "APL-SN.h"
#include "Test.h"
#include "afxdialogex.h"


// Okno dialogowe CTest

IMPLEMENT_DYNAMIC(CTest, CDialogEx)

CTest::CTest(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TEST, pParent)
{

}

CTest::~CTest()
{
}

void CTest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTest, CDialogEx)
END_MESSAGE_MAP()


// Procedury obsługi komunikatów CTest
