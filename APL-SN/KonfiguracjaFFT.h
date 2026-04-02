#pragma once
#include "sys_def_wspolny.h"

//#define FFT_WYKLADNIK_MIN	6		//najmniejszy wykładnik FFT 2^6 = 64
//#define FFT_WYKLADNIK_MAX	12		//największy wykładnik FFT 2^12 = 4096
//#define FFT_MAX_ROZMIAR		4096	//największy rozmiar danych do liczenia FFT
//#define LICZBA_TESTOW_FFT	100		//liczba warunków pracy dla których zdeto komplet FFT np, różnych wysterowań silników
//#define LICZB_FLOAT_WRAMCE	32
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
	afx_msg void OnEnChangeEditWysterowanie();
	uint8_t m_chWykładnikPotęgi;
	uint8_t m_chRodzajOkna;
	uint8_t m_chWysterowanieMaxProcent;
	uint8_t m_chAktywneSilniki;
	BOOL m_bZmienionoDane;
	afx_msg void OnBnClickedOk();
	CSliderCtrl m_ctlZakresWysterowania;
	afx_msg void OnNMCustomdrawSlidZakresWysterowania(NMHDR* pNMHDR, LRESULT* pResult);
	CString m_strWysterowanie;
	BOOL m_bSilnik1;
	BOOL m_bSilnik2;
	BOOL m_bSilnik3;
	BOOL m_bSilnik4;
	BOOL m_bSilnik5;
	BOOL m_bSilnik6;
	afx_msg void OnBnClickedCheckSilnik1();
	afx_msg void OnBnClickedCheckSilnik2();
	afx_msg void OnBnClickedCheckSilnik3();
	afx_msg void OnBnClickedCheckSilnik4();
	afx_msg void OnBnClickedCheckSilnik5();
	afx_msg void OnBnClickedCheckSilnik6();
	afx_msg void OnBnClickedButPobierzDane();
	int m_nRozmiarFFT;
	afx_msg void OnBnClickedButRozpocznijDiagnostyke();
};
