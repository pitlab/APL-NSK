#include "pch.h"
#include "Errors.h"
#include "JsonWykresu.h"
#include <fstream>


uint8_t JsonWykresu::Zapisz(const std::string& cNazwaPliku)
{
    json j;

    j["TypWykresu"] = nTypWykresu;
    j["Zmienna"] = nIndeksZmiennej;
    j["Kolor"] = nKolor;

    std::ofstream file(cNazwaPliku);

    if (!file.is_open())
        return ERR_FILE_READ;

    file << j.dump(4);      // 4 = ³adne wciêcia
    return ERR_OK;
}


uint8_t JsonWykresu::Czytaj(const std::string& cNazwaPliku)
{
    std::ifstream file(cNazwaPliku);

    if (!file.is_open())
        return ERR_FILE_READ;

    json j;
    file >> j;

    nTypWykresu = j.value("TypWykresu", 1);
    nIndeksZmiennej = j.value("Zmienna", 1);
    nKolor = j.value("Kolor", 30);
}