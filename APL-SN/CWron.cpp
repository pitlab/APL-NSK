#include "pch.h"
#include "CWron.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
// Konwertuje nazwê wyra¿on¹ w uint8_t przechowywan¹ w APL3 na CString
// Parametr: *chNazwa - wskaŸnik na tablicê znaków
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CWron::UstawNazwe(uint8_t* chNazwa)
{
	for (int n = 0; n < DLUGOSC_NAZWY; n++)
		m_strNazwa.Insert(n, (wchar_t)*(chNazwa + n));
}