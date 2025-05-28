#include "pch.h"
#include "Wron.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
// Konwertuje nazwê wyra¿on¹ w uint8_t przechowywan¹ w APL3 na CString
// Parametr: *chNazwa - wskaŸnik na tablicê znaków
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void Wron::UstawNazwe(uint8_t* chNazwa)
{
	for (int n = 0; n < DLUGOSC_NAZWY; n++)
		m_strNazwa.Insert(n, (wchar_t)*(chNazwa + n));
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Zwraca czêstotliwoœæ telemetrii na podstawie okresu z uwzglednieniem stanu wy³¹czenia
// Parametr: chIndeksZmiennej - wskazuje na zmienn¹ w tablicy
// zwraca: czêstotliwoœæ
///////////////////////////////////////////////////////////////////////////////////////////////////
float Wron::PobierzCzestotliwoscTelemetrii(uint8_t chIndeksZmiennej)
{
	if ((m_sOkresTelemetrii[chIndeksZmiennej] == TEMETETRIA_WYLACZONA) || (m_sOkresTelemetrii[chIndeksZmiennej] == 0))
		return 0.0f;
	else
		return MAX_CZESTOTLIWOSC_TELEMETRII / (m_sOkresTelemetrii[chIndeksZmiennej]); 
}
