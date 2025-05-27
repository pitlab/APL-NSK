#include <vector>
#include "polecenia_komunikacyjne.h"
#pragma once



class CAnalizatorLogu
{
public:
	CAnalizatorLogu();
	~CAnalizatorLogu();

	struct stZmiennaLogu_t 
	{
		uint8_t chNazwaZmiennej[DLUGOSC_NAZWY];
		uint8_t chIndeksZmiennej;
		uint8_t chAdresWrona;	//identyfikator urz¹dzenia z którego pochodzi log
		uint8_t chNazwaWrona[DLUGOSC_NAZWY];
		std::vector <float> vfWartosci;
	} ;

	
	uint8_t Analizuj(uint8_t * chBufor, UINT nRozmiar, std::vector<stZmiennaLogu_t> &vLogu);

private:
	BOOL m_bAnalizaNaglowka;
	uint8_t m_chIndeksZmiennejLogu;
	uint8_t m_chZmienna[DLUGOSC_NAZWY];
	uint8_t m_chIndeksNazwy = 0;
	uint8_t m_chIndeksZmiennej = 0;
};


