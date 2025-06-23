#pragma once


// Okno dialogowe OdbiornikiRC

class OdbiornikiRC : public CDialogEx
{
	DECLARE_DYNAMIC(OdbiornikiRC)

public:
	OdbiornikiRC(CWnd* pParent = nullptr);   // konstruktor standardowy
	virtual ~OdbiornikiRC();

// Dane okna dialogowego
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ODBIORNIKI_RC };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // obsługa DDX/DDV
	uint8_t WstawDaneKanalow();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CButton m_ctlPPM1;
	CButton m_ctlSBus1;
	CButton m_ctlPPM2;
	CButton m_ctlSbus2;
	CButton m_ctlRC2_QSPI;
	CProgressCtrl m_ctlRC1Kan1;
	CProgressCtrl m_ctlRC1Kan2;
	CProgressCtrl m_ctlRC1Kan3;
	CProgressCtrl m_ctlRC1Kan4;
	CProgressCtrl m_ctlRC1Kan5;
	CProgressCtrl m_ctlRC1Kan6;
	CProgressCtrl m_ctlRC1Kan7;
	CProgressCtrl m_ctlRC1Kan8;
	CProgressCtrl m_ctlRC1Kan9;
	CProgressCtrl m_ctlRC1Kan10;
	CProgressCtrl m_ctlRC1Kan11;
	CProgressCtrl m_ctlRC1Kan12;
	CProgressCtrl m_ctlRC1Kan13;
	CProgressCtrl m_ctlRC1Kan14;
	CProgressCtrl m_ctlRC1Kan15;
	CProgressCtrl m_ctlRC1Kan16;

	CProgressCtrl m_ctlRC2Kan1;
	CProgressCtrl m_ctlRC2Kan2;
	CProgressCtrl m_ctlRC2Kan3;
	CProgressCtrl m_ctlRC2Kan4;
	CProgressCtrl m_ctlRC2Kan5;
	CProgressCtrl m_ctlRC2Kan6;
	CProgressCtrl m_ctlRC2Kan7;
	CProgressCtrl m_ctlRC2Kan8;
	CProgressCtrl m_ctlRC2Kan9;
	CProgressCtrl m_ctlRC2Kan10;
	CProgressCtrl m_ctlRC2Kan11;
	CProgressCtrl m_ctlRC2Kan12;
	CProgressCtrl m_ctlRC2Kan13;
	CProgressCtrl m_ctlRC2Kan14;
	CProgressCtrl m_ctlRC2Kan15;
	CProgressCtrl m_ctlRC2Kan16;

	CProgressCtrl m_ctlSerwo1;
	CProgressCtrl m_ctlSerwo2;
	CProgressCtrl m_ctlSerwo3;
	CProgressCtrl m_ctlSerwo4;
	CProgressCtrl m_ctlSerwo5;
	CProgressCtrl m_ctlSerwo6;
	CProgressCtrl m_ctlSerwo7;
	CProgressCtrl m_ctlSerwo8;
	CProgressCtrl m_ctlSerwo9;
	CProgressCtrl m_ctlSerwo10;
	CProgressCtrl m_ctlSerwo11;
	CProgressCtrl m_ctlSerwo12;
	CProgressCtrl m_ctlSerwo13;
	CProgressCtrl m_ctlSerwo14;
	CProgressCtrl m_ctlSerwo15;
	CProgressCtrl m_ctlSerwo16;

	CComboBox m_ctlTypWyjscia1;
	CComboBox m_ctlTypWyjscia2;
	CComboBox m_ctlTypWyjscia3;
	CComboBox m_ctlTypWyjscia4;
	CComboBox m_ctlTypWyjscia5;
	CComboBox m_ctlTypWyjscia6;
	CComboBox m_ctlTypWyjscia7;
	CComboBox m_ctlTypWyjscia8;
	CComboBox m_ctlTypWyjscia9_16;

	afx_msg void OnBnClickedRadioPpm1();
	afx_msg void OnBnClickedRadioSbus1();
	afx_msg void OnBnClickedRadioPpm2();
	afx_msg void OnBnClickedRadioSbus2();
	BOOL m_bZmienionoUstawienie;
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnCbnSelchangeCombo3();
	afx_msg void OnCbnSelchangeCombo4();
	afx_msg void OnCbnSelchangeCombo5();
	afx_msg void OnCbnSelchangeCombo6();
	afx_msg void OnCbnSelchangeCombo7();
	afx_msg void OnCbnSelchangeCombo8();
	afx_msg void OnCbnSelchangeCombo9();
	afx_msg void OnBnClickedRadioRc23();
};
