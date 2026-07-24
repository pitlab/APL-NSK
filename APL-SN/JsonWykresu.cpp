#include "pch.h"
#include "Errors.h"
#include "JsonWykresu.h"
#include <fstream>
#include <filesystem>


uint8_t JsonWykresu::Zapisz(const std::filesystem::path& wcNazwaPliku)
{
    json jGrupa;

    for (const auto& konf : vKonfWykresow)
    {
        json jWykres;

        jWykres["TypWykresu"] = konf.nTypWykresu;
        jWykres["Zmienna"] = konf.nIndeksZmiennej;
        jWykres["Kolor"] =
        {
            {"r", konf.fKolor.r},
            {"g", konf.fKolor.g},
            {"b", konf.fKolor.b},
            {"a", konf.fKolor.a}
        };
        jGrupa["Konfiguracja Wykresow"].push_back(jWykres);
    }
  

    std::ofstream file(wcNazwaPliku);

    if (!file.is_open())
        return ERR_FILE_READ;

    file << jGrupa.dump(4);      // 4 = ³adne wciêcia
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
    fKolor = j.value("Kolor", 30);
}