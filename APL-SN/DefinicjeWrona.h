#pragma once


// Okno dialogowe DefinicjeWrona

class DefinicjeWrona : public CDialogEx
{
	DECLARE_DYNAMIC(DefinicjeWrona)

public:
	DefinicjeWrona(CWnd* pParent = nullptr);   // konstruktor standardowy
	virtual ~DefinicjeWrona();

// Dane okna dialogowego
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DEFINICJE_WRONA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // obsługa DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CString m_strNumer;
	CString m_strNazwa;
	DWORD m_dNumIP;
	BOOL m_bZmieniono;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEditNumer();
	afx_msg void OnEnChangeEditNazwa();
	afx_msg void OnIpnFieldchangedIpaddressDefinicje(NMHDR* pNMHDR, LRESULT* pResult);
};
