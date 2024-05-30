#pragma once
#include "PortSzeregowy.h"
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
#define TR_TIMEOUT			250		//timeout ramki w ms
#define TR_PROB_WYSLANIA	3

//stany protoko�u odbioru ramki
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

//nazwy polece� protoko�u
#define POL_OK					0x00	//akceptacja
#define POL_NIE					0x01	//brak akceptacji
#define POL_MOJE_ID				0x02	//urz�dzenie przedstawia si� w sieci
//#define POL_POBIERZ_NAZWE		0x03	//pobierz nazwe od BSP

//polecenia typu broadcast, bez odpowiedzi


class CProtokol //: public CAsyncSocket
{
public:
	CProtokol();	//noexcept;
	virtual ~CProtokol();
	//std::vector< BinaryFrame > m_inputTelemetryData;	//Kolejka danych telemetrycznych przychodz�cych z urz�dzenia.
	//std::vector< BinaryFrame > m_inputAnswerData;		//Kolejka danych b�d�cych odpowiedziami na polecenia 
	static std::vector <_Ramka> m_vRamkaTelemetryczna;		//wektor do przechowywania ramek
	static std::vector <_Ramka> m_vRamkaPolecenia;
	static HANDLE m_hZdarzenieRamkaPolecenGotowa;
	static HANDLE m_hZdarzenieRamkaTelemetriiGotowa;
	uint8_t PolaczPort(uint8_t chTypPortu, int nNumerPortu, int nPredkosc, CString strAdres, CView* pWnd);
	uint8_t ZamknijPort();
	void PolaczonoETH();
	void OdbierzDaneETH();
	void WyslanoDaneETH();	
	void AkceptujPolaczenieETH();
	uint8_t WyslijOdbierzRamke(uint8_t chAdrOdb, uint8_t chAdrNad, uint8_t chPolecenie, uint8_t* chDaneWy, uint8_t chRozmiarWy, uint8_t* chDaneWe, uint8_t* chRozmiarWe, uint32_t iCzasNaRamke = TR_TIMEOUT);
	void UstawAdresy(uint8_t chAdresOdb, uint8_t chAdresNad) { m_chAdresOdbiorcy = chAdresOdb; m_chAdresNadawcy = chAdresNad; }
	uint8_t PrzygotujRamke(uint8_t chAdrOdb, uint8_t chAdrNad, uint8_t chPolecenie, uint8_t* chDane, uint8_t chRozmiar, uint8_t* wskRamka);
	uint8_t WyslijRamke(uint8_t chTypPortu, uint8_t* wskRamka, uint8_t chRozmiar);
	uint8_t OdbierzDane();
	

private:
	static uint8_t WatekSluchajPortuCom(LPVOID pParam);
	static uint8_t WatekSluchajPortuEth(LPVOID pParam);
	void AnalizujOdebraneDane(uint8_t* chDaneWe, uint32_t iOdczytano);
	uint8_t AnalizujRamke(uint8_t chDaneWe, uint8_t* chStanProtokolu, uint8_t* chAdresNadawcy, uint8_t* chZnakCzasu, uint8_t* chPolecenie, uint8_t* chRozmiarDanychWy, uint8_t* chWskOdbDanej, uint8_t* chDaneWy, uint16_t* sCRC16);
	uint8_t CzyscBuforPortu(uint8_t chTypPortu);
	

private:
	uint8_t	m_chTypPortu;		//rodzaj nawi�zanego po��czenia
	uint8_t WlasciwyWatekSluchajPortuCom();
	uint8_t WlasciwyWatekSluchajPortuEth();
	uint16_t LiczCRC16(uint8_t dane, uint16_t crc);
	static CGniazdoSieci m_cGniazdoSluchajace;
	static CGniazdoSieci m_cGniazdoPolaczenia;

	CWinThread*	pWskWatkuSluchajacegoUart;
	CWinThread*	pWskWatkuSluchajacegoEth;
	static BOOL	m_bKoniecWatkuUart;
	static BOOL	m_bKoniecWatkuEth;

	
	int	m_iLecznikWejRamekTelemetrii;		///< Zlicza przychodz�ce ramki
	int	m_iLecznikWejRamekZwyklych;
	//void getIncommingAnswerFrames(std::vector< BinaryFrame >& _frames);
	static CRITICAL_SECTION m_SekcjaKrytycznaPolecen;		//Sekcja chroni�ca dost�p do wektora danych polece�
	static CRITICAL_SECTION m_SekcjaKrytycznaTelemetrii;	//Sekcja chroni�ca dost�p do wektora danych telemetrycznych
	uint8_t	m_chBuforOdbiorczyETH[ROZM_DANYCH_WE_ETH];		//bufor odbiorczy ethernetu
	uint32_t m_iOdebranoETH;		//ilo�� odebranych danych przez ethernet
	BOOL m_bWyslanoETH;		//potwierdzenie wys�ania danych przez ethernet

	uint8_t	m_chStanProtokolu;
	uint8_t	m_chAdresOdbiorcy;
	uint8_t	m_chAdresNadawcy;
	uint8_t m_chZnakCzasu;
	uint8_t m_chIloscDanychRamki;
	uint8_t m_chOdbieranyBajt;
	uint16_t m_sCRC16;
	uint8_t m_chPolecenie;
	uint8_t m_chDaneWy[ROZM_DANYCH_WY_UART + ROZM_CIALA_RAMKI];
};



CProtokol& getProtokol();


