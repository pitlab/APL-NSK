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

	//struktura zawartosci mapy numerycznej
	struct stNumerycznyModelTerenu_t
	{
		int nWierszy;
		int nKolumn;
		double dSrodekX;
		double dSrodekY;
		float fRozmiar;	
		float fNodata;	//wartoœæ oznaczajaca niewa¿ne dane
		float fWysMin;	//minimum potrzebne do znalezienia skali kolorowania
		float fWysMax;	//maksimum
		std::vector <float> vfWysokoœæ;
	};

	//struktura do dekodowania nazwy mapy
	struct stGodloNMT_t
	{
		uint8_t cPasLit;	//najstarszaczêœæ nazwy: M
		uint8_t cPasNum;	//najstarszaczêœæ nazwy: 34
		uint16_t sArkusz100k;	//arkusz w podzia³ce 1:100000
		uint8_t cArkusz50k;		//wielka litera ABCD oznaczaj¹ca æwiartkê arkusza 100k
		uint8_t cArkusz25k;		//ma³a litera abcd oznaczaj¹ca æwiartkê arkusza 50k
		uint8_t cArkusz12k;		//cyfra 1234 oznaczaj¹ca æwiartkê arkusza 25k
		uint8_t cArkusz6k;		//cyfra 1234 oznaczaj¹ca æwiartkê arkusza 12k
	};

	uint8_t m_cZmienna[ROZMIAR_BUFORA_ANALIZY];
	uint8_t m_cIndeksZmiennej;
	uint8_t m_cTypParametru;
	BOOL m_bAnalizaNaglowka;
	BOOL m_bPierwszeWa¿neDane = TRUE;
	uint16_t m_sIndeksX, m_sIndeksY;	//indeksy punktów terenu

	uint8_t Analizuj(uint8_t* chBufor, UINT nRozmiar, stNumerycznyModelTerenu_t *stNMT);
	uint8_t ZnajdŸS¹siada(stGodloNMT_t *stGod³o, int8_t cdX, int8_t cdY, std::string *strNazwa);
	uint8_t PobierzS¹siedni¹LiczbêGod³a(uint8_t cLiczba, int8_t cdX, int8_t cdY);
	uint8_t PobierzS¹siedni¹LiterêGod³a(uint8_t cLitera, int8_t cdX, int8_t cdY);
};

