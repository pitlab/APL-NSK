﻿#pragma once
#include "DrzewoWykresow.h"
#include "APL-SNDoc.h"
#include "afxcolorpickerctrl.h"

//definicje kolumn listy Danych
#define LD_NAZWA	0
#define LD_ID		1
#define LD_ZRODLO	2
#define LD_LPOMIAR	3
#define LD_MIN		4
#define LD_MAX		5


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
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLvnItemchangedListaDanych(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedMfccolor();
	afx_msg void OnTvnSelchangedTreeWykresow(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTvnSelchangingTreeWykresow(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickTreeWykresow(NMHDR* pNMHDR, LRESULT* pResult);

	BOOL m_bPrzeciaganieMysza{FALSE};
	BOOL m_bKursorPrzeciaganie{ FALSE };
	CPoint m_cpPozycjaMyszy;	
	CImageList m_ObrazkiDrzewa;

public:
	CListCtrl m_cListaDanych;
	DrzewoWykresow m_cDrzewoWykresow;
	COleDropTarget m_DropTarget;
	CMFCColorButton m_ctrlKolor;
	BOOL m_bZawieraLog;
};
