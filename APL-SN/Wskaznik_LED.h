#pragma once


// Okno dialogowe Wskaznik_LED

class Wskaznik_LED : public CDialogEx
{
	DECLARE_DYNAMIC(Wskaznik_LED)

public:
	Wskaznik_LED(CWnd* pParent = nullptr);   // konstruktor standardowy
	virtual ~Wskaznik_LED();

// Dane okna dialogowego
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WSKAZNIK_LED };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // obsługa DDX/DDV


private:
	BOOL m_bBylaZmianaLiczbyLed;
	BOOL m_bBylaZmianaKolorow;

	DECLARE_MESSAGE_MAP()
public:
	CString m_strLiczbaLED;
	int m_nSzerokoscPunktu;
	int m_nJasnoscTla;
	int m_nLiczbaLED;
	int m_nMinCzerwony;
	int m_nMaxCzerwony;
	int m_nMinZielony;
	int m_nMaxZielony;
	int m_nMinNiebieski;
	int m_nMaxNiebieski;
	CString m_strMinCzerwony;
	CString m_strMaxCzerwony;
	CString m_strMinZielony;
	CString m_strMaxZielony;
	CString m_strMinNiebieski;
	CString m_strMaxNiebieski;
	CComboBox m_ctlZmienna;
	CString m_strMinZmiennej;
	CString m_strMaxZmiennej;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CSliderCtrl m_ctlSzerokoscPunktu;
	CSliderCtrl m_ctlJasnoscTla;
	afx_msg void OnEnChangeEditLiczbaLed();
	afx_msg void OnNMCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnChangeEditMinCzerwony();
	afx_msg void OnEnChangeEditMaxCzerwony();
	afx_msg void OnEnChangeMinZielony();
	afx_msg void OnEnChangeEditMaxZielony();
	afx_msg void OnEnChangeEditminNiebieski();
	afx_msg void OnEnChangeEditMaxNiebieski();
	afx_msg void OnCbnSelchangeComboZmienna();
	afx_msg void OnEnChangeEditZmiennaMin();
	afx_msg void OnEnChangeEditZmiennaMax();
	afx_msg void OnNMCustomdrawSliderSzerokoscPunktu(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSliderJasnoscTla(NMHDR* pNMHDR, LRESULT* pResult);
	CString m_strSzerokoscPunktu;
	CString m_strJasnoscTla;
};
