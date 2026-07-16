#pragma once
#include "PIDtabCtrl.h"
#include "Konfig_fram.h"
#include "pid_kanaly.h"

// Okno dialogowe KonfigPID

class KonfigPID : public CDialogEx
{
	DECLARE_DYNAMIC(KonfigPID)

public:
	KonfigPID(CWnd* pParent = nullptr);   // konstruktor standardowy
	virtual ~KonfigPID();

	const int nPidPredkPrzech = 0;
	const int nPidPredkPochyl = 1;
	const int nPidPredkOdchyl = 2;
	const int nPidPredkWysoko = 3;
	const int nPidKataPrzech = 4;
	const int nPidKataPochyl = 5;
	const int nPidKataOdchyl = 6;
	const int nPidWysokosci = 7;
	struct pid_t
	{
		float fKp;		
		float fTi;
		float fTd;
		float fKw;
		float fOgrCalki;	//ograniczenie członu całkujacego
		float fMinWyj;		//minimalna wartość wyjścia
		float fMaxWyj;		//maksymalna wartość wyjścia
		float fMnożnikWartZadanej;
		float fPrzesunięcieWartościZadanej;	//stała wartość dodawana do wartości zadanej (umożliwia lot pod niezerowym kątem lub asymetrię prędkości wznoszenia i opadania)
		float fWolne1;

		uint8_t cPodstFiltraD;	//podstawa filtra IIR
		uint8_t cProcWartZadWyprz;	//procent zmiany wartości zadanej podawany na wejsście wyprzedzające
		uint8_t cPodstawaFiltraWartZad;
		BOOL bKatowy;		//zawija kąt miedzy 0 i 2Pi
		BOOL bZmieniony;	//zmieniono nastawy regulatora
	} m_stPID[LICZBA_PID];

	union _un8_32
	{
		float daneFloat;
		uint32_t dane32;
		uint16_t dane16[2];
		uint8_t dane8[4];
	} m_unia8_32 = {0};

	uint8_t m_chTrybRegulacji[LICZBA_REG_PARAM];
	BOOL m_bZmienionyTrybRegulacji;
	BOOL m_bZmienioneParametryStrojenia;
	BOOL m_bZmienioneWartościStrojenia;
	float m_fWartośćParametru[2 * LICZBA_KAN_RC_DO_STROJENIA_PID];	//w jednej zmiennej są parametry: min1, min2, max1, max2 aby uprościć zapis i odczyt
// Dane okna dialogowego
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KONFIG_PID };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // obsługa DDX/DDV
	void UstawKontrolki(int nParametr);
	void UstawTrybRegulacji(int nParametr);
	float ZamienStrNaFloat(CString str);
	void WlaczKontrolki(uint8_t cTrybPracyRegulatora, uint8_t cKanal);
	void UstawJednostke(int nParametr);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

private:
	int m_nBiezacyParametr;
	CTabCtrl m_ctrlKanalPID;	
	CString m_strTI1;
	CString m_strTI2;
	CString m_strKP1;
	CString m_strKP2;
	CString m_strFiltrD1;
	CString m_strFiltrD2;
	CString m_strTD1;
	CString m_strTD2;
	CString m_strKW1;
	CString m_strKW2;
	CString m_strOgrCalki1;
	CString m_strOgrCalki2;
	CString m_strMinWyj1;
	CString m_strMinWyj2;
	CString m_strMaxWyj1;	
	CString m_strMaxWyj2;
	CString m_strPrzesunięcieWartościZadanej1;
	CString m_strPrzesunięcieWartościZadanej2;
	BOOL m_bKatowy;
	int m_nPodstFiltraD1;
	int m_nPodstFiltraD2;
	int m_nProcWyprzedzenia1;
	int m_nProcWyprzedzenia2;
	int m_nPodstFiltraWartZadanej1;
	int m_nPodstFiltraWartZadanej2;
	CString m_strPodstFiltraD1;
	CString m_strPodstFiltraD2;
	CString m_strProcWyprzedzenia1;
	CString m_strProcWyprzedzenia2;
	CString m_strPodstFiltraWZad1;
	CString m_strPodstFiltraWZad2;
	CSliderCtrl m_ctlSlidPodstCzasuFiltraD1;
	CSliderCtrl m_ctlSlidPodstCzasuFiltraD2;
	CSliderCtrl m_ctlSlidPodstFiltraWartZad1;
	CSliderCtrl m_ctlSlidPodstFiltraWartZad2;
	CSliderCtrl m_ctlSlidProcWyprzedzenia1;
	CSliderCtrl m_ctlSlidProcWyprzedzenia2;
	BOOL m_bTrybRegulacjiWylaczony;
	BOOL m_bTrybRegulacjiReczny;
	BOOL m_bTrybRegulacjiAkro;
	BOOL m_bTrybRegulacjiStab;
	BOOL m_bTrybRegulacjiAuto;
	CString m_strSkalaWartZadanejStab;
	CString m_strSkalaWartZadanejAkro;
	CString m_strJednostkaStab;
	CString m_strJednostkaAkro;
	CString m_strJednostkaStab2;
	CComboBox m_ctlStrojonyParametr1;
	CComboBox m_ctlStrojonyParametr2;
	CString m_strWartoscMinParametru1;
	CString m_strWartoscMaxParametru1;
	CString m_strWartoscMinParametru2;
	CString m_strWartoscMaxParametru2;
	CString m_strOpisParametru1;
	CString m_strOpisParametru2;
	

public:
	afx_msg void OnTcnSelchangeTabKanalPid(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnChangeEditKp1();
	afx_msg void OnEnChangeEditKp2();
	afx_msg void OnEnChangeEditTi1();
	afx_msg void OnEnChangeEditTi2();
	afx_msg void OnEnChangeEditTd1();
	afx_msg void OnEnChangeEditTd2();
	afx_msg void OnEnChangeEditMinWy1();
	afx_msg void OnEnChangeEditMinWy2();
	afx_msg void OnEnChangeEditMaxWy1();	
	afx_msg void OnEnChangeEditMaxWy2();
	afx_msg void OnNMCustomdrawSliderFiltrD1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSliderFiltrD2(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnEnChangeEditLimitCalki1();
	afx_msg void OnEnChangeEditLimitCalki2();
	afx_msg void OnBnClickedCheckKatowy();
	afx_msg void OnNMReleasedcaptureSliderFiltrD1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMReleasedcaptureSliderFiltrD2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedRadioRegWylacz();
	afx_msg void OnBnClickedRadioRegReczny();
	afx_msg void OnBnClickedRadioRegAkro();
	afx_msg void OnBnClickedRadioRegStab();
	afx_msg void OnBnClickedRadioRegAuto();
	afx_msg void OnEnChangeEditSkalaWartZadStab();
	afx_msg void OnEnChangeEditSkalaWartZadAkro();
	afx_msg void OnBnClickedButUstawDomyslne();
	afx_msg void OnCbnSelchangeComboStrojonyParametr1();
	afx_msg void OnEnChangeEditParametrMin1();
	afx_msg void OnEnChangeEditParametrMax1();
	afx_msg void OnEnChangeEditParametrMin2();
	afx_msg void OnEnChangeEditParametrMax2();
	afx_msg void OnCbnSelchangeComboStrojonyParametr2();
	afx_msg void OnNMCustomdrawSliderFiltrWzad1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSliderFiltrWzad2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderFiltrWzad1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderFiltrWzad2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSliderProcWyprzedzenia2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSliderProcWyprzedzenia1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderProcWyprzedzenia1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderProcWyprzedzenia2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnChangeEditPrzesuniecieWartZadanej1();
	afx_msg void OnEnChangeEditPrzesuniecieWartZadanej2();
	afx_msg void OnStnClickedStaticFiltrD1();
	afx_msg void OnEnChangeEditKw1();
	afx_msg void OnEnChangeEditKw2();
};
