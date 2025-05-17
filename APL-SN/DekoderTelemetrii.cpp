#include "pch.h"
#include "DekoderTelemetrii.h"
#include "..\..\APL-SN\APL-SN\Errors.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// Pobiera zmienn¹ o podanym numerze z telemetrii
// Parametry:
// [we] *wejscie - wskaŸnik na strukturê ramki telemetrycznej
// [we] sNumerZmiennej - indeks szukanej zmiennej liczony od zera
// [wy] *fZmienna - wskaŸnik na znalezion¹ zmienn¹
// Zwraca: kod b³êdu: ERR_OK je¿eli znaleziono zmienn¹, -1 je¿eli nie znaleziono
///////////////////////////////////////////////////////////////////////////////////////////////////
long CDekoderTelemetrii::PobierzZmienna(_Telemetria *wejscie, uint16_t sNumerZmiennej, float* fZmienna)
{
	//uint8_t chListaBitow[LICZBA_BAJTOW_ID_TELEMETRII];

	if (wejscie->dane.size())
	{
		//sprawdŸ na liscie bitów czy sygna³ jest dostêpny
		uint8_t chBajtBituZmiennej = sNumerZmiennej / 8;	//w tym bajcie bêdzie bit obecnosci zmiennej w ramce
		uint8_t chBitZmiennej = (1 << (sNumerZmiennej - sNumerZmiennej / 8)); //numer bitu obecnosci zmiennej w bajcie dolcelowym bajcie
		uint8_t chBity = (uint8_t)wejscie->chBityDanych[chBajtBituZmiennej];

		if ((chBity & chBitZmiennej) != chBitZmiennej)
		{
			*fZmienna = 0.0f;
			return -1;	//nie ma zmiennej
		}
 
		uint16_t sLicznikBitów = 0;
		uint8_t chListaBitow;

		//policz ile jest bitów, w bajtach poprzedzajacych, czyli jaka jest pozycja zmiennej
		for (int n = 0; n < chBajtBituZmiennej; n++)
		{
			chListaBitow = (uint8_t)wejscie->chBityDanych[n];
			if (chListaBitow)
			{
				for (int m = 0; m < 8; m++)
				{
					if (chListaBitow & 0x01)
						sLicznikBitów++;
					chListaBitow >>= 1;
					if (chListaBitow == 0)
						break;
				}
			}
		}
		//teraz zsumuj to z m³odszymi bitami w bajcie w któym jest szukany bit
		chListaBitow = (uint8_t)wejscie->chBityDanych[chBajtBituZmiennej];
		for (int m = 0; m < (sNumerZmiennej - sNumerZmiennej / 8); m++)
		{
			if (chListaBitow & 0x01)
				sLicznikBitów++;
			chListaBitow >>= 1;
			if (chListaBitow == 0)
				break;
		}

		*fZmienna = wejscie->dane[sLicznikBitów];
	}
	return ERR_OK;
}