#include "pch.h"
#include "Komunikacja.h"
#include "Errors.h"
#include "KomunikatySieci.h"

/*
Klasa komunikacyjna poœrednicz¹ca miêdzy aplikacj¹ a protoko³em komunikacyjnym. 
Aplikacja przesy³a polecenia wymiany danych a klasa nawi¹zuje po³¹czenie po znanym sobie interfejsie
pakuje dane w ramki i przesy³a przy u¿yciu podrzêdnej klasy CProtokol

*/

BOOL CKomunikacja::m_bKoniecWatkuDekoderaPolecen = FALSE;
std::vector <CKomunikacja::_sWron> CKomunikacja::m_vRoj;
uint8_t CKomunikacja::m_chTypPolaczenia = ETHS;


///////////////////////////////////////////////////////////////////////////////////////////////////
// Konstruktor
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
CKomunikacja::CKomunikacja()
	: m_bPolaczono(FALSE)
	//, m_chTypPolaczenia(0)
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
// Nawi¹zuje po³¹czenia ze znanymi sobie portami komunikacyjnymi
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::Polacz(CView* pWnd)
{
	uint8_t chErr = ERR_NOT_CONNECTED;
	switch (m_chTypPolaczenia)
	{
	case UART:	chErr = m_cProto.PolaczPort(UART, m_iNumerPortuUART, m_iPredkoscUART, 0, m_pWnd);		break;
	case ETHS:	chErr = m_cProto.PolaczPort(ETHS, m_iNumerPortuETH, 0, m_strAdresPortuETH, m_pWnd);	break;
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
	chErr = m_cProto.ZamknijPort();			
	return chErr;
}



void CKomunikacja::AkceptujPolaczenieETH()
{
	m_cProto.AkceptujPolaczenieETH();
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

	m_cProto.OdbierzDaneETH();
	

	//int iRozmiar = m_cProto.m_inputAnswerData.size();
	//m_cProto.m_inputAnswerData.pop_back(BinaryFrame(m_iLecznikWejRamekTelemetrii++, m_chPolecenie, m_chZnakCzasu, m_chDaneWy, m_chIloscDanychRamki));


}




void CKomunikacja::WyslanoDaneETH()
{
	m_cProto.WyslanoDaneETH();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wysy³a aplikacji swoj¹ nazwê 
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::PobierzNazweBSP(CString* strNazwa)
{
	uint8_t chErr = ERR_OK;
	_Ramka s_Ramka;
	
	

	m_cProto.PrzygotujRamke(s_Ramka.chAdrNadawcy, ADRES_STACJI, POL_MOJE_ID, NULL, 0, m_chRamkaWych);
	m_cProto.WyslijRamke(m_chTypPolaczenia, m_chRamkaWych, ROZM_CIALA_RAMKI);
	
	return chErr;
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
	uint8_t chErr;
	_Ramka s_Ramka;
	int n, m;
	BOOL bMamyTo;
	_sWron sWron;
	CString strNazwa = L" ";
	std::vector <_sWron>::iterator iter;	//iterator wektora wronów

	while (!m_bKoniecWatkuDekoderaPolecen)
	{		
		WaitForSingleObject(m_cProto.m_hZdarzenieRamkaPolecenGotowa, INFINITE);		//czekaj na odbiór zdekodowanej ramki danych
		m_cProto.m_vRamkaPolecenia.size();
		for (n = 0; n < m_cProto.m_vRamkaPolecenia.size(); n++)
		{
			s_Ramka = m_cProto.m_vRamkaPolecenia[n];
			switch (s_Ramka.chPolecenie)
			{
			case POL_MOJE_ID: //przysz³a ramka z identyfikatorem nowego BSP
				//m_strNazwa = L"";
				for (m = 0; m < s_Ramka.chRozmiar; m++)
					strNazwa.Insert(m, s_Ramka.dane[m]);

				//sprawdŸ czy w roju mamy ju¿ tego wrona
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
				//je¿eli nie mamy to go dodaj
				if (bMamyTo == FALSE)
				{
					sWron.chAdres = s_Ramka.chAdrNadawcy;
					sWron.strNazwa = strNazwa;
					m_vRoj.push_back(sWron);
				}	
				strNazwa = L"";
				WyslijOK(s_Ramka.chAdrNadawcy);		//wyœlij odpowiedŸ
				break;

			default:	break;
			}
		}
		m_cProto.m_vRamkaPolecenia.clear();	//wyczyœæ wektor
	}
	return ERR_OK;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wysy³a aplikacji swoj¹ nazwê 
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::WyslijOK(uint8_t chAdrOdb)
{
	uint8_t chErr = ERR_OK;
	//_Ramka s_Ramka;
	uint8_t chRamka[ROZM_CIALA_RAMKI];

	m_cProto.PrzygotujRamke(chAdrOdb, ADRES_STACJI, POL_OK, NULL, 0, chRamka);
	chErr = m_cProto.WyslijRamke(m_chTypPolaczenia, chRamka, ROZM_CIALA_RAMKI);
	return chErr;
}