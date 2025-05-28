#include "pch.h"
#include "Wron.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
// Konwertuje nazw� wyra�on� w uint8_t przechowywan� w APL3 na CString
// Parametr: *chNazwa - wska�nik na tablic� znak�w
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void Wron::UstawNazwe(uint8_t* chNazwa)
{
	for (int n = 0; n < DLUGOSC_NAZWY; n++)
		m_strNazwa.Insert(n, (wchar_t)*(chNazwa + n));
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Zwraca cz�stotliwo�� telemetrii na podstawie okresu z uwzglednieniem stanu wy��czenia
// Parametr: chIndeksZmiennej - wskazuje na zmienn� w tablicy
// zwraca: cz�stotliwo��
///////////////////////////////////////////////////////////////////////////////////////////////////
float Wron::PobierzCzestotliwoscTelemetrii(uint8_t chIndeksZmiennej)
{
	if ((m_sOkresTelemetrii[chIndeksZmiennej] == TEMETETRIA_WYLACZONA) || (m_sOkresTelemetrii[chIndeksZmiennej] == 0))
		return 0.0f;
	else
		return MAX_CZESTOTLIWOSC_TELEMETRII / (m_sOkresTelemetrii[chIndeksZmiennej]); 
}
