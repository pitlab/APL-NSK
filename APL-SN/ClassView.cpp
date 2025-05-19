
#include "pch.h"
#include "framework.h"
#include "MainFrm.h"
#include "ClassView.h"
#include "Resource.h"
#include "APL-SN.h"
#include "CkonfigTelemetrii.h"

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
	HTREEITEM hRoot = m_wndClassView.InsertItem(_T("Telemetria"), 0, 0);
	m_wndClassView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
	m_wndClassView.Expand(hRoot, TVE_EXPAND);

	HTREEITEM hClass = m_wndClassView.InsertItem(_T("Akcelerometr"), 2, 2, hRoot);
	m_wndClassView.InsertItem(_T("Oś X1"), 3, 4, hClass, (HTREEITEM)0x1000);
	m_wndClassView.InsertItem(_T("Oś Y1"), 3, 4, hClass, (HTREEITEM)0x2000);
	m_wndClassView.InsertItem(_T("Oś Z1"), 3, 4, hClass, (HTREEITEM)0x3000);
	m_wndClassView.InsertItem(_T("Oś X2"), 5, 6, hClass, (HTREEITEM)0x4000);
	m_wndClassView.InsertItem(_T("Oś Y2"), 5, 6, hClass, (HTREEITEM)0x5000);
	m_wndClassView.InsertItem(_T("Oś Z2"), 5, 6, hClass, (HTREEITEM)0x6000);
	m_wndClassView.Expand(hClass, TVE_EXPAND);

	hClass = m_wndClassView.InsertItem(_T("Żyroskop"), 2, 2, hRoot);
	m_wndClassView.InsertItem(_T("Oś X1"), 3, 4, hClass);
	m_wndClassView.InsertItem(_T("Oś Y1"), 3, 4, hClass);
	m_wndClassView.InsertItem(_T("Oś Z1"), 3, 4, hClass);
	m_wndClassView.InsertItem(_T("Oś X2"), 5, 6, hClass);
	m_wndClassView.InsertItem(_T("Oś Y2"), 5, 6, hClass);
	m_wndClassView.InsertItem(_T("Oś Z2"), 5, 6, hClass);
	m_wndClassView.Expand(hClass, TVE_EXPAND);
	
	hClass = m_wndClassView.InsertItem(_T("Magnetometr"), 2, 2, hRoot);
	m_wndClassView.InsertItem(_T("Oś X1"), 3, 4, hClass);
	m_wndClassView.InsertItem(_T("Oś Y1"), 3, 4, hClass);
	m_wndClassView.InsertItem(_T("Oś Z1"), 3, 4, hClass);
	m_wndClassView.InsertItem(_T("Oś X2"), 5, 6, hClass);
	m_wndClassView.InsertItem(_T("Oś Y2"), 5, 6, hClass);
	m_wndClassView.InsertItem(_T("Oś Z2"), 5, 6, hClass);
	m_wndClassView.InsertItem(_T("Oś X3"), 3, 4, hClass);
	m_wndClassView.InsertItem(_T("Oś Y3"), 3, 4, hClass);
	m_wndClassView.InsertItem(_T("Oś Z3"), 3, 4, hClass);
	m_wndClassView.Expand(hClass, TVE_EXPAND);

	hClass = m_wndClassView.InsertItem(_T("AHRS"), 2, 2, hRoot);
	m_wndClassView.InsertItem(_T("Phi tryg."), 3, 4, hClass);
	m_wndClassView.InsertItem(_T("Theta tryg."), 3, 4, hClass);
	m_wndClassView.InsertItem(_T("Psi tryg."), 3, 4, hClass);
	m_wndClassView.InsertItem(_T("Phi kwat."), 5, 6, hClass);
	m_wndClassView.InsertItem(_T("Theta kwat."), 5, 6, hClass);
	m_wndClassView.InsertItem(_T("Psi kwat."), 5, 6, hClass);
	m_wndClassView.InsertItem(_T("Phi akcel."), 3, 4, hClass);
	m_wndClassView.InsertItem(_T("Theta akcel."), 3, 4, hClass);
	m_wndClassView.InsertItem(_T("Psi akcel."), 3, 4, hClass);
	m_wndClassView.InsertItem(_T("Phi żyro."), 5, 6, hClass);
	m_wndClassView.InsertItem(_T("Theta żyro."), 5, 6, hClass);
	m_wndClassView.InsertItem(_T("Psi żyro."), 5, 6, hClass);
	m_wndClassView.Expand(hClass, TVE_EXPAND);

	hClass = m_wndClassView.InsertItem(_T("Baro"), 2, 2, hRoot);
	m_wndClassView.InsertItem(_T("Cisnienie bzwzgl. 1"), 3, 4, hClass);
	m_wndClassView.InsertItem(_T("Cisnienie bzwzgl. 2"), 3, 4, hClass);
	m_wndClassView.InsertItem(_T("Wysokosć AGL 1"), 5, 6, hClass);
	m_wndClassView.InsertItem(_T("Wysokosć AGL 2"), 5, 6, hClass);
	m_wndClassView.InsertItem(_T("Cisnienie różn. 1"), 3, 4, hClass);
	m_wndClassView.InsertItem(_T("Cisnienie różn. 2"), 3, 4, hClass);
	m_wndClassView.InsertItem(_T("IAS 1"), 5, 6, hClass);
	m_wndClassView.InsertItem(_T("IAS 2"), 5, 6, hClass);
	m_wndClassView.Expand(hClass, TVE_EXPAND);


	//hClass = m_wndClassView.InsertItem(_T("Globals"), 2, 2, hRoot);
	//m_wndClassView.InsertItem(_T("theFakeApp"), 5, 5, hClass);
}

void CClassView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CString strNazwaZmiennej;
	int nIdZmiennej;
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndClassView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// Zaznacz kliknięty element:
		CPoint ptTree = point;
		
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != nullptr)
		{
			pWndTree->SelectItem(hTreeItem);
			nIdZmiennej = pWndTree->GetIndent();
			//nIdZmiennej = pWndTree->GetStyle();
			nIdZmiennej = pWndTree->m_nFlags;			
			nIdZmiennej = pWndTree->GetDlgCtrlID();
			nIdZmiennej = pWndTree->GetItemData(hTreeItem);
			strNazwaZmiennej = pWndTree->GetItemText(hTreeItem);
			//pWndTree->get
		}
	}

	pWndTree->SetFocus();
	CKonfigTelemetrii m_cKonfigTelemetrii;

	m_cKonfigTelemetrii.UstawNazweZmiennej(strNazwaZmiennej);
	m_cKonfigTelemetrii.UstawIDZmiennej(5);
	m_cKonfigTelemetrii.DoModal();
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
