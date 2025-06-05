
// APL-SNView.h: interfejs klasy CAPLSNView
//

#pragma once
#include "komunikacja/Protokol.h"
#include "komunikacja/Komunikacja.h"
#include "CDaneFlash.h"
#include "Rejestr.h"
#include "CkonfigTelemetrii.h"
#include "KonfiguracjaWyresow.h"
#include "KonfigRejestratora.h"
#include <vector>

#define MIEJSCE_MIEDZY_WYKRESAMI	5
#define MIEJSCE_PRZED_WYKRESEM		30


class CAPLSNView : public CView

{
protected: // utwórz tylko na podstawie serializacji
	CAPLSNView() noexcept;
	DECLARE_DYNCREATE(CAPLSNView)

// Atrybuty
public:
	CAPLSNDoc* GetDocument() const;
	static uint8_t WatekRysujPasekPostepu(LPVOID pParam);
	static uint8_t WatekInvalidujWytkresTelemetrii(LPVOID pParam);
	uint8_t WlasciwyWatekRysujPasekPostepu();
	uint8_t WlasciwyWatekInvalidujWytkresTelemetrii();

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
	CWinThread* pWskWatkuPaskaPostepu;
	CWinThread* pWskWatkuOdswiezaniaTelemetrii;

// Implementacja
public:
	virtual ~CAPLSNView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//CKomunikacja m_cKomunikacja;
	CKonfigPolacz m_cKonfigPolacz;
	CDaneFlash m_cDaneFlash;
	CProtokol m_cProtokol;
	KonfiguracjaWyresow m_cKonfiguracjaWykresow;
	CKonfigTelemetrii m_cKonfigTelemetrii;
	KonfigRejestratora m_cKonfigRejestratora;

	BOOL m_bPolaczono;
	BOOL m_bKoniecWatkuPaskaPostepu;
	BOOL m_bKoniecWatkuOdswiezaniaTelemtrii;
	BOOL m_bRysujPasekPostepu;
	BOOL m_bRysujTelemetrie;
	BOOL m_bOknoGotowe;
	uint16_t m_sLiczbaFragmentowPaskaPostepu;
	uint16_t m_sBiezacyStanPaskaPostepu;
	uint8_t m_chAdresAutopilota;
	CObslugaRejestru m_cObslugaRejestru;
	CD2DTextFormat* m_pTextFormat;
	//CD2DBrush* m_pBrushLegendy;
	CD2DSolidColorBrush* m_pBrushBlack;
	CD2DSolidColorBrush* m_pBrushWykresuR;
	CD2DSolidColorBrush* m_pBrushWykresuG;
	CD2DSolidColorBrush* m_pBrushWykresuB;
	CD2DSolidColorBrush* m_pBrushOsiWykresu;
	CD2DLinearGradientBrush* m_pLinearGradientBrush;
	float m_fZoomPoziomo;
	float m_fZoomPionowo;
	UINT m_nVscroll;
	//UINT m_nHScroll;
	void UstawScrollOdWidoku();
	static const int SCROLL_UNIT = 1 << 24;
	int m_nIloscDanychWykresu;
	int m_nSzerokoscOkna;
	int m_nMaxScrollPoziomo;
	int m_nBiezacyScrollPoziomo;


public:
	uint32_t m_nNumerPortuCom;
	uint32_t m_nTypPolaczenia;	//0=UART, 1=ETH
	uint8_t m_chNumerIP[4] = {0,0,0,0};
	uint32_t m_nPredkoscPortuCom;
	uint32_t m_nNumerPortuEth;

	void RysujWykresTelemetrii(CRect okno, float fHscroll, float fVpos, float fSkalaX, float fSkalaY, std::vector<_Telemetria>vRamkaTele, int nIndeksZmiennej, CHwndRenderTarget* pRenderTarget, CD2DSolidColorBrush* pBrush, int* nStartLegendy);
	void RysujWykresLogu(CRect okno, float fHscroll, float fVpos, float fSkalaX, float fSkalaY, int nIndeksZmiennej, CHwndRenderTarget* pRenderTarget, CD2DSolidColorBrush* pBrush);
	void RysujOsieGrupyWykresow(CRect okno, int nZero, CHwndRenderTarget* pRenderTarget, CD2DSolidColorBrush* pBrush);

// Wygenerowano funkcje mapy komunikatów
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnRawInput(UINT nInputcode, HRAWINPUT hRawInput);
	afx_msg void OnKonfigPort();
	afx_msg void OnUpdateKonfigPort(CCmdUI* pCmdUI);
	afx_msg void OnPolaczCom();
	afx_msg void OnUpdatePolaczCom(CCmdUI* pCmdUI);
	afx_msg void OnZrobZdjecie();
	afx_msg void OnUpdateZrobZdjecie(CCmdUI* pCmdUI);
	afx_msg void OnZapiszPamiec();
	afx_msg void OnUpdateZapiszPamiec(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	afx_msg LRESULT OnDraw2d(WPARAM wParam, LPARAM lParam);
	
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnOleInsertNew();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnKonfigWykresow();
	afx_msg void OnUpdateKonfigWykresow(CCmdUI* pCmdUI);
	afx_msg void OnKonfigTelemetrii();
	afx_msg void OnUpdateKonfigTelemetrii(CCmdUI* pCmdUI);
	afx_msg void OnKonfigRejestratora();
	afx_msg void OnUpdateKonfigRejestratora(CCmdUI* pCmdUI);
};

#ifndef _DEBUG  // debuguj wersję w elemencie APL-SNView.cpp
inline CAPLSNDoc* CAPLSNView::GetDocument() const
   { return reinterpret_cast<CAPLSNDoc*>(m_pDocument); }
#endif

