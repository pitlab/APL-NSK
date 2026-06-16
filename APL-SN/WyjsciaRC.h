#pragma once

//definicje funkcji wyjść RC
#define FSER_SILNIK1			0	//steruj silnikiem 1
#define FSER_SILNIK2			1
#define FSER_SILNIK3			2
#define FSER_SILNIK4			3
#define FSER_SILNIK5			4
#define FSER_SILNIK6			5
#define FSER_SILNIK7			6
#define FSER_SILNIK8			7
#define FSER_WE_RC1				8
#define FSER_WE_RC2				9
#define FSER_WE_RC3				10
#define FSER_WE_RC4				11
#define FSER_WE_RC5				12
#define FSER_WE_RC6				13
#define FSER_WE_RC7				14
#define FSER_WE_RC8				15
#define FSER_WE_RC9				16
#define FSER_WE_RC10			17
#define FSER_WE_RC11			18
#define FSER_WE_RC12			19
#define FSER_WE_RC13			20
#define FSER_WE_RC14			21
#define FSER_WE_RC15			22
#define FSER_WE_RC16			23


//definicje typów wyjść
#define TWY_IO					0	//wyjście skonfigurowane jako wjściowy port IO do debugowania algorytmów
#define TWY_SBUS				1	//wyjście S-Bus
#define TWY_ADC					2	//wejście ADC
#define TWY_ALTER				3	//funkcja alternatywna
#define TWY_SERWO_50HZ			4	//wyjście PWM 50Hz typowe dla serw
#define TWY_PWM_100HZ			5	//wyjście PWM 100Hz
#define TWY_PWM_200HZ			6	//wyjście PWM 200Hz
#define TWY_PWM_400HZ			7	//wyjście PWM 400Hz	
#define TWY_DSHOT_150			8	//wyjście DShot150
#define TWY_DSHOT_300			9	//wyjście DShot300
#define TWY_DSHOT_600			10	//wyjście DShot600
#define TWY_DSHOT_1200			11	//wyjście DShot1200
#define TWY_WS281X				12 //obsługa programowalnego LED RGB


// Okno dialogowe WyjsciaRC

class WyjsciaRC : public CDialogEx
{
	DECLARE_DYNAMIC(WyjsciaRC)

public:
	WyjsciaRC(CWnd* pParent = nullptr);   // konstruktor standardowy
	virtual ~WyjsciaRC();

// Dane okna dialogowego
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WYJSCIA_RC };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // obsługa DDX/DDV
	uint16_t m_sBackupOkresuTelemetrii[LICZBA_ZMIENNYCH_TELEMETRYCZNYCH];


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CProgressCtrl m_ctlSerwo1;
	CProgressCtrl m_ctlSerwo2;
	CProgressCtrl m_ctlSerwo3;
	CProgressCtrl m_ctlSerwo4;
	CProgressCtrl m_ctlSerwo5;
	CProgressCtrl m_ctlSerwo6;
	CProgressCtrl m_ctlSerwo7;
	CProgressCtrl m_ctlSerwo8;
	CProgressCtrl m_ctlSerwo9;
	CProgressCtrl m_ctlSerwo10;
	CProgressCtrl m_ctlSerwo11;
	CProgressCtrl m_ctlSerwo12;
	CProgressCtrl m_ctlSerwo13;
	CProgressCtrl m_ctlSerwo14;
	CProgressCtrl m_ctlSerwo15;
	CProgressCtrl m_ctlSerwo16;
	CComboBox m_ctlTypWyjscia1;
	CComboBox m_ctlTypWyjscia2;
	CComboBox m_ctlTypWyjscia3;
	CComboBox m_ctlTypWyjscia4;
	CComboBox m_ctlTypWyjscia5;
	CComboBox m_ctlTypWyjscia6;
	CComboBox m_ctlTypWyjscia7;
	CComboBox m_ctlTypWyjscia8;
	CComboBox m_ctlTypWyjscia9_16;
	CComboBox m_ctlFunkcjaWyjscia1;
	CComboBox m_ctlFunkcjaWyjscia2;
	CComboBox m_ctlFunkcjaWyjscia3;
	CComboBox m_ctlFunkcjaWyjscia4;
	CComboBox m_ctlFunkcjaWyjscia5;
	CComboBox m_ctlFunkcjaWyjscia6;
	CComboBox m_ctlFunkcjaWyjscia7;
	CComboBox m_ctlFunkcjaWyjscia8;
	CComboBox m_ctlFunkcjaWyjscia9;
	CComboBox m_ctlFunkcjaWyjscia10;
	CComboBox m_ctlFunkcjaWyjscia11;
	CComboBox m_ctlFunkcjaWyjscia12;
	CComboBox m_ctlFunkcjaWyjscia13;
	CComboBox m_ctlFunkcjaWyjscia14;
	CComboBox m_ctlFunkcjaWyjscia15;
	CComboBox m_ctlFunkcjaWyjscia16;
	virtual BOOL OnInitDialog();
	BOOL m_bZmodyfikowanoTelemetrie;
	BOOL m_bZmienionoUstawienie;
	BOOL m_bZmienionoFunkcjeWyjscRC;
	void AktywujComboFunkcjiWyjscia(int NrWyjscia);
	uint8_t WstawDaneKanalow();
	CString strWyjscie[KANALY_WYJSC_RC];
	afx_msg void OnBnClickedCancel();
	void PrzywrocTelemetrie(uint16_t* Backup);
	afx_msg void OnCbnSelchangeComboSerwo1();
	afx_msg void OnCbnSelchangeComboSerwo2();
	afx_msg void OnCbnSelchangeComboSerwo3();
	afx_msg void OnCbnSelchangeComboSerwo4();
	afx_msg void OnCbnSelchangeComboSerwo6();
	afx_msg void OnCbnSelchangeComboSerwo8();
	afx_msg void OnCbnSelchangeComboSerwo7();
	afx_msg void OnCbnSelchangeComboSerwo9();
	afx_msg void OnCbnSelchangeComboFunkcjaWyjscia1();
	afx_msg void OnCbnSelchangeComboFunkcjaWyjscia2();
	afx_msg void OnCbnSelchangeComboFunkcjaWyjscia3();
	afx_msg void OnCbnSelchangeComboFunkcjaWyjscia4();
	afx_msg void OnCbnSelchangeComboFunkcjaWyjscia5();
	afx_msg void OnCbnSelchangeComboFunkcjaWyjscia6();
	afx_msg void OnCbnSelchangeComboFunkcjaWyjscia7();
	afx_msg void OnCbnSelchangeComboFunkcjaWyjscia8();
	afx_msg void OnCbnSelchangeComboFunkcjaWyjscia9();
	afx_msg void OnCbnSelchangeComboFunkcjaWyjscia10();
	afx_msg void OnCbnSelchangeComboFunkcjaWyjscia11();
	afx_msg void OnCbnSelchangeComboFunkcjaWyjscia12();
	afx_msg void OnCbnSelchangeComboFunkcjaWyjscia13();
	afx_msg void OnCbnSelchangeComboFunkcjaWyjscia14();
	afx_msg void OnCbnSelchangeComboFunkcjaWyjscia15();
	afx_msg void OnCbnSelchangeComboFunkcjaWyjscia16();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCbnSelchangeComboSerwo5();
};
