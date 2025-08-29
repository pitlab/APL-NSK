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
	afx_msg void OnBnClickedCheckOdwrPoz();
	afx_msg void OnBnClickedCheckOdwrPion();
	afx_msg void OnBnClickedOk();
	int WyslijDoKamery();
	st_KonfKam m_stKonfKamery;
	st_KonfKam m_stPierwotnaKonfiguracjaKamery;	
	CSliderCtrl m_ctlSzerokoscZdjecia;
	CSliderCtrl m_ctlWysokoscZdjecia;
	CSliderCtrl m_ctlPrzesunieciePoziome;
	CSliderCtrl m_ctlPrzesunieciePionowe;
	CString m_StrSzerokosc;
	CString m_strWysokosc;
	BOOL m_bOdwrocPionowo;
	BOOL m_bOdwrocPoziomo;
	
	afx_msg void OnNMCustomdrawSlidPrzesPoziome(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSlidPrzesPionowe(NMHDR* pNMHDR, LRESULT* pResult);
	CString m_strPrzesunieciePionowe;
	CString m_strPrzesunieciePoziome;
	BOOL m_bRecznyBalansBieli;
	afx_msg void OnBnClickedCheckRecznyBalansBieli();
	CSliderCtrl m_ctlBalansBieli_Czerwony;
	CSliderCtrl m_ctlBalansBieli_Zielony;
	CSliderCtrl m_ctlBalansBieli_Niebieski;
	afx_msg void OnNMCustomdrawSliderBalansBieli_Czerwony(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSliderBalansBieli_Zielony(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSliderBalansBieli_Niebieski(NMHDR* pNMHDR, LRESULT* pResult);
	CString m_strBalansBieliCzerwona;
	CString m_strBalansBieliZielona;
	CString m_strBalansBieliNiebieska;
	BOOL m_bRecznyCzasEkspozycji;
	CSliderCtrl m_ctlCzasEkspozycji;
	CSliderCtrl m_ctlGornaGranicaEkspozycji;
	CSliderCtrl m_ctlDolnaGranicaEkspozycji;
	CString m_strCzasEkspozycji;
	CString m_strGornaGranicaEkspozycji;
	CString m_strDolnaGranicaEkspozycji;
	afx_msg void OnNMCustomdrawSlidAecCzas(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSlidAecGornaGranica(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSlidAecDolnaGranica(NMHDR* pNMHDR, LRESULT* pResult);
	
	BOOL m_bKorekcjaLENC;
	afx_msg void OnBnClickedCheckKorekcjaLenc();
	BOOL m_bGammaYUV;
	afx_msg void OnBnClickedCheckGamaYuv();
	afx_msg void OnBnClickedCheckGammaSurowa();
	BOOL m_bGammaSurowa;
	BOOL m_bUsuwaUsrednianie;
	afx_msg void OnBnClickedCheckIsp0UsuwaUsred();
	CSliderCtrl m_ctlProgUsuwaniaUsrednianiaParzystychKolumn;
	afx_msg void OnNMCustomdrawSlider1ProgUsrednianiaKolumn(NMHDR* pNMHDR, LRESULT* pResult);
	BOOL m_bOdszumianie;
	afx_msg void OnBnClickedCheckOdszumianie();
	BOOL m_bUsuwanieCzarnychPikseli;
	afx_msg void OnBnClickedCheckIsp0UsuwanieCzarnych();
	BOOL m_bUsuwanieBialychPikseli;
	afx_msg void OnBnClickedCheckIsp0UsuwanieBialych();
	BOOL m_bInterpolacja;
	afx_msg void OnBnClickedCheckIsp0Interpolacja();
	BOOL m_bCyfroweEfektySpecjalne;
	afx_msg void OnBnClickedCheckIsp1CyfroweEfektySpecjalne();
	BOOL m_bUstawienieChrominancji;
	afx_msg void OnBnClickedCheckIsp1UstawienieChrominancji();
	BOOL m_bSkalowaniePionowe;
	afx_msg void OnBnClickedCheckIsp1SkalowaniePionowe();
	BOOL m_bSkalowaniePoziome;
	afx_msg void OnBnClickedCheckIsp1SkalowaniePoziome();
	BOOL m_bRozciaganieLinii;
	afx_msg void OnBnClickedCheckIsp1RozciaganieLini();
	BOOL m_bUsrednianieChrominancji;
	afx_msg void OnBnClickedCheckIsp1UsrednianieChrominancji();
	BOOL m_bMacierzKolorow;
	afx_msg void OnBnClickedCheckIsp1MacierzKolorow();
	BOOL m_bAutomatycznyBalansBieli;
	afx_msg void OnBnClickedCheck1Isp1AutoBalansBieli();
	afx_msg void OnBnClickedCheckEkspozycjaReczna();

	CString m_strProgUsuwaniaSredniejKolumnParzystych;
	afx_msg void OnBnClickedButWyslijDoKamery();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnNMCustomdrawSlidSzerokoscPatrzenia(NMHDR* pNMHDR, LRESULT* pResult);
	CSliderCtrl m_ctlSzerokoscPatrzenia;
	CSliderCtrl m_ctlWysokoscPatrzenia;
	afx_msg void OnNMCustomdrawSlidWysokoscPatrzenia(NMHDR* pNMHDR, LRESULT* pResult);
	CString m_strSzerokoscPatrzenia;
	CString m_strWysokoscPatrzenia;
};
