#pragma once

#include "DrawDesc.h"

namespace logger
{
class RecordSet;
}

namespace view2d
{

class LayoutTree : public CTreeCtrl
{
public:

	static const int NODEID_SEPARATE = 0;
	static const int NODEID_JOIN = 1;
	static const int NODEID_JOIN_EQSCALE = 2;
	static const int NODEID_XY = 3;

	static const TCHAR NODE_SEPARATE_TEXT[];
	static const TCHAR NODE_JOIN_TEXT[];
	static const TCHAR NODE_JOIN_EQSCALE_TEXT[];
	static const TCHAR NODE_XY_TEXT[];

	typedef std::map< int, DrawDesc > DrawDescMap;

	struct DragSourceElem
	{
		enum Type
		{
			DST_SPECIAL_NODE,
			DST_VARIABLE
		};

		Type type;
		int	 ident;
	};

	LayoutTree( const logger::RecordSet & _recordSet );
	virtual ~LayoutTree();

	bool isDragging() const { return m_bLDragging ? true : false; }

	void onBeginDrag( const std::vector< DragSourceElem > & _dragSrc );

	static bool isSpecialNode( int _nodeId );

	void fixSiblingsLeftRightAxis( HTREEITEM _hItem );

	std::pair< bool, bool > checkVerticalAxisSpace( 
		HTREEITEM _hItem, bool _stopOnCheckedItem = false ) const;

	DrawDescMap	m_drawDescriptors;
	int			m_idCount;

protected:

	DECLARE_MESSAGE_MAP()

	string_type getDragSourceElemText( const DragSourceElem & _elem ) const;


	afx_msg void OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnMouseMove(UINT nFlags, CPoint _point);
	afx_msg void OnCancelMode();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint _point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint _point);

	afx_msg void OnRButtonDown(UINT nFlags, CPoint _point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint _point);
	afx_msg void OnRClick(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnContextMenu(CWnd* pWnd, CPoint pos);

	afx_msg void OnMenuTreeSplit();
	afx_msg void OnMenuTreeJoin();
	afx_msg void OnMenuTreeJoinEqscale();
	afx_msg void OnMenuTreeXY();
	afx_msg void OnMenuChangeToSplit();
	afx_msg void OnMenuChangeToJoin();
	afx_msg void OnMenuChangeToJoinEqscale();

	afx_msg void OnMenuTreeDelete();

	void fixLeftRightAxis( HTREEITEM _hItem );

	bool canChangeToSplit( HTREEITEM _hItem ) const;
	bool canChangeToJoin( HTREEITEM _hItem ) const;
	bool canChangeToEqscale( HTREEITEM _hItem ) const;

	void extractSiblings( HTREEITEM _item, std::vector< HTREEITEM > & _output ) const;

	bool isSpecialNode( HTREEITEM _item ) const;
	bool isXYNode( HTREEITEM _item ) const;
	bool isJoinEqualScale( HTREEITEM _item ) const;

	CImageList* CreateDragImageEx( HTREEITEM hItem );
	CImageList* CreateTextDragImage( const string_type & _text );

	bool checkDrop1( HTREEITEM _hItemDrag, HTREEITEM _hItemDrop ) const;
	bool checkDrop2( HTREEITEM _hItemDrag, HTREEITEM _hItemDrop ) const;

	HTREEITEM copyBranch( HTREEITEM _hSrcSubtree, 
		HTREEITEM _hDstNode, HTREEITEM _hDstSubtreePos);

	void insertExtSource( HTREEITEM _hParent, HTREEITEM _hInsertAfter );
	void insertExtSourceToXY( HTREEITEM _hXYItem, const CPoint & _point );
	bool canInsertExtSource( HTREEITEM _hParent, HTREEITEM _hInsertAfter ) const;

	void copySubtree( HTREEITEM _dstSubtree, HTREEITEM _srcSubtree );

	bool canDrop( CPoint _point ) const;
	void onDragOver( CPoint _point );
	void onDragDrop( CPoint _point );
	void onDragCancel();

	const logger::RecordSet & m_recordSet;

	CImageList*	m_pDragImage;
	BOOL		m_bLDragging;
	BOOL		m_extDragging;
	HTREEITEM	m_hItemDrag;
	HTREEITEM	m_hItemDrop;
	HTREEITEM	m_hActiveItem;
	std::vector< DragSourceElem > m_extDragSrc;
};

}

