#pragma once
#include "UstawieniaKamery.h"

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
	afx_msg void OnNMCustomdrawSlidZoomZdjecia(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedCheckOdwrPoz();
	afx_msg void OnBnClickedCheckOdwrPion();
	afx_msg void OnCbnSelchangeComboTrybDiagnostyczny();
	afx_msg void OnBnClickedOk();
	int WyslijDoKamery();
	st_KonfKam m_stKonfKamery;
	int m_nSzerokoscZdjecia;
	int m_nWysokoscZdjecia;
	int m_nPowiekszenieZdjecia;
	
	CSliderCtrl m_ctlSzerokoscZdjecia;
	CSliderCtrl m_ctlWysokoscZdjecia;
	CSliderCtrl m_ctlPowiekszenie;
	CSliderCtrl m_ctlPrzesunieciePoziome;
	CSliderCtrl m_ctlPrzesunieciePionowe;

	CComboBox m_ctlTrybDiagnostyczny;
	
	CString m_StrSzerokosc;
	CString m_strWysokosc;
	CString m_strPowiekszenie;
	BOOL m_bOdwrocPionowo;
	BOOL m_bOdwrocPoziomo;

	
};
