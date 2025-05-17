
#include "pch.h"
#include "Rejestr.h"
//using System;

/*CObslugaRejestru::CObslugaRejestru()


}

CObslugaRejestru::~CObslugaRejestru()
{

} */



long CObslugaRejestru::CzytajRejestrInt(const wchar_t* wchKlucz, uint32_t* nWartosc)
{
	DWORD dwWartosc;
	HKEY hKey;
	LONG lErr;
	DWORD dwResult;
	DWORD dwRegType, dwRegSize;

	lErr = RegCreateKeyExW(HKEY_CURRENT_USER, _T("SOFTWARE\\PitLab\\APL-SN\\Settings"), 0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwResult);
	if (lErr == ERROR_SUCCESS)
	{
		lErr = RegQueryValueEx(hKey, wchKlucz, 0, &dwRegType, (LPBYTE)&dwWartosc, &dwRegSize);
		if (lErr == ERROR_SUCCESS)
		{
			*nWartosc = (uint32_t)(dwWartosc & 0xFFFFFFFF);
			RegCloseKey(hKey);
		}
	}
	return lErr;
}


long CObslugaRejestru::CzytajRejestrIP(const wchar_t* wchKlucz, uint8_t* chTablicaIP)
{
	HKEY hKey;
	LONG lErr;
	DWORD dwResult;
	DWORD dwRegType, dwRegSize;
	//CString Str;
	wchar_t chNapis[50];

	//Odczytaj z rejestru adres IP
	lErr = RegCreateKeyExW(HKEY_CURRENT_USER, _T("SOFTWARE\\PitLab\\APL-SN\\Settings"), 0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwResult);
	if (lErr == ERROR_SUCCESS)
	{
		//za pierwszym razem zwraca błąd ERROR_MORE_DATA sugerujący że bufor jest za mały ale powiększanie nie pomaga
		lErr = RegQueryValueExW(hKey, L"AdresIP", NULL, &dwRegType, (LPBYTE)&chNapis, &dwRegSize);
		lErr = RegQueryValueExW(hKey, L"AdresIP", NULL, &dwRegType, (LPBYTE)&chNapis, &dwRegSize);
		if (lErr == ERROR_SUCCESS)
		{
			wchar_t* pwNapis;
			pwNapis = chNapis;
			for (uint8_t x = 0; x < 4; x++)
			{
				*(chTablicaIP + x) = _wtoi(pwNapis);
				pwNapis = wcschr(chNapis, L'.');
				for (uint8_t y = 0; y <= (pwNapis - chNapis); y++)
				{
					chNapis[y] = L' ';
				}
			}
			RegCloseKey(hKey);
		}
	}
	return lErr;
}