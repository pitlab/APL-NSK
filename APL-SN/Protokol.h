#pragma once

#include "RS232.h"
#include "GniazdoSieci.h"
#include <queue>

#define UART	1
#define ETH		2
#define USB		3

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


class CProtokol //: public CAsyncSocket
{
public:
	CProtokol();	//noexcept;
	virtual ~CProtokol();

	struct BinaryFrame
	{
		BinaryFrame(int _frameNo, unsigned char _command, unsigned char _time, const void* _pData, size_t _dataLen) :
			frameNo(_frameNo), command(_command), time(_time), data(_dataLen)
		{
			memcpy(&data[0], _pData, _dataLen);
		}

		int					frameNo;	///< Kolejny numer ramki
		unsigned char		command;	///< Komenda
		unsigned char		time;		///< Znacznik czasu ramki
		std::vector< unsigned char > data;		///< Binarne dane ramki
	};

	
	//uint8_t PolaczPort(int nTypPortu, int nNumerPortu, int nPredkosc, CView* pWnd);
	uint8_t PolaczPort(int nTypPortu, int nNumerPortu, int nPredkosc, CString strAdres, CView* pWnd);
	uint8_t ZamknijPort(int nTypPortu);
	static uint8_t WatekSluchajPortuCom(LPVOID pParam);
	uint8_t PrzygotujRamke(uint8_t chAdrOdb, uint8_t chAdrNad, uint8_t chZnakCzasu, uint8_t chPolecenie, uint8_t* chDane, uint8_t chRozmiar, uint8_t* wskRamka);
	uint8_t WyslijOdbierzRamke(uint8_t chAdrNad, uint8_t chAdrOdb, uint8_t chPolecenie, uint8_t* chDaneWe, uint8_t chRozmiarWe, uint8_t* chDaneWy, uint8_t* chRozmiarWy, uint32_t iTimeOut = TR_TIMEOUT);
	uint8_t AnalizujRamke(uint8_t chDaneWe, uint8_t* chStanProtokolu, uint8_t* chAdresNadawcy, uint8_t* chZnakCzasu, uint8_t* chPolecenie, uint8_t* chRozmiarDanychWy, uint8_t* chWskOdbDanej, uint8_t* chDaneWy, uint16_t* sCRC16);
	void AkceptujPolaczenieETH(void);
	void OdbierzDaneETH(void);
	void WyslanoDaneETH(void);



private:
	CPortSzeregowy	m_cPortSzeregowy;
	uint8_t WlasciwyWatekSluchajPortuCom();
	uint16_t LiczCRC16(uint8_t dane, uint16_t crc);
	CGniazdoSieci	m_sGniazdoSluchajace;
	CGniazdoSieci	m_sGniazdoPolaczenia;
	HANDLE			m_hZdarzenieRamkaDanychGotowa;
	HANDLE			m_hZdarzenieRamkaTelemetriiGotowa;
	CWinThread*		pWskWatkuSluchajacego;
	uint8_t			m_chStanProtCom;		//bie¿¹cy stan protoko³u odbiorczego na porcie COM
	BOOL			m_bKoniecWatku;
	std::vector< BinaryFrame > m_inputTelemetryData;	//Kolejka danych telemetrycznych przychodz¹cych z urz¹dzenia.
	std::vector< BinaryFrame > m_inputAnswerData;		//Kolejka danych bêd¹cych odpowiedziami na polecenia  
	int				m_iLecznikWejRamekTelemetrii;		///< Zlicza przychodz¹ce ramki
	int				m_iLecznikWejRamekZwyklych;
	void getIncommingAnswerFrames(std::vector< BinaryFrame >& _frames);

	CRITICAL_SECTION m_SekcjaKrytycznaPolecen;		//Sekcja chroni¹ca dostêp do wektora danych poleceñ
	CRITICAL_SECTION m_SekcjaKrytycznaTelemetrii;	//Sekcja chroni¹ca dostêp do wektora danych telemetrycznych

	//uint32_t m_iPort = 4000;
	//CString m_strNazwa = (L"127.0.0.1");
	uint8_t			m_chBuforOdbiorczyETH[ROZM_DANYCH_WE_ETH];		//bufor odbiorczy ethernetu
	uint16_t		m_sOdebranoETH;		//iloœæ odebranych danych przez ethernet
	BOOL			m_bWyslanoETH;		//potwierdzenie wys³ania danych przez ethernet
};



