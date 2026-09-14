#include "pch.h"
#include "MapaWysokosciowa.h"
#include "Errors.h"




///////////////////////////////////////////////////////////////////////////////////////////////////
// Analizuje otwarty log, wyciaga z niego dane i pakuje do struktury stZmiennaLogu_t
// Parametry:
//  *chBufor - wskaŸnik na tablicê snaków logu
//  nRozmiar - rozmiar logu do analizy
//  vLogu - wektor logu, do którego wpisywane s¹ zdekodowane wartoœci
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t MapaWysokosciowa::Analizuj(uint8_t* cBufor, UINT nRozmiar, stNumerycznyModelTerenu_t stNMT)
{
	uint8_t cErr = ERR_OK;
	float fWysokoœæ;
	uint8_t cZmienna[ROZMIAR_BUFORA_ANALIZY];
	uint8_t cIndeksZmiennej = 0;

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
				cIndeksZmiennej = 0;
			}
			else
			if (cBufor[n] == '\n')	//czy koniec wiersza oznaczajacy koniec wartoœci parametru
			{
				//dekoduj wartoœæ parametru
				switch (m_cTypParametru)
				{
				case TPAR_NCOLS:	stNMT.nKolumn = atoi((const char*)cZmienna);	break;
				case TPAR_NROWS:	stNMT.nWierszy = atoi((const char*)cZmienna);	break;
				case TPAR_XLCEN:	stNMT.dSrodekX = atof((const char*)cZmienna);	break;
				case TPAR_YLCEN:	stNMT.dSrodekY = atof((const char*)cZmienna);	break;
				case TPAR_CSIZE:	stNMT.fRozmiar = (float)atof((const char*)cZmienna);	break;
				case TPAR_NDATA:	stNMT.fNodata = (float)atof((const char*)cZmienna);		
					m_bAnalizaNaglowka = FALSE;	break;	//mamy ju¿ zdekodowany ca³y nag³ówek, przechodzimy do analizy danych
				default: return ERR_ZLE_DANE;
				}
				//stNMT.vfWysokoœæ.push_back(fWysokoœæ);
			}
			else
			{
				//pobieraj tekst do stringu sk¹d bêdzie dekodowana wartoœæ
				if (cIndeksZmiennej < ROZMIAR_BUFORA_ANALIZY)
				{
					cZmienna[cIndeksZmiennej] = cBufor[n];
					cIndeksZmiennej++;
				}
			}
		}
		else
		{
			//analiza treœci kafelka mapy
			if (cBufor[n] == ' ')
			{
				fWysokoœæ = (float)atof((const char*)cZmienna);
				stNMT.vfWysokoœæ.push_back(fWysokoœæ);
				cIndeksZmiennej = 0;
			}
			else
			{
				if (cIndeksZmiennej < ROZMIAR_BUFORA_ANALIZY)
				{
					cZmienna[cIndeksZmiennej] = cBufor[n];
					cIndeksZmiennej++;
				}
			}
			
		}
	}
	return cErr;
}