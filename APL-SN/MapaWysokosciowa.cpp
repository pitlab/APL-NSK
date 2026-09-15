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
uint8_t MapaWysokosciowa::Analizuj(uint8_t* cBufor, UINT nRozmiar, stNumerycznyModelTerenu_t *stNMT)
{
	uint8_t cErr = ERR_OK;
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
			if (cBufor[n] == ' ')
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
	return cErr;
}