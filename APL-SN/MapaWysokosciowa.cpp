#include "pch.h"
#include "MapaWysokosciowa.h"
#include "Errors.h"


/*
Nazwa mapy np. M-34-101-A-a-2-1 sk³ada siê z kilku pól, których znaczenie jest nastepujace:
M-34 najwiêkszy arkusz podzia³owy o skali ? Narozniki maj¹ wspó³rzêdne 52°N, 18°E - 48°N, 24°E
101 - arkusz w skali 1:100000, sk³ada siê z 12 wierszy
A - cztery s¹siednie obszary: w górnym wierszu A i B, w dolnym C i D
a - cztery s¹siednie obszary: w górnym wierszu a i b, w dolnym c i d
2 - cztery s¹siednie obszary: w górnym wierszu 1 i 2, w dolnym 3 i 4
1 - cztery s¹siednie obszary o podzia³ce 1:10000: w górnym wierszu 1 i 2, w dolnym 3 i 4
*/


///////////////////////////////////////////////////////////////////////////////////////////////////
// Analizuje otwarty log, wyciaga z niego dane i pakuje do struktury stZmiennaLogu_t
// Parametry:
//  *chBufor - wskaŸnik na tablicê snaków logu
//  nRozmiar - rozmiar logu do analizy
//  vLogu - wektor logu, do którego wpisywane s¹ zdekodowane wartoœci
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t MapaWysokosciowa::Analizuj(uint8_t* cBufor, UINT nRozmiar, stNumerycznyModelTerenu_t *stNMT)
{
	uint8_t cB³¹d = ERR_OK;
	float fWysokoœæ;


	for (UINT n = 0; n < nRozmiar; n++)
	{
		if (m_bAnalizaNaglowka)
		{
			if (cBufor[n] == ' ')	//czy przerwa miêdzy opisem a wartosci¹ parametru
			{
				//okreœl rodzaj parametru
				if (cBufor[n - 1] == 's')	//parametry ncols i nrows
				{
					if (cBufor[n - 4] == 'c')	//parametr ncols
						m_cTypParametru = TPAR_NCOLS;
					else
					if (cBufor[n - 4] == 'r')	//parametr nrows
						m_cTypParametru = TPAR_NROWS;
				}
				else
				if (cBufor[n - 1] == 'r')	//parametry xllcenter i yllcenter
				{
					if (cBufor[n - 9] == 'x')	//parametr xllcenter
						m_cTypParametru = TPAR_XLCEN;
					else
					if (cBufor[n - 9] == 'y')	//parametr yllcenter
						m_cTypParametru = TPAR_YLCEN;
				}
				else
				if (cBufor[n - 1] == 'e')	//parametry cellsize i nodata_value
				{
					if (cBufor[n - 2] == 'z')	//parametr cellsize
						m_cTypParametru = TPAR_CSIZE;
					else
					if (cBufor[n - 2] == 'u')	//parametr nodata_value
						m_cTypParametru = TPAR_NDATA;
				}
				m_cIndeksZmiennej = 0;
			}
			else
			if (cBufor[n] == '\n')	//czy koniec wiersza oznaczajacy koniec wartoœci parametru
			{
				//dekoduj wartoœæ parametru
				switch (m_cTypParametru)
				{
				case TPAR_NCOLS:	stNMT->nKolumn = atoi((const char*)m_cZmienna);	break;
				case TPAR_NROWS:	stNMT->nWierszy = atoi((const char*)m_cZmienna);	break;
				case TPAR_XLCEN:	stNMT->dSrodekX = atof((const char*)m_cZmienna);	break;
				case TPAR_YLCEN:	stNMT->dSrodekY = atof((const char*)m_cZmienna);	break;
				case TPAR_CSIZE:	stNMT->fRozmiar = (float)atof((const char*)m_cZmienna);	break;
				case TPAR_NDATA:	stNMT->fNodata = (float)atof((const char*)m_cZmienna);		
					m_bAnalizaNaglowka = FALSE;	
					m_cIndeksZmiennej = 0;
					for (int x = 0; n < ROZMIAR_BUFORA_ANALIZY; x++)	//wyczyœæ bufor
						m_cZmienna[x] = 0;
					break;	//mamy ju¿ zdekodowany ca³y nag³ówek, przechodzimy do analizy danych
				default: return ERR_ZLE_DANE;
				}
			}
			else
			{
				//pobieraj tekst do stringu sk¹d bêdzie dekodowana wartoœæ
				if (m_cIndeksZmiennej < ROZMIAR_BUFORA_ANALIZY)
				{
					m_cZmienna[m_cIndeksZmiennej] = cBufor[n];
					m_cIndeksZmiennej++;
				}
			}
		}
		else
		{
			//analiza treœci kafelka mapy
			if (((cBufor[n] == ' ') || (cBufor[n] == '\n'))  && (m_cZmienna[0]))
			{
				fWysokoœæ = (float)atof((const char*)m_cZmienna);
				stNMT->vfWysokoœæ.push_back(fWysokoœæ);				
				if (fWysokoœæ != stNMT->fNodata)
				{				
					if (m_bPierwszeWa¿neDane)
					{
						//inicjuj ekstrema pierwszymi danymi
						stNMT->fWysMin = fWysokoœæ;
						stNMT->fWysMax = fWysokoœæ;
						m_bPierwszeWa¿neDane = FALSE;
					}
					else
					{
						//szukaj kolejnych ekstremów
						if (fWysokoœæ < stNMT->fWysMin)
							stNMT->fWysMin = fWysokoœæ;
						else
							if (fWysokoœæ > stNMT->fWysMax)
								stNMT->fWysMax = fWysokoœæ;
					}
				}
				for (int x=0; x < m_cIndeksZmiennej; x++)	//wyczyœæ bufor
					m_cZmienna[x] = 0;
				//m_cZmienna[0] = 0;
				m_cIndeksZmiennej = 0;
			}
			else
			{
				if (m_cIndeksZmiennej < ROZMIAR_BUFORA_ANALIZY)
				{
					m_cZmienna[m_cIndeksZmiennej] = cBufor[n];
					m_cIndeksZmiennej++;
				}
			}
			
		}
	}
	return cB³¹d;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Funkcja znajduje nazwê s¹siedniego kafelka mapy
// Parametry:
//  *stGodlo - wskaŸnik na strukturê zawierajac¹ elementy god³a mapy
//  cdX, cdY - przesuniêcie +-1  wzglêdem bie¿¹cego kafelka mapy
//  *strNazwa - wskaŸnik na nazwê pliku mapy do pobrania
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t MapaWysokosciowa::ZnajdŸS¹siada(stGodloNMT_t *stGod³o, int8_t cdX, int8_t cdY, std::string *strNazwa)
{
	uint8_t cB³¹d = ERR_OK;
	stGodloNMT_t stNoweGod³o;

	//Tworzenie nazwy zaczynam od koñca
	stGod³o->cArkusz6k + cdX + cdY;
	while (stGod³o->cArkusz6k > 4)
	{
		stGod³o->cArkusz6k -= 2;
	}
	stNoweGod³o.cArkusz6k;

	return cB³¹d;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Funkcja znajduje s¹siedni element liczybowy god³a odpowiadajacy skalom 6k i 12k
// Parametry:
//  cLiczba - indeks liczbowy dopowiadajacy skalom 6k i 12k god³a
//  cdX, cdY - przesuniêcie +-1  wzglêdem bie¿¹cej skali mapy
// zwraca: indeks liczbowy s¹siada
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t MapaWysokosciowa::PobierzS¹siedni¹LiczbêGod³a(uint8_t cLiczba, int8_t cdX, int8_t cdY)
{
	uint8_t cS¹siedniaLiczba;

	//przesuniecie w X, czyli w osi pó³noc-po³udnie, oœ Y w normalnym uk³¹dzie kartezjañskim
	if (cdX)
	{
		switch (cLiczba)
		{
		case 1: cS¹siedniaLiczba = 3;	break;
		case 2: cS¹siedniaLiczba = 4;	break;
		case 3: cS¹siedniaLiczba = 1;	break;
		case 4: cS¹siedniaLiczba = 2;	break;
		default:	 cS¹siedniaLiczba = 0;	break;	//zwróæ zerow¹ wartoœæ jako b³¹d, gdy podano niew³aœciwy indeks
		}
	}
	else
		cS¹siedniaLiczba = cLiczba;

	//przesuniecie w Y, czyli w osi wschód-Zachód, oœ X w normalnym uk³¹dzie kartezjañskim
	if (cdY)
	{
		switch (cS¹siedniaLiczba)
		{
		case 1: cS¹siedniaLiczba = 2;	break;
		case 2: cS¹siedniaLiczba = 1;	break;
		case 3: cS¹siedniaLiczba = 4;	break;
		case 4: cS¹siedniaLiczba = 3;	break;
		default:	 cS¹siedniaLiczba = 0;	break;	//zwróæ zerow¹ wartoœæ jako b³¹d, gdy podano niew³aœciwy indeks
		}
	}
	return cS¹siedniaLiczba;
}



// Funkcja znajduje s¹siedni element literowy god³a odpowiadajacy skalom 25k i 50k
// Parametry:
//  cLiczba - indeks literowy dopowiadajacy skalom 25k i 50k god³a
//  cdX, cdY - przesuniêcie +-1  wzglêdem bie¿¹cej skali mapy
// zwraca: indeks literowy s¹siada
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t MapaWysokosciowa::PobierzS¹siedni¹LiterêGod³a(uint8_t cLitera, int8_t cdX, int8_t cdY)
{
	uint8_t cS¹siedniaLitera;

	//przesuniecie w X, czyli w osi pó³noc-po³udnie, oœ Y w normalnym uk³¹dzie kartezjañskim
	if (cdX)
	{
		switch (cLitera)
		{
		case 1: cS¹siedniaLitera = 3;	break;
		case 2: cS¹siedniaLitera = 4;	break;
		case 3: cS¹siedniaLitera = 1;	break;
		case 4: cS¹siedniaLitera = 2;	break;
		default:	 cS¹siedniaLitera = 0;	break;	//zwróæ zerow¹ wartoœæ jako b³¹d, gdy podano niew³aœciwy indeks
		}
	}
	else
		cS¹siedniaLitera = cLitera;

	//przesuniecie w Y, czyli w osi wschód-Zachód, oœ X w normalnym uk³¹dzie kartezjañskim
	if (cdY)
	{
		switch (cS¹siedniaLitera)
		{
		case 1: cS¹siedniaLitera = 2;	break;
		case 2: cS¹siedniaLitera = 1;	break;
		case 3: cS¹siedniaLitera = 4;	break;
		case 4: cS¹siedniaLitera = 3;	break;
		default:	 cS¹siedniaLitera = 0;	break;	//zwróæ zerow¹ wartoœæ jako b³¹d, gdy podano niew³aœciwy indeks
		}
	}
	return cS¹siedniaLitera;
}

