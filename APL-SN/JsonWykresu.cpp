#include "pch.h"
#include "Errors.h"
#include "JsonWykresu.h"
#include <fstream>
#include <filesystem>
#include <atlconv.h>


uint8_t JsonWykresu::Zapisz(const std::filesystem::path& wcNazwaPliku)
{
    json jGrupa;

    for (const auto& konf : vKonfWykresow)
    {
        json jWykres;

        CW2A utf8(konf.strNazwa, CP_UTF8);
        jWykres["Nazwa"] = std::string(utf8);
        jWykres["TypWykresu"] = konf.nTypWykresu;
        jWykres["Zmienna"] = konf.nIndeksZmiennej;
        jWykres["Min"] = konf.fMin;
        jWykres["Max"] = konf.fMax;
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
    json jGrupa;
    json jWykres;
    std::ifstream file(wcNazwaPliku);

    if (!file.is_open())
        return ERR_FILE_READ;

    file >> jGrupa;
    vKonfWykresow.clear();

    for (const auto& jWykres : jGrupa["Konfiguracja Wykresow"])
    {
        stKonfWykr konf;

        std::string wcNazwa = jWykres.value("Nazwa", "");
        CA2W wide(wcNazwa.c_str(), CP_UTF8);
        konf.strNazwa = wide;
        konf.nTypWykresu = jWykres["TypWykresu"];
        konf.nIndeksZmiennej = jWykres["Zmienna"];
        konf.fMin = jWykres["Min"];
        konf.fMax = jWykres["Max"];
        konf.fKolor.r = jWykres["Kolor"]["r"];
        konf.fKolor.g = jWykres["Kolor"]["g"];
        konf.fKolor.b = jWykres["Kolor"]["b"];
        konf.fKolor.a = jWykres["Kolor"]["a"];

        vKonfWykresow.push_back(konf);
    }
    return ERR_OK;
}