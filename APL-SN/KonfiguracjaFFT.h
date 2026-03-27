#pragma once


// Okno dialogowe KonfiguracjaFFT

class KonfiguracjaFFT : public CDialogEx
{
	DECLARE_DYNAMIC(KonfiguracjaFFT)

public:
	KonfiguracjaFFT(CWnd* pParent = nullptr);   // konstruktor standardowy
	virtual ~KonfiguracjaFFT();

// Dane okna dialogowego
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PARAMETRY_FFT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // obsługa DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_ctlRozdzielczoscFFT;
	afx_msg void OnNMCustomdrawSlidRozdzielczoscFft(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	CComboBox m_ctlRodzajOkna;
	CString m_strRozdzielczośćFFT;
	CString m_strWysterowanie;
	afx_msg void OnEnChangeEditWysterowanie();
	uint8_t m_chWykładnikPotęgi;
	uint8_t m_chRodzajOkna;
	uint16_t m_sWysterowanie;
	uint8_t m_chAktywneSilniki;
	BOOL m_bZmienionoDane;
	afx_msg void OnBnClickedOk();
};
