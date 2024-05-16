#include "pch.h"
#include "Protokol.h"
#include "..\..\APL-SN\APL-SN\Errors.h"
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


//inicjuj zmienne statyczny u�ywane w�tkach odbiorczych
HANDLE			CProtokol::m_hZdarzenieRamkaPolecenGotowa = NULL;
HANDLE			CProtokol::m_hZdarzenieRamkaTelemetriiGotowa = NULL;
CGniazdoSieci	CProtokol::m_cGniazdoSluchajace;
CGniazdoSieci	CProtokol::m_cGniazdoPolaczenia;
BOOL			CProtokol::m_bKoniecWatkuUart = FALSE;
BOOL			CProtokol::m_bKoniecWatkuEth = FALSE;
CRITICAL_SECTION CProtokol::m_SekcjaKrytycznaPolecen;		//Sekcja chroni�ca dost�p do wektora danych polece�
CRITICAL_SECTION CProtokol::m_SekcjaKrytycznaTelemetrii;
std::vector <_Ramka> CProtokol::m_vRamkaTelemetryczna;		//wektor do przechowywania ramek
std::vector <_Ramka> CProtokol::m_vRamkaPolecenia;


CProtokol::CProtokol() 
	: m_iLecznikWejRamekTelemetrii(0)
	, m_iLecznikWejRamekZwyklych(0)
	, m_chStanProtokolu(0)
	, m_iOdebranoETH(0)
	, m_bWyslanoETH(FALSE)
	, m_chAdresOdbiorcy(0)
	, m_chAdresNadawcy(0)
	, m_chIloscDanychRamki(0)
	, m_chOdbieranyBajt(0)
	, m_chPolecenie(0)
	, m_chZnakCzasu(0)
	, m_sCRC16(0)
	, m_chTypPortu(0)
	, m_chDaneWy{0}
	, m_chBuforOdbiorczyETH{0}
{
	InitializeCriticalSection(&m_SekcjaKrytycznaPolecen);
	InitializeCriticalSection(&m_SekcjaKrytycznaTelemetrii);
	m_hZdarzenieRamkaPolecenGotowa = CreateEvent(NULL, false, false, _T("")); // auto-reset event, non-signalled state
	m_hZdarzenieRamkaTelemetriiGotowa = CreateEvent(NULL, false, false, _T("")); // auto-reset event, non-signalled state
}


CProtokol::~CProtokol()
{
	CloseHandle(m_hZdarzenieRamkaPolecenGotowa);
	CloseHandle(m_hZdarzenieRamkaTelemetriiGotowa);
	DeleteCriticalSection(&m_SekcjaKrytycznaPolecen);
	DeleteCriticalSection(&m_SekcjaKrytycznaTelemetrii);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// ��czy si� z portem komunikacyjnym o podanych parametrach
// [we] chTypPortu - okre�la rodzaj portu komunikacyjnego [UART, Ethernet, USB]
// [we] nNumerPortu - identyfikator portu [nr COM dla UART, gniazdo dla ethernet]
// [we] nPredkosc - pr�dko�� transmisji dla UART
// [we] pWnd - wska�nik na widok obiektu wywoluj�cego
// zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CProtokol::PolaczPort(uint8_t chTypPortu, int nNumerPortu, int nPredkosc, CString strAdres, CView* pWnd)
{
	uint8_t chErr = ERR_CANT_CONNECT;
	DWORD dwErr;

	m_chTypPortu = 0;	//niezainicjowany port

	//utw�rz zdarzenie sygnalizuj�ce przyj�cie ramki
	//m_hZdarzenieRamkaPolecenGotowa = CreateEvent(NULL, false, false, _T("")); // auto-reset event, non-signalled state
	//m_hZdarzenieRamkaTelemetriiGotowa = CreateEvent(NULL, false, false, _T("")); // auto-reset event, non-signalled state

	switch (chTypPortu)
	{
	case UART:
		chErr = m_cPortSzeregowy.Connect(nNumerPortu, nPredkosc);
		if (chErr == ERR_OK)
		{
			pWskWatkuSluchajacegoUart = AfxBeginThread((AFX_THREADPROC)WatekSluchajPortuCom, (LPVOID)pWnd, THREAD_PRIORITY_ABOVE_NORMAL, 0, 0, NULL);
			m_chTypPortu = UART;
		}
		break;

	case USB:	break;

	case ETHS:	//ethernet jako serwer
		m_cGniazdoSluchajace.UstawRodzica(pWnd);
		m_cGniazdoPolaczenia.UstawRodzica(pWnd);		//jest potrzebne do uruchomienia accept
		if (m_cGniazdoSluchajace.Create(nNumerPortu, SOCK_STREAM, FD_READ | FD_WRITE | FD_ACCEPT | FD_CLOSE, strAdres))
		{
			chErr = ERR_OK;
		}
		else
		{
			chErr = ERR_CANT_CONNECT;
			dwErr = GetLastError();
		}

		if (m_cGniazdoSluchajace.Listen())
		{
			m_chTypPortu = ETHS;
			pWskWatkuSluchajacegoEth = AfxBeginThread((AFX_THREADPROC)WatekSluchajPortuEth, (LPVOID)pWnd, THREAD_PRIORITY_ABOVE_NORMAL, 0, 0, NULL);
			chErr = ERR_OK;
		}
		else
		{
			chErr = ERR_CANT_CONNECT;
			dwErr = GetLastError();
		}
		break;

	case ETHK:	//ethernet jako klient
		//m_hZdarzenieNawiazanoPolaczenieETH = CreateEvent(NULL, false, false, _T("")); // auto-reset event, non-signalled state
		m_cGniazdoPolaczenia.UstawRodzica(pWnd);
		//if (m_cGniazdoPolaczenia == NULL)
		//{
			if (m_cGniazdoPolaczenia.Create())
			{
				chErr = ERR_OK;
			}
			else
			{
				chErr = ERR_CANT_CONNECT;
				dwErr = GetLastError();
			}
		//}

		if (m_cGniazdoPolaczenia.Connect(strAdres, nNumerPortu))
		{
			chErr = ERR_OK;
			dwErr = GetLastError();		
			
		}
		else
		{
			chErr = ERR_CANT_CONNECT;
			dwErr = GetLastError();
		}

		pWskWatkuSluchajacegoEth = AfxBeginThread((AFX_THREADPROC)WatekSluchajPortuEth, (LPVOID)pWnd, THREAD_PRIORITY_ABOVE_NORMAL, 0, 0, NULL);
		//nErr = WaitForSingleObject(m_hZdarzenieNawiazanoPolaczenieETH, 5000);
		//if (nErr == WAIT_TIMEOUT)
			//chErr = ERR_CANT_CONNECT;
	
		
		break;

	default: chErr = ERR_ZLY_TYP_PORTU;	break;
	}
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Odbiera dane na aktywnym interfejsie
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CProtokol::OdbierzDane()
{
	uint8_t chErr = ERR_OK;
	return chErr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Generuje potwierdzenie nawi�zania po��czenia przez ethernet. Jest wywo�ywane z okna w�a�ciciela
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CProtokol::AkceptujPolaczenieETH(void)
{
	m_cGniazdoSluchajace.Accept(m_cGniazdoPolaczenia);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Generuje potwierdzenie nawi�zania po��czenia przez ethernet. Jest wywo�ywane z okna w�a�ciciela
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CProtokol::PolaczonoETH()
{
	m_chTypPortu = ETHK;
	//SetEvent(m_hZdarzenieNawiazanoPolaczenieETH);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Generuje potwierdzenie odebrania danych przez ethernet. Jest wywo�ywane z okna w�a�ciciela
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////

void CProtokol::OdbierzDaneETH()
{

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
uint8_t CProtokol::ZamknijPort(void)
{
	uint8_t chErr = ERR_OK;
	
	switch(m_chTypPortu)
	{
	case UART:	
		chErr = m_cPortSzeregowy.Disconnect();	
		m_bKoniecWatkuUart = TRUE;
		WaitForSingleObject(pWskWatkuSluchajacegoUart, INFINITE);
		break;

	case ETHS:	
		if (m_cGniazdoSluchajace.m_bPolaczone)
			m_cGniazdoSluchajace.Close();	
		break;

	case ETHK:
		if (m_cGniazdoPolaczenia.m_bPolaczone)
			m_cGniazdoPolaczenia.Close();
		m_bKoniecWatkuEth = TRUE;
		WaitForSingleObject(pWskWatkuSluchajacegoEth, INFINITE);
		break;

	case USB:	break;

	default: chErr = ERR_ZLY_TYP_PORTU;	break;
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
	m_bKoniecWatkuUart = FALSE;
	return reinterpret_cast<CProtokol*>(pParam)->WlasciwyWatekSluchajPortuCom();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Wywo�uje w�tek s�uchaj�cy portu komunikacyjnego
// [we] pParam - typ zamykanego portu
// Zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CProtokol::WatekSluchajPortuEth(LPVOID pParam)
{
	m_bKoniecWatkuEth = FALSE;
	return reinterpret_cast<CProtokol*>(pParam)->WlasciwyWatekSluchajPortuEth();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// W�tek s�uchaj�cy portu Com
// Zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CProtokol::WlasciwyWatekSluchajPortuCom()
{
	unsigned int iOdczytano;
	uint8_t chErr;
	uint8_t chBuforOdb[ROZM_DANYCH_WE_UART + ROZM_CIALA_RAMKI];

	if (!m_cPortSzeregowy.GetConnectedState())
		return ERR_NOT_CONNECTED;

	while (!m_bKoniecWatkuUart)
	{
		chErr = m_cPortSzeregowy.ReceiveData(chBuforOdb, &iOdczytano, ROZM_DANYCH_WE_UART);
		if (chErr == ERR_OK)
		{
			//analizuj dane binarne bajt po bajcie
			AnalizujOdebraneDane(chBuforOdb, iOdczytano);
			
		}
	}
	return ERR_OK;
}




//////////////////////////////////////////////////////////////////////////////////////////////////
// W�tek s�uchaj�cy portu Ethernet
// Zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CProtokol::WlasciwyWatekSluchajPortuEth()
{
	unsigned int iOdczytano = 0;
	uint8_t chBuforOdb[ROZM_DANYCH_WE_ETH + ROZM_CIALA_RAMKI];

	WaitForSingleObject(m_cGniazdoSluchajace.m_hZdarzeniePolaczonoEth, INFINITE);		//czekaj na po��czenie	
	while (!m_bKoniecWatkuEth)
	{
		WaitForSingleObject(m_cGniazdoSluchajace.m_hZdarzenieOdebranoEth, INFINITE);		//czekaj na odbi�r danych

		//iOdczytano = m_cGniazdoSluchajace.Receive(chBuforOdb, ROZM_DANYCH_WE_ETH);
		iOdczytano = m_cGniazdoPolaczenia.Receive(chBuforOdb, ROZM_DANYCH_WE_ETH);
		if (iOdczytano != SOCKET_ERROR)		
		{
			//analizuj dane binarne bajt po bajcie
			AnalizujOdebraneDane(chBuforOdb, iOdczytano);
		}
	}
	return ERR_OK;
}




void CProtokol::AnalizujOdebraneDane(uint8_t* chDaneWe, uint32_t iOdczytano)
{
	unsigned int x, n;
	uint8_t chErr;
	BOOL bRamkaTelemetrii;
	_Ramka sRamka;

	for (n = 0; n < iOdczytano; n++)
	{
		chErr = AnalizujRamke(chDaneWe[n], &m_chStanProtokolu, &m_chAdresNadawcy, &m_chZnakCzasu, &m_chPolecenie, &m_chIloscDanychRamki, &m_chOdbieranyBajt, m_chDaneWy, &m_sCRC16);
		if ((chErr == ERR_OK) && (m_chStanProtokolu == PR_ODB_NAGLOWKA) && (m_chIloscDanychRamki > -1))
		{
			bRamkaTelemetrii = (BOOL)(m_chPolecenie & 0x80);
			if (bRamkaTelemetrii)
				m_chPolecenie &= ~0x80;	//maska dla bitu telemetrii

			if (bRamkaTelemetrii)
			{
				EnterCriticalSection(&m_SekcjaKrytycznaTelemetrii);
				//m_inputTelemetryData.push_back(BinaryFrame(m_iLecznikWejRamekTelemetrii++, m_chPolecenie, m_chZnakCzasu, m_chDaneWy, m_chIloscDanychRamki));

				//zbierz dane ramki do struktury
				sRamka.chPolecenie = m_chPolecenie;
				sRamka.chZnakCzasu = m_chZnakCzasu;
				sRamka.chRozmiar = m_chIloscDanychRamki;
				sRamka.chAdrNadawcy = m_chAdresNadawcy;
				for (x = 0; x < m_chIloscDanychRamki; x++)
					sRamka.dane.push_back(m_chDaneWy[x]);

				//wstaw struktur� do wektora
				m_vRamkaTelemetryczna.push_back(sRamka);
				LeaveCriticalSection(&m_SekcjaKrytycznaTelemetrii);
				SetEvent(m_hZdarzenieRamkaTelemetriiGotowa);
			}
			else
			{
				EnterCriticalSection(&m_SekcjaKrytycznaPolecen);
				if (!m_chIloscDanychRamki)	//wykrzacza si� na zerowych ramkach typu OK, wi�c zr�b niezerowy rozmiar danych
					m_chIloscDanychRamki = 1;
				//m_inputAnswerData.push_back(BinaryFrame(m_iLecznikWejRamekZwyklych++, m_chPolecenie, m_chZnakCzasu, m_chDaneWy, m_chIloscDanychRamki));
				//zbierz dane ramki do struktury
				sRamka.chPolecenie = m_chPolecenie;
				sRamka.chZnakCzasu = m_chZnakCzasu;
				sRamka.chRozmiar = m_chIloscDanychRamki;
				sRamka.chAdrNadawcy = m_chAdresNadawcy;
				for (x = 0; x < m_chIloscDanychRamki; x++)
					sRamka.dane.push_back(m_chDaneWy[x]);

				//wstaw struktur� do wektora
				m_vRamkaPolecenia.push_back(sRamka);
				LeaveCriticalSection(&m_SekcjaKrytycznaPolecen);
				SetEvent(m_hZdarzenieRamkaPolecenGotowa);
			}
		}
	}
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
uint8_t CProtokol::PrzygotujRamke(uint8_t chAdrOdb, uint8_t chAdrNad, uint8_t chPolecenie, uint8_t* chDane, uint8_t chRozmiar, uint8_t* wskRamka)
{
	uint8_t n;
	uint8_t* chKopiaDane = wskRamka;
	uint16_t chCRC = 0;

	*wskRamka++ = NAGLOWEK;
	*wskRamka++ = chAdrOdb;
	*wskRamka++ = chAdrNad;
	*wskRamka++ = (uint8_t)(clock() * 100 / CLOCKS_PER_SEC);	//czas w "tickach" setnych sekund;
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
// Wys�a ramk� komunikacyjn� przez UART
// [i] chAdrNad - adres nadawcy (w�asny)
// [i] cAdrOdb - adres odbiorcy
// [i] chPolecenie - polecenie do wykonania
// [i] chDaneWy* - wska�nik na struktur� danych do wys�ania
// [i] chRozmiarWy - rozmiar danych do wys�ania
// [i] chDaneWe* - wska�nik na struktur� danych do odebrania
// [i] chRozmiarWe* - wska�nik na rozmiar odebranych danych
// [i] iCzasNaRamke - dopuszczalny czas przeznaczony na wys�anie ramki
// Zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CProtokol::WyslijOdbierzRamke(uint8_t chAdrOdb, uint8_t chAdrNad, uint8_t chPolecenie, uint8_t* chDaneWy, uint8_t chRozmiarWy, uint8_t* chDaneWe, uint8_t* chRozmiarWe, uint32_t iCzasNaRamke)
{
	uint8_t* chKopiaDaneWe;
	uint8_t* chKopiaRozmiarWe;
	BOOL bPolecenieGotowe = FALSE;
	BOOL bRamkaOK = FALSE;
	uint8_t chLicznikRetransmisji = 0;
	uint8_t chErr = ERR_OK;
	uint32_t nErr;
	int32_t x, iRozmiar, iNumer;
	uint8_t chRamka[ROZM_DANYCH_WE_UART + ROZM_CIALA_RAMKI];
	clock_t poczatek, koniec;
//	std::vector< BinaryFrame > vPrzychodzaceRamki;
	uint32_t iCzasOczekiwania;

	//zachowaj wska�niki. B�d� potrzebne do retransmisji.
	chKopiaDaneWe = chDaneWe;
	chKopiaRozmiarWe = chRozmiarWe;

	
	PrzygotujRamke(chAdrOdb, chAdrNad, chPolecenie, chDaneWy, chRozmiarWy, chRamka);

	while (!bPolecenieGotowe && (chLicznikRetransmisji < TR_PROB_WYSLANIA))
	{
		//je�eli przed wys�aniem polecenia w buforze odbiorczym s� jakie� dane to wyczy�� je 
		chErr = CzyscBuforPortu(m_chTypPortu);
		if (chErr != ERR_OK)
			return chErr;
		
		//je�eli ponowne wys�anie ramki
		if (chLicznikRetransmisji > 0)
		{
			//odtw�rz wska�niki
			chDaneWe = chKopiaDaneWe;
			chRozmiarWe = chKopiaRozmiarWe;
		}

		//wyslij ramk�
		chErr = WyslijRamke(m_chTypPortu, chRamka, chRozmiarWy + ROZM_CIALA_RAMKI);
		if (chErr == ERR_OK)
		{
			//gdy przyjdzie odpowied� to zostanie wygenerowane zdarzenie: m_hZdarzenieRamkaDanychGotowa. Czekaj na nie
			poczatek = clock();
			do
			{
				//wersja na zdarzeniach - wychodzi na timeoucie
				nErr = WaitForSingleObject(m_hZdarzenieRamkaPolecenGotowa, iCzasNaRamke);
				if (nErr == WAIT_TIMEOUT)
					nErr = GetLastError();	//breakpoint do testowania timeoutu

				//m_iOdebranoETH = m_cGniazdoPolaczenia.Receive(m_chBuforOdbiorczyETH, ROZM_DANYCH_WE_ETH);


				koniec = clock();
				iCzasOczekiwania = (koniec - poczatek) / (CLOCKS_PER_SEC / 1000);	//timeout licz w ms

				//sprawd� wszystkie odebrane ramki kt�ra z nich ma taki sam TimeStamp jak nadawcza
				//iRozmiar = (uint32_t)vPrzychodzaceRamki.size();
				iRozmiar = (uint32_t)m_vRamkaPolecenia.size();
				for (x = 0; x < iRozmiar; x++)
				{
					//if (vPrzychodzaceRamki[x].time == chZnakczasu)	//por�wnuj czas
					if (m_vRamkaPolecenia[x].chZnakCzasu == chRamka[PR_ODB_ZNAK_CZASU])	//por�wnuj czas
					{
						iNumer = x;	//zapami�taj indeks ramki
						bRamkaOK = TRUE;
					}
				}
			} while (!bRamkaOK && (iCzasOczekiwania < iCzasNaRamke));

			if (iCzasOczekiwania < iCzasNaRamke)
			{
				//iRozmiar = (uint32_t)vPrzychodzaceRamki[iNumer].data.size();
				iRozmiar = (uint32_t)m_vRamkaPolecenia[iNumer].dane.size();
				*chRozmiarWe = iRozmiar;
				for (x = 0; x < iRozmiar; x++)
					//*chDaneWe++ = vPrzychodzaceRamki[iNumer].data[x];
					*chDaneWe++ = m_vRamkaPolecenia[iNumer].dane[x];
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
// Czy�ci bufory transmisyjne portu
// [we] chTypPortu - okre�la rodzaj portu komunikacyjnego [UART, Ethernet, USB]
// Zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CProtokol::CzyscBuforPortu(uint8_t chTypPortu)
{
	uint8_t chErr = ERR_OK;
	int32_t iRozmiar;

	switch (chTypPortu)
	{
	case UART:
		iRozmiar = m_cPortSzeregowy.InBufSize();
		if (iRozmiar)
		{
			m_cPortSzeregowy.PurgeIncomingData();
			iRozmiar = m_cPortSzeregowy.InBufSize();	//test zaj�to�ci bufora odbiorczego po czyszczeniu
		}
		break;
	case ETHK:		
		break;

	default: chErr = ERR_ZLY_TYP_PORTU;	break;
	}
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// wysy�a ramk� przez port
// [we] chTypPortu - okre�la rodzaj portu komunikacyjnego [UART, Ethernet, USB]
// [we] *wskRamka - wska�nik na gotow� ramk� komunikacyjn�
// [we] chRozmiar - rozmiar ramki komunikacyjnej
// Zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CProtokol::WyslijRamke(uint8_t chTypPortu, uint8_t* wskRamka, uint8_t chRozmiar)
{
	uint8_t chErr = ERR_OK;
	int32_t iWyslanych;

	switch (chTypPortu)
	{
	case UART:	chErr = m_cPortSzeregowy.SendFrame(wskRamka, chRozmiar); break;
	case ETHS:
	case ETHK:	iWyslanych = m_cGniazdoPolaczenia.Send(wskRamka, chRozmiar); 
		if (iWyslanych == SOCKET_ERROR)
			chErr = ERR_SEND_DATA;
		break;

	default: chErr = ERR_ZLY_TYP_PORTU;	break;
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

	switch (*chStanProtokolu)
	{
	case PR_ODB_NAGLOWKA:	//testuj czy odebrano nag��wek
		if (chDaneWe == NAGLOWEK)
		{
			*chStanProtokolu = PR_ODB_ADR_ODBIORCY;
			m_sCRC16 = LiczCRC16(chDaneWe, 0);	//inicjuj CRC
		}
		else
			chErr = ERR_ZLY_NAGLOWEK;
		break;

	case PR_ODB_ADR_ODBIORCY:
		if ((chDaneWe == m_chAdresOdbiorcy) | (chDaneWe == ADRES_BROADCAST))
		{
			*chStanProtokolu = PR_ODB_ADR_NADAWCY;
			m_sCRC16 = LiczCRC16(chDaneWe, m_sCRC16);
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
		m_sCRC16 = LiczCRC16(chDaneWe, m_sCRC16);
		break;

	case PR_ODB_ZNAK_CZASU:
		*chStanProtokolu = PR_ODB_POLECENIE;
		*chZnakCzasu = chDaneWe;
		m_sCRC16 = LiczCRC16(chDaneWe, m_sCRC16);
		break;

	case PR_ODB_POLECENIE:
		*chStanProtokolu = PR_ODB_ROZMIAR;
		*chPolecenie = chDaneWe;
		m_sCRC16 = LiczCRC16(chDaneWe, m_sCRC16);
		break;

	case PR_ODB_ROZMIAR:
		*chStanProtokolu = PR_ODB_DANE;
		*chRozmiarDanychWy = chDaneWe;
		*chWskOdbDanej = 0;
		m_sCRC16 = LiczCRC16(chDaneWe, m_sCRC16);
		break;

	case PR_ODB_DANE:
		*(chDaneWy + *chWskOdbDanej) = chDaneWe;
		(*chWskOdbDanej)++;
		m_sCRC16 = LiczCRC16(chDaneWe, m_sCRC16);
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
		}
		else
		{
			*sCRC16 += chDaneWe;
			*chStanProtokolu = PR_ODB_NAGLOWKA;
			*chWskOdbDanej = 0;			
			if (*sCRC16 != m_sCRC16)
				chErr = ERR_CRC16;
		}	
		break;

	default: chErr = ERR_ZLY_STAN_PROT;	break;
	}
	return chErr;
}

