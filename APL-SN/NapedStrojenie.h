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
	BOOL m_bBylaZmianaWychyleniaDrazkowAkro;
	BOOL m_bBylaZmianaWychyleniaDrazkowStab;
	BOOL m_bBylaZmianaObrotow;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEditPrzechylenieAkro();
	afx_msg void OnEnChangeEditPochylenieAkro();
	afx_msg void OnEnChangeEditOdchylenieAkro();
	afx_msg void OnEnChangeEditWysokoscAkro();
	CString m_strZakresPrzechyleniaAkro;
	CString m_strZakresPochyleniaAkro;
	CString m_strZakresOdchyleniaAkro;
	CString m_strZakresWysokosciAkro;
	afx_msg void OnEnChangeEditPrzechylenieStab();
	afx_msg void OnEnChangeEditPochylenieStab();
	afx_msg void OnEnChangeEditOdchylenieStab();
	afx_msg void OnEnChangeEditWysokoscStab();
	CString m_strZakresPrzechyleniaStab;
	CString m_strZakresPochyleniaStab;
	CString m_strZakresOdchyleniaStab;
	CString m_strZakresWysokosciStab;
	afx_msg void OnEnChangeEditObrJalowe();
	afx_msg void OnEnChangeEditObrMin();
	afx_msg void OnEnChangeEditObrZawis();
	afx_msg void OnEnChangeEditObrMax();
	CString m_strObrotyJalowe;
	CString m_strObrotyMin;
	CString m_strObrotyZawis;
	CString m_strObrotyMax;
	float m_fSkalaWartosciZadanejAkro[ROZMIAR_DRAZKOW];	//wartość zadana dla pełnego wychylenia drążka aparatury w trybie AKRO
	float m_fSkalaWartosciZadanejStab[ROZMIAR_DRAZKOW];	//wartość zadana dla pełnego wychylenia drążka aparatury w trybie STAB
	int m_nObrotyJalowe;
	int m_nObrotyMin;
	int m_nObrotyZawis;
	int m_nObrotyMax;
	
	float ZamienStrNaFloat(CString strLiczba);

};
