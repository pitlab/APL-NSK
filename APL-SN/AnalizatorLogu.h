#include <vector>
#include "komunikacja/polecenia_komunikacyjne.h"
#pragma once



class CAnalizatorLogu
{
public:
	CAnalizatorLogu();
	~CAnalizatorLogu();

	struct stZmiennaLogu_t 
	{
		uint8_t chIndeksZmiennej;
		//uint8_t chNazwaZmiennej[DLUGOSC_NAZWY];
		CString strNazwaZmiennej;
		std::vector <float> vfWartosci;
	} ;

	
	uint8_t Analizuj(uint8_t * chBufor, UINT nRozmiar, std::vector<stZmiennaLogu_t> &vLogu);

private:
	BOOL m_bAnalizaNaglowka;
	uint8_t m_chIndeksZmiennejLogu;
	uint8_t m_chZmienna[DLUGOSC_NAZWY] = {0,0};
	uint8_t m_chIndeksNazwy = 0;
	uint8_t m_chIndeksZmiennej = 0;
};


