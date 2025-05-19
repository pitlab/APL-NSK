#pragma once


// Okno dialogowe OknoPolacz

class CKonfigPolacz : public CDialogEx
{
	DECLARE_DYNAMIC(CKonfigPolacz)

public:
	CKonfigPolacz(CWnd* pParent = nullptr);   // konstruktor standardowy
	virtual ~CKonfigPolacz();
	void UstawNumerPortuCom(uint8_t chNumer) { m_chNumerPortuCom = chNumer; }
	void UstawPredkoscPortuCom(uint32_t nPredkosc) { m_nPredkoscPortuCom = nPredkosc; }
	void UstawNumerPortuEth(uint32_t nNumer) { m_chNumerPortuEth = nNumer; }
	void UstawAdresIP(uint8_t chNumer[4]) { for (uint8_t n = 0; n < 4; n++) m_chAdresIP[n] = chNumer[n]; }
	void UstawTypPolaczenia(uint8_t chTyp) { m_chTypPolaczenia = chTyp; }
	

// Dane okna dialogowego
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PORT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // obsługa DDX/DDV
	uint8_t m_chTypPolaczenia;
	uint8_t m_chNumerPortuCom;
	uint8_t m_chAdresIP[4];
	uint32_t m_nPredkoscPortuCom;
	uint32_t m_chNumerPortuEth;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComboPortCom();
	afx_msg void OnCbnSelchangeComboPredkosc();
	afx_msg void OnEnChangeEditAdres();
	afx_msg void OnEnChangeEditPortEth();
	CComboBox m_cPortCom;
	CComboBox m_cPredkoscCom;
	CString m_strAdresIP;
	CString m_strPortETH;
	BOOL m_bTypPolaczenia;
	virtual BOOL OnInitDialog();
	afx_msg void OnHotitemchangeRadUart(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRadUart();
	afx_msg void OnBnClickedRadEth();
};
