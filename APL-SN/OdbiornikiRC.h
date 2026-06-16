#pragma once
#include "Komunikacja/Komunikacja.h"
#include "sys_def_wspolny.h"
//#define CZESTOTLIWOSC_ODSWIEZANIA	10	//[Hz]


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

//definicje funkcji uruchamianych rozszerzonymi kanałami RC przechowywane w zmiennej chFunkcjaMinKanaluRC[] i chFunkcjaMaxKanaluRC[]
#define FRC_NIE_TOB_NIC			0
#define FRC_WLACZ_OD1			1	//aktywuj wyjście otwarty dren 1 sterowanie przez CM7
#define FRC_WLACZ_OD2			2	//aktywuj wyjście otwarty dren 2 sterowanie przez CM7
#define FRC_STROJ_PID_PARAM1	3	//kanał służy do zmiany wybranego parametru 1 regulatorów PID
#define FRC_STROJ_PID_PARAM2	4	//kanał służy do zmiany wybranego parametru 2 regulatorów PID
#define FRC_MOW_WYSOKOSC		5	//mów komunikat o wysokości
#define FRC_MOW_NAPIECIE		6
#define FRC_MOW_TEMPERAT		7
#define FRC_MOW_PREDKOSC		8
#define FRC_MOW_KIERUNEK		9
#define LICZBA_FUNKCJI_RC		10	//liczba dostępnych funkcji



// 
// 
// Okno dialogowe OdbiornikiRC

class OdbiornikiRC : public CDialogEx
{
	DECLARE_DYNAMIC(OdbiornikiRC)

public:
	OdbiornikiRC(CWnd* pParent = nullptr);   // konstruktor standardowy
	virtual ~OdbiornikiRC();

// Dane okna dialogowego
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ODBIORNIKI_RC };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // obsługa DDX/DDV
	uint8_t WstawDaneKanalow();
	uint16_t m_sBackupOkresuTelemetrii[LICZBA_ZMIENNYCH_TELEMETRYCZNYCH];
	BOOL m_bZmodyfikowanoTelemetrie;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CProgressCtrl m_ctlRC1Kan1;
	CProgressCtrl m_ctlRC1Kan2;
	CProgressCtrl m_ctlRC1Kan3;
	CProgressCtrl m_ctlRC1Kan4;
	CProgressCtrl m_ctlRC1Kan5;
	CProgressCtrl m_ctlRC1Kan6;
	CProgressCtrl m_ctlRC1Kan7;
	CProgressCtrl m_ctlRC1Kan8;
	CProgressCtrl m_ctlRC1Kan9;
	CProgressCtrl m_ctlRC1Kan10;
	CProgressCtrl m_ctlRC1Kan11;
	CProgressCtrl m_ctlRC1Kan12;
	CProgressCtrl m_ctlRC1Kan13;
	CProgressCtrl m_ctlRC1Kan14;
	CProgressCtrl m_ctlRC1Kan15;
	CProgressCtrl m_ctlRC1Kan16;
	CComboBox m_ctlOdbiornikRC1;
	CComboBox m_ctlOdbiornikRC2;
	CString strWejscie[KANALY_ODB_RC];
	BOOL m_bZmienionoUstawienie;
	BOOL m_bZmienionoKanalDrazkow;
	BOOL m_bZmienionoFunkcjeKanalow;
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComboRc1();
	afx_msg void OnCbnSelchangeComboRc2();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnCbnSelchangeCombo3();
	afx_msg void OnCbnSelchangeCombo4();
	afx_msg void OnCbnSelchangeCombo5();
	afx_msg void OnCbnSelchangeCombo6();
	afx_msg void OnCbnSelchangeCombo7();
	afx_msg void OnCbnSelchangeCombo8();
	afx_msg void OnCbnSelchangeCombo9();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCancel();
	CComboBox m_ctlKanalPrzechylenia;
	CComboBox m_ctlKanalPochylenia;
	CComboBox m_ctlKanalOdchylenia;
	CComboBox m_ctlKanalWysokosci;
	CComboBox m_ctlFunkcjaKanalu5;
	CComboBox m_ctlFunkcjaKanalu6;
	CComboBox m_ctlFunkcjaKanalu7;
	CComboBox m_ctlFunkcjaKanalu8;
	CComboBox m_ctlFunkcjaKanalu9;
	CComboBox m_ctlFunkcjaKanalu10;
	CComboBox m_ctlFunkcjaKanalu11;
	CComboBox m_ctlFunkcjaKanalu12;
	CComboBox m_ctlFunkcjaKanalu13;
	CComboBox m_ctlFunkcjaKanalu14;
	CComboBox m_ctlFunkcjaKanalu15;
	CComboBox m_ctlFunkcjaKanalu16;
	afx_msg void OnCbnSelchangeComboKanPrze();
	afx_msg void OnCbnSelchangeComboKanPoch1();
	afx_msg void OnCbnSelchangeComboKanOdch();
	afx_msg void OnCbnSelchangeComboKanWyso();
	afx_msg void OnCbnSelchangeComboKan5();
	afx_msg void OnCbnSelchangeComboKan6();
	afx_msg void OnCbnSelchangeComboKan7();
	afx_msg void OnCbnSelchangeComboKan8();
	afx_msg void OnCbnSelchangeComboKan9();
	afx_msg void OnCbnSelchangeComboKan10();
	afx_msg void OnCbnSelchangeComboKan11();
	afx_msg void OnCbnSelchangeComboKan12();
	afx_msg void OnCbnSelchangeComboKan13();
	afx_msg void OnCbnSelchangeComboKan14();
	afx_msg void OnCbnSelchangeComboKan15();
	afx_msg void OnCbnSelchangeComboKan16();
	afx_msg void OnBnClickedButNormalizuj();
};
