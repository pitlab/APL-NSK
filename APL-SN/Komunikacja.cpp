#include "pch.h"
#include "Komunikacja.h"
#include "Errors.h"
#include "KomunikatySieci.h"

/*
Klasa komunikacyjna po�rednicz�ca mi�dzy aplikacj� a protoko�em komunikacyjnym. 
Aplikacja przesy�a polecenia wymiany danych a klasa nawi�zuje po��czenie po znanym sobie interfejsie
pakuje dane w ramki i przesy�a przy u�yciu podrz�dnej klasy CProtokol

*/

BOOL CKomunikacja::m_bKoniecWatkuDekoderaPolecen = FALSE;
std::vector <CKomunikacja::_sWron> CKomunikacja::m_vRoj;
uint8_t CKomunikacja::m_chTypPolaczenia = ETHS;


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
{
	pWskWatkuDekodujacego = AfxBeginThread((AFX_THREADPROC)WatekDekodujRamkiPolecen, (LPVOID)m_pWnd, THREAD_PRIORITY_ABOVE_NORMAL, 0, 0, NULL);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Destruktor
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
CKomunikacja::~CKomunikacja()
{	
	m_bKoniecWatkuDekoderaPolecen = TRUE;
	WaitForSingleObject(pWskWatkuDekodujacego, INFINITE);
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
	//uint8_t chErr;
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
			case PK_POBIERZ_ID: //przysz�a ramka z identyfikatorem nowego BSP
				//m_strNazwa = L"";
				for (m = 0; m < s_Ramka.chRozmiar; m++)
					strNazwa.Insert(m, s_Ramka.dane[m]);

				//sprawd� czy w roju mamy ju� tego wrona
				bMamyTo = FALSE;
				if (m_vRoj.size())
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
				}
				//je�eli nie mamy to go dodaj
				if (bMamyTo == FALSE)
				{
					sWron.chAdres = s_Ramka.chAdrNadawcy;
					sWron.strNazwa = strNazwa;
					m_vRoj.push_back(sWron);
				}	
				strNazwa = L"";
				WyslijOK(s_Ramka.chAdrNadawcy);		//wy�lij odpowied�
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
uint8_t CKomunikacja::WyslijOK(uint8_t chAdrOdb)
{
	uint8_t chRamka[ROZM_CIALA_RAMKI];

	getProtokol().PrzygotujRamke(chAdrOdb, ADRES_STACJI, PK_OK, NULL, 0, chRamka);
	return getProtokol().WyslijRamke(m_chTypPolaczenia, chRamka, ROZM_CIALA_RAMKI);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Wysy�a polecenie zrobienia zdj�cia
// zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::ZrobZdjecie(uint8_t chAdres, uint16_t sSzerokosc, uint16_t sWysokosc, uint16_t* sBuforZdjecia)
{
	//uint8_t chRamka[ROZM_CIALA_RAMKI + 5];
	uint8_t chDane[5];
	uint8_t x, chErr, chOdebrano;
	uint32_t nPobrano, nRozmiarDanych;
	m_unia8_16.dane16 = sSzerokosc;
	chDane[0] = m_unia8_16.dane8[0];
	chDane[1] = m_unia8_16.dane8[1];
	m_unia8_16.dane16 = sWysokosc;
	chDane[2] = m_unia8_16.dane8[0];
	chDane[3] = m_unia8_16.dane8[1];

	chErr = getProtokol().WyslijOdbierzRamke(chAdres, ADRES_STACJI, PK_ZROB_ZDJECIE, chDane, 4, chDane, &chOdebrano);
	if (chErr == ERR_OK)
	{
		//sprawd� status wykonania zdj�cia
		chDane[0] = SGZ_CZEKA;
		while (chDane[0] == SGZ_CZEKA)
		{
			chErr = getProtokol().WyslijOdbierzRamke(chAdres, ADRES_STACJI, PK_POB_STAT_ZDJECIA, NULL, 0, chDane, &chOdebrano);
			if (chErr == ERR_OK)
			{
				if (chDane[0] == SGZ_BLAD)
					return SGZ_BLAD;
			}
		}

		//pobierz zdj�cie
		nPobrano = 0;
		nRozmiarDanych = (uint32_t)(sSzerokosc * sWysokosc);
		while (nPobrano < nRozmiarDanych)
		{
			m_unia8_32.dane32 = nPobrano;	//offset pobieranych danych zdj�cia
			for (x = 0; x < 4; x++)
				chDane[x] = m_unia8_32.dane8[x];

			//rozmiar paczki
			if ((nRozmiarDanych - nPobrano) > ROZM_DANYCH_UART)
				chDane[4] = ROZM_DANYCH_UART;
			else
				chDane[4] = nRozmiarDanych - nPobrano;

			chErr = getProtokol().WyslijOdbierzRamke(chAdres, ADRES_STACJI, PK_POBIERZ_ZDJECIE, chDane, 5, (uint8_t*)(sBuforZdjecia + nPobrano), &chOdebrano);
			if (chErr == ERR_OK)
			{
				nPobrano += chOdebrano;
			}
		}
	}	
	return chErr;
}
		 