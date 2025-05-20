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
std::vector <_Telemetria> CProtokol::m_vRamkaTelemetryczna;		//wektor do przechowywania ramek
std::vector <_Ramka> CProtokol::m_vRamkaPolecenia;
int				CProtokol::m_LicznikInstancji = 0;

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
	, m_wskaznikInstancji(0)
{
	m_wskaznikInstancji = m_LicznikInstancji++;
	InitializeCriticalSection(&m_SekcjaKrytycznaPolecen);
	InitializeCriticalSection(&m_SekcjaKrytycznaTelemetrii);
	m_hZdarzenieRamkaPolecenGotowa = CreateEvent(NULL, false, false, _T("RamPol")); // auto-reset event, non-signalled state
	m_hZdarzenieRamkaTelemetriiGotowa = CreateEvent(NULL, false, false, _T("RamTel")); // auto-reset event, non-signalled state
}


CProtokol::~CProtokol()
{
	m_wskaznikInstancji = --m_wskaznikInstancji;
	if (m_hZdarzenieRamkaPolecenGotowa)
	{
		CloseHandle(m_hZdarzenieRamkaPolecenGotowa);
		m_hZdarzenieRamkaPolecenGotowa = NULL;
	}
	if (m_hZdarzenieRamkaTelemetriiGotowa)
	{
		CloseHandle(m_hZdarzenieRamkaTelemetriiGotowa);
		m_hZdarzenieRamkaTelemetriiGotowa = NULL;
	}
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

	switch (chTypPortu)
	{
	case UART:
		chErr = getPortSzeregowy().Connect(nNumerPortu, nPredkosc);
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
			//pWskWatkuSluchajacegoEth = AfxBeginThread((AFX_THREADPROC)WatekSluchajPortuEth, (LPVOID)pWnd, THREAD_PRIORITY_ABOVE_NORMAL, 0, 0, NULL);
			pWskWatkuSluchajacegoEth = AfxBeginThread(WatekSluchajPortuEth, (LPVOID)pWnd, THREAD_PRIORITY_ABOVE_NORMAL, 0, 0, NULL);
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

		//pWskWatkuSluchajacegoEth = AfxBeginThread((AFX_THREADPROC)WatekSluchajPortuEth, (LPVOID)pWnd, THREAD_PRIORITY_ABOVE_NORMAL, 0, 0, NULL);
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
uint8_t CProtokol::ZamknijPort(uint8_t chTypPortu)
{
	uint8_t chErr = ERR_OK;
	
	switch(chTypPortu)
	{
	case UART:	
		chErr = getPortSzeregowy().Disconnect();
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
UINT CProtokol::WatekSluchajPortuCom(LPVOID pParam)
{
	m_bKoniecWatkuUart = FALSE;
	return reinterpret_cast<CProtokol*>(pParam)->WlasciwyWatekSluchajPortuCom();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wywo�uje w�tek s�uchaj�cy portu komunikacyjnego
// [we] pParam - typ zamykanego portu
// Zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
UINT CProtokol::WatekSluchajPortuEth(LPVOID pParam)
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
	uint8_t chBuforOdb[ROZMIAR_RAMKI_UART];

	
	if (!getPortSzeregowy().GetConnectedState())
		return ERR_NOT_CONNECTED;

	while (!m_bKoniecWatkuUart)
	{
		chErr = getPortSzeregowy().ReceiveData(chBuforOdb, &iOdczytano, ROZM_DANYCH_UART);
		if ((chErr == ERR_OK) && (iOdczytano))
		{
			//analizuj dane binarne bajt po bajcie
			getProtokol().AnalizujOdebraneDane(chBuforOdb, iOdczytano);
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
	uint8_t chBuforOdb[ROZM_DANYCH_ETH + ROZM_CIALA_RAMKI];

	WaitForSingleObject(m_cGniazdoSluchajace.m_hZdarzeniePolaczonoEth[0], INFINITE);		//czekaj na po��czenie	
	while (!m_bKoniecWatkuEth)
	{
		WaitForSingleObject(m_cGniazdoSluchajace.m_hZdarzenieOdebranoEth[0], INFINITE);		//czekaj na odbi�r danych

		//iOdczytano = m_cGniazdoSluchajace.Receive(chBuforOdb, ROZM_DANYCH_WE_ETH);
		iOdczytano = m_cGniazdoPolaczenia.Receive(chBuforOdb, ROZM_DANYCH_ETH);
		if (iOdczytano != SOCKET_ERROR)		
		{
			//analizuj dane binarne bajt po bajcie
			getProtokol().AnalizujOdebraneDane(chBuforOdb, iOdczytano);
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
	_Telemetria sTelemetria;
	float fTemp;

	TRACE("odczytano %d\n", iOdczytano);
	for (n = 0; n < iOdczytano; n++)
	{		
		chErr = AnalizujRamke(chDaneWe[n], &m_chStanProtokolu, &m_chAdresNadawcy, &m_chZnakCzasu, &m_chPolecenie, &m_chIloscDanychRamki, &m_chOdbieranyBajt, m_chDaneWy, &m_sCRC16);
		if ((chErr == ERR_OK) && (m_chStanProtokolu == PR_ODBIOR_NAGL) && (m_chIloscDanychRamki > -1))
		{
			bRamkaTelemetrii = (BOOL)(m_chPolecenie == PK_TELEMETRIA);
			if (bRamkaTelemetrii)
			{
				EnterCriticalSection(&m_SekcjaKrytycznaTelemetrii);				
				//zbierz dane ramki do struktury
				sTelemetria.chZnakCzasu = m_chZnakCzasu;
				sTelemetria.chAdrNadawcy = m_chAdresNadawcy;
				for (x = 0; x < LICZBA_BAJTOW_ID_TELEMETRII; x++)
					sTelemetria.chBityDanych[x] = m_chDaneWy[x];

				for (uint8_t x = 0; x < (m_chIloscDanychRamki - LICZBA_BAJTOW_ID_TELEMETRII) / 2; x++)
				{
					fTemp = Char2Float16(&m_chDaneWy[2 * x + LICZBA_BAJTOW_ID_TELEMETRII]);
					sTelemetria.dane.push_back(fTemp);
				}				
				m_vRamkaTelemetryczna.push_back(sTelemetria);		//wstaw struktur� do wektora
				sTelemetria.dane.clear();							//po wstawieniu struktury do wektora ramki, czy�� wektor struktury
				LeaveCriticalSection(&m_SekcjaKrytycznaTelemetrii);
				SetEvent(m_hZdarzenieRamkaTelemetriiGotowa);
				TRACE("SetEvent: Telemetria\n");				
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
				m_Koniec = clock();
				SetEvent(m_hZdarzenieRamkaPolecenGotowa);	
				TRACE("SetEvent: Polecenia\n");
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
	uint16_t sCRC = 0;

	*wskRamka++ = NAGLOWEK;
	*wskRamka++ = chAdrOdb;
	*wskRamka++ = chAdrNad;
	//*wskRamka++ = (uint8_t)(clock() * 100 / CLOCKS_PER_SEC);	//czas w "tickach" setnych sekund;
	*wskRamka++ = 0x12;
	*wskRamka++ = chPolecenie;
	*wskRamka++ = chRozmiar;
	for (n = 0; n < chRozmiar; n++)
		*wskRamka++ = *chDane++;
		
	for (n=1; n< chRozmiar + ROZM_CIALA_RAMKI-2; n++)	//zacznij liczy� od adresu odbiorcy (1) do CRC (-2)
		sCRC = LiczCRC16(*(chKopiaDane+n), sCRC);

	*wskRamka++ = sCRC >> 8;
	* wskRamka++ = sCRC & 0xFF;
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
	uint8_t chRamka[ROZMIAR_RAMKI_UART];
	clock_t poczatek, koniec;
//	std::vector< BinaryFrame > vPrzychodzaceRamki;
	uint32_t iCzasOczekiwania;

	//zachowaj wska�niki. B�d� potrzebne do retransmisji.
	chKopiaDaneWe = chDaneWe;
	chKopiaRozmiarWe = chRozmiarWe;

	m_chStanProtokolu = PR_ODBIOR_NAGL;	//resetuj stan protoko�u odbiorczego
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
		TRACE("Wy�lij ramk�, ret=%d\n", chLicznikRetransmisji);

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

				koniec = clock();
				iCzasOczekiwania = (koniec - poczatek) / (CLOCKS_PER_SEC / 1000);	//timeout licz w ms
				TRACE("Czas %d\n", iCzasOczekiwania);
				//iCzasOczekiwania = (m_Koniec - poczatek) / (CLOCKS_PER_SEC / 1000);	//timeout licz w ms

				//sprawd� wszystkie odebrane ramki kt�ra z nich ma taki sam TimeStamp jak nadawcza
				//iRozmiar = (uint32_t)vPrzychodzaceRamki.size();
				iRozmiar = (uint32_t)m_vRamkaPolecenia.size();
				for (x = 0; x < iRozmiar; x++)
				{
					//if (vPrzychodzaceRamki[x].time == chZnakczasu)	//por�wnuj czas
					if (m_vRamkaPolecenia[x].chZnakCzasu == chRamka[PR_ZNAK_CZASU])	//por�wnuj czas
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
	int32_t iRozmiar = 0;

	switch (chTypPortu)
	{
	case UART:
		iRozmiar = getPortSzeregowy().InBufSize();
		if (iRozmiar)
		{
			getPortSzeregowy().PurgeIncomingData();
			iRozmiar = getPortSzeregowy().InBufSize();	//test zaj�to�ci bufora odbiorczego po czyszczeniu
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
	case UART:	chErr = getPortSzeregowy().SendFrame(wskRamka, chRozmiar); break;
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
	case PR_ODBIOR_NAGL:	//testuj czy odebrano nag��wek
		if (chDaneWe == NAGLOWEK)
		{
			*chStanProtokolu = PR_ADRES_ODB;
			m_sCRC16 = LiczCRC16(chDaneWe, 0);	//inicjuj CRC
		}
		else
			chErr = ERR_ZLY_NAGLOWEK;
		break;

	case PR_ADRES_ODB:
		if ((chDaneWe == m_chAdresOdbiorcy) | (chDaneWe == ADRES_BROADCAST))
		{
			*chStanProtokolu = PR_ADRES_NAD;
			m_sCRC16 = LiczCRC16(chDaneWe, m_sCRC16);
		}
		else    //dane nie s� dla nas wi�c zacznij nas�uchiwa� kolejnej ramki
		{
			*chStanProtokolu = PR_ODBIOR_NAGL;
			chErr = ERR_ZLY_ADRESAT;
		}
		break;

	case PR_ADRES_NAD:
		*chStanProtokolu = PR_ZNAK_CZASU;
		*chAdresNadawcy = chDaneWe;
		m_sCRC16 = LiczCRC16(chDaneWe, m_sCRC16);
		break;

	case PR_ZNAK_CZASU:
		*chStanProtokolu = PR_POLECENIE;
		*chZnakCzasu = chDaneWe;
		m_sCRC16 = LiczCRC16(chDaneWe, m_sCRC16);
		break;

	case PR_POLECENIE:
		*chStanProtokolu = PR_ROZM_DANYCH;
		*chPolecenie = chDaneWe;
		m_sCRC16 = LiczCRC16(chDaneWe, m_sCRC16);
		break;

	case PR_ROZM_DANYCH:
		*chStanProtokolu = PR_DANE;
		*chRozmiarDanychWy = chDaneWe;
		*chWskOdbDanej = 0;
		m_sCRC16 = LiczCRC16(chDaneWe, m_sCRC16);
		break;

	case PR_DANE:
		*(chDaneWy + *chWskOdbDanej) = chDaneWe;
		(*chWskOdbDanej)++;
		m_sCRC16 = LiczCRC16(chDaneWe, m_sCRC16);
		if (*chWskOdbDanej == *chRozmiarDanychWy)
			*chStanProtokolu = PR_CRC16_1;
		if (*chWskOdbDanej > *chRozmiarDanychWy)
			*chStanProtokolu = PR_ODBIOR_NAGL;
		break;

	case PR_CRC16_1:
		*sCRC16 = chDaneWe * 0x100;
		(*chWskOdbDanej)++;
		*chStanProtokolu = PR_CRC16_2;
		break;

	case PR_CRC16_2:
		*sCRC16 += chDaneWe;
		*chStanProtokolu = PR_ODBIOR_NAGL;
		*chWskOdbDanej = 0;			
		if (*sCRC16 != m_sCRC16)
			chErr = ERR_CRC16;
		break;

	default: chErr = ERR_ZLY_STAN_PROT;	break;
	}
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
//dokonaj konwersji 2 bajt�w na float (po�owa precyzji)
//znaczenie bit�w obu format�w float, 32 bitowego pojedy�czej precyzji i 16 bitowego p�owy precyzji
//gdzie z=znak, c=wyk�adnik, m=mantysa
//float32: zccccccc cmmmmmmm mmmmmmmm mmmmmmmm	(1+8+23)
//float16: zcccccmm mmmmmmmm (1+5+10)
///////////////////////////////////////////////////////////////////////////////////////////////////
float CProtokol::Char2Float16(uint8_t* chDane)
{
	typedef union
	{
		unsigned char array[4];
		float fuData;
	} fUnion;
	fUnion temp;
	unsigned char chCecha, chTemp;
	unsigned short sMantysa;

	chTemp = *(chDane + 1);

	temp.array[3] = chTemp & 0x80;	//znak liczby
	chCecha = ((chTemp & 0x7C) >> 2) + (127 - 15);	//cecha 
	temp.array[3] += chCecha >> 1;
	temp.array[2] = (chCecha & 1) << 7;		//najm�odszy bit wyk�adnika

	sMantysa = 0x100 * (chTemp & 0x03) + *chDane;		//mantysa
	temp.array[2] += (unsigned char)((sMantysa & 0x03F8) >> 3);
	temp.array[1] = (unsigned char)(sMantysa & 0x0007) << 5;
	temp.array[0] = 0;

	return temp.fuData;
}