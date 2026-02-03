//#define NUM_AXIS  6 //liczba regulowanych osi: pochylenie, przechylenie, odchylenie, wysokość, prędkość + rezerwa
//#define FRAM_FLOAT_SIZE     4   //rozmiar liczby float
#define _USE_MATH_DEFINES
#include <cmath>

#define DEG2RAD	(M_PI/180)
#define RAD2DEG	(180/M_PI)


//Rodzaj regulatora
#define REG_KAT     1   //regulator kątowy
#define REG_LIN     0   //regulator liniowy

//definicje nazw regulowanych parametrów
#define PRZE	0    //regulator sterowania przechyleniem (lotkami w samolocie)
#define POCH	1    //regulator sterowania pochyleniem (sterem wysokości)
#define ODCH    2    //regulator sterowania obrotem (sterem kierunku)
#define WYSO	3    //regulator sterowania wysokością
#define POZN	4    //regulator sterowania prędkością i położeniem północnym
#define POZE	5    //regulator sterowania prędkością i położeniem wschodnim

#define LICZBA_REG_PARAM  6 //liczba regulowanych parametrów


//definicje nazw regulatorów
#define PID_PRZE 	0   //regulator sterowania przechyleniem (lotkami w samolocie)
#define PID_PK_PRZE 1   //regulator sterowania prędkością kątową przechylenia (żyroskop P)
#define PID_POCH 	2   //regulator sterowania pochyleniem (sterem wysokości)
#define PID_PK_POCH 3   //regulator sterowania prędkością kątową pochylenia (żyroskop Q)
#define PID_ODCH 	4  	//regulator sterowania odchyleniem (sterem kierunku)
#define PID_PK_ODCH	5   //regulator sterowania prędkością kątową odchylenia (żyroskop R)
#define PID_WYSO 	6   //regulator sterowania wysokością
#define PID_WARIO 	7   //regulator sterowani prędkością wznoszenia (wario)
#define PID_NAW_N 	8   //regulator sterowania nawigacją w kierunku północnym
#define PID_PRE_N	9  	//regulator sterowania prędkością w kierunku północnym
#define PID_NAW_E 	10  //regulator sterowania nawigacją w kierunku wschodnim
#define PID_PRE_E	11 	//regulator sterowania prędkością w kierunku wschodnim

#define LICZBA_PID  (2 * LICZBA_REG_PARAM)	//liczba regulatorów
#define ROZMIAR_PID_FLOAT	8

//definicje trybów pracy regulatora
#define REG_WYLACZ		0	//regultor wyłączony
#define REG_RECZNA		1	//regulacja ręczna, bezpośrednio z drążków aparatury
#define REG_AKRO		2	//regulacja akrobacyjna, steruje pochodną parametru głównego: prędkością kątową lub prędkości zmiany wysokości
#define REG_STAB		3	//regulacja stabilizująca, steruje parametrem głównym: kątem lub wysokością
#define REG_AUTO		4	//regulacja automatyczna, steruje wartością nadrzędną czyli nawigacją po wspołrzędnych geograficznych
#define REG_GPS_SPEED   4   //regulacja prędkości liniowej w osiach XYZ
#define REG_GPS_POS     5   //regulacja prędkości liniowej w osiach XYZ ze stabilizacją położenia
//definicje trybów regulacji








//definicje trybów regulacji
#define REG_WYLACZ		0		//regultor wyłączony
#define REG_RECZNA		1		//regulacja ręczna, bezpośrednio z drążków aparatury
#define REG_AKRO		2		//regulacja akrobacyjna, steruje pochodną parametru głównego: prędkością kątową lub prędkości zmiany wysokości
#define REG_STAB		3		//regulacja stabilizująca, steruje parametrem głównym: kątem lub wysokością
#define REG_AUTO		4		//regulacja automatyczna, steruje wartością nadrzędną czyli nawigacją po wspołrzędnych geograficznych

#define NUM_REG_MOD       6   //liczba trybów regulatora

#define KANALY_SERW			16	//liczba sterowanych kanałów serw
#define KANALY_ODB_RC		16	//liczba odbieranych kanałów na każdym z dwu wejść odbiorników RC
#define KANALY_MIKSERA		8	//liczba kanałów wyjściowych, które mogą wchodzić do miksera
#define KANALY_FUNKCYJNE	(KANALY_ODB_RC - LICZBA_DRAZKOW)	//liczba kanałów mogących uruchamiać funkcje autopilota
#define LICZBA_DRAZKOW		4	//liczba regulatorów które mają wartość zadaną ustawianą drążkami aparatury


//definicje bitów konfiguracji PID
#define PID_MASKA_FILTRA_D		0x3F
//#define PID_WLACZONY			0x40
#define PID_KATOWY				0x80
