#pragma once

#define DLUGOSC_NAZWY_ZMIENNEJ_TELEMETRII	30
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

// Dane okna dialogowego
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TELEMETRIA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // obsługa DDX/DDV
	int PozycjaDlaOkresu(uint8_t chOkres, uint8_t* chZaokraglonyOkres);
	uint8_t OkresDlaPozycji(int nPozycja);
	uint8_t m_chIdZmiennej;
	int m_nOkres;
	CString m_strNazwaZmiennej;
	uint8_t m_chOkresTelemetrii[LICZBA_ZMIENNYCH_TELEMETRYCZNYCH];
	BOOL m_bZmieniono;
	int m_nIndeksZmiennej;
	int m_nIndeksOkresu;

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_ctlOkresTelemetrii;
	afx_msg void OnBnClickedOk();
	CListCtrl m_ctlListaTele;
	virtual BOOL OnInitDialog();
	afx_msg void OnLvnItemchangedListZmienneTele(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLbnSelchangeOkresTelemetrii();
};


