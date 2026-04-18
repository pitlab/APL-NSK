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
#define PID_KATA_PRZE 	0   //regulator sterowania przechyleniem (lotkami w samolocie)
#define PID_PRED_PRZE 	1   //regulator sterowania prędkością kątową przechylenia (żyroskop P)
#define PID_KATA_POCH 	2   //regulator sterowania pochyleniem (sterem wysokości)
#define PID_PRED_POCH 	3   //regulator sterowania prędkością kątową pochylenia (żyroskop Q)
#define PID_KATA_ODCH 	4  	//regulator sterowania odchyleniem (sterem kierunku)
#define PID_PRED_ODCH	5   //regulator sterowania prędkością kątową odchylenia (żyroskop R)
#define PID_WYSOKOSCI 	6   //regulator sterowania wysokością
#define PID_WARIO 		7   //regulator sterowani prędkością zmiany wysokości (wario)
#define PID_NAWIG_N 	8   //regulator sterowania nawigacją w kierunku północnym
#define PID_PREDK_N		9  	//regulator sterowania prędkością w kierunku północnym
#define PID_NAWIG_E 	10  //regulator sterowania nawigacją w kierunku wschodnim
#define PID_PREDK_E		11 	//regulator sterowania prędkością w kierunku wschodnim

#define LICZBA_PID  (2 * LICZBA_REG_PARAM)	//liczba regulatorów
#define LICZBA_KAN_RC_DO_STROJENIA_PID	2	//tyle kanałów RC jest używanych do strojenia wybranych parametrów PID
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

//definicje strojonych parametrów regulatorów PID
#define STRP_NIC				0	//strojenie wyłączone
#define STRP_KATA_PRZE_KP		1	//strojenie wzmocnienia w regulatorze przechylenia
#define STRP_KATA_PRZE_TI		2	//strojenie członu całkujacego w regulatorze przechylenia
#define STRP_KATA_PRZE_TD		3	//strojenie członu różniczkującego w regulatorze przechylenia
#define STRP_PRED_PRZE_KP		4	//strojenie wzmocnienia w regulatorze prędkości kątowej przechylenia
#define STRP_PRED_PRZE_TI		5	//strojenie członu całkujacego w regulatorze prędkości kątowej przechylenia
#define STRP_PRED_PRZE_TD		6	//strojenie członu różniczkującego w regulatorze prędkości kątowej przechylenia
#define STRP_KATA_POCH_KP		7	//strojenie wzmocnienia w regulatorze pochylenia
#define STRP_KATA_POCH_TI		8	//strojenie członu całkujacego w regulatorze pochylenia
#define STRP_KATA_POCH_TD		9	//strojenie członu różniczkującego w regulatorze pochylenia
#define STRP_PRED_POCH_KP		10	//strojenie wzmocnienia w regulatorze prędkości kątowej pochylenia
#define STRP_PRED_POCH_TI		11	//strojenie członu całkujacego w regulatorze prędkości kątowej pochylenia
#define STRP_PRED_POCH_TD		12	//strojenie członu różniczkującego w regulatorze prędkości kątowej pochylenia
#define STRP_KATA_ODCH_KP		13	//strojenie wzmocnienia w regulatorze odchylenia
#define STRP_KATA_ODCH_TI		14	//strojenie członu całkujacego w regulatorze odchylenia
#define STRP_KATA_ODCH_TD		15	//strojenie członu różniczkującego w regulatorze odchylenia
#define STRP_PRED_ODCH_KP		16	//strojenie wzmocnienia w regulatorze prędkości kątowej odchylenia
#define STRP_PRED_ODCH_TI		17	//strojenie członu całkujacego w regulatorze prędkości kątowej odchylenia
#define STRP_PRED_ODCH_TD		18	//strojenie członu różniczkującego w regulatorze prędkości kątowej odchylenia
#define STRP_WYSOK_KP			19	//strojenie wzmocnienia w regulatorze wysokości
#define STRP_WYSOK_TI			20	//strojenie członu całkujacego w regulatorze wysokości
#define STRP_WYSOK_TD			21	//strojenie członu różniczkującego w regulatorze wysokości
#define STRP_WARIO_KP			22	//strojenie wzmocnienia w regulatorze prędkości zmiany wysokości
#define STRP_WARIO_TI			23	//strojenie członu całkujacego w regulatorze prędkości zmiany wysokości
#define STRP_WARIO_TD			24	//strojenie członu różniczkującego w regulatorze prędkości zmiany wysokości
#define STRP_NAWI_N_KP			25	//strojenie wzmocnienia w regulatorze nawigacji w kierunku północnym
#define STRP_NAWI_N_TI			26	//strojenie członu całkujacego w regulatorze nawigacji w kierunku północnym
#define STRP_NAWI_N_TD			27	//strojenie członu różniczkującego w regulatorze nawigacji w kierunku północnym
#define STRP_PRED_N_KP			28	//strojenie wzmocnienia w regulatorze prędkości w kierunku północnym
#define STRP_PRED_N_TI			29	//strojenie członu całkujacego w regulatorze prędkości w kierunku północnym
#define STRP_PRED_N_TD			30	//strojenie członu różniczkującego w regulatorze prędkości w kierunku północnym
#define STRP_NAWI_E_KP			31	//strojenie wzmocnienia w regulatorze nawigacji w kierunku wschodnim
#define STRP_NAWI_E_TI			32	//strojenie członu całkujacego w regulatorze nawigacji w kierunku wschodnim
#define STRP_NAWI_E_TD			33	//strojenie członu różniczkującego w regulatorze nawigacji w kierunku wschodnim
#define STRP_PRED_E_KP			34	//strojenie wzmocnienia w regulatorze prędkości w kierunku wschodnim
#define STRP_PRED_E_TI			35	//strojenie członu całkujacego w regulatorze prędkości w kierunku wschodnim
#define STRP_PRED_E_TD			36	//strojenie członu różniczkującego w regulatorze prędkości w kierunku wschodnim

#define LICZBA_STROJONYCH_PARAMETROW_PID	37






//definicje trybów regulacji
#define REG_WYLACZ		0		//regultor wyłączony
#define REG_RECZNA		1		//regulacja ręczna, bezpośrednio z drążków aparatury
#define REG_AKRO		2		//regulacja akrobacyjna, steruje pochodną parametru głównego: prędkością kątową lub prędkości zmiany wysokości
#define REG_STAB		3		//regulacja stabilizująca, steruje parametrem głównym: kątem lub wysokością
#define REG_AUTO		4		//regulacja automatyczna, steruje wartością nadrzędną czyli nawigacją po wspołrzędnych geograficznych

#define NUM_REG_MOD       6   //liczba trybów regulatora

//#define KANALY_SERW			16	//liczba sterowanych kanałów serw
#define KANALY_WYJSC_RC		16	//liczba sterowanych kanałów serw
#define KANALY_ODB_RC		16	//liczba odbieranych kanałów na każdym z dwu wejść odbiorników RC
#define KANALY_MIKSERA		8	//liczba kanałów wyjściowych, które mogą wchodzić do miksera
#define KANALY_FUNKCYJNE	(KANALY_ODB_RC - LICZBA_DRAZKOW)	//liczba kanałów mogących uruchamiać funkcje autopilota
#define LICZBA_DRAZKOW		4	//liczba regulatorów które mają wartość zadaną ustawianą drążkami aparatury


//definicje bitów konfiguracji PID
#define PID_MASKA_FILTRA_D		0x3F
//#define PID_WLACZONY			0x40
#define PID_KATOWY				0x80
