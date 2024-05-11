
#pragma once

/////////////////////////////////////////////////////////////////////////////
// Okno CViewTree

class CViewTree : public CTreeCtrl
{
// Konstrukcja
public:
	CViewTree() noexcept;

// Przesłania
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

// Implementacja
public:
	virtual ~CViewTree();

protected:
	DECLARE_MESSAGE_MAP()
};
