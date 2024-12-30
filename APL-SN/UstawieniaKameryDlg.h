#pragma once



// Okno dialogowe UstawieniaKameryDlg

//class CUstawieniaKameryDlg : public CDialogEx
class CUstawieniaKameryDlg : public CDialog
{
	DECLARE_DYNAMIC(CUstawieniaKameryDlg)

public:
	CUstawieniaKameryDlg(CWnd* pParent = nullptr);   // konstruktor standardowy
	virtual ~CUstawieniaKameryDlg();
	virtual BOOL OnInitDialog();

// Dane okna dialogowego
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USTAWIENIA_KAMERY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // obsługa DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdrawSlidSzerZdjecia(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSlidWysZdjecia(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSlidZoomZdjecia2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedCheckOdwrPoz();
	afx_msg void OnBnClickedCheckOdwrPion();
	afx_msg void OnCbnSelchangeComboTrybDiagnostyczny();
	afx_msg void OnBnClickedOk();
	int m_nSzerokoscZdjecia;
	int m_nWysokoscZdjecia;
	int m_nPowiekszenieZdjecia;
	
	CSliderCtrl m_ctlSzerokoscZdjecia;
	CSliderCtrl m_ctlWysokoscZdjecia;
	CButton m_ctlOdwrocPoziomo;
	BOOL m_bOdwrocPionowo;
	CComboBox m_ctlTrybDiagnostyczny;
	CButton m_ctlOdwrocPionowo;
	CSliderCtrl m_ctlPowiekszenie;
	CString m_StrSzerokosc;
	CString m_strWysokosc;
};
