
// APL-SNView.cpp: implementacja klasy CAPLSNView
//

#include "pch.h"
#include "framework.h"
// Element SHARED_HANDLERS można zdefiniować w projekcie ATL z implementacją podglądu, miniaturze
// procedury obsługi serializacji i filtrów wyszukiwania oraz umożliwia udostępnianie kodu dokumentu w tym projekcie.
#ifndef SHARED_HANDLERS
#include "APL-SN.h"
#endif

#include "APL-SNDoc.h"
#include "APL-SNView.h"
#include "KomunikatySieci.h"
#include "Errors.h"
#include <stdio.h>
#include <string.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAPLSNView

IMPLEMENT_DYNCREATE(CAPLSNView, CView)

BEGIN_MESSAGE_MAP(CAPLSNView, CView)
	// Standardowe polecenia drukowania
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CAPLSNView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_INPUT()
	ON_COMMAND(ID_KONFIG_PORT, &CAPLSNView::OnKonfigPort)
	ON_UPDATE_COMMAND_UI(ID_KONFIG_PORT, &CAPLSNView::OnUpdateKonfigPort)
	ON_COMMAND(ID_POLACZ_COM, &CAPLSNView::OnPolaczCom)
	ON_UPDATE_COMMAND_UI(ID_POLACZ_COM, &CAPLSNView::OnUpdatePolaczCom)
	ON_COMMAND(ID_ZROB_ZDJECIE, &CAPLSNView::OnZrobZdjecie)
	ON_UPDATE_COMMAND_UI(ID_ZROB_ZDJECIE, &CAPLSNView::OnUpdateZrobZdjecie)
	ON_COMMAND(ID_ZAPISZ_PAMIEC, &CAPLSNView::OnZapiszPamiec)
	ON_UPDATE_COMMAND_UI(ID_ZAPISZ_PAMIEC, &CAPLSNView::OnUpdateZapiszPamiec)
END_MESSAGE_MAP()

// Tworzenie/niszczenie obiektu CAPLSNView

CAPLSNView::CAPLSNView() noexcept
: m_bPolaczono(FALSE)
, m_chAdresAutopilota(2)
{
	
}


CAPLSNView::~CAPLSNView()
{
	if (m_bPolaczono)
		m_cKomunikacja.Rozlacz();
}


BOOL CAPLSNView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: zmodyfikuj klasę Window lub style w tym miejscu, modyfikując
	//  styl kaskadowy CREATESTRUCT
	uint8_t chErr;

	CzytajRejestr();
	//uint8_t m_chNumerIP[4];


	m_cKomunikacja.UstawRodzica(this);
	m_cKomunikacja.UstawAdresPortuETH(L"127.0.0.1");
	m_cKomunikacja.UstawNumerPortuETH(m_nNumerPortuEth);
	m_cKomunikacja.UstawTypPolaczenia(m_chTypPolaczenia + UART);
	m_cKomunikacja.UstawPredkoscPortuUART(m_nPredkoscPortuCom);
	m_cKomunikacja.UstawNumerPortuUART(m_chNumerPortuCom);
	

	chErr = m_cKomunikacja.Polacz(this);
	if (chErr == ERR_OK)
		m_bPolaczono = TRUE;
	else
		m_bPolaczono = FALSE;

	return CView::PreCreateWindow(cs);
	return TRUE;
}

// Rysowanie obiektu CAPLSNView

void CAPLSNView::OnDraw(CDC* pDC)
{
	uint32_t x, y;
	uint8_t chKolor[3];
	uint16_t sPix;
	COLORREF crKolor;
	CAPLSNDoc* pDoc = GetDocument();
	RECT prost;
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: tutaj dodaj kod rysowania danych natywnych
	if (pDoc->m_bZdjecieGotowe)
	{
		for (y = 0; y < 240; y++)
		{
			for (x = 0; x < 320; x++)
			{
				sPix = pDoc->m_sZdjecie[y * 320 + x];
				chKolor[0] = ((sPix & 0xF800) >> 11)*8;		//R
				chKolor[1] = ((sPix & 0x070E) >> 5)*4;		//G
				chKolor[2] = (sPix & 0x001F)*8;			//B
				crKolor = RGB(chKolor[0], chKolor[1], chKolor[2]);

				pDC->SetPixel(x, y, crKolor);
			}
		}
		pDoc->m_bZdjecieGotowe = FALSE;
	}

	//rysuj pasek postepu
	pDC->GetBoundsRect(&prost, DCB_RESET);
	float fPrzyrost = (float)prost.right  / m_sLiczbaFragmentowPaskaPostepu;

	prost.top = prost.bottom - 10;
	prost.right = (uint32_t)(m_sBiezacyStanPaskaPostepu * fPrzyrost);
	pDC->Rectangle(&prost);}


// Drukowanie obiektu CAPLSNView


void CAPLSNView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CAPLSNView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// domyślne przygotowanie
	return DoPreparePrinting(pInfo);
}

void CAPLSNView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: dodaj kolejne inicjowanie przed rozpoczęciem drukowania
}

void CAPLSNView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: dodaj czyszczenie po drukowaniu
}

void CAPLSNView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CAPLSNView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Diagnostyka klasy CAPLSNView

#ifdef _DEBUG
void CAPLSNView::AssertValid() const
{
	CView::AssertValid();
}

void CAPLSNView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAPLSNDoc* CAPLSNView::GetDocument() const // wbudowana jest wersja bez debugowania
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAPLSNDoc)));
	return (CAPLSNDoc*)m_pDocument;
}
#endif //_DEBUG


//połączenie sieciowe APL do okna widoku





// Procedury obsługi komunikatów CAPLSNView



///////////////////////////////////////////////////////////////////////////////////////////////////
// Odbiera komunikat WM_INPUT generowany przez zdarzenia klasy CGniazdoSieci i przekazuje sterowanie do klasy protokołu
// [i] nInputcode - identyfikator zdarzenia
// [i] hRawInput - wolne do wykorzystania
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::OnRawInput(UINT nInputcode, HRAWINPUT hRawInput)
{
	//char chBufor[1025];
	//int iRozmiar;

	CAPLSNDoc* pDoc = GetDocument();
	assert(pDoc);

	//iRozmiar = sizeof(chBufor);
	switch (nInputcode)
	{
	case ON_ACCEPT:		m_cKomunikacja.AkceptujPolaczenieETH();
		pDoc->SetTitle(L"Ustanowiono połączenie ETH");
		break;

	case ON_SEND:	//m_cKomunikacja.WyslanoDaneETH();
		pDoc->SetTitle(L"Wysłano dane");
		break;

	case ON_RECEIVE:	//m_cKomunikacja.OdebranoDaneETH();
		//pDoc->SetTitle(m_cKomunikacja.m_vRoj[0].strNazwa);		
		break;

	case ON_CLOSE:		
		pDoc->SetTitle(L"Zamknięto port ETH");
		break;
	}

}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Otwiera okno aktualiacji parametrów portów komunikacyjnych
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::OnKonfigPort()
{
	m_cKonfigPolacz.UstawNumerPortuCom(m_chNumerPortuCom);
	m_cKonfigPolacz.UstawPredkoscPortuCom(m_nPredkoscPortuCom);
	m_cKonfigPolacz.UstawNumerPortuEth(m_nNumerPortuEth);
	m_cKonfigPolacz.UstawAdresIP(m_chNumerIP);
	m_cKonfigPolacz.UstawTypPolaczenia(m_chTypPolaczenia);
	m_cKonfigPolacz.DoModal();
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Aktualizuje stan przycisku połącz eth w pasku narzędzi
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::OnUpdateKonfigPort(CCmdUI* pCmdUI)
{
	// TODO: Dodaj tutaj swój kod procedury obsługi polecenia uaktualnienia UI
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Łączy się z portem com / UART
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::OnPolaczCom()
{
	m_cKomunikacja.UstawTypPolaczenia(UART + m_chTypPolaczenia);
	m_cKomunikacja.UstawNumerPortuUART(m_chNumerPortuCom);
	m_cKomunikacja.UstawPredkoscPortuUART(m_nPredkoscPortuCom);
	if (m_cKomunikacja.CzyPolaczonoUart())
		m_cKomunikacja.Rozlacz();
	else
		m_cKomunikacja.Polacz(this);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Aktualizuje stan przycisku połącz com w pasku narzędzi
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::OnUpdatePolaczCom(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_cKomunikacja.CzyPolaczonoUart());
}




///////////////////////////////////////////////////////////////////////////////////////////////////
// wywyła polecenie zrobienia zdjecia
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::OnZrobZdjecie()
{
	uint8_t chErr;

	CAPLSNDoc* pDoc = GetDocument();
	assert(pDoc);

	uint32_t rozmiar = sizeof(pDoc->m_sZdjecie);
	//uruchom wątek aktualizujący pasek postępu pobierania zdjęcia
	m_bKoniecWatkuPaskaPostepu = FALSE;
	pWskWatkuPaskaPostepu = AfxBeginThread((AFX_THREADPROC)WatekRysujPasekPostepu, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);

	m_sLiczbaFragmentowPaskaPostepu = rozmiar / ROZM_DANYCH_UART;
	m_sBiezacyStanPaskaPostepu = 0;
	pDoc->m_bZdjecieGotowe = FALSE;
	//chErr = m_cKomunikacja.ZrobZdjecie(2, 320, 240, pDoc->m_sZdjecie);
	chErr = m_cKomunikacja.ZrobZdjecie(m_chAdresAutopilota, pDoc->m_sZdjecie);
	if (chErr == ERR_OK)
	{
		pDoc->m_bZdjecieGotowe = TRUE;
		m_bKoniecWatkuPaskaPostepu = TRUE;
		Invalidate(TRUE);
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Aktualizuje stan przycisku zrób zdjęcie w pasku narzędzi
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::OnUpdateZrobZdjecie(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_cKomunikacja.CzyPolaczonoUart());
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Wywołuje wątek rysujący pasek postępu
// Zwraca: kod błędu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CAPLSNView::WatekRysujPasekPostepu(LPVOID pParam)
{
	return reinterpret_cast<CAPLSNView*>(pParam)->WlasciwyWatekRysujPasekPostepu();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Wątek  rysujący pasek postępu
// Zwraca: kod błędu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CAPLSNView::WlasciwyWatekRysujPasekPostepu()
{
	uint32_t nErr;

	while (!m_bKoniecWatkuPaskaPostepu)
	{
		nErr = WaitForSingleObject(m_cKomunikacja.m_hZdarzeniePaczkaDanych, 200);
		if (nErr != WAIT_TIMEOUT)
		{
			m_sBiezacyStanPaskaPostepu++;
			this->Invalidate(TRUE);
			//Invalidate(TRUE);
		}
	}
	return ERR_OK;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Otwiera okno dialogowe Zapisz Pamięć Flash
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::OnZapiszPamiec()
{
	m_cDaneFlash.DoModal();
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Aktualizuje stan przycisku Zapisz Pamięć Flash w pasku narzędzi
// zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::OnUpdateZapiszPamiec(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_cKomunikacja.CzyPolaczonoUart());
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Odczytuje ustawienia z rejestru
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAPLSNView::CzytajRejestr()
{
	int x, y;
	DWORD dwWartosc;
	CString Str;
	wchar_t chNapis[50];
	HKEY hkSoftware, hkPitLab, hkAplSn, hkSettings;
	LONG result;
	DWORD dwDisp;
	DWORD dwRegType, dwRegSize;

	result = RegOpenKeyExW(HKEY_CURRENT_USER, L"Software", 0, KEY_ALL_ACCESS, &hkSoftware);
	if (result == ERROR_SUCCESS)
	{
		result = RegCreateKeyExW(hkSoftware, L"PitLab", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkPitLab, &dwDisp);
		if (result == ERROR_SUCCESS)
		{
			result = RegCreateKeyExW(hkPitLab, L"APL-SN", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkAplSn, &dwDisp);
			if (result == ERROR_SUCCESS)
			{
				result = RegCreateKeyExW(hkAplSn, L"Settings", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkSettings, &dwDisp);
				if (result == ERROR_SUCCESS)
				{
					result = RegQueryValueEx(hkSettings, L"NrPortuCOM", 0, &dwRegType, (LPBYTE)&dwWartosc, &dwRegSize);
					if (result == ERROR_SUCCESS)
					{
						m_chNumerPortuCom = (uint8_t)(dwWartosc & 0xFF);
					}
				}				
			}
		}
	}

	//odczytaj rejestr i ustaw pozycje z odczytaną prędkością portu COM
	result = RegOpenKeyExW(HKEY_CURRENT_USER, L"Software", 0, KEY_ALL_ACCESS, &hkSoftware);
	if (result == ERROR_SUCCESS)
	{
		result = RegCreateKeyExW(hkSoftware, L"PitLab", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkPitLab, &dwDisp);
		if (result == ERROR_SUCCESS)
		{
			result = RegCreateKeyExW(hkPitLab, L"APL-SN", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkAplSn, &dwDisp);
			if (result == ERROR_SUCCESS)
			{
				result = RegCreateKeyExW(hkAplSn, L"Settings", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkSettings, &dwDisp);
				if (result == ERROR_SUCCESS)
				{
					result = RegQueryValueEx(hkSettings, L"PredkoscCOM", 0, &dwRegType, (LPBYTE)&dwWartosc, &dwRegSize);
					if (result == ERROR_SUCCESS)
					{
						m_nPredkoscPortuCom = (uint32_t)(dwWartosc & 0xFFFFFFFF);
					}
				}
			}
		}
	}

	//Odczytaj z rejestru adres IP
	result = RegOpenKeyExW(HKEY_CURRENT_USER, L"Software", 0, KEY_ALL_ACCESS, &hkSoftware);
	if (result == ERROR_SUCCESS)
	{
		result = RegCreateKeyExW(hkSoftware, L"PitLab", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkPitLab, &dwDisp);
		if (result == ERROR_SUCCESS)
		{
			result = RegCreateKeyExW(hkPitLab, L"APL-SN", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkAplSn, &dwDisp);
			if (result == ERROR_SUCCESS)
			{
				result = RegCreateKeyExW(hkAplSn, L"Settings", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkSettings, &dwDisp);
				if (result == ERROR_SUCCESS)
				{
					//za pierwszym razem zwraca błąd ERROR_MORE_DATA sugerujący że bufor jest za mały ale powiększanie nie pomaga
					result = RegQueryValueExW(hkSettings, L"AdresIP", NULL, &dwRegType, (LPBYTE)&chNapis, &dwRegSize);
					result = RegQueryValueExW(hkSettings, L"AdresIP", NULL, &dwRegType, (LPBYTE)&chNapis, &dwRegSize);
					if (result == ERROR_SUCCESS)
					{
						//wchar_t chNapis[50] = { L"123.234.156.012" };
						wchar_t* pwNapis;

						pwNapis = chNapis;
						for (x = 0; x < 4; x++)
						{							
							m_chNumerIP[x] = _wtoi(pwNapis);
							pwNapis = wcschr(chNapis, L'.');							
							for (y = 0; y <= (pwNapis - chNapis); y++)
							{
								chNapis[y] = L' ';
							}
						}
					}
				}
			}
		}
	}

	//odczytaj numer portu ETH
	result = RegOpenKeyExW(HKEY_CURRENT_USER, L"Software", 0, KEY_ALL_ACCESS, &hkSoftware);
	if (result == ERROR_SUCCESS)
	{
		result = RegCreateKeyExW(hkSoftware, L"PitLab", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkPitLab, &dwDisp);
		if (result == ERROR_SUCCESS)
		{
			result = RegCreateKeyExW(hkPitLab, L"APL-SN", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkAplSn, &dwDisp);
			if (result == ERROR_SUCCESS)
			{
				result = RegCreateKeyExW(hkAplSn, L"Settings", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkSettings, &dwDisp);
				if (result == ERROR_SUCCESS)
				{
					result = RegQueryValueExW(hkSettings, L"PortETH", 0, &dwRegType, (LPBYTE)&dwWartosc, &dwRegSize);
					if (result == ERROR_SUCCESS)
					{
						m_nNumerPortuEth = (uint32_t)(dwWartosc & 0xFFFFFFFF);
					}
				}				
			}
		}
	}


	//ustaw widoczność aktywnych kontrolek
	result = RegOpenKeyExW(HKEY_CURRENT_USER, L"Software", 0, KEY_ALL_ACCESS, &hkSoftware);
	if (result == ERROR_SUCCESS)
	{
		result = RegCreateKeyExW(hkSoftware, L"PitLab", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkPitLab, &dwDisp);
		if (result == ERROR_SUCCESS)
		{
			result = RegCreateKeyExW(hkPitLab, L"APL-SN", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkAplSn, &dwDisp);
			if (result == ERROR_SUCCESS)
			{
				result = RegCreateKeyExW(hkAplSn, L"Settings", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkSettings, &dwDisp);
				if (result == ERROR_SUCCESS)
				{
					result = RegQueryValueExW(hkSettings, L"TypPortu", 0, &dwRegType, (LPBYTE)&dwWartosc, &dwRegSize);
					if (result == ERROR_SUCCESS)
					{
						m_chTypPolaczenia = (uint8_t)(dwWartosc & 0x1);						
					}
				}
			}
		}
	}
}


