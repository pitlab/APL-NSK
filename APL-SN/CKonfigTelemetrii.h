#pragma once

#define DLUGOSC_NAZWY_ZMIENNEJ_TELEMETRII	30
#define ILOSC_OKRESOW	15

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
	void UstawIndeksDronaWRoju(int nIndeks) { m_nIndeksDronaWRoju = nIndeks; };

// Dane okna dialogowego
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KONFIG_TELEMETRII };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // obsługa DDX/DDV
	int PozycjaDlaOkresu(int nOkres, uint16_t* sZaokraglonyOkres);
	uint16_t OkresDlaPozycji(int nPozycja);
	void AktualizujZajetoscLacza();
	uint8_t m_chIdZmiennej;
	int m_nOkres;
	CString m_strNazwaZmiennej;
	uint16_t m_sOkresTelemetrii[LICZBA_ZMIENNYCH_TELEMETRYCZNYCH];
	BOOL m_bZmieniono;
	int m_nIndeksZmiennej;
	int m_nIndeksOkresu;
	int m_nIndeksDronaWRoju;
	CString m_strZajetosc;
	CProgressCtrl m_ctrlZajetosc;

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_ctlListaCzestotliwosciTelemetrii; 
	CListCtrl m_ctlOkresTelemetrii;
	afx_msg void OnBnClickedOk();
	
	virtual BOOL OnInitDialog();
	afx_msg void OnLvnItemchangedListZmienneTele(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLbnSelchangeOkresTelemetrii();
	afx_msg void OnLbnSelchangeListaCzestotliwosci();

};


