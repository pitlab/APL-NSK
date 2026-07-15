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


#define NAGLOWEK				0xAA
#define ADRES_STACJI			0x00
#define ADRES_BROADCAST			0xFF
#define WIELOMIAN_CRC			0x1021
#define DLUGOSC_NAZWY			20		//maksymalna długość nazwy BSP, zmiennych telemetrycznych, lub innych nazw
#define ROZMIAR_ROZNE_FLOAT		12
#define ROZMIAR_ROZNE_SHORT		24
#define ROZMIAR_ROZNE_CHAR		48

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
//#define PK_REKONFIG_SERWA_RC	22	//wykonuje ponowną konfigurację wejść i wyjść RC po zmianie zawartosci FRAM
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
#define PK_WYLACZ_POLECENIE_CM4	33	//wykonuje polecenie POL_NIC wyłączajac wykonywanie poprzedniego polecenia
#define PK_PRZELADUJ_WSKAZN_LED	34	//odczytaj konfigurację wskaźników LED z pamieci FRAM i załaduj do zmiennych aby wprowadzona zmiana stała się widoczna
#define PK_CZYTAJ_PARAMETRY_FFT	35	//odczytaj parametry pracy FFT z APL3
#define PK_ZAPISZ_PARAMETRY_FFT	36	//zapisz parametry pracy FFT
#define PK_CZYTAJ_WYNIKI_FFT	37	//odczytaj z pamiędci DRAM wyniki serii testów FFT dla akcelerometrów i żyroskopów
#define PK_ROZP_ANALIZE_DRGAN	38	//rozpoczyna proces analizy drgań przez wykonywanie serii pomiarów FFT i coraz szybszym kręceniu silnikami
#define PK_ZATRZYMAJ_SILNIKI	39	//zatrzymuje silniki w trakcie testu FFT
#define PK_PRZELADUJ_KONF_PID	40	//przeładuj konfigurację PID po zmianie
#define PK_REKONFIG_WEJSCIA_RC	41	//wykonuje ponowną konfigurację wejść RC po zmianie konfiguracji we FRAM
#define PK_REKONFIG_WYJSCIA_RC	42	//wykonuje ponowną konfigurację wyjść RC po zmianie konfiguracji we FRAM
#define PK_URUCHOM_INDENT_SILN	43	//uruchamia proces identyfikacji silników, kręcąc kolejno każdym z nich

#define PK_ILOSC_POLECEN		44	//liczba poleceń do sprawdzania czy polecenie mieści się w obsługiwanych granicach


#define PK_TELEM_FFT			95	//szybka ramka telemetryczna do przesyłania wyników FFT
#define PK_TELEMETRIA1			96	//ramka telemetryczna 1
#define PK_TELEMETRIA2			97	//ramka telemetryczna 2
#define PK_TELEMETRIA3			98	//ramka telemetryczna 3 - na razie nie używane
#define PK_TELEMETRIA4			99	//ramka telemetryczna 4 - na razie nie używane


//polecenia ramki PK_WSTRZYMAJ_TELEMETRIE służące do zarządzania strumieniem telemetrii
#define TELEM_SZYBKA	2	//ma być transmitowana szybka ramka telemetryczna z wynikami FFT
#define TELEM_WSTRZYMAJ	1	//zatrzymaj wysyłanie telemetrii
#define TELEM_WZNOW		0	//wznów wysyłanie telemetrii


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

//definicja wstępych wartości danych telemetrycznych w celu znalezienia zakresu jego zmienności
#define WARTOSC_MAX		100000.0f
#define WARTOSC_MIN		-100000.0f

//definicje zmiennych telemetrycznych
//zmienne IMU
#define TID_AKCEL1X				0
#define TID_AKCEL1Y				1
#define TID_AKCEL1Z				2
#define TID_AKCEL2X				3
#define TID_AKCEL2Y				4
#define TID_AKCEL2Z				5
#define TID_ZYRO1P				6
#define TID_ZYRO1Q				7
#define TID_ZYRO1R				8
#define TID_ZYRO2P				9
#define TID_ZYRO2Q				10
#define TID_ZYRO2R				11
#define TID_MAGNE1X				12
#define TID_MAGNE1Y				13
#define TID_MAGNE1Z				14
#define TID_MAGNE2X				15
#define TID_MAGNE2Y				16
#define TID_MAGNE2Z				17
#define TID_MAGNE3X				18
#define TID_MAGNE3Y				19
#define TID_MAGNE3Z				20
#define TID_TEMPIMU1			21
#define TID_TEMPIMU2			22

//zmienne AHRS
#define TID_KAT_IMU1X			23
#define TID_KAT_IMU1Y			24
#define TID_KAT_IMU1Z			25
#define TID_KAT_IMU2X			26
#define TID_KAT_IMU2Y			27
#define TID_KAT_IMU2Z			28
#define TID_KAT_ZYRO1X			29
#define TID_KAT_ZYRO1Y			30
#define TID_KAT_ZYRO1Z			31
#define TID_KAT_AKCELX			32
#define TID_KAT_AKCELY			33
#define TID_KAT_AKCELZ			34

//zmienne barometryczne
#define TID_CISBEZW1			35
#define TID_CISBEZW2			36
#define TID_WYSOKOSC_AGL1		37
#define TID_WYSOKOSC_AGL2		38
#define TID_WYSOKOSC_MSL1		39
#define TID_WYSOKOSC_MSL2		40
#define TID_CISROZN1			41
#define TID_CISROZN2			42
#define TID_PREDIAS1			43
#define TID_PREDIAS2			44
#define TID_WARIO1				45
#define TID_WARIO2				46
#define TID_TEMPCISB1			47
#define TID_TEMPCISB2			48
#define TID_TEMPCISR1			49
#define TID_TEMPCISR2			50

#define TID_RC_KAN1				51		//kanał 1 odbiorników RC
#define TID_RC_KAN2				52
#define TID_RC_KAN3				53
#define TID_RC_KAN4				54
#define TID_RC_KAN5				55
#define TID_RC_KAN6				56
#define TID_RC_KAN7				57
#define TID_RC_KAN8				58
#define TID_RC_KAN9				59
#define TID_RC_KAN10			60
#define TID_RC_KAN11			61
#define TID_RC_KAN12			62
#define TID_RC_KAN13			63
#define TID_RC_KAN14			64
#define TID_RC_KAN15			65
#define TID_RC_KAN16			66

#define TID_SERWO1				67		//serwo 1
#define TID_SERWO2				68
#define TID_SERWO3				69
#define TID_SERWO4				70
#define TID_SERWO5				71
#define TID_SERWO6				72
#define TID_SERWO7				73
#define TID_SERWO8				74
#define TID_SERWO9				75
#define TID_SERWO10				76
#define TID_SERWO11				77
#define TID_SERWO12				78
#define TID_SERWO13				79
#define TID_SERWO14				80
#define TID_SERWO15				81
#define TID_SERWO16				82

#define TID_BSP_AKCELX			83
#define TID_BSP_AKCELY			84
#define TID_BSP_AKCELZ			85
#define TID_BSP_ZYROP			86
#define TID_BSP_ZYROQ			87
#define TID_BSP_ZYROR			88
#define TID_BSP_MAGNEX			89
#define TID_BSP_MAGNEY			90
#define TID_BSP_MAGNEZ			91

#define TID_BSP_IMUX			92
#define TID_BSP_IMUY			93
#define TID_BSP_IMUZ			94
#define TID_BSP_AGL				95
#define TID_BSP_AMSL			96
#define TID_BSP_IAS				97
#define TID_BSP_KURS			98

#define TID_BSP_PRED_POLN		99
#define TID_BSP_PRED_WSCH		100
#define TID_BSP_PRED_WDOL		101
#define TID_BSP_SZER_GEO		102
#define TID_BSP_DLUG_GEO		103

#define TID_DOTYK_ADC0			104
#define TID_DOTYK_ADC1			105
#define TID_DOTYK_ADC2			106
#define TID_CZAS_PETLI			107	//czas trwania ostatniej petli głównej w us
#define TID_JAKOSC_UP_RC1		108
#define TID_JAKOSC_UP_RC2		109
#define TID_JAKOSC_DOWN_RC		110

#define TID_ADC1_1				111
#define TID_ADC1_2				112
#define TID_ADC2_1				113
#define TID_ADC2_2				114
#define TID_BAT1_NAPIECIE		115
#define TID_BAT1_PRAD			116
#define TID_BAT1_ENERGIA		117
#define TID_BAT2_NAPIECIE		118
#define TID_BAT2_PRAD			119
#define TID_BAT2_ENERGIA		120
#define TID_BAT_RTC_NAPIECIE	121
#define TID_TEMPERATURA_CPU		122
#define TID_NAPIECIE_WE1		123
#define TID_NAPIECIE_WE2		124
#define TID_NAPIECIE_SERW		125
#define TID_NAPIECIE_USB		126
#define TID_FFT_ZYRO_AKCEL		127	//wyniki transformaty Fouriera przesyłane w specyficznej szybkiej ramce


//zmiene telemetryczne w ramce 2
#define TID_PID_PRZE_WZAD		128	//wartość zadana regulatora sterowania przechyleniem
#define TID_PID_PRZE_FWEJ		129	//przefiltrowana wartość wejściowa do liczenia akcji różniczkującej
#define TID_PID_PRZE_WYJ		130	//wyjście regulatora sterowania przechyleniem
#define TID_PID_PRZE_WY_P		131	//wyjście członu P
#define TID_PID_PRZE_WY_I		132	//wyjście członu I
#define TID_PID_PRZE_WY_D		133	//wyjście członu D
#define TID_PID_PK_PRZE_WZAD	134	//wartość zadana regulatora sterowania prędkością kątową przechylenia
#define TID_PID_PK_PRZE_FWEJ	135	//przefiltrowana wartość wejściowa do liczenia akcji różniczkującej
#define TID_PID_PK_PRZE_FZAD	136	//przefiltrowana wartość zadana do liczenia wartosci wyprzedzającej
#define TID_PID_PK_PRZE_WYJ		137	//wyjście regulatora sterowania prędkością kątową przechylenia
#define TID_PID_PK_PRZE_WY_P	138	//wyjście członu P
#define TID_PID_PK_PRZE_WY_D	139	//wyjście członu D
#define TID_PID_PK_PRZE_WYPRZ	140	//wyjście akcji wyprzedzającej

#define TID_PID_POCH_WZAD		142	//wartość zadana regulatora sterowania pochyleniem
#define TID_PID_POCH_FWEJ		143 //przefiltrowana wartość wejściowa do liczenia akcji różniczkującej
#define TID_PID_POCH_WYJ		144	//wyjście regulatora sterowania pochyleniem
#define TID_PID_POCH_WY_P		145	//wyjście członu P
#define TID_PID_POCH_WY_I		146	//wyjście członu I
#define TID_PID_POCH_WY_D		147	//wyjście członu D
#define TID_PID_PK_POCH_WZAD	148	//wartość zadana
#define TID_PID_PK_POCH_FWEJ	149	//przefiltrowana wartość wejściowa do liczenia akcji różniczkującej
#define TID_PID_PK_POCH_FZAD	150	//przefiltrowana wartość zadana do liczenia wartosci wyprzedzającej
#define TID_PID_PK_POCH_WYJ		151	//wyjście regulatora sterowania prędkością kątową pochylenia
#define TID_PID_PK_POCH_WY_P	152	//wyjście członu P
#define TID_PID_PK_POCH_WY_D	153	//wyjście członu D
#define TID_PID_PK_POCH_WYPRZ	154	//wyjście akcji wyprzedzającej

#define TID_PID_ODCH_WZAD		156	//wartość zadana regulatora sterowania odchyleniem
#define TID_PID_ODCH_FWEJ		157	//przefiltrowana wartość wejściowa do liczenia akcji różniczkującej
#define TID_PID_ODCH_WYJ		158	//wyjście regulatora sterowania odchyleniem
#define TID_PID_ODCH_WY_P		159	//wyjście członu P
#define TID_PID_ODCH_WY_I		160	//wyjście członu I
#define TID_PID_ODCH_WY_D		161	//wyjście członu D
#define TID_PID_PK_ODCH_WZAD	162	//wartość zadana
#define TID_PID_PK_ODCH_FWEJ	163	//przefiltrowana wartość wejściowa do liczenia akcji różniczkującej
#define TID_PID_PK_ODCH_FZAD	164	//przefiltrowana wartość zadana do liczenia wartosci wyprzedzającej
#define TID_PID_PK_ODCH_WYJ		165	//wyjście regulatora sterowania prędkością kątową odchylenia
#define TID_PID_PK_ODCH_WY_P	166	//wyjście członu P
#define TID_PID_PK_ODCH_WY_D	167	//wyjście członu D
#define TID_PID_PK_ODCH_WYPRZ	168	//wyjście akcji wyprzedzającej

#define TID_PID_WYSO_WZAD		170	//wartość zadana regulatora sterowania wysokością
#define TID_PID_WYSO_FWEJ		171	//przefiltrowana wartość wejściowa do liczenia akcji różniczkującej
#define TID_PID_WYSO_WYJ		172	//wyjście regulatora sterowania odchyleniem
#define TID_PID_WYSO_WY_P		173	//wyjście członu P
#define TID_PID_WYSO_WY_I		174	//wyjście członu I
#define TID_PID_WYSO_WY_D		175	//wyjście członu D
#define TID_PID_PR_WYSO_WZAD	176	//wartość zadana
#define TID_PID_PR_WYSO_FWEJ	177	//przefiltrowana wartość wejściowa do liczenia akcji różniczkującej
#define TID_PID_PR_WYSO_FZAD	178	//przefiltrowana wartość zadana do liczenia wartosci wyprzedzającej
#define TID_PID_PR_WYSO_WYJ		179	//wyjście regulatora sterowania prędkością zmiany wysokości
#define TID_PID_PR_WYSO_WY_P	180	//wyjście członu P
#define TID_PID_PR_WYSO_WY_D	181	//wyjście członu D
#define TID_PID_PR_WYSO_WYPRZ	182	//wyjście akcji wyprzedzającej

#define TID_PID_NAWN_WZAD		184	//wartość zadana regulatora sterowania nawigacją w kierunku północnym
#define TID_PID_NAWN_FWEJ		185	//przefiltrowana wartość wejściowa do liczenia akcji różniczkującej
#define TID_PID_NAWN_WYJ		186	//wyjście regulatora sterowania nawigacją w kierunku północnym
#define TID_PID_NAWN_WY_P		187	//wyjście członu P
#define TID_PID_NAWN_WY_I		188	//wyjście członu I
#define TID_PID_NAWN_WY_D		189	//wyjście członu D
#define TID_PID_PR_NAWN_WZAD	190	//wartość zadana
#define TID_PID_PR_NAWN_FWEJ	191	//przefiltrowana wartość wejściowa do liczenia akcji różniczkującej
#define TID_PID_PR_NAWN_WYJ		192	//wyjście regulatora sterowania prędkością w kierunku północnym
#define TID_PID_PR_NAWN_WY_P	193	//wyjście członu P
#define TID_PID_PR_NAWN_WY_I	194	//wyjście członu I
#define TID_PID_PR_NAWN_WY_D	195	//wyjście członu D

#define TID_PID_NAWE_WZAD		197	//wartość zadana regulatora sterowania nawigacją w kierunku wschodnim
#define TID_PID_NAWE_FWEJ		198	//przefiltrowana wartość wejściowa do liczenia akcji różniczkującej
#define TID_PID_NAWE_WYJ		199	//wyjście regulatora sterowania nawigacją w kierunku północnym
#define TID_PID_NAWE_WY_P		200	//wyjście członu P
#define TID_PID_NAWE_WY_I		201	//wyjście członu I
#define TID_PID_NAWE_WY_D		202	//wyjście członu D
#define TID_PID_PR_NAWE_WZAD	203	//wartość zadana
#define TID_PID_PR_NAWE_FWEJ	204	//przefiltrowana wartość wejściowa do liczenia akcji różniczkującej
#define TID_PID_PR_NAWE_WYJ		205	//wyjście regulatora sterowania prędkością w kierunku wschodnim
#define TID_PID_PR_NAWE_WY_P	206	//wyjście członu P
#define TID_PID_PR_NAWE_WY_I	207	//wyjście członu I
#define TID_PID_PR_NAWE_WY_D	208	//wyjście członu D

#define TID_PID_STROJENIE1		210	//wartość parametru strojącego 1
#define TID_PID_STROJENIE2		211	//wartość parametru strojącego 2
//max do 255

#define LICZBA_ZMIENNYCH_TELEMETRYCZNYCH	214


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

