#include "pch.h"
#include "Errors.h"
#include "JsonWykresu.h"
#include <fstream>
#include <filesystem>
#include <atlconv.h>


///////////////////////////////////////////////////////////////////////////////////////////////////
// Funkcja zapsuje do pliku JSON konfiguracjê wykresów logu lub telemetrii.
// Konfiguracja obejmuje grupy wykresów ró¿nych typów a w grupie wykresy zmiennych rysowane kolorami
// parametry:
// [i] wcNazwaPliku - œcie¿ka do pliku konfiguacji
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t JsonWykresu::Zapisz(const std::filesystem::path& wcNazwaPliku)
{
    json jKonfig;

    for (const auto& vGrupa : vKonfGrupy)
    {    
        json jGrupa;

        jGrupa["Typ wykresu"] = vGrupa.nTypWykresu;
        CW2A utf8(vGrupa.strNazwaGrupy, CP_UTF8);
        jGrupa["Nazwa grupy"] = std::string(utf8);

        for (const auto& vWykr : vGrupa.vKonfWykresow)
        {      
            json jWykres;

            CW2A utf8(vWykr.strNazwaWykresu, CP_UTF8);
            jWykres["Nazwa"] = std::string(utf8);
            jWykres["Zmienna"] = vWykr.nIndeksZmiennej;
            jWykres["Min"] = vWykr.fMin;
            jWykres["Max"] = vWykr.fMax;
            jWykres["Kolor"] =
            {
                {"r", vWykr.fKolor.r},
                {"g", vWykr.fKolor.g},
                {"b", vWykr.fKolor.b},
                {"a", vWykr.fKolor.a}
            };
            jGrupa["Konfiguracja wykresu"].push_back(jWykres);
        }
        jKonfig["Konfiguracja grupy"].push_back(jGrupa);
    }

    std::ofstream file(wcNazwaPliku);

    if (!file.is_open())
        return ERR_FILE_WRITE;

    file << jKonfig.dump(4);      // 4 = ³adne wciêcia
    return ERR_OK;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Funkcja odczytuje z pliku JSON konfiguracjê wykresów logu lub telemetrii.
// Konfiguracja obejmuje grupy wykresów ró¿nych typów a w grupie wykresy zmiennych rysowane kolorami
// parametry:
// [i] wcNazwaPliku - œcie¿ka do pliku konfiguacji
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t JsonWykresu::Czytaj(const std::filesystem::path& wcNazwaPliku)
{
    json jKonfig;
    std::ifstream file(wcNazwaPliku);

    if (!file.is_open())
        return ERR_FILE_READ;

    file >> jKonfig;
    vKonfGrupy.clear();

    for (const auto& vGrupa : jKonfig["Konfiguracja grupy"])
    {
        stKonfGrupy_t stKonfGrupy;

        stKonfGrupy.nTypWykresu = vGrupa["Typ wykresu"];
        std::string wcNazwa = vGrupa.value("Nazwa grupy", "");
        CA2W wide(wcNazwa.c_str(), CP_UTF8);
        stKonfGrupy.strNazwaGrupy = wide;
        for (const auto& jWykres : vGrupa["Konfiguracja wykresu"])
        {
            stKonfWykr_t stKonfWykr;

            std::string wcNazwa = jWykres.value("Nazwa", "");
            CA2W wide(wcNazwa.c_str(), CP_UTF8);
            stKonfWykr.strNazwaWykresu = wide;
            stKonfWykr.nIndeksZmiennej = jWykres["Zmienna"];
            stKonfWykr.fMin = jWykres["Min"];
            stKonfWykr.fMax = jWykres["Max"];
            stKonfWykr.fKolor.r = jWykres["Kolor"]["r"];
            stKonfWykr.fKolor.g = jWykres["Kolor"]["g"];
            stKonfWykr.fKolor.b = jWykres["Kolor"]["b"];
            stKonfWykr.fKolor.a = jWykres["Kolor"]["a"];
            stKonfGrupy.vKonfWykresow.push_back(stKonfWykr);
        }
        vKonfGrupy.push_back(stKonfGrupy);
    }
    return ERR_OK;
}