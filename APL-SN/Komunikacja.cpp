#include "pch.h"
#include "Komunikacja.h"
#include "Errors.h"
#include "KomunikatySieci.h"
#include "Protokol.h"
#include "APL-SNDoc.h"
/*
Klasa komunikacyjna poœrednicz¹ca miêdzy aplikacj¹ a protoko³em komunikacyjnym. 
Aplikacja przesy³a polecenia wymiany danych a klasa nawi¹zuje po³¹czenie po znanym sobie interfejsie
pakuje dane w ramki i przesy³a przy u¿yciu podrzêdnej klasy CProtokol

*/

BOOL CKomunikacja::m_bKoniecWatkuDekoderaPolecen = FALSE;
uint8_t CKomunikacja::m_chTypPolaczenia = ETHS;
HANDLE CKomunikacja::m_hZdarzeniePaczkaDanych = NULL;
HANDLE CKomunikacja::m_hZdarzenieZmianaPolaczeniaWrona = NULL;
//HANDLE CKomunikacja::m_hZdarzeniePaczkaTelemetrii = NULL;
int	CKomunikacja::m_LicznikInstancji = 0;



///////////////////////////////////////////////////////////////////////////////////////////////////
// Konstruktor
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
CKomunikacja::CKomunikacja()
	: m_bPolaczonoUart(FALSE)
	, m_bPolaczonoEth(FALSE)
	, m_iNumerPortuETH(0)
	, m_strAdresPortuETH("")
	, m_iNumerPortuUART(0)
	, m_iPredkoscUART(115200)
	, m_chAdresAutopilota(0)
{
	pWskWatkuDekodujacego = AfxBeginThread((AFX_THREADPROC)WatekDekodujRamkiPolecen, (LPVOID)m_pWnd, THREAD_PRIORITY_ABOVE_NORMAL, 0, 0, NULL);
	m_hZdarzeniePaczkaDanych = CreateEvent(NULL, false, false, _T("PaczkaDanych")); // auto-reset event, non-signalled state	
	m_hZdarzenieZmianaPolaczeniaWrona = CreateEvent(NULL, false, false, _T("ZmianaPo³¹czenia")); // auto-reset event, non-signalled state	;
	m_LicznikInstancji++;

	//D³ugoœæ nazwy nie powinna przekraczaæ sta³ej DLUGOSC_NAZWY
	m_strNazwyZmiennychTele[TELEID_AKCEL1X] = "Akcelerometr1 X";
	m_strNazwyZmiennychTele[TELEID_AKCEL1Y] = "Akcelerometr1 Y";
	m_strNazwyZmiennychTele[TELEID_AKCEL1Z] = "Akcelerometr1 Z";
	m_strNazwyZmiennychTele[TELEID_AKCEL2X] = "Akcelerometr2 X";
	m_strNazwyZmiennychTele[TELEID_AKCEL2Y] = "Akcelerometr2 Y";
	m_strNazwyZmiennychTele[TELEID_AKCEL2Z] = "Akcelerometr2 Z";
	m_strNazwyZmiennychTele[TELEID_ZYRO1P] = "¯yroskop1 P";
	m_strNazwyZmiennychTele[TELEID_ZYRO1Q] = "¯yroskop1 Q";
	m_strNazwyZmiennychTele[TELEID_ZYRO1R] = "¯yroskop1 R";
	m_strNazwyZmiennychTele[TELEID_ZYRO2P] = "¯yroskop2 P";
	m_strNazwyZmiennychTele[TELEID_ZYRO2Q] = "¯yroskop2 Q";
	m_strNazwyZmiennychTele[TELEID_ZYRO2R] = "¯yroskop2 R";
	m_strNazwyZmiennychTele[TELEID_MAGNE1X] = "Magnetometr 1 X";
	m_strNazwyZmiennychTele[TELEID_MAGNE1Y] = "Magnetometr 1 Y";
	m_strNazwyZmiennychTele[TELEID_MAGNE1Z] = "Magnetometr 1 Z";
	m_strNazwyZmiennychTele[TELEID_MAGNE2X] = "Magnetometr 2 X";
	m_strNazwyZmiennychTele[TELEID_MAGNE2Y] = "Magnetometr 2 Y";
	m_strNazwyZmiennychTele[TELEID_MAGNE2Z] = "Magnetometr 2 Z";
	m_strNazwyZmiennychTele[TELEID_MAGNE3X] = "Magnetometr 3 X";
	m_strNazwyZmiennychTele[TELEID_MAGNE3Y] = "Magnetometr 3 Y";
	m_strNazwyZmiennychTele[TELEID_MAGNE3Z] = "Magnetometr 3 Z";
	m_strNazwyZmiennychTele[TELEID_TEMPIMU1] = "Temperatura IMU1";
	m_strNazwyZmiennychTele[TELEID_TEMPIMU2] = "Temperatura IMU2";

	//zmienna AHRS
	m_strNazwyZmiennychTele[TELEID_KAT_IMU1X] = "Trygono. Phi";
	m_strNazwyZmiennychTele[TELEID_KAT_IMU1Y] = "Trygono. Theta";
	m_strNazwyZmiennychTele[TELEID_KAT_IMU1Z] = "Trygono. Psi";
	m_strNazwyZmiennychTele[TELEID_KAT_IMU2X] = "Kwatern. Phi";
	m_strNazwyZmiennychTele[TELEID_KAT_IMU2Y] = "Kwatern. Theta";
	m_strNazwyZmiennychTele[TELEID_KAT_IMU2Z] = "Kwatern. Psi";
	m_strNazwyZmiennychTele[TELEID_KAT_ZYRO1X] = "¯yro. Phi";
	m_strNazwyZmiennychTele[TELEID_KAT_ZYRO1Y] = "¯yro. Theta";
	m_strNazwyZmiennychTele[TELEID_KAT_ZYRO1Z] = "¯yro. Psi";
	//m_strNazwyZmiennychTele[TELEID_KAT_ZYRO1X] = "Akcel. Phi";

	//zmienne barametryczne
	m_strNazwyZmiennychTele[TELEID_CISBEZW1] = "Ciœn.Bzwzgl.1";
	m_strNazwyZmiennychTele[TELEID_CISBEZW2] = "Ciœn.Bzwzgl.2";
	m_strNazwyZmiennychTele[TELEID_WYSOKOSC1] = "Wysokoœæ AGL1";
	m_strNazwyZmiennychTele[TELEID_WYSOKOSC2] = "Wysokoœæ AGL2";
	m_strNazwyZmiennychTele[TELEID_CISROZN1] = "Ciœn.Ró¿n.1";
	m_strNazwyZmiennychTele[TELEID_CISROZN2] = "Ciœn.Ró¿n.2";
	m_strNazwyZmiennychTele[TELEID_PREDIAS1] = "Prêdkoœæ IAS1";
	m_strNazwyZmiennychTele[TELEID_PREDIAS2] = "Prêdkoœæ IAS2";
	m_strNazwyZmiennychTele[TELEID_TEMPCISR1] = "Temp.Ciœn.Ró¿n1";
	m_strNazwyZmiennychTele[TELEID_TEMPCISR2] = "Temp.Ciœn.Ró¿n2";
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Destruktor
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
CKomunikacja::~CKomunikacja()
{	
	m_bKoniecWatkuDekoderaPolecen = TRUE;
	WaitForSingleObject(pWskWatkuDekodujacego, INFINITE);
	if (m_hZdarzeniePaczkaDanych)
	{
		CloseHandle(m_hZdarzeniePaczkaDanych);
		m_hZdarzeniePaczkaDanych = NULL;
	}

	if (m_hZdarzenieZmianaPolaczeniaWrona)
	{
		CloseHandle(m_hZdarzenieZmianaPolaczeniaWrona);
		m_hZdarzenieZmianaPolaczeniaWrona = NULL;
	}
}


void CKomunikacja::UstawRodzica(CView* pWnd)
{
	m_pWnd = pWnd;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Nawi¹zuje po³¹czenia ze znanymi sobie portami komunikacyjnymi
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::Polacz(CView* pWnd)
{
	uint8_t chErr = ERR_NOT_CONNECTED;
	CWron cWron;
	uint8_t chNazwa[DLUGOSC_NAZWY];

	switch (m_chTypPolaczenia)
	{
	case UART:	
		chErr = getProtokol().PolaczPort(UART, m_iNumerPortuUART, m_iPredkoscUART, 0, m_pWnd);		
		if (chErr == ERR_OK)
		{
			//wyœlij na adres rozg³oszeniowy poecenie pobrania adresów i nazwy BSP
			chErr = PobierzBSP(&cWron.m_chAdres, chNazwa, &cWron.m_chAdresIP[0]);
			if (chErr == ERR_OK)
			{				
				//sprawdŸ czy w roju jest ju¿ ten wron
				int nIndeks = 0;
				for (int n = 0; n < m_cRoj.vWron.size(); n++)
				{
					if (m_cRoj.vWron[n].m_chAdres == cWron.m_chAdres)
					{
						nIndeks = n;
						break;
					}
				}

				//je¿eli nie ma go w roju to wstaw
				if (nIndeks == 0)
				{
					m_cRoj.vWron.push_back(cWron);	
					nIndeks = (int)m_cRoj.vWron.size() - 1;
				}
				
				m_cRoj.vWron[nIndeks].UstawNazwe(chNazwa);
				m_cRoj.vWron[nIndeks].m_chPolaczony = UART;
				chErr = CzytajOkresTelemetrii(m_cRoj.vWron[nIndeks].m_chOkresTelemetrii, LICZBA_ZMIENNYCH_TELEMETRYCZNYCH);
				SetEvent(m_hZdarzenieZmianaPolaczeniaWrona);		//wygeneruj komunikat o zmianie po³¹czenia
			}
			
		}
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
// Zamyka po³aczenie z portem komunikacyjnym
// zwraca: kod b³êdu
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
// Odbiór wektorów z danymi ramek i ich parsowanie
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
void CKomunikacja::OdebranoDaneETH()
{

	/*
	* Dodaæ w¹tek sprawdzaj¹cy czy przychodza ramki odebrane w  CProtokol przez dowolne ³¹cze
	* Dodaæ podzia³ na telemetriê i ramki z odpowiedzi¹ - osobne w¹tki?
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
// Pobiera ID, nazwê oraz numer IP z urz¹dzenia przy pomocuy adresu rozg³oszeniowego, po to aby móc prowadziæ dalsz¹ komunikacjê pinkt do punktu
// *chId - wskaŸnik na zwracane ID
// *strNazwa - wskaŸnik na nazwê urz¹dzenia
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::PobierzBSP(uint8_t* chId, uint8_t* chNazwa, uint8_t* chIP)
{
	uint8_t chErr = ERR_OK;
	uint8_t chOdebrano;
	uint8_t chDanePrzych[32];

	chErr = getProtokol().WyslijOdbierzRamke(ADRES_BROADCAST, ADRES_STACJI, PK_POBIERZ_BSP, NULL, 0, chDanePrzych, &chOdebrano);
	if (!chErr)
	{
		*chId = chDanePrzych[0];
		for (int n = 0; n < DLUGOSC_NAZWY; n++)
			*(chNazwa + n) = chDanePrzych[n+1];
		for (int n = 0; n < 4; n++)
			*(chIP + n) = chDanePrzych[n + 21];
	}
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Ustawia ID, nazwê oraz numer IP dla BSP
// chId - ID, czyli adres komunikacyjny po UART
// *strNazwa - wskaŸnik na nazwê urz¹dzenia
// *chIP - wskaŸnika na adres IP
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::UstawBSP(uint8_t chId, uint8_t* chNazwa, uint8_t* chIP)
{
	uint8_t chDaneWych[25];

	chDaneWych[0] = chId;
	for (int n = 0; n < DLUGOSC_NAZWY; n++)
		chDaneWych[n + 1] = *(chNazwa + n);
	for (int n = 0; n < 4; n++)
		chDaneWych[n + 21] = *(chIP + n);

	return getProtokol().WyslijOdbierzRamke(ADRES_BROADCAST, ADRES_STACJI, PK_POBIERZ_BSP, chDaneWych, 25, NULL, NULL);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wywo³uje w¹tek analizuj¹cy ramki poleceñ
// Zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::WatekDekodujRamkiPolecen(LPVOID pParam)
{
	m_bKoniecWatkuDekoderaPolecen = FALSE;
	return reinterpret_cast<CKomunikacja*>(pParam)->WlasciwyWatekDekodujRamkiPolecen();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// W¹tek analizuj¹cy ramki poleceñ
// Zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::WlasciwyWatekDekodujRamkiPolecen ()
{
	//_Ramka s_Ramka;
	//int n, m;
	//BOOL bMamyTo;
	//_sWron sWron;
	//CString strNazwa = L" ";
	//std::vector <_sWron>::iterator iter;	//iterator wektora wronów


	while (!m_bKoniecWatkuDekoderaPolecen)
	{		
		WaitForSingleObject(getProtokol().m_hZdarzenieRamkaPolecenGotowa, INFINITE);		//czekaj na odbiór zdekodowanej ramki danych

		/*for (n = 0; n < getProtokol().m_vRamkaPolecenia.size(); n++)
		{
			s_Ramka = getProtokol().m_vRamkaPolecenia[n];
			switch (s_Ramka.chPolecenie)
			{
			case PK_OK:	//przysz³a ramka OK
			case PK_BLAD:	//przysz³a ramka z kodem b³êdu
				//m_stPotwierdzenie.chTyp = s_Ramka.chPolecenie;
				//m_stPotwierdzenie.chKodBledu = s_Ramka.dane[0];
				//m_stPotwierdzenie.chParametr = s_Ramka.dane[1];
				break;

			case PK_POBIERZ_ID: //przysz³a ramka z identyfikatorem nowego BSP
				//m_strNazwa = L"";
				for (m = 0; m < s_Ramka.chRozmiar; m++)
					strNazwa.Insert(m, s_Ramka.dane[m]);

				//sprawdŸ czy w roju mamy ju¿ tego wrona
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
				//je¿eli nie mamy to go dodaj
				/*if (bMamyTo == FALSE)
				{
					sWron.chAdres = s_Ramka.chAdrNadawcy;
					sWron.strNazwa = strNazwa;
					m_vRoj.push_back(sWron);
				}*/	
			/*	strNazwa = L"";
				m_chAdresAutopilota = s_Ramka.chAdrNadawcy;
				CKomunikacja::WyslijOK();		//wyœlij odpowiedŸ				
				break;

			case PK_TELEMETRIA:
				
				break;

			default:	break;
			}
		}
		getProtokol().m_vRamkaPolecenia.clear();	//wyczyœæ wektor*/
	}
	return ERR_OK;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wysy³a ramkê OK 
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::WyslijOK()
{
	uint8_t chRamka[ROZM_CIALA_RAMKI];

	getProtokol().PrzygotujRamke(m_chAdresAutopilota, ADRES_STACJI, PK_OK, NULL, 0, chRamka);
	return getProtokol().WyslijRamke(m_chTypPolaczenia, chRamka, ROZM_CIALA_RAMKI);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Wysy³a polecenie zrobienia zdjêcia
// zwraca: kod b³êdu
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
		//sprawdŸ status wykonania zdjêcia
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

		//pobierz zdjêcie
		nPobrano = 0;
		nRozmiarDanych = (uint32_t)(480 * 320 * 2);
		while (nPobrano < nRozmiarDanych)
		{
			m_unia8_32.dane32 = nPobrano/4;	//offset pobieranych danych zdjêcia, /4 - konwersja z bajtów s³owa 32/bit 
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
// Wysy³a polecenie odczytu konfiguracji kamery
// parametry:
// [i] chAdres - adres autopilota
// [o] *chSzerWy i *chWysWy - wskaŸniki na szerokoœæ i wysokoœæ wyjœciowa kamery skalowana przez 16
// [o] *chSzerWe i *chWysWe - wskaŸniki na szerokoœæ i wysokoœæ z jakiej zbiera i skaluje kamera, skalowana przez 16
// [o] *chTrybDiagn - wskaŸnik na kod trybu diagnostycznego
// [o] *chFlagi - zbiór bitów definiuj¹cych funkcjonalnoœæ: bit 0: odwróæ w poziomie, bit 1: odwróæ w pionie, bit 2: 1=zdjecie, 0=film
// zwraca: kod b³êdu
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
// Wysy³a polecenie zapisu konfiguracji kamery
// parametry:
// [i] chSzerWy i chWysWy - szerokoœæ i wysokoœæ wyjœciowa kamery skalowana przez 16
// [i] chSzerWe i chWysWe - szerokoœæ i wysokoœæ z jakiej zbiera i skaluje kamera, skalowana przez 16
// [i] chTrybDiagn - kod trybu diagnostycznego
// [i] chFlagi - zbiór bitów definiuj¹cych funkcjonalnoœæ: bit 0: odwróæ w poziomie, bit 1: odwróæ w pionie, bit 2: 1=zdjecie, 0=film
// zwraca: kod b³êdu
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
// Wysy³a polecenie zapisu danych pod wskazany adres we flash. Optymalne jest programowanie co najmniej ca³ymi stronami po 16 s³ów (32 bajty)
// parametry:
// [i] chAdresAPL - adres autopilota
// [i] nAdres - adres bezwzglêdny pocz¹tku obszaru pamiêci z zakresu 0x68000000..0x6A000000
// [i] *sDane - wskaŸnik na bufor z danymi do zaprogramowania
// [i] chRozmiar - iloœæ s³ów do zapisu max 128
// zwraca: kod b³êdu
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
	chDaneWychodzace[2] = chRozmiar/2;		//liczba s³ów 16-bitowych do zapisu

	for (uint8_t n = 0; n < chRozmiar; n++)
		chDaneWychodzace[n + 3] = chDane[n];
	
	return getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_ZAPISZ_BUFOR, chDaneWychodzace, chRozmiar+3, chDanePrzychodzace, &chOdebrano);	//timeout normalny
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wysy³a polecenie skasowania sektora pamieci flash o rozmiarze 128kB
// parametry:
// [i] chAdresAPL - adres autopilota
// [i] nAdresPamieci - adres bezwzglêdny pocz¹tku obszaru pamiêci z zakresu 0x68000000..0x6A000000
// // zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::SkasujSektorFlash(uint32_t nAdresPamieci)
{
	uint8_t chOdebrano = 0;

	m_unia8_32.dane32 = nAdresPamieci;
	for (uint8_t n = 0; n < 4; n++)
		m_chRamkaWych[n] = m_unia8_32.dane8[n];
	return getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_KASUJ_FLASH, m_chRamkaWych, 4, m_chRamkaPrzy, &chOdebrano, 2000);	//du¿y timeout bo Sector Erase time = 1100 ms
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wysy³a polecenie zapisu danych z bufora pod wskazany adres we flash. 
// parametry:
// [i] nAdres - adres bezwzglêdny pocz¹tku obszaru pamiêci z zakresu 0x68000000..0x6A000000
// zwraca: kod b³êdu
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
// Wysy³a polecenie zapisu danych z bufora pod wskazany adres we flash. 
// parametry:
// [i] nAdres - adres bezwzglêdny pocz¹tku obszaru pamiêci z zakresu 0x68000000..0x6A000000
// zwraca: kod b³êdu
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



///////////////////////////////////////////////////////////////////////////////////////////////////
// Odczytuje z APL listê okresów dla wszystkich zmiennych telemetrycznych
// parametry:
// [i] chOKres - wskaŸnik na tablicê z okresem
// [i] chRozmiar - rozmiar tablicy
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::CzytajOkresTelemetrii(uint8_t* chOKres, uint8_t chRozmiar)
{
	uint8_t chErr, chOdebrano;
	uint8_t chDaneWychodzace[1];
	uint8_t chDanePrzychodzace[256];

	ASSERT(chRozmiar <= LICZBA_ZMIENNYCH_TELEMETRYCZNYCH);
	chDaneWychodzace[0] = chRozmiar;

	chErr = getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_CZYTAJ_OKRES_TELE, chDaneWychodzace, 1, chDanePrzychodzace, &chOdebrano);
	if (chErr == ERR_OK)
	{
		ASSERT(chRozmiar == chOdebrano);
		for (uint8_t n = 0; n < chRozmiar; n++)
			*(chOKres + n) = chDanePrzychodzace[n];
	}
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Zapiosuje do APL listê okresów dla wszystkich zmiennych telemetrycznych
// parametry:
// [i] chOKres - wskaŸnik na tablicê z okresem
// [i] chRozmiar - rozmiar tablicy
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::ZapiszOkresTelemetrii(uint8_t *chOKres, uint8_t chRozmiar)
{
	uint8_t chErr, chOdebrano;
	uint8_t chDaneWychodzace[LICZBA_ZMIENNYCH_TELEMETRYCZNYCH];
	uint8_t chDanePrzychodzace[3];

	ASSERT(chRozmiar <= LICZBA_ZMIENNYCH_TELEMETRYCZNYCH);

	for (uint8_t n = 0; n < chRozmiar; n++)
		chDaneWychodzace[n] = *(chOKres + n);

	chErr = getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_ZAPISZ_OKRES_TELE, chDaneWychodzace, chRozmiar, chDanePrzychodzace, &chOdebrano);
	return chErr;
}


