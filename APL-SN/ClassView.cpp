
#include "pch.h"
#include "framework.h"
#include "MainFrm.h"
#include "ClassView.h"
#include "Resource.h"
#include "APL-SN.h"
#include "CkonfigTelemetrii.h"
#include "Komunikacja.h"
#include "Errors.h"
#include "Protokol.h"

class CClassViewMenuButton : public CMFCToolBarMenuButton
{
	friend class CClassView;

	DECLARE_SERIAL(CClassViewMenuButton)

public:
	CClassViewMenuButton(HMENU hMenu = nullptr) noexcept : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
	{
		pImages = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		pImages->EndDrawImage(ds);
	}
};

IMPLEMENT_SERIAL(CClassViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// Tworzenie/niszczenie
//////////////////////////////////////////////////////////////////////

CClassView::CClassView() noexcept
: m_bKoniecWatkuCzekaniaNaZmianePolaczenia(FALSE)
{
	m_nCurrSort = ID_SORTING_GROUPBYTYPE;	
	pWskWatkuCzekaniaNaPolaczenie = AfxBeginThread((AFX_THREADPROC)WatekCzekajNaZmianePolaczenia, this, THREAD_PRIORITY_ABOVE_NORMAL, 0, 0, NULL);
}

CClassView::~CClassView()
{
	m_bKoniecWatkuCzekaniaNaZmianePolaczenia = TRUE;
	WaitForSingleObject(pWskWatkuCzekaniaNaPolaczenie, INFINITE);
}

BEGIN_MESSAGE_MAP(CClassView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)
	ON_COMMAND(ID_CLASS_ADD_MEMBER_VARIABLE, OnClassAddMemberVariable)
	ON_COMMAND(ID_CLASS_DEFINITION, OnClassDefinition)
	ON_COMMAND(ID_CLASS_PROPERTIES, OnClassProperties)
	ON_COMMAND(ID_NEW_FOLDER, OnNewFolder)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	//ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
	//ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)
//	ON_WM_SETCURSOR()
//ON_WM_RBUTTONDBLCLK()
//ON_WM_LBUTTONDBLCLK()
//ON_WM_USERCHANGED()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Procedury obsługi komunikatów CClassView

int CClassView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Utwórz widoki:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndClassView.Create(dwViewStyle, rectDummy, this, 2))
	{
		TRACE0("Nie można utworzyć widoku klasy\n");
		return -1;      // nie można utworzyć
	}

	// Ładuj obrazy:
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* Jest zablokowany */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// Wszystkie polecenia będą kierowane za pośrednictwem tej kontrolki, a nie ramki nadrzędnej:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	//CMenu menuSort;
	//menuSort.LoadMenu(IDR_POPUP_SORT);

	//m_wndToolBar.ReplaceButton(ID_SORT_MENU, CClassViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

	CClassViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != nullptr)
	{
		pButton->m_bText = FALSE;
		pButton->m_bImage = TRUE;
		pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
		pButton->SetMessageWnd(this);
	}

	// Wprowadź dane statycznego widoku drzewa (fikcyjny kod, nic szczególnego)
	//FillClassView();	
	return 0;
}

void CClassView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CClassView::FillClassView()
{
	HTREEITEM hWron;
	HTREEITEM hZasob;
	HTREEITEM hZmienna;
	uint8_t chOkresTele;

	//Używam tej funkcji do przerysowania okna, więc zanim wstawi nowe drzewo to usuń wszystkie istniejące wcześniej
	m_wndClassView.DeleteAllItems();

	HTREEITEM hRoj = m_wndClassView.InsertItem(_T("Rój"), 0, 0);

	//wstaw jednego wirtualnego wrona aby był nawet bez połączenia z rzeczywistym
	CWron cWron;
	cWron.m_chAdres = 5;
	cWron.m_strNazwa = " Wron wirtualny";
	getKomunikacja().m_cRoj.vWron.push_back(cWron);
	size_t LiczbaWronow = getKomunikacja().m_cRoj.vWron.size();

	//wstaw do drzewa wszystkie wrony
	for (int w = 0; w < LiczbaWronow; w++)
	{
		m_wndClassView.SetItemState(hRoj, TVIS_BOLD, TVIS_BOLD);	//pogrub rój jeżeli są w nim wrony

		hWron = m_wndClassView.InsertItem(getKomunikacja().m_cRoj.vWron[w].m_strNazwa, 0, 0, hRoj);
		if (getKomunikacja().m_cRoj.vWron[w].m_chPolaczony)
			m_wndClassView.SetItemState(hWron, TVIS_BOLD, TVIS_BOLD);	//pogrubiony
		else
			m_wndClassView.SetItemState(hWron, TVIS_DROPHILITED, TVIS_DROPHILITED);	//cieniowany
		

		hZasob = m_wndClassView.InsertItem(_T("Telemetria"), 0, 0, hWron);
			
		//wstaw do gałezi IMU wszystkie zmienne telemetryczne od akcelerometrów do końca magnetometrów
		hZmienna = m_wndClassView.InsertItem(_T("IMU"), 2, 2, hZasob);
		for (uint8_t n = TELEID_AKCEL1X; n <= TELEID_MAGNE3Z; n++)
		{
			chOkresTele = getKomunikacja().m_cRoj.vWron[w].m_chOkresTelemetrii[n];
			if (chOkresTele)
				m_wndClassView.WstawZmiennaTelemetrii(n, chOkresTele, getKomunikacja().m_strNazwyZmiennychTele[n], 3, hZmienna);
		}

		hZmienna = m_wndClassView.InsertItem(_T("AHRS"), 2, 2, hZasob);
		for (uint8_t n = TELEID_KAT_IMU1X; n <= TELEID_KAT_ZYRO1Z; n++)
		{
			chOkresTele = getKomunikacja().m_cRoj.vWron[w].m_chOkresTelemetrii[n];
			if (chOkresTele)
				m_wndClassView.WstawZmiennaTelemetrii(n, chOkresTele, getKomunikacja().m_strNazwyZmiennychTele[n], 3, hZmienna);
		}

		hZmienna = m_wndClassView.InsertItem(_T("Barometryczne"), 2, 2, hZasob);
		for (uint8_t n = TELEID_CISBEZW1; n <= TELEID_TEMPCISR2; n++)
		{
			chOkresTele = getKomunikacja().m_cRoj.vWron[w].m_chOkresTelemetrii[n];
			if (chOkresTele)
				m_wndClassView.WstawZmiennaTelemetrii(n, chOkresTele, getKomunikacja().m_strNazwyZmiennychTele[n], 3, hZmienna);
		}		
		//m_wndClassView.Expand(hZmienna, TVE_EXPAND);

		hZasob = m_wndClassView.InsertItem(_T("Zmiene Logu"), 0, 0, hWron);
		m_wndClassView.Expand(hZasob, TVE_EXPAND);
		m_wndClassView.Expand(hWron, TVE_EXPAND);	//rozwiń zasoby wrona
			
	}
	m_wndClassView.Expand(hRoj, TVE_EXPAND);	//rozwiń rój
}



void CClassView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CString strNazwaZmiennej;
	//int nIdZmiennej;
	uint8_t chId;
	uint8_t chOkres;
	//CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndClassView;
	CDrzewoTelemetrii* pDrzewoTele = (CDrzewoTelemetrii*)&m_wndClassView;
	ASSERT_VALID(pDrzewoTele);

	if (pWnd != pDrzewoTele)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// Zaznacz kliknięty element:
		CPoint ptTree = point;
		
		pDrzewoTele->ScreenToClient(&ptTree);

		UINT flags = 0;
		
		HTREEITEM hTreeItem = pDrzewoTele->HitTest(ptTree, &flags);
		if (hTreeItem != nullptr)
		{
			pDrzewoTele->SelectItem(hTreeItem);
			strNazwaZmiennej = pDrzewoTele->GetItemText(hTreeItem);
			chId = pDrzewoTele->PobierzId(strNazwaZmiennej);
			chOkres = pDrzewoTele->PobierzOkres(chId);
		}
	}

	pDrzewoTele->SetFocus();
	CKonfigTelemetrii m_cKonfigTelemetrii;

	//m_cKonfigTelemetrii.UstawNazweZmiennej(strNazwaZmiennej);
	//m_cKonfigTelemetrii.UstawOkresTelem(chOkres);
	//m_cKonfigTelemetrii.UstawIDZmiennej(chId);


	m_nIndeksWrona = 0;	//trzeba wymyśleć jak to ustawić
	m_cKonfigTelemetrii.UstawIndeksDronaWRoju(m_nIndeksWrona);
	INT_PTR nKodBledu = m_cKonfigTelemetrii.DoModal();
	if (nKodBledu == IDOK)
		FillClassView();

	/*CMenu menu;
	menu.LoadMenu(IDR_POPUP_SORT);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}*/
}

void CClassView::AdjustLayout()
{
	if (GetSafeHwnd() == nullptr)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndClassView.SetWindowPos(nullptr, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL CClassView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void CClassView::OnClassAddMemberFunction()
{
	AfxMessageBox(_T("Dodaj funkcję członkowską..."));
}

void CClassView::OnClassAddMemberVariable()
{
	// TODO: dodaj tutaj kod procedury obsługi poleceń
}

void CClassView::OnClassDefinition()
{
	// TODO: dodaj tutaj kod procedury obsługi poleceń
}

void CClassView::OnClassProperties()
{
	// TODO: dodaj tutaj kod procedury obsługi poleceń
}

void CClassView::OnNewFolder()
{
	AfxMessageBox(_T("Nowy folder..."));
}

//uruchamia się przy zmienianie szerokosci panelu
void CClassView::OnPaint()
{
	CPaintDC dc(this); // kontekst urządzenia dotyczący malowania

	CRect rectTree;
	m_wndClassView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}


//Aktywacja paska narzędziowego w widoku roju
void CClassView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndClassView.SetFocus();
}

void CClassView::OnChangeVisualStyle()
{
	m_ClassViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Nie można załadować mapy bitowej: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_ClassViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_ClassViewImages.Add(&bmp, RGB(255, 0, 0));

	m_wndClassView.SetImageList(&m_ClassViewImages, TVSIL_NORMAL);

	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* Zablokowano */);
}

/****************************************************************************************************************************************/
//Konstruktor
CDrzewoTelemetrii::CDrzewoTelemetrii() noexcept
	: m_nIndeksWrona(0)
{

}

//destruktor
CDrzewoTelemetrii::~CDrzewoTelemetrii()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Funkcja rozszerza działanie standardowej funkcji InsertItem() przez dodanie okresu i indeksu zmiennej oraz wyświetlenie finalnej częstotliwości</summary>
/// <param name="chId"></param> - identyfikator zmiennej taki jak w APL3
/// <param name="Okres"></param> - okres logowania w wielokrotności KWANT_CZASU
/// <param name="lpszItem"></param> - podstawowa nazwa zmiennej
/// <param name="hParent"></param> - rodzic tej gałęzi drzewa
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDrzewoTelemetrii::WstawZmiennaTelemetrii(uint8_t chId, uint8_t Okres, LPCTSTR lpszItem, int nObrazek, HTREEITEM hParent)
{
	if (Okres == TEMETETRIA_WYLACZONA)
		swprintf(stZmienne[chId].tchNazwa, DLUGOSC_NAZWY_ZMIENNEJ_TELEMETRII, _T("%s - wyłącz."), lpszItem);
	else
		swprintf(stZmienne[chId].tchNazwa, DLUGOSC_NAZWY_ZMIENNEJ_TELEMETRII, _T("%s - %.1f Hz"), lpszItem, MAX_CZESTOTLIWOSC_TELEMETRII / (Okres + 1));
	InsertItem(stZmienne[chId].tchNazwa, nObrazek, nObrazek+1, hParent);
	stZmienne[chId].m_chOkresTelemetrii = Okres;
}



uint8_t CDrzewoTelemetrii::PobierzOkres(uint8_t chId)
{
	return  stZmienne[chId].m_chOkresTelemetrii;
}


void CDrzewoTelemetrii::UstawOkres(uint8_t chId, uint8_t chOkres)
{
	TCHAR tchNazwa[10];
	int m, n, nDlugosc;
	//stZmienne[chId].m_chOkresTelemetrii = chOkres;

	//znajdź pozycję myślinika
	for (n = 0; n < DLUGOSC_NAZWY_ZMIENNEJ_TELEMETRII; n++)
	{
		if (stZmienne[chId].tchNazwa[n] == '-')
			break;
	}

	//wygeneruj napis nowej częstotliwości
	if (chOkres == TEMETETRIA_WYLACZONA)
		nDlugosc = swprintf(tchNazwa, 10, _T(" wyłącz."));
	else
		nDlugosc = swprintf(tchNazwa, 10, _T(" %.1f Hz"), MAX_CZESTOTLIWOSC_TELEMETRII / (chOkres + 1));

	//wstaw nowy napis do starego
	for (m=0; m < nDlugosc; m++)
		stZmienne[chId].tchNazwa[m + n] = tchNazwa[m];
}



uint8_t CDrzewoTelemetrii::PobierzId(CString strNazwa)
{
	//TCHAR tchNazwa[DLUGOSC_NAZWY_ZMIENNEJ_TELEMETRII];

	
	for (uint8_t n = 0; n < LICZBA_ZMIENNYCH_TELEMETRYCZNYCH; n++)
	{
		if (stZmienne[n].tchNazwa == strNazwa)
			return n;
	}
	return 0;
}

//CDrzewoTelemetrii::GetOleControlSite()


///////////////////////////////////////////////////////////////////////////////////////////////////
// Wywołuje wątek czekający na połączenie
// Zwraca: kod błędu
///////////////////////////////////////////////////////////////////////////////////////////////////
UINT CClassView::WatekCzekajNaZmianePolaczenia(LPVOID pParam)
{
	
	return reinterpret_cast<CClassView*>(pParam)->WlasciwyWatekCzekajNaZmianePolaczenia();
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Wątek uruchamiający odświezanie wykresu po przyjściu telemetrii
// Zwraca: kod błędu
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CClassView::WlasciwyWatekCzekajNaZmianePolaczenia()
{
	while (!m_bKoniecWatkuCzekaniaNaZmianePolaczenia)
	{
		if (this)
		{
			uint32_t nErr = WaitForSingleObject(getKomunikacja().m_hZdarzenieZmianaPolaczeniaWrona, INFINITE);
			if (nErr != WAIT_TIMEOUT)
			{
				FillClassView();
				Invalidate();
			}
		}
	}
	return ERR_OK;
}


//void CClassView::OnUserChanged()
//{
//	// TODO: Dodaj tutaj swój kod procedury obsługi komunikatów i/lub wywołaj domyślny
//
//	CDockablePane::OnUserChanged();
//}
BEGIN_MESSAGE_MAP(CDrzewoTelemetrii, CViewTree)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()


void CDrzewoTelemetrii::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: Dodaj tutaj swój kod procedury obsługi komunikatów
}
