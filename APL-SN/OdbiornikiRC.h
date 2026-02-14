#pragma once
#include "Komunikacja/Komunikacja.h"
#define CZESTOTLIWOSC_ODSWIEZANIA	10	//[Hz]

#define PPM_MIN			1000    //wartość minimalna sygnału -125%
#define PPM_M100    	1100    //-100%
#define PPM_M90     	1140    //-90%
#define PPM_JALOWY  	1200    //obroty jałowe silników
#define PPM_M75     	1200    //-75%
#define PPM_M50     	1300    //-50%
#define PPM_M25     	1400    //-25%
#define PPM_M20     	1420    //-20%
#define PPM_NEUTR   	1500    //neutrum 1500us
#define PPM_P20     	1580    //+20%
#define PPM_P25     	1600    //+25%
#define PPM_P50     	1700    //+50%
#define PPM_P75     	1800    //+75%
#define PPM_P90     	1860    //+90%
#define PPM_P100    	1900    //+100%
#define PPM_MAX			2000    //wartość maksymalna sygnału +125%

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
	void UstawMinMax();
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

	CComboBox m_ctlOdbiornikRC1;
	CComboBox m_ctlOdbiornikRC2;
	CComboBox m_ctlTypWyjscia1;
	CComboBox m_ctlTypWyjscia2;
	CComboBox m_ctlTypWyjscia3;
	CComboBox m_ctlTypWyjscia4;
	CComboBox m_ctlTypWyjscia5;
	CComboBox m_ctlTypWyjscia6;
	CComboBox m_ctlTypWyjscia7;
	CComboBox m_ctlTypWyjscia8;
	CComboBox m_ctlTypWyjscia9_16;

	struct {
		CString strMinMax;
		uint16_t sMin;
		uint16_t sMax;
		BOOL bZmieniono;
	} m_stEkstrema[16];
	BOOL m_bZmienionoUstawienie;
	BOOL m_bZmienionoMinMax;
	BOOL m_bZmienionoKanalDrazkow;
	BOOL m_bZmienionoFunkcjeKanalow;
	BOOL m_bZmienionoFunkcjeWyjscRC;
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
	afx_msg void OnCbnSelchangeComboFunkcjaSerwa1();
	afx_msg void OnCbnSelchangeComboFunkcjaSerwa2();
	afx_msg void OnCbnSelchangeComboFunkcjaSerwa3();
	afx_msg void OnCbnSelchangeComboFunkcjaSerwa4();
	afx_msg void OnCbnSelchangeComboFunkcjaSerwa5();
	afx_msg void OnCbnSelchangeComboFunkcjaSerwa6();
	afx_msg void OnCbnSelchangeComboFunkcjaSerwa7();
	afx_msg void OnCbnSelchangeComboFunkcjaSerwa8();
	afx_msg void OnCbnSelchangeComboFunkcjaSerwa9();
	afx_msg void OnCbnSelchangeComboFunkcjaSerwa10();
	afx_msg void OnCbnSelchangeComboFunkcjaSerwa11();
	afx_msg void OnCbnSelchangeComboFunkcjaSerwa12();
	afx_msg void OnCbnSelchangeComboFunkcjaSerwa13();
	afx_msg void OnCbnSelchangeComboFunkcjaSerwa14();
	afx_msg void OnCbnSelchangeComboFunkcjaSerwa15();
	afx_msg void OnCbnSelchangeComboFunkcjaSerwa16();
};
