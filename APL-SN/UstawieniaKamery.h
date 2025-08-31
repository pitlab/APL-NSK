#pragma once

#define SKALA_POZIOMU_EKSPOZYCJI	96

struct st_KonfKam
{
	uint8_t chSzerWe;			//szerokoœæ patrzenia kamery / KROK_ROZDZ_KAM
	uint8_t chWysWe;
	uint8_t chSzerWy;			//szerokoœæ obrazu kamery / KROK_ROZDZ_KAM
	uint8_t chWysWy;
	uint8_t chPrzesWyPoz;		//przesuniêcie obrazu wzglêdem pocz¹tku matrycy w poziomie
	uint8_t chPrzesWyPio;		//przesuniêcie obrazu wzglêdem pocz¹tku matrycy w pionie
	uint8_t chTrybDiagn;
	uint8_t chObracanieObrazu;	//Timing Control: 0x3818
	uint8_t chFormatObrazu;		//Format Control 0x4300
	uint16_t sWzmocnienieR;		//AWB R Gain: 0x3400..01
	uint16_t sWzmocnienieG;		//AWB G Gain: 0x3402..03
	uint16_t sWzmocnienieB;		//AWB B Gain: 0x3404..05
	uint8_t chKontrolaBalansuBieli;	//AWB Manual: 0x3406
	uint16_t sCzasEkspozycji;	//AEC Long Channel Exposure [19:0]: 0x3500..02
	uint16_t sAGCLong;			//AEC PK Long Gain 0x3508..09
	uint16_t sAGCAdjust;		//AEC PK Long Gain 0x350A..0B
	uint16_t sAGCVTS;			//AEC PK VTS Output 0x350C..0D
	uint8_t chKontrolaExpo;		//AEC Manual Mode Control: 0x3503
	uint8_t chTrybyEkspozycji;	//AEC System Control 0: 0x3A00
	uint8_t chKontrolaISP0;		//0x5000
	uint8_t chKontrolaISP1;		//0x50001
	uint8_t chProgUsuwania;		//0x5080 Even CTRL 00 Treshold for even odd  cancelling
	uint8_t chNasycenie;		//SDE Control3 i 4: 0x5583 Saturation U i 5584 Saturation V
	uint8_t chPoziomEkspozycji;	//wp³ywa na grupê rejestrów: 0x3A0F..1F
};
