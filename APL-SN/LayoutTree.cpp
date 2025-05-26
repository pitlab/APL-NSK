#include "stdafx.h"
#include "LayoutTree.h"
#include "resource.h"
#include "Log/RecordSet.h"
#include "Helpers/Unicode.h"
#include "Tokenizer.h"

namespace view2d
{

const TCHAR LayoutTree::NODE_SEPARATE_TEXT[] = _T( "[Divide]" );
const TCHAR LayoutTree::NODE_JOIN_TEXT[] = _T( "[Join]" );
const TCHAR LayoutTree::NODE_JOIN_EQSCALE_TEXT[] = _T( "[Join (equal scale)]" );
const TCHAR LayoutTree::NODE_XY_TEXT[] = _T( "X-Y" );

LayoutTree::LayoutTree( const logger::RecordSet & _recordSet ) :
	m_recordSet( _recordSet ),
	m_pDragImage( NULL ),
	m_bLDragging( FALSE ),
	m_extDragging( FALSE )
{
	m_idCount = max( NODEID_SEPARATE, max( NODEID_JOIN, NODEID_JOIN_EQSCALE ) ) + 1;
}

LayoutTree::~LayoutTree()
{
}

BEGIN_MESSAGE_MAP(LayoutTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &LayoutTree::OnBeginDrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_CANCELMODE()
	ON_NOTIFY_REFLECT(NM_RCLICK, &LayoutTree::OnRClick)
	ON_WM_CONTEXTMENU()
	ON_COMMAND( ID_MENU_DATABROWSER_SEPARATE, &LayoutTree::OnMenuTreeSplit)
	ON_COMMAND( ID_MENU_DATABROWSER_JOIN, &LayoutTree::OnMenuTreeJoin)
	ON_COMMAND( ID_MENU_DATABROWSER_JOIN_EQSCALE, &LayoutTree::OnMenuTreeJoinEqscale)
	ON_COMMAND( ID_MENU_DATABROWSER_XY, &LayoutTree::OnMenuTreeXY )
	ON_COMMAND( ID_MENU_DATABROWSER_DELETE, &LayoutTree::OnMenuTreeDelete)
	ON_COMMAND( ID_MENU_DATABROWSER_CHANGETYPE_SEPARATE, &LayoutTree::OnMenuChangeToSplit)
	ON_COMMAND( ID_MENU_DATABROWSER_CHANGETYPE_JOIN, &LayoutTree::OnMenuChangeToJoin)
	ON_COMMAND( ID_MENU_DATABROWSER_JOIN_CHANGETYPE_EQSCALE, &LayoutTree::OnMenuChangeToJoinEqscale )
END_MESSAGE_MAP()


void LayoutTree::OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	m_hItemDrag = pNMTreeView->itemNew.hItem;
	m_hItemDrop = NULL;

	m_pDragImage = CreateDragImageEx( m_hItemDrag );
	assert( m_pDragImage );
	m_bLDragging = TRUE;
	m_pDragImage->BeginDrag(0, CPoint( -15, -15 ) );

	POINT pt = pNMTreeView->ptDrag;
	ClientToScreen( &pt );
	m_pDragImage->DragEnter( NULL, pt );

	SetCapture();

	*pResult = 1;
}

void LayoutTree::onBeginDrag( const std::vector< DragSourceElem > & _dragSrc )
{
	assert( !_dragSrc.empty() );

	m_hItemDrag = NULL;
	m_hItemDrop = NULL;
	m_extDragSrc = _dragSrc;

	string_type dragText;

	for( size_t i = 0; i < _dragSrc.size(); ++i )
	{
		if( !dragText.empty() )
			dragText.append( _T( "\n" ) );
		dragText.append( getDragSourceElemText( _dragSrc[i] ) );
	}

	m_pDragImage = CreateTextDragImage( dragText.c_str() );
	assert( m_pDragImage );
	m_bLDragging = TRUE;
	m_extDragging = TRUE;
	m_pDragImage->BeginDrag(0, CPoint( -15, -15 ) );

	POINT point;
	GetCursorPos( &point );
	ClientToScreen( &point );
	m_pDragImage->DragEnter( NULL, point );

	SetCapture();
}

void LayoutTree::onDragOver( CPoint _point )
{
	assert( m_bLDragging );

	POINT pt = _point;
	ClientToScreen( &pt );

	CImageList::DragMove(pt);

	if( canDrop( _point ) )
		SetCursor( ::LoadCursor( NULL, IDC_ARROW ) );
	else
		SetCursor( ::LoadCursor( NULL, IDC_NO ) );

	UINT flags;
	HTREEITEM hitem = HitTest( _point, &flags);
	if( hitem != m_hItemDrop )
	{
		CImageList::DragShowNolock( FALSE );
		SelectDropTarget( hitem );
		m_hItemDrop = hitem;
		CImageList::DragShowNolock( TRUE );
	}
}

bool LayoutTree::canDrop( CPoint _point ) const
{
	assert( m_bLDragging );

	HTREEITEM hItemDrop = m_hItemDrop;

	if( m_hItemDrag )
	{
		if( !checkDrop1( m_hItemDrag, hItemDrop ) )
			return false;

		if( !hItemDrop )
		{
			hItemDrop = GetParentItem( m_hItemDrag );
			if( hItemDrop && !GetNextItem( m_hItemDrag, TVGN_NEXT ) )
				hItemDrop = GetParentItem( hItemDrop );

			if( hItemDrop )
			{
				if( !checkDrop2( m_hItemDrag, hItemDrop ) )
					return false;

				return true;
			}

			return false;
		}
		else
		{
			if( !checkDrop2( m_hItemDrag, hItemDrop ) )
				return false;

			return true;
		}
	}
	else
	{
		assert( !m_extDragSrc.empty() );

		if( !hItemDrop )
		{
			hItemDrop = GetParentItem( m_hItemDrag );
			if( GetCount() == 0 )
				hItemDrop = TVI_ROOT;

			if( hItemDrop )
				return canInsertExtSource( hItemDrop, TVI_LAST );

			return false;
		}
		else
		{
			HTREEITEM hParent = GetParentItem( hItemDrop );
			HTREEITEM hInsertAfter = hItemDrop;

			if( isSpecialNode( hItemDrop ) )
			{
				hParent = hItemDrop;
				hInsertAfter = TVI_FIRST;
			}

			return canInsertExtSource( hParent, hInsertAfter );
		}
	}
}


void LayoutTree::onDragDrop( CPoint _point )
{
	assert( m_bLDragging );

	m_bLDragging = FALSE;
	m_extDragging = FALSE;

	CImageList::DragLeave(this);
	CImageList::EndDrag();
	ReleaseCapture();

	delete m_pDragImage;

	SelectDropTarget(NULL);

	if( m_hItemDrag )
	{
		if( !checkDrop1( m_hItemDrag, m_hItemDrop ) )
			return;

		if( !m_hItemDrop )
		{
			m_hItemDrop = GetParentItem( m_hItemDrag );
			if( m_hItemDrop && !GetNextItem( m_hItemDrag, TVGN_NEXT ) )
				m_hItemDrop = GetParentItem( m_hItemDrop );

			if( m_hItemDrop )
			{
				if( !checkDrop2( m_hItemDrag, m_hItemDrop ) )
					return;

				LockWindowUpdate();

				HTREEITEM hNewItem = copyBranch( m_hItemDrag, m_hItemDrop, TVI_LAST );
				DeleteItem( m_hItemDrag );
				fixLeftRightAxis( hNewItem );
				Select( hNewItem, TVGN_CARET );

				UnlockWindowUpdate();
			}
		}
		else
		{
			if( !checkDrop2( m_hItemDrag, m_hItemDrop ) )
				return;

			HTREEITEM hParent = GetParentItem( m_hItemDrop );
			HTREEITEM hInsertAfter = m_hItemDrop;

			LockWindowUpdate();

			if( isSpecialNode( m_hItemDrop ) )
			{
				hParent = m_hItemDrop;
				hInsertAfter = TVI_FIRST;
			}

			HTREEITEM hNewItem = copyBranch( m_hItemDrag, hParent, hInsertAfter );
			DeleteItem( m_hItemDrag );
			fixLeftRightAxis( hNewItem );
			Select( hNewItem, TVGN_CARET );

			UnlockWindowUpdate();
		}
	}
	else
	{
		assert( !m_extDragSrc.empty() );

		if( !m_hItemDrop )
		{
			m_hItemDrop = GetParentItem( m_hItemDrag );
			if( GetCount() == 0 )
				m_hItemDrop = TVI_ROOT;

			if( m_hItemDrop )
				insertExtSource( m_hItemDrop, TVI_LAST );
		}
		else
		{
			HTREEITEM hParent = GetParentItem( m_hItemDrop );
			HTREEITEM hInsertAfter = m_hItemDrop;

			if( isSpecialNode( m_hItemDrop ) )
			{
				hParent = m_hItemDrop;
				hInsertAfter = TVI_FIRST;
				insertExtSource( hParent, hInsertAfter );
			}
			else if( isXYNode( m_hItemDrop ) )
			{
				insertExtSourceToXY( m_hItemDrop, _point );
			}
			else
			{
				insertExtSource( hParent, hInsertAfter );
			}
		}

		m_extDragSrc.clear();
	}
}

string_type LayoutTree::getDragSourceElemText( const DragSourceElem & _elem ) const
{
	if( _elem.type == DragSourceElem::DST_SPECIAL_NODE )
	{
		switch( _elem.ident )
		{
		case NODEID_SEPARATE:
			return NODE_SEPARATE_TEXT;
		case NODEID_JOIN:
			return NODE_JOIN_TEXT;
		case NODEID_JOIN_EQSCALE:
			return NODE_JOIN_EQSCALE_TEXT;
		case NODEID_XY:
			return NODE_XY_TEXT;
		default:
			assert( false );
			return string_type();
		}
	}
	else if( _elem.type == DragSourceElem::DST_VARIABLE )
	{
		const logger::VariableDesc & varDesc = 
			m_recordSet.getConfig().getVariableDesc( _elem.ident );

		return toWstring( varDesc.name );
	}
	else
	{
		assert( false );
		return string_type();
	}
}

bool LayoutTree::canInsertExtSource( HTREEITEM _hParent, HTREEITEM _hInsertAfter ) const
{
	for( size_t i = 0; i < m_extDragSrc.size(); ++i )
	{
		const DragSourceElem & elem = m_extDragSrc[i];
		
		if( elem.type == DragSourceElem::DST_SPECIAL_NODE )
		{
			// Nie dodawaj do rodzica "nak³adaj z równ¹ skal¹"
			if( _hParent != TVI_ROOT && isJoinEqualScale( _hParent ) )
				continue;

			return true;
		}
		else if( elem.type == DragSourceElem::DST_VARIABLE )
		{
			if( _hParent == TVI_ROOT || !isSpecialNode( _hParent ) )
				continue;

			return true;
		}
		else
		{
			assert( false );
		}
	}

	return false;
}

void LayoutTree::insertExtSourceToXY( HTREEITEM _hXYItem, const CPoint & _point )
{
	int id = GetItemData( _hXYItem );
	assert( !isSpecialNode( id ) );
	DrawDescMap::iterator it = m_drawDescriptors.find( id );
	assert( it != m_drawDescriptors.end() );

	// XY drawing descriptor to be modified
	DrawDesc &desc = it->second;

	CRect rect;
	GetItemRect( _hXYItem, &rect, TRUE );

	// Figure out cursor position. 
	bool inXarea = false;

	if( _point.x <= rect.left )
		inXarea = true;
	else if( _point.x >= rect.right )
		inXarea = false;
	else
	{
		CSize xVarLabelSize = GetDC()->GetTextExtent(desc.getXVarLabel().c_str(), desc.getXVarLabel().length() );
		CSize separatorSize = GetDC()->GetTextExtent(desc.getXYVarSeparator().c_str(), desc.getXYVarSeparator().length() );
		inXarea = _point.x < rect.left + xVarLabelSize.cx + separatorSize.cx / 2;
	}

	bool xVarFilled = false;
	bool yVarFilled = false;

	// Process dragged variables.
	for( size_t i = 0; i < m_extDragSrc.size(); ++i )
	{
		const DragSourceElem & elem = m_extDragSrc[i];

		// Skip non-variables.
		if( elem.type != DragSourceElem::DST_VARIABLE )
			continue;

		// If X-var not already filled and cursor is within X-variable area.
		if( !xVarFilled && inXarea )
		{
			desc.varXId = elem.ident;
			desc.varXName = 
				m_recordSet.getConfig().getVariableDesc( elem.ident ).name;
			xVarFilled = true;
		}
		else if( !yVarFilled && ( !inXarea || desc.varYId == logger::Config::INVALID_ID ) )
		{
			// If Y-var not already filled and cursor is within Y-var area or cursor within X-var area and Y-var not filled.
			desc.varYId = elem.ident;
			desc.varYName = 
				m_recordSet.getConfig().getVariableDesc( elem.ident ).name;
			yVarFilled = true;
		}
	}

	// Create new node description
	string_type nodeName = desc.getXVarLabel() + desc.getXYVarSeparator() + desc.getYVarLabel();

	SetItemText( _hXYItem, nodeName.c_str() );

	// Update details description.
	SelectItem( NULL );
	SelectItem( _hXYItem );
}

void LayoutTree::insertExtSource( HTREEITEM _hParent, HTREEITEM _hInsertAfter )
{
	HTREEITEM hLastAdded = NULL;
	for( size_t i = 0; i < m_extDragSrc.size(); ++i )
	{
		const DragSourceElem & elem = m_extDragSrc[i];
		
		if( elem.type == DragSourceElem::DST_SPECIAL_NODE && elem.ident != NODEID_XY )
		{
			// Nie dodawaj do rodzica "nak³adaj z równ¹ skal¹"
			if( _hParent != TVI_ROOT && isJoinEqualScale( _hParent ) )
				continue;

			hLastAdded = InsertItem( getDragSourceElemText( elem ).c_str(), 
				_hParent, _hInsertAfter );

			SetItemData( hLastAdded, elem.ident );
			_hInsertAfter = hLastAdded;
		}
		else if( elem.type == DragSourceElem::DST_SPECIAL_NODE && elem.ident == NODEID_XY )
		{
			if( _hParent == TVI_ROOT || !isSpecialNode( _hParent ) )
				continue;

			// Stwórz domyœlny deskryptor wykresu
			DrawDesc drawDesc;

			drawDesc.mode = DrawDesc::DM_XY_POINT;	
			drawDesc.xySampling = DrawDesc::XYS_XY;
			drawDesc.dataRange = DrawDesc::DR_VISIBLE;

			// Zapamiêtaj deskryptor
			int descID = ++m_idCount;
			m_drawDescriptors[descID] = drawDesc;

			// Wstaw wêze³ drzewa symbolizuj¹cy deskryptor ..
			hLastAdded = InsertItem( 
				_T( "[none] x [none]" ),
				_hParent, _hInsertAfter );
			// .. i zapamiêtaj w wêŸle drzewa powi¹zanie z nim.
			SetItemData( hLastAdded, descID );

			_hInsertAfter = hLastAdded;
		}
		else if( elem.type == DragSourceElem::DST_VARIABLE )
		{
			if( _hParent == TVI_ROOT || !isSpecialNode( _hParent ) )
				continue;

			// Stwórz domyœlny deskryptor wykresu
			DrawDesc drawDesc;
			drawDesc.varYId = elem.ident;
			drawDesc.varYName = 
				m_recordSet.getConfig().getVariableDesc( drawDesc.varYId ).name;

			// Zapamiêtaj deskryptor
			int descID = ++m_idCount;
			m_drawDescriptors[descID] = drawDesc;

			// Wstaw wêze³ drzewa symbolizuj¹cy deskryptor ..
			hLastAdded = InsertItem( 
				toWstring( drawDesc.varYName ).c_str(),
				_hParent, _hInsertAfter );
			// .. i zapamiêtaj w wêŸle drzewa powi¹zanie z nim.
			SetItemData( hLastAdded, descID );

			// Dodaj osie wykresów jeœli jest miejsce
			std::pair< bool, bool > axisSpace = checkVerticalAxisSpace( hLastAdded );
			if( axisSpace.first )
				m_drawDescriptors[descID].scale = DrawDesc::DS_ONLEFT;
			else if( axisSpace.second )
				m_drawDescriptors[descID].scale = DrawDesc::DS_ONRIGHT;

			_hInsertAfter = hLastAdded;
		}
		else
		{
			assert( false );
		}
	}

	if( hLastAdded )
		Select( hLastAdded, TVGN_CARET );
}


void LayoutTree::onDragCancel()
{
	assert( m_bLDragging );

	m_bLDragging = FALSE;
	m_extDragging = FALSE;
	CImageList::DragLeave(this);
	CImageList::EndDrag();
	ReleaseCapture();

	delete m_pDragImage;

	m_extDragSrc.clear();
	m_hItemDrag = NULL;
	m_hItemDrop = NULL;

	SelectDropTarget(NULL);
}

void LayoutTree::OnMouseMove(UINT nFlags, CPoint point)
{
	CTreeCtrl::OnMouseMove(nFlags, point);

	if( m_bLDragging )
		onDragOver( point );
} 

void LayoutTree::OnRButtonDown(UINT nFlags, CPoint _point)
{
	CTreeCtrl::OnRButtonDown( nFlags, _point );

	if( m_bLDragging )
		onDragDrop( _point );
}

void LayoutTree::OnLButtonDown(UINT nFlags, CPoint _point)
{
	CTreeCtrl::OnLButtonDown( nFlags, _point );

	if( m_bLDragging && m_extDragging )
		onDragDrop( _point );
}

void LayoutTree::OnRClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// Send WM_CONTEXTMENU to self
	SendMessage(WM_CONTEXTMENU, (WPARAM) m_hWnd, GetMessagePos());
	// Mark message as handled and suppress default handling
	*pResult = 1;
}

void LayoutTree::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CTreeCtrl::OnLButtonUp(nFlags, point);

	if( m_bLDragging )
		onDragDrop( point );
}

void LayoutTree::OnCancelMode()
{
	CTreeCtrl::OnCancelMode();

	if( m_bLDragging )
		onDragCancel();
}

void LayoutTree::OnRButtonUp(UINT nFlags, CPoint _point)
{
	CTreeCtrl::OnRButtonUp( nFlags, _point );

	if( m_bLDragging )
		onDragCancel();
}

void LayoutTree::OnContextMenu(CWnd* pWnd, CPoint _point)
{
	CMenu menu;
	menu.LoadMenu( IDR_POPUP_DATABROWSER_VIEWTREE );

	CMenu* pMenu = menu.GetSubMenu( 0 );
	assert( pMenu );

	UINT flags;
	ScreenToClient( &_point );
	m_hActiveItem = HitTest( _point, &flags );
	if( m_hActiveItem )
		SelectItem( m_hActiveItem );
	else
		m_hActiveItem = GetSelectedItem();

	pMenu->EnableMenuItem( ID_MENU_DATABROWSER_SEPARATE, MF_BYCOMMAND | MF_ENABLED );
	pMenu->EnableMenuItem( ID_MENU_DATABROWSER_JOIN, MF_BYCOMMAND | MF_ENABLED );
	pMenu->EnableMenuItem( ID_MENU_DATABROWSER_JOIN_EQSCALE, MF_BYCOMMAND | MF_ENABLED );

	pMenu->EnableMenuItem( ID_MENU_DATABROWSER_CHANGETYPE_SEPARATE, 
		MF_BYCOMMAND | ( canChangeToSplit( m_hActiveItem ) ? MF_ENABLED : MF_GRAYED ) );
	pMenu->EnableMenuItem( ID_MENU_DATABROWSER_CHANGETYPE_JOIN, 
		MF_BYCOMMAND | ( canChangeToJoin( m_hActiveItem ) ? MF_ENABLED : MF_GRAYED ) );
	pMenu->EnableMenuItem( ID_MENU_DATABROWSER_JOIN_CHANGETYPE_EQSCALE, 
		MF_BYCOMMAND | ( canChangeToEqscale( m_hActiveItem ) ? MF_ENABLED : MF_GRAYED ) );

	pMenu->EnableMenuItem( ID_MENU_DATABROWSER_DELETE, MF_BYCOMMAND | 
		( m_hActiveItem ? MF_ENABLED : MF_GRAYED ) );

	POINT cursorPt;
	GetCursorPos( &cursorPt );
	pMenu->TrackPopupMenu( 0, cursorPt.x, cursorPt.y, this );
}
bool LayoutTree::checkDrop1( HTREEITEM _hItemDrag, HTREEITEM _hItemDrop ) const
{
	if( _hItemDrag == _hItemDrop )
		return false;

	// If Drag item is an ancestor of Drop item then return
	for( HTREEITEM htiParent = _hItemDrop;
		htiParent = GetParentItem( htiParent ); htiParent )
	{
		if( htiParent == _hItemDrag ) 
			return false;
	}

	return true;
}

bool LayoutTree::checkDrop2( HTREEITEM _hItemDrag, HTREEITEM _hItemDrop ) const
{
	if( !isSpecialNode( _hItemDrag ) )
		return true;

	if( isJoinEqualScale( _hItemDrop ) )
		return false;

	_hItemDrop = GetParentItem( _hItemDrop );
	if( _hItemDrop && isJoinEqualScale( _hItemDrop ) )
		return false;

	return true;
}

void LayoutTree::extractSiblings( HTREEITEM _item, std::vector< HTREEITEM > & _output ) const
{
	assert( _item );

	HTREEITEM hItem = GetNextItem( _item, TVGN_PARENT );

	for( hItem = GetNextItem( hItem, TVGN_CHILD ); hItem;
		hItem = GetNextItem( hItem, TVGN_NEXT ) )
	{
		_output.push_back( hItem );
	}
}

bool LayoutTree::isSpecialNode( HTREEITEM _item ) const
{
	assert( _item );
	return isSpecialNode( GetItemData( _item ) );
}

bool LayoutTree::isXYNode( HTREEITEM _item ) const
{
	assert( _item );
	int nodeId = GetItemData( _item );
	if( isSpecialNode( nodeId ) )
		return false;

	DrawDescMap::const_iterator it = m_drawDescriptors.find( nodeId );
	assert( it != m_drawDescriptors.end() );

	if( it->second.mode == DrawDesc::DM_XY_POINT || it->second.mode == DrawDesc::DM_XY_LINES )
		return true;

	return false;
}

bool LayoutTree::isJoinEqualScale( HTREEITEM _item ) const
{
	assert( _item );
	return NODEID_JOIN_EQSCALE == GetItemData( _item );
}

bool LayoutTree::isSpecialNode( int _nodeId )
{
	return _nodeId == NODEID_SEPARATE ||
		_nodeId == NODEID_JOIN || _nodeId == NODEID_JOIN_EQSCALE;
}

CImageList* LayoutTree::CreateDragImageEx( HTREEITEM hItem )
{
	if( GetImageList( TVSIL_NORMAL ) != NULL )
		return CreateDragImage(hItem);

	CRect rect;
	GetItemRect( hItem, rect, TRUE );

	// Create bitmap
	CClientDC	dc( this );
	CDC 		memDC;	

	if( !memDC.CreateCompatibleDC( &dc ) )
		return NULL;

	CBitmap bitmap;
	if( !bitmap.CreateCompatibleBitmap( &dc, rect.Width(), rect.Height() ) )
		return NULL;

	CBitmap* pOldMemDCBitmap = memDC.SelectObject( &bitmap );
	CFont* pOldFont = memDC.SelectObject( GetFont() );

	CRect rect2( 0, 0, rect.Width(), rect.Height() );
	memDC.FillSolidRect( &rect2, RGB( 0, 255, 0 ) );
	memDC.SetTextColor( GetSysColor( COLOR_GRAYTEXT ) );
	memDC.TextOut( 0, 0, GetItemText( hItem ) );

	memDC.SelectObject( pOldFont );
	memDC.SelectObject( pOldMemDCBitmap );

	// Create imagelist
	CImageList* pImageList = new CImageList;
	pImageList->Create( rect.Width(), rect.Height(), ILC_COLOR | ILC_MASK, 0, 1 );
	pImageList->Add( &bitmap, RGB( 0, 255, 0 ) ); // Here green is used as mask color

	return pImageList;
}

CImageList* LayoutTree::CreateTextDragImage( const string_type & _text )
{
	// Create bitmap
	CClientDC	dc( this );
	CDC 		memDC;

	if( !memDC.CreateCompatibleDC( &dc ) )
		return NULL;

	struct TextEntry
	{
		CString text;
		int y;
	};

	std::vector< TextEntry > textEntries;

	boost::scoped_array< wchar_t > textBuffer( new wchar_t[_text.length() + 1] );
	memcpy( textBuffer.get(), _text.c_str(), sizeof( wchar_t ) * ( _text.length() + 1 ) );
	WTokenizer tokenizer( textBuffer.get(), L"\n" );

	CSize maxSize( 0, 0 );

	for( const wchar_t * pText = tokenizer.nextToken(); pText; pText = tokenizer.nextToken() )
	{
		CSize textSize = dc.GetOutputTextExtent( pText );

		TextEntry textEntry;
		textEntry.y = maxSize.cy;
		textEntry.text = pText;

		textEntries.push_back( textEntry );

		maxSize.cx = max( maxSize.cx, textSize.cx );
		maxSize.cy += textSize.cy;
	}

	CRect rect( 0, 0, maxSize.cx, maxSize.cy );

	CBitmap bitmap;
	if( !bitmap.CreateCompatibleBitmap( &dc, rect.Width(), rect.Height() ) )
		return NULL;

	CBitmap* pOldMemDCBitmap = memDC.SelectObject( &bitmap );
	CFont* pOldFont = memDC.SelectObject( GetFont() );

	CRect rect2( 0, 0, rect.Width(), rect.Height() );
	memDC.FillSolidRect( &rect2, RGB( 0, 255, 0 ) );
	memDC.SetTextColor( GetSysColor( COLOR_GRAYTEXT ) );

	for( size_t i = 0; i < textEntries.size(); ++i )
	{
		memDC.TextOut( 0, textEntries[i].y, textEntries[i].text );
	}

	memDC.SelectObject( pOldFont );
	memDC.SelectObject( pOldMemDCBitmap );

	// Create imagelist
	CImageList* pImageList = new CImageList;
	pImageList->Create( rect.Width(), rect.Height(), ILC_COLOR | ILC_MASK, 0, 1 );
	pImageList->Add( &bitmap, RGB( 0, 255, 0 ) ); // Here green is used as mask color

	return pImageList;
}

HTREEITEM LayoutTree::copyBranch( 
	HTREEITEM _hSrcSubtree, HTREEITEM _hDstNode, HTREEITEM _hDstSubtreePos )
{
	HTREEITEM newItem = InsertItem( GetItemText( _hSrcSubtree ), _hDstNode, _hDstSubtreePos );
	SetItemData( newItem, GetItemData( _hSrcSubtree ) );

	copySubtree( newItem, _hSrcSubtree );
	Expand( newItem, TVE_EXPAND );

	return newItem;
}

void LayoutTree::copySubtree( HTREEITEM _dstSubtree, HTREEITEM _srcSubtree )
{
	HTREEITEM srcChild = GetChildItem( _srcSubtree );
	HTREEITEM newChild;

	while( srcChild )
	{
		newChild = InsertItem( GetItemText( srcChild ), _dstSubtree, TVI_LAST );
		SetItemData( newChild, GetItemData( srcChild ) );

		copySubtree( newChild, srcChild );
		Expand( newChild, TVE_EXPAND );

		srcChild = GetNextItem( srcChild, TVGN_NEXT );
	}
} 

void LayoutTree::OnMenuTreeSplit()
{
	DragSourceElem dragElem;
	dragElem.type = DragSourceElem::DST_SPECIAL_NODE;
	dragElem.ident = NODEID_SEPARATE;

	onBeginDrag( std::vector< DragSourceElem >( 1, dragElem ) );
}

void LayoutTree::OnMenuTreeJoin()
{
	DragSourceElem dragElem;
	dragElem.type = DragSourceElem::DST_SPECIAL_NODE;
	dragElem.ident = NODEID_JOIN;

	onBeginDrag( std::vector< DragSourceElem >( 1, dragElem ) );
}

void LayoutTree::OnMenuTreeJoinEqscale()
{
	DragSourceElem dragElem;
	dragElem.type = DragSourceElem::DST_SPECIAL_NODE;
	dragElem.ident = NODEID_JOIN_EQSCALE;

	onBeginDrag( std::vector< DragSourceElem >( 1, dragElem ) );
}

void LayoutTree::OnMenuTreeXY()
{
	DragSourceElem dragElem;
	dragElem.type = DragSourceElem::DST_SPECIAL_NODE;
	dragElem.ident = NODEID_XY;

	onBeginDrag( std::vector< DragSourceElem >( 1, dragElem ) );
}

void LayoutTree::OnMenuChangeToSplit()
{
	assert( canChangeToSplit( m_hActiveItem ) );
	SetItemText( m_hActiveItem, NODE_SEPARATE_TEXT );
	SetItemData( m_hActiveItem, NODEID_SEPARATE );

	fixLeftRightAxis( m_hActiveItem );
}

void LayoutTree::OnMenuChangeToJoin()
{
	assert( canChangeToJoin( m_hActiveItem ) );
	SetItemText( m_hActiveItem, NODE_JOIN_TEXT );
	SetItemData( m_hActiveItem, NODEID_JOIN );

	fixLeftRightAxis( m_hActiveItem );
}

void LayoutTree::OnMenuChangeToJoinEqscale()
{
	assert( canChangeToEqscale( m_hActiveItem ) );
	SetItemText( m_hActiveItem, NODE_JOIN_EQSCALE_TEXT );
	SetItemData( m_hActiveItem, NODEID_JOIN_EQSCALE );

	fixLeftRightAxis( m_hActiveItem );
}

bool LayoutTree::canChangeToSplit( HTREEITEM _hItem ) const
{
	if( !_hItem )
		return false;

	if( !isSpecialNode( _hItem ) )
		return false;

	if( GetItemData( _hItem ) == NODEID_SEPARATE )
		return false;

	return true;
}

bool LayoutTree::canChangeToJoin( HTREEITEM _hItem ) const
{
	if( !_hItem )
		return false;

	if( !isSpecialNode( _hItem ) )
		return false;

	if( GetItemData( _hItem ) == NODEID_JOIN )
		return false;

	return true;
}

bool LayoutTree::canChangeToEqscale( HTREEITEM _hItem ) const
{
	if( !_hItem )
		return false;

	if( !isSpecialNode( _hItem ) )
		return false;

	if( GetItemData( _hItem ) == NODEID_JOIN_EQSCALE )
		return false;

	for( HTREEITEM hChild = GetNextItem( _hItem, TVGN_CHILD );
		hChild; hChild = GetNextItem( hChild, TVGN_NEXT ) )
	{
		assert( hChild );
		if( isSpecialNode( hChild ) )
			return false;
	}

	return true;
}

void LayoutTree::OnMenuTreeDelete()
{
	if( !m_hActiveItem )
		return;

	DeleteItem( m_hActiveItem );
}

std::pair< bool, bool > LayoutTree::checkVerticalAxisSpace( 
	HTREEITEM _hItem, bool _stopOnCheckedItem ) const
{
	// Na pocz¹tku za³ó¿ ¿e miejsce jest wolne po obydwu stronach.
	std::pair< bool, bool > result( true, true );

	if( !_hItem )
		return result;

	// Pobierz identyfikator wêz³a.
	int id = GetItemData( _hItem );

	for( HTREEITEM hParent = isSpecialNode( id ) ? _hItem : GetParentItem ( _hItem ); 
		hParent; hParent = GetParentItem ( hParent ) )
	{
		// Musi byæ wêz³em specjalnym.
		assert( isSpecialNode( hParent ) );

		id = GetItemData( hParent );

		// W przypadku wêz³a rozdzielaj¹cego z jego definicji
		// miejsce po prawej i lewej stronie wykresu jest gwarantowane w wêŸle.
		if( id == NODEID_SEPARATE )
			continue;

		// W przypadku wêz³ów ³¹cz¹cych mo¿e istnieæ ju¿ wêze³ ze skal¹,
		// w takim wypadku nie jest ju¿ mo¿liwe wyœwietlenie skali w tym miejscu
		if( id == NODEID_JOIN || id == NODEID_JOIN_EQSCALE )
		{
			for( HTREEITEM hChild = GetNextItem( hParent, TVGN_CHILD ); 
				hChild;	hChild = GetNextItem( hChild, TVGN_NEXT ) )
			{
				if( hChild == _hItem )
				{
					if( _stopOnCheckedItem )
						break;
					else
						continue;
				}

				int childId = GetItemData( hChild );
				DrawDescMap::const_iterator it = 
					m_drawDescriptors.find( childId );
				// TODO: fix
//				assert( it != m_drawDescriptors.end() );

				if( it != m_drawDescriptors.end() )
				{
					switch( it->second.scale )
					{
					case DrawDesc::DS_ONLEFT:
						result.first = false;
						break;
					case DrawDesc::DS_ONRIGHT:
						result.second = false;
						break;
					}
				}
			}
		}
	}

	return result;
}

void LayoutTree::fixLeftRightAxis( HTREEITEM _hItem )
{
	if( !isSpecialNode( _hItem ) )
	{
		int id = GetItemData( _hItem );
		DrawDescMap::iterator it = m_drawDescriptors.find( id );
		assert( it != m_drawDescriptors.end() );
		DrawDesc & drawDesc = it->second;

		std::pair< bool, bool > axisSpace = checkVerticalAxisSpace( _hItem, true );

		if( drawDesc.scale == DrawDesc::DS_ONLEFT )
		{
			// Jeœli na lewo wolne to nic nie rób.
			if( axisSpace.first )
				return;

			// W przeciwnym wypadku jeœli jest miejsce na prawo to przerzuæ tam wykres.
			// a jeœli i tam brak miejsca to wyzeruj wykres.
			if( axisSpace.second )
				drawDesc.scale = DrawDesc::DS_ONRIGHT;
			else 
				drawDesc.scale = DrawDesc::DS_NONE;
		}
		else if( drawDesc.scale == DrawDesc::DS_ONRIGHT )
		{
			if( axisSpace.second )
				return;

			if( axisSpace.first )
				drawDesc.scale = DrawDesc::DS_ONLEFT;
			else 
				drawDesc.scale = DrawDesc::DS_NONE;
		}
	}
	else
	{
		for( HTREEITEM hChild = GetNextItem( _hItem, TVGN_CHILD );
			hChild; hChild = GetNextItem( hChild, TVGN_NEXT ) )
		{
			fixLeftRightAxis( hChild );
		}
	}
}

void LayoutTree::fixSiblingsLeftRightAxis( HTREEITEM _hItem )
{
	assert( _hItem );
	assert( !isSpecialNode( _hItem ) );

	// Omiñ jeœli wêze³ jest typu rozdziel
	HTREEITEM hParent = GetParentItem( _hItem );
	if( GetItemData( hParent ) == NODEID_SEPARATE )
		return;

	int itemId = GetItemData( _hItem );
	DrawDescMap::const_iterator it = m_drawDescriptors.find( itemId );
	assert( it != m_drawDescriptors.end() );
	const DrawDesc & itemDesc = it->second;

	bool leftAxis = itemDesc.scale == DrawDesc::DS_ONLEFT;
	bool rightAxis = itemDesc.scale == DrawDesc::DS_ONRIGHT;
	
	for( HTREEITEM hChild = GetNextItem( hParent, TVGN_CHILD );
		hChild; hChild = GetNextItem( hChild, TVGN_NEXT ) )
	{
		if( hChild == _hItem )
			continue;

		DrawDesc & childDesc = m_drawDescriptors[GetItemData( hChild )];

		if( childDesc.scale == DrawDesc::DS_ONLEFT && leftAxis )
			childDesc.scale = DrawDesc::DS_NONE;

		if( childDesc.scale == DrawDesc::DS_ONRIGHT && rightAxis )
			childDesc.scale = DrawDesc::DS_NONE;
	}
}

}