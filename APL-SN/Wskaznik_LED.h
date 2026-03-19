#pragma once
#include "konfig_fram.h"

// Okno dialogowe Wskaznik_LED

class Wskaznik_LED : public CDialogEx
{
	DECLARE_DYNAMIC(Wskaznik_LED)

public:
	Wskaznik_LED(CWnd* pParent = nullptr);   // konstruktor standardowy
	virtual ~Wskaznik_LED();

	typedef struct
	{
		float fMinZmiennej;
		float fMaxZmiennej;
		uint8_t chNumZmiennej;
		uint8_t chSzerWskaznika;
		uint8_t chKontrastTla;
		uint8_t chMinCzer;
		uint8_t chMaxCzer;
		uint8_t chMinZiel;
		uint8_t chMaxZiel;
		uint8_t chMinNieb;
		uint8_t chMaxNieb;
		uint8_t chLiczbaLed;
	} stWskaznikLED_t;

// Dane okna dialogowego
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WSKAZNIK_LED };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // obsługa DDX/DDV


private:
	BOOL m_bBylaZmianaLiczbFloat[LICZBA_WSKAZNIKOW_LED];
	BOOL m_bBylaZmianaLiczbUint8[LICZBA_WSKAZNIKOW_LED];
	BOOL m_bBylaZmianaTypuLed;
	void WstawDaneDoKontrolek(stWskaznikLED_t *WskLed, int nIndeks);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_strLiczbaLED;
	CString m_strMinCzerwony;
	CString m_strMaxCzerwony;
	CString m_strMinZielony;
	CString m_strMaxZielony;
	CString m_strMinNiebieski;
	CString m_strMaxNiebieski;
	CComboBox m_ctlZmienna;
	CComboBox m_ctlTypLed;
	CString m_strMinZmiennej;
	CString m_strMaxZmiennej;
	CSliderCtrl m_ctlSzerWskazn;
	CSliderCtrl m_ctlKontrastTla;
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEditLiczbaLed();
	afx_msg void OnEnChangeEditMinCzerwony();
	afx_msg void OnEnChangeEditMaxCzerwony();
	afx_msg void OnEnChangeMinZielony();
	afx_msg void OnEnChangeEditMaxZielony();
	afx_msg void OnEnChangeEditminNiebieski();
	afx_msg void OnEnChangeEditMaxNiebieski();
	afx_msg void OnCbnSelchangeComboZmienna();
	afx_msg void OnEnChangeEditZmiennaMin();
	afx_msg void OnEnChangeEditZmiennaMax();
	afx_msg void OnNMCustomdrawSliderSzerWskazn(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSliderJasnoscTla(NMHDR* pNMHDR, LRESULT* pResult);
	CString m_strSzerokoscWskaznika;
	CString m_strKontrastTla;
	stWskaznikLED_t m_stWskaznikLED[LICZBA_WSKAZNIKOW_LED];
	CTabCtrl m_ctlIndeksWskaznika;
	int m_nIndeksWskaznikaLed;
	afx_msg void OnTcnSelchangeTabNrWskaznika(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCbnSelchangeComboTypLed();
	int m_nTypLed;	
	
	afx_msg void OnNMCustomdrawSlidSzerWskazn(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButWyslij();
};
