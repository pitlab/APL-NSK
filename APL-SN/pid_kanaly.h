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
#define STRP_KATA_PRZE_FWE		6	//Strojenie filtra wartości wejściowej
#define STRP_KATA_PRZE_WYPRZ	7	//strojenie wielkości akcji wyprzedzającej
#define STRP_PRED_PRZE_KP		8	//strojenie wzmocnienia w regulatorze prędkości kątowej przechylenia
#define STRP_PRED_PRZE_TI		9	//strojenie członu całkujacego w regulatorze prędkości kątowej przechylenia
#define STRP_PRED_PRZE_TD		10	//strojenie członu różniczkującego w regulatorze prędkości kątowej przechylenia
#define STRP_PRED_PRZE_FD		11	//Strojenie filtra sygnału różniczkowanego
#define STRP_PRED_PRZE_FWZ		12	//Strojenie filtra wartości zadanej
#define STRP_PRED_PRZE_FWE		13	//Strojenie filtra wartości wejściowej
#define STRP_PRED_PRZE_WYPRZ	14	//strojenie wielkości akcji wyprzedzającej

#define STRP_KATA_POCH_KP		15	//strojenie wzmocnienia w regulatorze pochylenia
#define STRP_KATA_POCH_TI		16	//strojenie członu całkujacego w regulatorze pochylenia
#define STRP_KATA_POCH_TD		17	//strojenie członu różniczkującego w regulatorze pochylenia
#define STRP_KATA_POCH_FD		18	//Strojenie filtra sygnału różniczkowanego
#define STRP_KATA_POCH_FWZ		19	//Strojenie filtra wartości zadanej
#define STRP_KATA_POCH_FWE		20	//Strojenie filtra wartości wejściowej
#define STRP_KATA_POCH_WYPRZ	21	//strojenie wielkości akcji wyprzedzającej
#define STRP_PRED_POCH_KP		22	//strojenie wzmocnienia w regulatorze prędkości kątowej pochylenia
#define STRP_PRED_POCH_TI		23	//strojenie członu całkujacego w regulatorze prędkości kątowej pochylenia
#define STRP_PRED_POCH_TD		24	//strojenie członu różniczkującego w regulatorze prędkości kątowej pochylenia
#define STRP_PRED_POCH_FD		25	//Strojenie filtra sygnału różniczkowanego
#define STRP_PRED_POCH_FWZ		26	//Strojenie filtra wartości zadanej
#define STRP_PRED_POCH_FWE		27	//Strojenie filtra wartości wejściowej
#define STRP_PRED_POCH_WYPRZ	28	//strojenie wielkości akcji wyprzedzającej

#define STRP_KATA_ODCH_KP		29	//strojenie wzmocnienia w regulatorze odchylenia
#define STRP_KATA_ODCH_TI		30	//strojenie członu całkujacego w regulatorze odchylenia
#define STRP_KATA_ODCH_TD		31	//strojenie członu różniczkującego w regulatorze odchylenia
#define STRP_KATA_ODCH_FD		32	//Strojenie filtra sygnału różniczkowanego
#define STRP_KATA_ODCH_FWZ		33	//Strojenie filtra wartości zadanej
#define STRP_KATA_ODCH_FWE		34	//Strojenie filtra wartości wejściowej
#define STRP_KATA_ODCH_WYPRZ	35	//strojenie wielkości akcji wyprzedzającej
#define STRP_PRED_ODCH_KP		36	//strojenie wzmocnienia w regulatorze prędkości kątowej odchylenia
#define STRP_PRED_ODCH_TI		37	//strojenie członu całkujacego w regulatorze prędkości kątowej odchylenia
#define STRP_PRED_ODCH_TD		38	//strojenie członu różniczkującego w regulatorze prędkości kątowej odchylenia
#define STRP_PRED_ODCH_FD		39	//Strojenie filtra sygnału różniczkowanego
#define STRP_PRED_ODCH_FWZ		40	//Strojenie filtra wartości zadanej
#define STRP_PRED_ODCH_FWE		41	//Strojenie filtra wartości wejściowej
#define STRP_PRED_ODCH_WYPRZ	42	//strojenie wielkości akcji wyprzedzającej

#define STRP_WYSOKOSCI_KP		43	//strojenie wzmocnienia w regulatorze wysokości
#define STRP_WYSOKOSCI_TI		44	//strojenie członu całkujacego w regulatorze wysokości
#define STRP_WYSOKOSCI_TD		45	//strojenie członu różniczkującego w regulatorze wysokości
#define STRP_WYSOKOSCI_FD		46	//Strojenie filtra sygnału różniczkowanego
#define STRP_WYSOKOSCI_FWZ		47	//Strojenie filtra wartości zadanej
#define STRP_WYSOKOSCI_FWE		48	//Strojenie filtra wartości wejściowej
#define STRP_WYSOKOSCI_WYPRZ	49	//strojenie wielkości akcji wyprzedzającej
#define STRP_PRED_ZWYS_KP		50	//strojenie wzmocnienia w regulatorze prędkości zmiany wysokości
#define STRP_PRED_ZWYS_TI		51	//strojenie członu całkujacego w regulatorze prędkości zmiany wysokości
#define STRP_PRED_ZWYS_TD		52	//strojenie członu różniczkującego w regulatorze prędkości zmiany wysokości
#define STRP_PRED_ZWYS_FD		53	//Strojenie filtra sygnału różniczkowanego
#define STRP_PRED_ZWYS_FWZ		54	//Strojenie filtra wartości zadanej
#define STRP_PRED_ZWYS_FWE		55	//Strojenie filtra wartości wejściowej
#define STRP_PRED_ZWYS_WYPRZ	56	//strojenie wielkości akcji wyprzedzającej

#define STRP_NAWI_POLN_KP		57	//strojenie wzmocnienia w regulatorze nawigacji w kierunku północnym
#define STRP_NAWI_POLN_TI		58	//strojenie członu całkujacego w regulatorze nawigacji w kierunku północnym
#define STRP_NAWI_POLN_TD		59	//strojenie członu różniczkującego w regulatorze nawigacji w kierunku północnym
#define STRP_PRED_POLN_KP		60	//strojenie wzmocnienia w regulatorze prędkości w kierunku północnym
#define STRP_PRED_POLN_TI	 	61	//strojenie członu całkujacego w regulatorze prędkości w kierunku północnym
#define STRP_PRED_POLN_TD		62	//strojenie członu różniczkującego w regulatorze prędkości w kierunku północnym
#define STRP_NAWI_WSCH_KP		63	//strojenie wzmocnienia w regulatorze nawigacji w kierunku wschodnim
#define STRP_NAWI_WSCH_TI		64	//strojenie członu całkujacego w regulatorze nawigacji w kierunku wschodnim
#define STRP_NAWI_WSCH_TD		65	//strojenie członu różniczkującego w regulatorze nawigacji w kierunku wschodnim
#define STRP_PRED_WSCH_KP		66	//strojenie wzmocnienia w regulatorze prędkości w kierunku wschodnim
#define STRP_PRED_WSCH_TI		67	//strojenie członu całkujacego w regulatorze prędkości w kierunku wschodnim
#define STRP_PRED_WSCH_TD		68	//strojenie członu różniczkującego w regulatorze prędkości w kierunku wschodnim
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
