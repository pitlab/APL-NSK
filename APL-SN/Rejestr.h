#pragma once
#include "pch.h"
#include "framework.h"

class CObslugaRejestru
{
public:
    //CObslugaRejestru();	//noexcept;
    //virtual ~CObslugaRejestru();

    long CzytajRejestrInt(const wchar_t* wchKlucz, uint32_t* nWartosc);
    long CzytajRejestrIP(const wchar_t* wchKlucz, uint8_t* chTablicaIP);
};

