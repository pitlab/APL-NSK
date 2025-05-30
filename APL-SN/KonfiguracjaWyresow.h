﻿#pragma once
#include "DrzewoWykresow.h"
#include "APL-SNDoc.h"
#include "APL-SNView.h"

// Okno dialogowe KonfiguracjaWyresow

class KonfiguracjaWyresow : public CDialogEx
{
	DECLARE_DYNAMIC(KonfiguracjaWyresow)

public:
	KonfiguracjaWyresow(CWnd* pParent = nullptr);   // konstruktor standardowy
	virtual ~KonfiguracjaWyresow();

// Dane okna dialogowego
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KONFIG_WYKRESOW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // obsługa DDX/DDV

	DECLARE_MESSAGE_MAP()
	afx_msg void OnTvnBegindragTreeWykresow(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnBegindragListaDanych(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnDropdownIdok(NMHDR* pNMHDR, LRESULT* pResult);

	//CAPLSNDoc* CAPLSNView::GetDocument() const;


public:
	CListCtrl m_cListaDanych;
	DrzewoWykresow m_cDrzewoWykresow;
	COleDropTarget m_DropTarget;
//	afx_msg void OnDropFiles(HDROP hDropInfo);
};
