#include "pch.h"
#include "Errors.h"
#include "JsonWykresu.h"
#include <fstream>
#include <filesystem>


uint8_t JsonWykresu::Zapisz(const std::filesystem::path& wcNazwaPliku)
{
    json j;

    j["TypWykresu"] = nTypWykresu;
    j["Zmienna"] = nIndeksZmiennej;
    j["Kolor"] = nKolor;

    std::ofstream file(wcNazwaPliku);

    if (!file.is_open())
        return ERR_FILE_READ;

    file << j.dump(4);      // 4 = ³adne wciêcia
    return ERR_OK;
}



uint8_t JsonWykresu::Czytaj(const std::filesystem::path& wcNazwaPliku)
{
    std::ifstream file(wcNazwaPliku);

    if (!file.is_open())
        return ERR_FILE_READ;

    json j;
    file >> j;

    nTypWykresu = j.value("TypWykresu", 1);
    nIndeksZmiennej = j.value("Zmienna", 1);
    nKolor = j.value("Kolor", 30);
}