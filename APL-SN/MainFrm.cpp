﻿
// MainFrm.cpp: implementacja klasy CMainFrame
//

#include "pch.h"
#include "framework.h"
#include "APL-SN.h"
#include "MainFrm.h"
#include "UstawieniaKameryDlg.h"
#include "CKonfigTelemetrii.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame
static const TCHAR APP_VER[] = _T("1.0.1");

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_WM_SETTINGCHANGE()

	ON_COMMAND(ID_USTAW_PARKAMERY, &CMainFrame::OnUstawParkamery)
	ON_COMMAND(ID_BUT_KONF_TELEMETRII, &CMainFrame::OnButKonfTelemetrii)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // wskaźnik stanu wiersza
	//ID_INDICATOR_POLACZ,
	ID_INDICATOR_CAPS,
	//ID_INDICATOR_NUM,
	//ID_INDICATOR_SCRL,	
};

// Tworzenie/niszczenie obiektu CMainFrame

CMainFrame::CMainFrame() noexcept
: m_bPolaczono(FALSE)
{
	// TODO: dodaj tutaj kod inicjowania elementu członkowskiego
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // inne dostępne style...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // ustaw na wartość FALSE, aby umieścić przycisk zamknięcia po prawej stronie obszaru karty
	mdiTabParams.m_bTabIcons = FALSE;    // ustaw na wartość TRUE, aby włączyć ikony dokumentu na kartach MDI
	mdiTabParams.m_bAutoColor = TRUE;    // ustaw na wartość FALSE, aby wyłączyć automatyczne kolorowanie kart MDI
	mdiTabParams.m_bDocumentMenu = TRUE; // włącz menu dokumentu na prawej krawędzi obszaru karty
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Nie można utworzyć paska menu\n");
		return -1;      // nie można utworzyć
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// blokuj ustawienie fokusu na pasku menu przy aktywacji
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("Nie można utworzyć paska narzędzi\n");
		return -1;      // nie można utworzyć
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// Zezwalaj na operacje paska narzędzi zdefiniowane przez użytkownika:
	InitUserToolbars(nullptr, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Nie można utworzyć paska stanu\n");
		return -1;      // nie można utworzyć
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: Usuń te pięć wierszy, jeśli nie chcesz, aby można było dokować pasek narzędzi i pasek menu
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);


	// włącz zachowanie okna dokującego w stylu programu Visual Studio 2005
	CDockingManager::SetDockingMode(DT_SMART);
	// włącz zachowanie automatycznego ukrywania okna dokującego w stylu programu Visual Studio 2005
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Ładuj obraz pozycji menu (nieumieszczony na żadnym ze standardowych pasków narzędzi):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// utwórz okna dokujące
	if (!CreateDockingWindows())
	{
		TRACE0("Nie można utworzyć okien dokujących\n");
		return -1;
	}

	m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndClassView);
	CDockablePane* pTabbedBar = nullptr;
	//m_wndClassView.AttachToTabWnd(&m_wndFileView, DM_SHOW, TRUE, &pTabbedBar);
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);
	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProperties);


	// ustaw menedżera obiektów wizualnych pod kątem rysowania wszystkich elementów interfejsu użytkownika
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));

	// Włącz rozszerzone okno dialogowe zarządzania oknami
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// Włącz zastąpienie paska narzędzi i menu okna dokującego
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// włącz szybkie dostosowywanie pasków narzędzi (Alt + przeciągnięcie)
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == nullptr)
	{
		// ładuj obrazy paska narzędzi zdefiniowane przez użytkownika
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// Zamień kolejność nazwy dokumentu i nazwy aplikacji na pasku tytułu okna. Ten
	// zwiększa użyteczność paska zadań, ponieważ razem z miniaturą jest widoczna nazwa dokumentu.
	ModifyStyle(0, FWS_PREFIXTITLE);

	m_bPolaczono = FALSE;
	//int NrPortu;
	//m_cObslugaRejestru.CzytajRejestrInt(L"NrPortuCOM", &NrPortu);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: zmodyfikuj klasę Window lub style w tym miejscu, modyfikując
	//  styl kaskadowy CREATESTRUCT

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// Utwórz widok klasy - tutaj siedzi konfiguracja telemetrii
	CString strClassView;
	bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	ASSERT(bNameValid);
	if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Nie można utworzyć okna widoku klasy\n");
		return FALSE; // utworzenie nie powiodło się
	}

	// Utwórz okno danych wyjściowych
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("Nie można utworzyć okna danych wyjściowych\n");
		return FALSE; // utworzenie nie powiodło się
	}

	// Utwórz okno właściwości
	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Nie można utworzyć okna właściwości\n");
		return FALSE; // utworzenie nie powiodło się
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	//HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	//m_wndFileView.SetIcon(hFileViewIcon, FALSE);

	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndClassView.SetIcon(hClassViewIcon, FALSE);

	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

	UpdateMDITabbedBarsIcons();
}

// Diagnostyka klasy CMainFrame

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// Procedury obsługi komunikatów CMainFrame

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();

}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* skanuj menu */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CMDIFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext)
{
	// klasa bazowa wykonuje rzeczywistą pracę

	if (!CMDIFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// włącz przycisk dostosowania dla wszystkich pasków narzędzi użytkownika
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != nullptr)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CMDIFrameWndEx::OnSettingChange(uFlags, lpszSection);
	m_wndOutput.UpdateFonts();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Okno dialogowe z ustawieniami kamery
// Zwraca: nic
///////////////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUstawParkamery()
{
	CUstawieniaKameryDlg dlg;
	dlg.DoModal();
}


void CMainFrame::OnButKonfTelemetrii()
{
	// TODO: Dodaj tutaj swój kod procedury obsługi polecenia
	CKonfigTelemetrii dlg;
	dlg.DoModal();
}
