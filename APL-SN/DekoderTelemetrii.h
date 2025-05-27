#pragma once

#include "KomunikatySieci.h"

class CDekoderTelemetrii
{
public:
	long PobierzZmienna(_Telemetria* wejscie, uint16_t sNumerZmiennej, float* fZmienna);
};