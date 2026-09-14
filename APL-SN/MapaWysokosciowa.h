#include <vector>
#include <string>
#define ROZMIAR_BUFORA_ANALIZY	10

//typy parametrów
#define TPAR_NCOLS	1
#define TPAR_NROWS	2
#define TPAR_XLCEN	3
#define TPAR_YLCEN	4
#define TPAR_CSIZE	5
#define TPAR_NDATA	6

#pragma once
class MapaWysokosciowa
{
	public:

	struct stNumerycznyModelTerenu_t
	{
		int nWierszy;
		int nKolumn;
		double dSrodekX;
		double dSrodekY;
		float fRozmiar;	
		float fNodata;	//wartoœæ oznaczajaca niewa¿ne dane
		float fWysMin;	//ekstrema potrzebne do znalezienia skali kolorowania
		float fWysMax;
		std::vector <float> vfWysokoœæ;
	};

	uint8_t m_cZmienna[ROZMIAR_BUFORA_ANALIZY];
	uint8_t m_cIndeksZmiennej;
	uint8_t m_cTypParametru;
	BOOL m_bAnalizaNaglowka;
	BOOL m_bPierwszeWa¿neDane = TRUE;
	uint16_t m_sIndeksX, m_sIndeksY;	//indeksy punktów terenu

	uint8_t MapaWysokosciowa::Analizuj(uint8_t* chBufor, UINT nRozmiar, stNumerycznyModelTerenu_t *stNMT);
};

