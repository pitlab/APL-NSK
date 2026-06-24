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
#define PRZE		0    //regulator sterowania przechyleniem (lotkami w samolocie)
#define POCH		1    //regulator sterowania pochyleniem (sterem wysokości)
#define ODCH	    2    //regulator sterowania obrotem (sterem kierunku)
#define WYSO		3    //regulator sterowania wysokością
#define POZN		4    //regulator sterowania prędkością i położeniem północnym
#define POZE		5    //regulator sterowania prędkością i położeniem wschodnim

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
//#define ROZMIAR_PID_FLOAT	8

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
#define STRP_KATA_PRZE_FD		4	//Strojenie filtra sygnału różniczkowanego
#define STRP_KATA_PRZE_FWZ		5	//Strojenie filtra wartości zadanej
#define STRP_KATA_PRZE_WYPRZ	6	//strojenie wielkości akcji wyprzedzającej
#define STRP_PRED_PRZE_KP		7	//strojenie wzmocnienia w regulatorze prędkości kątowej przechylenia
#define STRP_PRED_PRZE_TI		8	//strojenie członu całkujacego w regulatorze prędkości kątowej przechylenia
#define STRP_PRED_PRZE_TD		9	//strojenie członu różniczkującego w regulatorze prędkości kątowej przechylenia
#define STRP_PRED_PRZE_FD		10	//Strojenie filtra sygnału różniczkowanego
#define STRP_PRED_PRZE_FWZ		11	//Strojenie filtra wartości zadanej
#define STRP_PRED_PRZE_WYPRZ	12	//strojenie wielkości akcji wyprzedzającej

#define STRP_KATA_POCH_KP		13	//strojenie wzmocnienia w regulatorze pochylenia
#define STRP_KATA_POCH_TI		14	//strojenie członu całkujacego w regulatorze pochylenia
#define STRP_KATA_POCH_TD		15	//strojenie członu różniczkującego w regulatorze pochylenia
#define STRP_KATA_POCH_FD		16	//Strojenie filtra sygnału różniczkowanego
#define STRP_KATA_POCH_FWZ		17	//Strojenie filtra wartości zadanej
#define STRP_KATA_POCH_WYPRZ	18	//strojenie wielkości akcji wyprzedzającej
#define STRP_PRED_POCH_KP		19	//strojenie wzmocnienia w regulatorze prędkości kątowej pochylenia
#define STRP_PRED_POCH_TI		20	//strojenie członu całkujacego w regulatorze prędkości kątowej pochylenia
#define STRP_PRED_POCH_TD		21	//strojenie członu różniczkującego w regulatorze prędkości kątowej pochylenia
#define STRP_PRED_POCH_FD		22	//Strojenie filtra sygnału różniczkowanego
#define STRP_PRED_POCH_FWZ		23	//Strojenie filtra wartości zadanej
#define STRP_PRED_POCH_WYPRZ	24	//strojenie wielkości akcji wyprzedzającej
#define STRP_KATA_ODCH_KP		25	//strojenie wzmocnienia w regulatorze odchylenia
#define STRP_KATA_ODCH_TI		26	//strojenie członu całkujacego w regulatorze odchylenia
#define STRP_KATA_ODCH_TD		27	//strojenie członu różniczkującego w regulatorze odchylenia
#define STRP_KATA_ODCH_FD		28	//Strojenie filtra sygnału różniczkowanego
#define STRP_KATA_ODCH_FWZ		29	//Strojenie filtra wartości zadanej
#define STRP_KATA_ODCH_WYPRZ	30	//strojenie wielkości akcji wyprzedzającej
#define STRP_PRED_ODCH_KP		31	//strojenie wzmocnienia w regulatorze prędkości kątowej odchylenia
#define STRP_PRED_ODCH_TI		32	//strojenie członu całkujacego w regulatorze prędkości kątowej odchylenia
#define STRP_PRED_ODCH_TD		33	//strojenie członu różniczkującego w regulatorze prędkości kątowej odchylenia
#define STRP_PRED_ODCH_FD		34	//Strojenie filtra sygnału różniczkowanego
#define STRP_PRED_ODCH_FWZ		35	//Strojenie filtra wartości zadanej
#define STRP_PRED_ODCH_WYPRZ	36	//strojenie wielkości akcji wyprzedzającej
#define STRP_WYSOKOSCI_KP		37	//strojenie wzmocnienia w regulatorze wysokości
#define STRP_WYSOKOSCI_TI		38	//strojenie członu całkujacego w regulatorze wysokości
#define STRP_WYSOKOSCI_TD		39	//strojenie członu różniczkującego w regulatorze wysokości
#define STRP_WYSOKOSCI_FD		40	//Strojenie filtra sygnału różniczkowanego
#define STRP_WYSOKOSCI_FWZ		41	//Strojenie filtra wartości zadanej
#define STRP_WYSOKOSCI_WYPRZ	42	//strojenie wielkości akcji wyprzedzającej
#define STRP_PRED_ZWYS_KP		43	//strojenie wzmocnienia w regulatorze prędkości zmiany wysokości
#define STRP_PRED_ZWYS_TI		44	//strojenie członu całkujacego w regulatorze prędkości zmiany wysokości
#define STRP_PRED_ZWYS_TD		45	//strojenie członu różniczkującego w regulatorze prędkości zmiany wysokości
#define STRP_PRED_ZWYS_FD		46	//Strojenie filtra sygnału różniczkowanego
#define STRP_PRED_ZWYS_FWZ		47	//Strojenie filtra wartości zadanej
#define STRP_PRED_ZWYS_WYPRZ	48	//strojenie wielkości akcji wyprzedzającej
#define STRP_NAWI_POLN_KP		49	//strojenie wzmocnienia w regulatorze nawigacji w kierunku północnym
#define STRP_NAWI_POLN_TI		50	//strojenie członu całkujacego w regulatorze nawigacji w kierunku północnym
#define STRP_NAWI_POLN_TD		51	//strojenie członu różniczkującego w regulatorze nawigacji w kierunku północnym
#define STRP_PRED_POLN_KP		52	//strojenie wzmocnienia w regulatorze prędkości w kierunku północnym
#define STRP_PRED_POLN_TI		53	//strojenie członu całkujacego w regulatorze prędkości w kierunku północnym
#define STRP_PRED_POLN_TD		54	//strojenie członu różniczkującego w regulatorze prędkości w kierunku północnym
#define STRP_NAWI_WSCH_KP		55	//strojenie wzmocnienia w regulatorze nawigacji w kierunku wschodnim
#define STRP_NAWI_WSCH_TI		56	//strojenie członu całkujacego w regulatorze nawigacji w kierunku wschodnim
#define STRP_NAWI_WSCH_TD		57	//strojenie członu różniczkującego w regulatorze nawigacji w kierunku wschodnim
#define STRP_PRED_WSCH_KP		58	//strojenie wzmocnienia w regulatorze prędkości w kierunku wschodnim
#define STRP_PRED_WSCH_TI		59	//strojenie członu całkujacego w regulatorze prędkości w kierunku wschodnim
#define STRP_PRED_WSCH_TD		60	//strojenie członu różniczkującego w regulatorze prędkości w kierunku wschodnim
#define LICZBA_STROJONYCH_PARAMETROW_PID	61



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


//definicje bitów pola flagi konfiguracji PID
#define PID_KATOWY			0x01
