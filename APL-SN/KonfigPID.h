#pragma once
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
		float fLimitCalki;
		uint8_t chRozmiarFiltraD;
	} m_stPID[LICZBA_REGULATOROW_PID];
// Dane okna dialogowego
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KONFIG_PID };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // obsługa DDX/DDV

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
	CString m_strLimitCalki1;
	CString m_strLimitCalki2;
public:

};
