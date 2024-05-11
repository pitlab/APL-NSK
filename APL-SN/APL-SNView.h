
// APL-SNView.h: interfejs klasy CAPLSNView
//

#pragma once
#include "Protokol.h"


class CAPLSNView : public CView
{
protected: // utwórz tylko na podstawie serializacji
	CAPLSNView() noexcept;
	DECLARE_DYNCREATE(CAPLSNView)

// Atrybuty
public:
	CAPLSNDoc* GetDocument() const;

// Operacje
public:

// Przesłania
public:
	virtual void OnDraw(CDC* pDC);  // przesłonięty, aby narysować ten widok
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//CGniazdoSieci	  m_sGniazdoSluchajace;
	//CGniazdoSieci	  m_sGniazdoPolaczenia;
// Implementacja
public:
	virtual ~CAPLSNView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
	void OnPolacz();
	void OnAkceptuj();
#endif

protected:
	CProtokol m_cProtokol;
	BOOL m_bPolaczonoETH;
	BOOL m_bPolaczonoUART;

// Wygenerowano funkcje mapy komunikatów
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnRawInput(UINT nInputcode, HRAWINPUT hRawInput);
};

#ifndef _DEBUG  // debuguj wersję w elemencie APL-SNView.cpp
inline CAPLSNDoc* CAPLSNView::GetDocument() const
   { return reinterpret_cast<CAPLSNDoc*>(m_pDocument); }
#endif

