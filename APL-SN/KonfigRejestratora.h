#pragma once


// Okno dialogowe KonfigRejestratora

class KonfigRejestratora : public CDialogEx
{
	DECLARE_DYNAMIC(KonfigRejestratora)

public:
	KonfigRejestratora(CWnd* pParent = nullptr);   // konstruktor standardowy
	virtual ~KonfigRejestratora();

// Dane okna dialogowego
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KONFIG_REJESTRATORA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // obsługa DDX/DDV

	DECLARE_MESSAGE_MAP()
};
