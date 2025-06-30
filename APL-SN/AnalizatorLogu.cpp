#include "pch.h"
#include "AnalizatorLogu.h"
#include "Errors.h"


CAnalizatorLogu::CAnalizatorLogu()

{
	m_bAnalizaNaglowka = TRUE;
	m_chIndeksZmiennejLogu = 0;
}

CAnalizatorLogu::~CAnalizatorLogu()
{
	
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Analizuje otwarty log, wyciaga z niego dane i pakuje do struktury stZmiennaLogu_t
// Parametry:
//  *chBufor - wskaŸnik na tablicê snaków logu
//  nRozmiar - rozmiar logu do analizy
//  vLogu - wektor logu, do którego wpisywane s¹ zdekodowane wartoœci
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CAnalizatorLogu::Analizuj(uint8_t* chBufor, UINT nRozmiar, std::vector<stZmiennaLogu_t> &vLogu)
{
	uint8_t chErr = ERR_OK;
	float fWartosc;
	stZmiennaLogu_t stZmiennaLogu;
	m_chIndeksNazwy = 0;

	for (UINT n = 0; n < nRozmiar; n++)
	{
		if (m_bAnalizaNaglowka)
		{
			//szukaj kolejnych zmiennych zakoñczonych œrednikiem lub CR
			if ((chBufor[n] == ';') || (chBufor[n] == 0x0D))
			{
				vLogu.push_back(stZmiennaLogu);
				stZmiennaLogu.strNazwaZmiennej = "";		//wyczyœæ nazwê zmiennej
				m_chIndeksNazwy = 0;
				m_chIndeksZmiennejLogu++;
			}
			else
			{
				stZmiennaLogu.chIndeksZmiennej = m_chIndeksZmiennejLogu;
				stZmiennaLogu.strNazwaZmiennej.Insert(m_chIndeksNazwy, chBufor[n]);				
				m_chIndeksNazwy++;
			}

			if (chBufor[n] == 0x0A)	//LF - koniec wiersza nag³ówka
			{
				m_bAnalizaNaglowka = FALSE;
				m_chIndeksZmiennejLogu = 0;
			}
		}
		else
		{
			//analiza treœci logu
			if (chBufor[n] == ';')
			{
				m_chZmienna[m_chIndeksZmiennej++] = 0;	//zero terminuj¹ce liczbê
				if (m_chIndeksZmiennejLogu == 0)	//czas hh:mm:ss.ss interpretuj jako liczbê sekund od pocz¹tku doby
					fWartosc = 3600 * atoi((const char*)m_chZmienna) + 60* atoi((const char*)(m_chZmienna + 3)) + (float)atof((const char*)(m_chZmienna + 6));
				else
					fWartosc = (float)atof((const char*)m_chZmienna);
				vLogu[m_chIndeksZmiennejLogu].vfWartosci.push_back(fWartosc);
				m_chIndeksZmiennejLogu++;
				m_chIndeksZmiennej = 0;
			}
			else
			{
				m_chZmienna[m_chIndeksZmiennej++] = chBufor[n];
			}
			if (chBufor[n] == 0x0A)	//LF - koniec wiersza nag³ówka
				m_chIndeksZmiennejLogu = 0;
		}
	}
	return chErr;
}

