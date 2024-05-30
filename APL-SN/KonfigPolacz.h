#pragma once


// Okno dialogowe OknoPolacz

class CKonfigPolacz : public CDialogEx
{
	DECLARE_DYNAMIC(CKonfigPolacz)

public:
	CKonfigPolacz(CWnd* pParent = nullptr);   // konstruktor standardowy
	virtual ~CKonfigPolacz();

// Dane okna dialogowego
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PORT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // obsługa DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComboPortCom();
	afx_msg void OnCbnSelchangeComboPredkosc();
	afx_msg void OnEnChangeEditAdres();
	afx_msg void OnEnChangeEditPortEth();
};
