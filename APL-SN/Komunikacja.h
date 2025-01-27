#pragma once

//#include "Protokol.h"
#define WIELKOSC_ROJU	4
#define LICZBA_PROB_ZANIM_ZGLOSI_BLAD	10

class CKomunikacja
{
public:
	CKomunikacja();	//noexcept;
	virtual ~CKomunikacja();
	static HANDLE m_hZdarzeniePaczkaDanych;

	void UstawTypPolaczenia(uint8_t chTyp) { m_chTypPolaczenia = chTyp; }
	uint8_t Polacz(CView* pWnd);
	uint8_t Rozlacz();
	void AkceptujPolaczenieETH();
	void OdebranoDaneETH();
	void WyslanoDaneETH();

	void UstawRodzica(CView* pWnd);
	void UstawNumerPortuETH(uint32_t iNumer) { m_iNumerPortuETH = iNumer; }
	uint32_t PobierzNumerPortuETH() { return m_iNumerPortuETH; }
	void UstawAdresPortuETH(CString strAdres) { m_strAdresPortuETH = strAdres; }
	CString PobierzAdresPortuETH() { return m_strAdresPortuETH; }
	void UstawNumerPortuUART(uint32_t iNumer) { m_iNumerPortuUART = iNumer; }
	uint32_t PobierzNumerPortuUART() { return m_iNumerPortuUART; }
	void UstawPredkoscPortuUART(uint32_t iPredkosc) { m_iPredkoscUART = iPredkosc; }
	uint32_t PobierzPredkoscPortuUART() { return m_iPredkoscUART; }
	BOOL CzyPolaczonoUart() { return m_bPolaczonoUart; }
	BOOL CzyPolaczonoEth() { return m_bPolaczonoEth; }
	uint8_t PobierzNazweBSP(CString* strNazwa);
	uint8_t WyslijOK();
	uint8_t ZrobZdjecie(uint16_t* sBuforZdjecia);
	uint8_t PobierzKamere(uint8_t* chSzerWy, uint8_t* chWysWy, uint8_t* chSzerWe, uint8_t* chWysWe, uint8_t* chTrybDiagn, uint8_t* chFlagi);
	uint8_t UstawKamere(uint8_t chSzerWy, uint8_t chWysWy, uint8_t chSzerWe, uint8_t chWysWe, uint8_t chTrybDiagn, uint8_t chFlagi);
	uint8_t ZapiszFlash(uint32_t nAdresPamieci);
	uint8_t SkasujSektorFlash(uint32_t nAdresPamieci);
	//uint8_t ZapiszBuforFlash(uint16_t sAdresBufora, uint16_t* sDane, uint8_t chRozmiar);
	uint8_t ZapiszBuforFlash(uint16_t sAdresBufora, uint8_t* chDane, uint8_t chRozmiar);
	uint8_t CzytajFlash(uint32_t nAdresPamieci, uint16_t* sDane, uint8_t chRozmiar);

	//struktura zbierająca parametry BSP
	struct _sWron
	{
		uint8_t chAdres;
		CString strNazwa;
	} m_stWron[WIELKOSC_ROJU];
	//static std::vector <_sWron> m_vRoj;		//wektor przechowujący wszystkie wrony w roju 
	union _un8_16
	{
		uint16_t dane16;
		uint8_t dane8[2];
	} m_unia8_16;

	union _un8_32
	{
		uint32_t dane32;
		uint16_t dane16[2];
		uint8_t dane8[4];
	} m_unia8_32;

private:
	static uint8_t WatekDekodujRamkiPolecen(LPVOID pParam);
	uint8_t WlasciwyWatekDekodujRamkiPolecen();
	static BOOL m_bKoniecWatkuDekoderaPolecen;
	BOOL m_bPolaczonoUart;
	BOOL m_bPolaczonoEth;
	static uint8_t m_chTypPolaczenia;
	uint32_t m_iNumerPortuETH;
	CString m_strAdresPortuETH;
	uint32_t m_iNumerPortuUART;
	uint32_t m_iPredkoscUART;
	uint8_t m_chRamkaWych[260];
	uint8_t m_chRamkaPrzy[260];
	CView* m_pWnd;
	CWinThread* pWskWatkuDekodujacego;
	


	//pola publiczne z odbieranymi danymi
public:
	CString m_strNazwa;
	uint8_t m_chAdresAutopilota;
	
};

CKomunikacja& getKomunikacja();
