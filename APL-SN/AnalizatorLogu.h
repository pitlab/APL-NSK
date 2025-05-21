#include <vector>

#pragma once
#define MAX_ROZMIAR_WPISU_LOGU	20


class CAnalizatorLogu
{
public:
	CAnalizatorLogu();
	~CAnalizatorLogu();

	struct stZmiennaLogu_t 
	{
		uint8_t chNazwa[MAX_ROZMIAR_WPISU_LOGU];
		uint8_t chIndeks;
		std::vector <float> vfWartosci;
	} ;

	
	uint8_t Analizuj(uint8_t * chBufor, UINT nRozmiar, std::vector<stZmiennaLogu_t> &vLogu);

private:
	BOOL m_bAnalizaNaglowka;
	uint8_t m_chIndeksZmiennejLogu;
	uint8_t m_chZmienna[MAX_ROZMIAR_WPISU_LOGU];
	uint8_t m_chIndeksNazwy = 0;
	uint8_t m_chIndeksZmiennej = 0;
};


