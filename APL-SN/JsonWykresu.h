#pragma once
#include <string>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class JsonWykresu
{
public:
	int nTypWykresu = 0;
	int nIndeksZmiennej = 0;
	int nKolor = 0;
	//std::string outputPath = "./KonfigWykresow";

	uint8_t Zapisz(const std::filesystem::path& wcNazwaPliku);
	uint8_t Czytaj(const std::filesystem::path& wcNazwaPliku);
};

