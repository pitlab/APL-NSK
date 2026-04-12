#pragma once
#include "PortSzeregowy.h"
#include "GniazdoSieci.h"
#include <queue>
#include "KomunikatySieci.h"
#include "polecenia_komunikacyjne.h"
#include "../sys_def_wspolny.h"

#define PK_UST_ROZD_KAMERY	5


class CProtokol 
{
public:
	CProtokol();	
	virtual ~CProtokol();
	static std::vector <stTelemetria_t> m_vDaneTelemetryczne;	
	static std::vector <stRamka_t> m_vRamkaPolecenia;
	std::vector <float> m_vDaneFFT[LICZBA_TESTOW_FFT][LICZBA_ZMIENNYCH_FFT];
	static HANDLE m_hZdarzenieRamkaPolecenGotowa;
	static HANDLE m_hZdarzenieRamkaTelemetriiGotowa;
	static HANDLE m_hZdarzenieRamkaFFTGotowa;
	uint8_t PolaczPort(uint8_t chTypPortu, int nNumerPortu, int nPredkosc, CString strAdres, CView* pWnd);
	uint8_t ZamknijPort(uint8_t chTypPortu);
	 BOOL CzyPolaczonoEth();
	void OdbierzDaneETH();
	void WyslanoDaneETH();	
	void AkceptujPolaczenieETH();
	uint8_t WyslijOdbierzRamke(uint8_t chAdrOdb, uint8_t chAdrNad, uint8_t chPolecenie, uint8_t* chDaneWy, uint8_t chRozmiarWy, uint8_t* chDaneWe, uint8_t* chRozmiarWe, uint32_t iCzasNaRamke = TR_TIMEOUT);
	void UstawAdresy(uint8_t chAdresOdb, uint8_t chAdresNad) { m_chAdresOdbiorcy = chAdresOdb; m_chAdresNadawcy = chAdresNad; }
	uint8_t PrzygotujRamke(uint8_t chAdrOdb, uint8_t chAdrNad, uint8_t chPolecenie, uint8_t* chDane, uint8_t chRozmiar, uint8_t* wskRamka);
	uint8_t WyslijRamke(uint8_t chTypPortu, uint8_t* wskRamka, uint8_t chRozmiar);
	uint8_t OdbierzDane();
	int m_wskaznikInstancji;
	static int m_LicznikInstancji;
	struct stEkstremaTelemetrii_t
	{
		float fMin = WARTOSC_MAX;
		float fMax = WARTOSC_MIN;
	} m_stEkstremaTelemetrii[LICZBA_ZMIENNYCH_TELEMETRYCZNYCH];
	float Char2Float16(uint8_t* chDane);
	union _un8_32
	{
		float daneFloat;
		uint32_t dane32;
		uint16_t dane16[2];
		uint8_t dane8[4];
	} m_unia8_32;

private:
	static UINT WatekSluchajPortuCom(LPVOID pParam);
	static UINT WatekSluchajPortuEth(LPVOID pParam);
	void AnalizujOdebraneDane(uint8_t* chDaneWe, uint32_t iOdczytano);
	uint8_t AnalizujRamke(uint8_t chDaneWe, uint8_t* chStanProtokolu, uint8_t* chAdresNadawcy, uint8_t* chZnakCzasu, uint8_t* chPolecenie, uint8_t* chRozmiarDanychWy, uint8_t* chWskOdbDanej, uint8_t* chDaneWy, uint16_t* sCRC16);
	uint8_t CzyscBuforPortu(uint8_t chTypPortu);	
	clock_t m_Koniec;

private:
	uint8_t	m_chTypPortu;		//rodzaj nawi¹zanego po³¹czenia
	uint8_t WlasciwyWatekSluchajPortuCom();
	uint8_t WlasciwyWatekSluchajPortuEth();
	uint16_t LiczCRC16(uint8_t dane, uint16_t crc);
	static CGniazdoSieci m_cGniazdoSluchajace;
	static CGniazdoSieci m_cGniazdoPolaczenia;

	CWinThread*	pWskWatkuSluchajacegoUart;
	CWinThread*	pWskWatkuSluchajacegoEth;
	static BOOL	m_bKoniecWatkuUart;
	static BOOL	m_bKoniecWatkuEth;

	
	int	m_iLecznikWejRamekTelemetrii;		///< Zlicza przychodz¹ce ramki
	int	m_iLecznikWejRamekZwyklych;
	static CRITICAL_SECTION m_SekcjaKrytycznaPolecen;		//Sekcja chroni¹ca dostêp do wektora danych poleceñ
	static CRITICAL_SECTION m_SekcjaKrytycznaTelemetrii;	//Sekcja chroni¹ca dostêp do wektora danych telemetrycznych
	static CRITICAL_SECTION m_SekcjaKrytycznaFFT;			//Sekcja chroni¹ca dostêp do wektora danych FFT
	
	uint8_t	m_chBuforOdbiorczyETH[ROZM_DANYCH_ETH];		//bufor odbiorczy ethernetu
	uint32_t m_iOdebranoETH;		//iloœæ odebranych danych przez ethernet
	BOOL m_bWyslanoETH;		//potwierdzenie wys³ania danych przez ethernet
	static BOOL m_bPolaczonoEth;
	uint8_t	m_chStanProtokolu;
	uint8_t	m_chAdresOdbiorcy;
	uint8_t	m_chAdresNadawcy;
	uint8_t m_chZnakCzasu;
	uint8_t m_chIloscDanychRamki;
	uint8_t m_chOdbieranyBajt;
	uint16_t m_sCRC16;
	uint8_t m_chPolecenie;
	uint8_t m_chDaneWy[ROZMIAR_RAMKI_UART];
};

CProtokol& getProtokol();


