#pragma once


// Okno dialogowe Rejestrator

class Rejestrator : public CDialogEx
{
	DECLARE_DYNAMIC(Rejestrator)

public:
	Rejestrator(CWnd* pParent = nullptr);   // konstruktor standardowy
	virtual ~Rejestrator();

// Dane okna dialogowego
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REJESTRATOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // obsługa DDX/DDV

	DECLARE_MESSAGE_MAP()
};
