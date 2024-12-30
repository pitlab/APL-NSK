#pragma once


// Okno dialogowe CDaneFlash

class CDaneFlash : public CDialogEx
{
	DECLARE_DYNAMIC(CDaneFlash)

public:
	CDaneFlash(CWnd* pParent = nullptr);   // konstruktor standardowy
	virtual ~CDaneFlash();

// Dane okna dialogowego
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ZAPISZ_FLASH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // obsługa DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButCzytajPlik();
	CListCtrl m_ctlListaKomunikatow;
};
