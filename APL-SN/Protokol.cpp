#include "pch.h"
#include "Protokol.h"
#include "Errors.h"
#include "multithreading.h"



/*Struktura ramki komunikacyjnej
0xAA - Nag��wek.Strumie� danych wej�ciowych analizujemy pod k�tem obecno�ci preambu�y.
ADRES ODBIORCY - 0x00 stacja naziemna, 0xFF broadcast, pozosta�e to numery BSP w roju.Podczas odbioru w tym miejscu przestajemy analizowa� je�eli ramka nas nie dotyczy.Od tego miejsca mo�e zaczyna� si� szyfrowanie
ADERS NADAWCY - aby mo�na by�o zidentyfikowa� od kogo pochodzi ramka
TIMESTAMP - licznik setnych cz�ci sekundy po to aby mo�na by�o posk�ada� we w�a�ciwej kolejno�ci dane przesy�ane w wielu ramkach
POLECENIE - kod polecenia do wykonania. 
ROZMIAR - liczba bajt�w danych ramki
DANE - opcjonalne dane
CRC16 - suma kontrolna ramki od nag��wka do CRC16. Starszy przodem


Ramki wyst�puj� w dwu typach identyfikowanych najstarszym bitem pola POLECENIE: 
- Standardowa ramka wymaga odpowiedzi potwierdzajacej wys�ania
- Ramka telemetryczna nie wymaga odpowiedzi, podobnie jak broadcast
*/



CProtokol::CProtokol() 
	: m_iLecznikWejRamekTelemetrii(0)
	, m_iLecznikWejRamekZwyklych(0)
	, pWskWatkuSluchajacego(NULL)
	, m_bKoniecWatku(FALSE)
	, m_chStanProtCom(0)
{
	InitializeCriticalSection(&m_SekcjaKrytycznaPolecen);
	InitializeCriticalSection(&m_SekcjaKrytycznaTelemetrii);
}


CProtokol::~CProtokol()
{
	DeleteCriticalSection(&m_SekcjaKrytycznaPolecen);
	DeleteCriticalSection(&m_SekcjaKrytycznaTelemetrii);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// ��czy si� z portem komunikacyjnym o podanych parametrach
// [we] nTypPortu - okre�la rodzaj portu komunikacyjnego [UART, Ethernet, USB]
// [we] nNumerPortu - identyfikator portu [nr COM dla UART, gniazdo dla ethernet]
// [we] nPredkosc - pr�dko�� transmisji dla UART
// [we] pWnd - wska�nik na widok obiektu wywoluj�cego
// zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CProtokol::PolaczPort(int nTypPortu, int nNumerPortu, int nPredkosc, CString strAdres, CView* pWnd)
{
	uint8_t chErr = ERR_OK;
	DWORD dwErr;
	//LPVOID pWnd = NULL;

	switch (nTypPortu)
	{
	case UART:	
		chErr = m_cPortSzeregowy.Connect(nNumerPortu, nPredkosc);
		if (chErr == ERR_OK)
		{
			//utw�rz zdarzenie sygnalizuj�ce przyj�cie ramki
			m_hZdarzenieRamkaDanychGotowa = CreateEvent(NULL, false, false, _T("")); // auto-reset event, non-signalled state
			m_hZdarzenieRamkaTelemetriiGotowa = CreateEvent(NULL, false, false, _T("")); // auto-reset event, non-signalled state
			pWskWatkuSluchajacego = AfxBeginThread((AFX_THREADPROC)WatekSluchajPortuCom, (LPVOID)pWnd, THREAD_PRIORITY_ABOVE_NORMAL, 0, 0, NULL);
		}
		break;

	case USB:	break;
	case ETH:	
		m_sGniazdoSluchajace.UstawRodzica(pWnd);
		m_sGniazdoPolaczenia.UstawRodzica(pWnd);

		m_sGniazdoSluchajace.m_iPort = nNumerPortu;
		if (m_sGniazdoSluchajace.Create(nNumerPortu, SOCK_STREAM, FD_READ | FD_WRITE | FD_ACCEPT | FD_CLOSE, strAdres))
		{
			chErr = ERR_CANT_CONNECT;		//przechwyci� kod b��du
			dwErr = GetLastError();

			chErr = (uint8_t)dwErr;
		}
		//else
		{
			if (m_sGniazdoSluchajace.Listen())
			{
				dwErr = GetLastError();
				chErr = ERR_CANT_CONNECT;		//przechwyci� kod b��du
			}
			else
			{
				if (!m_sGniazdoSluchajace.m_bPolaczono)
					chErr = ERR_NOT_CONNECTED;
			}
		}
		break;
	}
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Generuje potwierdzenie nawi�zania po��czenia przez ethernet. Jest wywo�ywane z okna w�a�ciciela
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CProtokol::AkceptujPolaczenieETH(void)
{
	m_sGniazdoSluchajace.Accept(m_sGniazdoPolaczenia);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Generuje potwierdzenie odebrania danych przez ethernet. Jest wywo�ywane z okna w�a�ciciela
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CProtokol::OdbierzDaneETH(void)
{
	m_sOdebranoETH = m_sGniazdoPolaczenia.Receive(m_chBuforOdbiorczyETH, ROZM_DANYCH_WE_ETH);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Generuje potwierdzenie wys�ania danych przez ethernet. Jest wywo�ywane z okna w�a�ciciela
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CProtokol::WyslanoDaneETH(void)
{
	m_bWyslanoETH = TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Roz��cza si� z portem
// [we] nTypPortu - typ zamykanego portu
// zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CProtokol::ZamknijPort(int nTypPortu)
{
	uint8_t chErr = ERR_OK;
	
	switch(nTypPortu)
	{
	case UART:	
		chErr = m_cPortSzeregowy.Disconnect();	
		m_bKoniecWatku = TRUE;
		WaitForSingleObject(pWskWatkuSluchajacego, INFINITE); 
		break;

	case USB:	break;

	case ETH:	
		m_sGniazdoPolaczenia.Close();
		m_sGniazdoSluchajace.Close();	
		break;
	}
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wywo�uje w�tek s�uchaj�cy portu komunikacyjnego
// [we] pParam - typ zamykanego portu
// Zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CProtokol::WatekSluchajPortuCom(LPVOID pParam)
{
	return reinterpret_cast<CProtokol*>(pParam)->WlasciwyWatekSluchajPortuCom();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// W�tek s�uchaj�cy portu komunikacyjnego
// Zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CProtokol::WlasciwyWatekSluchajPortuCom()
{
	unsigned int x, iOdczytano;
	uint8_t chPolecenie, chDaneWe[ROZM_DANYCH_WE_UART + ROZM_CIALA_RAMKI];
	BOOL bRamkaTelemetrii;
	uint8_t chErr, chDaneWy[ROZM_DANYCH_WY_UART + ROZM_CIALA_RAMKI];
	unsigned int time;
	uint8_t	chStanProtokolu;
	uint8_t	chAdresNadawcy;
	uint8_t chZnakCzasu;
	uint8_t chIloscDanychRamki;
	uint8_t chOdbieranyBajt;
	uint16_t sCRC16;

	if (!m_cPortSzeregowy.GetConnectedState())
		return ERR_NOT_CONNECTED;

	while (!m_bKoniecWatku)
	{
		chErr = m_cPortSzeregowy.ReceiveData(chDaneWe, &iOdczytano, ROZM_DANYCH_WE_UART);
		if (chErr == ERR_OK)
		{
			//analizuj dane binarne bajt po bajcie
			for (x = 0; x < iOdczytano; x++)
			{
				chErr = AnalizujRamke(chDaneWe[x], &chStanProtokolu, &chAdresNadawcy, &chZnakCzasu, &chPolecenie, &chIloscDanychRamki, &chOdbieranyBajt, chDaneWy, &sCRC16);
				if ((chErr == ERR_OK) && (m_chStanProtCom == PR_ODB_NAGLOWKA) && (chIloscDanychRamki > -1))
				{
					bRamkaTelemetrii = (BOOL)(chPolecenie & 0x80);
					if (bRamkaTelemetrii)
						chPolecenie &= ~0x80;	//maska dla bitu telemetrii

					if (bRamkaTelemetrii)
					{
						EnterCriticalSection(&m_SekcjaKrytycznaTelemetrii);
						m_inputTelemetryData.push_back(BinaryFrame(m_iLecznikWejRamekTelemetrii++, chPolecenie, chZnakCzasu, chDaneWy, chIloscDanychRamki));
						LeaveCriticalSection(&m_SekcjaKrytycznaTelemetrii);
						SetEvent(m_hZdarzenieRamkaTelemetriiGotowa);

					}
					else
					{
						EnterCriticalSection(&m_SekcjaKrytycznaPolecen);
						if (!chIloscDanychRamki)	//wykrzacza si� na zerowych ramkach typu OK, wi�c zr�b niezerowy rozmiar danych
							chIloscDanychRamki = 1;
						m_inputAnswerData.push_back(BinaryFrame(m_iLecznikWejRamekZwyklych++, chPolecenie, chZnakCzasu, chDaneWy, chIloscDanychRamki));
						LeaveCriticalSection(&m_SekcjaKrytycznaPolecen);
						SetEvent(m_hZdarzenieRamkaDanychGotowa);
					}
				}
			}
		}
	}
	return ERR_OK;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Przygotowuje do wys�ania ramk� komunikacyjn�
// [i] chAdrNad - adres nadawcy (w�asny)
// [i] cAdrOdb - adres odbiorcy
// [i] chZnakCzasu - znacznik czasu
// [i] chPolecenie - polecenie do wykonania
// [i] chDane* - wska�nik na struktur� przekazywanych danych
// [i] chRozmiar - rozmiar danych
// [o] wskRamka* - wska�nik na bufor gotowej ramki
// Zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CProtokol::PrzygotujRamke(uint8_t chAdrOdb, uint8_t chAdrNad, uint8_t chZnakCzasu, uint8_t chPolecenie, uint8_t* chDane, uint8_t chRozmiar, uint8_t* wskRamka)
{
	uint8_t n;
	uint8_t* chKopiaDane = wskRamka;
	uint16_t chCRC = 0;

	*wskRamka++ = NAGLOWEK;
	*wskRamka++ = chAdrOdb;
	*wskRamka++ = chAdrNad;
	*wskRamka++ = chZnakCzasu;
	*wskRamka++ = chPolecenie;
	*wskRamka++ = chRozmiar;
	for (n = 0; n < chRozmiar; n++)
		*wskRamka++ = *chDane++;
		
	for (n=0; n< chRozmiar + ROZM_CIALA_RAMKI; n++)
		chCRC = LiczCRC16(*(chKopiaDane+n), chCRC);

	*wskRamka++ = chCRC >> 8;
	* wskRamka++ = chCRC & 0xFF;
	return ERR_OK;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Liczy wielomian CRC16
// [i] dane - liczba wejsciowa
// Zwraca: obliczony wielomian
///////////////////////////////////////////////////////////////////////////////////////////////////
uint16_t CProtokol::LiczCRC16(uint8_t dane, uint16_t crc)
{
	uint8_t n;

	crc = crc ^ ((uint16_t)dane << 8);
	for (n = 0; n < 8; n++) {
		if (crc & 0x8000)
			crc = (crc << 1) ^ WIELOMIAN_CRC;
		else
			crc <<= 1;
	}
	return crc;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wys�a ramk� komunikacyjn�
// Zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CProtokol::WyslijOdbierzRamke(uint8_t chAdrNad, uint8_t chAdrOdb, uint8_t chPolecenie, uint8_t* chDaneWe, uint8_t chRozmiarWe, uint8_t* chDaneWy, uint8_t* chRozmiarWy, uint32_t iCzasNaRamke)
{
	uint8_t* chKopiaDaneWy;
	uint8_t* chKopiaRozmiarWy;
	uint8_t chZnakczasu;
	BOOL bPolecenieGotowe = FALSE;
	BOOL bRamkaOK = FALSE;
	uint8_t chLicznikRetransmisji = 0;
	uint8_t chErr = ERR_OK;
	uint32_t nErr;
	int32_t x, iRozmiar, iNumer;
	uint8_t chRamka[ROZM_DANYCH_WE_UART + ROZM_CIALA_RAMKI];
	clock_t poczatek, koniec;
	std::vector< BinaryFrame > vPrzychodzaceRamki;
	uint32_t iCzasOczekiwania;

	//zachowaj wska�niki. B�d� potrzebne do retransmisji.
	chKopiaDaneWy = chDaneWy;
	chKopiaRozmiarWy = chRozmiarWy;

	chZnakczasu = clock() * 100 / CLOCKS_PER_SEC;	//czas w "tickach" setnych sekund
	PrzygotujRamke(chAdrNad, chAdrOdb, chZnakczasu, chPolecenie, chDaneWe, chRozmiarWe, chRamka);

	while (!bPolecenieGotowe && (chLicznikRetransmisji < TR_PROB_WYSLANIA))
	{
		//je�eli przed wys�aniem polecenia w buforze odbiorczym s� jakie� dane to wyczy�� je 
		iRozmiar = m_cPortSzeregowy.InBufSize();
		if (iRozmiar)
		{
			m_cPortSzeregowy.PurgeIncomingData();
			iRozmiar = m_cPortSzeregowy.InBufSize();	//test zaj�to�ci bufora odbiorczego po czyszczeniu
		}

		//je�eli ponowne wys�anie ramki
		if (chLicznikRetransmisji > 0)
		{
			//odtw�rz wska�niki
			chDaneWy = chKopiaDaneWy;
			chRozmiarWy = chKopiaRozmiarWy;
		}

		//wyslij ramk�
		chErr = m_cPortSzeregowy.SendFrame(chRamka, chRozmiarWe + ROZM_CIALA_RAMKI);
		if (chErr == ERR_OK)
		{
			poczatek = clock();
			do
			{
				//wersja na zdarzeniach - wychodzi na timeoucie
				nErr = WaitForSingleObject(m_hZdarzenieRamkaDanychGotowa, iCzasNaRamke);
				if (nErr == WAIT_TIMEOUT)
					nErr = GetLastError();	//breakpoint do testowania timeoutu

				//odbierz ramk�
				EnterCriticalSection(&m_SekcjaKrytycznaPolecen);
				vPrzychodzaceRamki.clear();
				vPrzychodzaceRamki.swap(m_inputAnswerData);
				LeaveCriticalSection(&m_SekcjaKrytycznaPolecen);
				koniec = clock();
				iCzasOczekiwania = (koniec - poczatek) / (CLOCKS_PER_SEC / 1000);	//timeout licz w ms

				//sprawd� wszystkie odebrane ramki kt�ra z nich ma taki sam TimeStamp jak nadawcza
				iRozmiar = vPrzychodzaceRamki.size();
				for (x = 0; x < iRozmiar; x++)
				{
					if (vPrzychodzaceRamki[x].time == chZnakczasu)	//por�wnuj czas
					{
						iNumer = x;	//zapami�taj indeks ramki
						bRamkaOK = TRUE;
					}
				}
			} while (!bRamkaOK && (iCzasOczekiwania < iCzasNaRamke));

			if (iCzasOczekiwania < iCzasNaRamke)
			{
				iRozmiar = vPrzychodzaceRamki[iNumer].data.size();
				*chRozmiarWy = iRozmiar;
				for (x = 0; x < iRozmiar; x++)
					*chDaneWy++ = vPrzychodzaceRamki[iNumer].data[x];
				bPolecenieGotowe = TRUE;
			}
			else
			{
				chLicznikRetransmisji++;
				chErr = ERR_TIMEOUT;
			}
		}
		else
			if (chErr == ERR_NOT_CONNECTED)
				return chErr;
	}
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Analizuje przychod�ce dane i sk�ada ramk� komunikacyjn�
// [i] chDaneWe - odebrany bajt wchodz�cy do analizy
// [o] *chStanProtokolu - zwraca wska�nik n informuj� jaka cz�� ramki obecnie jest dekodowana
// [o] *chAdresNadawcy - wska�nik na adres nadawcy
// [o] *chZnakCzasu - wska�nik na znacznik czasu ramki
// [o] *chPolecenie - wska�nik na polecenie w ramce
// [o] *chRozmiarDanychWy - wska�nik na rozmiar danych w ramce
// [o] *chWskOdbDanej - wska�nik na bie��co odbieran� dan�
// [o] *chDaneWy - wska�nik na odbierane dane
// [o] *sCRC16 - wska�nik na odebrany wielomian CRC16
// Zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CProtokol::AnalizujRamke(uint8_t chDaneWe, uint8_t* chStanProtokolu,  uint8_t* chAdresNadawcy,  uint8_t* chZnakCzasu, uint8_t* chPolecenie, uint8_t* chRozmiarDanychWy, uint8_t* chWskOdbDanej, uint8_t* chDaneWy, uint16_t* sCRC16)
{
	uint8_t chErr = ERR_OK;
	uint16_t sCRC16Liczone = 0;


	switch (*chStanProtokolu)
	{
	case PR_ODB_NAGLOWKA:	//testuj czy odebrano nag��wek
		if (chDaneWe == NAGLOWEK)
		{
			*chStanProtokolu = PR_ODB_ADR_ODBIORCY;
			sCRC16Liczone = LiczCRC16(chDaneWe, sCRC16Liczone);
		}
		else
			chErr = ERR_ZLY_NAGLOWEK;
		break;

	case PR_ODB_ADR_ODBIORCY:
		if ((chDaneWe == ADRES_STACJI) | (chDaneWe == ADRES_BROADCAST))
		{
			*chStanProtokolu = PR_ODB_ADR_NADAWCY;
			sCRC16Liczone = LiczCRC16(chDaneWe, sCRC16Liczone);
		}
		else    //dane nie s� dla nas wi�c zacznij nas�uchiwa� kolejnej ramki
		{
			*chStanProtokolu = PR_ODB_NAGLOWKA;
			chErr = ERR_ZLY_ADRESAT;
		}
		break;

	case PR_ODB_ADR_NADAWCY:
		*chStanProtokolu = PR_ODB_ZNAK_CZASU;
		*chAdresNadawcy = chDaneWe;
		sCRC16Liczone = LiczCRC16(chDaneWe, sCRC16Liczone);
		break;

	case PR_ODB_ZNAK_CZASU:
		*chStanProtokolu = PR_ODB_POLECENIE;
		*chZnakCzasu = chDaneWe;
		sCRC16Liczone = LiczCRC16(chDaneWe, sCRC16Liczone);
		break;

	case PR_ODB_POLECENIE:
		*chStanProtokolu = PR_ODB_ROZMIAR;
		*chPolecenie = chDaneWe;
		sCRC16Liczone = LiczCRC16(chDaneWe, sCRC16Liczone);
		break;

	case PR_ODB_ROZMIAR:
		*chStanProtokolu = PR_ODB_DANE;
		*chRozmiarDanychWy = chDaneWe;
		*chWskOdbDanej = 0;
		sCRC16Liczone = LiczCRC16(chDaneWe, sCRC16Liczone);
		break;

	case PR_ODB_DANE:
		*(chDaneWy + *chWskOdbDanej) = chDaneWe;
		(*chWskOdbDanej)++;
		sCRC16Liczone = LiczCRC16(chDaneWe, sCRC16Liczone);
		if (*chWskOdbDanej == *chRozmiarDanychWy)
		{
			*chStanProtokolu = PR_ODB_CRC16;
			*chWskOdbDanej = 0;
		}
		break;

	case PR_ODB_CRC16:
		if (*chWskOdbDanej == 0)
		{
			*sCRC16 = chDaneWe * 0x100;
			(*chWskOdbDanej)++;
			sCRC16Liczone = LiczCRC16(chDaneWe, sCRC16Liczone);
		}
		else
		{
			*sCRC16 += chDaneWe;
			*chStanProtokolu = PR_ODB_NAGLOWKA;
			*chWskOdbDanej = 0;			
			sCRC16Liczone = LiczCRC16(chDaneWe, sCRC16Liczone);
			if (*sCRC16 != sCRC16Liczone)
				chErr = ERR_CRC16;
		}	
		break;

	default:	break;
	}
	return chErr;
}

