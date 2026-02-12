#pragma once

//definicje portów
#define UART	1
#define ETHS	2	//ethernet jako serwer
#define ETHK	4	//ethernet jako klient
//#define USB		8
#define LICZBA_TYPOW_PORTOW	3

#define LICZBA_SPRAWDZANYCH_RAMEK	5	//tyle ramek wstecz jest porównywanych po polu znacznika czasu z ramką wychodzącą

//#define ROZM_DANYCH_WE_UART	128
//#define ROZM_DANYCH_WY_UART	128
//#define ROZM_DANYCH_WE_ETH	1024
//#define ROZM_DANYCH_WY_ETH	1024


#define ROZM_DANYCH_ETH		1024
#define ROZM_CIALA_RAMKI	8
#define TR_TIMEOUT			200		//timeout ramki w ms
#define TR_PROB_WYSLANIA	5

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
//efine ROZMIAR_ROZNE		8		//rozmiar tablicy float w strukturze wymiany danych miedzy rdzeniami
#define ROZMIAR_ROZNE_FLOAT				8
#define ROZMIAR_ROZNE_SHORT				16
#define ROZMIAR_ROZNE_CHAR				32

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
#define PK_ZAPISZ_FRAM_U8		16	//Wysyła dane typu uint8_t do zapisu we FRAM w rdzeniu CM4 o podanym rozmiarze liczb uint8_t
#define PK_ZAPISZ_FRAM_FLOAT	17	//Wysyła dane typu float do zapisu we FRAM w rdzeniu CM4 o podanym rozmiarze liczb float
#define PK_WYSLIJ_POTW_ZAPISU	18	//wysyła potwierdzenie zapisania danych ERR_OK lub ERR_PROCES_TRWA
#define PK_CZYTAJ_FRAM_U8		19	//Wysyła  polecenie odczytu zawartości FRAM typu uint8_t
#define PK_CZYTAJ_FRAM_FLOAT	20	//wysyła polecenie odczytu zawartości FRAM typu float
#define PK_WYSLIJ_ODCZYT_FRAM	21	//pobiera odczytane wcześniej dane lub zwraca ERR_PROCES_TRWA
#define PK_REKONFIG_SERWA_RC	22	//wykonuje ponowną konfigurację wejść i wyjść RC po zmianie zawartosci FRAM
#define PK_UST_PAR_KAMERY_GRUP	23	//ustaw parametry pracy kamery grupowo
#define PK_RESETUJ_KAMERE		24	//sprzetowo resetuje kamerę i ładuje domyślne ustawienia
#define PK_ZAMKNIJ_POLACZENIE	25	//NSK kończy pracę i zatrzymuje transmisję danych
#define PK_ZAPISZ_KONFIG_PID	26	//zapiszuje zmodyfikowaną konfigurację regulatorów PID
#define PK_ZBIERAJ_EKSTREMA_RC	27	//rozpoczyna zbieranie ekstremalnych wartości kanałów obu odbiorników RC
#define PK_ZAPISZ_EKSTREMA_RC	28	//kończy zbieranie ekstremalnych wartości kanałów obu odbiorników RC i zapisuje wyniki do FRAM
#define PK_ZAPISZ_WYSTER_NAPEDU	29	//zapsuje nastawy wysterowania napędu dla wartości jałowej, minimalnej, zawisu i maksymalnej
#define PK_ZAPISZ_TRYB_REG		30	//zapisuje we FRAM i w zmiannych roboczych rodzaj podstawowych 4 regulacji
#define PK_RESETUJ_CM4			31	//wykinuje reset CM4 po zmianie krytycznej konfiguracji
#define PK_WSTRZYMAJ_TELEMETRIE	32	//wstrzymuje lub wznawia wysyłnie telemetrii na czas transmisji innych danych


#define PK_ILOSC_POLECEN		33	//liczba poleceń do sprawdzania czy polecenie mieści się w obsługiwanych granicach

#define PK_TELEMETRIA1			96	//ramka telemetryczna 1
#define PK_TELEMETRIA2			97	//ramka telemetryczna 2
#define PK_TELEMETRIA3			98	//ramka telemetryczna 3 - na razie nie używane
#define PK_TELEMETRIA4			99	//ramka telemetryczna 4 - na razie nie używane


//polecenia ramki PK_WSTRZYMAJ_TELEMETRIE służące do zarządzania strumieniem telemetrii
#define WSTRZYMAJ_TELEMETRIE	1
#define WZNOW_TELEMETRIE		0


//Status gotowośco wykonania zdjęcia
#define SGZ_CZEKA		0		//oczekiwania na wykonanie zdjęcia
#define SGZ_GOTOWE		1		//Zdjecie gotowe, można je pobrać
#define SGZ_BLAD		2		//wystapił błąd wykonania zdjecia

//kamera
#define KROK_ROZDZ_KAM	16		//najmnijeszy krok zmiany rozmiaru obrazu o tyle pikseli. Umożliwia wysłanie rozmiaru jako liczby 8-bitowej
#define MAX_SZER_KAM	2592	//mominalna szerokość przetwornika kamery
#define MAX_WYS_KAM		1944	//nominalna wysokość przetwornika kamery

//Flagi Ustawien Kamery - numery bitów określających funkcjonalność w UstawieniaKamery.cpp
#define FUK1_ZDJ_FILM	0x01	//1 = zdjecie, 0 = film
#define FUK1_OBR_POZ	0x02	//odwróć obraz w poziomie
#define FUK1_OBR_PION	0x04	//odwróć obraz w pionie

//definicja wstępych wartości danych telemetrycznych w celu znamezienia zakresu jego zmienności
#define WARTOSC_MAX		1000.0f
#define WARTOSC_MIN		-1000.0f

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

//odbiorniki RC 
#define TELEID_RC_KAN1		47
#define TELEID_RC_KAN2		48
#define TELEID_RC_KAN3		49
#define TELEID_RC_KAN4		50
#define TELEID_RC_KAN5		51
#define TELEID_RC_KAN6		52
#define TELEID_RC_KAN7		53
#define TELEID_RC_KAN8		54
#define TELEID_RC_KAN9		55
#define TELEID_RC_KAN10		56
#define TELEID_RC_KAN11		57
#define TELEID_RC_KAN12		58
#define TELEID_RC_KAN13		59
#define TELEID_RC_KAN14		60
#define TELEID_RC_KAN15		61
#define TELEID_RC_KAN16		62

//serwa
#define TELEID_SERWO1		63
#define TELEID_SERWO2		64
#define TELEID_SERWO3		65
#define TELEID_SERWO4		66
#define TELEID_SERWO5		67
#define TELEID_SERWO6		68
#define TELEID_SERWO7		69
#define TELEID_SERWO8		70
#define TELEID_SERWO9		71
#define TELEID_SERWO10		72
#define TELEID_SERWO11		73
#define TELEID_SERWO12		74
#define TELEID_SERWO13		75
#define TELEID_SERWO14		76
#define TELEID_SERWO15		77
#define TELEID_SERWO16		78

#define TELEID_DOTYK_ADC0	79
#define TELEID_DOTYK_ADC1	80
#define TELEID_DOTYK_ADC2	81
#define TELEID_CZAS_PETLI	82	//czas trwania ostatniej petli głównej w us
//max do 127


//zmiene telemetryczne w ramce 2
#define TELEID_PID_PRZE_WZAD	128	//wartość zadana regulatora sterowania przechyleniem
#define TELEID_PID_PRZE_WYJ		129	//wyjście regulatora sterowania przechyleniem
#define TELEID_PID_PRZE_WY_P	130	//wyjście członu P
#define TELEID_PID_PRZE_WY_I	131	//wyjście członu I
#define TELEID_PID_PRZE_WY_D	132	//wyjście członu D

#define TELEID_PID_PK_PRZE_WYJ	133	//wyjście regulatora sterowania prędkością kątową przechylenia
#define TELEID_PID_PK_PRZE_WY_P	134	//wyjście członu P
#define TELEID_PID_PK_PRZE_WY_I	135	//wyjście członu I
#define TELEID_PID_PK_PRZE_WY_D	136	//wyjście członu D

#define TELEID_PID_POCH_WZAD	137	//wartość zadana regulatora sterowania pochyleniem
#define TELEID_PID_POCH_WYJ		138	//wyjście regulatora sterowania pochyleniem
#define TELEID_PID_POCH_WY_P	139	//wyjście członu P
#define TELEID_PID_POCH_WY_I	140	//wyjście członu I
#define TELEID_PID_POCH_WY_D	141	//wyjście członu D

#define TELEID_PID_PK_POCH_WYJ	142	//wyjście regulatora sterowania prędkością kątową pochylenia
#define TELEID_PID_PK_POCH_WY_P	143	//wyjście członu P
#define TELEID_PID_PK_POCH_WY_I	144	//wyjście członu I
#define TELEID_PID_PK_POCH_WY_D	145	//wyjście członu D

#define TELEID_PID_ODCH_WZAD	146	//wartość zadana regulatora sterowania odchyleniem
#define TELEID_PID_ODCH_WYJ		147	//wyjście regulatora sterowania odchyleniem
#define TELEID_PID_ODCH_WY_P	148	//wyjście członu P
#define TELEID_PID_ODCH_WY_I	149	//wyjście członu I
#define TELEID_PID_ODCH_WY_D	150	//wyjście członu D

#define TELEID_PID_PK_ODCH_WYJ	151	//wyjście regulatora sterowania prędkością kątową odchylenia
#define TELEID_PID_PK_ODCH_WY_P	152	//wyjście członu P
#define TELEID_PID_PK_ODCH_WY_I	153	//wyjście członu I
#define TELEID_PID_PK_ODCH_WY_D	154	//wyjście członu D

#define TELEID_PID_WYSO_WZAD	155	//wartość zadana regulatora sterowania wysokością
#define TELEID_PID_WYSO_WYJ		156	//wyjście regulatora sterowania odchyleniem
#define TELEID_PID_WYSO_WY_P	157	//wyjście członu P
#define TELEID_PID_WYSO_WY_I	158	//wyjście członu I
#define TELEID_PID_WYSO_WY_D	159	//wyjście członu D

#define TELEID_PID_PR_WYSO_WYJ	160	//wyjście regulatora sterowania prędkością zmiany wysokości
#define TELEID_PID_PR_WYSO_WY_P	161	//wyjście członu P
#define TELEID_PID_PR_WYSO_WY_I	162	//wyjście członu I
#define TELEID_PID_PR_WYSO_WY_D	163	//wyjście członu D

#define TELEID_PID_NAWN_WZAD	164	//wartość zadana regulatora sterowania nawigacją w kierunku północnym
#define TELEID_PID_NAWN_WYJ		165	//wyjście regulatora sterowania nawigacją w kierunku północnym
#define TELEID_PID_NAWN_WY_P	166	//wyjście członu P
#define TELEID_PID_NAWN_WY_I	167	//wyjście członu I
#define TELEID_PID_NAWN_WY_D	168	//wyjście członu D

#define TELEID_PID_PR_NAWN_WYJ	169	//wyjście regulatora sterowania prędkością w kierunku północnym
#define TELEID_PID_PR_NAWN_WY_P	170	//wyjście członu P
#define TELEID_PID_PR_NAWN_WY_I	171	//wyjście członu I
#define TELEID_PID_PR_NAWN_WY_D	172	//wyjście członu D

#define TELEID_PID_NAWE_WZAD	173	//wartość zadana regulatora sterowania nawigacją w kierunku wschodnim
#define TELEID_PID_NAWE_WYJ		174	//wyjście regulatora sterowania nawigacją w kierunku północnym
#define TELEID_PID_NAWE_WY_P	175	//wyjście członu P
#define TELEID_PID_NAWE_WY_I	176	//wyjście członu I
#define TELEID_PID_NAWE_WY_D	177	//wyjście członu D

#define TELEID_PID_PR_NAWE_WYJ	178	//wyjście regulatora sterowania prędkością w kierunku wschodnim
#define TELEID_PID_PR_NAWE_WY_P	179	//wyjście członu P
#define TELEID_PID_PR_NAWE_WY_I	180	//wyjście członu I
#define TELEID_PID_PR_NAWE_WY_D	181	//wyjście członu D
//max do 255

#define LICZBA_ZMIENNYCH_TELEMETRYCZNYCH	182


//#define MAX_LICZBA_ZMIENNYCH_TELEMETRYCZNYCH2	112
//#define LICZBA_BAJTOW_ID_TELEMETRII			(MAX_LICZBA_ZMIENNYCH_TELEMETRYCZNYCH / 8)	//liczba bajtów w ramce telemetrii identyfikujaca przesyłane zmienne
//#define MAX_ZMIENNYCH_TELEMETR_W_RAMCE	115		//tyle zmiennych może być przesłanych w jednej ramce telemetrycznej (ramek może być kilka)
#define MAX_INDEKSOW_TELEMETR_W_RAMCE	128		//zmienne w ramce można wybrać z takiej puli indeksów
#define LICZBA_BAJTOW_ID_TELEMETRII		16		//liczba bajtów w ramce telemetrii identyfikujaca przesyłane zmienne
#define LICZBA_RAMEK_TELEMETR			2		//obecnie są 2 ramki dla zmiennych 0..127 i 128..256
#define OKRESOW_TELEMETRII_W_RAMCE		120		//w ramce przesyłane jest na raz się tyle 16-bitowych okresów telemetrii


#define KWANT_CZASU_TELEMETRII	0.01f	//[s]
#define MAX_CZESTOTLIWOSC_TELEMETRII (1 / KWANT_CZASU_TELEMETRII)
#define TEMETETRIA_WYLACZONA	0xFFFF
