#pragma once


// Okno dialogowe CKonfigTelemetrii

class CKonfigTelemetrii : public CDialogEx
{
	DECLARE_DYNAMIC(CKonfigTelemetrii)

public:
	CKonfigTelemetrii(CWnd* pParent = nullptr);   // konstruktor standardowy
	virtual ~CKonfigTelemetrii();
	void UstawIDZmiennej(uint8_t chID) { m_chIdZmiennej = chID; };

// Dane okna dialogowego
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TELEMETRIA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // obsługa DDX/DDV
	uint8_t m_chIdZmiennej;

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ctlOkresTelemetrii;
	afx_msg void OnBnClickedOk();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
