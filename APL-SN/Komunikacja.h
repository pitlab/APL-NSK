#pragma once

#include "Protokol.h"

class CKomunikacja
{
public:
	CKomunikacja();	//noexcept;
	virtual ~CKomunikacja();


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

	uint8_t PobierzNazweBSP(CString* strNazwa);
	uint8_t WyslijOK(uint8_t chAdrOdb);

	//struktura zbieraj¹ca parametry BSP
	struct _sWron
	{
		uint8_t chAdres;
		CString strNazwa;
	};
	static std::vector <_sWron> m_vRoj;		//wektor przechowuj¹cy wszystkie wrony w roju


private:
	//CProtokol m_cProto;
	static uint8_t WatekDekodujRamkiPolecen(LPVOID pParam);
	uint8_t WlasciwyWatekDekodujRamkiPolecen();
	static BOOL m_bKoniecWatkuDekoderaPolecen;
	BOOL m_bPolaczono = FALSE;
	static uint8_t m_chTypPolaczenia;
	uint32_t m_iNumerPortuETH;
	CString m_strAdresPortuETH;
	uint32_t m_iNumerPortuUART;
	uint32_t m_iPredkoscUART;
	uint8_t m_chRamkaWych[ROZM_DANYCH_WY_UART + ROZM_CIALA_RAMKI];
	CView* m_pWnd;
	CWinThread* pWskWatkuDekodujacego;
	

	//pola publiczne z odbieranymi danymi
public:
	CString m_strNazwa;
};

