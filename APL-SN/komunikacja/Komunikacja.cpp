#include "pch.h"
#include "Komunikacja.h"
#include "../Errors.h"
#include "KomunikatySieci.h"
#include "Protokol.h"
#include "../APL-SNDoc.h"
#include "../konfig_fram.h"
#include "../pid_kanaly.h"


/*
Klasa komunikacyjna poœrednicz¹ca miêdzy aplikacj¹ a protoko³em komunikacyjnym. 
Aplikacja przesy³a polecenia wymiany danych a klasa nawi¹zuje po³¹czenie po znanym sobie interfejsie
pakuje dane w ramki i przesy³a przy u¿yciu podrzêdnej klasy CProtokol

*/

BOOL CKomunikacja::m_bKoniecWatkuDekoderaPolecen = FALSE;
uint8_t CKomunikacja::m_chTypPolaczenia = ETHS;
HANDLE CKomunikacja::m_hZdarzeniePaczkaDanych = NULL;
HANDLE CKomunikacja::m_hZdarzenieZmianaPolaczeniaWrona = NULL;
int	CKomunikacja::m_LicznikInstancji = 0;



///////////////////////////////////////////////////////////////////////////////////////////////////
// Konstruktor
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
CKomunikacja::CKomunikacja()
	: m_bPolaczonoUart(FALSE)
	, m_bPolaczonoEth(FALSE)
	, m_iNumerPortuETH(0)
	, m_strAdresIP("")
	, m_iNumerPortuUART(0)
	, m_iPredkoscUART(115200)
	, m_chAdresAutopilota(0)
{
	pWskWatkuDekodujacego = AfxBeginThread((AFX_THREADPROC)WatekDekodujRamkiPolecen, (LPVOID)m_pWnd, THREAD_PRIORITY_ABOVE_NORMAL, 0, 0, NULL);
	m_hZdarzeniePaczkaDanych = CreateEvent(NULL, false, false, _T("PaczkaDanych")); // auto-reset event, non-signalled state	
	m_hZdarzenieZmianaPolaczeniaWrona = CreateEvent(NULL, false, false, _T("ZmianaPo³¹czenia")); // auto-reset event, non-signalled state	;
	m_wskaznikInstancji = m_LicznikInstancji++;

	//D³ugoœæ nazwy nie powinna przekraczaæ sta³ej DLUGOSC_NAZWY
	m_strNazwyZmiennychTele[TELEID_AKCEL1X]		= "Akcelerometr1 X [m/s^2]";
	m_strNazwyZmiennychTele[TELEID_AKCEL1Y]		= "Akcelerometr1 Y [m/s^2]";
	m_strNazwyZmiennychTele[TELEID_AKCEL1Z]		= "Akcelerometr1 Z [m/s^2]";
	m_strNazwyZmiennychTele[TELEID_AKCEL2X]		= "Akcelerometr2 X [m/s^2]";
	m_strNazwyZmiennychTele[TELEID_AKCEL2Y]		= "Akcelerometr2 Y [m/s^2]";
	m_strNazwyZmiennychTele[TELEID_AKCEL2Z]		= "Akcelerometr2 Z [m/s^2]";
	m_strNazwyZmiennychTele[TELEID_ZYRO1P]		= "¯yroskop1 P [rad/s]";
	m_strNazwyZmiennychTele[TELEID_ZYRO1Q]		= "¯yroskop1 Q [rad/s]";
	m_strNazwyZmiennychTele[TELEID_ZYRO1R]		= "¯yroskop1 R [rad/s]";
	m_strNazwyZmiennychTele[TELEID_ZYRO2P]		= "¯yroskop2 P [rad/s]";
	m_strNazwyZmiennychTele[TELEID_ZYRO2Q]		= "¯yroskop2 Q [rad/s]";
	m_strNazwyZmiennychTele[TELEID_ZYRO2R]		= "¯yroskop2 R [rad/s]";
	m_strNazwyZmiennychTele[TELEID_MAGNE1X]		= "Magnetometr 1 X [uT]";
	m_strNazwyZmiennychTele[TELEID_MAGNE1Y]		= "Magnetometr 1 Y [uT]";
	m_strNazwyZmiennychTele[TELEID_MAGNE1Z]		= "Magnetometr 1 Z [uT]";
	m_strNazwyZmiennychTele[TELEID_MAGNE2X]		= "Magnetometr 2 X [uT]";
	m_strNazwyZmiennychTele[TELEID_MAGNE2Y]		= "Magnetometr 2 Y [uT]";
	m_strNazwyZmiennychTele[TELEID_MAGNE2Z]		= "Magnetometr 2 Z [uT]";
	m_strNazwyZmiennychTele[TELEID_MAGNE3X]		= "Magnetometr 3 X [uT]";
	m_strNazwyZmiennychTele[TELEID_MAGNE3Y]		= "Magnetometr 3 Y [uT]";
	m_strNazwyZmiennychTele[TELEID_MAGNE3Z]		= "Magnetometr 3 Z [uT]";
	m_strNazwyZmiennychTele[TELEID_TEMPIMU1]	= "Temperatura IMU1 [K]";
	m_strNazwyZmiennychTele[TELEID_TEMPIMU2]	= "Temperatura IMU2 [K]";

	//zmienna AHRS
	m_strNazwyZmiennychTele[TELEID_KAT_IMU1X]	= "Trygono. Phi [rad]";
	m_strNazwyZmiennychTele[TELEID_KAT_IMU1Y]	= "Trygono. Theta [rad]";
	m_strNazwyZmiennychTele[TELEID_KAT_IMU1Z]	= "Trygono. Psi [rad]";
	m_strNazwyZmiennychTele[TELEID_KAT_IMU2X]	= "Kwatern. Phi [rad]";
	m_strNazwyZmiennychTele[TELEID_KAT_IMU2Y]	= "Kwatern. Theta [rad]";
	m_strNazwyZmiennychTele[TELEID_KAT_IMU2Z]	= "Kwatern. Psi [rad]";
	m_strNazwyZmiennychTele[TELEID_KAT_ZYRO1X]	= "¯yro. Phi [rad]";
	m_strNazwyZmiennychTele[TELEID_KAT_ZYRO1Y]	= "¯yro. Theta [rad]";
	m_strNazwyZmiennychTele[TELEID_KAT_ZYRO1Z]	= "¯yro. Psi [rad]";
	m_strNazwyZmiennychTele[TELEID_KAT_AKCELX]	= "Akcel. Phi [rad]";
	m_strNazwyZmiennychTele[TELEID_KAT_AKCELY]	= "Akcel. Theta [rad]";
	m_strNazwyZmiennychTele[TELEID_KAT_AKCELZ]	= "Akcel. Psi [rad]";

	//zmienne barametryczne
	m_strNazwyZmiennychTele[TELEID_CISBEZW1]	= "Ciœn.Bzwzgl.1 [Pa]";
	m_strNazwyZmiennychTele[TELEID_CISBEZW2]	= "Ciœn.Bzwzgl.2 [Pa]";
	m_strNazwyZmiennychTele[TELEID_WYSOKOSC1]	= "Wysokoœæ AGL1 [m]";
	m_strNazwyZmiennychTele[TELEID_WYSOKOSC2]	= "Wysokoœæ AGL2 [m]";
	m_strNazwyZmiennychTele[TELEID_CISROZN1]	= "Ciœn.Ró¿n.1 [Pa]";
	m_strNazwyZmiennychTele[TELEID_CISROZN2]	= "Ciœn.Ró¿n.2 [Pa]";
	m_strNazwyZmiennychTele[TELEID_PREDIAS1]	= "Prêdkoœæ IAS1 [m/s]";
	m_strNazwyZmiennychTele[TELEID_PREDIAS2]	= "Prêdkoœæ IAS2 [m/s]";
	m_strNazwyZmiennychTele[TELEID_TEMPCISB1]	= "Temp.Ciœn.Bzwz1 [K]";
	m_strNazwyZmiennychTele[TELEID_TEMPCISB2]	= "Temp.Ciœn.Bzwz2 [K]";
	m_strNazwyZmiennychTele[TELEID_TEMPCISR1]	= "Temp.Ciœn.Ró¿n1 [K]";
	m_strNazwyZmiennychTele[TELEID_TEMPCISR2]	= "Temp.Ciœn.Ró¿n2 [K]";

	//odbiorniki RC i serwa
	m_strNazwyZmiennychTele[TELEID_RC_KAN1]		= "Odb RC kan.1 [us]";
	m_strNazwyZmiennychTele[TELEID_RC_KAN2]		= "Odb RC kan.2 [us]";
	m_strNazwyZmiennychTele[TELEID_RC_KAN3]		= "Odb RC kan.3 [us]";
	m_strNazwyZmiennychTele[TELEID_RC_KAN4]		= "Odb RC kan.4 [us]";
	m_strNazwyZmiennychTele[TELEID_RC_KAN5]		= "Odb RC kan.5 [us]";
	m_strNazwyZmiennychTele[TELEID_RC_KAN6]		= "Odb RC kan.6 [us]";
	m_strNazwyZmiennychTele[TELEID_RC_KAN7]		= "Odb RC kan.7 [us]";
	m_strNazwyZmiennychTele[TELEID_RC_KAN8]		= "Odb RC kan.8 [us]";
	m_strNazwyZmiennychTele[TELEID_RC_KAN9]		= "Odb RC kan.9 [us]";
	m_strNazwyZmiennychTele[TELEID_RC_KAN10]	= "Odb RC kan.10 [us]";
	m_strNazwyZmiennychTele[TELEID_RC_KAN11]	= "Odb RC kan.11 [us]";
	m_strNazwyZmiennychTele[TELEID_RC_KAN12]	= "Odb RC kan.12 [us]";
	m_strNazwyZmiennychTele[TELEID_RC_KAN13]	= "Odb RC kan.13 [us]";
	m_strNazwyZmiennychTele[TELEID_RC_KAN14]	= "Odb RC kan.14 [us]";
	m_strNazwyZmiennychTele[TELEID_RC_KAN15]	= "Odb RC kan.15 [us]";
	m_strNazwyZmiennychTele[TELEID_RC_KAN16]	= "Odb RC kan.16 [us]";

	m_strNazwyZmiennychTele[TELEID_SERWO1]		= "Serwo kan.1 [us]";
	m_strNazwyZmiennychTele[TELEID_SERWO2]		= "Serwo kan.2 [us]";
	m_strNazwyZmiennychTele[TELEID_SERWO3]		= "Serwo kan.3 [us]";
	m_strNazwyZmiennychTele[TELEID_SERWO4]		= "Serwo kan.4 [us]";
	m_strNazwyZmiennychTele[TELEID_SERWO5]		= "Serwo kan.5 [us]";
	m_strNazwyZmiennychTele[TELEID_SERWO6]		= "Serwo kan.6 [us]";
	m_strNazwyZmiennychTele[TELEID_SERWO7]		= "Serwo kan.7 [us]";
	m_strNazwyZmiennychTele[TELEID_SERWO8]		= "Serwo kan.8 [us]";
	m_strNazwyZmiennychTele[TELEID_SERWO9]		= "Serwo kan.9 [us]";
	m_strNazwyZmiennychTele[TELEID_SERWO10]		= "Serwo kan.10 [us]";
	m_strNazwyZmiennychTele[TELEID_SERWO11]		= "Serwo kan.11 [us]";
	m_strNazwyZmiennychTele[TELEID_SERWO12]		= "Serwo kan.12 [us]";
	m_strNazwyZmiennychTele[TELEID_SERWO13]		= "Serwo kan.13 [us]";
	m_strNazwyZmiennychTele[TELEID_SERWO14]		= "Serwo kan.14 [us]";
	m_strNazwyZmiennychTele[TELEID_SERWO15]		= "Serwo kan.15 [us]";
	m_strNazwyZmiennychTele[TELEID_SERWO16]		= "Serwo kan.16 [us]";

	m_strNazwyZmiennychTele[TELEID_DOTYK_ADC0] = "Ekran dotyk. ADC[0]";
	m_strNazwyZmiennychTele[TELEID_DOTYK_ADC1] = "Ekran dotyk. ADC[1]";
	m_strNazwyZmiennychTele[TELEID_DOTYK_ADC2] = "Ekran dotyk. ADC[2]";
	m_strNazwyZmiennychTele[TELEID_CZAS_PETLI] = "Czas pêtli g³ównej [us]";


	m_strNazwyZmiennychTele[TELEID_PID_PRZE_WZAD]		= "Wart.zadana przech";	//wartoœæ zadana regulatora sterowania przechyleniem
	m_strNazwyZmiennychTele[TELEID_PID_PRZE_WYJ]		= "Wy PID przechyl";	//wyjœcie regulatora sterowania przechyleniem
	m_strNazwyZmiennychTele[TELEID_PID_PRZE_WY_P]		= "Wy P przechylenia";	//wyjœcie cz³onu P
	m_strNazwyZmiennychTele[TELEID_PID_PRZE_WY_I]		= "Wy I przechylenia";	//wyjœcie cz³onu I
	m_strNazwyZmiennychTele[TELEID_PID_PRZE_WY_D]		= "Wy D przechylenia";	//wyjœcie cz³onu D
	m_strNazwyZmiennychTele[TELEID_PID_PK_PRZE_WYJ]		= "Wy PID pr.k¹t.prze";	//wyjœcie regulatora sterowania prêdkoœci¹ k¹tow¹ przechylenia
	m_strNazwyZmiennychTele[TELEID_PID_PK_PRZE_WY_P]	= "Wy P pr.k¹t.prze";	//wyjœcie cz³onu P
	m_strNazwyZmiennychTele[TELEID_PID_PK_PRZE_WY_I]	= "Wy I pr.k¹t.prze";	//wyjœcie cz³onu I
	m_strNazwyZmiennychTele[TELEID_PID_PK_PRZE_WY_D]	= "Wy D pr.k¹t.prze";	//wyjœcie cz³onu D

	m_strNazwyZmiennychTele[TELEID_PID_POCH_WZAD]		= "Wart.zadana poch";	//wartoœæ zadana regulatora sterowania pochyleniem
	m_strNazwyZmiennychTele[TELEID_PID_POCH_WYJ]		= "Wy PID pochylenia";	//wyjœcie regulatora sterowania pochyleniem
	m_strNazwyZmiennychTele[TELEID_PID_POCH_WY_P]		= "Wy P pochylenia";	//wyjœcie cz³onu P
	m_strNazwyZmiennychTele[TELEID_PID_POCH_WY_I]		= "Wy I pochylenia";	//wyjœcie cz³onu I
	m_strNazwyZmiennychTele[TELEID_PID_POCH_WY_D]		= "Wy D pochylenia";	//wyjœcie cz³onu D
	m_strNazwyZmiennychTele[TELEID_PID_PK_POCH_WYJ]		= "Wy PID pr.k¹t.poch";	//wyjœcie regulatora sterowania prêdkoœci¹ k¹tow¹ pochylenia
	m_strNazwyZmiennychTele[TELEID_PID_PK_POCH_WY_P]	= "Wy P pr.k¹t.poch";	//wyjœcie cz³onu P
	m_strNazwyZmiennychTele[TELEID_PID_PK_POCH_WY_I]	= "Wy I pr.k¹t.poch";	//wyjœcie cz³onu I
	m_strNazwyZmiennychTele[TELEID_PID_PK_POCH_WY_D]	= "Wy D pr.k¹t.poch";	//wyjœcie cz³onu D

	m_strNazwyZmiennychTele[TELEID_PID_ODCH_WZAD]		= "Wart.zadana odch";	//wartoœæ zadana regulatora sterowania odchyleniem
	m_strNazwyZmiennychTele[TELEID_PID_ODCH_WYJ]		= "Wy PID odchylenia";	//wyjœcie regulatora sterowania odchyleniem
	m_strNazwyZmiennychTele[TELEID_PID_ODCH_WY_P]		= "Wy P odchylenia";	//wyjœcie cz³onu P
	m_strNazwyZmiennychTele[TELEID_PID_ODCH_WY_I]		= "Wy I odchylenia";	//wyjœcie cz³onu I
	m_strNazwyZmiennychTele[TELEID_PID_ODCH_WY_D]		= "Wy D odchylenia";	//wyjœcie cz³onu D
	m_strNazwyZmiennychTele[TELEID_PID_PK_ODCH_WYJ]		= "Wy PID pr.k¹t.odch";	//wyjœcie regulatora sterowania prêdkoœci¹ k¹tow¹ odchylenia
	m_strNazwyZmiennychTele[TELEID_PID_PK_ODCH_WY_P]	= "Wy P pr.k¹t.odch";	//wyjœcie cz³onu P
	m_strNazwyZmiennychTele[TELEID_PID_PK_ODCH_WY_I]	= "Wy I pr.k¹t.odch";	//wyjœcie cz³onu I
	m_strNazwyZmiennychTele[TELEID_PID_PK_ODCH_WY_D]	= "Wy D pr.k¹t.odch";	//wyjœcie cz³onu D

	m_strNazwyZmiennychTele[TELEID_PID_WYSO_WZAD]		= "Wart.zadana wysoko";	//wartoœæ zadana regulatora sterowania wysokoœci¹
	m_strNazwyZmiennychTele[TELEID_PID_WYSO_WYJ]		= "Wy PID wysokoœci";	//wyjœcie regulatora sterowania wysokoœci¹
	m_strNazwyZmiennychTele[TELEID_PID_WYSO_WY_P]		= "Wy P wysokoœci";	//wyjœcie cz³onu P
	m_strNazwyZmiennychTele[TELEID_PID_WYSO_WY_I]		= "Wy I wysokoœci";	//wyjœcie cz³onu I
	m_strNazwyZmiennychTele[TELEID_PID_WYSO_WY_D]		= "Wy D wysokoœci";	//wyjœcie cz³onu D
	m_strNazwyZmiennychTele[TELEID_PID_PR_WYSO_WYJ]		= "Wy PID pr.zm.wysoko";	//wyjœcie regulatora sterowania prêdkoœci¹ zmiany wysokoœci
	m_strNazwyZmiennychTele[TELEID_PID_PR_WYSO_WY_P]	= "Wy P pr.zm.wysoko";	//wyjœcie cz³onu P
	m_strNazwyZmiennychTele[TELEID_PID_PR_WYSO_WY_I]	= "Wy I pr.zm.wysoko";	//wyjœcie cz³onu I
	m_strNazwyZmiennychTele[TELEID_PID_PR_WYSO_WY_D]	= "Wy D pr.zm.wysoko";	//wyjœcie cz³onu D

	m_strNazwyZmiennychTele[TELEID_PID_NAWN_WZAD]		= "Wart.zad.naw. N";	//wartoœæ zadana regulatora sterowania nawigacj¹ w kierunku pó³nocnym
	m_strNazwyZmiennychTele[TELEID_PID_NAWN_WYJ]		= "Wy PID nawig. N";	//wyjœcie regulatora sterowania nawigacj¹ w kierunku pó³nocnym
	m_strNazwyZmiennychTele[TELEID_PID_NAWN_WY_P]		= "Wy P nawigacji N";	//wyjœcie cz³onu P
	m_strNazwyZmiennychTele[TELEID_PID_NAWN_WY_I]		= "Wy I nawigacji N";	//wyjœcie cz³onu I
	m_strNazwyZmiennychTele[TELEID_PID_NAWN_WY_D]		= "Wy D nawigacji N";	//wyjœcie cz³onu D
	m_strNazwyZmiennychTele[TELEID_PID_PR_NAWN_WYJ]		= "Wy PID pr.nawig.N";	//wyjœcie regulatora sterowania prêdkoœci¹ w kierunku pó³nocnym
	m_strNazwyZmiennychTele[TELEID_PID_PR_NAWN_WY_P]	= "Wy P prêd.nawig.N";	//wyjœcie cz³onu P
	m_strNazwyZmiennychTele[TELEID_PID_PR_NAWN_WY_I]	= "Wy I prêd.nawig.N";	//wyjœcie cz³onu I
	m_strNazwyZmiennychTele[TELEID_PID_PR_NAWN_WY_D]	= "Wy D prêd.nawig.N";	//wyjœcie cz³onu D

	m_strNazwyZmiennychTele[TELEID_PID_NAWE_WZAD]		= "Wart.zad.naw. E";	//wartoœæ zadana regulatora sterowania nawigacj¹ w kierunku wschodnim
	m_strNazwyZmiennychTele[TELEID_PID_NAWE_WYJ]		= "Wy PID nawig. E";	//wyjœcie regulatora sterowania nawigacj¹ w kierunku wschodnim
	m_strNazwyZmiennychTele[TELEID_PID_NAWE_WY_P]		= "Wy P nawigacji E";	//wyjœcie cz³onu P
	m_strNazwyZmiennychTele[TELEID_PID_NAWE_WY_I]		= "Wy I nawigacji E";	//wyjœcie cz³onu I
	m_strNazwyZmiennychTele[TELEID_PID_NAWE_WY_D]		= "Wy D nawigacji E";	//wyjœcie cz³onu D
	m_strNazwyZmiennychTele[TELEID_PID_PR_NAWE_WYJ]		= "Wy PID pr.nawig.E";	//wyjœcie regulatora sterowania prêdkoœci¹ w kierunku wschodnim
	m_strNazwyZmiennychTele[TELEID_PID_PR_NAWE_WY_P]	= "Wy P prêd.nawig.E";	//wyjœcie cz³onu P
	m_strNazwyZmiennychTele[TELEID_PID_PR_NAWE_WY_I]	= "Wy I prêd.nawig.E";	//wyjœcie cz³onu I
	m_strNazwyZmiennychTele[TELEID_PID_PR_NAWE_WY_D]	= "Wy D prêd.nawig.E";	//wyjœcie cz³onu D
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Destruktor
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
CKomunikacja::~CKomunikacja()
{	
	m_bKoniecWatkuDekoderaPolecen = TRUE;
	WaitForSingleObject(pWskWatkuDekodujacego, INFINITE);
	if (m_hZdarzeniePaczkaDanych)
	{
		CloseHandle(m_hZdarzeniePaczkaDanych);
		m_hZdarzeniePaczkaDanych = NULL;
	}

	if (m_hZdarzenieZmianaPolaczeniaWrona)
	{
		CloseHandle(m_hZdarzenieZmianaPolaczeniaWrona);
		m_hZdarzenieZmianaPolaczeniaWrona = NULL;
	}
}


void CKomunikacja::UstawRodzica(CView* pWnd)
{
	m_pWnd = pWnd;
}


/*void CKomunikacja::UstawAdresIP(CString strAdres)
{
	m_strAdresIP = strAdres;
}*/


///////////////////////////////////////////////////////////////////////////////////////////////////
// Nawi¹zuje po³¹czenia ze znanymi sobie portami komunikacyjnymi
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::Polacz(CView* pWnd)
{
	uint8_t chErr = ERR_NOT_CONNECTED;
	Wron cWron;
	uint8_t chNazwa[DLUGOSC_NAZWY];
	int nIndeks = -1;

	switch (m_chTypPolaczenia)
	{
	case UART:	
		chErr = getProtokol().PolaczPort(UART, m_iNumerPortuUART, m_iPredkoscUART, 0, m_pWnd);		
		if (chErr == ERR_OK)
		{
			m_bPolaczonoUart = TRUE;
			//wyœlij na adres rozg³oszeniowy polecenie pobrania adresów i nazwy BSP
			chErr = PobierzBSP(&cWron.m_chAdres, chNazwa, &cWron.m_chAdresIP[0]);
			if (chErr == ERR_OK)
			{
				//sprawdŸ czy w roju jest ju¿ ten wron					
				for (int n = 0; n < m_cRoj.vWron.size(); n++)
				{
					if (m_cRoj.vWron[n].m_chAdres == cWron.m_chAdres)
					{
						nIndeks = n;
						break;
					}
				}

				//je¿eli nie ma go w roju to wstaw
				if (nIndeks < 0)
				{
					m_cRoj.vWron.push_back(cWron);
					nIndeks = (int)m_cRoj.vWron.size() - 1;
					m_cRoj.m_nIndeksWrona = nIndeks;
					m_cRoj.vWron[nIndeks].UstawNazwe(chNazwa);
					m_cRoj.vWron[nIndeks].m_chPolaczony = UART;
					UstawAdresWrona(cWron.m_chAdres);	//ustaw adres na potrzeby klasy komunikacji
				}
				chErr = CzytajOkresTelemetrii(m_cRoj.vWron[nIndeks].m_sOkresTelemetrii, LICZBA_ZMIENNYCH_TELEMETRYCZNYCH);
				SetEvent(m_hZdarzenieZmianaPolaczeniaWrona);		//wygeneruj komunikat o zmianie po³¹czenia			
			}				
		}
		else
		{
			m_bPolaczonoUart = FALSE;
			if (chErr == ERR_CANT_CONNECT)
				MessageBoxExW(pWnd->m_hWnd, _T("Nie mogê po³¹czyæ siê z portem. \nSprawdŸ czy nie jest ju¿ po³¹czony."), _T("Ojojojoj!"), MB_ICONWARNING, 0);
		}
		break;

	case ETHS:	
		chErr = getProtokol().PolaczPort(ETHS, m_iNumerPortuETH, 0, m_strAdresIP, m_pWnd);
		m_bPolaczonoEth = (chErr == ERR_OK);
		break;

	case ETHK:
		m_bPolaczonoEth = FALSE;	//po po³aczeniu zmienna CProtokol::m_bPolaczonoEth bêdzie ustawiona w w¹tku CProtokol::WatekSluchajPortuEth() i bêdzie ustawiona w trakcie odpytania CAPLSNView::OnUpdatePolaczEth
		chErr = getProtokol().PolaczPort(ETHK, m_iNumerPortuETH, 0, m_strAdresIP, m_pWnd);
		break;
	
	default:	break;
	}
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Zamyka po³aczenie z portem komunikacyjnym
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::Rozlacz()
{
	uint8_t chErr = ERR_OK;
	
	if (m_bPolaczonoEth)
	{
		//chErr = getProtokol().ZamknijPort(ETHK);
		chErr = getProtokol().ZamknijPort(ETHS);
		m_bPolaczonoEth = FALSE;
	}

	if (m_bPolaczonoUart)
	{
		chErr = getProtokol().ZamknijPort(UART);
		m_bPolaczonoUart = FALSE;
	}
	return chErr;
}



void CKomunikacja::AkceptujPolaczenieETH()
{
	getProtokol().AkceptujPolaczenieETH();
}

//
BOOL CKomunikacja::CzyPolaczonoEth()
{ 
	m_bPolaczonoEth = getProtokol().CzyPolaczonoEth();
	return m_bPolaczonoEth; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Odbiór wektorów z danymi ramek i ich parsowanie
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
void CKomunikacja::OdebranoDaneETH()
{

	/*
	* Dodaæ w¹tek sprawdzaj¹cy czy przychodza ramki odebrane w  CProtokol przez dowolne ³¹cze
	* Dodaæ podzia³ na telemetriê i ramki z odpowiedzi¹ - osobne w¹tki?
	*/

	getProtokol().OdbierzDaneETH();
	

	//int iRozmiar = m_cProto.m_inputAnswerData.size();
	//m_cProto.m_inputAnswerData.pop_back(BinaryFrame(m_iLecznikWejRamekTelemetrii++, m_chPolecenie, m_chZnakCzasu, m_chDaneWy, m_chIloscDanychRamki));


}




void CKomunikacja::WyslanoDaneETH()
{
	getProtokol().WyslanoDaneETH();
}




///////////////////////////////////////////////////////////////////////////////////////////////////
// Pobiera ID, nazwê oraz numer IP z urz¹dzenia przy pomocuy adresu rozg³oszeniowego, po to aby móc prowadziæ dalsz¹ komunikacjê pinkt do punktu
// *chId - wskaŸnik na zwracane ID
// *strNazwa - wskaŸnik na nazwê urz¹dzenia
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::PobierzBSP(uint8_t* chId, uint8_t* chNazwa, uint8_t* chIP)
{
	uint8_t chErr;
	uint8_t chOdebrano;
	uint8_t chDanePrzych[32];

	chErr = getProtokol().WyslijOdbierzRamke(ADRES_BROADCAST, ADRES_STACJI, PK_POBIERZ_BSP, NULL, 0, chDanePrzych, &chOdebrano);
	if (chErr == ERR_OK)
	{
		*chId = chDanePrzych[0];
		for (int n = 0; n < DLUGOSC_NAZWY; n++)
			*(chNazwa + n) = chDanePrzych[n+1];
		for (int n = 0; n < 4; n++)
			*(chIP + n) = chDanePrzych[n + 21];
	}
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Ustawia ID, nazwê oraz numer IP dla BSP
// chId - ID, czyli adres komunikacyjny po UART
// *strNazwa - wskaŸnik na nazwê urz¹dzenia
// *chIP - wskaŸnika na adres IP
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::UstawBSP(uint8_t chId, uint8_t* chNazwa, uint8_t* chIP)
{
	uint8_t chDaneWych[25];
	uint8_t chOdebrano;
	uint8_t chDanePrzych[ROZM_DANYCH_UART];

	chDaneWych[0] = chId;
	for (int n = 0; n < DLUGOSC_NAZWY; n++)
		chDaneWych[n + 1] = *(chNazwa + n);
	for (int n = 0; n < 4; n++)
		chDaneWych[n + 21] = *(chIP + n);

	return getProtokol().WyslijOdbierzRamke(ADRES_BROADCAST, ADRES_STACJI, PK_USTAW_BSP, chDaneWych, 25, chDanePrzych, &chOdebrano);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wywo³uje w¹tek analizuj¹cy ramki poleceñ
// Zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::WatekDekodujRamkiPolecen(LPVOID pParam)
{
	m_bKoniecWatkuDekoderaPolecen = FALSE;
	return reinterpret_cast<CKomunikacja*>(pParam)->WlasciwyWatekDekodujRamkiPolecen();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// W¹tek analizuj¹cy ramki poleceñ
// Zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::WlasciwyWatekDekodujRamkiPolecen ()
{
	//_Ramka s_Ramka;
	//int n, m;
	//BOOL bMamyTo;
	//_sWron sWron;
	//CString strNazwa = L" ";
	//std::vector <_sWron>::iterator iter;	//iterator wektora wronów


	while (!m_bKoniecWatkuDekoderaPolecen)
	{		
		WaitForSingleObject(getProtokol().m_hZdarzenieRamkaPolecenGotowa, INFINITE);		//czekaj na odbiór zdekodowanej ramki danych

		/*for (n = 0; n < getProtokol().m_vRamkaPolecenia.size(); n++)
		{
			s_Ramka = getProtokol().m_vRamkaPolecenia[n];
			switch (s_Ramka.chPolecenie)
			{
			case PK_OK:	//przysz³a ramka OK
			case PK_BLAD:	//przysz³a ramka z kodem b³êdu
				//m_stPotwierdzenie.chTyp = s_Ramka.chPolecenie;
				//m_stPotwierdzenie.chKodBledu = s_Ramka.dane[0];
				//m_stPotwierdzenie.chParametr = s_Ramka.dane[1];
				break;

			case PK_POBIERZ_ID: //przysz³a ramka z identyfikatorem nowego BSP
				//m_strNazwa = L"";
				for (m = 0; m < s_Ramka.chRozmiar; m++)
					strNazwa.Insert(m, s_Ramka.dane[m]);

				//sprawdŸ czy w roju mamy ju¿ tego wrona
				bMamyTo = FALSE;
				/*if (m_vRoj.size())
				{
					for (iter = m_vRoj.begin(); iter < m_vRoj.end(); iter++)
					{
						if (iter->chAdres == s_Ramka.chAdrNadawcy)
						{
							if (iter->strNazwa != strNazwa)
								iter->strNazwa = strNazwa;
							bMamyTo = TRUE;
							break;
						}
					}
				}*/
				//je¿eli nie mamy to go dodaj
				/*if (bMamyTo == FALSE)
				{
					sWron.chAdres = s_Ramka.chAdrNadawcy;
					sWron.strNazwa = strNazwa;
					m_vRoj.push_back(sWron);
				}*/	
			/*	strNazwa = L"";
				m_chAdresAutopilota = s_Ramka.chAdrNadawcy;
				CKomunikacja::WyslijOK();		//wyœlij odpowiedŸ				
				break;

			case PK_TELEMETRIA:
				
				break;

			default:	break;
			}
		}
		getProtokol().m_vRamkaPolecenia.clear();	//wyczyœæ wektor*/
	}
	return ERR_OK;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wysy³a ramkê OK 
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::WyslijOK()
{
	uint8_t chRamka[ROZM_CIALA_RAMKI];

	getProtokol().PrzygotujRamke(m_chAdresAutopilota, ADRES_STACJI, PK_OK, NULL, 0, chRamka);
	return getProtokol().WyslijRamke(m_chTypPolaczenia, chRamka, ROZM_CIALA_RAMKI);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Wysy³a polecenie zrobienia zdjêcia
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::ZrobZdjecie(uint16_t* sBuforZdjecia)
{
	uint8_t chDane[5];
	uint8_t x, chErr, chOdebrano;
	uint8_t chLicznikBledow = 0;
	uint32_t nPobranoBajtow, nRozmiarDanych;

	chErr = getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_ZROB_ZDJECIE, chDane, 0, chDane, &chOdebrano);
	if (chErr == ERR_OK)
	{
		SetEvent(m_hZdarzeniePaczkaDanych);
		//sprawdŸ status wykonania zdjêcia
		chDane[0] = SGZ_CZEKA;
		while (chDane[0] == SGZ_CZEKA)
		{
			chErr = getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_POB_STAT_ZDJECIA, NULL, 0, chDane, &chOdebrano);
			if (chErr == ERR_OK)
			{
				if (chDane[0] == SGZ_BLAD)
					return ERR_BLAD_KAMERY;
				chLicznikBledow = 0;
			}
			else
			{
				chLicznikBledow++;
				if (chLicznikBledow > LICZBA_PROB_ZANIM_ZGLOSI_BLAD)
					return ERR_TIMEOUT;
			}
		}

		//pobierz zdjêcie
		nPobranoBajtow = 0;
		nRozmiarDanych = (uint32_t)(480 * 320 * 2);	//rozmiar obrazu w bajtach
		while (nPobranoBajtow < nRozmiarDanych)
		{
			//m_unia8_32.dane32 = nPobranoBajtow /4;	//offset pobieranych danych zdjêcia, /4 - konwersja z bajtów s³owa 32/bit 
			m_unia8_32.dane32 = nPobranoBajtow / 2;	//offset pobieranych danych zdjêcia, /2 - konwersja z bajtów  16 bitowe piksele
			for (x = 0; x < 4; x++)
				chDane[x] = m_unia8_32.dane8[x];

			//rozmiar paczki danych do pobrania
			if ((nRozmiarDanych - nPobranoBajtow) > ROZM_DANYCH_UART)
				chDane[4] = ROZM_DANYCH_UART;
			else
				chDane[4] = nRozmiarDanych - nPobranoBajtow;

			chErr = getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_POBIERZ_ZDJECIE, chDane, 5, (uint8_t*)(sBuforZdjecia + nPobranoBajtow /2), &chOdebrano);
			if (chErr == ERR_OK)
			{
				nPobranoBajtow += chOdebrano;
				SetEvent(m_hZdarzeniePaczkaDanych);
				chLicznikBledow = 0;
			}
			else
			{
				chLicznikBledow++;
				if (chLicznikBledow > LICZBA_PROB_ZANIM_ZGLOSI_BLAD)
					return ERR_TIMEOUT;;
			}
		}
	}	
	return chErr;
}
	


///////////////////////////////////////////////////////////////////////////////////////////////////
// Uruchaie polecenie inicjalizacji kamery resetujac j¹ i przywracaj¹c nastawy domyœlne
// parametry: brak
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::ResetujKamere()
{
	uint8_t chDane[10];
	uint8_t chOdebrano;
	uint8_t chErr;

	chErr = getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_RESETUJ_KAMERE, NULL, 0, chDane, &chOdebrano);
	if (!chErr)
	{
		if (chDane[0] == SGZ_BLAD)
			chErr = ERR_BLAD_KAMERY;
	}
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wysy³a polecenie odczytu konfiguracji kamery
// parametry:
// [i] chAdres - adres autopilota
// [o] *chSzerWy i *chWysWy - wskaŸniki na szerokoœæ i wysokoœæ wyjœciowa kamery skalowana przez 16
// [o] *chSzerWe i *chWysWe - wskaŸniki na szerokoœæ i wysokoœæ z jakiej zbiera i skaluje kamera, skalowana przez 16
// [o] *chTrybDiagn - wskaŸnik na kod trybu diagnostycznego
// [o] *chFlagi - zbiór bitów definiuj¹cych funkcjonalnoœæ: bit 0: odwróæ w poziomie, bit 1: odwróæ w pionie, bit 2: 1=zdjecie, 0=film
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::PobierzKamere(st_KonfKam *stKonfig)// uint8_t *chSzerWy, uint8_t *chWysWy, uint8_t *chSzerWe, uint8_t *chWysWe, uint8_t *chTrybDiagn, uint8_t *chFlagi)
{
	uint8_t chDane[30];
	uint8_t chErr, chOdebrano;

	chErr = getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_POB_PAR_KAMERY, NULL, 0, chDane, &chOdebrano);
	if (chErr == ERR_OK)
	{
		stKonfig->chSzerWy = chDane[0];
		stKonfig->chWysWy = chDane[1];
		stKonfig->chSzerWe = chDane[2];
		stKonfig->chWysWe = chDane[3];
		stKonfig->chPrzesWyPoz = chDane[4];
		stKonfig->chPrzesWyPio = chDane[5];
		stKonfig->chObracanieObrazu = chDane[6];
		stKonfig->chFormatObrazu = chDane[7];
		stKonfig->sWzmocnienieR = ((uint16_t)chDane[8] << 8) + chDane[9];
		stKonfig->sWzmocnienieG = ((uint16_t)chDane[10] << 8) + chDane[11];
		stKonfig->sWzmocnienieB = ((uint16_t)chDane[12] << 8) + chDane[13];
		stKonfig->chKontrolaBalansuBieli = chDane[14];
		stKonfig->chNasycenie = chDane[15];
		stKonfig->sCzasEkspozycji = ((uint16_t)chDane[16] << 8) + chDane[17]; 
		stKonfig->chKontrolaExpo = chDane[18];
		stKonfig->chTrybyEkspozycji = chDane[19];
		stKonfig->sAGCLong = ((uint16_t)chDane[20] << 8) + chDane[21];
		stKonfig->sAGCVTS = ((uint16_t)chDane[22] << 8) + chDane[23];
		stKonfig->chKontrolaISP0 = chDane[24];		//0x5000
		stKonfig->chKontrolaISP1 = chDane[25];		//0x50001
		stKonfig->chProgUsuwania = chDane[26];		//0x5080 Even CTRL 00 Treshold for even odd  cancelling
		stKonfig->sAGCAdjust = ((uint16_t)chDane[27] << 8) + chDane[28];
		stKonfig->chPoziomEkspozycji = chDane[29];
	}	
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wysy³a polecenie zapisu konfiguracji kamery
// parametry:
// [i] chSzerWy i chWysWy - szerokoœæ i wysokoœæ wyjœciowa kamery skalowana przez 16
// [i] chSzerWe i chWysWe - szerokoœæ i wysokoœæ z jakiej zbiera i skaluje kamera, skalowana przez 16
// [i] chTrybDiagn - kod trybu diagnostycznego
// [i] chFlagi - zbiór bitów definiuj¹cych funkcjonalnoœæ: bit 0: odwróæ w poziomie, bit 1: odwróæ w pionie, bit 2: 1=zdjecie, 0=film
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::UstawKamere(st_KonfKam* stKonfig)
{
	uint8_t chDane[30];
	uint8_t chOdebrano;

	chDane[0] = stKonfig->chSzerWy;
	chDane[1] = stKonfig->chWysWy;
	chDane[2] = stKonfig->chSzerWe;
	chDane[3] = stKonfig->chWysWe;
	chDane[4] = stKonfig->chPrzesWyPoz;
	chDane[5] = stKonfig->chPrzesWyPio;
	chDane[6] = stKonfig->chObracanieObrazu;
	chDane[7] = stKonfig->chFormatObrazu;
	chDane[8] = (uint8_t)(stKonfig->sWzmocnienieR >> 8);
	chDane[9] = (uint8_t)(stKonfig->sWzmocnienieR & 0xFF);
	chDane[10] = (uint8_t)(stKonfig->sWzmocnienieG >> 8);
	chDane[11] = (uint8_t)(stKonfig->sWzmocnienieG & 0xFF);
	chDane[12] = (uint8_t)(stKonfig->sWzmocnienieB >> 8);
	chDane[13] = (uint8_t)(stKonfig->sWzmocnienieB & 0xFF);
	chDane[14] = stKonfig->chKontrolaBalansuBieli;
	chDane[15] = stKonfig->chNasycenie;
	chDane[16] = (uint8_t)(stKonfig->sCzasEkspozycji >> 8);
	chDane[17] = (uint8_t)(stKonfig->sCzasEkspozycji & 0xFF);
	chDane[18] = stKonfig->chKontrolaExpo;
	chDane[19] = stKonfig->chTrybyEkspozycji;
	chDane[20] = (uint8_t)(stKonfig->sAGCLong >> 8);
	chDane[21] = (uint8_t)(stKonfig->sAGCLong & 0xFF);
	chDane[22] = (uint8_t)(stKonfig->sAGCVTS >> 8);
	chDane[23] = (uint8_t)(stKonfig->sAGCVTS & 0xFF);
	chDane[24] = stKonfig->chKontrolaISP0;		//0x5000
	chDane[25] = stKonfig->chKontrolaISP1;		//0x50001
	chDane[26] = stKonfig->chProgUsuwania;		//0x5080 Even CTRL 00 Treshold for even odd  cancelling
	chDane[27] = (uint8_t)(stKonfig->sAGCAdjust >> 8);
	chDane[28] = (uint8_t)(stKonfig->sAGCAdjust & 0xFF);
	chDane[29] = stKonfig->chPoziomEkspozycji;

	return getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_UST_PAR_KAMERY, chDane, 30, m_chRamkaPrzy, &chOdebrano);
}


uint8_t CKomunikacja::UstawKamereGrupowo(st_KonfKam* stKonfig)
{
	uint8_t chDane[30];
	uint8_t chOdebrano;


	chDane[0] = stKonfig->chSzerWy;
	chDane[1] = stKonfig->chWysWy;
	chDane[2] = stKonfig->chSzerWe;
	chDane[3] = stKonfig->chWysWe;
	chDane[4] = stKonfig->chPrzesWyPoz;
	chDane[5] = stKonfig->chPrzesWyPio;
	chDane[6] = stKonfig->chObracanieObrazu;
	chDane[7] = stKonfig->chFormatObrazu;
	chDane[8] = (uint8_t)(stKonfig->sWzmocnienieR >> 8);
	chDane[9] = (uint8_t)(stKonfig->sWzmocnienieR & 0xFF);
	chDane[10] = (uint8_t)(stKonfig->sWzmocnienieG >> 8);
	chDane[11] = (uint8_t)(stKonfig->sWzmocnienieG & 0xFF);
	chDane[12] = (uint8_t)(stKonfig->sWzmocnienieB >> 8);
	chDane[13] = (uint8_t)(stKonfig->sWzmocnienieB & 0xFF);
	chDane[14] = stKonfig->chKontrolaBalansuBieli;
	chDane[15] = stKonfig->chNasycenie;
	chDane[16] = (uint8_t)(stKonfig->sCzasEkspozycji >> 8);
	chDane[17] = (uint8_t)(stKonfig->sCzasEkspozycji & 0xFF);
	chDane[18] = stKonfig->chKontrolaExpo;
	chDane[19] = stKonfig->chTrybyEkspozycji;
	chDane[20] = (uint8_t)(stKonfig->sAGCLong >> 8);
	chDane[21] = (uint8_t)(stKonfig->sAGCLong & 0xFF);
	chDane[22] = (uint8_t)(stKonfig->sAGCVTS >> 8);
	chDane[23] = (uint8_t)(stKonfig->sAGCVTS & 0xFF);
	chDane[24] = stKonfig->chKontrolaISP0;		//0x5000
	chDane[25] = stKonfig->chKontrolaISP1;		//0x50001
	chDane[26] = stKonfig->chProgUsuwania;		//0x5080 Even CTRL 00 Treshold for even odd  cancelling
	chDane[27] = (uint8_t)(stKonfig->sAGCAdjust >> 8);
	chDane[28] = (uint8_t)(stKonfig->sAGCAdjust & 0xFF);
	chDane[29] = stKonfig->chPoziomEkspozycji;

	return getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_UST_PAR_KAMERY_GRUP, chDane, 30, m_chRamkaPrzy, &chOdebrano);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wysy³a polecenie zapisu danych pod wskazany adres we flash. Optymalne jest programowanie co najmniej ca³ymi stronami po 16 s³ów (32 bajty)
// parametry:
// [i] chAdresAPL - adres autopilota
// [i] nAdres - adres bezwzglêdny pocz¹tku obszaru pamiêci z zakresu 0x68000000..0x6A000000
// [i] *sDane - wskaŸnik na bufor z danymi do zaprogramowania
// [i] chRozmiar - iloœæ s³ów do zapisu max 128
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::ZapiszBuforFlash(uint16_t sAdresBufora, uint8_t* chDane, uint8_t chRozmiar)
{
	uint8_t chOdebrano = 0;
	uint8_t chDaneWychodzace[255-3];
	uint8_t chDanePrzychodzace[ROZM_DANYCH_UART];

	if (chRozmiar > 252)
		return ERR_INVALID_DATA_SIZE;

	m_unia8_32.dane16[0] = sAdresBufora/2;		//w APL bufor jest 16-bitowy
	chDaneWychodzace[0] = m_unia8_32.dane8[0];
	chDaneWychodzace[1] = m_unia8_32.dane8[1];
	chDaneWychodzace[2] = chRozmiar/2;		//liczba s³ów 16-bitowych do zapisu

	for (uint8_t n = 0; n < chRozmiar; n++)
		chDaneWychodzace[n + 3] = chDane[n];
	
	return getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_ZAPISZ_BUFOR, chDaneWychodzace, chRozmiar+3, chDanePrzychodzace, &chOdebrano);	//timeout normalny
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wysy³a polecenie skasowania sektora pamieci flash o rozmiarze 128kB
// parametry:
// [i] chAdresAPL - adres autopilota
// [i] nAdresPamieci - adres bezwzglêdny pocz¹tku obszaru pamiêci z zakresu 0x68000000..0x6A000000
// // zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::SkasujSektorFlash(uint32_t nAdresPamieci)
{
	uint8_t chOdebrano = 0;

	m_unia8_32.dane32 = nAdresPamieci;
	for (uint8_t n = 0; n < 4; n++)
		m_chRamkaWych[n] = m_unia8_32.dane8[n];
	return getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_KASUJ_FLASH, m_chRamkaWych, 4, m_chRamkaPrzy, &chOdebrano, 2000);	//du¿y timeout bo Sector Erase time = 1100 ms
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wysy³a polecenie zapisu danych z bufora pod wskazany adres we flash. 
// parametry:
// [i] nAdres - adres bezwzglêdny pocz¹tku obszaru pamiêci z zakresu 0x68000000..0x6A000000
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::ZapiszFlash(uint32_t nAdresPamieci)
{
	uint8_t chOdebrano = 0;
	uint8_t chDaneWychodzace[4];
	uint8_t chDanePrzychodzace[ROZM_DANYCH_UART];

	m_unia8_32.dane32 = nAdresPamieci;
	for (uint8_t n = 0; n < 4; n++)
		chDaneWychodzace[n] = m_unia8_32.dane8[n];

	return getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_ZAPISZ_FLASH, chDaneWychodzace, 4, chDanePrzychodzace, &chOdebrano, 500);	//Sector Programming time = 192ms (pdf str.46)
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wysy³a polecenie zapisu danych z bufora pod wskazany adres we flash. 
// parametry:
// [i] nAdres - adres bezwzglêdny pocz¹tku obszaru pamiêci z zakresu 0x68000000..0x6A000000
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::CzytajFlash(uint32_t nAdresPamieci, uint16_t* sDane, uint8_t chRozmiar)
{
	uint8_t chErr, chOdebrano;
	uint8_t chDaneWychodzace[5];
	uint8_t chDanePrzychodzace[ROZM_DANYCH_UART];

	m_unia8_32.dane32 = nAdresPamieci;
	for (uint8_t n = 0; n < 4; n++)
		chDaneWychodzace[n] = m_unia8_32.dane8[n];
	chDaneWychodzace[4] = chRozmiar;

	chErr = getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_CZYTAJ_FLASH, chDaneWychodzace, 5, chDanePrzychodzace, &chOdebrano);
	if (chErr == ERR_OK)
	{
		for (uint8_t n = 0; n < chOdebrano/2; n++)
		{
			m_unia8_32.dane8[0] = chDanePrzychodzace[2 * n + 0];
			m_unia8_32.dane8[1] = chDanePrzychodzace[2 * n + 1];
			sDane[n] = m_unia8_32.dane16[0];
		}
	}
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Odczytuje z APL listê okresów dla wszystkich zmiennych telemetrycznych
// parametry:
// [i] chOKres - wskaŸnik na tablicê z okresem
// [i] chRozmiar - rozmiar tablicy
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::CzytajOkresTelemetrii(uint16_t* sOKres, uint16_t sRozmiar)
{
	uint8_t chErr, chOdebrano;
	uint8_t chDaneWychodzace[3];
	uint8_t chDanePrzychodzace[ROZM_DANYCH_UART];
	uint8_t chLiczbaRamek = (uint8_t)ceil((double)sRozmiar / OKRESOW_TELEMETRII_W_RAMCE);
	uint16_t sOdebranoLacznie = 0;

	ASSERT(sRozmiar <= LICZBA_ZMIENNYCH_TELEMETRYCZNYCH);

	//w ramce nie zmieszcz¹ siê wszystkie 16-bitowe okresy, wiec na raz przesy³am maksymalnie OKRESOW_TELEMETRII_W_RAMCE
	for (int n = 0; n < chLiczbaRamek; n++)
	{
		if ((sRozmiar - sOdebranoLacznie) > OKRESOW_TELEMETRII_W_RAMCE)
			chDaneWychodzace[0] = OKRESOW_TELEMETRII_W_RAMCE;
		else
			chDaneWychodzace[0] = sRozmiar - sOdebranoLacznie;			//rozmiar
		chDaneWychodzace[1] = (uint8_t)((n * OKRESOW_TELEMETRII_W_RAMCE) & 0x00FF);		//przesuniêcie, czyli czytaj od tego indeksu
		chDaneWychodzace[2] = (uint8_t)(((n * OKRESOW_TELEMETRII_W_RAMCE) & 0xFF00) >> 8);

		chErr = getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_CZYTAJ_OKRES_TELE, chDaneWychodzace, 3, chDanePrzychodzace, &chOdebrano);
		if (chErr == ERR_OK)
		{
			for (uint8_t m = 0; m < chOdebrano/2; m++)
				*(sOKres + n * OKRESOW_TELEMETRII_W_RAMCE + m) = chDanePrzychodzace[m * 2 + 0] + chDanePrzychodzace[m * 2 + 1] * 0x100;
			sOdebranoLacznie += chOdebrano/2;
		}		
	}
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Zapisuje do APL listê okresów dla zmiennych telemetrycznych w formacie m³odszy przodem
// Dane s¹ zaspisywane we Flash dostêpny dla CM7, zapsuje siê w paczkach 32 bajtowych, gdzie nag³ówek paczki i suma kontrolna zajmuj¹ 2 bajty,
// wiêc do efektywnego u¿ycia jest 30 bajtów, czyli 15 okresów. Dane nale¿y wysy³ac paczkami o rozmiarze  podzielnym przez 15, tak aby 
// nie by³o problemu polegaj¹cego na tym ¿e czêœæ danych z koñca pierwszej i pocz¹tku drugiej ramki mia³a by byæ zapisana w jednej paczce.
// To bardzo utrudni³o by zarz¹dzanie danymi, wiêc porz¹dek trzeba zrobiæ ju¿ tutaj wysy³ajac liczê danych podzieln¹ przez 15 czyli np 120 zmiennych -> 240 bajtów
// parametry:
// [i] chOKres - wskaŸnik na tablicê z okresem
// [i] sRozmiar - rozmiar tablicy, czyli liczba okresów do zapisania
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::ZapiszOkresTelemetrii(uint16_t *sOKres, uint16_t sRozmiar)
{
	uint8_t chErr, chOdebrano;
	uint8_t chDaneWychodzace[ROZM_DANYCH_UART];
	uint8_t chDanePrzychodzace[ROZM_DANYCH_UART];
	uint16_t sTemp;
	uint8_t chLiczbaRamek = (uint8_t)ceil((double)sRozmiar / OKRESOW_TELEMETRII_W_RAMCE);
	uint8_t chRozmiar;
	uint16_t sDoZapisu = sRozmiar;

	ASSERT(sRozmiar <= LICZBA_RAMEK_TELEMETR * OKRESOW_TELEMETRII_W_RAMCE);

	for (int n = 0; n < chLiczbaRamek; n++)
	{
		//na pocz¹tku wyœlij indeks pierwszej zmiennej
		chDaneWychodzace[0] = (uint8_t) ((n* OKRESOW_TELEMETRII_W_RAMCE) & 0x00FF);
		chDaneWychodzace[1] = (uint8_t)(((n* OKRESOW_TELEMETRII_W_RAMCE) & 0xFF00) >> 8);

		//okreœl iloœæ danych w bie¿¹cej ramce
		if (sDoZapisu > OKRESOW_TELEMETRII_W_RAMCE)
			chRozmiar = OKRESOW_TELEMETRII_W_RAMCE;
		else
			chRozmiar = (uint8_t)sDoZapisu;

		//potem wyœlij kolejne zmienne
		for (uint8_t m = 0; m < chRozmiar; m++)
		{
			sTemp = *(sOKres + n * OKRESOW_TELEMETRII_W_RAMCE + m);
			chDaneWychodzace[2 * m + 2] = (uint8_t)(sTemp & 0x00FF);
			chDaneWychodzace[2 * m + 3] = (uint8_t)((sTemp & 0xFF00) >> 8);
		}
		chErr = getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_ZAPISZ_OKRES_TELE, chDaneWychodzace, 2 * chRozmiar + 2, chDanePrzychodzace, &chOdebrano);
		sDoZapisu -= chRozmiar;
	}	
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Zapisuje do FRAM w APL liczby uint8_t (max 4*ROZMIAR_ROZNE)
// parametry:
// [i] chDane - wskaŸnik na tablicê z danymi do zapisu
// [i] chRozmiar - rozmiar tablicy liczb
// [i] sAdres - adres we FRAM
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::ZapiszDaneU8FRAM(uint8_t* chDane, uint8_t chRozmiar, uint16_t sAdres)
{
	uint8_t chErr, chOdebrano;
	uint8_t chDaneWychodzace[3 + ROZMIAR_ROZNE_CHAR];
	uint8_t chDanePrzychodzace[ROZM_DANYCH_UART];

	chDaneWychodzace[0] = chRozmiar;
	m_unia8_32.dane16[0] = sAdres;
	chDaneWychodzace[1] = m_unia8_32.dane8[0];
	chDaneWychodzace[2] = m_unia8_32.dane8[1];
	for (uint8_t n = 0; n < chRozmiar; n++)
		chDaneWychodzace[3 + n] = *(chDane + n);
	chErr = getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_ZAPISZ_FRAM_U8, chDaneWychodzace, 4 * chRozmiar + 3, chDanePrzychodzace, &chOdebrano);
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Zapisuje do FRAM w APL liczbê float (max ROZMIAR_ROZNE)
// parametry:
// [i] fDane - wskaŸnik na tablicê z danymi do zapisu
// [i] chRozmiar - rozmiar tablicy liczb
// [i] sAdres - adres we FRAM
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::ZapiszDaneFloatFRAM(float* fDane, uint8_t chRozmiar, uint16_t sAdres)
{
	uint8_t chErr, chOdebrano;
	uint8_t chDaneWychodzace[3+ ROZMIAR_ROZNE_CHAR];
	uint8_t chDanePrzychodzace[ROZM_DANYCH_UART];

	chDaneWychodzace[0] = chRozmiar;
	m_unia8_32.dane16[0] = sAdres;
	chDaneWychodzace[1] = m_unia8_32.dane8[0];
	chDaneWychodzace[2] = m_unia8_32.dane8[1];
	for (uint8_t n = 0; n < chRozmiar; n++)
	{
		m_unia8_32.daneFloat = *(fDane + n);
		for (int i = 0; i < 4; i++)
			chDaneWychodzace[3 + 4 * n + i] = m_unia8_32.dane8[i];
	}
	chErr = getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_ZAPISZ_FRAM_FLOAT, chDaneWychodzace, 4 * chRozmiar + 3, chDanePrzychodzace, &chOdebrano);
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Odsy³a potwierdzenie zaspisania danych lub jego brak
// parametry:
// [i] sAdres - adres we FRAM
// zwraca: kod b³êdu: ERROK lub ERR_PROCES_TRWA
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::PotwierdzZapisDanych(uint16_t sAdres)
{
	uint8_t chErr, chOdebrano;
	uint8_t chDaneWychodzace[3 + ROZMIAR_ROZNE_CHAR];
	uint8_t chDanePrzychodzace[ROZM_DANYCH_UART];

	m_unia8_32.dane16[0] = sAdres;
	chDaneWychodzace[0] = m_unia8_32.dane8[0];
	chDaneWychodzace[1] = m_unia8_32.dane8[1];
	chErr = getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_WYSLIJ_POTW_ZAPISU, chDaneWychodzace, 2, chDanePrzychodzace, &chOdebrano);
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Zapisuje do FRAM w APL liczby uint8_t (max 4*ROZMIAR_ROZNE) z potwierdzeniem zapisu
// parametry:
// [i] *chDane - wskaŸnik na tablicê z danymi do zapisu
// [i] chRozmiar - rozmiar tablicy liczb
// [i] sAdres - adres we FRAM
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::ZapiszU8FRAM(uint8_t* chDane, uint8_t chRozmiar, uint16_t sAdres)
{
	uint8_t chLicznikProbPotw = LICZBA_PROB_ZANIM_ZGLOSI_BLAD;
	uint8_t chErr = ZapiszDaneU8FRAM(chDane, chRozmiar, sAdres);
	if (chErr == ERR_OK)
	{
		do  //czekaj na potwierdzenie zanim wyœle kolejne dane do zapisu
		{
			chErr = PotwierdzZapisDanych(sAdres);
			chLicznikProbPotw--;
		} while ((chErr != ERR_OK) && chLicznikProbPotw);
		if (!chLicznikProbPotw)
			chErr = ERR_BRAK_POTWIERDZ;
	}
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Zapisuje do FRAM w APL liczbê float (max ROZMIAR_ROZNE) z potwierdzeniem zapisu
// parametry:
// [i] fDane - wskaŸnik na tablicê z danymi do zapisu
// [i] chRozmiar - rozmiar tablicy liczb
// [i] sAdres - adres we FRAM
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::ZapiszFloatFRAM(float* fDane, uint8_t chRozmiar, uint16_t sAdres)
{
	uint8_t chLicznikProbPotw = LICZBA_PROB_ZANIM_ZGLOSI_BLAD;
	uint8_t chErr = ZapiszDaneFloatFRAM(fDane, chRozmiar, sAdres);
	if (chErr == ERR_OK)
	{		
		do  //czekaj na potwierdzenie zanim wyœle kolejne dane do zapisu
		{
			chErr = PotwierdzZapisDanych(sAdres);
			chLicznikProbPotw--;
		} while ((chErr != ERR_OK) && chLicznikProbPotw);
		if (!chLicznikProbPotw)
			chErr = ERR_BRAK_POTWIERDZ;
	}
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Inicjuje odczyt liczb uint8_t z FRAM w APL (max 4*ROZMIAR_ROZNE)
// parametry:
// [i] chRozmiar - iloœæ liczb do odczytu
// [i] sAdres - adres we FRAM
// zwraca: kod b³êdu: ERR_OK lub ERR_ZLA_ILOSC_DANYCH
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::InicjujOdczytU8FRAM(uint8_t chRozmiar, uint16_t sAdres)
{
	uint8_t chErr, chOdebrano;
	uint8_t chDaneWychodzace[3];
	uint8_t chDanePrzychodzace[ROZM_DANYCH_UART];

	if (chRozmiar > ROZMIAR_ROZNE_CHAR)
		return ERR_INVALID_DATA_SIZE;

	chDaneWychodzace[0] = chRozmiar;
	m_unia8_32.dane16[0] = sAdres;
	chDaneWychodzace[1] = m_unia8_32.dane8[0];
	chDaneWychodzace[2] = m_unia8_32.dane8[1];

	chErr = getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_CZYTAJ_FRAM_U8, chDaneWychodzace, 3, chDanePrzychodzace, &chOdebrano);
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Inicjuje odczyt z FRAM w APL liczb float (max ROZMIAR_ROZNE)
// parametry:
// [i] chRozmiar - iloœæ liczb do odczytu
// [i] sAdres - adres we FRAM
// zwraca: kod b³êdu: ERR_OK lub ERR_ZLA_ILOSC_DANYCH
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::InicjujOdczytFloatFRAM(uint8_t chRozmiar, uint16_t sAdres)
{
	uint8_t chErr, chOdebrano;
	uint8_t chDaneWychodzace[3];
	uint8_t chDanePrzychodzace[ROZM_DANYCH_UART];

	if (chRozmiar > ROZMIAR_ROZNE_FLOAT)
		return ERR_INVALID_DATA_SIZE;

	chDaneWychodzace[0] = chRozmiar;
	m_unia8_32.dane16[0] = sAdres;
	chDaneWychodzace[1] = m_unia8_32.dane8[0];
	chDaneWychodzace[2] = m_unia8_32.dane8[1];

	chErr = getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_CZYTAJ_FRAM_FLOAT, chDaneWychodzace, 3 , chDanePrzychodzace, &chOdebrano);
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Odczytuje liczby uint8_t z FRAM w APL (max 4*ROZMIAR_ROZNE)
// parametry:
// [i] chDane - wskaŸnik na tablicê z danymi do odczytu
// [i] chRozmiar - iloœæ czwrek liczb
// [i] sAdres - adres we FRAM
// zwraca: kod b³êdu: ERR_OK lub ERR_HAL_BUSY je¿eli nie s¹ jeszcze gotowe
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::CzytajDaneU8FRAM(uint8_t* chDane, uint8_t chRozmiar)
{
	uint8_t chErr, chOdebrano;
	uint8_t chDaneWychodzace;
	uint8_t chDanePrzychodzace[ROZM_DANYCH_UART];

	chDaneWychodzace = chRozmiar;
	chErr = getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_WYSLIJ_ODCZYT_FRAM, &chDaneWychodzace, 1, chDanePrzychodzace, &chOdebrano);
	if (chErr == ERR_OK)
	{
		if ((chOdebrano == 2) && (chDanePrzychodzace[0] == ERR_PROCES_TRWA))
			return ERR_PROCES_TRWA;
		for (uint8_t n = 0; n < chRozmiar; n++)
			*(chDane + n) = chDanePrzychodzace[n];
	}
	else
	{
		chDane = NULL;
	}
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Kompletna procedura odczytu liczb float z FRAM w APL (max ROZMIAR_ROZNE). 
// parametry:
// [i] fDane - wskaŸnik na tablicê z danymi do odczytu
// [i] chRozmiar - rozmiar tablicy liczb
// [i] sAdres - adres we FRAM
// zwraca: kod b³êdu: ERR_OK lub ERR_BRAK_POTWIERDZ je¿eli nie s¹ jeszcze gotowe
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::CzytajFloatFRAM(float* fDane, uint8_t chRozmiar, uint16_t sAdres)
{
	uint8_t chLicznikProbOdczytu = LICZBA_PROB_ZANIM_ZGLOSI_BLAD;
	uint8_t chDane[4 * ROZMIAR_ROZNE_FLOAT];
	uint8_t chErr = InicjujOdczytFloatFRAM(chRozmiar, sAdres);
	if (chErr == ERR_OK)
	{
		do
		{
			chErr = CzytajDaneU8FRAM(chDane, 4*chRozmiar);
			chLicznikProbOdczytu--;
		} while ((chErr != ERR_OK) && (chLicznikProbOdczytu));

		if (!chLicznikProbOdczytu)
			return ERR_BRAK_POTWIERDZ;

		for (int n = 0; n < chRozmiar; n++)
		{
			for (int i = 0; i < 4; i++)
				m_unia8_32.dane8[i] = chDane[4 * n + i];
			*(fDane + n) = m_unia8_32.daneFloat;
		}
	}
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Kompletna procedura odczytu liczb uint8_t z FRAM w APL (max 4*ROZMIAR_ROZNE). 
// parametry:
// [i] chDane - wskaŸnik na tablicê z danymi do odczytu
// [i] chRozmiar - liczba czwórek liczb
// [i] sAdres - adres we FRAM
// zwraca: kod b³êdu: ERR_OK lub ERR_BRAK_POTWIERDZ je¿eli nie s¹ jeszcze gotowe
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::CzytajU8FRAM(uint8_t* chDane, uint8_t chRozmiar, uint16_t sAdres)
{

	uint8_t chLicznikProbOdczytu = LICZBA_PROB_ZANIM_ZGLOSI_BLAD;
	uint8_t chErr = InicjujOdczytU8FRAM(chRozmiar, sAdres);
	if (chErr == ERR_OK)
	{
		do
		{
			chErr = CzytajDaneU8FRAM(chDane, chRozmiar);
			chLicznikProbOdczytu--;
		} while ((chErr != ERR_OK) && (chLicznikProbOdczytu));

		if (!chLicznikProbOdczytu)
			chErr = ERR_BRAK_POTWIERDZ;
	}
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Pakuje danne uint8 do float i w takiej postaci jest to wysy³ane do zapisania we FRMA ze wzgledu na uniwersalnoœæ
// parametry:
// [i] *chDane - wskaŸnik na tablicê z danymi do spakowania
// [i] chRozmiar - rozmiar tablicy liczb uint8_t
// [o] *fData - wskaŸnik na tablicê danych float
// zwraca: rozmiar tablicy liczb float
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::SpakujU8doFloat(uint8_t* chDane, uint8_t chRozmiarU8, float* fData)
{
	uint8_t chRozmiarFloat = chRozmiarU8 / 4;
	if (chRozmiarFloat * 4 < chRozmiarU8)
		chRozmiarFloat++;

	for (uint8_t n = 0; n < chRozmiarFloat; n++)
	{
		for (uint8_t i = 0; i < 4; i++)
			m_unia8_32.dane8[i] = *(chDane + i + n * 4);
		*(fData+n) = m_unia8_32.daneFloat;
	}
	return chRozmiarFloat;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Rozpakowuje danne przesy³ana jako float do uint8_t
// parametry:
// [i] *fData - wskaŸnik na tablicê danych float
// [i] chRozmiar - rozmiar tablicy liczb uint8_t
// [o] *chDane - wskaŸnik na tablicê z danymi do spakowania
// zwraca: rozmiar tablicy liczb float
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::RozpakujFloatDoU8(float* fData, uint8_t chRozmiarU8, uint8_t* chDane)
{
	uint8_t chRozmiarFloat = chRozmiarU8 / 4;
	if (chRozmiarFloat * 4 < chRozmiarU8)
		chRozmiarFloat++;

	for (uint8_t n = 0; n < chRozmiarFloat; n++)
	{
		m_unia8_32.daneFloat = *(fData + n);
		for (uint8_t i = 0; i < 4; i++)
			*(chDane + i + n * 4) = m_unia8_32.dane8[i];
		
	}
	return chRozmiarFloat;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Uruchamia ponown¹ konfiguracjê wejsæ i wyjsæ RC po zmianie zawartoœci FRAM
// Abyt polecenie nie wykonywa³o siê w pêtli, po poleceniu trzeba wydaæ inne polecenie zakoñczone POL_NIC, np. Odczyt FRAM
// parametry: brak
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::RekonfigurujWeWyRC()
{
	uint8_t chErr, chOdebrano;
	uint8_t chDaneWychodzace;
	uint8_t chDanePrzychodzace[ROZM_DANYCH_UART];

	chErr = getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_REKONFIG_SERWA_RC, &chDaneWychodzace, 0, chDanePrzychodzace, &chOdebrano);

	CzytajU8FRAM(chDanePrzychodzace, 1, FAU_KONF_ODB_RC);
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Przekazuje do autopilota zmodyfikowan¹ konfiguracjê regulatora PID
// parametry: chIndeksRegulatora - wskazuje na regualtor do zapisania
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::ZapiszKonfiguracjePID(uint8_t chIndeksRegulatora, float fKp, float fTi, float fTd, float fLimitCalki, float fMinPid, float fMaxPid, float fSkalaWZadanej, uint8_t chStalaCzasowaFiltraD)
{
	uint8_t chErr, chOdebrano;
	uint8_t chDaneWychodzace[ROZMIAR_PID_FLOAT * sizeof(float)];
	uint8_t chDanePrzychodzace[ROZM_DANYCH_UART];

	chDaneWychodzace[0] = chIndeksRegulatora;
	chDaneWychodzace[1] = chStalaCzasowaFiltraD;
	chDaneWychodzace[2] = 0;	//wolny
	chDaneWychodzace[3] = 0;	//wolny
	
	m_unia8_32.daneFloat = fKp;
	for (int i = 0; i < 4; i++)
		chDaneWychodzace[4 + i] = m_unia8_32.dane8[i];
	m_unia8_32.daneFloat = fTi;
	for (int i = 0; i < 4; i++)
		chDaneWychodzace[8 + i] = m_unia8_32.dane8[i];
	m_unia8_32.daneFloat = fTd;
	for (int i = 0; i < 4; i++)
		chDaneWychodzace[12 + i] = m_unia8_32.dane8[i];

	m_unia8_32.daneFloat = fLimitCalki;
	for (int i = 0; i < 4; i++)
		chDaneWychodzace[16 + i] = m_unia8_32.dane8[i];
	m_unia8_32.daneFloat = fMinPid;
	for (int i = 0; i < 4; i++)
		chDaneWychodzace[20 + i] = m_unia8_32.dane8[i];
	m_unia8_32.daneFloat = fMaxPid;
	for (int i = 0; i < 4; i++)
		chDaneWychodzace[24 + i] = m_unia8_32.dane8[i];
	
	m_unia8_32.daneFloat = fSkalaWZadanej;
	for (int i = 0; i < 4; i++)
		chDaneWychodzace[28 + i] = m_unia8_32.dane8[i];

	chErr = getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_ZAPISZ_KONFIG_PID, chDaneWychodzace, ROZMIAR_PID_FLOAT *sizeof(float), chDanePrzychodzace, &chOdebrano);
	if ((chErr == ERR_OK) && (chOdebrano >= 2))
	{
		if (chDanePrzychodzace[1] == PK_ZAPISZ_KONFIG_PID)
			if (chDanePrzychodzace[0] != ERR_OK)
				chErr = chDanePrzychodzace[0];
	}
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Przekazuje do autopilota wartoœci wysterowania dla zapewnienia obrotów
// parametry: 
//  sJalowe - wysterowanie dla uzyskania obrotów ja³owych
//  sMin - wysterowanie dla uzyskania obrotów minimalnych w trakcie lotu
//  sZawis - wysterowanie dla uzyskania obrotów pozwalaj¹cych na zawis
//  sMax - wysterowanie dla uzyskania obrotów maksymalnych
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::ZapiszWysterowanieObrotow(uint16_t sJalowe, uint16_t sMin, uint16_t sZawis, uint16_t sMax)
{
	uint8_t chErr, chOdebrano;
	uint8_t chDaneWychodzace[4 * sizeof(uint16_t)];
	uint8_t chDanePrzychodzace[ROZM_DANYCH_UART];
	uint8_t chLicznikProbPotw = LICZBA_PROB_ZANIM_ZGLOSI_BLAD;

	m_unia8_32.dane16[0] = sJalowe;
	for (int i = 0; i < 2; i++)
		chDaneWychodzace[i + 0] = m_unia8_32.dane8[i];
	m_unia8_32.dane16[0] = sMin;
	for (int i = 0; i < 2; i++)
		chDaneWychodzace[i + 2] = m_unia8_32.dane8[i];
	m_unia8_32.dane16[0] = sZawis;
	for (int i = 0; i < 2; i++)
		chDaneWychodzace[i + 4] = m_unia8_32.dane8[i];
	m_unia8_32.dane16[0] = sMax;
	for (int i = 0; i < 2; i++)
		chDaneWychodzace[i + 6] = m_unia8_32.dane8[i];

	chErr = getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_ZAPISZ_WYSTER_NAPEDU, chDaneWychodzace, 4 * sizeof(uint16_t), chDanePrzychodzace, &chOdebrano);
	if ((chErr == ERR_OK) && (chOdebrano >= 2))
	{
		if (chDanePrzychodzace[1] == PK_ZAPISZ_WYSTER_NAPEDU)
			if (chDanePrzychodzace[0] != ERR_OK)
				chErr = chDanePrzychodzace[0];
			else
			{
				do  //czekaj na potwierdzenie zapisu
				{
					chErr = PotwierdzZapisDanych(PK_ZAPISZ_WYSTER_NAPEDU);
					chLicznikProbPotw--;
				} while ((chErr != ERR_OK) && chLicznikProbPotw);
				if (!chLicznikProbPotw)
					chErr = ERR_BRAK_POTWIERDZ;
			}
	}
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Zapisuje we FRAM nowa wartosci trybów regulacji
// parametry: 
//  *chTrybRegulacji - wskaŸnik na tablicê trybów regulacji
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::ZapiszTrybRegulacji(uint8_t *chTrybRegulacji)
{
	uint8_t chErr, chOdebrano;
	uint8_t chDaneWychodzace[LICZBA_REG_PARAM];
	uint8_t chDanePrzychodzace[ROZM_DANYCH_UART];
	uint8_t chLicznikProbPotw = LICZBA_PROB_ZANIM_ZGLOSI_BLAD;

	for (int n = 0; n < LICZBA_REG_PARAM; n++)
	{
		chDaneWychodzace[n] = chTrybRegulacji[n];
	}

	chErr = getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_ZAPISZ_TRYB_REG, chDaneWychodzace, LICZBA_REG_PARAM, chDanePrzychodzace, &chOdebrano);
	if ((chErr == ERR_OK) && (chOdebrano >= 2))
	{
		if (chDanePrzychodzace[1] == PK_ZAPISZ_TRYB_REG)
			if (chDanePrzychodzace[0] != ERR_OK)
				chErr = chDanePrzychodzace[0];
			else
			{
				do  //czekaj na potwierdzenie zapisu
				{
					chErr = PotwierdzZapisDanych(PK_ZAPISZ_TRYB_REG);
					chLicznikProbPotw--;
				} while ((chErr != ERR_OK) && chLicznikProbPotw);
				if (!chLicznikProbPotw)
					chErr = ERR_BRAK_POTWIERDZ;
			}
	}
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Informuje rdzeñ CM4 aby rozpocz¹³ zbieranie ekstremów dla wszystkich kana³ów obu odborników
// parametry: brak
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::ZbierajEkstremaWejscRC()
{
	uint8_t chErr, chOdebrano;
	uint8_t chDaneWychodzace[2];
	uint8_t chDanePrzychodzace[ROZM_DANYCH_UART];


	chErr = getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_ZBIERAJ_EKSTREMA_RC, chDaneWychodzace, 0, chDanePrzychodzace, &chOdebrano);
	if (chOdebrano != 2)
		chErr = ERR_BRAK_POTWIERDZ;

	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Informuje rdzeñ CM4 aby zakoñczy³ zbieranie ekstremów dla wszystkich kana³ów obu odborników i wyniki zapisa³ do FRAM
// parametry: brak
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::ZapiszEkstremaWejscRC()
{
	uint8_t chErr, chOdebrano;
	uint8_t chDaneWychodzace[2];
	uint8_t chDanePrzychodzace[ROZM_DANYCH_UART];


	chErr = getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_ZAPISZ_EKSTREMA_RC, chDaneWychodzace, 0, chDanePrzychodzace, &chOdebrano);
	if (chOdebrano != 2)
		chErr = ERR_BRAK_POTWIERDZ;

	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wysy³a polecenie nie robienia niczego, zatrzymuj¹c cykliczne wykonywanie poprzedniego polecenia dla CM4
// parametry: brak
// zwraca: kod b³êdu: OK lub odmowa wykonania je¿eli silniki s¹ uzbrojone
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::Wy³aczWykonywaniePoleceniaCM4()
{
	uint8_t chErr, chOdebrano;
	uint8_t chDaneWychodzace[2];
	uint8_t chDanePrzychodzace[ROZM_DANYCH_UART];


	chErr = getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_WYLACZ_POLECENIE_CM4, chDaneWychodzace, 0, chDanePrzychodzace, &chOdebrano);
	if (chOdebrano != 2)
		chErr = ERR_BRAK_POTWIERDZ;

	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wysy³a polecenie resetu rdzenia CM4, zwykle po zmianie konfiguracji.
// parametry: brak
// zwraca: kod b³êdu: OK lub odmowa wykonania je¿eli silniki s¹ uzbrojone
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::ResetujCM4()
{
	uint8_t chErr, chOdebrano;
	uint8_t chDaneWychodzace[2];
	uint8_t chDanePrzychodzace[ROZM_DANYCH_UART];

	chErr = getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_RESETUJ_CM4, chDaneWychodzace, 0, chDanePrzychodzace, &chOdebrano);
	if ((chErr == ERR_OK) && (chOdebrano == 2))
	{
		if (chDanePrzychodzace[1] == PK_RESETUJ_CM4)
			if (chDanePrzychodzace[0] != ERR_OK)
				chErr = chDanePrzychodzace[0];			
	}
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wysy³a polecenie zatrzymania telemetrii
// parametry: chPrzerwa - wartoœæ niezerowa wstrzumuje telemetriê, zerowa wznawia wysy³kê
// zwraca: kod b³êdu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::WstrzymajTelemetrie(uint8_t chPrzerwa)
{
	uint8_t chErr, chOdebrano;
	uint8_t chDaneWychodzace[2];
	uint8_t chDanePrzychodzace[ROZM_DANYCH_UART];

	chDaneWychodzace[0] = chPrzerwa;
	chErr = getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_WSTRZYMAJ_TELEMETRIE, chDaneWychodzace, 1, chDanePrzychodzace, &chOdebrano);
	return chErr;
}

