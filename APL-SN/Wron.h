#pragma once
#include "komunikacja\polecenia_komunikacyjne.h"

class Wron
{
public:
	uint8_t m_chAdres;
	CString m_strNazwa;
	uint8_t m_chAdresIP[4];
	uint8_t m_chPolaczony;	//0=nie po³aczony,  1=UART, 2=ETH jako serwer, 3=ETH jako klient, 4=USB
	uint16_t m_sOkresTelemetrii[LICZBA_ZMIENNYCH_TELEMETRYCZNYCH];
	void UstawNazwe(uint8_t* chNazwa);
	float PobierzCzestotliwoscTelemetrii(uint8_t chIndeksZmiennej);
	
};

