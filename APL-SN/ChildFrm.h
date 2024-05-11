
// ChildFrm.h: interfejs klasy CChildFrame
//

#pragma once

class CChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame() noexcept;

// Atrybuty
protected:
	CSplitterWndEx m_wndSplitter;
public:

// Operacje
public:

// Przesłania
	public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementacja
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Wygenerowano funkcje mapy komunikatów
protected:
	DECLARE_MESSAGE_MAP()
};
