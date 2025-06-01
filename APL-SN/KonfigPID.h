﻿#pragma once
#include "PIDtabCtrl.h"
#include "Konfig_fram.h"


// Okno dialogowe KonfigPID

class KonfigPID : public CDialogEx
{
	DECLARE_DYNAMIC(KonfigPID)

public:
	KonfigPID(CWnd* pParent = nullptr);   // konstruktor standardowy
	virtual ~KonfigPID();

	const int nPidPredkPrzech = 0;
	const int nPidPredkPochyl = 1;
	const int nPidPredkOdchyl = 2;
	const int nPidPredkWysoko = 3;
	const int nPidKataPrzech = 4;
	const int nPidKataPochyl = 5;
	const int nPidKataOdchyl = 6;
	const int nPidWysokosci = 7;
	struct pid_t
	{
		float fKp;		
		float fTi;
		float fTd;
		float fOgrCalki;	//ograniczenie członu całkujacego
		float fMinWyj;		//minimalna wartość wyjścia
		float fMaxWyj;		//maksymalna wartość wyjścia
		uint8_t chPodstFiltraD;	//podstawa filtra IIR
		BOOL bKatowy;		//zawija kąt miedzy 0 i 2Pi
		BOOL bWylaczony;	//regulator jest wyłączony
		BOOL bZmieniony;	//zmieniono nastawy regulatora
	} m_stPID[LICZBA_REGULATOROW_PID];
// Dane okna dialogowego
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KONFIG_PID };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // obsługa DDX/DDV
	void UstawKontrolki();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

private:
	int m_nBiezacyRegulator;
	CTabCtrl m_ctrlKanalPID;
	//PIDtabCtrl m_ctrlKanalPID;
	//afx_msg void OnPaint();
	CString m_strTI2;
	CString m_strTI1;
	CString m_strKP1;
	CString m_strKP2;
	CString m_strFiltrD1;
	CString m_strFiltrD2;
	CString m_strTD1;
	CString m_strTD2;
	CString m_strOgrCalki1;
	CString m_strOgrCalki2;
	CString m_strMinWyj1;
	CString m_strMaxWyj1;
	CString m_strMinWyj2;
	CString m_strMaxWyj2;
	BOOL m_bKatowy1;
	BOOL m_bKatowy2;
	BOOL m_bWylaczony1;
	BOOL m_bWylaczony2;
	int m_PodstFiltraD1;
	int m_PodstFiltraD2;
	CString m_strPodstFiltraD1;
	CString m_strPodstFiltraD2;
	CSliderCtrl m_ctlSlidPOdstCzasuFiltraD1;
	CSliderCtrl m_ctlSlidPOdstCzasuFiltraD2;
public:

	CString m_strTest;
	afx_msg void OnTcnSelchangeTabKanalPid(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnChangeEditKp1();
	afx_msg void OnEnChangeEditKp2();
	afx_msg void OnEnChangeEditTi1();
	afx_msg void OnEnChangeEditTi2();
	afx_msg void OnEnChangeEditTd1();
	afx_msg void OnEnChangeEditTd2();
	afx_msg void OnEnChangeEditMinWy1();
	afx_msg void OnEnChangeEditMaxWy1();
	afx_msg void OnEnChangeEditMinWy2();
	afx_msg void OnEnChangeEditMaxWy2();
	afx_msg void OnNMCustomdrawSliderFiltrD1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSliderFiltrD2(NMHDR* pNMHDR, LRESULT* pResult);
	

	afx_msg void OnEnChangeEditLimitCalki2();
	afx_msg void OnEnChangeEditLimitCalki1();
	afx_msg void OnBnClickedCheckKatowy1();
	afx_msg void OnBnClickedCheckWylacz1();
	afx_msg void OnBnClickedCheckKatowy2();
	afx_msg void OnBnClickedCheckWylacz2();

	afx_msg void OnTRBNThumbPosChangingSliderFiltrD1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTRBNThumbPosChangingSliderFiltrD2(NMHDR* pNMHDR, LRESULT* pResult);
};
