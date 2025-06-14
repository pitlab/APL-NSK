#pragma once
#include "sys_def_wspolny.h"
#include "resource.h"
#include "afxcmn.h"
// CMixerDlg dialog

#define deg2rad PI/180
#define rad2deg 180/PI
#define ILOSC_PREDEF_KONF	5	//iloœæ predefiniowanej konfiguracji

class CMixerDlg : public CDialog
{
	DECLARE_DYNAMIC(CMixerDlg)

public:
	CMixerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMixerDlg();

// Dialog Data
	enum { IDD = IDD_MIKSER };
	struct Konfiguracja
	{
		float fKat[KANALY_MIKSERA];		//[rad]
		float fDlug[KANALY_MIKSERA];	//[mm]

		CString strName;
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	CString m_strKat1;
	CString m_strKat2;
	CString m_strKat3;
	CString m_strKat4;
	CString m_strKat5;
	CString m_strKat6;
	CString m_strKat7;
	CString m_strKat8;
	CString m_strDlugosc1;
	CString m_strDlugosc2;
	CString m_strDlugosc3;
	CString m_strDlugosc4;
	CString m_strDlugosc5;
	CString m_strDlugosc6;
	CString m_strDlugosc7;
	CString m_strDlugosc8;
	CListCtrl m_ctlPredefiniowane;
	CString m_strSrednicaSmiglaMm;
	CString m_strSrednicaSmiglaCale;

	float m_fKatRamienia[KANALY_MIKSERA];		//[rad]
	float m_fDlugoscRamienia[KANALY_MIKSERA];	//[mm]
	float m_fPromienSmigla;	//[mm]
	Konfiguracja m_stPredefKonfig[ILOSC_PREDEF_KONF];	//5 konfiguracji
	int m_nActualSel;
	BOOL m_bBylaZmianaDlugosci;
	BOOL m_bBylaZmianaKata;
	void AktualizujDlugoscKat();

	afx_msg void OnLvnHotTrackLstPredefined(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditKat1();
	afx_msg void OnEnChangeEditKat2();
	afx_msg void OnEnChangeEditKat3();
	afx_msg void OnEnChangeEditKat4();
	afx_msg void OnEnChangeEditKat5();
	afx_msg void OnEnChangeEditKat6();
	afx_msg void OnEnChangeEditKat7();
	afx_msg void OnEnChangeEditKat8();
	afx_msg void OnEnChangeEditDlugosc1();
	afx_msg void OnEnChangeEditDlugosc2();
	afx_msg void OnEnChangeEditDlugosc3();
	afx_msg void OnEnChangeEditDlugosc4();
	afx_msg void OnEnChangeEditDlugosc5();
	afx_msg void OnEnChangeEditDlugosc6();
	afx_msg void OnEnChangeEditDlugosc7();
	afx_msg void OnEnChangeEditDlugosc8();
	afx_msg void OnEnChangeEditSrednicaSmiglaMm();
	afx_msg void OnEnChangeEditSrednicaSmiglaCale();
};
