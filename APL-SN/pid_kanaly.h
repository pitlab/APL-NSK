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
#define STRP_KATA_PRZE_KI		2	//strojenie członu całkujacego w regulatorze przechylenia
#define STRP_KATA_PRZE_KD		3	//strojenie członu różniczkującego w regulatorze przechylenia
#define STRP_KATA_PRZE_WYPRZ	4	//strojenie wzmocnienia akcji wyprzedzającej
#define STRP_KATA_PRZE_FD		5	//Strojenie filtra sygnału różniczkowanego
#define STRP_KATA_PRZE_FWZ		6	//Strojenie filtra wartości zadanej
#define STRP_KATA_PRZE_FWE		7	//Strojenie filtra wartości wejściowej
#define STRP_PRED_PRZE_KP		8	//strojenie wzmocnienia w regulatorze prędkości kątowej przechylenia
#define STRP_PRED_PRZE_KI		9	//strojenie członu całkujacego w regulatorze prędkości kątowej przechylenia
#define STRP_PRED_PRZE_KD		10	//strojenie członu różniczkującego w regulatorze prędkości kątowej przechylenia
#define STRP_PRED_PRZE_WYPRZ	11	//strojenie wzmocnienia akcji wyprzedzającej
#define STRP_PRED_PRZE_FD		12	//Strojenie filtra sygnału różniczkowanego
#define STRP_PRED_PRZE_FWZ		13	//Strojenie filtra wartości zadanej
#define STRP_PRED_PRZE_FWE		14	//Strojenie filtra wartości wejściowej

#define STRP_KATA_POCH_KP		15	//strojenie wzmocnienia w regulatorze pochylenia
#define STRP_KATA_POCH_KI		16	//strojenie członu całkujacego w regulatorze pochylenia
#define STRP_KATA_POCH_KD		17	//strojenie członu różniczkującego w regulatorze pochylenia
#define STRP_KATA_POCH_WYPRZ	18	//strojenie wzmocnienia akcji wyprzedzającej
#define STRP_KATA_POCH_FD		19	//Strojenie filtra sygnału różniczkowanego
#define STRP_KATA_POCH_FWZ		20	//Strojenie filtra wartości zadanej
#define STRP_KATA_POCH_FWE		21	//Strojenie filtra wartości wejściowej
#define STRP_PRED_POCH_KP		22	//strojenie wzmocnienia w regulatorze prędkości kątowej pochylenia
#define STRP_PRED_POCH_KI		23	//strojenie członu całkujacego w regulatorze prędkości kątowej pochylenia
#define STRP_PRED_POCH_KD		24	//strojenie członu różniczkującego w regulatorze prędkości kątowej pochylenia
#define STRP_PRED_POCH_WYPRZ	25	//strojenie wzmocnienia akcji wyprzedzającej
#define STRP_PRED_POCH_FD		26	//Strojenie filtra sygnału różniczkowanego
#define STRP_PRED_POCH_FWZ		27	//Strojenie filtra wartości zadanej
#define STRP_PRED_POCH_FWE		28	//Strojenie filtra wartości wejściowej

#define STRP_KATA_ODCH_KP		29	//strojenie wzmocnienia w regulatorze odchylenia
#define STRP_KATA_ODCH_KI		30	//strojenie członu całkujacego w regulatorze odchylenia
#define STRP_KATA_ODCH_KD		31	//strojenie członu różniczkującego w regulatorze odchylenia
#define STRP_KATA_ODCH_WYPRZ	32	//strojenie wzmocnienia akcji wyprzedzającej
#define STRP_KATA_ODCH_FD		33	//Strojenie filtra sygnału różniczkowanego
#define STRP_KATA_ODCH_FWZ		34	//Strojenie filtra wartości zadanej
#define STRP_KATA_ODCH_FWE		35	//Strojenie filtra wartości wejściowej
#define STRP_PRED_ODCH_KP		36	//strojenie wzmocnienia w regulatorze prędkości kątowej odchylenia
#define STRP_PRED_ODCH_KI		37	//strojenie członu całkujacego w regulatorze prędkości kątowej odchylenia
#define STRP_PRED_ODCH_KD		38	//strojenie członu różniczkującego w regulatorze prędkości kątowej odchylenia
#define STRP_PRED_ODCH_WYPRZ	39	//strojenie wzmocnienia akcji wyprzedzającej
#define STRP_PRED_ODCH_FD		40	//Strojenie filtra sygnału różniczkowanego
#define STRP_PRED_ODCH_FWZ		41	//Strojenie filtra wartości zadanej
#define STRP_PRED_ODCH_FWE		42	//Strojenie filtra wartości wejściowej

#define STRP_WYSOKOSCI_KP		43	//strojenie wzmocnienia w regulatorze wysokości
#define STRP_WYSOKOSCI_KI		44	//strojenie członu całkujacego w regulatorze wysokości
#define STRP_WYSOKOSCI_KD		45	//strojenie członu różniczkującego w regulatorze wysokości
#define STRP_WYSOKOSCI_WYPRZ	46	//strojenie wzmocnienia akcji wyprzedzającej
#define STRP_WYSOKOSCI_FD		47	//Strojenie filtra sygnału różniczkowanego
#define STRP_WYSOKOSCI_FWZ		48	//Strojenie filtra wartości zadanej
#define STRP_WYSOKOSCI_FWE		49	//Strojenie filtra wartości wejściowej
#define STRP_PRED_ZWYS_KP		50	//strojenie wzmocnienia w regulatorze prędkości zmiany wysokości
#define STRP_PRED_ZWYS_KI		51	//strojenie członu całkujacego w regulatorze prędkości zmiany wysokości
#define STRP_PRED_ZWYS_KD		52	//strojenie członu różniczkującego w regulatorze prędkości zmiany wysokości
#define STRP_PRED_ZWYS_WYPRZ	53	//strojenie wzmocnienia akcji wyprzedzającej
#define STRP_PRED_ZWYS_FD		54	//Strojenie filtra sygnału różniczkowanego
#define STRP_PRED_ZWYS_FWZ		55	//Strojenie filtra wartości zadanej
#define STRP_PRED_ZWYS_FWE		56	//Strojenie filtra wartości wejściowej

#define STRP_NAWI_POLN_KP		57	//strojenie wzmocnienia w regulatorze nawigacji w kierunku północnym
#define STRP_NAWI_POLN_KI		58	//strojenie członu całkujacego w regulatorze nawigacji w kierunku północnym
#define STRP_NAWI_POLN_KD		59	//strojenie członu różniczkującego w regulatorze nawigacji w kierunku północnym
#define STRP_PRED_POLN_KP		60	//strojenie wzmocnienia w regulatorze prędkości w kierunku północnym
#define STRP_PRED_POLN_KI	 	61	//strojenie członu całkujacego w regulatorze prędkości w kierunku północnym
#define STRP_PRED_POLN_KD		62	//strojenie członu różniczkującego w regulatorze prędkości w kierunku północnym
#define STRP_NAWI_WSCH_KP		63	//strojenie wzmocnienia w regulatorze nawigacji w kierunku wschodnim
#define STRP_NAWI_WSCH_KI		64	//strojenie członu całkujacego w regulatorze nawigacji w kierunku wschodnim
#define STRP_NAWI_WSCH_KD		65	//strojenie członu różniczkującego w regulatorze nawigacji w kierunku wschodnim
#define STRP_PRED_WSCH_KP		66	//strojenie wzmocnienia w regulatorze prędkości w kierunku wschodnim
#define STRP_PRED_WSCH_KI		67	//strojenie członu całkujacego w regulatorze prędkości w kierunku wschodnim
#define STRP_PRED_WSCH_KD		68	//strojenie członu różniczkującego w regulatorze prędkości w kierunku wschodnim
#define LICZBA_STROJONYCH_PARAMETROW_PID	69

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
