#include "pch.h"
#include "DekoderTelemetrii.h"
#include "../Errors.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// Pobiera zmienn� o podanym numerze z telemetrii
// Parametry:
// [we] *wejscie - wska�nik na struktur� ramki telemetrycznej
// [we] sNumerZmiennej - indeks szukanej zmiennej liczony od zera
// [wy] *fZmienna - wska�nik na znalezion� zmienn�
// Zwraca: kod b��du: ERR_OK je�eli znaleziono zmienn�, -1 je�eli nie znaleziono
///////////////////////////////////////////////////////////////////////////////////////////////////
long CDekoderTelemetrii::PobierzZmienna(_Telemetria *wejscie, uint16_t sNumerZmiennej, float* fZmienna)
{
	//uint8_t chListaBitow[LICZBA_BAJTOW_ID_TELEMETRII];

	if (wejscie->dane.size())
	{
		//sprawd� na liscie bit�w czy sygna� jest dost�pny
		uint8_t chBajtBituZmiennej = sNumerZmiennej / 8;	//w tym bajcie b�dzie bit obecnosci zmiennej w ramce
		uint8_t chBitZmiennej = (1 << (sNumerZmiennej - chBajtBituZmiennej * 8)); //numer bitu obecnosci zmiennej w bajcie docelowym bajcie
		uint8_t chBity = (uint8_t)wejscie->chBityDanych[chBajtBituZmiennej];

		if ((chBity & chBitZmiennej) != chBitZmiennej)
		{
			*fZmienna = 0.0f;
			return -1;	//nie ma zmiennej
		}
 
		uint16_t sLicznikBit�w = 0;
		

		//policz ile jest bit�w, w bajtach poprzedzajacych, czyli jaka jest pozycja zmiennej
		for (int n = 0; n < chBajtBituZmiennej; n++)
		{
			chBity = (uint8_t)wejscie->chBityDanych[n];
			if (chBity)
			{
				for (int m = 0; m < 8; m++)
				{
					if (chBity & 0x01)
						sLicznikBit�w++;
					chBity >>= 1;
					if (chBity == 0)
						break;
				}
			}
		}
		//teraz zsumuj to z m�odszymi bitami w bajcie w kt�rym jest szukany bit
		chBity = (uint8_t)wejscie->chBityDanych[chBajtBituZmiennej];
		uint8_t chLiczbaBitow = sNumerZmiennej - chBajtBituZmiennej * 8;
		for (int m = 0; m < chLiczbaBitow; m++)
		{
			if (chBity & 0x01)
				sLicznikBit�w++;
			chBity >>= 1;
			if (chBity == 0)
				break;
		}
		*fZmienna = wejscie->dane[sLicznikBit�w];
	}
	return ERR_OK;
}