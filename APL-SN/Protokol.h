#pragma once

#include "RS232.h"
#include "GniazdoSieci.h"
#include <queue>
#include "KomunikatySieci.h"

#define UART	1
#define ETHS	2	//ethernet jako serwer
#define ETHK	3	//ethernet jako klient
#define USB		4

#define ROZM_DANYCH_WE_UART	128
#define ROZM_DANYCH_WY_UART	128
#define ROZM_DANYCH_WE_ETH	1024
#define ROZM_DANYCH_WY_ETH	1024
#define ROZM_CIALA_RAMKI	8
#define TR_TIMEOUT			150		//timeout ramki w ms
#define TR_PROB_WYSLANIA	3

//stany protoko³u odbioru ramki
#define PR_ODB_NAGLOWKA		0
#define PR_ODB_ADR_ODBIORCY	1
#define PR_ODB_ADR_NADAWCY	2
#define PR_ODB_ZNAK_CZASU	3
#define PR_ODB_POLECENIE	4
#define PR_ODB_ROZMIAR		5
#define PR_ODB_DANE			6
#define PR_ODB_CRC16		7

#define NAGLOWEK			0xAA
#define ADRES_STACJI		0x00
#define ADRES_BROADCAST		0xFF
#define WIELOMIAN_CRC	 0x1021

//nazwy poleceñ protoko³u
#define POL_OK			0	//akceptacja
#define POL_NIE			1	//brak akceptacji
#define POL_NAZWA		2	//przesy³ana nazwa



class CProtokol //: public CAsyncSocket
{
public:
	CProtokol();	//noexcept;
	virtual ~CProtokol();


	//std::vector< BinaryFrame > m_inputTelemetryData;	//Kolejka danych telemetrycznych przychodz¹cych z urz¹dzenia.
	//std::vector< BinaryFrame > m_inputAnswerData;		//Kolejka danych bêd¹cych odpowiedziami na polecenia  

	std::vector <_Ramka> m_vRamkaTelemetryczna;		//wektor do przechowywania ramek
	std::vector <_Ramka> m_vRamkaZOdpowiedzia;

	uint8_t PolaczPort(uint8_t chTypPortu, int nNumerPortu, int nPredkosc, CString strAdres, CView* pWnd);
	uint8_t ZamknijPort();
	void PolaczonoETH();
	//void OdbierzDaneETH();
	void OdbierzDaneETH();
	void WyslanoDaneETH();	
	void AkceptujPolaczenieETH();
	uint8_t WyslijOdbierzRamke(uint8_t chAdrOdb, uint8_t chAdrNad, uint8_t chPolecenie, uint8_t* chDaneWy, uint8_t chRozmiarWy, uint8_t* chDaneWe, uint8_t* chRozmiarWe, uint32_t iCzasNaRamke = TR_TIMEOUT);
	void UstawAdresy(uint8_t chAdresOdb, uint8_t chAdresNad) { m_chAdresOdbiorcy = chAdresOdb; m_chAdresNadawcy = chAdresNad; }

private:
	static uint8_t WatekSluchajPortuCom(LPVOID pParam);
	void AnalizujOdebraneDane(uint8_t* chDaneWe, uint32_t iOdczytano);
	uint8_t PrzygotujRamke(uint8_t chAdrOdb, uint8_t chAdrNad, uint8_t chZnakCzasu, uint8_t chPolecenie, uint8_t* chDane, uint8_t chRozmiar, uint8_t* wskRamka);
	uint8_t AnalizujRamke(uint8_t chDaneWe, uint8_t* chStanProtokolu, uint8_t* chAdresNadawcy, uint8_t* chZnakCzasu, uint8_t* chPolecenie, uint8_t* chRozmiarDanychWy, uint8_t* chWskOdbDanej, uint8_t* chDaneWy, uint16_t* sCRC16);
	uint8_t CzyscBuforPortu(uint8_t chTypPortu);
	uint8_t WyslijRamke(uint8_t chTypPortu, uint8_t* wskRamka, uint8_t chRozmiar);

private:
	uint8_t			m_chTypPortu;		//rodzaj nawi¹zanego po³¹czenia
	CPortSzeregowy	m_cPortSzeregowy;
	uint8_t WlasciwyWatekSluchajPortuCom();
	uint16_t LiczCRC16(uint8_t dane, uint16_t crc);
	CGniazdoSieci	m_cGniazdoSluchajace;
	CGniazdoSieci	m_cGniazdoPolaczenia;
	HANDLE			m_hZdarzenieRamkaDanychGotowa;
	HANDLE			m_hZdarzenieRamkaTelemetriiGotowa;
	//HANDLE			m_hZdarzenieNawiazanoPolaczenieETH;
	CWinThread*		pWskWatkuSluchajacego;
	BOOL			m_bKoniecWatku;
	
	int				m_iLecznikWejRamekTelemetrii;		///< Zlicza przychodz¹ce ramki
	int				m_iLecznikWejRamekZwyklych;
	//void getIncommingAnswerFrames(std::vector< BinaryFrame >& _frames);
	CRITICAL_SECTION m_SekcjaKrytycznaPolecen;		//Sekcja chroni¹ca dostêp do wektora danych poleceñ
	CRITICAL_SECTION m_SekcjaKrytycznaTelemetrii;	//Sekcja chroni¹ca dostêp do wektora danych telemetrycznych
	uint8_t			m_chBuforOdbiorczyETH[ROZM_DANYCH_WE_ETH];		//bufor odbiorczy ethernetu
	uint32_t		m_iOdebranoETH;		//iloœæ odebranych danych przez ethernet
	BOOL			m_bWyslanoETH;		//potwierdzenie wys³ania danych przez ethernet

	uint8_t	m_chStanProtokolu;
	uint8_t	m_chAdresOdbiorcy;
	uint8_t	m_chAdresNadawcy;
	uint8_t m_chZnakCzasu;
	uint8_t m_chIloscDanychRamki;
	uint8_t m_chOdbieranyBajt;
	uint16_t m_sCRC16;
	uint8_t m_chPolecenie;
	uint8_t m_chDaneWe[ROZM_DANYCH_WE_UART + ROZM_CIALA_RAMKI];
	uint8_t m_chDaneWy[ROZM_DANYCH_WY_UART + ROZM_CIALA_RAMKI];
};



