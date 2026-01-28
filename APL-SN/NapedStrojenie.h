#pragma once

#include "Konfig_fram.h"
#include "pid_kanaly.h"


// Okno dialogowe NapedStrojenie

class NapedStrojenie : public CDialogEx
{
	DECLARE_DYNAMIC(NapedStrojenie)

public:
	NapedStrojenie(CWnd* pParent = nullptr);   // konstruktor standardowy
	virtual ~NapedStrojenie();

// Dane okna dialogowego
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NAPED_I_STROJENIE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // obsługa DDX/DDV
	BOOL m_bBylaZmianaObrotow;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEditObrJalowe();
	afx_msg void OnEnChangeEditObrMin();
	afx_msg void OnEnChangeEditObrZawis();
	afx_msg void OnEnChangeEditObrMax();
	CString m_strObrotyJalowe;
	CString m_strObrotyMin;
	CString m_strObrotyZawis;
	CString m_strObrotyMax;
	int m_nObrotyJalowe;
	int m_nObrotyMin;
	int m_nObrotyZawis;
	int m_nObrotyMax;
	
	float ZamienStrNaFloat(CString strLiczba);
};
