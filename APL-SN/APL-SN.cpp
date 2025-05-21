
// APL-SN.cpp: definiuje zachowania klasy dla aplikacji.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "APL-SN.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "APL-SNDoc.h"
#include "APL-SNView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAPLSNApp

BEGIN_MESSAGE_MAP(CAPLSNApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CAPLSNApp::OnAppAbout)
	// Standardowe polecenia dokumentu oparte na pliku
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standardowe polecenie konfiguracji wydruku
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)

END_MESSAGE_MAP()


// Konstrukcja klasy CAPLSNApp

CAPLSNApp::CAPLSNApp() noexcept
{
	m_bHiColorIcons = TRUE;


	// obsługa menedżera ponownego uruchamiania
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART_ASPECTS;
#ifdef _MANAGED
	// Jeśli aplikacja została skompilowana z obsługą środowiska uruchomieniowego języka wspólnego (/clr):
	//     1) To dodatkowe ustawienie jest wymagane do poprawnego działania obsługi Menedżera ponownego uruchamiania.
	//     2) W projekcie musisz dodać odwołanie do elementu System.Windows.Forms, aby skompilować.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: Zastąp ciąg identyfikatora aplikacji poniżej za pomocą unikatowego ciągu identyfikatora; zalecane
	// format ciągu to CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("PitLab.APLSN2.v1_0"));

	// TODO: W tym miejscu dodaj kod konstruktora,
	// Umieść wszystkie znaczące inicjacje w InitInstance
}

// Jedyny obiekt CAPLSNApp

CAPLSNApp theApp;


// Inicjowanie klasy CAPLSNApp

BOOL CAPLSNApp::InitInstance()
{
	// Funkcja InitCommonControlsEx() jest wymagana w systemie Windows XP, jeśli aplikacja
	// manifest określa użycie pliku ComCtl32.dll w wersji 6 lub nowszej, który ma zostać użyty
	// style wizualne. W przeciwnym razie utworzenie jakiegokolwiek okna nie powiedzie się.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Ustaw ten element tak, aby uwzględniał wszystkie typowe klasy kontrolek, których chcesz użyć
	// w aplikacji.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// Zainicjuj biblioteki OLE
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	// Funkcja AfxInitRichEdit2() musi używać kontrolki RichEdit
	// AfxInitRichEdit2();

	// Inicjowanie standardowe
	// Jeśli nie używasz tych funkcji i chcesz zmniejszyć rozmiar
	// końcowego pliku wykonywalnego, usuń poniżej
	// określone procedury inicjowania, które nie są potrzebne
	// Zmień klucz rejestru, w którym są przechowywane ustawienia
	// TODO: zmodyfikuj ten ciąg, aby był poprawny
	// takie jak nazwa firmy lub organizacji
	SetRegistryKey(_T("PitLab"));
	LoadStdProfileSettings(4);  // Ładuj opcje ze standardowego pliku INI (włącznie z listą ostatnio używanych)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Rejestruj szablony dokumentu aplikacji. Szablony dokumentu
	//  służą jako połączenie między dokumentami, oknami ramek i widokami
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_APLSNTYPE,
		RUNTIME_CLASS(CAPLSNDoc),
		RUNTIME_CLASS(CChildFrame), // niestandardowa podrzędna ramka MDI
		RUNTIME_CLASS(CAPLSNView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// utwórz główne okno ramek MDI
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// wywołaj element DragAcceptFiles tylko wtedy, gdy występuje sufiks
	//  W aplikacji MDI należy to wykonać bezpośrednio po ustawieniu elementu m_pMainWnd
	// Włącz otwieranie w trybie przeciągnij i upuść
	m_pMainWnd->DragAcceptFiles();

	// Analizuj wiersz polecenia pod kątem standardowych poleceń powłoki, DDE i otwarcia pliku
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Włącz otwieranie za pomocą polecenia Execute DDE
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	// Polecenia wysyłania określone w wierszu polecenia. Zwraca wartość FALSE, jeśli
	// aplikacja została uruchomiona z parametrem /RegServer, /Register, /Unregserver lub /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// Zainicjowano okno główne, więc wyświetl je i zaktualizuj
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	//int nNumerPortu;
	//m_cObslugaRejestru.CzytajRejestrInt(L"NrPortuCOM", &nNumerPortu);
	return TRUE;
}

int CAPLSNApp::ExitInstance()
{
	//TODO: Obsłuż kolejne zasoby, które mogły zostać dodane
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// procedury obsługi komunikatów klasy CAPLSNApp


// Okno dialogowe CAboutDlg używane na potrzeby informacji o aplikacji

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// Dane okna dialogowego
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // obsługa DDX/DDV

// Implementacja
protected:
	DECLARE_MESSAGE_MAP()
public:

};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)


	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// Polecenie aplikacji uruchamiające okno dialogowe
void CAPLSNApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// Metody ładowania/zapisu dostosowania klasy CAPLSNApp

void CAPLSNApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CAPLSNApp::LoadCustomState()
{
}

void CAPLSNApp::SaveCustomState()
{
}

// procedury obsługi komunikatów klasy CAPLSNApp

CPortSzeregowy& CAPLSNApp::getPortSzeregowy()
{
	return m_cPortSzeregowy;
}

CPortSzeregowy& getPortSzeregowy()
{
	return theApp.getPortSzeregowy();
}



CProtokol& CAPLSNApp::getProtokol()
{
	return m_cProtokol;
}

CProtokol& getProtokol()
{
	return theApp.getProtokol();
}

