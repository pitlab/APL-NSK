#pragma once


// Okno dialogowe CKonfigTelemetrii

class CKonfigTelemetrii : public CDialogEx
{
	DECLARE_DYNAMIC(CKonfigTelemetrii)

public:
	CKonfigTelemetrii(CWnd* pParent = nullptr);   // konstruktor standardowy
	virtual ~CKonfigTelemetrii();
	void UstawIDZmiennej(uint8_t chID) { m_chIdZmiennej = chID; };
	void UstawNazweZmiennej(CString nazwa) { m_strNazwaZmiennej = nazwa; };
	void UstawOkresTelem(uint8_t chOkres) { m_nOkres = chOkres; };
	uint8_t PobierzOkresTelem(void);
// Dane okna dialogowego
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TELEMETRIA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // obsługa DDX/DDV
	int PozycjaDlaOkresu(uint8_t chOkres, uint8_t* chZaokraglonyOkres);
	uint8_t m_chIdZmiennej;
	int m_nOkres;
	CString m_strNazwaZmiennej;

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_ctlOkresTelemetrii;
	afx_msg void OnBnClickedOk();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnLbnSelchangeOkresTelemetrii();
};


//CKonfigTelemetrii& getKonfigTelemetrii();