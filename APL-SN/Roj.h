#pragma once
#include <vector>
#include "Wron.h"
#include "polecenia_komunikacyjne.h"

class Roj :Wron
{

public:
	std::vector <Wron> vWron;
	int m_nIndeksWrona;

};

