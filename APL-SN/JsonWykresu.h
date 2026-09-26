#pragma once
#include <string>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class JsonWykresu
{
public:
	int nTypWykresu = 0;
	int nIndeksZmiennej = 0;
	D2D1::ColorF fKolor = 0;

	typedef struct 		//konfiguracja wykresu w obrêbie grupy
	{
		int nIndeksZmiennej = 0;
		D2D1::ColorF fKolor = 0;
		CString strNazwaWykresu;
		float fMin;
		float fMax;
	} stKonfWykr_t;

	typedef struct 		//konfiguracja grupy wykresów
	{
		int nTypWykresu = 0;
		CString strNazwaGrupy;	//na razie pusta
		std::vector<stKonfWykr_t> vKonfWykresow;
	} stKonfGrupy_t;

	std::vector<stKonfGrupy_t> vKonfGrupy;
	
	uint8_t Zapisz(const std::filesystem::path& wcNazwaPliku);
	uint8_t Czytaj(const std::filesystem::path& wcNazwaPliku);
};

