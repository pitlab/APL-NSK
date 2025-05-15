#include "pch.h"
#include "Komunikacja.h"
#include "Errors.h"
#include "KomunikatySieci.h"
#include "Protokol.h"
#include "APL-SNDoc.h"
/*
Klasa komunikacyjna po�rednicz�ca mi�dzy aplikacj� a protoko�em komunikacyjnym. 
Aplikacja przesy�a polecenia wymiany danych a klasa nawi�zuje po��czenie po znanym sobie interfejsie
pakuje dane w ramki i przesy�a przy u�yciu podrz�dnej klasy CProtokol

*/

BOOL CKomunikacja::m_bKoniecWatkuDekoderaPolecen = FALSE;
//std::vector <CKomunikacja::_sWron> CKomunikacja::m_vRoj;
uint8_t CKomunikacja::m_chTypPolaczenia = ETHS;
HANDLE CKomunikacja::m_hZdarzeniePaczkaDanych = NULL;




///////////////////////////////////////////////////////////////////////////////////////////////////
// Konstruktor
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
CKomunikacja::CKomunikacja()
	: m_bPolaczonoUart(FALSE)
	, m_bPolaczonoEth(FALSE)
	, m_iNumerPortuETH(0)
	, m_strAdresPortuETH("")
	, m_strNazwa("")
	, m_iNumerPortuUART(0)
	, m_iPredkoscUART(115200)
	//, m_iPredkoscUART(19200)
{
	pWskWatkuDekodujacego = AfxBeginThread((AFX_THREADPROC)WatekDekodujRamkiPolecen, (LPVOID)m_pWnd, THREAD_PRIORITY_ABOVE_NORMAL, 0, 0, NULL);
	m_hZdarzeniePaczkaDanych = CreateEvent(NULL, false, false, _T("Paczka")); // auto-reset event, non-signalled state
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Destruktor
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
CKomunikacja::~CKomunikacja()
{	
	m_bKoniecWatkuDekoderaPolecen = TRUE;
	WaitForSingleObject(pWskWatkuDekodujacego, INFINITE);
	CloseHandle(m_hZdarzeniePaczkaDanych);
}


void CKomunikacja::UstawRodzica(CView* pWnd)
{
	m_pWnd = pWnd;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Nawi�zuje po��czenia ze znanymi sobie portami komunikacyjnymi
// zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::Polacz(CView* pWnd)
{
	uint8_t chErr = ERR_NOT_CONNECTED;
	switch (m_chTypPolaczenia)
	{
	case UART:	
		chErr = getProtokol().PolaczPort(UART, m_iNumerPortuUART, m_iPredkoscUART, 0, m_pWnd);		
		if (chErr == ERR_OK)
			m_bPolaczonoUart = TRUE;
		break;

	case ETHS:	
		chErr = getProtokol().PolaczPort(ETHS, m_iNumerPortuETH, 0, m_strAdresPortuETH, m_pWnd);
		if (chErr == ERR_OK)
			m_bPolaczonoEth = TRUE;
		break;

	case USB:	break;
	
	default:	break;
	}
	return chErr;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Zamyka po�aczenie z portem komunikacyjnym
// zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::Rozlacz()
{
	uint8_t chErr = ERR_OK;
	
	if (m_bPolaczonoEth)
		chErr = getProtokol().ZamknijPort(ETHK);
	chErr = getProtokol().ZamknijPort(ETHS);
	m_bPolaczonoEth = FALSE;

	if (m_bPolaczonoUart)
		chErr = getProtokol().ZamknijPort(UART);
	m_bPolaczonoUart = FALSE;
	return chErr;
}



void CKomunikacja::AkceptujPolaczenieETH()
{
	getProtokol().AkceptujPolaczenieETH();
}

//

///////////////////////////////////////////////////////////////////////////////////////////////////
// Odbi�r wektor�w z danymi ramek i ich parsowanie
// zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
void CKomunikacja::OdebranoDaneETH()
{

	/*
	* Doda� w�tek sprawdzaj�cy czy przychodza ramki odebrane w  CProtokol przez dowolne ��cze
	* Doda� podzia� na telemetri� i ramki z odpowiedzi� - osobne w�tki?
	*/

	getProtokol().OdbierzDaneETH();
	

	//int iRozmiar = m_cProto.m_inputAnswerData.size();
	//m_cProto.m_inputAnswerData.pop_back(BinaryFrame(m_iLecznikWejRamekTelemetrii++, m_chPolecenie, m_chZnakCzasu, m_chDaneWy, m_chIloscDanychRamki));


}




void CKomunikacja::WyslanoDaneETH()
{
	getProtokol().WyslanoDaneETH();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wysy�a aplikacji swoj� nazw� 
// zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::PobierzNazweBSP(CString* strNazwa)
{
	uint8_t chErr = ERR_OK;
	_Ramka s_Ramka;
	
	

	getProtokol().PrzygotujRamke(s_Ramka.chAdrNadawcy, ADRES_STACJI, PK_POBIERZ_ID, NULL, 0, m_chRamkaWych);
	getProtokol().WyslijRamke(m_chTypPolaczenia, m_chRamkaWych, ROZM_CIALA_RAMKI);
	
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wywo�uje w�tek analizuj�cy ramki polece�
// Zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::WatekDekodujRamkiPolecen(LPVOID pParam)
{
	m_bKoniecWatkuDekoderaPolecen = FALSE;
	return reinterpret_cast<CKomunikacja*>(pParam)->WlasciwyWatekDekodujRamkiPolecen();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// W�tek analizuj�cy ramki polece�
// Zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::WlasciwyWatekDekodujRamkiPolecen ()
{
	_Ramka s_Ramka;
	int n, m;
	BOOL bMamyTo;
	_sWron sWron;
	CString strNazwa = L" ";
	std::vector <_sWron>::iterator iter;	//iterator wektora wron�w


	while (!m_bKoniecWatkuDekoderaPolecen)
	{		
		WaitForSingleObject(getProtokol().m_hZdarzenieRamkaPolecenGotowa, INFINITE);		//czekaj na odbi�r zdekodowanej ramki danych
		for (n = 0; n < getProtokol().m_vRamkaPolecenia.size(); n++)
		{
			s_Ramka = getProtokol().m_vRamkaPolecenia[n];
			switch (s_Ramka.chPolecenie)
			{
			case PK_OK:	//przysz�a ramka OK
			case PK_BLAD:	//przysz�a ramka z kodem b��du
				//m_stPotwierdzenie.chTyp = s_Ramka.chPolecenie;
				//m_stPotwierdzenie.chKodBledu = s_Ramka.dane[0];
				//m_stPotwierdzenie.chParametr = s_Ramka.dane[1];
				break;

			case PK_POBIERZ_ID: //przysz�a ramka z identyfikatorem nowego BSP
				//m_strNazwa = L"";
				for (m = 0; m < s_Ramka.chRozmiar; m++)
					strNazwa.Insert(m, s_Ramka.dane[m]);

				//sprawd� czy w roju mamy ju� tego wrona
				bMamyTo = FALSE;
				/*if (m_vRoj.size())
				{
					for (iter = m_vRoj.begin(); iter < m_vRoj.end(); iter++)
					{
						if (iter->chAdres == s_Ramka.chAdrNadawcy)
						{
							if (iter->strNazwa != strNazwa)
								iter->strNazwa = strNazwa;
							bMamyTo = TRUE;
							break;
						}
					}
				}*/
				//je�eli nie mamy to go dodaj
				/*if (bMamyTo == FALSE)
				{
					sWron.chAdres = s_Ramka.chAdrNadawcy;
					sWron.strNazwa = strNazwa;
					m_vRoj.push_back(sWron);
				}*/	
				strNazwa = L"";
				m_chAdresAutopilota = s_Ramka.chAdrNadawcy;
				CKomunikacja::WyslijOK();		//wy�lij odpowied�				
				break;

			case PK_TELEMETRIA:
				
				break;

			default:	break;
			}
		}
		getProtokol().m_vRamkaPolecenia.clear();	//wyczy�� wektor
	}
	return ERR_OK;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wysy�a ramk� OK 
// zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::WyslijOK()
{
	uint8_t chRamka[ROZM_CIALA_RAMKI];

	getProtokol().PrzygotujRamke(m_chAdresAutopilota, ADRES_STACJI, PK_OK, NULL, 0, chRamka);
	return getProtokol().WyslijRamke(m_chTypPolaczenia, chRamka, ROZM_CIALA_RAMKI);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Wysy�a polecenie zrobienia zdj�cia
// zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
//uint8_t CKomunikacja::ZrobZdjecie(uint8_t chAdres, uint16_t sSzerokosc, uint16_t sWysokosc, uint16_t* sBuforZdjecia)
uint8_t CKomunikacja::ZrobZdjecie(uint16_t* sBuforZdjecia)
{
	//uint8_t chRamka[ROZM_CIALA_RAMKI + 5];
	uint8_t chDane[5];
	uint8_t x, chErr, chOdebrano;
	uint8_t chLicznikBledow = 0;
	uint32_t nPobrano, nRozmiarDanych;
	/*m_unia8_16.dane16 = sSzerokosc;
	chDane[0] = m_unia8_16.dane8[0];
	chDane[1] = m_unia8_16.dane8[1];
	m_unia8_16.dane16 = sWysokosc;
	chDane[2] = m_unia8_16.dane8[0];
	chDane[3] = m_unia8_16.dane8[1];*/

	chErr = getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_ZROB_ZDJECIE, chDane, 0, chDane, &chOdebrano);
	if (chErr == ERR_OK)
	{
		SetEvent(m_hZdarzeniePaczkaDanych);
		//sprawd� status wykonania zdj�cia
		chDane[0] = SGZ_CZEKA;
		while (chDane[0] == SGZ_CZEKA)
		{
			chErr = getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_POB_STAT_ZDJECIA, NULL, 0, chDane, &chOdebrano);
			if (chErr == ERR_OK)
			{
				if (chDane[0] == SGZ_BLAD)
					return ERR_BLAD_KAMERY;
				chLicznikBledow = 0;
			}
			else
			{
				chLicznikBledow++;
				if (chLicznikBledow > LICZBA_PROB_ZANIM_ZGLOSI_BLAD)
					return ERR_TIMEOUT;
			}
		}

		//pobierz zdj�cie
		nPobrano = 0;
		nRozmiarDanych = (uint32_t)(480 * 320 * 2);
		while (nPobrano < nRozmiarDanych)
		{
			m_unia8_32.dane32 = nPobrano/4;	//offset pobieranych danych zdj�cia, /4 - konwersja z bajt�w s�owa 32/bit 
			for (x = 0; x < 4; x++)
				chDane[x] = m_unia8_32.dane8[x];

			//rozmiar paczki danych do pobrania
			if ((nRozmiarDanych - nPobrano) > ROZM_DANYCH_UART)
				chDane[4] = ROZM_DANYCH_UART;
			else
				chDane[4] = nRozmiarDanych - nPobrano;

			chErr = getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_POBIERZ_ZDJECIE, chDane, 5, (uint8_t*)(sBuforZdjecia + nPobrano/2), &chOdebrano);
			if (chErr == ERR_OK)
			{
				nPobrano += chOdebrano;
				SetEvent(m_hZdarzeniePaczkaDanych);
				chLicznikBledow = 0;
			}
			else
			{
				chLicznikBledow++;
				if (chLicznikBledow > LICZBA_PROB_ZANIM_ZGLOSI_BLAD)
					return ERR_TIMEOUT;;
			}
		}
	}	
	return chErr;
}
	


///////////////////////////////////////////////////////////////////////////////////////////////////
// Wysy�a polecenie odczytu konfiguracji kamery
// parametry:
// [i] chAdres - adres autopilota
// [o] *chSzerWy i *chWysWy - wska�niki na szeroko�� i wysoko�� wyj�ciowa kamery skalowana przez 16
// [o] *chSzerWe i *chWysWe - wska�niki na szeroko�� i wysoko�� z jakiej zbiera i skaluje kamera, skalowana przez 16
// [o] *chTrybDiagn - wska�nik na kod trybu diagnostycznego
// [o] *chFlagi - zbi�r bit�w definiuj�cych funkcjonalno��: bit 0: odwr�� w poziomie, bit 1: odwr�� w pionie, bit 2: 1=zdjecie, 0=film
// zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::PobierzKamere(uint8_t *chSzerWy, uint8_t *chWysWy, uint8_t *chSzerWe, uint8_t *chWysWe, uint8_t *chTrybDiagn, uint8_t *chFlagi)
{
	uint8_t chDane[6];
	uint8_t chErr, chOdebrano;

	chErr = getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_POB_PAR_KAMERY, NULL, 0, chDane, &chOdebrano);
	if (chErr == ERR_OK)
	{
		*chSzerWy = chDane[0];
		*chWysWy = chDane[1];
		*chSzerWe = chDane[2];
		*chWysWe = chDane[3];
		*chTrybDiagn = chDane[4];
		*chFlagi = chDane[5];
	}	
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wysy�a polecenie zapisu konfiguracji kamery
// parametry:
// [i] chSzerWy i chWysWy - szeroko�� i wysoko�� wyj�ciowa kamery skalowana przez 16
// [i] chSzerWe i chWysWe - szeroko�� i wysoko�� z jakiej zbiera i skaluje kamera, skalowana przez 16
// [i] chTrybDiagn - kod trybu diagnostycznego
// [i] chFlagi - zbi�r bit�w definiuj�cych funkcjonalno��: bit 0: odwr�� w poziomie, bit 1: odwr�� w pionie, bit 2: 1=zdjecie, 0=film
// zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::UstawKamere(uint8_t chSzerWy, uint8_t chWysWy, uint8_t chSzerWe, uint8_t chWysWe, uint8_t chTrybDiagn, uint8_t chFlagi)
{
	uint8_t chDane[6];
	uint8_t chOdebrano;

	chDane[0] = chSzerWy;
	chDane[1] = chWysWy;
	chDane[2] = chSzerWe;
	chDane[3] = chWysWe;
	chDane[4] = chTrybDiagn;
	chDane[5] = chFlagi;

	return getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_UST_PAR_KAMERY, chDane, 5, m_chRamkaPrzy, &chOdebrano);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wysy�a polecenie zapisu danych pod wskazany adres we flash. Optymalne jest programowanie co najmniej ca�ymi stronami po 16 s��w (32 bajty)
// parametry:
// [i] chAdresAPL - adres autopilota
// [i] nAdres - adres bezwzgl�dny pocz�tku obszaru pami�ci z zakresu 0x68000000..0x6A000000
// [i] *sDane - wska�nik na bufor z danymi do zaprogramowania
// [i] chRozmiar - ilo�� s��w do zapisu max 128
// zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::ZapiszBuforFlash(uint16_t sAdresBufora, uint8_t* chDane, uint8_t chRozmiar)
{
	uint8_t chOdebrano = 0;
	uint8_t chDaneWychodzace[255-3];
	uint8_t chDanePrzychodzace[2];

	if (chRozmiar > 252)
		return ERR_INVALID_DATA_SIZE;

	m_unia8_16.dane16 = sAdresBufora/2;		//w APL bufor jest 16-bitowy
	chDaneWychodzace[0] = m_unia8_16.dane8[0];
	chDaneWychodzace[1] = m_unia8_16.dane8[1];
	chDaneWychodzace[2] = chRozmiar/2;		//liczba s��w 16-bitowych do zapisu

	for (uint8_t n = 0; n < chRozmiar; n++)
		chDaneWychodzace[n + 3] = chDane[n];
	
	return getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_ZAPISZ_BUFOR, chDaneWychodzace, chRozmiar+3, chDanePrzychodzace, &chOdebrano);	//timeout normalny
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wysy�a polecenie skasowania sektora pamieci flash o rozmiarze 128kB
// parametry:
// [i] chAdresAPL - adres autopilota
// [i] nAdresPamieci - adres bezwzgl�dny pocz�tku obszaru pami�ci z zakresu 0x68000000..0x6A000000
// // zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::SkasujSektorFlash(uint32_t nAdresPamieci)
{
	uint8_t chOdebrano = 0;

	m_unia8_32.dane32 = nAdresPamieci;
	for (uint8_t n = 0; n < 4; n++)
		m_chRamkaWych[n] = m_unia8_32.dane8[n];
	return getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_KASUJ_FLASH, m_chRamkaWych, 4, m_chRamkaPrzy, &chOdebrano, 2000);	//du�y timeout bo Sector Erase time = 1100 ms
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wysy�a polecenie zapisu danych z bufora pod wskazany adres we flash. 
// parametry:
// [i] nAdres - adres bezwzgl�dny pocz�tku obszaru pami�ci z zakresu 0x68000000..0x6A000000
// zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::ZapiszFlash(uint32_t nAdresPamieci)
{
	uint8_t chOdebrano = 0;
	uint8_t chDaneWychodzace[4];
	uint8_t chDanePrzychodzace[2];

	m_unia8_32.dane32 = nAdresPamieci;
	for (uint8_t n = 0; n < 4; n++)
		chDaneWychodzace[n] = m_unia8_32.dane8[n];

	return getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_ZAPISZ_FLASH, chDaneWychodzace, 4, chDanePrzychodzace, &chOdebrano, 500);	//Sector Programming time = 192ms (pdf str.46)
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wysy�a polecenie zapisu danych z bufora pod wskazany adres we flash. 
// parametry:
// [i] nAdres - adres bezwzgl�dny pocz�tku obszaru pami�ci z zakresu 0x68000000..0x6A000000
// zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::CzytajFlash(uint32_t nAdresPamieci, uint16_t* sDane, uint8_t chRozmiar)
{
	uint8_t chErr, chOdebrano;
	uint8_t chDaneWychodzace[5];
	uint8_t chDanePrzychodzace[256];

	m_unia8_32.dane32 = nAdresPamieci;
	for (uint8_t n = 0; n < 4; n++)
		chDaneWychodzace[n] = m_unia8_32.dane8[n];
	chDaneWychodzace[4] = chRozmiar;

	chErr = getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_CZYTAJ_FLASH, chDaneWychodzace, 5, chDanePrzychodzace, &chOdebrano);
	if (chErr == ERR_OK)
	{
		for (uint8_t n = 0; n < chOdebrano/2; n++)
		{
			m_unia8_32.dane8[0] = chDanePrzychodzace[2 * n + 0];
			m_unia8_32.dane8[1] = chDanePrzychodzace[2 * n + 1];
			sDane[n] = m_unia8_32.dane16[0];
		}
	}
	return chErr;
}