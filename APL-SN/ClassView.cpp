
#include "pch.h"
#include "framework.h"
#include "MainFrm.h"
#include "ClassView.h"
#include "Resource.h"
#include "APL-SN.h"
#include "CkonfigTelemetrii.h"
#include "definicje_telemetrii.h"
#include "Komunikacja.h"

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
{
	m_nCurrSort = ID_SORTING_GROUPBYTYPE;
	
}

CClassView::~CClassView()
{
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
	ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)
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
	FillClassView();

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
	CString strNazwa;

	HTREEITEM hRoj = m_wndClassView.InsertItem(_T("Rój"), 0, 0);
	m_wndClassView.SetItemState(hRoj, TVIS_BOLD, TVIS_BOLD);	

	CWron cWron;
	cWron.m_chAdres = 5;

	getKomunikacja().m_cRoj.vWron.push_back(cWron);
	cWron.m_chAdres = 6;
	getKomunikacja().m_cRoj.vWron.push_back(cWron);
	cWron.m_chAdres = 7;
	getKomunikacja().m_cRoj.vWron.push_back(cWron);
	
	//wstaw do drzewa wszystkie wrony
	for (int n = 0; n < getKomunikacja().m_cRoj.vWron.size(); n++)
	{
		

		hWron = m_wndClassView.InsertItem(strNazwa, 0, 0, hRoj);
		m_wndClassView.SetItemState(hWron, TVIS_BOLD, TVIS_BOLD);			

		hZasob = m_wndClassView.InsertItem(_T("Telemetria"), 0, 0, hWron);
			
		HTREEITEM hZmienna = m_wndClassView.InsertItem(_T("Akcelerometr"), 2, 2, hZasob);
		m_wndClassView.WstawZmiennaTelemetrii(0x00, 19, _T("Oś X1"), 3, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x02, 19, _T("Oś Y1"), 3, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x03, 19, _T("Oś Z1"), 3, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x04, 255, _T("Oś X2"), 5, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x05, 255, _T("Oś Y2"), 5, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x06, 255, _T("Oś Z2"), 5, hZmienna);
		//m_wndClassView.Expand(hZmienna, TVE_EXPAND);

		hZmienna = m_wndClassView.InsertItem(_T("Żyroskop"), 2, 2, hZasob);
		m_wndClassView.WstawZmiennaTelemetrii(0x07, 0, _T("Predk. kąt. P1"), 3, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x08, 1, _T("Predk. kąt. Q1"), 3, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x09, 2, _T("Predk. kąt. R1"), 3, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x0A, 3, _T("Predk. kąt. P2"), 5, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x0B, 4, _T("Predk. kąt. Q2"), 5, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x0C, 5, _T("Predk. kąt. R2"), 5, hZmienna);
		//m_wndClassView.Expand(hZmienna, TVE_EXPAND);

		hZmienna = m_wndClassView.InsertItem(_T("Magnetometr"), 2, 2, hZasob);
		m_wndClassView.WstawZmiennaTelemetrii(0x0D, 6, _T("Pole magn. X1"), 3, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x0E, 7, _T("Pole magn. Y1"), 3, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x0F, 8, _T("Pole magn. Z1"), 3, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x10, 9, _T("Pole magn. X2"), 5, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x11, 10, _T("Pole magn. Y2"), 5, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x12, 11, _T("Pole magn. Z2"), 5, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x13, 12, _T("Pole magn. X3"), 3, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x14, 13, _T("Pole magn. Y3"), 3, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x15, 14, _T("Pole magn. Z3"), 3, hZmienna);
		//m_wndClassView.Expand(hZmienna, TVE_EXPAND);

		hZmienna = m_wndClassView.InsertItem(_T("AHRS"), 2, 2, hZasob);
		m_wndClassView.WstawZmiennaTelemetrii(0x16, 255, _T("Trygono. Phi"), 3, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x17, 255, _T("Trygono. Theta"), 3, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x18, 255, _T("Trygono. Psi"), 3, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x19, 255, _T("Kwatern. Phi"), 5, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x1A, 255, _T("Kwatern. Theta"), 5, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x1B, 255, _T("Kwatern. Psi"), 5, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x1C, 255, _T("Akcel. Phi"), 3, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x1D, 255, _T("Akcel. Theta"), 3, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x1E, 255, _T("Akcel. Psi"), 3, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x1F, 255, _T("Żyro. Phi"), 5, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x20, 255, _T("Żyro. Theta"), 5, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x21, 255, _T("Żyro. Psi"), 5, hZmienna);
		//m_wndClassView.Expand(hZmienna, TVE_EXPAND);

		hZmienna = m_wndClassView.InsertItem(_T("Barometryczne"), 2, 2, hZasob);
		m_wndClassView.WstawZmiennaTelemetrii(0x07, 255, _T("Cisn. bzwzgl. 1"), 3, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x07, 255, _T("Cisn. bzwzgl. 2"), 3, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x07, 255, _T("Wysokosć AGL 1"), 5, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x07, 255, _T("Wysokosć AGL 2"), 5, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x07, 255, _T("Cisn. różn. 1"), 3, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x07, 255, _T("Cisn. różn. 2"), 3, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x07, 255, _T("Prędkość IAS 1"), 5, hZmienna);
		m_wndClassView.WstawZmiennaTelemetrii(0x07, 255, _T("Prędkość IAS 2"), 5, hZmienna);
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

	m_cKonfigTelemetrii.UstawNazweZmiennej(strNazwaZmiennej);
	m_cKonfigTelemetrii.UstawOkresTelem(chOkres);
	m_cKonfigTelemetrii.UstawIDZmiennej(chId);
	m_cKonfigTelemetrii.DoModal();
	//chOkres = m_cKonfigTelemetrii.PobierzOkresTelem();	//pobierz nową wartość okresu
	//pDrzewoTele->UstawOkres(chId, chOkres);
	Invalidate();

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

void CClassView::OnSort(UINT id)
{
	if (m_nCurrSort == id)
	{
		return;
	}

	m_nCurrSort = id;

	CClassViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != nullptr)
	{
		pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
		m_wndToolBar.Invalidate();
		m_wndToolBar.UpdateWindow();
	}
}

void CClassView::OnUpdateSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrSort);
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