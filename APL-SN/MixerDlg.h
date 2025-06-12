#pragma once
#include "sys_def_wspolny.h"
#include "resource.h"
#include "afxcmn.h"
// CMixerDlg dialog

#define deg2rad PI/180
#define rad2deg 180/PI


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
		float fKat[KANALY_MIKSERA];
		float fDlug[KANALY_MIKSERA];

		CString strName;
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	CString m_strAngle01;
	CString m_strAngle02;
	CString m_strAngle03;
	CString m_strAngle04;
	CString m_strAngle05;
	CString m_strAngle06;
	CString m_strAngle07;
	CString m_strAngle08;
	CString m_strLength01;
	CString m_strLength02;
	CString m_strLength03;
	CString m_strLength04;
	CString m_strLength05;
	CString m_strLength06;
	CString m_strLength07;
	CString m_strLength08;

	float m_fKatRamienia[KANALY_MIKSERA];
	float m_fDlugoscRamienia[KANALY_MIKSERA];
	Konfiguracja m_stPredefKonfig[4];	//4 konfiguracje
	
	CListCtrl m_ctlPredefList;
	afx_msg void OnLvnHotTrackLstPredefined(NMHDR *pNMHDR, LRESULT *pResult);
	int m_nActualSel;

	afx_msg void OnBnClickedButSave();
	afx_msg void OnBnClickedButRead();
	void UpdateAngleLen();
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
	CListBox m_ctlPredefiniowane;
};
