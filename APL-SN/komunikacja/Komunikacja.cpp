#include "pch.h"
#include "Komunikacja.h"
#include "../Errors.h"
#include "KomunikatySieci.h"
#include "Protokol.h"
#include "../APL-SNDoc.h"
#include "../konfig_fram.h"

/*
Klasa komunikacyjna po�rednicz�ca mi�dzy aplikacj� a protoko�em komunikacyjnym. 
Aplikacja przesy�a polecenia wymiany danych a klasa nawi�zuje po��czenie po znanym sobie interfejsie
pakuje dane w ramki i przesy�a przy u�yciu podrz�dnej klasy CProtokol

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
	m_hZdarzenieZmianaPolaczeniaWrona = CreateEvent(NULL, false, false, _T("ZmianaPo��czenia")); // auto-reset event, non-signalled state	;
	m_wskaznikInstancji = m_LicznikInstancji++;

	//D�ugo�� nazwy nie powinna przekracza� sta�ej DLUGOSC_NAZWY
	m_strNazwyZmiennychTele[TELEID_AKCEL1X]		= "Akcelerometr1 X";
	m_strNazwyZmiennychTele[TELEID_AKCEL1Y]		= "Akcelerometr1 Y";
	m_strNazwyZmiennychTele[TELEID_AKCEL1Z]		= "Akcelerometr1 Z";
	m_strNazwyZmiennychTele[TELEID_AKCEL2X]		= "Akcelerometr2 X";
	m_strNazwyZmiennychTele[TELEID_AKCEL2Y]		= "Akcelerometr2 Y";
	m_strNazwyZmiennychTele[TELEID_AKCEL2Z]		= "Akcelerometr2 Z";
	m_strNazwyZmiennychTele[TELEID_ZYRO1P]		= "�yroskop1 P";
	m_strNazwyZmiennychTele[TELEID_ZYRO1Q]		= "�yroskop1 Q";
	m_strNazwyZmiennychTele[TELEID_ZYRO1R]		= "�yroskop1 R";
	m_strNazwyZmiennychTele[TELEID_ZYRO2P]		= "�yroskop2 P";
	m_strNazwyZmiennychTele[TELEID_ZYRO2Q]		= "�yroskop2 Q";
	m_strNazwyZmiennychTele[TELEID_ZYRO2R]		= "�yroskop2 R";
	m_strNazwyZmiennychTele[TELEID_MAGNE1X]		= "Magnetometr 1 X";
	m_strNazwyZmiennychTele[TELEID_MAGNE1Y]		= "Magnetometr 1 Y";
	m_strNazwyZmiennychTele[TELEID_MAGNE1Z]		= "Magnetometr 1 Z";
	m_strNazwyZmiennychTele[TELEID_MAGNE2X]		= "Magnetometr 2 X";
	m_strNazwyZmiennychTele[TELEID_MAGNE2Y]		= "Magnetometr 2 Y";
	m_strNazwyZmiennychTele[TELEID_MAGNE2Z]		= "Magnetometr 2 Z";
	m_strNazwyZmiennychTele[TELEID_MAGNE3X]		= "Magnetometr 3 X";
	m_strNazwyZmiennychTele[TELEID_MAGNE3Y]		= "Magnetometr 3 Y";
	m_strNazwyZmiennychTele[TELEID_MAGNE3Z]		= "Magnetometr 3 Z";
	m_strNazwyZmiennychTele[TELEID_TEMPIMU1]	= "Temperatura IMU1";
	m_strNazwyZmiennychTele[TELEID_TEMPIMU2]	= "Temperatura IMU2";

	//zmienna AHRS
	m_strNazwyZmiennychTele[TELEID_KAT_IMU1X]	= "Trygono. Phi";
	m_strNazwyZmiennychTele[TELEID_KAT_IMU1Y]	= "Trygono. Theta";
	m_strNazwyZmiennychTele[TELEID_KAT_IMU1Z]	= "Trygono. Psi";
	m_strNazwyZmiennychTele[TELEID_KAT_IMU2X]	= "Kwatern. Phi";
	m_strNazwyZmiennychTele[TELEID_KAT_IMU2Y]	= "Kwatern. Theta";
	m_strNazwyZmiennychTele[TELEID_KAT_IMU2Z]	= "Kwatern. Psi";
	m_strNazwyZmiennychTele[TELEID_KAT_ZYRO1X]	= "�yro. Phi";
	m_strNazwyZmiennychTele[TELEID_KAT_ZYRO1Y]	= "�yro. Theta";
	m_strNazwyZmiennychTele[TELEID_KAT_ZYRO1Z]	= "�yro. Psi";
	m_strNazwyZmiennychTele[TELEID_KAT_AKCELX]	= "Akcel. Phi";
	m_strNazwyZmiennychTele[TELEID_KAT_AKCELY]	= "Akcel. Theta";
	m_strNazwyZmiennychTele[TELEID_KAT_AKCELZ]	= "Akcel. Psi";

	//zmienne barametryczne
	m_strNazwyZmiennychTele[TELEID_CISBEZW1]	= "Ci�n.Bzwzgl.1";
	m_strNazwyZmiennychTele[TELEID_CISBEZW2]	= "Ci�n.Bzwzgl.2";
	m_strNazwyZmiennychTele[TELEID_WYSOKOSC1]	= "Wysoko�� AGL1";
	m_strNazwyZmiennychTele[TELEID_WYSOKOSC2]	= "Wysoko�� AGL2";
	m_strNazwyZmiennychTele[TELEID_CISROZN1]	= "Ci�n.R�n.1";
	m_strNazwyZmiennychTele[TELEID_CISROZN2]	= "Ci�n.R�n.2";
	m_strNazwyZmiennychTele[TELEID_PREDIAS1]	= "Pr�dko�� IAS1";
	m_strNazwyZmiennychTele[TELEID_PREDIAS2]	= "Pr�dko�� IAS2";
	m_strNazwyZmiennychTele[TELEID_TEMPCISB1]	= "Temp.Ci�n.Bzwz1";
	m_strNazwyZmiennychTele[TELEID_TEMPCISB2]	= "Temp.Ci�n.Bzwz2";
	m_strNazwyZmiennychTele[TELEID_TEMPCISR1]	= "Temp.Ci�n.R�n1";
	m_strNazwyZmiennychTele[TELEID_TEMPCISR2]	= "Temp.Ci�n.R�n2";

	//odbiorniki RC i serwa
	m_strNazwyZmiennychTele[TELEID_RC_KAN1]		= "Wej�cie RC kan.1";
	m_strNazwyZmiennychTele[TELEID_RC_KAN2]		= "Wej�cie RC kan.2";
	m_strNazwyZmiennychTele[TELEID_RC_KAN3]		= "Wej�cie RC kan.3";
	m_strNazwyZmiennychTele[TELEID_RC_KAN4]		= "Wej�cie RC kan.4";
	m_strNazwyZmiennychTele[TELEID_RC_KAN5]		= "Wej�cie RC kan.5";
	m_strNazwyZmiennychTele[TELEID_RC_KAN6]		= "Wej�cie RC kan.6";
	m_strNazwyZmiennychTele[TELEID_RC_KAN7]		= "Wej�cie RC kan.7";
	m_strNazwyZmiennychTele[TELEID_RC_KAN8]		= "Wej�cie RC kan.8";
	m_strNazwyZmiennychTele[TELEID_RC_KAN9]		= "Wej�cie RC kan.9";
	m_strNazwyZmiennychTele[TELEID_RC_KAN10]	= "Wej�cie RC kan.10";
	m_strNazwyZmiennychTele[TELEID_RC_KAN11]	= "Wej�cie RC kan.11";
	m_strNazwyZmiennychTele[TELEID_RC_KAN12]	= "Wej�cie RC kan.12";
	m_strNazwyZmiennychTele[TELEID_RC_KAN13]	= "Wej�cie RC kan.13";
	m_strNazwyZmiennychTele[TELEID_RC_KAN14]	= "Wej�cie RC kan.14";
	m_strNazwyZmiennychTele[TELEID_RC_KAN15]	= "Wej�cie RC kan.15";
	m_strNazwyZmiennychTele[TELEID_RC_KAN16]	= "Wej�cie RC kan.16";

	m_strNazwyZmiennychTele[TELEID_SERWO1]		= "Wyj�cie Serw kan.1";
	m_strNazwyZmiennychTele[TELEID_SERWO2]		= "Wyj�cie Serw kan.2";
	m_strNazwyZmiennychTele[TELEID_SERWO3]		= "Wyj�cie Serw kan.3";
	m_strNazwyZmiennychTele[TELEID_SERWO4]		= "Wyj�cie Serw kan.4";
	m_strNazwyZmiennychTele[TELEID_SERWO5]		= "Wyj�cie Serw kan.5";
	m_strNazwyZmiennychTele[TELEID_SERWO6]		= "Wyj�cie Serw kan.6";
	m_strNazwyZmiennychTele[TELEID_SERWO7]		= "Wyj�cie Serw kan.7";
	m_strNazwyZmiennychTele[TELEID_SERWO8]		= "Wyj�cie Serw kan.8";
	m_strNazwyZmiennychTele[TELEID_SERWO9]		= "Wyj�cie Serw kan.9";
	m_strNazwyZmiennychTele[TELEID_SERWO10]		= "Wyj�cie Serw kan.10";
	m_strNazwyZmiennychTele[TELEID_SERWO11]		= "Wyj�cie Serw kan.11";
	m_strNazwyZmiennychTele[TELEID_SERWO12]		= "Wyj�cie Serw kan.12";
	m_strNazwyZmiennychTele[TELEID_SERWO13]		= "Wyj�cie Serw kan.13";
	m_strNazwyZmiennychTele[TELEID_SERWO14]		= "Wyj�cie Serw kan.14";
	m_strNazwyZmiennychTele[TELEID_SERWO15]		= "Wyj�cie Serw kan.15";
	m_strNazwyZmiennychTele[TELEID_SERWO16]		= "Wyj�cie Serw kan.16";

	m_strNazwyZmiennychTele[TELEID_PID_PRZE_WZAD]		= "Wart.zadana przech";	//warto�� zadana regulatora sterowania przechyleniem
	m_strNazwyZmiennychTele[TELEID_PID_PRZE_WYJ]		= "Wy PID przechyl";	//wyj�cie regulatora sterowania przechyleniem
	m_strNazwyZmiennychTele[TELEID_PID_PRZE_WY_P]		= "Wy P przechylenia";	//wyj�cie cz�onu P
	m_strNazwyZmiennychTele[TELEID_PID_PRZE_WY_I]		= "Wy I przechylenia";	//wyj�cie cz�onu I
	m_strNazwyZmiennychTele[TELEID_PID_PRZE_WY_D]		= "Wy D przechylenia";	//wyj�cie cz�onu D
	m_strNazwyZmiennychTele[TELEID_PID_PK_PRZE_WYJ]		= "Wy PID pr.k�t.prze";	//wyj�cie regulatora sterowania pr�dko�ci� k�tow� przechylenia
	m_strNazwyZmiennychTele[TELEID_PID_PK_PRZE_WY_P]	= "Wy P pr.k�t.prze";	//wyj�cie cz�onu P
	m_strNazwyZmiennychTele[TELEID_PID_PK_PRZE_WY_I]	= "Wy I pr.k�t.prze";	//wyj�cie cz�onu I
	m_strNazwyZmiennychTele[TELEID_PID_PK_PRZE_WY_D]	= "Wy D pr.k�t.prze";	//wyj�cie cz�onu D

	m_strNazwyZmiennychTele[TELEID_PID_POCH_WZAD]		= "Wart.zadana poch";	//warto�� zadana regulatora sterowania pochyleniem
	m_strNazwyZmiennychTele[TELEID_PID_POCH_WYJ]		= "Wy PID pochylenia";	//wyj�cie regulatora sterowania pochyleniem
	m_strNazwyZmiennychTele[TELEID_PID_POCH_WY_P]		= "Wy P pochylenia";	//wyj�cie cz�onu P
	m_strNazwyZmiennychTele[TELEID_PID_POCH_WY_I]		= "Wy I pochylenia";	//wyj�cie cz�onu I
	m_strNazwyZmiennychTele[TELEID_PID_POCH_WY_D]		= "Wy D pochylenia";	//wyj�cie cz�onu D
	m_strNazwyZmiennychTele[TELEID_PID_PK_POCH_WYJ]		= "Wy PID pr.k�t.poch";	//wyj�cie regulatora sterowania pr�dko�ci� k�tow� pochylenia
	m_strNazwyZmiennychTele[TELEID_PID_PK_POCH_WY_P]	= "Wy P pr.k�t.poch";	//wyj�cie cz�onu P
	m_strNazwyZmiennychTele[TELEID_PID_PK_POCH_WY_I]	= "Wy I pr.k�t.poch";	//wyj�cie cz�onu I
	m_strNazwyZmiennychTele[TELEID_PID_PK_POCH_WY_D]	= "Wy D pr.k�t.poch";	//wyj�cie cz�onu D

	m_strNazwyZmiennychTele[TELEID_PID_ODCH_WZAD]		= "Wart.zadana odch";	//warto�� zadana regulatora sterowania odchyleniem
	m_strNazwyZmiennychTele[TELEID_PID_ODCH_WYJ]		= "Wy PID odchylenia";	//wyj�cie regulatora sterowania odchyleniem
	m_strNazwyZmiennychTele[TELEID_PID_ODCH_WY_P]		= "Wy P odchylenia";	//wyj�cie cz�onu P
	m_strNazwyZmiennychTele[TELEID_PID_ODCH_WY_I]		= "Wy I odchylenia";	//wyj�cie cz�onu I
	m_strNazwyZmiennychTele[TELEID_PID_ODCH_WY_D]		= "Wy D odchylenia";	//wyj�cie cz�onu D
	m_strNazwyZmiennychTele[TELEID_PID_PK_ODCH_WYJ]		= "Wy PID pr.k�t.odch";	//wyj�cie regulatora sterowania pr�dko�ci� k�tow� odchylenia
	m_strNazwyZmiennychTele[TELEID_PID_PK_ODCH_WY_P]	= "Wy P pr.k�t.odch";	//wyj�cie cz�onu P
	m_strNazwyZmiennychTele[TELEID_PID_PK_ODCH_WY_I]	= "Wy I pr.k�t.odch";	//wyj�cie cz�onu I
	m_strNazwyZmiennychTele[TELEID_PID_PK_ODCH_WY_D]	= "Wy D pr.k�t.odch";	//wyj�cie cz�onu D

	m_strNazwyZmiennychTele[TELEID_PID_WYSO_WZAD]		= "Wart.zadana wysoko";	//warto�� zadana regulatora sterowania wysoko�ci�
	m_strNazwyZmiennychTele[TELEID_PID_WYSO_WYJ]		= "Wy PID wysoko�ci";	//wyj�cie regulatora sterowania wysoko�ci�
	m_strNazwyZmiennychTele[TELEID_PID_WYSO_WY_P]		= "Wy P wysoko�ci";	//wyj�cie cz�onu P
	m_strNazwyZmiennychTele[TELEID_PID_WYSO_WY_I]		= "Wy I wysoko�ci";	//wyj�cie cz�onu I
	m_strNazwyZmiennychTele[TELEID_PID_WYSO_WY_D]		= "Wy D wysoko�ci";	//wyj�cie cz�onu D
	m_strNazwyZmiennychTele[TELEID_PID_PR_WYSO_WYJ]		= "Wy PID pr.zm.wysoko";	//wyj�cie regulatora sterowania pr�dko�ci� zmiany wysoko�ci
	m_strNazwyZmiennychTele[TELEID_PID_PR_WYSO_WY_P]	= "Wy P pr.zm.wysoko";	//wyj�cie cz�onu P
	m_strNazwyZmiennychTele[TELEID_PID_PR_WYSO_WY_I]	= "Wy I pr.zm.wysoko";	//wyj�cie cz�onu I
	m_strNazwyZmiennychTele[TELEID_PID_PR_WYSO_WY_D]	= "Wy D pr.zm.wysoko";	//wyj�cie cz�onu D

	m_strNazwyZmiennychTele[TELEID_PID_NAWN_WZAD]		= "Wart.zadana naw.N";	//warto�� zadana regulatora sterowania nawigacj� w kierunku p�nocnym
	m_strNazwyZmiennychTele[TELEID_PID_NAWN_WYJ]		= "Wy PID nawigacji N";	//wyj�cie regulatora sterowania nawigacj� w kierunku p�nocnym
	m_strNazwyZmiennychTele[TELEID_PID_NAWN_WY_P]		= "Wy P nawigacji N";	//wyj�cie cz�onu P
	m_strNazwyZmiennychTele[TELEID_PID_NAWN_WY_I]		= "Wy I  nawigacji N";	//wyj�cie cz�onu I
	m_strNazwyZmiennychTele[TELEID_PID_NAWN_WY_D]		= "Wy D  nawigacji N";	//wyj�cie cz�onu D
	m_strNazwyZmiennychTele[TELEID_PID_PR_NAWN_WYJ]		= "Wy PID pr.nawig.N";	//wyj�cie regulatora sterowania pr�dko�ci� w kierunku p�nocnym
	m_strNazwyZmiennychTele[TELEID_PID_PR_NAWN_WY_P]	= "Wy P pr�d.nawig.N";	//wyj�cie cz�onu P
	m_strNazwyZmiennychTele[TELEID_PID_PR_NAWN_WY_I]	= "Wy I pr�d.nawig.N";	//wyj�cie cz�onu I
	m_strNazwyZmiennychTele[TELEID_PID_PR_NAWN_WY_D]	= "Wy D pr�d.nawig.N";	//wyj�cie cz�onu D

	m_strNazwyZmiennychTele[TELEID_PID_NAWE_WZAD]		= "Wart.zadana naw.E";	//warto�� zadana regulatora sterowania nawigacj� w kierunku wschodnim
	m_strNazwyZmiennychTele[TELEID_PID_NAWE_WYJ]		= "Wy PID nawigacji E";	//wyj�cie regulatora sterowania nawigacj� w kierunku wschodnim
	m_strNazwyZmiennychTele[TELEID_PID_NAWE_WY_P]		= "Wy P nawigacji E";	//wyj�cie cz�onu P
	m_strNazwyZmiennychTele[TELEID_PID_NAWE_WY_I]		= "Wy I  nawigacji E";	//wyj�cie cz�onu I
	m_strNazwyZmiennychTele[TELEID_PID_NAWE_WY_D]		= "Wy D  nawigacji E";	//wyj�cie cz�onu D
	m_strNazwyZmiennychTele[TELEID_PID_PR_NAWE_WYJ]		= "Wy PID pr.nawig.E";	//wyj�cie regulatora sterowania pr�dko�ci� w kierunku wschodnim
	m_strNazwyZmiennychTele[TELEID_PID_PR_NAWE_WY_P]	= "Wy P pr�d.nawig.E";	//wyj�cie cz�onu P
	m_strNazwyZmiennychTele[TELEID_PID_PR_NAWE_WY_I]	= "Wy I pr�d.nawig.E";	//wyj�cie cz�onu I
	m_strNazwyZmiennychTele[TELEID_PID_PR_NAWE_WY_D]	= "Wy D pr�d.nawig.E";	//wyj�cie cz�onu D
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
// Nawi�zuje po��czenia ze znanymi sobie portami komunikacyjnymi
// zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::Polacz(CView* pWnd)
{
	uint8_t chErr = ERR_NOT_CONNECTED;
	Wron cWron;
	uint8_t chNazwa[DLUGOSC_NAZWY];
	int nIndeks = 0;

	switch (m_chTypPolaczenia)
	{
	case UART:	
		chErr = getProtokol().PolaczPort(UART, m_iNumerPortuUART, m_iPredkoscUART, 0, m_pWnd);		
		if (chErr == ERR_OK)
		{
			m_bPolaczonoUart = TRUE;
			//wy�lij na adres rozg�oszeniowy polecenie pobrania adres�w i nazwy BSP
			chErr = PobierzBSP(&cWron.m_chAdres, chNazwa, &cWron.m_chAdresIP[0]);
			if (chErr == ERR_OK)
			{
				//sprawd� czy w roju jest ju� ten wron					
				for (int n = 0; n < m_cRoj.vWron.size(); n++)
				{
					if (m_cRoj.vWron[n].m_chAdres == cWron.m_chAdres)
					{
						nIndeks = n;
						break;
					}
				}

				//je�eli nie ma go w roju to wstaw
				if (nIndeks == 0)
				{
					m_cRoj.vWron.push_back(cWron);
					nIndeks = (int)m_cRoj.vWron.size() - 1;
					m_cRoj.m_nIndeksWrona = nIndeks;
				}

				m_cRoj.vWron[nIndeks].UstawNazwe(chNazwa);
				m_cRoj.vWron[nIndeks].m_chPolaczony = UART;
				UstawAdresWrona(cWron.m_chAdres);	//ustaw adres na potrzeby klasy komunikacji

				chErr = CzytajOkresTelemetrii(m_cRoj.vWron[nIndeks].m_sOkresTelemetrii, LICZBA_ZMIENNYCH_TELEMETRYCZNYCH);
				SetEvent(m_hZdarzenieZmianaPolaczeniaWrona);		//wygeneruj komunikat o zmianie po��czenia			
			}				
		}
		else
		{
			m_bPolaczonoUart = FALSE;
			if (chErr == ERR_CANT_CONNECT)
				MessageBoxExW(pWnd->m_hWnd, _T("Nie mog� po��czy� si� z portem. \nSprawd� czy nie jest ju� po��czony."), _T("Ojojojoj!"), MB_ICONWARNING, 0);
		}
		break;

	case ETHS:	
		chErr = getProtokol().PolaczPort(ETHS, m_iNumerPortuETH, 0, m_strAdresIP, m_pWnd);
		m_bPolaczonoEth = (chErr == ERR_OK);
		break;

	case ETHK:
		m_bPolaczonoEth = FALSE;	//po po�aczeniu zmienna CProtokol::m_bPolaczonoEth b�dzie ustawiona w w�tku CProtokol::WatekSluchajPortuEth() i b�dzie ustawiona w trakcie odpytania CAPLSNView::OnUpdatePolaczEth
		chErr = getProtokol().PolaczPort(ETHK, m_iNumerPortuETH, 0, m_strAdresIP, m_pWnd);
		break;
	
	default:	break;
	}
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Zamyka po�aczenie z portem komunikacyjnym
// zwraca: kod b��du
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
// Odbi�r wektor�w z danymi ramek i ich parsowanie
// zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
void CKomunikacja::OdebranoDaneETH()
{

	/*
	* Doda� w�tek sprawdzaj�cy czy przychodza ramki odebrane w  CProtokol przez dowolne ��cze
	* Doda� podzia� na telemetri� i ramki z odpowiedzi� - osobne w�tki?
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
// Pobiera ID, nazw� oraz numer IP z urz�dzenia przy pomocuy adresu rozg�oszeniowego, po to aby m�c prowadzi� dalsz� komunikacj� pinkt do punktu
// *chId - wska�nik na zwracane ID
// *strNazwa - wska�nik na nazw� urz�dzenia
// zwraca: kod b��du
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
// Ustawia ID, nazw� oraz numer IP dla BSP
// chId - ID, czyli adres komunikacyjny po UART
// *strNazwa - wska�nik na nazw� urz�dzenia
// *chIP - wska�nika na adres IP
// zwraca: kod b��du
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
// Wywo�uje w�tek analizuj�cy ramki polece�
// Zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::WatekDekodujRamkiPolecen(LPVOID pParam)
{
	m_bKoniecWatkuDekoderaPolecen = FALSE;
	return reinterpret_cast<CKomunikacja*>(pParam)->WlasciwyWatekDekodujRamkiPolecen();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// W�tek analizuj�cy ramki polece�
// Zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::WlasciwyWatekDekodujRamkiPolecen ()
{
	//_Ramka s_Ramka;
	//int n, m;
	//BOOL bMamyTo;
	//_sWron sWron;
	//CString strNazwa = L" ";
	//std::vector <_sWron>::iterator iter;	//iterator wektora wron�w


	while (!m_bKoniecWatkuDekoderaPolecen)
	{		
		WaitForSingleObject(getProtokol().m_hZdarzenieRamkaPolecenGotowa, INFINITE);		//czekaj na odbi�r zdekodowanej ramki danych

		/*for (n = 0; n < getProtokol().m_vRamkaPolecenia.size(); n++)
		{
			s_Ramka = getProtokol().m_vRamkaPolecenia[n];
			switch (s_Ramka.chPolecenie)
			{
			case PK_OK:	//przysz�a ramka OK
			case PK_BLAD:	//przysz�a ramka z kodem b��du
				//m_stPotwierdzenie.chTyp = s_Ramka.chPolecenie;
				//m_stPotwierdzenie.chKodBledu = s_Ramka.dane[0];
				//m_stPotwierdzenie.chParametr = s_Ramka.dane[1];
				break;

			case PK_POBIERZ_ID: //przysz�a ramka z identyfikatorem nowego BSP
				//m_strNazwa = L"";
				for (m = 0; m < s_Ramka.chRozmiar; m++)
					strNazwa.Insert(m, s_Ramka.dane[m]);

				//sprawd� czy w roju mamy ju� tego wrona
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
				//je�eli nie mamy to go dodaj
				/*if (bMamyTo == FALSE)
				{
					sWron.chAdres = s_Ramka.chAdrNadawcy;
					sWron.strNazwa = strNazwa;
					m_vRoj.push_back(sWron);
				}*/	
			/*	strNazwa = L"";
				m_chAdresAutopilota = s_Ramka.chAdrNadawcy;
				CKomunikacja::WyslijOK();		//wy�lij odpowied�				
				break;

			case PK_TELEMETRIA:
				
				break;

			default:	break;
			}
		}
		getProtokol().m_vRamkaPolecenia.clear();	//wyczy�� wektor*/
	}
	return ERR_OK;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wysy�a ramk� OK 
// zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::WyslijOK()
{
	uint8_t chRamka[ROZM_CIALA_RAMKI];

	getProtokol().PrzygotujRamke(m_chAdresAutopilota, ADRES_STACJI, PK_OK, NULL, 0, chRamka);
	return getProtokol().WyslijRamke(m_chTypPolaczenia, chRamka, ROZM_CIALA_RAMKI);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Wysy�a polecenie zrobienia zdj�cia
// zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
//uint8_t CKomunikacja::ZrobZdjecie(uint8_t chAdres, uint16_t sSzerokosc, uint16_t sWysokosc, uint16_t* sBuforZdjecia)
uint8_t CKomunikacja::ZrobZdjecie(uint16_t* sBuforZdjecia)
{
	//uint8_t chRamka[ROZM_CIALA_RAMKI + 5];
	uint8_t chDane[5];
	uint8_t x, chErr, chOdebrano;
	uint8_t chLicznikBledow = 0;
	uint32_t nPobrano, nRozmiarDanych;
	/*m_unia8_16.dane16 = sSzerokosc;
	chDane[0] = m_unia8_16.dane8[0];
	chDane[1] = m_unia8_16.dane8[1];
	m_unia8_16.dane16 = sWysokosc;
	chDane[2] = m_unia8_16.dane8[0];
	chDane[3] = m_unia8_16.dane8[1];*/

	chErr = getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_ZROB_ZDJECIE, chDane, 0, chDane, &chOdebrano);
	if (chErr == ERR_OK)
	{
		SetEvent(m_hZdarzeniePaczkaDanych);
		//sprawd� status wykonania zdj�cia
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

		//pobierz zdj�cie
		nPobrano = 0;
		nRozmiarDanych = (uint32_t)(480 * 320 * 2);
		while (nPobrano < nRozmiarDanych)
		{
			m_unia8_32.dane32 = nPobrano/4;	//offset pobieranych danych zdj�cia, /4 - konwersja z bajt�w s�owa 32/bit 
			for (x = 0; x < 4; x++)
				chDane[x] = m_unia8_32.dane8[x];

			//rozmiar paczki danych do pobrania
			if ((nRozmiarDanych - nPobrano) > ROZM_DANYCH_UART)
				chDane[4] = ROZM_DANYCH_UART;
			else
				chDane[4] = nRozmiarDanych - nPobrano;

			chErr = getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_POBIERZ_ZDJECIE, chDane, 5, (uint8_t*)(sBuforZdjecia + nPobrano/2), &chOdebrano);
			if (chErr == ERR_OK)
			{
				nPobrano += chOdebrano;
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
// Uruchaie polecenie inicjalizacji kamery resetujac j� i przywracaj�c nastawy domy�lne
// parametry: brak
// zwraca: kod b��du
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
// Wysy�a polecenie odczytu konfiguracji kamery
// parametry:
// [i] chAdres - adres autopilota
// [o] *chSzerWy i *chWysWy - wska�niki na szeroko�� i wysoko�� wyj�ciowa kamery skalowana przez 16
// [o] *chSzerWe i *chWysWe - wska�niki na szeroko�� i wysoko�� z jakiej zbiera i skaluje kamera, skalowana przez 16
// [o] *chTrybDiagn - wska�nik na kod trybu diagnostycznego
// [o] *chFlagi - zbi�r bit�w definiuj�cych funkcjonalno��: bit 0: odwr�� w poziomie, bit 1: odwr�� w pionie, bit 2: 1=zdjecie, 0=film
// zwraca: kod b��du
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
// Wysy�a polecenie zapisu konfiguracji kamery
// parametry:
// [i] chSzerWy i chWysWy - szeroko�� i wysoko�� wyj�ciowa kamery skalowana przez 16
// [i] chSzerWe i chWysWe - szeroko�� i wysoko�� z jakiej zbiera i skaluje kamera, skalowana przez 16
// [i] chTrybDiagn - kod trybu diagnostycznego
// [i] chFlagi - zbi�r bit�w definiuj�cych funkcjonalno��: bit 0: odwr�� w poziomie, bit 1: odwr�� w pionie, bit 2: 1=zdjecie, 0=film
// zwraca: kod b��du
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
// Wysy�a polecenie zapisu danych pod wskazany adres we flash. Optymalne jest programowanie co najmniej ca�ymi stronami po 16 s��w (32 bajty)
// parametry:
// [i] chAdresAPL - adres autopilota
// [i] nAdres - adres bezwzgl�dny pocz�tku obszaru pami�ci z zakresu 0x68000000..0x6A000000
// [i] *sDane - wska�nik na bufor z danymi do zaprogramowania
// [i] chRozmiar - ilo�� s��w do zapisu max 128
// zwraca: kod b��du
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
	chDaneWychodzace[2] = chRozmiar/2;		//liczba s��w 16-bitowych do zapisu

	for (uint8_t n = 0; n < chRozmiar; n++)
		chDaneWychodzace[n + 3] = chDane[n];
	
	return getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_ZAPISZ_BUFOR, chDaneWychodzace, chRozmiar+3, chDanePrzychodzace, &chOdebrano);	//timeout normalny
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wysy�a polecenie skasowania sektora pamieci flash o rozmiarze 128kB
// parametry:
// [i] chAdresAPL - adres autopilota
// [i] nAdresPamieci - adres bezwzgl�dny pocz�tku obszaru pami�ci z zakresu 0x68000000..0x6A000000
// // zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::SkasujSektorFlash(uint32_t nAdresPamieci)
{
	uint8_t chOdebrano = 0;

	m_unia8_32.dane32 = nAdresPamieci;
	for (uint8_t n = 0; n < 4; n++)
		m_chRamkaWych[n] = m_unia8_32.dane8[n];
	return getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_KASUJ_FLASH, m_chRamkaWych, 4, m_chRamkaPrzy, &chOdebrano, 2000);	//du�y timeout bo Sector Erase time = 1100 ms
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wysy�a polecenie zapisu danych z bufora pod wskazany adres we flash. 
// parametry:
// [i] nAdres - adres bezwzgl�dny pocz�tku obszaru pami�ci z zakresu 0x68000000..0x6A000000
// zwraca: kod b��du
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
// Wysy�a polecenie zapisu danych z bufora pod wskazany adres we flash. 
// parametry:
// [i] nAdres - adres bezwzgl�dny pocz�tku obszaru pami�ci z zakresu 0x68000000..0x6A000000
// zwraca: kod b��du
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
// Odczytuje z APL list� okres�w dla wszystkich zmiennych telemetrycznych
// parametry:
// [i] chOKres - wska�nik na tablic� z okresem
// [i] chRozmiar - rozmiar tablicy
// zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::CzytajOkresTelemetrii(uint16_t* sOKres, uint16_t sRozmiar)
{
	uint8_t chErr, chOdebrano;
	uint8_t chDaneWychodzace[2];
	uint8_t chDanePrzychodzace[ROZM_DANYCH_UART];
	uint8_t chLiczbaRamek = (uint8_t)ceil((float)sRozmiar / OKRESOW_TELEMETRII_W_RAMCE);
	uint16_t sOdebranoLacznie = 0;

	ASSERT(sRozmiar <= LICZBA_ZMIENNYCH_TELEMETRYCZNYCH);

	//w ramce nie zmieszcz� si� wszystkie 16-bitowe okresy, wiec na raz przesy�am maksymalnie OKRESOW_TELEMETRII_W_RAMCE (64)
	for (int n = 0; n < chLiczbaRamek; n++)
	{
		if ((sRozmiar - sOdebranoLacznie) > OKRESOW_TELEMETRII_W_RAMCE)
			chDaneWychodzace[0] = OKRESOW_TELEMETRII_W_RAMCE;
		else
			chDaneWychodzace[0] = sRozmiar - sOdebranoLacznie;			//rozmiar
		chDaneWychodzace[1] = n * OKRESOW_TELEMETRII_W_RAMCE;	//przesuni�cie,czyli czytaj od tego indeksu
		chErr = getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_CZYTAJ_OKRES_TELE, chDaneWychodzace, 2, chDanePrzychodzace, &chOdebrano);
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
// Zapisuje do APL list� okres�w dla zmiennych telemetrycznych w formacie m�odszy przodem
// Dane s� zaspisywane we Flash dost�pny dla CM7, zapsuje si� w paczkach 32 bajtowych, gdzie nag��wek paczki i suma kontrolna zajmuj� 2 bajty,
// wi�c do efektywnego u�ycia jest 30 bajt�w, czyli 15 okres�w. Dane nale�y wysy�ac paczkami o rozmiarze  podzielnym przez 15, tak aby 
// nie by�o problemu polegaj�cego na tym �e cz�� danych z ko�ca pierwszej i pocz�tku drugiej ramki mai�a by by� zapisana w jednej paczce.
// To bardzo utrudni�o by zarz�dzanie danymi, wi�c porz�dek trzeba zrobi� ju� tutaj wysy�ajac licz� danych podzieln� przez 15 czyli np 120 zmiennych -> 240 bajt�w
// parametry:
// [i] chOKres - wska�nik na tablic� z okresem
// [i] sRozmiar - rozmiar tablicy, czyli liczba okres�w do zapisania
// zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::ZapiszOkresTelemetrii(uint16_t *sOKres, uint16_t sRozmiar)
{
	uint8_t chErr, chOdebrano;
	uint8_t chDaneWychodzace[ROZM_DANYCH_UART];
	uint8_t chDanePrzychodzace[ROZM_DANYCH_UART];
	uint16_t sTemp;
	uint8_t chLiczbaRamek = (uint8_t)ceil(sRozmiar / OKRESOW_TELEMETRII_W_RAMCE);
	uint8_t chRozmiar;
	uint16_t sDoZapisu = sRozmiar;

	ASSERT(sRozmiar <= LICZBA_RAMEK_TELEMETR * OKRESOW_TELEMETRII_W_RAMCE);

	for (int n = 0; n < chLiczbaRamek; n++)
	{
		//na pocz�tku wy�lij indeks pierwszej zmiennej
		chDaneWychodzace[0] = (uint8_t) ((n* OKRESOW_TELEMETRII_W_RAMCE) & 0x00FF);
		chDaneWychodzace[1] = (uint8_t)(((n* OKRESOW_TELEMETRII_W_RAMCE) & 0xFF00) >> 8);

		//okre�l ilo�� danych do zapisu
		if (sDoZapisu > OKRESOW_TELEMETRII_W_RAMCE)
			chRozmiar = OKRESOW_TELEMETRII_W_RAMCE;
		else
			chRozmiar = (uint8_t)sDoZapisu;

		//potem wy�lij kolejne zmienne
		for (uint8_t m = 0; m < chRozmiar; m++)
		{
			sTemp = *(sOKres + m);
			chDaneWychodzace[2 * m + 2] = (uint8_t)(sTemp & 0x00FF);
			chDaneWychodzace[2 * m + 3] = (uint8_t)((sTemp & 0xFF00) >> 8);
		}
	}
	chErr = getProtokol().WyslijOdbierzRamke(m_chAdresAutopilota, ADRES_STACJI, PK_ZAPISZ_OKRES_TELE, chDaneWychodzace, 2*chRozmiar, chDanePrzychodzace, &chOdebrano);
	return chErr;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Zapisuje do FRAM w APL liczby uint8_t (max 4*ROZMIAR_ROZNE)
// parametry:
// [i] chDane - wska�nik na tablic� z danymi do zapisu
// [i] chRozmiar - rozmiar tablicy liczb
// [i] sAdres - adres we FRAM
// zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::ZapiszDaneU8FRAM(uint8_t* chDane, uint8_t chRozmiar, uint16_t sAdres)
{
	uint8_t chErr, chOdebrano;
	uint8_t chDaneWychodzace[3 + 4 * ROZMIAR_ROZNE];
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
// Zapisuje do FRAM w APL liczb� float (max ROZMIAR_ROZNE)
// parametry:
// [i] fDane - wska�nik na tablic� z danymi do zapisu
// [i] chRozmiar - rozmiar tablicy liczb
// [i] sAdres - adres we FRAM
// zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::ZapiszDaneFloatFRAM(float* fDane, uint8_t chRozmiar, uint16_t sAdres)
{
	uint8_t chErr, chOdebrano;
	uint8_t chDaneWychodzace[3+ 4 * ROZMIAR_ROZNE];
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
// Odsy�a potwierdzenie zaspisania danych lub jego brak
// parametry:
// [i] sAdres - adres we FRAM
// zwraca: kod b��du: ERROK lub ERR_PROCES_TRWA
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::PotwierdzZapisDanych(uint16_t sAdres)
{
	uint8_t chErr, chOdebrano;
	uint8_t chDaneWychodzace[3 + 4 * ROZMIAR_ROZNE];
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
// [i] *chDane - wska�nik na tablic� z danymi do zapisu
// [i] chRozmiar - rozmiar tablicy liczb
// [i] sAdres - adres we FRAM
// zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::ZapiszU8FRAM(uint8_t* chDane, uint8_t chRozmiar, uint16_t sAdres)
{
	uint8_t chLicznikProbPotw = LICZBA_PROB_ZANIM_ZGLOSI_BLAD;
	uint8_t chErr = ZapiszDaneU8FRAM(chDane, chRozmiar, sAdres);
	if (chErr == ERR_OK)
	{
		do  //czekaj na potwierdzenie zanim wy�le kolejne dane do zapisu
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
// Zapisuje do FRAM w APL liczb� float (max ROZMIAR_ROZNE) z potwierdzeniem zapisu
// parametry:
// [i] fDane - wska�nik na tablic� z danymi do zapisu
// [i] chRozmiar - rozmiar tablicy liczb
// [i] sAdres - adres we FRAM
// zwraca: kod b��du
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::ZapiszFloatFRAM(float* fDane, uint8_t chRozmiar, uint16_t sAdres)
{
	uint8_t chLicznikProbPotw = LICZBA_PROB_ZANIM_ZGLOSI_BLAD;
	uint8_t chErr = ZapiszDaneFloatFRAM(fDane, chRozmiar, sAdres);
	if (chErr == ERR_OK)
	{		
		do  //czekaj na potwierdzenie zanim wy�le kolejne dane do zapisu
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
// [i] chRozmiar - ilo�� liczb do odczytu
// [i] sAdres - adres we FRAM
// zwraca: kod b��du: ERR_OK lub ERR_ZLA_ILOSC_DANYCH
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::InicjujOdczytU8FRAM(uint8_t chRozmiar, uint16_t sAdres)
{
	uint8_t chErr, chOdebrano;
	uint8_t chDaneWychodzace[3];
	uint8_t chDanePrzychodzace[ROZM_DANYCH_UART];

	if (chRozmiar > ROZMIAR_ROZNE)
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
// [i] chRozmiar - ilo�� liczb do odczytu
// [i] sAdres - adres we FRAM
// zwraca: kod b��du: ERR_OK lub ERR_ZLA_ILOSC_DANYCH
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::InicjujOdczytFloatFRAM(uint8_t chRozmiar, uint16_t sAdres)
{
	uint8_t chErr, chOdebrano;
	uint8_t chDaneWychodzace[3];
	uint8_t chDanePrzychodzace[ROZM_DANYCH_UART];

	if (chRozmiar > ROZMIAR_ROZNE)
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
// [i] chDane - wska�nik na tablic� z danymi do odczytu
// [i] chRozmiar - ilo�� czwrek liczb
// [i] sAdres - adres we FRAM
// zwraca: kod b��du: ERR_OK lub ERR_HAL_BUSY je�eli nie s� jeszcze gotowe
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
// [i] fDane - wska�nik na tablic� z danymi do odczytu
// [i] chRozmiar - rozmiar tablicy liczb
// [i] sAdres - adres we FRAM
// zwraca: kod b��du: ERR_OK lub ERR_BRAK_POTWIERDZ je�eli nie s� jeszcze gotowe
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CKomunikacja::CzytajFloatFRAM(float* fDane, uint8_t chRozmiar, uint16_t sAdres)
{
	uint8_t chLicznikProbOdczytu = LICZBA_PROB_ZANIM_ZGLOSI_BLAD;
	uint8_t chDane[4 * ROZMIAR_ROZNE];
	uint8_t chErr = InicjujOdczytFloatFRAM(chRozmiar, sAdres);
	if (chErr == ERR_OK)
	{
		do
		{
			chErr = CzytajDaneU8FRAM(chDane, 4*chRozmiar);
			chLicznikProbOdczytu--;
		} while ((chErr != ERR_OK) && (chLicznikProbOdczytu));

		if (!chLicznikProbOdczytu)
			chErr = ERR_BRAK_POTWIERDZ;

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
// [i] chDane - wska�nik na tablic� z danymi do odczytu
// [i] chRozmiar - liczba czw�rek liczb
// [i] sAdres - adres we FRAM
// zwraca: kod b��du: ERR_OK lub ERR_BRAK_POTWIERDZ je�eli nie s� jeszcze gotowe
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
// Pakuje danne uint8 do float i w takiej postaci jest to wysy�ane do zapisania we FRMA ze wzgledu na uniwersalno��
// parametry:
// [i] *chDane - wska�nik na tablic� z danymi do spakowania
// [i] chRozmiar - rozmiar tablicy liczb uint8_t
// [o] *fData - wska�nik na tablic� danych float
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
// Rozpakowuje danne przesy�ana jako float do uint8_t
// parametry:
// [i] *fData - wska�nik na tablic� danych float
// [i] chRozmiar - rozmiar tablicy liczb uint8_t
// [o] *chDane - wska�nik na tablic� z danymi do spakowania
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
// Uruchamia ponown� konfiguracj� wejs� i wyjs� RC po zmianie zawarto�ci FRAM
// Abyt polecenie nie wykonywa�o si� w p�tli, po poleceniu trzeba wyda� inne polecenie zako�czone POL_NIC, np. Odczyt FRAM
// parametry: brak
// zwraca: kod b��du
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