#include "pch.h"
#include "CWron.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
// Konwertuje nazw� wyra�on� w uint8_t przechowywan� w APL3 na CString
// Parametr: *chNazwa - wska�nik na tablic� znak�w
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CWron::UstawNazwe(uint8_t* chNazwa)
{
	for (int n = 0; n < DLUGOSC_NAZWY; n++)
		m_strNazwa.Insert(n, (wchar_t)*(chNazwa + n));
}