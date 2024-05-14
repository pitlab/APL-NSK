#include "pch.h"
#include "Komunikacja.h"
#include "Errors.h"
#include "KomunikatySieci.h"

/*
Klasa komunikacyjna poœrednicz¹ca miêdzy aplikacj¹ a protoko³em komunikacyjnym. 
Aplikacja przesy³a polecenia wymiany danych a klasa nawi¹zuje po³¹czenie po znanym sobie interfejsie
pakuje dane w ramki i przesy³a przy u¿yciu podrzêdnej klasy CProtokol

*/

///////////////////////////////////////////////////////////////////////////////////////////////////
// Konstruktor
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
CKomunikacja::CKomunikacja()
: m_bPolaczono(FALSE)
, m_chTypPolaczenia(0)
, m_iNumerPortuETH(0)
, m_strAdresPortuETH("")
, m_strNazwa("")
, m_iNumerPortuUART(0)
, m_iPredkoscUART(115200)
{

}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Destruktor
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
CKomunikacja::~CKomunikacja()
{

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
	case UART:	chErr = m_cProto.PolaczPort(UART, m_iNumerPortuUART, m_iPredkoscUART, 0, pWnd);		break;
	case ETHS:	chErr = m_cProto.PolaczPort(ETHS, m_iNumerPortuETH, 0, m_strAdresPortuETH, pWnd);	break;
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


///////////////////////////////////////////////////////////////////////////////////////////////////
// Odbiór wektorów z danymi ramek i ich parsowanie
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
void CKomunikacja::OdebranoDaneETH()
{
//	std::vector< m_cProto.BinaryFrame > vInputAnswerData;
	_Ramka s_Ramka;
	uint8_t chRamkaOdpowiedzi[20];
	int n, m;

	m_cProto.OdbierzDaneETH();
	for (n = 0; n < m_cProto.m_vRamkaZOdpowiedzia.size(); n++)
	{
		s_Ramka = m_cProto.m_vRamkaZOdpowiedzia[n];
		switch (s_Ramka.chPolecenie)
		{
		case POL_NAZWA: //przysz³a nazwa BSP
			m_strNazwa = L"";
			for (m = 0; m < s_Ramka.chRozmiar; m++)
				m_strNazwa.Insert(m, s_Ramka.dane[m]);

			//wyœlij odpowiedŸ
			m_cProto.PrzygotujRamke(s_Ramka.chAdrNadawcy, ADRES_STACJI, s_Ramka.chZnakCzasu, POL_OK, NULL, 0, chRamkaOdpowiedzi);
			m_cProto.WyslijRamke(m_chTypPolaczenia, chRamkaOdpowiedzi, ROZM_CIALA_RAMKI);
			break;
		}
	}
	m_cProto.m_vRamkaZOdpowiedzia.clear();	//wyczyœæ wektor

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
uint8_t CKomunikacja::WyslijSwojaNazwe(CString strNazwa)
{
	uint8_t chErr = ERR_OK;
	return chErr;
}