#pragma once


// Okno dialogowe CTest

class CTest : public CDialogEx
{
	DECLARE_DYNAMIC(CTest)

public:
	CTest(CWnd* pParent = nullptr);   // konstruktor standardowy
	virtual ~CTest();

// Dane okna dialogowego
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // obsługa DDX/DDV

	DECLARE_MESSAGE_MAP()
};
