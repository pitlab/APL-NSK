#pragma once

//definicje portów
#define UART	1
#define ETHS	2	//ethernet jako serwer
#define ETHK	3	//ethernet jako klient
#define USB		4
#define LICZBA_TYPOW_PORTOW	4

//#define ROZM_DANYCH_WE_UART	128
//#define ROZM_DANYCH_WY_UART	128
//#define ROZM_DANYCH_WE_ETH	1024
//#define ROZM_DANYCH_WY_ETH	1024


#define ROZM_DANYCH_ETH		1024
#define ROZM_CIALA_RAMKI	8
#define TR_TIMEOUT			250		//timeout ramki w ms
#define TR_PROB_WYSLANIA	1

#define ROZMIAR_RAMKI_UART	254
#define ROZM_DANYCH_UART	(ROZMIAR_RAMKI_UART - ROZM_CIALA_RAMKI)
#define ROZMIAR_NAGLOWKA	6
#define ROZMIAR_CRC			2

//definicje pól ramki
#define PR_ODBIOR_NAGL		0
#define PR_ADRES_ODB		1
#define PR_ADRES_NAD		2
#define PR_ZNAK_CZASU		3
#define PR_POLECENIE		4
#define PR_ROZM_DANYCH		5
#define PR_DANE				6
#define PR_CRC16_1			7
#define PR_CRC16_2			8


#define NAGLOWEK			0xAA
#define ADRES_STACJI		0x00
#define ADRES_BROADCAST		0xFF
#define WIELOMIAN_CRC		0x1021
#define DLUGOSC_NAZWY		20		//maksymalna długość nazwy BSP, zmiennych telemetrycznych, lub innych nazw
#define ROZMIAR_ROZNE		8		//rozmiar tablicy float w strukturze wymiany danych miedzy rdzeniami

//nazwy poleceń protokołu
#define PK_OK					0	//akceptacja
#define PK_BLAD					1
#define PK_ZROB_ZDJECIE			2	//polecenie wykonania zdjęcia. We: [0..1] - sSzerokosc zdjecia, [2..3] - wysokość zdjecia
#define PK_POB_STAT_ZDJECIA		3	//pobierz status gotowości zdjęcia
#define PK_POBIERZ_ZDJECIE		4	//polecenie przesłania fragmentu zdjecia. We: [0..3] - wskaźnik na pozycje bufora, [4] - rozmiar danych do przesłania

#define PK_USTAW_BSP			5	//ustawia identyfikator/adres urządzenia
#define PK_POBIERZ_BSP			6	//pobiera identyfikator/adres urządzenia
#define PK_UST_TR_PRACY			7	//ustaw tryb pracy
#define PK_POB_PAR_KAMERY		8	//pobierz parametry pracy kamery
#define PK_UST_PAR_KAMERY		9	//ustaw parametry pracy kamery

#define PK_ZAPISZ_BUFOR			10	//zapisz dane do bufora 128kB w pamięci RAM
#define PK_ZAPISZ_FLASH			11	//zapisz stronę 32 bajtów flash
#define PK_KASUJ_FLASH			12	//kasuj sektor 128kB flash
#define PK_CZYTAJ_FLASH			13	//odczytaj zawartość Flash 
#define PK_CZYTAJ_OKRES_TELE	14	//odczytaj okresy telemetrii
#define PK_ZAPISZ_OKRES_TELE	15	//zapisz okresy telemetrii
#define PK_ZAPISZ_FRAM_FLOAT	16	//Wysyła dane typu float do zapisu we FRAM w rdzeniu CM4 o rozmiarze ROZMIAR_ROZNE
#define PK_CZYTAJ_FRAM_FLOAT	17	//wysyła polecenie odczytu zawartości FRAM
#define PK_WYSLIJ_ODCZYT_FRAM	18	//pobiera odczytane wcześniej dane

#define PK_TELEMETRIA			99	//ramka telemetryczna

#define PK_ILOSC_POLECEN		17	//liczba poleceń do sprawdzania czy polecenie mieści się w obsługiwanych granicach




//Status gotowośco wykonania zdjęcia
#define SGZ_CZEKA		0		//oczekiwania na wykonanie zdjęcia
#define SGZ_GOTOWE		1		//Zdjecie gotowe, można je pobrać
#define SGZ_BLAD		2		//wystapił błąd wykonania zdjecia

//kamera
#define SKALA_ROZDZ_KAM	16	//proporcja między obrazem zbieranym przez kamerę (HS x VS) a wysyłanym (DVPHO x DVPVO)
#define MAX_SZER_KAM	2592
#define MAX_WYS_KAM		1944 

//Flagi Ustawien Kamery - numery bitów określających funkcjonalność w UstawieniaKamery.cpp
#define FUK1_ZDJ_FILM	0x01	//1 = zdjecie, 0 = film
#define FUK1_OBR_POZ	0x02	//odwróć obraz w poziomie
#define FUK1_OBR_PION	0x04	//odwróć obraz w pionie


//Tryby Diagnostyczne Kamery
#define TDK_PRACA		0		//normalna praca
#define TDK_KRATA_CB	1		//czarnobiała karata
#define TDK_PASKI		2		//7 pionowych pasków



//definicje zmiennych telemetrycznych
//zmienne IMU
#define TELEID_AKCEL1X		0
#define TELEID_AKCEL1Y		1
#define TELEID_AKCEL1Z		2
#define TELEID_AKCEL2X		3
#define TELEID_AKCEL2Y		4
#define TELEID_AKCEL2Z		5
#define TELEID_ZYRO1P		6
#define TELEID_ZYRO1Q		7
#define TELEID_ZYRO1R		8
#define TELEID_ZYRO2P		9
#define TELEID_ZYRO2Q		10
#define TELEID_ZYRO2R		11
#define TELEID_MAGNE1X		12
#define TELEID_MAGNE1Y		13
#define TELEID_MAGNE1Z		14
#define TELEID_MAGNE2X		15
#define TELEID_MAGNE2Y		16
#define TELEID_MAGNE2Z		17
#define TELEID_MAGNE3X		18
#define TELEID_MAGNE3Y		19
#define TELEID_MAGNE3Z		20
#define TELEID_TEMPIMU1		21
#define TELEID_TEMPIMU2		22

//zmienne AHRS
#define TELEID_KAT_IMU1X	23
#define TELEID_KAT_IMU1Y	24
#define TELEID_KAT_IMU1Z	25
#define TELEID_KAT_IMU2X	26
#define TELEID_KAT_IMU2Y	27
#define TELEID_KAT_IMU2Z	28
#define TELEID_KAT_ZYRO1X	29
#define TELEID_KAT_ZYRO1Y	30
#define TELEID_KAT_ZYRO1Z	31
#define TELEID_KAT_AKCELX	32
#define TELEID_KAT_AKCELY	33
#define TELEID_KAT_AKCELZ	34

//zmienne barametryczne
#define TELEID_CISBEZW1		35
#define TELEID_CISBEZW2		36
#define TELEID_WYSOKOSC1	37
#define TELEID_WYSOKOSC2	38
#define TELEID_CISROZN1		39
#define TELEID_CISROZN2		40
#define TELEID_PREDIAS1		41
#define TELEID_PREDIAS2		42
#define TELEID_TEMPCISB1	43
#define TELEID_TEMPCISB2	44
#define TELEID_TEMPCISR1	45
#define TELEID_TEMPCISR2	46

#define LICZBA_ZMIENNYCH_TELEMETRYCZNYCH	47
#define MAX_LICZBA_ZMIENNYCH_TELEMETRYCZNYCH	112
#define LICZBA_BAJTOW_ID_TELEMETRII			(MAX_LICZBA_ZMIENNYCH_TELEMETRYCZNYCH / 8)	//liczba bajtów w ramce telemetrii identyfikujaca przesyłane zmienne


#define KWANT_CZASU_TELEMETRII	0.01f	//[s]
#define MAX_CZESTOTLIWOSC_TELEMETRII (1 / KWANT_CZASU_TELEMETRII)
#define TEMETETRIA_WYLACZONA	0xFFFF
