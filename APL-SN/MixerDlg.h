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
	struct Config
	{
		float fAngle[KANALY_MIKSERA];
		float fLenght[KANALY_MIKSERA];

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
	CString m_strAngle09;
	CString m_strAngle10;
	CString m_strAngle11;
	CString m_strAngle12;
	CString m_strLength01;
	CString m_strLength02;
	CString m_strLength03;
	CString m_strLength04;
	CString m_strLength05;
	CString m_strLength06;
	CString m_strLength07;
	CString m_strLength08;

	float m_fAngle[KANALY_MIKSERA];
	float m_fLenght[KANALY_MIKSERA];
	Config m_stPredefConfig[4];	//4 konfiguracje
	
	CListCtrl m_ctlPredefList;
	afx_msg void OnLvnHotTrackLstPredefined(NMHDR *pNMHDR, LRESULT *pResult);
	int m_nActualSel;
	afx_msg void OnEnChangeEdtAngle01();
	afx_msg void OnEnChangeEdtAngle02();
	afx_msg void OnEnChangeEdtAngle03();
	afx_msg void OnEnChangeEdtAngle04();
	afx_msg void OnEnChangeEdtAngle05();
	afx_msg void OnEnChangeEdtAngle06();
	afx_msg void OnEnChangeEdtAngle07();
	afx_msg void OnEnChangeEdtAngle08();

	afx_msg void OnEnChangeEdtLength01();
	afx_msg void OnEnChangeEdtLength02();
	afx_msg void OnEnChangeEdtLength03();
	afx_msg void OnEnChangeEdtLength04();
	afx_msg void OnEnChangeEdtLength05();
	afx_msg void OnEnChangeEdtLength06();
	afx_msg void OnEnChangeEdtLength07();
	afx_msg void OnEnChangeEdtLength08();

	afx_msg void OnBnClickedButSave();
	afx_msg void OnBnClickedButRead();
	void UpdateAngleLen();
};
