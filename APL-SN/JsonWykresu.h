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

	struct stKonfWykr
	{
		int nTypWykresu = 0;
		int nIndeksZmiennej = 0;
		D2D1::ColorF fKolor = 0;
	};

	std::vector<stKonfWykr> vKonfWykresow;
	uint8_t Zapisz(const std::filesystem::path& wcNazwaPliku);
	uint8_t Czytaj(const std::filesystem::path& wcNazwaPliku);
};

