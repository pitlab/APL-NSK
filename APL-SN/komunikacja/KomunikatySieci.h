#pragma once

#define ON_ACCEPT	1
#define ON_CONNECT	2
#define ON_RECEIVE	3
#define ON_SEND		4
#define ON_CLOSE	5
#include <queue>
#include "polecenia_komunikacyjne.h"

struct BinaryFrame
{
	BinaryFrame(int _frameNo, unsigned char _command, unsigned char _time, const void* _pData, size_t _dataLen) :
		frameNo(_frameNo), command(_command), time(_time), data(_dataLen)
	{
		memcpy(&data[0], _pData, _dataLen);
	}

	int					frameNo;	///< Kolejny numer ramki
	unsigned char		command;	///< Komenda
	unsigned char		time;		///< Znacznik czasu ramki
	std::vector< unsigned char > data;		///< Binarne dane ramki
};

struct _Ramka
{
	uint8_t chPolecenie = 0;
	uint8_t chZnakCzasu = 0;
	uint8_t chRozmiar = 0;
	uint8_t chAdrNadawcy = 0;
	std::vector <uint8_t> dane;
};


//struct _TelemetriaUporzadkowana
struct _Telemetria
{
	uint8_t chZnakCzasu = 0;
	float dane[MAX_LICZBA_ZMIENNYCH_TELEMETRYCZNYCH] = {0,0,0,0,0};
};
